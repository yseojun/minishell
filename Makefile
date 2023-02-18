NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c

SRCS_BONUS = bonus.c

OBJS = $(addprefix philo/, $(SRCS:.c=.o))
OBJS_BONUS = $(addprefix philo_bonus/, $(SRCS_BONUS:.c=.o))

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

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