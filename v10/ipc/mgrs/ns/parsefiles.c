#include <fio.h>
#include <libc.h>
#include <string.h>
#include <sysent.h>
#include <sys/types.h>
#include <sys/stat.h>
extern "C" int stat (const char*, struct stat*);	/* added by pg so that
							 * this could compile
							 */
extern "C" char *memset(char*, int, int);
extern "C" long time(long *);
extern "C" long in_aton(char *);
extern "C" char *in_ntoa(long);

#include "dbtypes.h"

#define MINLINE 256

/*
 * list of information about each open database (up to NDB of them)
 * the system database is always at index 0
 */

#define	NDB	64
#define SYSDB	0

Db *db[NDB];

/*
 * predeclared
 */
int parseuusys(Db *, int, Tuple *);
int parseinhost(Db *, int, Tuple *);
int parseinnet(Db *, int, Tuple *);
int parsedb(Db *, int, Tuple *);
void parsefile(Db *, int (*)(Db *, int, Tuple *), char *, Tuple *, char *);
void clearDb(Db *);
void releaseDb(Db *);
int checkfiles(Db *);

/*
 *  command table
 */
struct cmdtbl {
	int (*p)(Db *, int, Tuple *);
	char *k;
} cmds[] = {
	{ parseuusys,	"uusys"		},
	{ parseinhost,	"inhost"	},
	{ parseinnet,	"innet"		},
	{ parsedb,	"include"	},
	{ 0,		(char *)0	},
};

/*
 * Return a pointer to a loaded database for *uid.  If (uid == 0), return a
 * pointer to the system database.  If the database is not loaded, return
 * (Db *)0.  *Hint provides the array location of the last user database,
 * and is updated for each user database created/found.
 */
Db *
finddb(int *uid, int *hint)
{
	int uidval;
	int dbindex;
	int hintval = *hint;
	Db *d = 0;

	if (uid) {
		uidval = *uid;
		/*
		 * See if it's the same db as last time
		 */
		if ((hintval > SYSDB) && (hintval < NDB) && (db[hintval]) &&
			(db[hintval]->uid == uidval))
			d = db[hintval];
		else
			/*
			 * Look for user db 
			 */
			for (dbindex = SYSDB + 1; dbindex < NDB; dbindex++) {
				if (db[dbindex]) {
					if (uidval == db[dbindex]->uid) {
						*hint = dbindex;
						d = db[dbindex];
						break;
					}
				}
			}
	} else
		d = db[SYSDB];

	if (d)
		d->lasttime = time ((long *) 0);
	return d;
}

/*
 * Load the database in rootdir/filename.  If uid == 0, load the database
 * as the system database; otherwise, load the database for user with *uid.
 * Return a pointer to the database loaded.  If the database is a user
 * database, update *hint with the array index of the database.
 */
Db *
newdb(int *uid, char *rootdir, char *filename, char *dbname, int *hint)
{
	int oldindex;
	int dbindex;
	long oldtime;
	int uidval = 0;
	Db *d;

	if (uid) {
		/*
		 *  Find the first free slot or the oldest slot
		 */
		uidval = *uid;
		oldindex = SYSDB + 1;
		if (db[oldindex]) {
			oldtime = db[oldindex]->lasttime;
			for (dbindex = oldindex; dbindex < NDB; dbindex++) {
				if (!db[dbindex])
					break;
				if (db[dbindex]->lasttime < oldtime) {
					oldindex = dbindex;
					oldtime = db[dbindex]->lasttime;
				}
			}
		}
		if (db[oldindex])
			releaseDb(db[oldindex]);
		dbindex = oldindex;
		*hint = dbindex;		
	} else
		dbindex = SYSDB;

	if (!(d = db[dbindex])) {
		db[dbindex] = d = new Db;
		memset((char *)d, 0, sizeof(Db));
	}

	d->uid = uidval;
	d->rootdir = strdup(rootdir);
	d->filename = strdup(filename);
	d->dbname = strdup(dbname);
	d->lasttime = time((long *)0);
	return d;
}

/*
 *  Start parsing with the root of the parse files.  Return an
 *  ordered list of the entries.
 */
void
parsefiles(Db *d)
{
	char	dbattr[MAXDBNAME+4];

	clearDb(d);
	d->o = new Ordered(12);
	strcpy(dbattr,d->dbname);
	strcat(dbattr,",db");
	parsefile(d, parsedb, d->filename, (Tuple *)0, dbattr);
	d->origin = lookup("local,origin", d->o);
	logevent("db %s: parsed from %s/%s\n",d->dbname,d->rootdir,d->filename);
}

/*
 * Release all databases.  System database is cleared, so it will be
 * reparsed by main loop.
 */
void
releasedbs()
{
	int dbindex;

	clearDb(db[SYSDB]);
	for (dbindex = SYSDB+1; dbindex < NDB; dbindex++) {
		if (db[dbindex]) {
			releaseDb(db[dbindex]);
			delete db[dbindex];
			db[dbindex] = 0;
		}
	}
}

