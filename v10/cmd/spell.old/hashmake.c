/*	@(#)hashmake.c	1.1	*/
#include "hash.h"
#include <stdio.h>
unsigned long codetable[EXTRA];
char *codestr = NULL;
char *countstr = NULL;

main(argc,argv)
char *argv[];
{
	char word[30],*s; /* warning: 30 is built into fscanf format */
	long hashval,ohashval = -1;
	long h;
	unsigned long code;
	FILE *fcode = NULL,*fcount = NULL;
	int t,u;
	register i = -1;
	register no = 0;
	int first = 1;

	hashinit();
	if(argc >1) {
		if(argv[1][0] == '-' && argv[1][1] == 'c') {
			if(--argc <=1) {
				error("option error %s\n",argv[1]);
			}
			argv++;
			countstr = argv[1];
			argc--; ++argv;
		}
	}
	if(argc >1)codestr = argv[1];
	else {
		fprintf(stderr,
		"Hashmake : code, serial out to %s\n", codestr);
	}
	if(codestr) {
		t= creat(codestr,0666);
		fcode = fdopen(t,"w");
		if(fcode== NULL)
		error("can't open code table",codestr);
	}
	if(countstr) {
		t= creat(countstr,0666);
		fcount = fdopen(t,"w");
		if(fcount == NULL)
		error("can't open code table",countstr);
	}
	while(scanf("%o %30s",&code,word)==2) {
		hashval = hash(word);
		if(hashval == (1<<HASHWIDTH)-1) {
			fprintf(stderr,"hashmake: rejecting %s\n",word);
			continue;
		}
		if(i <0 ) codetable[++i] = code;
		else
		if((codetable[i] != code) && ++i < EXTRA)
			codetable[i] = code;
		if(i>=EXTRA && first) {
			fprintf(stderr,"hashmake: code overflow, %s\n",word);
			first = 0;
		}
		printf("%.*lo %5d %s\n",
			(HASHWIDTH+2)/3,hashval,i,word);
		if(hashval != ohashval)no++;
		ohashval = hashval;
	}
	if(fcode ) {
		for(t = 0; t <= i; t++)fprintf(fcode,"%o\t%d\n",
			codetable[t],t);
		fclose(fcode);
	}
	if(first)
		fprintf(stderr,"hashmake: %d distinct typecodes\n",i+1);
	if(fcount) {
		fprintf(fcount,"%d",no);
		fclose(fcount);
	}
	return first!=1;	
}
error(s,s1)
char *s,*s1;
{
	fprintf(stderr,"hashmake: ");
	fprintf(stderr,s,s1);
	exit(2);
}
