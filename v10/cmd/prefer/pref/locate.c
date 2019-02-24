#include	<stdio.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"streams.h"
#include	"bib.h"

#define    maxrefs      500
#define LONGSTR	1280


char *strncat(), *strncpy(), *strcat(), *strcpy(), *strrchr();

#define ireturn(s,name)			\
{					\
	fprintf(stderr,s,name);		\
	nodatabase = 1;			\
        return(NULL);			\
}

struct reftype{
	char reffile[maxstr];
	long int start, length;
};

char *malloc();
char *stripkeys();
int   fetchref();

/*  locate(keys, name):
        Returns a string containing all references pointed to by name
        that contain all keys in keys.  Common is name of common word file.
	Pointer returned comes from malloc.  Use free to return storage.
	NB A zero length string returned if nothing is found.
	A NULL pointer indicates an error accessing the file "name".
*/
char *locate(keys,name)
char *keys, *name;
{
	static int	max_klen = MAXKLEN;	/* max key length */
	static char	*common	= COMFILE;	/* pointer to common file */
	static char	comstr[maxstr];		/* holds name of common file */

	static char	oldname[maxstr] = "";	/* oldname is name of stream index */
	static  FILE *index = NULL;
	static  long int i_size; 		/* size of index                   */
	static  char oldtext[maxstr];		/* oldtext is the path to stream   */
	static  FILE *text = NULL;		/*  text.  if it is a relative     */
	FILE	*head;

	static  int  pathlen;		/*  path, it is relative to index  */
	int  len;
	char key[maxstr];                   /* refs[i] is a line of index for  */
	char line[maxstr];
	struct reftype  refs[maxrefs];      /* all keys up to key */

	int  refcnt, copied, comp;          /* refcnt = # of refs */
	struct reftype ref;
	char   str[LONGSTR];
	char istr[LONGSTR];
	char tstr[maxstr], *tp;
	int    more;
	int outofdate;

	long int ans;
	int i,j;
	unsigned total;
	char *allrefs, *next;               /* all refs (separated by null line)*/
	char *p;

	static int nodatabase = 0;	/* cannot open or invert INDEX */

	struct stat ibuf, tbuf;	/* for comparing mtimes of index and text */
	/*  open index */
	if(strcmp(oldname,name)!=0) {
		nodatabase = 0;
		if (index) fclose(index);
		if (text) fclose(text);
		strcpy(oldname,name);
		strcpy(oldtext,"");
		/*  determine pathlen  */
		p= strrchr(oldname, '/');
		if (p!=NULL) pathlen= p-oldname+1;
		else pathlen= 0;

		*istr = NULL;
		if(stat(oldname,&ibuf) == -1) {		/* create index */
			strncpy(istr,oldname,strlen(oldname)-2);
			/* assume that bib database is named istr */
			if(makeindex(istr,istr,max_klen,common))
				ireturn("locate: cannot invert %s\n",istr);
		}
		else {		/* see if index is up to date */
			*str = NULL;
			strcpy(str,oldname);
			str[strlen(oldname)-1] = NULL;
			strcat(str,"h");
			if((head = fopen(str,"r")) == NULL)
				ireturn("locate: cannot open %s\n",str);
			if(fscanf(head,"%s %d\n",comstr,&max_klen) != 2) {
				ireturn("locate: %s is in the wrong format; rerun pinvert\n",str);
			}
			else {
				common = comstr;
			}
			*str = NULL;
			outofdate = 0;
			while(fgets(line,maxstr,head)) {
				line[strlen(line)-1] = NULL;
				if(*line != '/') {
					strncat(str, oldname, pathlen);
				}
				strcat(str,line);
				strcat(str," ");
				if(!outofdate) {
					if(*line != '/') {
						*tstr = NULL;
						strncat(tstr, oldname, pathlen);
						strcat(tstr,line);
						tp = tstr;
					}
					else tp = line;
					if(stat(tp,&tbuf) == -1)
						ireturn("locate: cannot stat %s\n",tp);
					if(tbuf.st_mtime > ibuf.st_mtime)
						outofdate++;
				}
			}
			fclose(head);
			if(outofdate) {
				if(*istr == NULL)
					strncpy(istr,oldname,strlen(oldname)-2);
				if(makeindex(str,istr,max_klen,common))
					ireturn("locate: cannot invert %s\n",str);
			}
		}
		if ((index= fopen(oldname,"r"))==NULL)
			ireturn("locate: cannot open %s\n", oldname);
		fseek(index,0L,2);     /*  seeks last newline      */
		i_size= ftell(index);
	}
	else	if/* database known to be unaccessible */
	(nodatabase) return(NULL);

	/*  load references to first key  */
	keys= stripkeys(keys,key, max_klen, common);
	if (*key==NULL) {
		allrefs = malloc(1);
		if (allrefs==NULL) {
			fprintf(stderr, 
			    "locate: insufficient space for references\n");
			exit(1);
		}
		*allrefs= NULL;
		return(allrefs);
	}
	len= strlen(key);
	strcat(key," ");
	alpha_seek(index, key, i_size, 0);
	key[len]= NULL;                     /*  strip blank off */

	refcnt= 0;
	fscanf(index,"%s ", str);
	if (strcmp(str,key) == 0) {
		str[0]= NULL;
		while (refcnt < maxrefs && fetchref(index, str, &ref) ) {
			refs[refcnt]= ref;
			refcnt++;
		}
	}

	if (refcnt==maxrefs)
		fprintf(stderr,
		    "locate: first key (%s) matched too many refs\n", key);

	/*  intersect the reference sets for remaining keys with first set */
	while (*keys!=NULL) {
		keys= stripkeys(keys, key, max_klen, common);
		if (*key==NULL) continue;

		len= strlen(key);
		strcat(key," ");
		alpha_seek(index, key, i_size, 0);
		key[len]= NULL;

		fscanf(index,"%s ", str);
		if (strcmp(str,key) != 0)  refcnt= 0;   /*  no matching refs */

		copied= 0;
		comp= 0;
		more= fetchref(index, str, &ref);
		while (comp < refcnt && more) {   /*  ans= ref-refs[comp]    */
			ans= strcmp(ref.reffile, refs[comp].reffile);
			if (ans==0)     ans= ref.start-refs[comp].start;
			if (ans==0)     ans= ref.length-refs[comp].length;
			if (ans<0)  more= fetchref(index, str, &ref);
			if (ans==0) {
				refs[copied]= refs[comp];
				comp++;
				copied++;
				more= fetchref(index, str, &ref);
			}
			if (ans>0)  comp++;
		}

		refcnt= copied;
	}

	total= 0;
	for (i=0; i<refcnt; i++)    total += refs[i].length+1;

	allrefs= malloc(total+1);
	if (allrefs==NULL) {
		fprintf(stderr, "locate: insufficient space for references\n");
		exit(1);
	}

	/* copy refs into allrefs */
	next= allrefs;
	for (i=0; i<refcnt; i++) {   /*  open text */
		if (strcmp(oldtext,refs[i].reffile) != 0) {
			strcpy(oldtext,refs[i].reffile);
			if (oldtext[0]=='/') {   /* absolute path */
				strcpy(str,oldtext);
			} else {   /* relative name */
				strncpy(str, oldname, pathlen);
				str[pathlen]= NULL;
				strcat(str, oldtext);
			}
			if (text) fclose(text);
			text= fopen(str, "r");
			if (text==NULL) {
				fprintf(stderr, "locate: cannot open %s\n", str);
				strcpy(oldtext, "");
				return(NULL);
			}
		}
		fseek(text, refs[i].start, 0);
		for (j=0; j<refs[i].length; j++)    *next++ = getc(text);
		*next++ = '\n';
	}
	*next = NULL;
	return(allrefs);
}



