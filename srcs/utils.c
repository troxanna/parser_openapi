#include "../includes/parser.h"

void	free_item(t_method *item)
{
	if (item)
	{
		if (item->path)
			free(item->path);
		if (item->type)
			free(item->type);
		free(item);
	}
}

void	free_memory(t_method **methods)
{
	t_method	*new_item;
	t_method	*pl;

	new_item = *methods;
	while (new_item)
	{
		pl = new_item->next;
		free_item(new_item);
		new_item = pl;
	}
	*methods = NULL;
}

int is_allow_symbol(char ch) {
	if (isalpha(ch) || ch == '/' || ch == '-') {
		return 1;
	}
	return 0;
}

void print_method(char *path, int status, char *type, FILE *fp) {
	fputs("|", fp);
	// fputs(BLACK, fp);
	fputs(type, fp);
	fputs(path, fp);
	fputs("|", fp);
	if (status == COMPLETED) {	
		// fputs(GREEN, fp);
		fputs("<span style=\"color:green\">COMPLETED</span>.", fp);
	} else if (status == INPROGRESS) {
		// fputs(YELLOW, fp);
		fputs("INPROGRESS", fp);
	} else if (status == DEPRECATED) {
		// fputs(RED, fp);
		fputs("DEPRECATED", fp);
	} else {
		return ;
	}
	// fputs(BLACK, fp);
	fputs("|\n", fp);
}

void clear_not_allow_symbols(char *input, char *output) {
	int count = 0;

	size_t i = 0;
	size_t j = 0;

	for (size_t i = 0; i < strlen(input); i++) {
		if (!is_allow_symbol(input[i])) {
			count++;
		}
	}

	while (j < strlen(input) - count) {
		if (is_allow_symbol(input[i])) {
			output[j] = input[i];
			j++;
		} 
		i++;
	}
}

int search_item(char *query, char *input) {
   	char *str;

   	str = strstr (input, query);

	if (str == NULL)
      return 0;
   	else
      return 1;
}

void clear_line(char *line, size_t count) {
	for (size_t i = 0; i < count; i++) {
		line[i] = '\0';
	}
}


void	write_item(char *type, t_method *method, int status, char *line)
{
	method->status = status;

	if (line != NULL) {
		method->path = strdup(line);
	}
	if (type != NULL) {	
		method->type = strdup(type);
	}
}

t_method 	*new_item(void)
{
	t_method	*new_method;

	new_method = (t_method *)malloc(sizeof(t_method));
	if (!new_method)
		exit(EXIT_FAILURE);
	new_method->path = NULL;
	new_method->type = NULL;
	new_method->status = NONE;
	new_method->next = NULL;
	return (new_method);
}

void	add_item(t_method **methods, t_method *new_method,
			void (*wrt)(char *type, t_method *method, int status, char *line), char *line, char *type, int status)
{
	t_method	*ptr;

	ptr = *methods;
	wrt(type, new_method, status, line);

	if (methods && new_method)
	{
		if (*methods == NULL)
			*methods = new_method;
		else
		{
			while (ptr->next)
				ptr = ptr->next;
			ptr->next = new_method;
		}
	}
}

t_method *get_last_item(t_method **methods) {
	t_method	*ptr;

	ptr = *methods;
	if (methods) {
		while (ptr->next)
			ptr = ptr->next;
		return ptr;
	} 
	return ptr;
}


void init_data(t_flags *data) {
	data->close_bracket = 0;
	data->is_operation_id = 0;
	data->open_bracket = 0;
	data ->path_is_active = 0;
}
