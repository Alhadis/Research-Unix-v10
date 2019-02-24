/**********
				docgen.c

	(091984 version: from llc 080284; minor changes by vbt)

	modify *mmfile, *msfile, and *mcfile strings as needed to supply
		full pathnames for mmdata, msdata, and mcsdata files;

**********/
#include <stdio.h>
#include <signal.h>
char *mmfile="/usr/lib/docgen/mmdata";
char *msfile="/usr/lib/docgen/msdata";
char *mcfile="/usr/lib/docgen/mcsdata";
char *wr = "/usr/lib/docgen/wr";
#define eatblanks(a) while((c=getc(a)) == ' '|| c == '\t')
#define MAX 100
#define CMAX 7500
#define LMAX 200
#define QMAX 100
#define NPATH 10
#define NENT 200
#define BUFS 512
#define LABMAX 20
#define STACK 10
#define NEST 10

int inap;
int debug = 0;
int sdebug = 0;
int verbose = 0;
#define ARGS 1
#define ONELINE 2
#define MANY 4
#define OUT 8
#define PRINT 16
#define SKIP 32
#define BLOOP 128
#define PATH 256
#define QUEST 512
#define STUFF 1024
#define NAME 2048
#define SUM 4096
#define PAR 8192
#define FLOOP 16384
#define SEVERAL	32768
#define QSTUFF 65536

struct q_a {
	char *qtext;
	char *text;
	char *macro;
	unsigned long type;
	struct qa *argq;
}  s[MAX];
struct q_a *p = s;

unsigned long gettype();
int sig_in_ed();
char buf[CMAX];
char *ptr = buf;
char obuf[BUFS];
char *optr = obuf;
FILE *outp;
char *ofile;
char line[LMAX];
char sline[LMAX];
char *first;
int lineno = 1;

struct qa {
	char *text;
	struct qa *nexta;
} ques[QMAX];
struct qa *qp = ques;

struct pa {
	char *name;
	struct list *root;
} paths[NPATH];
struct pa *pp = paths;

struct list {
	struct q_a *entry;
	struct list *next;
	struct list *loop;
	char *quest;
	struct list *floop;
} list[NENT];
struct list *lp = list;
struct list *stack[STACK];
struct list **st = stack;

struct pa *findpath();
struct lab {
	struct list *path;
	struct list *place;
} save[LABMAX], *sav=save;
struct lab *savp;
struct lab *savc[10];
unsigned stype[10];
int curly = 0;
char *onepath;
char *qfile;
char *namesf="tagfile";
char nname[50];
FILE *popen(), *edp;

main(argc,argv)
int argc;
char *argv[];
{

	qfile = mcfile;
	while(argc > 1 && *argv[1] == '-'){
		switch(argv[1][1]){
		case 'f':
			if(argc > 2)qfile=argv[2];
			argc--;
			argv++;
			break;
		case 'm':
			if(argv[1][2] == 's')qfile=msfile;
			else if(argv[1][2] == 'c')qfile=mcfile;
			else if(argv[1][2] == 'm')qfile=mmfile;
			else {
				fprintf("unknown macro package %s\n",argv);
				exit(0);
			}
			break;
		case 'w':
			qfile = wr;
			break;
		case 'd':
			sdebug=1;
			break;
		case 'p':
			debug=1;
			break;
		case 'v':
			verbose=1;
			break;
		default:
			fprintf(stderr,"unknown flag %c\n",argv[1][1]);
			exit(0);
		}
		argc--; argv++;
	}
	if(argc > 1)ofile=argv[1];
	init(qfile);
	if(onepath != 0){
		pp=findpath(onepath);
		lp=pp->root;
		process(1);
	}
	else process(0);
	done();
}
init(file)
char *file;
{
	FILE *inp;
	int c;
	int i;
	int printall=0;
	char command[50];
	FILE *dumb;

	if(ofile == 0){
		if(file == wr){
			printf("Tags going to file tagfile\n");
			ofile = namesf;
		}
		else if(file == mcfile){
			printf("Output going to file temp.cover\n");
			ofile = "temp.cover";
		}
		else {
			printf("Output going to file temp\n");
			ofile = "temp";
		}
	}
	if((inp=fopen(file,"r"))==NULL){
		fprintf(stderr,"can't open script file %s\n",file);
		exit(0);
	}
	pp->name = onepath = ptr;
	i=0;
	while((c=getc(inp)) != '\n'){
		if(c == ','){
			*ptr++ = '\0';
			(++pp)->name = ptr;
			i++;
			if(pp > &paths[NPATH-1])
				error("more than 10 paths-NPATH");
		}
		else if(c != ' ' && c != '\t')
			*ptr++ = c;
	}
	*ptr++ = '\0';
	if(i > 0)onepath=0;
	lineno++;
	first = ptr;
	if(getstr(inp)== 0)
		if(onepath == 0)
			error("text string expected");
	while(getentry(inp) != 0){
		if(p++ >= &s[MAX-1])
			error("number of instructions exceeded-MAX");
	}
	if(debug)printf("\n");
	if((dumb = fopen(ofile,"r")) != NULL){
		if(file == wr){
		printf("File %s already exists; Tags will be appended\n",ofile);
		printall++;
		}
		else {
			printf("File %s already exists; Text will be appended\n",ofile);
		}
		fclose(dumb);
	}
	if((dumb = fopen(ofile,"a")) == NULL){
		fprintf(stderr,"can't open %s\n",ofile);
		exit(1);
	}
	fclose(dumb);
	sprintf(command,"ed - %s\n",ofile);
	signal(SIGINT,sig_in_ed);
	if((edp = popen(command,"w")) == NULL){
		printf("can't involk ed\n");
		exit(0);
	}
	setbuf(edp,NULL);
	if(printall){
		printf("The following tags are already defined:\n");
		fputs("1,$p\n",edp);
		fflush(edp);
		sleep(8);
		printf("\n");
	}
	fputs("a\n",edp);
	inap = 1;
}

