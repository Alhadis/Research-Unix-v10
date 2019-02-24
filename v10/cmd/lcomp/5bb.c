#define unsafe 1	/* pretend killing all but C is as good as killing
			 * all condition codes */
#ifdef unsafe
#define E	(C|K)
#else
#define E	K
#endif
#include "stdio.h"
#include "instr.c"
#include "ctype.h"
struct inst *index[128];

#define SBBLK	1	/* looking for the start of a basic block */
#define SINST	2	/* thinking about generating a tally */
#define SPRO	3	/* thinking about generating prolog code */
#define SMAYBE	4	/* seen _, thinking about SPRO */

#define MAXLA	5	/* max no. of lines that can be read for lookahead */

FILE *outs, *fd, *outl;	/* outs goes to assembler, outl is for listing */
extern FILE *popen();
char line[256], fname[256];	/* file names must fit in fname */

char labuf[5][256];		/* look ahead needed for 5.0 stab entries */
int lastla, lookahead;
char begfcn[] = "	.def	.bf";	/* 5.0 stab entry */

int lineno, lastline;
int base=0;

int cnt;
int state = SBBLK;
char *ptr, *curarg;
char curdir[256];

main(argc, argv)
char **argv;
{	int i;
	if(argc <= 1) {
		fprintf(stderr, "no files given\n");
		exit(1);
	}
	for(i = 0; insts[i].iname; i++)
		;
	for(; i >= 0; i--)
		index[insts[i].iname[0]] = insts + i;
	fd = popen("pwd", "r");
	for(i = 0; i < sizeof(curdir) && !feof(fd); i++)
		curdir[i] = getc(fd);
	curdir[i-2] = 0;	/* thisdir\n */
	fclose(fd);
	for(i = 1; i < argc; i++) {
		if(setfd(argv[i]))	/* fix fd, outs, outl */
			doarg();	/* do the work */
	}
	exit(0);
}

setfd(s)
char *s;
{	char outnams[24], outnaml[24];
	fname[0] = 0;
	cnt = 3;
	if(fd != NULL)
		fclose(fd);
	if(outs != NULL)
		fclose(outs);
	if(outl != NULL)
		fclose(outl);
	sprintf(outnams, "X%s", s);
	sprintf(outnaml, "%sL", s);
	lastline = lineno = 0;
	fd = fopen(s, "r");
	if(fd == NULL) {
		perror(s);
		return(0);
	}
	outs = fopen(outnams, "w");
	if(outs == NULL) {
		perror(outnams);
		return(0);
	}
	outl = fopen(outnaml, "w");
	if(outl == NULL) {
		perror(outnaml);
		return(0);
	}
	curarg = s;
	return(1);
}

doarg()
{	struct inst *x, *firstword();

	state = SBBLK;
	lookahead = 0;		/* empty buffer */
	lastla = 0;
	for(;;) {
		if (lookahead != lastla){
			strcpy(line,labuf[ (lookahead++) % MAXLA ] );
			if (lookahead == lastla) lookahead = lastla = 0;
		}
		else
			(void) fgets(line, sizeof(line), fd);
		if(feof(fd))
			break;
		for(ptr = line; isspace(*ptr); *ptr++)
			;
		if(*ptr == 0 || *ptr == '#')
			continue;
		testlabel();
		/* deal with symbol table info */
		if(*ptr == '.') {
			stab();
			fprintf(outs, "	%s", ptr);
			continue;
		}
		if(*ptr == 0 || *ptr == '\n')
			continue;
		x = firstword();
		if(x == 0){
			printf("unknown inst: %s\n",ptr);
			continue;
		}
#ifdef u3b
		if ((state == SMAYBE) && !strncmp(ptr,"save",4)){
			state = SPRO;
			getlnum();	/* 5.0 true line # */
		}
#endif
		if(state == SPRO)
			prolog(x);
		if(state == SINST)
			tally(x);
		if(state == SBBLK && (x->type & JUMP))
			state = SINST;
		outinstr();
		if(x->type & BYTE)
			fixinstr(x);
		fprintf(outs, "	%s", ptr);
	}
	finish();
}


