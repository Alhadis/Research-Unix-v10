#ifdef KR_headers
extern FILE *fdopen();
#else
#ifdef MSDOS
#include "io.h"
#define close _close
#define creat _creat
#define open _open
#define read _read
#define write _write
#endif
#ifdef __cplusplus
extern "C" {
#endif
#ifndef MSDOS
#ifdef OPEN_DECL
extern int creat(const char*,int), open(const char*,int);
#endif
extern int close(int);
extern int read(int,void*,size_t), write(int,void*,size_t);
extern int unlink(const char*);
#ifndef _POSIX_SOURCE
extern FILE *fdopen(int, const char*);
#endif
#endif

extern char *mktemp(char*);

#ifdef __cplusplus
	}
#endif
#endif
