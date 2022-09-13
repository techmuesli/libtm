//
// tm_list.c
//

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tm_list.h"
#include "../include/tm_stack.h"

static void			**list_init_nodes(tm_list_t *list, size_t count, size_t size);
static void			set_up_nodes(void **nodes, char *buf, size_t count, size_t size);
static tm_stack_t	*list_init_stack(void **nodes, size_t count);
static int			list_resize(tm_list_t *list);
static int			list_add_new_nodes(tm_list_t *list, void **nodes);

tm_list_t	*tm_list_init(size_t capacity, size_t grow_count, size_t size)
{
	tm_list_t	*list;

	if (capacity == 0)
		return (NULL);
	list = calloc(1, sizeof(tm_list_t));
	if (list == NULL)
		return (NULL);
	list->nodes = list_init_nodes(list, capacity, size);
	if (list->nodes == NULL)
	{
		free(list);
		return (NULL);
	}
	list->free_nodes = list_init_stack(list->nodes, capacity);
	if (list->free_nodes == NULL)
	{
		tm_vector_cleanup(list->data_allocs);
		free(list->nodes);
		free(list);
		return (NULL);
	}
	list->capacity = capacity;
	list->grow_count = grow_count;
	list->data_size = size;
	return (list);
}

static void	**list_init_nodes(tm_list_t *list, size_t count, size_t size)
{
	char	*buf;
	void	**nodes;

	list->data_allocs = tm_vector_init(10, 10, 0);
	if (list->data_allocs == NULL)
		return (NULL);
	nodes = malloc(count * sizeof(void *));
	if (nodes == NULL)
	{
		tm_vector_cleanup(list->data_allocs);
		return (NULL);
	}
	buf = calloc(count, sizeof(node_t) + size);
	if (buf == NULL)
	{
		tm_vector_cleanup(list->data_allocs);
		free(nodes);
		return (NULL);
	}
	tm_vector_add(list->data_allocs, buf);
	set_up_nodes(nodes, buf, count, size);
	return (nodes);
}

static void	set_up_nodes(void **nodes, char *buf, size_t count, size_t size)
{
	size_t	index;
	node_t	*node;

	index = 0;
	while (index < count)
	{
		nodes[index] = &buf[index * (sizeof(node_t) + size)];
		if (size != 0)
		{
			node = nodes[index];
			node->data = &buf[index * (sizeof(node_t) + size)]
				+ sizeof(node_t);
		}
		index++;
	}
}

static tm_stack_t	*list_init_stack(void **nodes, size_t count)
{
	size_t		index;
	tm_stack_t	*stack;

	stack = tm_stack_init(count, 0);
	if (stack == NULL)
		return (NULL);
	index = count - 1;
	while (index > 0)
	{
		tm_stack_push(stack, nodes[index]);
		index--;
	}
	tm_stack_push(stack, nodes[index]);
	return (stack);
}

// callback should free everything, but the data pointer itself
// !!!!! stack_cleanup() is not called !!!!! check to call it
void	tm_list_cleanup(tm_list_t *list)
{
	node_t	*node;

	if (list == NULL)
		return ;
	node = list->first;
	while (node != NULL)
	{
		if (list->cleanup_cb != NULL)
			list->cleanup_cb(node->data);
		if (node->data != NULL && list->data_size == 0)
			free(node->data);
		node = node->next;
	}
	tm_vector_cleanup(list->data_allocs);
	free(list->nodes);
	free(list->free_nodes->nodes);
	free(list->free_nodes);
	free(list);
	return ;
}

void	tm_list_custom_cleanup(tm_list_t *list, void (*cleanup_cb)(void *data))
{
	list->cleanup_cb = cleanup_cb;
}

/* rename to list_add_back ? !!!!! */
void	*tm_list_add(tm_list_t *list, void *data)
{
	node_t	*node;

	if (list->total == list->capacity)
	{
		if (list_resize(list) != 0)
			return (NULL);
	}
	node = tm_stack_pop(list->free_nodes);
	if (node == NULL)
		return (NULL);
	if (list->total == 0)
		list->first = node;
	if (list->data_size == 0)
		node->data = data;
	else
		memcpy(node->data, data, list->data_size);
	if (list->last != NULL)
		list->last->next = node;
	else
		node->next = NULL;
	node->prev = list->last;
	list->total++;
	list->current = node;
	list->last = node;
	return (node);
}