getlnum(){	/* get true line number from 5.0 sdb info */
	char *la;
	int l;

	/* get true line number by looking ahead */
	/* this is necessary for 5.0 sdb output  */
	do
		(void) fgets(labuf[l = ((lastla++)%MAXLA)], sizeof(line), fd);
	while (!feof(fd) && strncmp(begfcn, labuf[l],
			9) && (lookahead != (lastla)%MAXLA ));
	if ( strncmp(begfcn, labuf[l], 9) ) 
		return;		/* bad input  or not 5.0 */
	la = labuf[l];
	while ( *la != '\0' ){
		while ( *la != ';' && *la != '\0') la++;
		if ( *la == ';' ) la++;
		while ( isspace(*la) ) la++;
		if ( !strncmp(la, ".line" , 5) ){
			lineno = base = atoi( la + 5 );
			return;
		}
	}
}


/* unbelievable variability in sdb info */
stab()
{	char buf[128];
	int i, j, k;

#ifndef u3b
	if(state == SMAYBE && strncmp(ptr, ".word", 5) == 0) {
		state = SPRO;
		getlnum();
		return;
	}
#endif

	if((i = *(ptr + 1)) != 's' && i != 'f' && i != 'l')
		return;
	/* real compiler output */
	if(sscanf(ptr, ".stabs \"%[^\"]\", %o", buf, &i) == 2 && i == 0144)
		strcat(fname, buf);
	else if(sscanf(ptr, ".stabd %o,%o,%o", &i, &j, &k) == 3 && i == 0104)
		lineno = k;
	/* pwb 3.0 */
	else if(sscanf(ptr, ".stab %[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d",
		buf, buf+2, buf+4, buf+6, buf+8, buf+10, buf+12, buf+14, &i)
		== 9 && i == 144) {
			for(i = k = 0; buf[i] ; i++)
				if(buf[i] == '\'')
					buf[k++] = buf[++i];
			buf[k] = 0;
			strcat(fname, buf);
		}
	else if(sscanf(ptr, ".stab %[0,]%d,%d,%d", buf,&i, &j, &k) == 4
		&& i == 104)
		lineno = k;
	/* pwb 5.0 */
 	else if(sscanf(ptr," .file \"%[^\"]\"", buf) == 1)
		strcat(fname, buf);
 	else if(sscanf(ptr, " .ln %d",&k) == 1)
		lineno = k + base - 1;
}

testlabel()
{	char *p;
	int i;

for(;;){
	for(p = ptr; *p; p++) {
		if(*p == ':')
			break;
		if(!isalnum(*p) && *p != '_' && *p != '.')
			return;
	}
	if(*p == 0)
		return;
	*p++ = 0;	/* that is overwriting the : */
	fprintf(outs, "%s:\n", ptr);
	if (lineno != lastline){
		for (i=lastline+1; i < lineno ; i++)	/* allign label with right inst */
			fprintf(outl, "0 %s: %d\n", fname, i);
		lastline = lineno - 1;
	}
	fprintf(outl, "%d %s:\n", 4*(cnt - 1), ptr);
#ifdef u3b
	if(*ptr != '.')
#else
	if(*ptr == '_' )
#endif
		state = SMAYBE;
	else
		if(state != SPRO)
			state = SINST;
	for(ptr = p; isspace(*ptr); ptr++)
		;
	}	/* L68:L70: ... */
}

struct inst *
firstword()
{	char buf[sizeof(line)], *p, *q;
	struct inst *x;
	for(p = buf, q = ptr; isalnum(*q); )
		*p++ = *q++;
	if(p == buf)
		return((struct inst *)0);
	*p = 0;
	for(x = index[buf[0]]; x && x->iname[0] == buf[0]; x++)
		if(strcmp(buf, x->iname) == 0)
			return(x);
	return(0);
}

outinstr()
{	int i;
	for(i = lastline + 1; i < lineno; i++)
		fprintf(outl, "0 %s: %d\n", fname, i);
	if (lastline != lineno){
		fprintf(outl, "%d %s: %d\n", 4*(cnt - 1), fname, lineno);
		lastline = lineno;
	}
	fprintf(outl, "%d	%s", 4*(cnt - 1), ptr);
}

#ifdef u3b

/* 3b code  ***********************************/
tally(x)
struct inst *x;
{
	if(x->type & E)
		fprintf(outs, "	addw2 &1,locprof+%d\n", 4*cnt++);
	else {
		fprintf(outs, "	stsm &1,savecc\n");
		fprintf(outs, "	gcc %%r0\n");
		fprintf(outs, "	addw2 &1,locprof+%d\n", 4*cnt++);
		fprintf(outs, "	scc %%r0\n");
		fprintf(outs, "	lsm &1,savecc\n");
	}
	state = SBBLK;
}

