/* read an annotated spelling list in the form
	word <tab> affixcode [ , affixcode ] ...
   print a reencoded version
	octal <tab> word
   -d append decimal equivalent
   -l append canonicalized affixcodes 
*/
#include <stdio.h>
#include <ctype.h>
#include "code.h"

int found = 0;
int lflag = 0; 
int dflag = 0;
int error = 0;

main(argc,argv)
char **argv;
{
	register char  *w,*s;
	int i,n;
	FILE *f;


	while(argc >1){
		if(argv[1][0] != '-') break;
		s = &argv[1][1];
		while(*s){ switch (*s) {
			case 'l':
				lflag++;
				break;
			case 'd':
				dflag++;
				break;
			}
			s++;
		}
		argv++;
		argc--;
	}
	if(argc <=1){ 
		f = stdin;
		readinput(f);
	}
	else 
	while(argc >1) {
		f = fopen(argv[1],"r");
		if(f == NULL) {
			fprintf(stderr,"Cannot open %s\n",argv[1]);
			exit(1);
		}
		else {readinput(f); fclose(f); }
		argc--;
		argv++;
	}
	return error;
}

readinput(f)
register FILE *f;
{
	register i;
	char line[512];
	char a[512];
	register char *code;
	int lineno = 0;

	while((fgets(line,512,f)) != NULL) {
		lineno++;
		code = line;
		while(isspace(*code))code++;
		while(*code && !isspace(*code) )code++;
		if(*code) {
			*code++ = 0;
			while(isspace(*code))code++;
		}
		i = typecode(code);
		if((i&ALL)==ALL || i==0) {
			error++;
			fprintf(stderr,"%d:%o\t%s\t",lineno,i,line);
			typeprint(stderr,i);
			fprintf(stderr,"\n");
		}
		printf("%o\t%s",i,line);
		if(dflag) printf("\t%d",i);
		if(!lflag || i == 0) {
			printf("\n");
			continue;
		}
		printf("\t");
		found = 0;
		if(typeprint(stdout,i)){
			error++;
			fprintf(stderr,"%d:%o\t%s\t",lineno,i,line);
			typeprint(stderr,i);
			fprintf(stderr,"ERROR\n");
		}
		printf("\n");
	}	
}