getentry(inp)
FILE *inp;
{
	char *l;
	int c;
	int comment = 0;

	if(debug)printf("\n place %o ",p);
	savp = sav;
	l = line;
	while((*l = c = getc(inp)) != ':'){
		if(c == EOF)return(0);
		if(c == '*'){
			if(comment){
				comment=0;
				continue;
			}
			else {
				comment=1;
				continue;
			}
		}
		if(comment){
			if(c == '\n')lineno++;
			continue;
		}
		if(c == '\\'){
			*l = getc(inp);
			if(++l >= &line[LMAX-1])
				error("line length exceeded-LMAX");
			continue;
		}
		if(c == '}'){
			if(debug)printf("found }\n");
			l=ptr;
			getstr(inp);
			addloop(--curly,l);
			eatline(inp);
			sav = savc[curly];
			p->type = BLOOP;
			return(1);
		}
		if(c == ','){
			*l = '\0';
			addentry(line,p);
			l=line;
		}
		else if(c == '\n'){
			l=line;
			lineno++;
		}
		else if(c != ' ' && c != '\t'){
			if(l++ >= &line[LMAX-1])
				error("line length exceeded-LMAX");
		}
	}
	*l = '\0';
	addentry(line,p);
	p->macro = ptr;
	while((*ptr = c = getc(inp))!= ':'){
		if(c == '\\'){
			*ptr = getc(inp);
			if(++ptr >= &buf[CMAX-1])
				error("string buffer exceeded-CMAX");
		}
		else if(c == ' ' && ptr == p->macro);
		else if(c == '{' && ptr == p->macro){
			if(debug)printf("in { savp %o\n",savp);
			(sav++)->path = 0;
			stype[curly] = BLOOP;
			savc[curly++] = savp;
			if(curly >= NEST)
				error("nesting depth exceeded-NEST");
			if((c = getc(inp)) == ' '|| c == '\t')
				eatblanks(inp);
			if(c == '\n'){
				lineno++;
				if((c = getc(inp)) == ' '|| c == '\t')
					eatblanks(inp);
			}
			if(c == '"'){
				p->type = FLOOP;
				ungetc(c,inp);
				l=ptr;
				getstr(inp);
				p->qtext = l;
				stype[curly-1] = FLOOP;
			}
			else {
				ungetc(c,inp);
				p->type = BLOOP;
			}
			return(1);
		}
		else if(ptr++ >= &buf[CMAX-1])
			error("string buffer exceeded-CMAX");
	}
	if(p->macro == ptr)p->macro=0;
	else *ptr++ = '\0';
	p->type = gettype(inp);
	if(p->type&QUEST){
		p->qtext = ptr;
		if(getstr(inp) == 0)
			error("text string expected");
	}
	else p->qtext = 0;
	p->text = ptr;
	if(p->type&(ONELINE|SEVERAL|PRINT|MANY|PATH))
		if(getstr(inp)== 0)
			error("text string expected");
	if(p->text == ptr)p->text = 0;
	if(debug)printf(" macro %s type %d text %s ",p->macro,p->type,p->text);
	if(sdebug)db(p);
	if(p->type & (ARGS |SKIP)){
		p->argq = qp;
		qp->text = ptr;
		while(getstr(inp) != 0){
			qp->nexta = (qp+1);
			if(debug||sdebug)printf("\n	arg question= %s",qp->text);
			if(debug)printf(" next %o",qp->nexta);
			if(qp++ >= &ques[QMAX-1])
				error("number of questions exceeded-QMAX");
			qp->text = ptr;
		}
		(qp-1)->nexta = 0;
		if(debug)printf("\n	last %s next %o",(qp-1)->text,(qp-1)->nexta);
		if(sdebug)printf("\n");
	}
	sav = savp;
	return(1);
}
getstr(inp)
FILE *inp;
{
	int c;
	while((c=getc(inp)) == ' ' || c == '\t' || c == '\n')if(c == '\n')lineno++;
	if(c != '\"'){
		ungetc(c,inp);
		return(0);
	}
	while((*ptr=c=getc(inp)) != '\"'){
		if(ptr++ >= &buf[CMAX])
			error("string buffer exceeded-CMAX");
		if(c == '\n')lineno++;
	}
	*ptr++ = '\0';
	return(1);
}
addentry(ch,p)
char *ch;
struct q_a *p;
{
	struct list *ll;
	if(debug || sdebug)printf("path= %s, ",ch);

