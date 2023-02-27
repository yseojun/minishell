NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
RLIB = -L $(HOME)/.brew/opt/readline/lib -lreadline
RLIBI = -I $(HOME)/.brew/opt/readline/include
SRCS = main.c \
parse.c \
base_data.c \
list_env.c \
pipe_run_pipe.c \
pipe_set_pipe.c \
pipe_file.c \
pipe_util.c \
built_in_func.c \
transform.c \
transform_expand.c \
transform_rm_quote.c \
transform_util.c \
tokenalize.c \
_util.c \
is_symbol.c \
util_arr.c
SRCS_BONUS = bonus.c
LIBT = libft 
GNL = gnl/get_next_line_bonus.c gnl/get_next_line_utils_bonus.c
OBJS = $(addprefix srcs/, $(SRCS:.c=.o)) $(GNL:.c=.o)
OBJS_BONUS = $(addprefix bonus/, $(SRCS_BONUS:.c=.o))

all : $(NAME)

$(NAME) : $(OBJS)
	make all -C $(LIBT)
	$(CC) $(CFLAGS) -o $@ $^ -L$(LIBT) -lft

bonus : $(OBJS_BONUS)
	$(CC) $(CFLAGS) -o $(NAME) $^

%.o: %.c
	$(CC) $(CFLAGS) -o $(<:.c=.o) -c $< $(RLIBI)

test : $(addprefix srcs/, $(SRCS)) $(GNL)
	make all -C $(LIBT)
	$(CC) $(CFLAGS) -g -o $(NAME) $^ -L$(LIBT) -lft $(RLIB) $(RLIBI)

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