/*
 * Completely release all space occupied by a database
 */
void
releaseDb(Db *db)
{
	if (db->rootdir)
		free(db->rootdir);
	if (db->filename)
		free(db->filename);
	if (db->dbname)
		free(db->dbname);
	db->rootdir = db->filename = db->dbname = (char *) 0;
	clearDb(db);
}

/*
 * Clear database data structures to prepare for reparsing the database
 * from the same root db file
 */
void
clearDb(Db *db)
{
	File *fp, *next;

	/*
	 *  forget about old files
	 */
	for(fp = db->flist; fp; fp = next){
		if (fp->name) free(fp->name);
		next = fp->next;
		delete fp;
	}
	db->flist = 0;

	/*
	 *  free old tuples
	 */
	freetuplelist(&db->dblist);
	freetuplelist(&db->constlist);

	/*
	 *  Delete the old set
	 */
	if(db->o)
		delete db->o;
	db->o = (Ordered *)0;

	/*
	 * Delete the origin
	 */
	if (db->origin)
		delete db->origin;
	db->origin = (Set *)0;
}

/*
 *  see if any db files have changed.  return non-zero if they have.
 */
int
checkfiles(Db *d)
{
	struct stat s;
	File *fp;

	if(d->flist==0)
		return 1;
	for(fp = d->flist; fp; fp = fp->next){
		if(stat(fp->name, &s)<0){
			if(fp->time) {
				logevent("db %s: %s disappeared\n", d->dbname,
					fp->name);
				return 1;
			}
		} else if(fp->time != s.st_mtime) {
			if(fp->time)
				logevent("db %s: %s changed\n", d->dbname, 
					fp->name);
			else
				logevent("db %s: %s appeared\n", d->dbname,
					fp->name);
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
	Db	*d,
	int (*parser)(Db *, int, Tuple *),
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
		ctp = new Tuple(conststring, &d->constlist);
		ctp->cat(consttuple);
	} else
		ctp = consttuple;

	/*
	 *  remember the file as one of the db files
	 */
	pp = path(file, d->rootdir);
	for(fp = d->flist; fp; fp = fp->next)
		if(strcmp(fp->name, pp)==0)
			break;
	if(fp==0){
		fp = new File;
		fp->name = strdup(pp);
		fp->next = d->flist;
		d->flist = fp;
	}

	/*
	 *  open file and remember it's update time
	 */
	fd = open(pp, 0);
	if(fd<0) {
		fp->time = 0;
		logevent("db %s: can't open %s\n", d->dbname, pp);
		return;
	}
	fstat(fd, &s);
	fp->time = s.st_mtime;

	/*
	 *  run the file through a parser
	 */
	logevent("db %s:parsing %s\n", d->dbname, file);
	Finit(fd, filebuf);
	if((*parser)(d, fd, ctp)<0) {
		logevent("db %s:error parsing %s\n", d->dbname, file);
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
parsedb(Db *d, int fd, Tuple *consttuple)
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
					parsefile(d, ctp->p, fields[1],
						consttuple, fields[2]);
					break;
				}
			}

			/*
			 *  anything else is a comment
			 */
			break;
		default:
			t = new Tuple(cp, d->o, &d->dblist);
			t->cat(consttuple);
			break;
		}
	}
	return 0;
}

/*
 *  parse a hosts file
 */
parseinhost(Db *d, int fd, Tuple *consttuple)
{
	char *line;
	char *fields[16];
	int n, i, len;
	char attributes[MINLINE];
	Tuple *t;

	while((line=Frdline(fd))!=(char *)0){
		setfields(" \t");
		n = getmfields(line, fields, 16);
		if(n < 2 || *fields[0] == '#')
			continue;
		strcpy(attributes, fields[0]);
		strcat(attributes, ",in ");
		len = strlen(attributes);
		for(i=1; i<n; i++){
			len += strlen(fields[i]) + 6;
			if(len >= MINLINE)
				break;
			strcat(attributes, fields[i]);
			if(strchr(fields[i], '.')!=(char *)0)
				strcat(attributes, ".,dom ");
			else
				strcat(attributes, " ");
		}
		t = new Tuple(attributes, d->o, &d->dblist);
		t->cat(consttuple);
	}
	return 0;
}

/*
 *  parse a networks file
 */
parseinnet(Db *d, int fd, Tuple *consttuple)
{
	char *line;
	char *fields[16];
	int n, i, len;
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
		len = strlen(attributes);
		for(i=2; i<n; i++){
			len += strlen(fields[i]) + 1;
			if(len >= MINLINE)
				break;
			strcat(attributes, fields[i]);
			strcat(attributes, " ");
		}
		t = new Tuple(attributes, d->o, &d->dblist);
		t->cat(consttuple);
	}
	return 0;
}

/*
 *  parse the systems file
 */
parseuusys(Db *d, int fd, Tuple *consttuple)
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
	ctp = new Tuple(attributes, &d->constlist);
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
		t = new Tuple(attributes, d->o, &d->dblist);
		t->cat(ctp);
	}
	return 0;
}
