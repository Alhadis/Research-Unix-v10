#include <fio.h>
#include <regexp.h>
#include <string.h>
#include <libc.h>

enum { nofile = -1, stdin, stdout, stderr };

char	digit[] = "0123456789";
char	*suffix = "";
char	*stem = "x";
char	suff[] = "aa";
char	name[200];
int	output = nofile;

char	*fold(), *slashfix();

main(argc, argv)
char **argv;
{
	regexp *exp = 0;
	char *pattern = 0;
	register n = 1000;
	char *line;
	int xflag = 0;
	int iflag = 0;

	for( ; argc>1 && argv[1][0]=='-'; ++argv, --argc) {
		if(strchr(digit, argv[1][1])) {
			if((n=atoi(&argv[1][1])) <= 0) {
				Fprint(stderr,"split: n not positive\n");
				exit(1);
			}
		} else switch(argv[1][1]) {
		default:
			usage();
		case 'n':
			if(++argv, --argc<=1)
				usage();
			n=atoi(argv[1]);
			break;
		case 'e':
			if(++argv, --argc<=1)
				usage();
			pattern = slashfix(argv[1]);
			break;
		case 'f':
			if(++argv, --argc<=1)
				usage();
			stem = strdup(argv[1]);
			break;
		case 's':
			if(++argv, --argc<=1)
				usage();
			suffix = strdup(argv[1]);
			break;
		case 'x':
			xflag++;
			break;
		case 'y':
			Fprint(stderr,"split: -y obsolete; -i assumed\n");
		case 'i':
			iflag++;
			break;
		}
	}
	if(argc > 2)
		usage();
	if(argc > 1) {
		close(stdin);
		if(open(argv[1], 0) != stdin) {
			Fprint(stderr,"split: cannot open %s\n",argv[1]);
			exit(1);
		}
	}
	if(pattern) {
		if(!(exp = regcomp(iflag? fold(pattern): pattern)))
			badexp();
		while((line=Frdline(stdin)) != 0) {
			regsubexp match[2];
			if(regexec(exp,iflag?fold(line):line,match,2)) {
				if(matchfile(match) && xflag)
					continue;
			} else if(output == nofile)
				nextfile();	/* at most once */
			Fwrite(output, line, FIOLINELEN(stdin));
			Fputc(output, '\n');
		}
	} else {
		register linecnt = n;
		while((line=Frdline(stdin)) != 0) {
			if(++linecnt > n) {
				nextfile();
				linecnt = 1;
			}
			Fwrite(output, line, FIOLINELEN(stdin));
			Fputc(output, '\n');
		}
	}
	return 0;
}

nextfile()
{
	static canopen = 1;
	if(suff[0] > 'z') {
		if(canopen)
			Fprint(stderr,"split: file %szz not split\n",stem);
		canopen = 0;
	} else {
		strcpy(name, stem);
		strcat(name, suff);
		if(++suff[1] > 'z') 
			suff[1] = 'a', ++suff[0];
		openf();
	}
	return canopen;
}

matchfile(match)
regsubexp *match;
{
	if(match[1].sp) {
		int len = match[1].ep - match[1].sp;
		strncpy(name, match[1].sp, len);
		strcpy(name+len, suffix);
		openf();
		return 1;
	} 
	return nextfile();
}

openf()
{
	Fflush(output);
	close(output);
	if((output=creat(name,0666)) == -1) {
		Fprint(stderr, "split: cannot open %s\n",name);
		exit(1);
	}
	Finit(output,(char*)0);
}

char *
fold(s)
char *s;
{
	static char *fline;
	char *t;
	if(fline==0)
		fline = malloc(FIOBSIZE);
	for(t=fline; *t++=tolower(*s++); )
		continue;
	return fline;
}

/* Convert from grep to egrep syntax.  Grep special casing like
   initial * will yield badexp() */
char *
slashfix(s)
register char *s;
{
	char *u = malloc(2*strlen(s));
	register char *t = u;
	for( ; *t = *s; t++, s++) {
		if(strchr("()+?|", *s))
			*t = '\\', *++t = *s;
		else if(*s=='\\' && s[1])
			if(strchr(digit,s[1]))
				badexp();
			else if(strchr("()",s[1]))
				*t = *++s;
			else 
				*++t = *++s;
	}
	return u;
}	

usage()
{
	Fprint(stderr, "usage: split [-n num] [-e exp] [-f stem] [-s suff] [-x] [-y] [file]\n");
	exit(1);
}

badexp()
{
	Fprint(stderr, "split: bad regular expression\n");
	exit(1);
}
