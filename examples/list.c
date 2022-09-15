//
// list.c
//

#include <stdio.h>
#include "tm_list.h"

int	main(void)
{
	tm_list_t	*list;

	list = tm_list_init(10, 10, 0);
	tm_list_cleanup(list);
	return (0);
}