//void	*list_add_before(tm_list_t *list, void *data);

/* if possible -> change to list_add_before */
void	*tm_list_add_front(tm_list_t *list, void *data)
{
	node_t	*node;

	if (list->total == list->capacity)
	{
		if (list_resize(list) != 0)
			return (NULL);
	}
	node = tm_stack_pop(list->free_nodes);
	if (node == NULL)
		return (NULL);
	node->next = list->first;
	if (list->first != NULL)
		list->first->prev = node;
	list->first = node;
	if (list->data_size == 0)
		node->data = data;
	else
		memcpy(node->data, data, list->data_size);
	node->prev = NULL;
	list->total++;
	list->current = node;
	if (list->total == 1)
		list->last = node;
	return (node);
}

static int	list_resize(tm_list_t *list)
{
	size_t	index;
	size_t	new_capacity;
	void	**new;

	new_capacity = list->capacity + list->grow_count;
	new = malloc(new_capacity * sizeof(void *));
	if (new == NULL)
		return (1);
	index = 0;
	while (index < list->capacity)
	{
		new[index] = list->nodes[index];
		index++;
	}
	if (list_add_new_nodes(list, new) != 0)
	{
		free(new);
		return (1);
	}
	free(list->nodes);
	list->nodes = new;
	list->capacity = new_capacity;
	return (0);
}

/* possbile optimization for while() */
static int	list_add_new_nodes(tm_list_t *list, void **nodes)
{
	char	*buf;
	size_t	index;
	size_t	pos;
	node_t	*node;

	buf = calloc(list->grow_count, sizeof(node_t) + list->data_size);
	if (buf == NULL)
		return (1);
	tm_vector_add(list->data_allocs, buf);
	pos = 0;
	index = list->capacity;
	while (index < list->capacity + list->grow_count)
	{
		nodes[index] = &buf[pos * (sizeof(node_t) + list->data_size)];
		if (list->data_size != 0)
		{
			node = nodes[index];
			node->data = &buf[pos * (sizeof(node_t) + list->data_size)]
				+ sizeof(node_t);
		}
		tm_stack_push(list->free_nodes, nodes[index]);
		pos++;
		index++;
	}
	return (0);
}

/* !!!!! discuss how current should be handled on delete */
void	tm_list_delete(tm_list_t *list)
{
	node_t	*node;

	if (list->current == NULL)
		return ;
	node = list->current;
	if (node->prev != NULL)
		node->prev->next = node->next;
	if (node->next != NULL)
		node->next->prev = node->prev;
	if (list->total == 1)
		list->first = NULL;
	if (list->first == node)
		list->first = node->next;
	if (list->last == node)
		list->last = node->prev;
	list->current = node->next;
	if (list->cleanup_cb != NULL)
		list->cleanup_cb(node->data);
	if (node->data != NULL && list->data_size == 0)
		free(node->data);
	list->total--;
	node->next = NULL;
	node->prev = NULL;
	tm_stack_push(list->free_nodes, node);
}

void	*tm_list_get(tm_list_t *list)
{
	if (list->current == NULL)
		return (NULL);
	return (list->current->data);
}

void	*tm_list_get_next(tm_list_t *list)
{
	node_t	*node;

	node = list->current;
	if (node == NULL)
		return (NULL);
	if (node->next == NULL)
		return (NULL);
	list->current = node->next;
	return (node->next->data);
}

void	*tm_list_get_prev(tm_list_t *list)
{
	node_t	*node;

	node = list->current;
	if (node == NULL)
		return (NULL);
	if (node->prev == NULL)
		return (NULL);
	list->current = node->prev;
	return (node->prev->data);
}

void	*tm_list_get_first(tm_list_t *list)
{
	if (list->first == NULL)
		return (NULL);
	list->current = list->first;
	return (list->first->data);
}

void	*tm_list_get_last(tm_list_t *list)
{
	if (list->last == NULL)
		return (NULL);
	list->current = list->last;
	return (list->last->data);
}

size_t	tm_list_get_total_number(tm_list_t *list)
{
	return (list->total);
}
