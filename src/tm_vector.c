//
// tm_vector.c
//

#include <stdlib.h>
#include <string.h>
#include "../include/tm_vector.h"

static void	*vector_resize(tm_vector_t *vector);
static void	copy_data(tm_vector_t *vector, void **new);

tm_vector_t	*tm_vector_init(size_t capacity, size_t grow_count, size_t size)
{
	tm_vector_t	*vec;

	vec = calloc(1, sizeof(tm_vector_t));
	if (vec == NULL)
		return (NULL);
	vec->capacity = capacity;
	vec->grow_count = grow_count;
	vec->data_size = size;
	if (size == 0)
		vec->nodes = malloc(capacity * sizeof(void *));
	else
		vec->nodes = malloc(capacity * size);
	if (vec->nodes == NULL)
	{
		free(vec);
		return (NULL);
	}
	return (vec);
}

void	tm_vector_cleanup(tm_vector_t *vector)
{
	size_t	i;

	if (vector == NULL)
		return ;
	i = 0;
	while (i < vector->total)
	{
		if (vector->cleanup_cb != NULL)
			vector->cleanup_cb(tm_vector_get(vector, i));
		if (vector->data_size == 0)
			free(vector->nodes[i]);
		i++;
	}
	free(vector->nodes);
	free(vector);
}

void	tm_vector_custom_cleanup(tm_vector_t *vector, void (*vector_cleanup_cb)(void *data))
{
	vector->cleanup_cb = vector_cleanup_cb;
}

void	tm_vector_clear(tm_vector_t *vector)
{
	size_t	i;

	if (vector == NULL)
		return ;
	i = 0;
	while (i < vector->total)
	{
		if (vector->cleanup_cb != NULL)
			vector->cleanup_cb(tm_vector_get(vector, i));
		if (vector->data_size == 0)
			free(vector->nodes[i]);
		i++;
	}
	vector->total = 0;
}

void	*tm_vector_add(tm_vector_t *vector, void *data)
{
	char	*buf;

	if (vector->total == vector->capacity)
	{
		if (vector_resize(vector) == NULL)
			return (NULL);
	}
	if (vector->data_size == 0)
	{
		vector->nodes[vector->total] = data;
		vector->total++;
		return (vector->nodes[vector->total - 1]);
	}
	buf = (char *)vector->nodes;
	memcpy(&buf[vector->total * vector->data_size], data, vector->data_size);
	vector->total++;
	return (&buf[(vector->total - 1) * vector->data_size]);
}

void	*tm_vector_add_index(tm_vector_t *vector, void *data, int index)
{
	char	*buf;

	if (vector->total == vector->capacity)
	{
		if (vector_resize(vector) == NULL)
			return (NULL);
	}
	buf = (char *)vector->nodes;
	if (vector->data_size == 0)
	{
		memmove(&buf[index + 1], &buf[index], (vector->total - index) * sizeof(void *));
		vector->nodes[index] = data;
		vector->total++;
		return (vector->nodes[index]);
	}
	memmove(&buf[(index + 1) * vector->data_size], &buf[index * vector->data_size],
		(vector->total - index) * vector->data_size);
	memcpy(&buf[index * vector->data_size], data, vector->data_size);
	vector->total++;
	return (&buf[index * vector->data_size]);
}

static void	*vector_resize(tm_vector_t *vector)
{
	size_t	new_capacity;
	void	**new;

	new_capacity = vector->capacity + vector->grow_count;
	if (vector->data_size == 0)
		new = malloc(new_capacity * sizeof(void *));
	else
		new = malloc(new_capacity * vector->data_size);
	if (new == NULL)
		return (NULL);
	copy_data(vector, new);
	free(vector->nodes);
	vector->nodes = new;
	vector->capacity = new_capacity;
	return (vector->nodes);
}

static void	copy_data(tm_vector_t *vector, void **new)
{
	size_t	i;

	if (vector->data_size == 0)
	{
		i = 0;
		while (i < vector->total)
		{
			new[i] = vector->nodes[i];
			i++;
		}
		return ;
	}
	memcpy(new, vector->nodes, vector->total * vector->data_size);
}

void	*tm_vector_get(tm_vector_t *vector, int index)
{
	char	*buf;

	if (index < 0 || index >= (int)vector->total)
		return (NULL);
	if (vector->data_size == 0)
		return (vector->nodes[index]);
	buf = (char *)vector->nodes;
	return (&buf[index * vector->data_size]);
}

void	tm_vector_delete(tm_vector_t *vector, int index)
{
	char	*buf;
	size_t	i;

	if (index < 0 || index >= (int)vector->total)
		return ;
	if (vector->data_size == 0)
	{
		vector->cleanup_cb(vector->nodes[index]);
		free(vector->nodes[index]);
		i = index;
		while (i < vector->total - 1)
		{
			vector->nodes[i] = vector->nodes[i + 1];
			i++;
		}
		vector->nodes[i + 1] = NULL;
	}
	else
	{
		buf = (char *)vector->nodes;
		vector->cleanup_cb(&buf[index * vector->data_size]);
		memmove(&buf[index * vector->data_size], &buf[(index + 1) * vector->data_size],
			(vector->total - (index + 1)) * vector->data_size);
	}
	vector->total--;
}
