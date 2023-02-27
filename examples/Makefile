#
# Makefile
#

CC ?= cc
CXX ?= c++
LD ?= cc
CFLAGS ?= -Wall -Wextra -Werror -pedantic
CXXFLAGS ?= -Wall -Wextra -Werror -pedantic

INC_DIR := ./../include
LIBTM := ./../libtm.a

LIST := list
STACK := stack
VECTOR := vector
CPP := cpp

.PHONY: all clean

all: $(LIST) $(STACK) $(VECTOR) $(CPP)

$(LIST):
	$(CC) $(CFLAGS) -I $(INC_DIR) list.c $(LIBTM) -o $@

$(STACK):
	$(CC) $(CFLAGS) -I $(INC_DIR) stack.c $(LIBTM) -o $@

$(VECTOR):
	$(CC) $(CFLAGS) -I $(INC_DIR) vector.c  $(LIBTM) -o $@

$(CPP):
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) vector.cpp $(LIBTM) -o $@

clean:
	rm -f $(LIST) $(STACK) $(VECTOR) $(CPP)
