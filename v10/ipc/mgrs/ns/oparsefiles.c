#include <fio.h>
#include <libc.h>
#include <string.h>
#include <sysent.h>
#include <sys/types.h>
#include <sys/stat.h>
extern "C" int stat (const char*, struct stat*);	/* added by pg so that
							 * this could compile
							 */
#include "dbtypes.h"

#define MINLINE 256

/*
 * predeclared
 */
int parseuusys(Ordered *, int, Tuple *);
int parseinhost(Ordered *, int, Tuple *);
int parseinnet(Ordered *, int, Tuple *);
int parsedb(Ordered *, int, Tuple *);
void parsefile(Ordered *, int (*)(Ordered *, int, Tuple *), char *, Tuple *,
		 char *);
int checkfiles();

/*
 *  command table
 */
struct cmdtbl {
	int (*p)(Ordered *, int, Tuple *);
	char *k;
} cmds[] = {
	{ parseuusys,	"uusys"		},
	{ parseinhost,	"inhost"	},
	{ parseinnet,	"innet"		},
	{ parsedb,	"include"	},
	{ 0,		(char *)0	},
};

/*
 *  list of all constant tuples and all data base tuples
 */
static Tuple *dblist;
static Tuple *constlist;

/*
 *  list of parsed files and their update times (up to 32 files)
 */
#define NF 32
struct File {
	File *next;
	char *name;
	long time;
};
File *flist;

/*
 *  Start parsing with the root of the parse files.  Return an
 *  ordered list of the entries.
 */
Ordered *
parsefiles(char *root, Ordered *o)
{
	File *fp, *next;

	/*
	 *  First see if any files have changed.
	 */
	if(checkfiles()==0)
		return o;

	/*
	 *  forget about old files
	 */
	for(fp = flist; fp; fp = next){
		free(fp->name);
		next = fp->next;
		delete fp;
	}
	flist = 0;

	/*
	 *  free old tuples
	 */
	freetuplelist(&dblist);
	freetuplelist(&constlist);

	/*
	 *  create a new ordered set
	 */
	if(o)
		delete o;
	o = new Ordered(12);

	parsefile(o, parsedb, root, (Tuple *)0, (char *)0);
	logevent("db parsed\n");
	return o;
}

/*
 *  see if any db files have changed.  return non-zero if they have.
 */
checkfiles()
{
	struct stat s;
	File *fp;

	if(flist==0)
		return 1;
	for(fp = flist; fp; fp = fp->next){
		if(stat(fp->name, &s)<0){
			if(fp->time) {
				logevent("%s disappeared\n", fp->name);
				return 1;
			}
		} else if(fp->time != s.st_mtime) {
			if(fp->time)
				logevent("%s changed\n", fp->name);
			else
				logevent("%s appeared\n", fp->name);
			return 1;
		}
	}
	return 0;
}

/*
 *  relative paths
 */
char *
path(char *cp, char *dp)
{
	static char path[MAXPATH];

	if(*cp=='/'){
		return cp;
	} else {
		strcpy(path, dp);
		strcat(path, "/");
		strcat(path, cp);
		return path;
	}
}

/*
 *  parse a file.  `parser' is the routine used to do the parsing.
 *  `consttuple' is a tuple of atributes to add to each tuple in the
 *  file.  `conststring' is a string of attributes to add to each
 *  tuple in the file.
 */
void
parsefile(
	Ordered *o,
	int (*parser)(Ordered *, int, Tuple *),
	char *file,
	Tuple *consttuple,
	char *conststring
)
{
	int fd;
	Tuple *ctp;
	char filebuf[sizeof(Fbuffer)];
	struct stat s;
	File *fp;
	char *pp;

	if(conststring){
		ctp = new Tuple(conststring, &constlist);
		ctp->cat(consttuple);
	} else
		ctp = consttuple;

	/*
	 *  remember the file as one of the db files
	 */
	pp = path(file, "/usr/ipc/lib/");
	for(fp = flist; fp; fp = fp->next)
		if(strcmp(fp->name, pp)==0)
			break;
	if(fp==0){
		fp = new File;
		fp->name = strdup(pp);
		fp->next = flist;
		flist = fp;
	}

	/*
	 *  open file and remember it's update time
	 */
	fd = open(pp, 0);
	if(fd<0) {
		fp->time = 0;
		logevent("can't open %s\n", file);
		return;
	}
	fstat(fd, &s);
	fp->time = s.st_mtime;

	/*
	 *  run the file through a parser
	 */
	Finit(fd, filebuf);
	if((*parser)(o, fd, ctp)<0) {
		logevent("error parsing %s\n", file);
		return;
	}
	close(fd);
}