	for(pp=paths;pp->name != 0 && pp < &paths[NPATH];pp++){
		if(match(ch,pp->name)){
			if(pp->root == 0){
				pp->root = lp;
			}
			else {
				ll=pp->root;
				while(ll->next != 0)ll=ll->next;
				ll->next = lp;
			}
			if(debug)printf("entry %o path %o\n",lp,pp);
			sav->path = (struct list *)pp;
			sav->place = lp;
			lp->entry = p;
			if(lp++ >= &list[NENT-1])
				error("tree exceeded-NENT");
			if(sav++ >= &save[LABMAX-1])
				error("saved paths exceeded-LABMAX");
			return;
		}
	}
	printf("no path to %s\n",ch);
	exit(0);
}

addloop(in,cp)
char *cp;
{
	struct lab *sp;
	struct list *ll;

	sp = savc[in];
	while((ll = sp->path) != 0){
		if(debug)printf("addloop sp %o path %o",sp,sp->path);
		while(ll->next != 0){
			ll=ll->next;
		}
		if(debug)printf("ll %o place %o\n",ll,sp->place);
		if(ll->loop != 0){
			ll->next = lp;
			lp->loop = sp->place;
			lp->quest = cp;
			lp->entry = p;
			if(lp++ >= &list[NENT-1])
				error("tree exceeded - NENT");
		}
		else {
			ll->loop = sp->place;
			ll->quest = cp;
		}
		if(stype[in] == FLOOP){
			sp->place->floop = lp;
			lp->entry = p;
			if(lp++ >= &list[NENT-1])
				error("tree exceeded - NENT");
		}
		sp++;
	}
}
unsigned long
gettype(inp)
FILE *inp;
{
	int c;
	unsigned long i;
	i=0;
another:
	while((c=getc(inp)) == ' ' || c == '\t');
	switch(c){
	case 'A':
		i |= ARGS;
		break;
	case 'M':
		i |= MANY;
		break;
	case 'O':
		if((c=getc(inp)) == 'U')
			i |= OUT;
		else if(c == 'N')
			i|= ONELINE;
		else error("unknown action");
		break;
	case 'P':
		if((c=getc(inp)) == 'R')
			i |= PRINT;
		else if(c == 'A'){
			if((c=getc(inp)) == 'T')
				i |= PATH;
			else if(c == 'R')
				i |= PAR|MANY;
			else error("unknown action");
		}
		else error("unknown action");
		break;
	case 'S':
		if((c=getc(inp)) == 'T')
			i |= STUFF|ARGS;
		else if(c == 'K')
			i |= SKIP;
		else if(c == 'U')
			i |= SUM|MANY;
		else if(c == 'E')
			i |= SEVERAL;
		else error("unknown action");
		break;
	case 'Q':
		if((c=getc(inp)) == 'U')
			i |= QUEST;
		else if(c == 'S')
			i |= QSTUFF|ARGS;
		else error("unknown action");
		break;
	case 'N':
		i |= NAME|ONELINE;
		break;
	default:
		error("unknown action");
	}
	while((c=getc(inp))!= '\n')
		if(c == '&')goto another;
	lineno++;
	if(i&OUT)
		if(i & ~OUT){
			if(i & ~(OUT|QUEST))
				error("action error - OUT not alone");
		}
	if(i & PRINT)
		if(i & ~PRINT){
			if(i & ~(PRINT|QUEST))
				error("action error - PRINT not alone");
		}
	if(i & PATH)
		if(i & ~PATH)error("action error - PATH not alone");
	if((i&(MANY|ONELINE))==(MANY|ONELINE))
		error("contridictory actions - MANY & ONELINE");
	return(i);
}

