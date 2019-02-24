/*	@(#)spellin.c	1.1	*/
#include <stdio.h>
#include "hash.h"
#include <signal.h>

#define S (BYTE*sizeof(long))
#define B (BYTE*sizeof(unsigned))
unsigned tabword,codeword;
unsigned short index[NI];
unsigned short cindex[NI];	/* index for code */
unsigned wp,cwp;		/* word pointer*/
int bp =B,cbp= B;		/* bit pointer*/
int extra;
char word[50], oword[50]; /* warning: 50 is built into fscanf format */
unsigned long codetable[EXTRA];

/*	usage: hashin N
	where N is number of words in dictionary
	and standard input contains sorted, unique
	hashed words in octal
	Output file:
		huff data
		code table 0177
		index to hash
		seek position of packed code
		index to code
		hashes
		packed code
		
*/
FILE *ftemp;
char *tempfile = "temp.XXXXXX";


main(argc,argv)
char **argv;
{
	long h,k,d,oh;
	long count;
	long w;
	long x;
	long usage, ousage;
	int t,u;
	register i;
	extern double huff();
	extern long ftell();
	extern onintr();
	char buf[512];
	long seekpt,cseekpt;
	double atof();
	double z, nwords;
	FILE *list, *code;
	if(argc!=4) 
		error("arg count");
	nwords = atof(argv[1]);
	list = fopen(argv[2],"r");
	if(list==0)
		error("can't open word list");
	code = fopen(argv[3],"r");
	if(code==0)
		error("can't open code table");
	while(fscanf(code,"%o %d", &t, &u)==2)
			codetable[u] = t;
	fclose(code);
	fwrite((char*)codetable,sizeof(codetable),1,stdout);
	z = huff((1L<<HASHWIDTH)/nwords);
	fprintf(stderr, "spellin: expected code widths = %f", z);
	z += sizeof(tabword)*BYTE/2*(double)(1<<INDEXWIDTH)/nwords;
	fprintf(stderr, " +usage+breakage = %f\n", CODEWIDTH+z);
	whuff();
	seekpt = ftell(stdout);
	fwrite((char*)index, sizeof(*index), NI, stdout); /*dummy data */
	fwrite((char*)index, sizeof(long),1,stdout);  /* dummy seek for code hashes */
	fwrite((char*)cindex, sizeof(*index), NI, stdout); /*dummy code data */
	
	k = 0;
	u = 0;
	mktemp(tempfile);
	ftemp = fopen(tempfile,"w");
	if(ftemp == NULL) {
		error("can't open tempfile");
	}
	if(signal(SIGINT,SIG_IGN) != SIG_IGN)
	signal(SIGINT, onintr);
	for(count=0; ; ++count) {
		if(fscanf(list,"%lo %ld %50s", &h, &usage, word)!=3)
			break; 
		if((strcmp(word,oword)==0) || h == oh) {
			fprintf(stderr, "spellin collision: %s %o, %s %o\n",
				word,usage,oword,ousage);
			continue;
		}
		strcpy(oword,word);
		ousage = usage;
		oh = h;
		if(usage>=EXTRA) {
			fprintf(stderr,"spellin: too many usage codes");
			exit(1);
		}
		if((t=h>>(HASHWIDTH-INDEXWIDTH)) != u) { /*bin bdry*/
			if(bp!=B)
				newword();
			if(cbp != B) newcode();
			cbp = bp = B;
			while(u<t) {
				index[++u] = wp;
				cindex[u] = cwp;
			}
			k =  (long)t<<(HASHWIDTH-INDEXWIDTH);
		}
		d = h-k;
		k = h;
		for(;;) {
			for(x=d;;x/=2) {
				i = encode(x,&w);
				if(i>0)
					break;
			}
			if(i>B) {
				append((unsigned)(w>>(i-B)), B);
				append((unsigned)(w<<(B+B-i)), i-B);
			} else
				append((unsigned)(w<<(B-i)), i);
			d -= x;
			if(d>0)
				extra++;
			else 
				break;
		}

		cappend((unsigned)(usage<<(B-CODEWIDTH)), CODEWIDTH);
	}
	if(bp!=B)
		newword();
	if(cbp != B) newcode();
	while(++u<NI){
		cindex[u] = cwp;
		index[u] = wp;
	}
	newword();	/* padding allows one out-of-bounds fetch */
	newword();
	newword();
	newcode();
	newcode();
	cseekpt = ftell(stdout);
	fclose(ftemp);
	k = open(tempfile,0);
	while(t = read(k,buf,512)) {
		fwrite(buf,t,1,stdout);
	}
	fflush(stdout);
	fseek(stdout, seekpt, 0);	/* overwrite dummy data */
	fwrite((char*)index, sizeof(*index), NI, stdout);
	fwrite((char*)&cseekpt,sizeof(long),1,stdout);
	fwrite((char*)cindex, sizeof(*cindex), NI, stdout);
	fprintf(stderr, "spellin: %ld items, %d extra, %u words occupied\n",
		count,extra,wp);
	fprintf(stderr, "spellin: %f table bits/item, ", 
		((float)BYTE*wp)*sizeof(tabword)/count);
	fprintf(stderr, "%f table+index bits\n",
		BYTE*((float)wp*sizeof(tabword) + sizeof(index))/count);

	unlink(tempfile);
	return(0);
}

append(w, i)
register unsigned w;
register i;
{
	for(;;) {
		tabword |= w>>(B-bp);
		i -= bp;
		if(i<0) {
			bp = -i;
			return;
		}
		w <<= bp;
		bp = B;
		newword();
	}
}

newword()
{
	fwrite((char*)&tabword, sizeof(tabword), 1, stdout);
	wp++;
	tabword = 0;
fflush(stdout);
}

cappend(w, i)
register unsigned w;
register i;
{
	for(;;) {
		codeword |= w>>(B-cbp);
		i -= cbp;
		if(i<0) {
			cbp = -i;
			return;
		}
		w <<= cbp;
		cbp = B;
		newcode();
	}
}

newcode()
{
	fwrite((char*)&codeword, sizeof(codeword), 1, ftemp);
	cwp++;
	codeword = 0;
fflush(stdout);
}

error(s)
char *s;
{
	fprintf(stderr,"spellin: %s\n",s);
	exit(1);
}
onintr()
{
	unlink(tempfile);
	exit(2);
}