/*
 *  Parse a data base file.  Anything not starting with a # is a tuple. # lines
 *  are of the formats:
 *	# xyz			- a comment
 *	#uusys file attributes	- a request to parse a uucp systems file.
 *				  the attributes are added to each line.
 *	#inhost file attributes	- a request to parse an internet hosts file.
 *				  the attributes are added to each line.
 *	#innet file attributes	- a request to parse an internet networks file.
 *				  the attributes are added to each line.
 *	#include file attributes	- a request to parse a file like this one.
 *				  the attributes are added to each line.
 */
parsedb(Ordered *o, int fd, Tuple *consttuple)
{
	char *cp;
	char *fields[4];
	struct cmdtbl *ctp;
	Tuple *t;

	while((cp = Frdline(fd))!=(char *)0){
		while(*cp==' ' || *cp=='\t')
			cp++;
		switch(*cp){
		case '\0':
			/*
			 *  a blank line is a comment
			 */
			break;
		case '#':
			/*
			 *  less than two fields after a # is a comment
			 */
			setfields("\t ");
			if(getmfields(cp+1, fields, 3)<2)
				break;

			/*
			 *  look for includes
			 */
			for(ctp=cmds; ctp->k; ctp++){
				if(strcmp(fields[0], ctp->k)==0){
					parsefile(o, ctp->p, fields[1],
						consttuple, fields[2]);
					break;
				}
			}

			/*
			 *  anything else is a comment
			 */
			break;
		default:
			t = new Tuple(cp, o, &dblist);
			t->cat(consttuple);
			break;
		}
	}
	return 0;
}

/*
 *  parse a hosts file
 */
parseinhost(Ordered *o, int fd, Tuple *consttuple)
{
	char *line;
	char *fields[16];
	int n, i;
	char attributes[MINLINE];
	Tuple *t;

	while((line=Frdline(fd))!=(char *)0){
		setfields(" \t");
		n = getmfields(line, fields, 16);
		if(n < 2 || *fields[0] == '#')
			continue;
		strcpy(attributes, fields[0]);
		strcat(attributes, ",in ");
		for(i=1; i<n; i++){
			strcat(attributes, fields[i]);
			if(strchr(fields[i], '.')!=(char *)0)
				strcat(attributes, ".,dom ");
			else
				strcat(attributes, " ");
		}
		t = new Tuple(attributes, o, &dblist);
		t->cat(consttuple);
	}
	return 0;
}

/*
 *  parse a networks file
 */
parseinnet(Ordered *o, int fd, Tuple *consttuple)
{
	char *line;
	char *fields[16];
	int n, i;
	char attributes[MINLINE];
	Tuple *t;

	while((line=Frdline(fd))!=(char *)0){
		setfields(" \t");
		n = getmfields(line, fields, 16);
		if(n < 2 || *fields[0] == '#')
			continue;
		strcpy(attributes, fields[1]);
		strcat(attributes, ",in ");
		strcat(attributes, fields[0]);
		strcat(attributes, " ");
		for(i=2; i<n; i++){
			strcat(attributes, fields[i]);
			strcat(attributes, " ");
		}
		t = new Tuple(attributes, o, &dblist);
		t->cat(consttuple);
	}
	return 0;
}

/*
 *  parse the systems file
 */
parseuusys(Ordered *o, int fd, Tuple *consttuple)
{
	char attributes[MINLINE];
	char *line, *cp, *type;
	char *fields[16];
	int n;
	Tuple *t, *ctp;

	static char lastn[32];
	static char lasta[32];
	static char *lastt;

	strcpy(attributes, "uucp,svc");
	ctp = new Tuple(attributes, &constlist);
	ctp->cat(consttuple);
	while((line=Frdline(fd))!=(char *)0){
		/*
		 *  parse a single line
		 */
		setfields(" \t");
		n = getmfields(line, fields, 16);
		if(n < 2 || *fields[0] == '#')
			continue;
		type = (char *)0;
		if(n>=5) {
			if(strncmp(fields[2], "DK", 2)==0){
				cp = strchr(fields[4], '.');
				if(cp!=(char *)0)
					*cp = '\0';
				type = ",dk";
			} else if(strncmp(fields[2], "ACU", 3)==0){
				type = ",tel";
			}
		}
		if(type==lastt && fstrcmp(lastn, fields[0])==0
		&& fstrcmp(lasta, fields[4])==0)
				continue;
		strcpy(attributes, fields[0]);
		if(type){
			strcat(attributes, " ");
			strcat(attributes, fields[4]);
			strcat(attributes, type);
		}
		lastt = type;
		strcpy(lastn, fields[0]);
		if(n>=5 && fields[4])
			strcpy(lasta, fields[4]);
		else
			lasta[0] = '\0';
		t = new Tuple(attributes, o, &dblist);
		t->cat(ctp);
	}
	return 0;
}
