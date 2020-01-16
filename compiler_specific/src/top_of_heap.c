/*
 * top_of_heap.c
 *
 *  Created on: 2014/02/24
 *      Author: a5012939
 */

#include	<stddef.h>

extern	char	_heap_end;

char	*_heap_of_memory	= NULL;
char	*_last_heap_object	= NULL;


char*
_top_of_heap( void )
{
	return &_heap_end;
}
/*EOF*/
