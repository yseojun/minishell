NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c
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
	$(CC) $(CFLAGS) -o $(<:.c=.o) -c $<

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME)

re :
	make fclean
	make all

.PHONY: clean fclean re