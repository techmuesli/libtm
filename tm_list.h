//
// tm_list.h
//

#ifndef TM_LIST_H
#define TM_LIST_H

#include <stdlib.h>
#include "tm_stack.h"
#include "tm_vector.h"

typedef void	(*tm_list_cleanup_cb_t)(void *data);

typedef struct node_s
{
	void			*data;
	struct node_s	*next;
	struct node_s	*prev;
}				node_t;

typedef struct tm_list_s
{
	size_t					capacity;
	size_t					data_size;
	size_t					grow_count;
	size_t					total;
	tm_list_cleanup_cb_t	cleanup_cb;
	node_t					*current;
	node_t					*first;
	node_t					*last;
	tm_stack_t				*free_nodes;
	tm_vector_t				*data_allocs;
	void					**nodes;
}				tm_list_t;

tm_list_t	*tm_list_init(size_t capacity, size_t grow_count, size_t size);
void	tm_list_cleanup(tm_list_t *list);
void	tm_list_custom_cleanup(tm_list_t *list, void (*cleanup_cb)(void *data));
void	*tm_list_add(tm_list_t *list, void *data);
void	*tm_list_add_front(tm_list_t *list, void *data);
//void	*tm_list_add_before(tm_list_t *list, void *data);
void	tm_list_delete(tm_list_t *list);
void	*tm_list_get(tm_list_t *list);
void	*tm_list_get_next(tm_list_t *list);
void	*tm_list_get_prev(tm_list_t *list);
void	*tm_list_get_first(tm_list_t *list);
void	*tm_list_get_last(tm_list_t *list);
size_t	tm_list_get_total_number(tm_list_t *list);

#endif