prolog(x)	/* no liveness test, presumes can't get here by jump */
struct inst *x;
{	int i;
	fprintf(outs, "	.data\n");
	fprintf(outs, "	.globl proFptr\n");	/* the global chain */
	fprintf(outs, "	.globl savecc\n");
	fprintf(outs, "	.text\n");
	fprintf(outs, "	cmpw &0,locprof+4\n");
	fprintf(outs, "	jne L%da\n", i = cnt);
	fprintf(outs, "	movw proFptr,locprof+4\n");
	fprintf(outs, "	movaw locprof,proFptr\n");
	fprintf(outs, "L%da: addw2 &1,locprof+%d\n", i, 4*cnt++);
	state = SBBLK;
}

finish()
{	int i;
	fprintf(outs, "	.data\n");
	fprintf(outs, "	.align	4\n");
	fprintf(outs, "locprof:\n");
	fprintf(outs, "	.word	%d\n", cnt);
	fprintf(outs, "	.word 0\n");
	fprintf(outs, "	.word L%db\n", cnt);
	fprintf(outs, "	.zero %d\n", 4 * cnt);
	fprintf(outs, "L%db: .byte ", cnt);
	for(i = 0; curdir[i]; i++)
		fprintf(outs, "	0x%x,", curdir[i]);
	fprintf(outs, "	0x%x\n", '/');
	fprintf(outs, "	.byte ");
	if(fname[0])
		for(i = 0; fname[i]; i++)
			fprintf(outs, "	0x%x,", fname[i]);
	else
		for(i = 0; curarg[i]; i++)
			fprintf(outs, "	0x%x,", curarg[i]);
	fprintf(outs, "0\n");
}

#else

/*  Vax code  **************************/
tally(x)
struct inst *x;
{
	if(x->type & E)
		fprintf(outs, "	incl locprof+%d\n", 4*cnt++);
	else {
		fprintf(outs, "	movpsl -(sp)\n");
		fprintf(outs, "	incl locprof+%d\n", 4*cnt++);
		fprintf(outs, "	movw (sp)+,(sp)\n");
		fprintf(outs, "	bicpsw $0xff\n");
		fprintf(outs, "	bispsw (sp)+\n");
		/* thanks to kirk mckusick */
	}
	state = SBBLK;
}


prolog(x)	/* no liveness test, presumes can't get here by jump */
struct inst *x;
{	int i;
	fprintf(outs, "	.data\n");
	fprintf(outs, "	.comm _proFptr,4\n");	/* the global chain */
	fprintf(outs, "	.text\n");
	/*if(!(x->type & E))
		fprintf(outs, "	movpsl -(sp)\n");*/
	fprintf(outs, "	tstl locprof+4\n");
	fprintf(outs, "	bneq L%da\n", i = cnt);
	fprintf(outs, "	movl _proFptr,locprof+4\n");
	fprintf(outs, "	moval locprof,_proFptr\n");
	fprintf(outs, "L%da: incl locprof+%d\n", i, 4*cnt++);
	/*if(!(x->type & E)) {
		fprintf(outs, "	movw (sp)+,(sp)\n");
		fprintf(outs, "	bicpsw $0xff\n");
		fprintf(outs, "	bispsw (sp)+\n");
	}*/
	state = SBBLK;
}

finish()
{	int i;
	fprintf(outs, "	.data\n");
	fprintf(outs, "locprof: .long %d\n", cnt);
	fprintf(outs, "	.long 0\n");
	fprintf(outs, "	.long L%db\n", cnt);
	fprintf(outs, "	.space %d\n", 4 * cnt);
	fprintf(outs, "L%db: .byte ", cnt);
	for(i = 0; curdir[i]; i++)
		fprintf(outs, "0x%x,", curdir[i]);
	fprintf(outs, "0x%x\n", '/');
	fprintf(outs, "	.byte ");
	if(fname[0])
		for(i = 0; fname[i]; i++)
			fprintf(outs, "0x%x,", fname[i]);
	else
		for(i = 0; curarg[i]; i++)
			fprintf(outs, "0x%x,", curarg[i]);
	fprintf(outs, "0\n");
}

#endif


fixinstr(x)
struct inst *x;
{
#ifndef u3b
	if(x->iname[0] == 'b')
		*ptr = 'j';	/* let assembler worry about branches */
	/* this is where the code for aob and sob goes */
#endif
}





