NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
RLIB = -L $(HOME)/.brew/opt/readline/lib -lreadline
RLIBI = -I $(HOME)/.brew/opt/readline/include
SRCS = main.c \
env_to_char.c \
set_beginning.c \
finish_line.c \
_util.c \
list_env.c \
list_wildcard.c \
list_token.c \
list_pipefd.c \
is_symbol.c \
parse_line.c \
chk_condition.c \
tokenize.c \
transform.c \
transform_expand.c \
transform_rm_quote.c \
transform_quote_util.c \
wildcard.c \
wildcard_make.c \
wildcard_merge.c \
wildcard_tofind.c \
tree_make1.c \
tree_make2.c \
tree_make3.c \
heredoc.c \
pipe_set.c \
pipe_check_cmd.c \
pipe_execute_tree.c \
pipe_execute_tree2.c \
pipe_run.c \
pipe_util.c \
builtin_func1.c \
builtin_func2.c \
builtin_func3.c
SRCS_BONUS = $(addprefix bonus_, $(SRCS:.c=.o))
LIBT = libft 
OBJS = $(addprefix srcs/, $(SRCS:.c=.o))
OBJS_BONUS = $(addprefix bonus/, $(SRCS_BONUS:.c=.o))

all : $(NAME)

$(NAME) : $(OBJS)
	make all -C $(LIBT)
	$(CC) $(CFLAGS) -o $@ $^ -L$(LIBT) -lft $(RLIB) $(RLIBI)

bonus : $(OBJS_BONUS)
	$(CC) $(CFLAGS) -o $(NAME) $^

%.o: %.c
	$(CC) $(CFLAGS) -o $(<:.c=.o) -c $< $(RLIBI)

clean:
	make clean -C $(LIBT)
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	make fclean -C $(LIBT)
	rm -f $(NAME)

re :
	make fclean
	make all

.PHONY: clean fclean re