process(start)
int start;
{
	if(start == 0){
		printf("%s\n",first);
		getl(stdin,line);
		pp=findpath(line);
		lp = pp->root;
	}
more:
	while(lp){
		if(question(lp->entry)== 1)
			continue;
		if(lp->loop != 0){
			printf("%s\n",lp->quest);
			getl(stdin,line);
			if(line[0] == 'y'|| line[0] == 'Y')
				lp=lp->loop;
			else{
				if(lp->loop->entry->type & 1){
					lp->loop->entry->type &= 0177776;
				}
				if(line[0] == 'q')done();
				else lp=lp->next;
			}
		}
		else
			lp = lp->next;
	}
	if(*st != 0){
		lp = *st--;
		if(lp->loop != 0){
			printf("%s\n",lp->quest);
			getl(stdin,line);
			if(line[0] == 'y' || line[0] == 'Y')
				lp=lp->loop;
			else{
				if(lp->loop->entry->type & 1)
					lp->loop->entry->type &= 0177776;
				if(line[0] == 'q')done();
				else lp=lp->next;
			}
		}
		else lp=lp->next;
		goto more;
	}
}

question(rp)
struct q_a *rp;
{
	int blanks;
	char *ch, *sv;

rest:
	if(debug)printf("in question %o\n",rp);
	if(rp->type & BLOOP)
		return(0);
	if(rp->type & FLOOP){
		if(rp->type & 1)return(0);
		printf("%s\n",rp->qtext);
		getl(stdin,line);
		if(line[0] == 'y' || line[0] == 'Y'){
			lp = lp->next;
			rp->type |= 1;
		}
		else if(line[0] == 'q')done();
		else lp= lp->floop;
		return(1);
	}
	if(rp->type & PATH){
		*(++st) = lp;
		if(st >= &stack[STACK-1])
			error("stack depth exceeded-STACK");
		printf("%s\n",rp->text);
		getl(stdin,line);
		if(line[0] == '\0'){
			st--;
			return(0);
		}
		pp=findpath(line);
		lp = pp->root;
		return(1);
	}
	if(rp->type&QUEST){
		printf("%s\n",rp->qtext);
		getl(stdin,line);
		if(line[0] != 'Y' && line[0] != 'y')return(0);
	}
	if(rp->text != 0)
		if((rp->type&MANY) == 0)printf("%s\n",rp->text);
	if(rp->type&(ONELINE|SEVERAL))
		getl(stdin,sline);
	if(rp->type&NAME){
		return(newname(sline));
	}
	if(rp->type&SUM){
		summary();
	}
	if(rp->type&PAR){
		par();
	}
	if(rp->type&(STUFF|QSTUFF)){
		sv = ch = rp->macro;
		qp = rp->argq;
		do{
			while(*ch != '$'){
				if(*ch == '\\')ch++;
				if(*ch == '\0')break;
				ch++;
			}
			if(*ch == '$'){
				*ch = '\0';
				out(sv);
				*ch++ = '$';
				sv = ch;
			}
			printf("%s\n",qp->text);
			getl(stdin,line);
			if(rp->type&QSTUFF)
				if(line[0] == '\0')break;
			if(*(ch-1) == '$')
				out(line);
			qp = qp->nexta;
		} while(qp != 0);
		if(rp->type&STUFF)
			out(sv);
		else if(*sv == '"')out("\"");
		out("\n");
	}
	else if(rp->type&(ARGS | SKIP)){
		if((rp->type&SKIP) == 0 && rp->macro != 0)
			out(rp->macro);
		qp = rp->argq;
		do{
			printf("%s\n",qp->text);
			blanks = getl(stdin,line);
			if(rp->type&SKIP){
				if(line[0] == '\0')
					return(0);
				else out(rp->macro);
			}
			out(" ");
			if(line[0] == '\0')blanks++;
			if(blanks)out("\"");
			out(line);
			if(blanks)out("\"");
			qp=qp->nexta;
		} while(qp != 0);
		out("\n");
	}
	else if((rp->type&(OUT|ONELINE|MANY))&& rp->macro != 0){
		out(rp->macro);
		out("\n");
	}
	if(rp->type&SEVERAL){
		out(rp->macro);
		out(" ");
		out(sline);
		out("\n");
	}
	if(rp->type&ONELINE){
		out(sline);
		out("\n");
	}
	if(rp->type&MANY){
		if(rp->type&(SUM))
			printf("%s named %s.sum\n",rp->text,nname);
		else if(rp->type&PAR)
			printf("%s named %s.par\n",rp->text,nname);
		else printf("%s\n",rp->text);
		printf("	end with : alone on a line (or read a file with :r filename)\n");
		getl(stdin,line);
		if(line[0] == ':' && line[1] == 'r'){
			ch = &line[2];
	more:
			readfiles(ch);
			printf("Type other file names or return\n");
			getl(stdin,line);
			if(line[0] == '\0')return(0);
			ch = line;
			goto more;
		}
		toed();
	}
	return(0);
}
sig_in_ed(){
	inap = 0;
	printf("type q to quit ");
	getl(stdin,line);
	if(line[0] == 'q')done();
	fputs("a\n",edp);
	optr = obuf;
	inap=1;
	signal(SIGINT,sig_in_ed);
	process(1);
	done();
}

