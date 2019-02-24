#ifndef __MALLOC__H
#define __MALLOC__H 1

typedef unsigned size_t;
extern "C" {
	char *malloc(size_t);
	void free(char*);
	char *realloc(char*,size_t);
	char *calloc(size_t,size_t);
	void cfree(char*);
}

#endif
