#include "../includes/parser.h"


void handler_openapi(FILE * fp_openapi, t_flags *flags, t_method **methods) {
	ssize_t read;
    char * line = NULL;
    size_t len = 0;
	size_t i = 0;

	char *tmp_line = NULL;

	tmp_line = (char *)malloc(sizeof(char) * SIZE);
	clear_line(tmp_line, SIZE);

	while ((read = getline(&line, &len, fp_openapi)) != -1) {
		if (search_item(COMPONENTS, line)) {
			break ;
		}
		if (flags->path_is_active == 0 && search_item(PATHS, line)) {
			flags->path_is_active = 1;
			continue;
		}
		if (flags->path_is_active) {
			if (flags->open_bracket == 0) {
				clear_line(tmp_line, SIZE);
				clear_not_allow_symbols(line, tmp_line);
			}
			if (search_item(OPERATION_ID, line)) {
				flags->is_operation_id = 1;
				get_last_item(methods)->status = COMPLETED;
			}
			if (search_item(GET, line) || search_item(POST, line)) {
				if (search_item(GET, line)) {
					add_item(methods, new_item(), write_item, tmp_line, "GET", INPROGRESS);
					i++;
				} else if (search_item(POST, line)) {
					add_item(methods, new_item(), write_item, tmp_line, "POST", INPROGRESS);
					i++;
				}
			}
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
}

void write_progress(FILE	*fp) {
	fputs("| Syntax      | Description |\n", fp);
	fputs("| :---        | 		  ---: |\n", fp);
	
}

int main(int argc, char *argv[])
{
    FILE	*fp_openapi;
	FILE	*output_file;

	t_method *method = NULL;
	t_method *ptr = NULL;
	t_flags flags;

	if (argc != 2) {
		printf("%s", "Error count arguments");
		exit(EXIT_FAILURE);
	}

    fp_openapi = fopen(argv[1], "r");
    if (fp_openapi == NULL)
        exit(EXIT_FAILURE);

	//malloc memory and init data
	init_data(&flags);	
	add_item(&method, new_item(), write_item, NULL, NULL, NONE);
	ptr = method;

	handler_openapi(fp_openapi, &flags, &method);

	output_file = fopen("progress.md", "w");

	ptr = method;
	write_progress(output_file);
	while (ptr != NULL) {
		print_method(ptr->path, ptr->status, ptr->type, output_file);
		ptr = ptr->next;
	}

    fclose(fp_openapi);
	fclose(output_file);
	free_memory(&method);
    exit(EXIT_SUCCESS);
}
