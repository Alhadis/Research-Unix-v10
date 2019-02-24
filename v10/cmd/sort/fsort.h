/* Copyright 1990, AT&T Bell Labs */
#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned short len_t;

/* A record header is immediately followed by its data,
   dlen bytes long.  According to variable "keyed", a
   key of len klen follows, or is superposed on, the data.
   All comparisons are strictly lexicographic. */

struct rec {
	struct rec *next;
	len_t len[2];	/* lengths of data and key strings */
			/* len[0] bytes of data follow */
			/* then len[1] (often 0) bytes of key */
};

#define dlen len[0]	/* (virtual field) length of data string */
#define klen len[1]	/* (virtual field) length of key string */
#define MAXREC 0xffff	/* bound on dlen or klen */
#define MINREC (10*sizeof(struct rec))	/* initial elbow room */

#define data(/*struct rec* */r) ((uchar*)((r)+1))
#define key(/*struct rec* */r) (data(r)+(r)->dlen)

#define succ(/*struct rec* */r) ((struct rec*)(data(r) + ((r)->dlen \
	+ (r)->klen + (sizeof(r)-1))/sizeof(r)*sizeof(r)))

struct list {
	struct rec *head;	/* pointer to first record */
	struct rec *tail;	/* pointer to last record */
};

struct pos {
	short fieldno;
	short charno;
};
	
	/* describes a field of the key
           coder(dataptr, keyptr, len, fieldptr) encodes
           data of specified length into a key, observing
           parameters of the field */

struct field {
	int (*coder)(uchar*, uchar*, int, struct field*);
	uchar *trans;		/* translation table */
	uchar *keep;		/* deletion table */
	uchar rflag;		/* sort in reverse order */
	uchar bflag;		/* skip initial blanks */
	uchar eflag;		/* bflag on end posn */
	uchar style;		/* -k or +pos -pos */
	struct pos begin;	/* where the key begins */
	struct pos end;		/* where it ends */
};

extern struct list *stack;
extern struct list *stackmax;
extern struct rec *buffer;
extern uchar *bufmax;
extern struct rec endfile;	/* dummy location with a */
#define ENDFILE &endfile	/* distinctive pointer value */

#define NF 30		/* NF-1 = max number of field specs */
#define NP 30		/* NP-1 is largest permitted field number */
extern struct field fields[NF];
extern int nfields;
extern struct field accum[NF];
extern int naccum;
extern int nmerge;

extern char *oname;	/* output file name */
extern char *tname[];	/* possible temporary directories */
extern char **files;	/* names of input files */
extern int nfiles;	/* number of input files */
extern int nextfile;	/* number of intermediate files */

extern int keyed;	/* key is separate from data */
extern int simplekeyed;	/* key not separate but not trivial */
extern int aflag;
extern int cflag;
extern int mflag;
extern int uflag;
extern int rflag;
extern int sflag;
extern int signedrflag; /* 1 or -1 for fields[0].rflag 0 or 1*/
extern int tab;		/* tab char for -t */

			/* translation and deletion tables */
extern uchar ident[];
extern uchar fold[];
extern uchar all[];
extern uchar dict[];
extern uchar ascii[];

	/* key-making functions and room function, which provides
	   a bound on how much space the key will take given the
           length of the data.  Room for Mcode is 1,
	   for tcode 2*len+1, for ncode (len+5)/2.
           Virtual function would be nice here */
   
extern int tcode(uchar*, uchar*, int, struct field*);
extern int Mcode(uchar*, uchar*, int, struct field*);
extern int ncode(uchar*, uchar*, int, struct field*);
extern int gcode(uchar*, uchar*, int, struct field*);
extern int acode(uchar*, uchar*, int, struct field*);
#define room(len) (2*(len)+2)

extern struct rec *getline(struct rec*, FILE*);
extern void printout(struct rec*, FILE*, char*);
extern void sort(struct list*, int);
extern void merge(int);
extern void check(char*);
extern int fieldarg(char*, char*);
extern int fieldcode(uchar*, uchar*, int, uchar*, struct field*, int);
extern void fieldwrapup(void);
extern void tabinit(void);
extern void tabfree(void);
extern void optiony(char*);
extern void optionk(char*, struct field*, int*);
extern void warn(char*, char*, int);
extern void fatal(char*, char*, int);
extern FILE *fileopen(char*, char*);
extern void fileclose(FILE*, char*);
extern char *filename(int);
extern int overwrite(int);
extern void cleanup(int);
extern void setsigs(void(*)(int));
extern struct rec* listaccum(struct rec*, struct rec*);
extern int doaccum(struct rec*, struct rec*);
extern void chkaccum(struct field*);
