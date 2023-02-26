//
// tm_stack.c
//

#include <stdlib.h>
#include <string.h>
#include "tm_stack.h"

static int	stack_resize(tm_stack_t *stack);

tm_stack_t	*tm_stack_init(size_t capacity, size_t data_size)
{
	tm_stack_t	*stack;

	stack = calloc(1, sizeof(tm_stack_t));
	if (stack == NULL)
		return (NULL);
	if (data_size == 0)
		stack->nodes = calloc(capacity, sizeof(void *));
	else
		stack->nodes = calloc(capacity, data_size);
	if (stack->nodes == NULL)
	{
		free(stack);
		return (NULL);
	}
	stack->data_size = data_size;
	stack->capacity = capacity;
	return (stack);
}

void	tm_stack_cleanup(tm_stack_t *stack)
{
	size_t	index;
	void	*node;

	if (stack == NULL)
		return ;
	for (index = 0; index < stack->total; index++)
	{
		node = tm_stack_pop(stack);
		if (node == NULL)
			continue ;
		if (stack->cleanup_cb != NULL)
			stack->cleanup_cb(node);
		if (stack->data_size == 0)
			free(node);
	}
	free(stack->nodes);
	free(stack);
}

void	tm_stack_custom_cleanup(tm_stack_t *stack, void (*stack_cleanup_cb)(void *data))
{
	stack->cleanup_cb = stack_cleanup_cb;
}

void	tm_stack_clear(tm_stack_t *stack)
{
	size_t	index;
	void	*node;

	if (stack == NULL)
		return ;
	for (index = 0; index < stack->total; index++)
	{
		node = tm_stack_pop(stack);
		if (node == NULL)
			continue ;
		if (stack->cleanup_cb != NULL)
			stack->cleanup_cb(node);
		if (stack->data_size == 0)
			free(node);
	}
	stack->total = 0;
}

void	*tm_stack_push(tm_stack_t *stack, void *node)
{
	char	*buf;
	size_t	index;	

	index = stack->total;
	if (index == stack->capacity)
	{
		if (stack_resize(stack) != 0)
			return (NULL);
	}
	if (stack->data_size == 0)
	{
		stack->nodes[index] = node;
		stack->total++;
		return (stack->nodes[index]);
	}
	buf = (char *)stack->nodes;
	memcpy(&buf[stack->total * stack->data_size], node, stack->data_size);
	stack->total++;
	return (&buf[index * stack->data_size]);
}

static int	stack_resize(tm_stack_t *stack)
{
	size_t	index;
	size_t	new_capacity;
	void	**new;

	new_capacity = stack->capacity * 2;
	if (stack->data_size == 0)
		new = calloc(new_capacity, sizeof(void *));
	else
		new = calloc(new_capacity, stack->data_size);
	if (new == NULL)
		return (1);
	if (stack->data_size == 0)
		for (index = 0; index < stack->total; index++)
			new[index] = stack->nodes[index];
	else
		memcpy(new[0], stack->nodes[0], stack->total * stack->data_size);
	free(stack->nodes);
	stack->nodes = new;
	stack->capacity = new_capacity;
	return (0);
}

void	*tm_stack_pop(tm_stack_t *stack)
{
	char	*buf;

	if (stack->total == 0)
		return (NULL);
	stack->total--;
	buf = (char *)stack->nodes;
	return (&buf[stack->total * stack->data_size]);
}
