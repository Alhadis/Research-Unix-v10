/*ident	"@(#)ctrans:src/alloc.c	1.1.1.8" */
/**************************************************************************

        C++ source for cfront, the C++ compiler front-end
        written in the computer science research center of Bell Labs

        Copyright (c) 1984 AT&T, Inc. All Rights Reserved
        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

alloc.c:

*****************************************************************************/
#include "cfront.h"
#include "size.h"

//int Nchunk;

//void print_free()
//{
//	fprintf(stderr,"free store: %d bytes alloc()=%d free()=%d\n",Nfree_store,Nal//loc, Nfree);
//	fprintf(stderr,"%d chunks: %d (%d)\n",Nchunk,CHUNK,Nchunk*CHUNK);
//}


void* chunk(int i)	// get memory that is not to be freed
{
	register char* cp = malloc(i*CHUNK-8);
	if (cp == 0) {			// no space
		free((char*)gtbl);	// get space for error message
//		if (Nspy) print_free();
		error('i',"free store exhausted");
	}
//	Nchunk += i;
//	Nfree_store += i*CHUNK;
	return cp;
}

/*
#ifdef __cplusplus
#include <new.h>
#define NEW_SIZE size_t
#else
#define NEW_SIZE long
#endif
*/

// now __HAVE_SIZE_T defined in 2.0 malloc.h c++ header file
#ifdef __HAVE_SIZE_T
#include <new.h>
#define NEW_SIZE size_t
#else
#define NEW_SIZE long
#endif

void* operator new(NEW_SIZE sz)	// get memory that might be freed
{
	char* p = calloc((unsigned)sz,1);

//fprintf(stderr,"alloc(%d)->%d\n",sz,p);
	if (p == 0) {			// no space
		free((char*)gtbl);	// get space for error message
//		if (Nspy) print_free();
		error('i',"free store exhausted");
	}
//	Nalloc++;
//	Nfree_store += sz+sizeof(int*);
	return p;
}
//int NFn, NFtn, NFbt, NFpv, NFf, NFe, NFs, NFc;

void operator delete (void* p)
{
	if (p == 0) return;

//fprintf(stderr,"free(%d) %d\n",p,((int*)p)[-1]-(int)p-1+sizeof(int*));

//if (Nspy) {
//	Pname pp = (Pname) p;
//	TOK t = pp->base;
//	Nfree++;
//	Nfree_store -= ((int*)p)[-1]-(int)p-1+sizeof(int*);
//	switch (t) {	// can be fooled by character strings
//	case INT: case CHAR: case TYPE: case VOID: case SHORT: case LONG:
//	case FLOAT: case DOUBLE: case LDOUBLE: case COBJ: case EOBJ: case FIELD:
//			NFbt++; break;
//
//	case PTR: case VEC:
//			NFpv++; break;
//
//	case FCT:	NFf++; break;
//
//	case ICON: case CCON: case STRING: case FCON: case THIS:
//			NFc++; break;
//	}
//}
	free((char*)p);
}
