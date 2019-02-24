
#ifndef va_start

/* 
   Proper usage of this version of varargs is slightly different from
   that found in C. Functions are declared

       foo(int arg ...) { va_list ap; ... va_start(ap,arg1) ; ...

   rather than

       foo(va_alist) va_dcl { va_list ap; ... va_start(ap);

   use of va_arg, and va_end remain the same.

   Arg does not have to be int, but it must be declared.  Hence
   all calls of foo must have the same first type for their first
   argument. (In the most common printf-like cases the first argument
   is a char* format.)
 */ 

typedef char *va_list;
#define va_start(list,last) list = (char *) &last
#define va_end(list)
#ifdef u370
#define va_arg(list, mode) ((mode *)(list = \
	(char *) ((int)list + 2*sizeof(mode) - 1 & -sizeof(mode))))[-1]
#else
#define va_arg(list, mode) ((mode *)(list += sizeof(mode)))[-1]
#endif

#endif 


