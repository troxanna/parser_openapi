#ifndef PARSER_H
# define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1024

//Tokens
#define PATHS "\"paths\":"
#define COMPONENTS "\"components\":"
#define GET "\"get\":"
#define POST "\"post\":"
#define OPERATION_ID "\"operationId\":"
#define CLOSE_BRACKET "}"
#define OPEN_BRACKET "{"

//Colors
#define RED "\033[31m"
#define BLACK "\033[39m"
#define GREEN "\033[92m"
#define YELLOW "\033[33m"

typedef struct s_flags {
	int path_is_active;
	int open_bracket;
	int close_bracket;
	int is_operation_id;
}				t_flags;


typedef struct s_method
{
	int status;
	char *path;
	char *type;
	struct s_method	*next;
}				t_method;

typedef enum status {
	NONE,
	INPROGRESS,
	COMPLETED,
	DEPRECATED
}           e_status;


int			is_allow_symbol(char ch);
void		print_method(char *path, int status, char *type, FILE *fp);
void		free_item(t_method *item);
void		free_memory(t_method **methods);
void		clear_not_allow_symbols(char *input, char *output);
int			search_item(char *query, char *input);
void		clear_line(char *line, size_t count);
void		write_item(char *type, t_method *method, int status, char *line);
t_method 	*new_item(void);
void		add_item(t_method **methods, t_method *new_method,
			void (*wrt)(char *type, t_method *method, int status, char *line), char *line, char *type, int status);
t_method	*get_last_item(t_method **methods);
void		init_data(t_flags *data);
void		handler_openapi(FILE * fp_openapi, t_flags *flags, t_method **methods);





#endif