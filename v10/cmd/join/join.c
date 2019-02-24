/*	join F1 F2 on stuff */

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>

enum { F1, F2, FX, F0 };
#define	NFLD	100	/* max field per line */
#define comp() strcmp(ppi[F1][j1],ppi[F2][j2])

FILE *f[2];
char buf[2][BUFSIZ];	/*input lines */
char *ppi[2][NFLD+1];	/* pointers to fields in lines */
int	j1	= 1;	/* join on this field in file 1 */
int	j2	= 1;	/* join on this field in file 2 */
int	olist[2*NFLD];	/* output these fields */
int	olistf[2*NFLD];	/* from these files */
int	no;		/* number of entries in olist */
int	sep1	= ' ';	/* default field separator */
int	sep2	= '\t';
int	discard;	/* count of truncated lines */
char*	null	= "";
int	a1;
int 	a2;
int	vflg;

char*	jcom	= "join: -j is archaic, replaced by -1 and -2\n";
char*	ocom	= "join: archaic -o; commas are preferred\n";
char*	usage	= "usage: join [-1 n] [-2 n] [-o m.n,m.n,...]"
		  " [-t c] [-e s] [-a m] [-v m] file1 file2";

int getopt(int, char**, char*);
extern char *optarg;
extern int optind;

int onelet(char*);
int xatoi(char*);
void optiono(int, char**);
void output(int, int);
int input(int);
void oparse(char*);
void error(char*, char*);
void seek1(void), seek2(void);

int
main(int argc, char **argv)
{
	int i;

	for(;;) {
		switch(getopt(argc, argv, "1:2:a:e:j:o:t:v:")) {
		case -1:
			break;
		case '?':
			error(usage, "");
		case 'v':
			vflg++;
		case 'a':
			switch(onelet(optarg)) {
			case '1':
				a1++;
				continue;
			case '2':
				a2++;
				continue;
			}
			error(usage, "");
		case 'e':
			null = optarg;
			continue;
		case 't':
			sep1 = sep2 = onelet(optarg);
			continue;
		case '1':
			j1 = xatoi(optarg);
			continue;
		case '2':
			j2 = xatoi(optarg);
			continue;
		case 'j':
			fprintf(stderr, jcom);
			if(optarg[-1] != 'j') {
				j1 = j2 = xatoi(optarg);
				continue;
			}
			switch(onelet(optarg)) {
			case '1':
				j1 = xatoi(argv[optind++]);
				continue;
			case '2':
				j2 = xatoi(argv[optind++]);
				continue;
			}
			error("improper -j", "");
		case 'o':
			optiono(argc, argv);
			continue;
		}
		break;
	}
proceed:
	if (argc-optind != 2)
		error(usage,"");
	for (i = 0; i < no; i++)
		if (--olist[i] >= NFLD)	/* 0 origin */
			error("field number too big in -o","");
	if(--j1<0 || --j2<0)
		error("join field number not positive", "");
	if(j1>=NFLD || j2>=NFLD)
		error("join field number too big", "");
	if (strcmp(argv[optind], "-") == 0)
		f[F1] = stdin;
	else if ((f[F1] = fopen(argv[optind], "r")) == 0)
		error("can't open %s", argv[optind]);
	if(strcmp(argv[optind+1], "-") == 0) {
		f[F2] = stdin;
	} else if ((f[F2] = fopen(argv[optind+1], "r")) == 0)
		error("can't open %s", argv[optind+1]);

	if(ftell(f[F2]) >= 0)
		seek2();
	else if(ftell(f[F1]) >= 0)
		seek1();
	else
		error("sorry, need one random-access file","");
	if (discard)
		error("some input line was truncated", "");
	return 0;
}

/* lazy.  there ought to be a clean way to combine seek1 & seek2 */
#define get1() n1=input(F1)
#define get2() n2=input(F2)
void
seek2()
{
	int n1, n2;	/* number of fields in each record */
	long top2;
	long bot2 = ftell(f[F2]);
	get1();
	get2();
	while(n1>0 && n2>0 || (a1||a2) && n1+n2>0) {
		if(n1>0 && n2>0 && comp()>0 || n1==0) {
			if(a2) output(0, n2);
			bot2 = ftell(f[F2]);
			get2();
		} else if(n1>0 && n2>0 && comp()<0 || n2==0) {
			if(a1) output(n1, 0);
			get1();
		} else /*(n1>0 && n2>0 && comp()==0)*/ {
			while(n2>0 && comp()==0) {
				if(!vflg) output(n1, n2);
				top2 = ftell(f[F2]);
				get2();
			}
			fseek(f[F2], bot2, 0);
			get2();
			get1();
			for(;;) {
				if(n1>0 && n2>0 && comp()==0) {
					if(!vflg) output(n1, n2);
					get2();
				} else if(n1>0 && n2>0 && comp()<0 || n2==0) {
					fseek(f[F2], bot2, 0);
					get2();
					get1();
				} else /*(n1>0 && n2>0 && comp()>0 || n1==0)*/{
					fseek(f[F2], top2, 0);
					bot2 = top2;
					get2();
					break;
				}
			}
		}
	}
}
void
seek1()
{
	int n1, n2;	/* number of fields in each record */
	long top1;
	long bot1 = ftell(f[F1]);
	get1();
	get2();
	while(n1>0 && n2>0 || (a1||a2) && n1+n2>0) {
		if(n1>0 && n2>0 && comp()>0 || n1==0) {
			if(a2) output(0, n2);
			get2();
		} else if(n1>0 && n2>0 && comp()<0 || n2==0) {
			if(a1) output(n1, 0);
			bot1 = ftell(f[F1]);
			get1();
		} else /*(n1>0 && n2>0 && comp()==0)*/ {
			while(n2>0 && comp()==0) {
				if(!vflg) output(n1, n2);
				top1 = ftell(f[F1]);
				get1();
			}
			fseek(f[F1], bot1, 0);
			get2();
			get1();
			for(;;) {
				if(n1>0 && n2>0 && comp()==0) {
					if(!vflg) output(n1, n2);
					get1();
				} else if(n1>0 && n2>0 && comp()>0 || n1==0) {
					fseek(f[F1], bot1, 0);
					get2();
					get1();
				} else /*(n1>0 && n2>0 && comp()<0 || n2==0)*/{
					fseek(f[F1], top1, 0);
					bot1 = top1;
					get1();
					break;
				}
			}
		}
	}
}

