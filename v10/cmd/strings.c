#include <stdio.h>
#include <ctype.h>
#include <libc.h>
#include </usr/include/a.out.h>

#define INF (~0UL>>1)
int minlen = 4;
int oflag;
int tflag, dflag, sflag, aflag;

void strings(FILE*, long, long);
char *flush(char*, long);

int
main(int argc, char **argv)
{
	int i;
	int errors = 0;
	struct exec header;
	while(argc>1 && argv[1][0]=='-') {
		if(argv[1][1] == 0)	/* oldthink */
			aflag++;
		while(*++argv[1]) {
			switch(*argv[1]) {
			case 'o':
				oflag++;
				continue;
			case 't':
				tflag++;
				continue;
			case 'd':
				dflag++;
				continue;
			case 's':
				sflag++;
				continue;
			case 'a':
				aflag++;
				continue;
			case '0':case '1':case '2':case '3':case '4':
			case '5':case '6':case '7':case '8':case '9':
				minlen = atoi(argv[1]);
			}
			break;
		}
		argc--;
		argv++;
	}
	if(!(tflag|dflag|sflag|aflag)) {
		tflag++;
		dflag++;
	}
	for(i=1; i<argc; i++) {
		long base = sizeof header;
		FILE *f = fopen(argv[i],"r");
		if(f == 0) {
			fprintf(stderr,"strings: ");
			perror(argv[i]);
			errors++;
			continue;
		}
		if(argc > 2) {
			if(i > 1) printf("\n");
			printf("%s:\n", argv[i]);
		}
		if(aflag)
			goto all;
		if(fread(&header,sizeof(header),1,f) != 1)					goto all;
		switch(header.a_magic) {
		all:
		default:
			strings(f, 0, INF);
			break;
		case ZMAGIC:
			base = 1024;
		case OMAGIC:
		case NMAGIC:
			if(tflag)
				strings(f, base, header.a_text);
			base += header.a_text;
			if(dflag)
				strings(f, base, header.a_data);
			base += header.a_data + header.a_syms +
				header.a_trsize + header.a_drsize;
			if(sflag)
				strings(f, base, INF);
		}
		fclose(f);
	}
	return errors;
}

char string[1024];

void
strings(FILE *f, long base, long length)
{
	int c;
	char *cp = string;
	fseek(f, base, 0);
	cp = string;
	for( ; --length>=0 && (c=getc(f))!=EOF; base++) {
		switch(c) {
		case '\n':
		case 0:
			cp = flush(cp, base);
			continue;
		case '\t':
		case '\b':
			break;
		default:
			if(isprint(c))
				break;
			cp = string;
			continue;
		}
		*cp++ = c;
		if(cp >= string + sizeof string - 2)
			cp = flush(cp, base);
	}
}

char *
flush(char *cp, long base)
{
	if(cp-string >= minlen) {
		*cp = 0;
		if(oflag)
			printf("%9lo ", base - (cp-string));
		puts(string);
	}
	return string;
}
