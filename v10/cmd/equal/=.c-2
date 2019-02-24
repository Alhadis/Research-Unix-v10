/*% cc -O %
 * =, == -- redo commands from history
 */
#include <stdio.h>
#define	TRUE	1
#define	FALSE	0
#define LBSIZE	512

char *malloc(), *realloc();

#define Malloc(type,size)	((type *)malloc((size)*sizeof(type)))
#define Realloc(type,ptr,size)	((type *)realloc(ptr,(size)*sizeof(type)))

char linebuf[LBSIZE];
char *linebp;
char *getenv();
char *cmd;
char genbuf[LBSIZE];
#define	RHSIZE	512
char rhsbuf[RHSIZE];
int nextcol(col,cp,input)
register col;
register char *cp;
int input;
{
	register char c;
	c = *cp;
	if (c=='\t')
		col |= 07;
	else if (c<' ' || c=='\177')
		error("Invalid character in command");
	return (++col);
}
xform(hfile)
FILE *hfile;
{
	register char *i, *m, *o;
	int *line, insert, ic, mc, c;
	char *tf, *tl;
	for(;;){
		dumpline();
		m=rhsbuf;
		while ((c = getchar())!='\n') {
			if (c == EOF)
				exit(0);
			*m++ = c;
			if (m==rhsbuf+RHSIZE-1)
				error("Out of space");
		}
		*m='\0';
		if (m==rhsbuf)
			break;
		if (rhsbuf[0] == '=' && rhsbuf[1] == 0) {
			prevline(hfile);
			continue;
		}
		i=linebuf;
		o=genbuf;
		do ; while (*o++ = *i++);
		if (i+(m-rhsbuf) > linebuf+LBSIZE)
			error("Out of space");
		i=genbuf;
		o=linebuf;
		m=rhsbuf;
		insert=FALSE;
		ic=0;
		mc=0;
		while (*i && *m && !insert) {
			if(*i=='\t' && *m!='#' && *m!='^' && *m!='$') {
				ic=nextcol(ic,i,FALSE);
				tf=m;
				tl=m;
				do {
					if (*m!=' ' && *m!='\t') {
						if(*m=='%')
							*m=' ';
						tl=m+1;
					}
					mc=nextcol(mc,m++,TRUE);
				} while (ic>mc && *m && *m!='#' &&
					 *m!='^' && *m!='$');
				if (ic>mc) {
					ic=mc;
					if (*m)
						tl=m;
				} else {
					if (tl==m)
						i++;
					else
						ic--;
				}
				while (tf!=tl)
					*o++ = *tf++;
			} else {
				mc=nextcol(mc,m,TRUE);
				*o = *m;
				switch (*m++) {
				case ' ':
				case '\t':
					break;
				case '^':
					mc=ic;
					insert++;
					break;
				case '$':
					i="";
					break;
				case '#':
					ic=nextcol(ic,i++,FALSE);
					while(*m=='#' && ic>mc)
					      mc=nextcol(mc,m++,TRUE);
					if (ic!=mc)
						error("Partly deleted tab");
					break;
				case '%':
					*o = ' ';
					/* fall through */
				default:
					o++;
					ic=nextcol(ic,i++,FALSE);
				}
			}
			for (;;) {
				if (ic>mc && *m) {
					if (*m!=' ' && *m!='\t')
						error("Skipped non-blank");
					mc=nextcol(mc,m++,TRUE);
				} else if (mc>ic && *i) {
					ic=nextcol(ic,i,FALSE);
					*o++ = *i++;
				} else
					break;
			}
		}
		if (mc>ic && m[-1]=='\t')
			*o++ = '\t';
		while (*m)
			*o++ = *m++;
		do ; while (*o++ = *i++);
	}
}
error(s)
char *s;
{
	fprintf(stderr, "%s: %s\n", cmd, s);
	exit(1);
}
char *histname;
main(argc, argv)
char *argv[];
{
	register FILE *f; FILE *getline();
	register nsubst, i;
	char *strchr();
	int edit=argv[0][1]!='\0';
	cmd=argv[0];
	for(nsubst=0;argc>1 && strchr(argv[argc-1], cmd[0])!=NULL;nsubst++){
		--argc;
		edit=0;
	}
	histname=getenv("HISTORY");
	f = getline(argc, argv);
	for(i=0;i!=nsubst;i++)
		alter(argv[argc+i]);
	if(edit)
		xform(f);
	else
		dumpline();
	fclose(f);
	if((f=fopen(histname, "a"))!=NULL){
		fprintf(f, "%s\n", linebuf);
		fclose(f);
	}
	execl("/bin/sh", "sh", "-c", linebuf, (char *)0);
	error("No shell!\n");
}
dumpline(){
	/* write on 2 directly for speed */
	write(2, linebuf, strlen(linebuf));
	write(2, "\n", 1);
}
/*
 * Look at $HISTORY.  If argc==1 get the last non-blank line in the file.
 * Otherwise, argv[1] is a pattern to match against the lines of the history
 * file.  The last matching line wins.
 */
