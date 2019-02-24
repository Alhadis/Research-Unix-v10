/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * stack routine definitions
 */

#ifndef __STACK_H

#define __STACK_H

#ifndef VOID
#define VOID	char
#endif

typedef struct stacktable* STACK;	/* stack pointer		*/
typedef struct stackposition STACKPOS;	/* stack position		*/

struct stackblock			/* stack block cell		*/
{
	char**		  stack;	/* actual stack			*/
	struct stackblock* prev;	/* previous block in list	*/
	struct stackblock* next;	/* next block in list		*/
};

struct stackposition			/* stack position		*/
{
	struct stackblock* block;	/* current block pointer	*/
	int		index;		/* index within current block	*/
};

struct stacktable			/* stack information		*/
{
	struct stackblock* blocks;	/* stack table blocks		*/
	char*		error;		/* error return value		*/
	int		size;		/* size of each block		*/
	STACKPOS	position;	/* current stack position	*/
};

#if __STDC__ || __cplusplus || c_plusplus

#if __cplusplus
extern "C" {
#endif

extern STACK	mkstack(int size, char* error);
extern void	rmstack(STACK stack);
extern void	clrstack(STACK stack);
extern char*	getstack(STACK stack);
extern int	pushstack(STACK stack, char* value);
extern int	popstack(STACK stack);
extern void	posstack(STACK stack, int set, STACKPOS* position);

#if __cplusplus
}
#endif

#else

extern STACK	mkstack();
extern void	rmstack();
extern void	clrstack();
extern char*	getstack();
extern int	pushstack();
extern int	popstack();
extern void	posstack();

#endif

#endif
