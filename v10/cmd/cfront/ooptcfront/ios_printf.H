/* Begin iostream_printf.H -- printf functions that work on ostreams.
   someday, perhaps, scanf functions for istreams. */

#include <stdarg.h>
#include <iostream.h>

int vostream_printf(const char *format, va_list argp, ostream& fp);
int ostream_printf(ostream& stream, const char * format ...);
// returns count of characters added.
int printf_to_string (char * string, int length, const char * format ...);
// returns string which came from malloc.
char * printf_to_alloc_string (const char * format ...);

/* End iostream_printf.H */

