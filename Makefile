NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
RLIB = -L $(HOME)/.brew/opt/readline/lib -lreadline
RLIBI = -I $(HOME)/.brew/opt/readline/include
SRCS = main.c \
parse.c \
base_data.c \
list_env.c \
list_wildcard.c \
pipe_run_unit.c \
pipe_set_pipe.c \
pipe_file.c \
pipe_util.c \
pipe_pid_util.c \
builtin_func.c \
transform.c \
transform_expand.c \
transform_rm_quote.c \
transform_util.c \
wildcard_make.c \
wildcard_merge.c \
tokenize.c \
list_token.c \
_util.c \
is_symbol.c \
util_arr.c \
bonus_tokenize.c
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