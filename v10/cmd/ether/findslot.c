#include <errno.h>

extern int errno;


findslot(prefix, min, max, mode)
char *prefix;
{
     char filename[40];
     int  n, fd;

     for (n = min; max < 0 || n <= max;) {
          sprintf(filename, "%s%02d", prefix, n++);
	  if ((fd = open(filename, mode)) >= 0)
	       return fd;
          if (errno != ENXIO)
	       return -1;
     }

     errno = ENOENT;
     return -1;
}
