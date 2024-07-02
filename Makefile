DIR_INC = ./includes/

DIR_SRCS = ./srcs/

FUNC = 	utils.c parser.c

SRCS = $(addprefix $(DIR_SRCS), $(FUNC))

OBJS = $(SRCS:.c=.o)

NAME = parser_openapi

GCC = gcc
FLAGS = -Wall -Wextra -Werror

%.o: %.c $(DIR_INC)parser.h
	$(GCC) $(FLAGS) -I $(DIR_INC) -c $< -o $@

$(NAME): $(OBJS)
	$(GCC) $(FLAGS) -I $(DIR_INC) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all