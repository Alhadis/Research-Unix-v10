// C++ library functions dealing with operator new

#ifndef _NEW_H
#define _NEW_H 1

typedef unsigned size_t;

extern void (*set_new_handler (void(*)()))();

void* operator new (size_t, void*);

#endif
