//
// vector.c
//

#include <stdio.h>
#include "tm_vector.h"

int	main(void)
{
	int			value = 0;
	tm_vector_t	*vector;

	{
		int	*result;
	
		vector = tm_vector_init(10, 10, sizeof(int));
		value = 21;
		tm_vector_add(vector, &value);
		result = tm_vector_get(vector, 0);
		if (*result != 21)
			printf("TEST1 - failed\n");
		else
			printf("TEST1 - successful result: %d\n", *result);
		tm_vector_cleanup(vector);
	}
	{
		int	*result;
	
		vector = tm_vector_init(10, 10, sizeof(int));
		value = 21;
		tm_vector_add(vector, &value);
		result = tm_vector_get(vector, 0);
		if (*result != 21)
			printf("TEST2 - failed\n");
		tm_vector_delete(vector, 0);
		if (tm_vector_get(vector, 0) != NULL)
			printf("TEST2 - failed\n");
		else
			printf("TEST2 - successful\n");
		tm_vector_cleanup(vector);
	}
	{
		int	*result;
	
		vector = tm_vector_init(10, 10, sizeof(int));
		value = 21;
		tm_vector_add(vector, &value);
		result = tm_vector_get(vector, 0);
		if (*result != 21)
			printf("TEST3 - failed\n");
		value = 42;
		tm_vector_add_index(vector, &value, 1);
		result = tm_vector_get(vector, 1);
		if (*result != 42)
			printf("TEST3 - failed\n");
		else
			printf("TEST3 - successful\n");
		tm_vector_cleanup(vector);
	}
	return (0);
}
