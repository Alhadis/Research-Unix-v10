/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */

/* Bfeats.h - typedef, constants, and function declarations for Bfeats.
   INCLUDES
	Requires prior #include "BMask.h"
   */

typedef struct Bfeats {
	BMask bm;		/* bitmask (1 for each feature) */
	unsigned short mny;	/* number of feature indices in fa[] */
	unsigned short *fa;	/* fa[mny] of feature (ss) nos, no dups (malloc) */
	} Bfeats;

#define Init_Bfeats {Init_BMask,0,NULL}
#if MAIN
	Bfeats empty_Bfeats = Init_Bfeats;
#else
	extern Bfeats empty_Bfeats;
#endif

Bfeats *alloc_bfeats();
free_bfeats();
char *bfeats_toa();
Bfeats *dup_bfeats();
