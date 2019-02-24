#ifndef __SETJMP
#define __SETJMP

typedef int jmp_buf[10];

extern void	longjmp(jmp_buf, int);
extern int	setjmp(jmp_buf);

#endif
