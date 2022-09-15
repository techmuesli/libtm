//
// vector.c
//

#include <stdio.h>
#include "tm_vector.h"

int	main(void)
{
	tm_vector_t	*vector;

	vector = tm_vector_init(10, 10, 0);
	tm_vector_cleanup(vector);
	return (0);
}
