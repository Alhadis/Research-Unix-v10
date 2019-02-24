#ifndef __STDARG
#define __STDARG

typedef char*	va_list;

#define va_start(list, start)	list = (char *) (&(start)+1)
#define va_end(list)
#define va_arg(list, mode)	((mode *)(list += sizeof(mode)))[-1]

#endif
