# **************************************************************************** #
#                          philo_bonus/Makefile                                #
# **************************************************************************** #

NAME = philo_bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -ggdb

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

GREEN = \033[0;32m
RED = \033[0;31m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)✓ $(NAME) compiled in bonus directory$(NC)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@echo "$(RED)✗ Object files removed from bonus directory$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)✗ $(NAME) removed from bonus directory$(NC)"

re: fclean all

.PHONY: all clean fclean re