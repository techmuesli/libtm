//
// example_stack.c
//

#include <stdio.h>
#include "tm_stack.h"

int	main(void)
{
	tm_stack_t	*stack;

	stack = tm_stack_init(10, 0);
	tm_stack_cleanup(stack);
	return (0);
}
