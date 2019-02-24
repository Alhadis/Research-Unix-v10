#include "ar.h"
#include "config.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>

/* system-dependent stuff */
#ifndef major
#include <sys/sysmacros.h>
#endif

#ifdef FCNTL
#include <fcntl.h>
#define dup2(x,y) (close(y),fcntl(x,F_DUPFD,y))
#endif

/* longest archive component name we will generate */
#define MAXCOMP 14

/* macro to allocate storage of a given type */
#define new(t) ((t *) alloc (sizeof (t)))

/* constants for tmpname function */
#define TMPDIR	"/tmp/"
#define TMPNAML	(sizeof(TMPDIR) + 15)

/* some systems define SIG_TYP, others don't, so we make our own */
typedef int	(*Sig_typ)();

struct replist {
	char *source;
	char *dest;
	struct replist *link;
} *replist;

/* structures to deal with archive headers */
struct ar_hdr ar_hdr;
struct {
	long size;
	int mode;
	long date;
} hdr;

/*
 *	This stat buffer makes it easy to check that none of the
 *	input files is the same as the output.  This will avoid
 *	filling up the entire file system by inadvertently saying
 *
 *		mkpkg . > foo
 */
struct stat outsb;

/*
 *	The following structure helps keep track of things being packaged.
 *	iname is the internal name of the component -- in other words,
 *	the archive element name.  ename is the (short) pathname of the
 *	file.  The structures are chained by the "link" field.  All the
 *	other fields are copies of things returned by "stat" and are
 *	used mostly to make sure nothing changed while we were packaging.
 *	head and tail point to the first and last items in the chain.
 *	The first item is known to refer to the "Instructions" component.
 */
struct pack {
	char *iname;
	char *ename;
	struct pack *link;
	dev_t dev;
	ino_t ino;
	int uid, gid, mode;
	time_t time;
	off_t size;
};

struct pack *pkhead, *pktail;


FILE *popen();
char *Xstr;
char *alloc();
char *copy();
char *fgets();
char *fullname();
char *getfield();
char *getpass();
char *getpath();
extern char *hextab;
char *iname();
extern char *instr;
char *keyfile;
char *mktemp();
extern char *optarg;
char *pwd();
char *ralloc();
char *strcat();
char *strchr();
char *strcpy();
char *strrchr();
char *strgid();
char *struid();
char *tmpname();
char *transname();
char *xstr;
int Kflag;
int bflag;
int consider();
int dflag;
int errno;
int kflag;
int nflag;
int install();
int numgid();
int numuid();
extern int optind;
int package();
int pkgend();
int retcode;
int seal();
int unseal();
int vflag;
long cvlong();
long read_header();
void delete();
void geteol();
void next_header();
void pkgstart();
void pkgfile();
void putpath();