int
input(int n)		/* get line, split, return field count */
{
	register int i, c;
	char *bp;
	char **pp;

	bp = buf[n];
	pp = ppi[n];
	pp[j1] = pp[j2] = "";	/* for absent join field */
	if (fgets(bp, BUFSIZ, f[n]) == 0)
		return(0);
	i = 0;
	do {
		i++;
		if (sep1 == ' ')	/* strip multiples */
			while ((c = *bp) == sep1 || c == sep2)
				bp++;	/* skip blanks */
		else
			c = *bp;
		*pp++ = bp;	/* record beginning */
		while ((c = *bp) != sep1 && c != '\n' && c != sep2 && c != '\0')
			bp++;
		*bp++ = '\0';	/* mark end by overwriting blank */
	} while (c != '\n' && c != '\0' && i < NFLD-1);
	if (c != '\n')
		discard++;
	return(i);
}

void
output(int on1, int on2)	/* print items from olist */
{
	int i;
	char *temp;

	if (no <= 0) {	/* default case */
		printf("%s", j1<on1? ppi[F1][j1]:
			     j2<on2? ppi[F2][j2]: null);
		for (i = 0; i < on1; i++)
			if (i != j1)
				printf("%c%s", sep1, ppi[F1][i]);
		for (i = 0; i < on2; i++)
			if (i != j2)
				printf("%c%s", sep1, ppi[F2][i]);
		printf("\n");
	} else {
		for (i = 0; i < no; i++) {
			temp = null;
			switch(olistf[i]) {
			case F0:
				if(j1 < on1)
					temp = ppi[F1][j1];
				else if(j2 < on2)
					temp = ppi[F2][j2];
				break;
			case F1:
				if(olist[i] < on1)
					temp = ppi[F1][olist[i]];
				break;
			case F2:
				if(olist[i] < on2)
					temp = ppi[F2][olist[i]];
				break;
			}
			printf("%s", temp);
			if (i == no - 1)
				printf("\n");
			else
				printf("%c", sep1);
		}
	}
}

void
error(char *s1, char *s2)
{
	fprintf(stderr, "join: ");
	fprintf(stderr, s1, s2);
	fprintf(stderr, "\n");
	exit(1);
}

char *
getoptarg(int *argcp, char ***argvp)
{
	int argc = *argcp;
	char **argv = *argvp;
	if(argv[1][2] != 0)
		return &argv[1][2];
	if(argc<=2 || argv[2][0]=='-')
		error("incomplete option %s", argv[1]);
	*argcp = --argc;
	*argvp = ++argv;
	return argv[1];
}

void
oparse(char *s)
{
	for (no = 0; no<2*NFLD && *s; no++, s++) {
		switch(*s) {
		case 0:
			return;
		case '0':
			olistf[no] = F0;
			break;
		case '1':
		case '2':
			if(s[1] == '.' && isdigit(s[2])) {
				olistf[no] = *s=='1'? F1: F2;
				olist[no] = xatoi(s += 2);
				break;
			} /* fall thru */
		default:
			error("invalid -o list", "");
		}
		if(s[1] == ',')
			s++;
	}
}

int
xatoi(char *s)
{
	if(!isdigit(*s))
		error("numeric argument expected", "");
	return atoi(s);
}

int
onelet(char *s)
{
	if(s[0] == 0 || s[1] != 0)
		error(usage, "");
	return s[0];
}	

void
optiono(int argc, char **argv)
{
	static multi;
	int f;
	for (no=0; *optarg; no++) {
		if(no >= 2*NFLD)
			error("too many output fields", "");
		f = *optarg++;
		f = f=='0'? F0: f=='1'? F1: f=='2'? F2: FX;
		if(f == FX)
			error("improper -o", "");
		olistf[no] = f;
		if(f != F0) {
			if(*optarg++ != '.')
				error("improper -o", "");
			olist[no] = xatoi(optarg);
			while(isdigit(*optarg))
				optarg++;
		}
		if(*optarg==',' || *optarg==' ')
			optarg++;
		else if(*optarg==0 &&
			(multi || no==0) &&
			optind<argc-2 &&
			isdigit(*argv[optind])) {
				optarg = argv[optind++];
				if(multi++ == 0)
					fprintf(stderr, ocom);
			}
	}
}
