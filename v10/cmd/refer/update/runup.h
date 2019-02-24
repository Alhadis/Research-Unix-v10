# include "stdio.h"
# include "assert.h"
# include "ctype.h"
struct bibfile {
	int bibfno; /* bib file no. %03d */
	char *bibfref; /* file referred to */
	long bibfstrt; /* starting byte of item */
	long bibival; /* pointer to index in Ind.ic file */
	int bibfleng; /* length of item */
	};
# define SAME 0
extern struct bibfile bibs[];
struct newfile {
	long nn_ic;
	long new_ic;
	};
struct mapper {
	long map_old;
	long map_by;
	};
extern struct mapper maps[];
extern struct newfile news[];
extern int nhash, lgout;
extern long ins1, ins2, add;
extern long mappos(), time();
extern char *Master, rundir[50];
extern int verbose, vhash, chatty;
extern double tuse();
extern long start;
extern int tlapse();
extern char *newplace;