/*  stripkeys(line,key,max_klen, common):
        assigns to key the first key in line
        and returns a pointer to the position following the key
*/
char *stripkeys(line,key,max_klen,common)
char *line, *key;
int  max_klen;
char *common;
{
	char *p;

	do {
		while (*line && !isalnum(*line))   line++;

		p= key;
		while (isalnum(*line))
			*p++ = *line++;
		*p= NULL;
		if(*key == NULL)
			break;
		makekey(key, max_klen, common);
	} while (*key == NULL);
	return(line);
}

/*  read a reference pair from stream into *ref.  if file not given,
    use oldfile. return 1 if pair found, 0 ow.
*/
int fetchref(stream, oldfile, ref)
FILE *stream;
char *oldfile;
struct reftype *ref;
{
	char cntl;

	fscanf(stream, "%c", &cntl);
	if (cntl=='\n') {
		return (0);
	}
	if (cntl==':')  fscanf(stream, "%s", oldfile);
	strcpy(ref->reffile, oldfile);
	fscanf(stream, "%ld/%ld", &ref->start, &ref->length);
	return(1);
}



char pinvert[] = "pinvert -l%d -c%s -p%s %s";

makeindex(names,index,k,com)
char *names,*com,*index;
int k;
{
	char pcmd[LONGSTR];

	sprintf(pcmd,pinvert,k,com,index,names);
	return(system(pcmd));
}



