NAME := libtm.a
SRCS_LIST := tm_list.c
SRCS_VECTOR := tm_vector.c
SRCS_STACK := tm_stack.c
OBJS_LIST := $(SRCS_LIST:.c=.o)
OBJS_VECTOR := $(SRCS_VECTOR:.c=.o)
OBJS_STACK := $(SRCS_STACK:.c=.o)
CC ?= cc
LD ?= cc
CFLAGS = -Wall -Wextra -Werror

.PHONY: all list vector stack debug clean fclean re

all: $(NAME) list vector stack

list: $(NAME) $(OBJS_LIST)
	ar r $^

vector: $(NAME) $(OBJS_VECTOR)
	ar r $^

stack: $(NAME) $(OBJS_STACK)
	ar r $^

debug: CFLAGS += -g3
debug: re

clean:
	rm -f $(OBJS_LIST) $(OBJS_VECTOR) $(OBJS_STACK)

fclean: clean
	rm -f $(NAME)

re: fclean all

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
