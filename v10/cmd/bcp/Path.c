/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */
/* Path.c - Path functions */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#define FILE_TREE_INCL 1
#include "CPU.h"
#include "Path.h"

#define dbg_FTW (0)

/* Return full path string in Path *pp, starting at level `sl';
   if the level is too deep or the name at level `sl' is null, return null string.
   */
char *path_toa(pp,sl)
    Path *pp;
    int sl;	/* starting level: 0 is first */
{   int len,lvl;
    static char s[MAX_PATH_DEPTH*(32)];
	s[0] = '\0';
	if( sl<=pp->level && pp->name[sl]!=NULL && pp->name[sl][0]!='\0' ) {
		for(lvl=sl; lvl<=pp->level; lvl++) {
			strcat(s,pp->name[lvl]);
			strcat(s,"/");
			};
		};
	if((len=strlen(s))>0) s[len-1] = '\0';
	return(s);
	}

/* no. slashes in a name */
int slashes(n)
    char *n;
{   register int res;
    register char *c;
	res=0;
	c=n;
	while(*c!='\0') {
		if(*c=='/') res++;
		c++;
		};
	return(res);
	}

/* basename */
char *bname(n)
    char *n;
{   register char *res,*c;
	res=n-1;
	c=n;
	while(*c!='\0') {
		if(*c=='/') res=c;
		c++;
		};
	return(res+1);
	}

#if CPU==VAX || CPU==CRAY

int process_file_tree_node(n,s,code,S)
    char *n;
    struct stat *s;
    int code;
    struct FTW *S;
/* Uses global `path_process' structure */
{   char *out_n;
    int persist;
    struct stat sbuf;
	switch(code) {
	   case FTW_F:	if(dbg_FTW)err("visit file %s %s %d",n,n+(S->base),S->level);
			if(S->level>0) {
				if(path_process.path.name[S->level]!=NULL)
					free(path_process.path.name[S->level]);
				path_process.path.name[S->level]=strdup(n+(S->base));
				};
			path_process.path.level = S->level;
			out_n = strdup(path_toa(&path_process.path,0));
			if(dbg_FTW)err("file: %s ==> %s",n,out_n);
			(*path_process.process)(n,out_n,path_process.arg);
			if(out_n!=NULL) free(out_n);
			break;
	   case FTW_SL:	if(dbg_FTW)err("visit link %s %s %d",n,n+(S->base),S->level);
			S->quit = FTW_FOLLOW;	 /* follow symbolic link */
			break;
	   case FTW_D:	/* pre-visit of directory */
			if(dbg_FTW)err("visit dir %s %s %d",n,n+(S->base),S->level);
			if(S->level>0) {
				if(path_process.path.name[S->level]!=NULL)
					free(path_process.path.name[S->level]);
				path_process.path.name[S->level]=strdup(n+(S->base));
				};
			path_process.path.level = S->level;
			out_n = path_toa(&path_process.path,0);
			if(dbg_FTW)err("dir:   %s --> %s",n,out_n);
			if(out_n[0]=='\0') /* no output name */ break;
			/* ensure directory named out_n exists */
			if(stat(out_n,&sbuf)==0) {
				/* file exists */
				if(!(sbuf.st_mode&S_IFDIR)) {
					/* not a directory - try to remove */
					if(unlink(out_n)==0) {
						if(mkdir(out_n,0777)!=0) {
							/* irrecoverable mkdir */
							abort("can't mkdir %s",
								out_n);
							};
						}
					else {	/* irrecoverable unlink */
						abort("can't unlink %s",out_n);
						};
					};
				}
			else if(errno==ENOENT) {
				/* file doesn't exist */
				if(mkdir(out_n,0777)!=0) {
					/* irrecoverable mkdir */
					abort("can't mkdir %s",out_n);
					};
				}
			else {	/* irrecoverable stat */
				abort("can't stat %s",out_n);
				};
			break;
	   case FTW_DP:	/* post-visit of directory: ignore it*/
			break;
	   case FTW_DNR:
			err("can't read dir %s (FTW_DNR) - ignore it",n);
			break;
	   case FTW_NS:
			err("can't stat %s (FTW_NS) - ignore it",n);
			break;
	   case FTW_NSL:
			if(0)	/* happens if file is missing */
			err("can't stat symbolic link %s (FTW_NSL) - ignore it",n);
			break;
	   default:
			err("unexpected FTW code %d - ignore it",code);
			break;
	   };
	return(0);
	}