getl(inp,l)
char *l;
FILE *inp;
{
	int c;
	int blanks;

	blanks=0;
	while((*l = c = getc(inp)) != '\n'){
		l++;
		if(c == ' ' || c == '\t')blanks++;
	}
	*l++ = 0;
	return(blanks);
}

out(ch)
char *ch;
{
	char c;

	while(*ch != '\0'){
		if(*ch == '\n'){
			*optr++ = *ch++;
			*optr = '\0';
			fputs(obuf,edp);
			optr=obuf;
			if(verbose)printf("%s",obuf);
		}
		else *optr++ = *ch++;
		if(optr >= &obuf[BUFS-1]){
			*optr = '\0';
			fputs(obuf,edp);
			optr = obuf;
		}
	}
}
ofl()
{
	if(optr != obuf){
		*optr='\0';
		fputs(obuf,edp);
		optr=obuf;
	}
	if(inap)
		fputs(".\nw\na\n",edp);
	else {
		fputs("w\na\n",edp);
		inap=1;
	}
}
done(){
	ofl();
	fputs(".\nq\n",edp);
	exit(0);
}

match(ch,hh)
char *ch, *hh;
{
	while(*ch == *hh){
		if(*ch == '\0')return(1);
		ch++;
		hh++;
	}
	return(0);
}

