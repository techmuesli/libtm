//
// example_stack.c
//

#include <stdio.h>
#include "tm_stack.h"

int	main(void)
{
	int			value = 0;
	tm_stack_t	*stack;

	{
		int	*result;
	
		stack = tm_stack_init(10, sizeof(int));
		value = 1;
		tm_stack_push(stack, &value);
		value = 4;
		tm_stack_push(stack, &value);
		value = 2;
		tm_stack_push(stack, &value);
		if (stack->total != 3)
		{
			printf ("TEST1 - wrong number of nodes\n");
		}
		else
		{
			printf ("TEST1 - ");
			result = tm_stack_pop(stack);
			printf ("%d", *result);
			result = tm_stack_pop(stack);
			printf ("%d", *result);
			result = tm_stack_pop(stack);
			printf ("%d\n", *result);
		}
		tm_stack_cleanup(stack);
	}
	{
		stack = tm_stack_init(10, sizeof(int));
		value = 1;
		tm_stack_push(stack, &value);
		value = 4;
		tm_stack_push(stack, &value);
		tm_stack_clear(stack);
		if (stack->total != 0)
			printf ("TEST2 - wrong number of nodes\n");
		else
			printf ("TEST2 - successfully cleared\n");
		tm_stack_cleanup(stack);
	}
	return (0);
}
