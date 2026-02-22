# **************************************************************************** #
#                                  VARIABLES                                   #
# **************************************************************************** #

NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
DEBUG_FLAGS = -g -fsanitize=thread

SRCS = main.c init.c utiles.c actions.c monitor.c
OBJS = $(SRCS:.c=.o)

GREEN = \033[0;32m
RED = \033[0;31m
NC = \033[0m

# **************************************************************************** #
#                                  RULES                                       #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)✓ $(NAME) compiled successfully$(NC)"

debug: CFLAGS += $(DEBUG_FLAGS)
debug: re

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)✓ Compiled: $<$(NC)"

clean:
	@rm -f $(OBJS)
	@echo "$(RED)✗ Object files removed$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)✗ $(NAME) removed$(NC)"

re: fclean all

test: all
	@echo "$(GREEN)Running basic tests...$(NC)"
	@echo "Note: Use 'make test1', 'make test2' etc. to run specific tests"

test1: all
	@echo "$(GREEN)Test 1: 4 philosophers, 410ms die time (should not die)$(NC)"
	@./$(NAME) 4 410 200 200 || true

test2: all
	@echo "$(GREEN)Test 2: 4 philosophers, 310ms die time (should die)$(NC)"
	@./$(NAME) 4 310 200 200 || true

test3: all
	@echo "$(GREEN)Test 3: 5 philosophers with meal count (should stop after 7 meals)$(NC)"
	@./$(NAME) 5 610 200 200 7 || true

test4: all
	@echo "$(GREEN)Test 4: 1 philosopher (should die)$(NC)"
	@./$(NAME) 1 800 200 200 || true

test5: all
	@echo "$(GREEN)Test 5: 200 philosophers stress test$(NC)"
	@./$(NAME) 200 410 200 200 || true

valgrind: all
	@echo "$(GREEN)Running with valgrind...$(NC)"
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 4 410 200 200 5

.PHONY: all clean fclean re debug test test1 test2 test3 test4 test5 valgrind