#else
#if CPU==MIPS
int process_file_tree_node(n,s,code)
    char *n;		/* name of the object */
    struct stat *s;	/* info. about object */
    int code;
/* Uses global `path_process' structure */
#define lvl path_process.path.level
{   char *out_n;
    int persist;
    struct stat sbuf;
    char *bn;
	lvl = slashes(n) - path_process.sl0;
	bn = bname(n);	
	switch(code) {
	   case FTW_F:	if(dbg_FTW) err("visit file %s, bn %s, lvl %d",n,bn,lvl);
			if(lvl>0) {
				if(path_process.path.name[lvl]!=NULL)
					free(path_process.path.name[lvl]);
				path_process.path.name[lvl]=strdup(bn);
				};
			out_n = strdup(path_toa(&path_process.path,0));
			if(dbg_FTW)err("file: %s ==> %s",n,out_n);
			(*path_process.process)(n,out_n,path_process.arg);
			if(out_n!=NULL) free(out_n);
			break;
	   case FTW_D:	/* pre-visit of directory */
			if(dbg_FTW) err("visit dir %s, bn %s, lvl %d",n,bn,lvl);
			if(lvl>0) {
				if(path_process.path.name[lvl]!=NULL)
					free(path_process.path.name[lvl]);
				path_process.path.name[lvl]=strdup(bn);
				};
			out_n = path_toa(&path_process.path,0);
			if(dbg_FTW)err("dir:  %s --> %s",n,out_n);
			if(out_n[0]=='\0') /* no output name */ break;
			/* ensure directory named out_n exists */
			if(stat(out_n,&sbuf)==0) {
				/* file exists */
				if(!(sbuf.st_mode&S_IFDIR)) {
					/* not a directory - try to remove */
					if(unlink(out_n)==0) {
						if(mkdir(out_n,0777)!=0) {
							/* irrecoverable mkdir */
							abort("can't mkdir %s",
								out_n);
							};
						}
					else {	/* irrecoverable unlink */
						abort("can't unlink %s",out_n);
						};
					};
				}
			else if(errno==ENOENT) {
				/* file doesn't exist */
				if(mkdir(out_n,0777)!=0) {
					/* irrecoverable mkdir */
					abort("can't mkdir %s",out_n);
					};
				}
			else {	/* irrecoverable stat */
				abort("can't stat %s",out_n);
				};
			break;
	   case FTW_DNR:
			err("can't read dir %s (FTW_DNR) - ignore it",n);
			break;
	   case FTW_NS:
			err("can't stat %s (FTW_NS) - ignore it",n);
			break;
	   default:
			err("unexpected FTW code %d - ignore it",code);
			break;
	   };
	return(0);
	}

#endif
#endif

/* Process every leaf file (non-directory) in the file-tree rooted at `in_ftn'.
   If `in_ftn' is itself a leaf, then only it is processed.  If it is a directory,
   then a corresponding file-tree is built, rooted at `out_ftn', and paths
   to its leaves created by `mkdir's as required;  for each pair of corresponding
   leaf filenames, `process' is called.  The filenames have not yet been opened.
   If `out_ftn' is the empty string, then no output file-tree will be generated
   and the corresponding output filenames will all be null.  If `in_ftn' is the
   empty string, then `process' is called only once, with the input filename
   empty.  Pass 'arg' to 'process' as third argument. */
process_file_trees(process,in_ftn,out_ftn,arg)
    VOID (*process)();
    char *in_ftn;	/* may be empty */
    char *out_ftn;	/* may be empty */
    VOID *arg;		/* passed to '(*process)()' as 3rd argument */
/* Uses global `path_process' structure */
{   int level;
	if(in_ftn[0]=='\0') (*process)(in_ftn,out_ftn,arg);
	else {	if(dbg_FTW) err("process_file_trees(%s,%s)",in_ftn,out_ftn);
		path_process.path.level = 0;
		path_process.path.name[0] = out_ftn;
		path_process.sl0 = slashes(in_ftn);
		for(level=1; level<MAX_PATH_DEPTH; level++)
			path_process.path.name[level] = NULL;
		path_process.process = process;
		path_process.arg = arg;
		if(ftw(in_ftn,process_file_tree_node,MAX_PATH_DEPTH)<0)
			err("ftw error: errno %d",errno);
		};
	}
