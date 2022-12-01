//
// tm_vector.h
//

#ifndef TM_VECTOR_H
# define TM_VECTOR_H

# include <stdlib.h>

typedef void tm_vector_t;

tm_vector_t	*tm_vector_init(size_t capacity, size_t grow_count, size_t size);
void		tm_vector_cleanup(tm_vector_t *vector);
void		tm_vector_custom_cleanup(tm_vector_t *vector, void (*cleanup_cb)(void *data));
void		tm_vector_clear(tm_vector_t *vector);
void		*tm_vector_add(tm_vector_t *vector, void *data);
void		*tm_vector_add_index(tm_vector_t *vector, void *data, int index);
void		*tm_vector_get(tm_vector_t *vector, int index);
void		tm_vector_delete(tm_vector_t *vector, int index);

#endif
