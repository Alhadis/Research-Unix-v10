#include	<stdio.h>

#define		NBUF		(64*1024)
int space[256];
char	*opt="lwc";
long	twords;
long	tlines;
long	tchars;

main(argc, argv)
	char *argv[];
{
	register i, fd, status=0;
	if(argc>1 && argv[1][0]=='-'){
		opt=++argv[1];
		--argc, argv++;
	}
	for(i = 0; i < 256; i++)
		space[i] = 0;
	space[' '] = space['\t'] = space['\n'] = 1;
	if(argc==1)
		count(0, (char *)0);
	else for(i=1; i<argc; i++){
		fd=open(argv[i], 0);
		if(fd<0){
			fprintf(stderr, "wc: ");
			perror(argv[i]);
			status=1;
			continue;
		}
		count(fd, argv[i]);	
	}
	if(argc>2)
		printout(tchars, twords, tlines, "total");
	return status;
}
unsigned char buf[NBUF+2];	/* 2 for sentinels */
count(fd, name)
	char *name;
{
	register n;
	register unsigned char *cp, *cpend;
	register long chars=0, lines=0, words=0;

	for(;;){
		if((n=read(fd, buf, NBUF))<=0)
			goto done;
		chars+=n;
		cp=buf;
		*(cpend = buf+n) = ' ';
		cpend[1] = 'a';
dospace:
		for(;;){
			if(*cp == '\n')
				lines++;
			if(space[*cp++] == 0){
				if(cp > cpend)
					break;
				goto doword;
			}
		}
	}
	for(;;){
		if((n=read(fd, buf, NBUF))<=0)
			goto done;
		chars+=n;
		cp=buf;
		*(cpend = buf+n) = ' ';
		cpend[1] = 'a';
doword:
		for(;;){
			if(space[*cp++]){
				if(cp > cpend)
					break;
				words++;
				if(cp[-1] == '\n')
					lines++;
				goto dospace;
			}
		}
	}
done:
	close(fd);
	printout(chars, words, lines, name);
	tchars+=chars;
	twords+=words;
	tlines+=lines;
}
printout(charct, wordct, linect, name)
	long charct, wordct, linect;
	char *name;
{
	register char *wd=opt;
	while (*wd) switch (*wd++) {
	case 'l':
		printf("%7ld ", linect);
		break;

	case 'w':
		printf("%7ld ", wordct);
		break;

	case 'c':
		printf("%7ld", charct);
		break;
	}
	if(name)
		printf(" %s\n", name);
	else
		printf("\n");
}

