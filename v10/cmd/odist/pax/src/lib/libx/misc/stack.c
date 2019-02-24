/*
 * G. S. Fowler
 *
 * stack routines
 */

#include "stack.h"

#if !lint
static char	id[] = "\n@(#)stack (AT&T Bell Laboratories) 05/01/84\0\n";
#endif

extern char	*malloc();

/*
 * create a new stack
 */

STACK
mkstack(size, error)
register int	size;
char		*error;
{
	register STACK			stack;
	register struct stackblock	*b;

	if (size <= 0) size = 100;
	if (!(stack = (STACK)malloc(sizeof(struct stacktable)))) return((STACK)0);
	if (!(b = (struct stackblock*)malloc(sizeof(struct stackblock))))
	{
		free(stack);
		return((STACK)0);
	}
	if (!(b->stack = (char**)malloc(sizeof(char*)*size)))
	{
		free(b);
		free(stack);
		return((STACK)0);
	}
	stack->blocks = b;
	stack->size = size;
	stack->error = error;
	stack->position.block = b;
	stack->position.index = -1;
	b->next = (struct stackblock*)0;
	b->prev = (struct stackblock*)0;
	return(stack);
}

/*
 * remove a stack
 */

void
rmstack(stack)
register STACK	stack;
{
	register struct stackblock	*b;

	for (b = stack->blocks; b; b = b->next)
	{
		free(b->stack);
		free(b);
	}
	free(stack);
}

/*
 * clear stack
 */

void
clrstack(stack)
register STACK	stack;
{
	stack->position.block = stack->blocks;
	stack->position.index = -1;
}

/*
 * get value on top of stack
 */

char*
getstack(stack)
register STACK	stack;
{
	if (stack->position.index < 0) return(stack->error);
	else return(stack->position.block->stack[stack->position.index]);
}

/*
 * push value on to stack
 */

int
pushstack(stack, value)
register STACK	stack;
char		*value;
{
	register struct stackblock	*b;

	if (++stack->position.index >= stack->size)
	{
		b = stack->position.block;
		if (!b->next)
		{
			if (!(b->next = (struct stackblock*)malloc(sizeof(struct stackblock))))
				return(-1);
			b = b->next;
			if (!(b->stack = (char**)malloc(sizeof(char*)*stack->size)))
				return(-1);
			b->prev = stack->position.block;
			b->next = (struct stackblock*)0;
			stack->position.block = b;
		}
		stack->position.index = 0;
	}
	stack->position.block->stack[stack->position.index] = value;
	return(0);
}

/*
 * pop value off stack
 */

int
popstack(stack)
register STACK	stack;
{
	/*
	 * return:
	 *
	 *	-1	if stack empty before pop
	 *	 0	if stack empty after pop
	 *	 1	if stack not empty before & after pop
	 */

	if (stack->position.index < 0) return(-1);
	else if (--stack->position.index < 0)
	{
		if (!stack->position.block->prev) return(0);
		stack->position.block = stack->position.block->prev;
		stack->position.index = stack->size - 1;
		return(1);
	}
	else return(1);
}

/*
 * set|get stack position
 */

void
posstack(stack, set, position)
register STACK	stack;
int		set;
STACKPOS	*position;
{
	if (set) stack->position = *position;
	else *position = stack->position;
}