FILE *
getline(argc, argv)
char *argv[];
{
	char history[LBSIZE];
	char pat[LBSIZE];
	register char *hp;
	register FILE *f;
	register i;
	int nmatch=0, ntell;
	if(histname==NULL)
		error("Environment lacks HISTORY\n");
	if((f=fopen(histname, "r"))==NULL){
		perror(histname);
		exit(1);
	}
	pat[0]='\0';
	for(i=1;i!=argc && argv[i][0]!=cmd[0];i++){
		strcat(pat, argv[i]);
		strcat(pat, " ");
	}
	pat[strlen(pat)-1]='\0';	/* annul the extra space */
	while(ntell=ftell(f), fgets(history, LBSIZE, f)){
		/*
		 * Skip leading blanks or tabs
		 */
		for(hp=history;*hp==' '||*hp=='\t';hp++)
			;
		if(anyequals(hp))
			continue;
		/*
		 * Welcome to the land of snakey logic
		 */
		if(pat[0]=='\0'?!empty(hp):match(hp, pat)){
			linesave(ntell, nmatch++);
			strncpy(linebuf, hp, LBSIZE);
		}
	}
	if(nmatch==0)
		error("Can't find a line to redo");
	linebuf[strlen(linebuf)-1]='\0';	/* delete a newline */
	return f;
}

#define VECSIZE	512
int *matchvec, *matchend, *lastmatch, *lastuniq, vecsize;

linesave(adr, i)
{
	if (matchvec == 0)
		matchend = matchvec = Malloc(int, vecsize = VECSIZE);
	if (i >= vecsize)
		matchvec = Realloc(int, matchvec, vecsize += VECSIZE);
	if (matchvec == 0)
		return;
	*matchend++ = adr;
}

prevline(f)
FILE *f;
{
	char history[LBSIZE]; char *strchr(), *savestr();
	register char *hp; register int *ip;
	if (matchvec == 0)
		return;
	if (lastmatch == 0)
		lastuniq = matchend, lastmatch = --matchend;
	*--lastuniq = (int)savestr(linebuf);
	do {
		if (--lastmatch < matchvec) {
			matchvec = 0;
			return;
		}
		fseek(f, *lastmatch, 0);
		fgets(history, LBSIZE, f);
		for(hp=history;*hp==' '||*hp=='\t';hp++)
			;
		*strchr(hp,'\n') = '\0';
		for (ip=lastuniq; ip <= matchend; ip++)
			if (strcmp((char *)(*ip), hp) == 0) break;
	} while (ip <= matchend);
	strncpy(linebuf, hp, LBSIZE);
}

char *
savestr(str)	/* Place string into permanent storage. */
register char *str;
{
	static int nchleft; static char *strpt;
	register int len; char *strcpy();

	if ((len = strlen(str)+1) > nchleft) {
		strpt = malloc(nchleft = 2*LBSIZE);
	}
	if (strpt == 0)
		return 0;
	str = strcpy(strpt, str);
	strpt += len; nchleft -= len;
	return str;
}

/*
 * Throw away any command with an = or == word
 */
anyequals(line)
	register char *line;
{
	register char *p, *q;
	for(p=line; *p; p++)
		if(*p=='=' && (p==line || p[-1]==' ' || p[-1]=='\t')){
			if(p[1]=='=')
				p++;
			if(p[1]=='\n' || p[1]==' ' || p[1]=='\t')
				return 1;
		}
	return 0;
}
match(s, p)
register char *s, *p;
{
	register char *h;
	for(h=s;*s!='\n' && *s!=' ' && *s!='\t' && *s!='\0';s++)
		if(*s=='/')
			h=s+1;
	return(startis(h, p));
}
startis(s, p)
register char *s, *p;
{
	while(*p!='\0')
		if(*p==' '){
			if(*s!=' ' && *s!='\t')
				return(0);
			while(*s==' ' || *s=='\t')
				s++;
			p++;
		}
		else if(*s++ != *p++)
			return(0);
	return(1);
}
empty(s)
register char *s;
{
	if(*s==cmd[0])
		return(1);
	while(*s!='\0'){
		if(*s!=' ' && *s!='\t' && *s!='\n')
			return(0);
		s++;
	}
	return(1);
}
alter(pat)
register char *pat;
{
	register char *sub, *s=linebuf, *t=genbuf;
	int patlen, sublen, matched=0;
	for(sub=pat;*sub!=cmd[0] && *sub!='\0';sub++);
	if(*sub!=cmd[0])
		error("Bad Substitution");
	patlen=sub-pat;
	if(*sub==cmd[0])
		*sub++='\0';
	sublen=strlen(sub);
	while(*s)
		if(!matched && startis(s, pat)){
			matched++;
			s+=patlen;
			strcpy(t, sub);
			t+=sublen;
		}
		else
			*t++ = *s++;
	if(!matched)
		error("No pattern match");
	*t='\0';
	strcpy(linebuf, genbuf);
}