struct pa *
findpath(ch)
char *ch;
{
	for(pp=paths;pp->name != 0;pp++){
		if(match(ch,pp->name))
			return(pp);
	}
	printf("can't find path for %s\n",ch);
	exit(0);
}
eatline(inp)
FILE *inp;
{
	while(getc(inp) != '\n');
	lineno++;
}
readfiles(ptr)
char *ptr;
{
	FILE *io;
	int nomore;
	char *bp;
	char fname[50];

	ofl();
	fputs(".\n",edp);
	nomore = 0;
	while(1){
		while(*ptr == ' ')ptr++;
		bp = ptr;
		while(*ptr != ' ' && *ptr != '\0')ptr++;
		if(*ptr == '\0')nomore++;
		else *ptr++ = '\0';
		sprintf(fname,"$r %s\n",bp);
		fputs(fname,edp);
		if(nomore != 0)break;
	}
	fputs("a\n",edp);
}
error(sp)
char *sp;
{
	fprintf(stderr,"%s at line %d file %s\n",sp,lineno,qfile);
	exit(1);
}
db(dbp)
struct q_a *dbp;
{
	printf(" macro= %s type=",dbp->macro);
	if(dbp->type&ARGS)printf(" ARGS");
	if(dbp->type&SKIP)printf(" SKIP");
	if(dbp->type&OUT)printf(" OUT");
	if(dbp->type&PRINT)printf(" PRINT");
	if(dbp->type&ONELINE)printf(" ONELINE");
	if(dbp->type&SEVERAL)printf(" SEVERAL");
	if(dbp->type&MANY)printf(" MANY");
	if(dbp->type&PATH)printf(" PATH");
	if(dbp->text != 0)printf("\nmain text= %s\n",dbp->text);
	else printf("\n");
}
newname(s)
char *s;
{
	int i;
	char *name1, *name2;
	int pid, rpid, retcode;
	char *sp,spac[10];
restart:
	sp = spac;
	*sp = '\0';
	if(*s == ' ' || *s == '\t')
		while(*s== ' ' || *s == '\t')*sp++ = *s++;
	*sp = '\0';
	for(i=0;*s != '\0';i++)
		nname[i] = *s++;
	if(nname[0] == ':'){
		switch(nname[1]){
		case 'p':
			sprintf(line,".\nw\ne %s\n1,$p\na\n",ofile);
			fputs(line,edp);
			sleep(8);
			return(1);
		case 'r':
			sprintf(line,".\nw\ne %s\na\n",&nname[3]);
			printf("Type text to append to file %s\n",&nname[3]);
			fputs(line,edp);
			getl(stdin,line);
			toed();
			return(1);
		case 'j':
			name1=0;
			name2 = nname;
	nextn:
			for(; *name2 != ' ';name2++)
				if(*name2 == '\0'){
					printf("error in command line: %s\nplease retype\n",nname);
					getl(stdin,sline);
					s=sline;
					goto restart;
				}
			*name2++ = '\0';
			while(*name2 == ' ')name2++;
			if(name1 == 0){
				name1 = name2;
				goto nextn;
			}
			sprintf(line,".\nw\ne %s.sum\n$r %s.sum\nw\n",name1,name2);
			fputs(line,edp);
			sprintf(line,"e %s.par\n$r %s.par\nw\n",name1,name2);
			fputs(line,edp);
			sprintf(line,"e %s\n/%s/d\nw\na\n",ofile);
			fputs(line,edp);
			return(1);
		default:
			printf("unknown command %s\n",nname);
			printf("please retype\n");
			goto restart;
		}
	}
	if(nname[0] == '!'){
		if((pid = fork()) == 0){
			execl("/bin/sh","sh","-c",&nname[1],0);
			exit(0);
		}
		while((rpid=wait(&retcode))!=pid&& rpid != -1);
		printf("!\n");
		return(1);
	}
	if(i > 10){
		printf("tag too long - please type new tag ");
		getl(stdin,sline);
		s=sline;
		goto restart;
	}
	nname[i] = '\0';
	sprintf(line, ".\nw\ne %s\na\n%s%s\n",ofile,spac,nname);
	fputs(line,edp);
	return(0);
}
summary(){
	FILE *ff;
	sprintf(line,"%s.sum",nname);
	if((ff = fopen(line,"a")) == NULL){
		printf("can't open %s\n",line);
	}
	sprintf(line,".\nw\ne %s.sum\na\n",nname);
	fputs(line,edp);
	fclose(ff);
}
par(){
	FILE *ff;
	sprintf(line,"%s.par",nname);
	if((ff=fopen(line,"a")) == NULL){
		printf("can't open %s\n",line);
		exit(1);
	}
	sprintf(line,".\nw\ne %s.par\na\n",nname);
	fputs(line,edp);
	fclose(ff);
}
toed(){
	while(1){
		if(line[1] == '\0'){
		switch(line[0]){
		case 'i':
		case 'a':
		case 'c':
			inap++;
			break;
		case '.':
			inap = 0;
			break;
		case 'q':
			if(inap != 0)break;
		case ':':
			if(inap == 0){fputs("a\n",edp);
				inap++;
			}
			return(0);
		}
		}
		fputs(line,edp);
		fputs("\n",edp);
		getl(stdin,line);
	}
}
