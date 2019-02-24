/*% cc -go # %
 * too-complex fortune program.
 * Tom Duff at Lucasfilm, Ltd. 81.11.23
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
char line[512], choice[512];
char index[]="/usr/games/lib/fortunes.index";
char fortunes[]="/usr/games/lib/fortunes";
main(argc, argv)
char *argv[];
{
	int i;
	long offs;
	register FILE *f, *ix;
	int newindex=0, oldindex=0;
	struct stat fbuf, ixbuf;
	if(argc==1){
		if(stat(fortunes, &fbuf)>=0
		&& (stat(index, &ixbuf)<0 || fbuf.st_mtime>=ixbuf.st_mtime)
		&& (ix=fopen(index, "w"))!=NULL)
			newindex++;
		else if((ix=fopen(index, "r"))==NULL){
			if((ix=fopen(index, "w"))!=NULL)
				newindex++;
		}
		else
			oldindex++;
	}
	if((f=fopen(argc>1?argv[1]:fortunes, "r"))==NULL){
		printf("Misfortune!\n");
		exit(1);
	}
	srand((int)(time((long)0)+getpid()*getpid()));
	if(oldindex){
		fseek(ix, 0L, 2);
		fseek(ix, nrand(ftell(ix)/sizeof(offs))*sizeof(offs), 0);
		fread(&offs, sizeof(offs), 1, ix);
		fseek(f, offs, 0);
		fgets(choice, 512, f);
	}
	else{
		for(i=1;;i++){
			if(newindex)
				offs=ftell(f);
			fgets(line, 512, f);
			if(feof(f))
				break;
			if(newindex)
				fwrite(&offs, sizeof(offs), 1, ix);
			if(nrand(i)==0)
				strcpy(choice, line);
		}
	}
	printf("%s", choice);
	return 0;
}
