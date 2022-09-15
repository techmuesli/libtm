#
# Makefile
#

NAME ?= libtm.a

CC ?= cc
LD ?= cc
CFLAGS ?= -Wall -Wextra -Werror -pedantic

BUILD_DIR := ./build
EXAMPLES_DIR := ./examples
INC_DIR := ./include
SRC_DIR := ./src

SRCS := tm_list.c tm_stack.c tm_vector.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

$(NAME): $(OBJS)
	ar r $(NAME) $^

.PHONY: all examples debug clean re

all: $(NAME) examples

examples: $(NAME)
	$(MAKE) -C $(EXAMPLES_DIR)

debug: CFLAGS += -g3
debug: re

clean:
	$(MAKE) clean -C $(EXAMPLES_DIR)
	rm -f $(OBJS)
	rm -rf $(BUILD_DIR)
	rm -f $(NAME)

re: clean $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@
