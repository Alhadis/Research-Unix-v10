/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */

/* Path.h - Path names */

#define MAX_PATH_DEPTH (16)	/* maximum expected names in path */

typedef struct Path {
	int level;			/* current level */
	char *name[MAX_PATH_DEPTH];	/* name[0] is output root name;
					   the rest are basenames (both input
					   and output) */
	} Path;

typedef struct Path_process {
	int sl0;		/* no. slashes in input root name */
	Path path;
	VOID (*process)();
	VOID *arg;		/* passed as 3rd arg to (*process)() */
	} Path_process;

Path_process path_process;	/* controls file-tree processing */

char *path_toa();	/* see Path.c */
