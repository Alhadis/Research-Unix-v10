#include "ar.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>

#if 0
/* system-dependent stuff */
#ifdef unix
#ifndef major
#include <sys/sysmacros.h>
#endif
#endif
#endif

/* longest archive component name we will generate */
#define MAXCOMP 14

/* macro to allocate storage of a given type */
#define new(t) ((t *) alloc (sizeof (t)))

/* some systems define SIG_TYP, others don't, so we make our own */
#ifdef __STDC__
typedef void (*Sig_typ)(int);
#else
typedef int	(*Sig_typ)();
#endif

struct replist {
	char *source;
	char *dest;
	struct replist *link;
};

extern struct replist *replist;

/* structures to deal with archive headers */
struct ar_hdr ar_hdr;
struct hdr {
	long size;
	int mode;
	long date;
};
extern struct hdr hdr;

/* alloc.c */
extern void *alloc(size_t);
extern void *ralloc(void *, size_t);
extern char *copy(char *);

/* args.c */
extern int bflag;
extern int kflag;
extern int Kflag;
extern int nflag;
extern int vflag;
extern char *xstr;
extern char *Xstr;
extern int getargs(int, char **, char *, int (*)(FILE *, char *));

/* chk.c */
extern void nchk(int);
extern void schk(void *);

/* crypt.c */
extern void resetN12(void);
extern void setup(char *);
extern void mangle(char *, char *);

/* data.c */
extern char *hextab;
extern char *instr;

/* fullname.c */
extern char *fullname(char *);

/* gid.c */
extern char *gidstr(gid_t);
extern gid_t gidnum(char *);

/* header.c */
extern long cvlong(char *, size_t, int);
extern long read_header(char *, FILE *);
extern void next_header(FILE *);
extern char *getfield(FILE *);
extern void geteol(FILE *);

/* mkdir.c */
extern int mkd(char *);
extern int rmdir(char *);
extern int rmall(char *);

/* package.c */
extern void pkgstart(void);
extern void pkgfile(char *);
extern int pkgend(void);

/* path.c */
extern char *getpath(FILE *);
extern void putpath(FILE *, char *);

/* pwd.c */
extern char *pwd(void);

/* transname.c */
extern char *transname(char *);

/* uid.c */
extern char *uidstr(uid_t);
extern uid_t uidnum(char *);
