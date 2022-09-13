//
// tm_stack.h
//

#ifndef TM_STACK_H
#define TM_STACK_H

#include <stdlib.h>

typedef void	(*tm_stack_cleanup_cb_t)(void *data);

typedef struct tm_stack_s
{
	size_t					data_size;
	size_t					capacity;
	size_t					total;
	tm_stack_cleanup_cb_t	cleanup_cb;
	void					**nodes;
}				tm_stack_t;

tm_stack_t	*tm_stack_init(size_t capacity, size_t data_size);
void	tm_stack_cleanup(tm_stack_t *stack);
void	tm_stack_custom_cleanup(tm_stack_t *stack, void (*cleanup_cb)(void *data));
void	tm_stack_clear(tm_stack_t *stack);
void	*tm_stack_push(tm_stack_t *stack, void *node);
void	*tm_stack_pop(tm_stack_t *stack);

#endif
