#include "../includes/parser.h"


void handler_openapi(FILE * fp_openapi, t_flags *flags, t_method **methods) {
ssize_t read;
    char * line = NULL;
    size_t len = 0;
	char *tmp_line = NULL;

	//malloc memory for tmp line
	tmp_line = (char *)malloc(sizeof(char) * SIZE);
	clear_line(tmp_line, SIZE);

	while ((read = getline(&line, &len, fp_openapi)) != -1) {
		//Check "components" object in openapi file. If he is, then parsing ends ("components" comes after "paths").
		if (search_item(COMPONENTS, line)) {
			break ;
		}
		//Check "paths" object in openapi file. If he is, then parsing starts (all methods are inside "paths" object).
		if (flags->path_is_active == 0 && search_item(PATHS, line)) {
			flags->path_is_active = 1;
			continue;
		}
		if (flags->path_is_active) {
			//Execute clear line for further checks
			if (flags->open_bracket == 0) {
				clear_line(tmp_line, SIZE);
				clear_not_allow_symbols(line, tmp_line);
			}
			//Check "operationId" object in openapi file. If he is, then method is updated (status is completed).
			if (search_item(OPERATION_ID, line)) {
				flags->is_operation_id = 1;
				get_last_item(methods)->status = COMPLETED;
			}
			//Check "GET" or "POST" object in openapi file. If he is, then adding a new method to the tree.
			if (search_item(GET, line) || search_item(POST, line)) {
				if (search_item(GET, line)) {
					add_item(methods, new_item(), write_item, tmp_line, "GET", INPROGRESS);
				} else if (search_item(POST, line)) {
					add_item(methods, new_item(), write_item, tmp_line, "POST", INPROGRESS);
				}
			}
			//Check "{" и "}" items in openapi file for parsing service
			if (search_item(OPEN_BRACKET, line)) {
				flags->open_bracket++;
			}
			if (search_item(CLOSE_BRACKET, line)) {
				flags->open_bracket--;
			}
		}
    }
	if (tmp_line)
		free(tmp_line);
	if (line)
		free(line);
}

void write_progress(FILE	*fp, t_method **methods, t_counter *counter) {
	t_method *ptr = NULL;
	
	ptr = *methods;

	//calculate total progress for methods
	calculate_total_count(methods, counter);

	//write total progress methods in progress file
	print_total_progress(fp, counter);

	fputs("| Method      | Status |\n", fp);
	fputs("|  :---     |  :----:  |\n", fp);

	//write method and his status in progress file
	while (ptr != NULL) {
		if (ptr->path != NULL)
			print_method(ptr->path, ptr->status, ptr->type, fp);
		ptr = ptr->next;
	}	
}

int main(int argc, char *argv[])
{
    FILE	*fp_openapi = NULL;
	FILE	*output_file = NULL;

	t_method *method = NULL;
	t_flags flags;
	t_counter counter;

	validate_args(argc, argv);

	//try open openapi file
    if (!try_open_file(argv[1], "r", &fp_openapi)) 
        exit(EXIT_FAILURE);

	//malloc memory and init data
	init_data(&flags, &counter);	
	add_item(&method, new_item(), write_item, NULL, NULL, NONE);

	//parse openapi file and make tree methods
	handler_openapi(fp_openapi, &flags, &method);

	//try create progress file
    if (!try_open_file("progress.md", "w", &output_file)) {
		free_memory(&method);
		exit(EXIT_FAILURE);
	}

	// write info about progress of methods in file
	write_progress(output_file, &method, &counter);

	// ending of program execution (free memory and close file)
    fclose(fp_openapi);
	fclose(output_file);
	free_memory(&method);

    exit(EXIT_SUCCESS);
}
