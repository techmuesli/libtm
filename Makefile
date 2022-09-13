#
# Makefile
#

NAME ?= libtm.a

CC ?= cc
LD ?= cc
CFLAGS ?= -Wall -Wextra -Werror -pedantic

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := tm_list.c tm_stack.c tm_vector.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

$(NAME): $(OBJS)
	ar r $(NAME) $^

.PHONY: debug clean fclean re

debug: CFLAGS += -g3
debug: re

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(BUILD_DIR)
	rm -f $(NAME)

re: fclean $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
