/* -*- Mode:C++ -*- */
/*ident	"@(#)ctrans:src/alloc.c	1.1.1.8" */
/*
	$Source: /usr3/lang/benson/work/stripped_cfront/RCS/alloc.c,v $ $RCSfile: alloc.c,v $
	$Revision: 1.1 $		$Date: 89/11/20 08:50:09 $
	$Author: benson $		$Locker:  $
	$State: Exp $


*/
/**************************************************************************

        C++ source for cfront, the C++ compiler front-end
        written in the computer science research center of Bell Labs

        Copyright (c) 1984 AT&T, Inc. All Rights Reserved
        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

alloc.c:

*****************************************************************************/
#include "cfront.h"
#include <memory.h>
#include "size.h"

//int Nchunk;

//void print_free()
//{
//	fprintf(stderr,"free store: %d bytes alloc()=%d free()=%d\n",Nfree_store,Nal//loc, Nfree);
//	fprintf(stderr,"%d chunks: %d (%d)\n",Nchunk,CHUNK,Nchunk*CHUNK);
//}


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

#ifdef OPTIMIZE
inline void mzero (register char * p, register int l)
{
    while(l>=0) { *p++ = 0; l--; }
}
#else
#define mzero(p,l) memset(p,0,l)
#endif

/* page chunking slows things down -- disimproved locality
   overwhelms cpu in malloc? */
#ifdef PageChunk
#define PAGESWORTH(sz) (8192/sizeof(sz))
#define PAGESIZE 8192
#endif

void *
expr::operator new (size_t)
{
    char * page;

    if(!free) {
#ifdef PageChunk
	Pexpr x;
	page = valloc(PAGESIZE);
	free = Pexpr(page);
	for(x = free; x < &free[PAGESWORTH(expr) - 1]; x++)
	    x->e1 = x+1;
	x->e1 = 0;
#else
	page = malloc(sizeof(expr));
	free = Pexpr(page);
	free->e1 = 0;
#endif
    }
    page = (char *)free;
    free = free->e1;
    mzero(page, sizeof(expr));
    return (void *)page;
}

void *
stmt::operator new (size_t)
{
    char * page;

    if(!free) {
#ifdef PageChunk
	Pstmt x;
	page = valloc(PAGESIZE);
	free = Pstmt(page);
	for(x = free; x < &free[PAGESWORTH(stmt) - 1]; x++)
	    x->s_list = x+1;
	x->s_list = 0;
#else
	page = malloc(sizeof(stmt));
	free = Pstmt(page);
	free->s_list = 0;
#endif
    }
    page = (char *)free;
    free = free->s_list;
    mzero(page, sizeof(stmt));
    return (void *) page;
}

void *
name::operator new (size_t)
{
    char * page;

    if(!free) {
#ifdef PageChunk
	Pname x;
	page = valloc(PAGESIZE);
	free = Pname(page);
	for(x = free; x < &free[PAGESWORTH(name) - 1]; x++)
	    x->n_tbl_list = x+1;
	x->n_tbl_list = 0;
#else
	page = malloc(sizeof(name));
	free = Pname(page);
	free->n_tbl_list = 0;
#endif
    }
    page = (char *)free;
    free = free->n_tbl_list;
    mzero(page, sizeof(name));
    return (void *) page;
}

void
expr::operator delete (void * ve)
{
    Pexpr(ve)->permanent = 3;
    Pexpr(ve)->e1 = new_free;
    new_free = Pexpr(ve);
}

void
stmt::operator delete (void * vs)
{
    Pstmt(vs)->permanent = 3;
    Pstmt(vs)->s_list = new_free;
    new_free = Pstmt(vs);
}

void
name::operator delete (void * vn)
{
    Pname(vn)->permanent = 3;
    Pname(vn)->n_tbl_list = new_free;
    new_free = Pname(vn);
}

void recycle_deleted_storage ()
{
    Pexpr fce;
    if(expr::free) {
	for (fce = expr::free;fce->e1;fce = fce->e1) ; /* just chase */
	fce->e1 = expr::new_free;
    } else expr::free = expr::new_free;
    expr::new_free = 0;

    Pstmt sce;
    if(stmt::free) {
	for (sce = stmt::free;sce->s_list;sce = sce->s_list) ; /* just chase */
	sce->s_list = stmt::new_free;
    } else stmt::free = stmt::new_free;
    stmt::new_free = 0;

    Pname nce;
    if(name::free) {
	for (nce = name::free;nce->n_tbl_list;nce = nce->n_tbl_list) ; /* just chase */
	nce->n_tbl_list = name::new_free;
    } else name::free = name::new_free;
    name::new_free = 0;
}

static char rcsinfo[] = "$Header: /usr3/lang/benson/work/stripped_cfront/RCS/alloc.c,v 1.1 89/11/20 08:50:09 benson Exp $";


/* $Log:	alloc.c,v $
 * Revision 1.1  89/11/20  08:50:09  benson
 * Initial revision
 * 
 * Revision 1.1  89/11/20  08:49:48  benson
 * Initial revision
 * 
 * Revision 1.1  89/11/20  08:49:25  benson
 * Initial revision
 * 
 * Revision 1.1  89/11/20  08:48:50  benson
 * Initial revision
 * 
 * Revision 1.6  89/10/05  11:37:40  benson
 * change free-list management. Deleted things aren't recycled ever
 * until the start of the next top level declaration.
 * 
 * Revision 1.5  89/09/02  22:08:56  benson
 * mark things deleted.
 * 
 * Revision 1.4  89/07/21  16:47:01  benson
 * add operator new/delete to expr, stmt, name that use free lists.
 * It speeds things up enough to be worthwhile.
 * 
 * Revision 1.3  89/07/20  17:42:10  benson
 * Rip out chunk allocator.
 * 
 * Revision 1.2  89/06/28  12:56:59  benson
 * Port of changes from 2.0beta6 odi bugfixes
 * 
   end_log

*/
