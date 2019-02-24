#include <u.h>
#include <libc.h>
#include <ctype.h>
#ifdef plan9
Dir	mbuf;
#else
#define print	printf
struct stat mbuf;
#define OREAD	0
#endif
/*
 * file - determine type of file
 */

uchar	buf[6000];
short	cfreq[140];
short	wfreq[50];
int	nbuf;
int	flag;
int	(*call[])(void);

enum
{
	Cword,
	Fword,
	Aword,
	I1,
	I2,
	I3,
	Clatin	= 128,
	Cbinary,
	Cnull,
	Ceascii,
};
struct
{
	char*	word;
	int	flag;
} dict[] =
{
	"TEXT",		Aword,
	"block",	Fword,
	"char",		Cword,
	"common",	Fword,
	"data",		Fword,
	"dimension",	Fword,
	"double",	Cword,
	"extern",	Cword,
	"fio",		I2,
	"float",	Cword,
	"function",	Fword,
	"h",		I3,
	"include",	I1,
	"int",		Cword,
	"integer",	Fword,
	"libc",		I2,
	"long",		Cword,
	"real",		Fword,
	"register",	Cword,
	"short",	Cword,
	"static",	Cword,
	"stdio",	I2,
	"struct",	Cword,
	"subroutine",	Fword,
	"u",		I2,
	"void",		Cword,
};

enum
{
	Short	= 1<<0,		/* size < 100 */
	Long	= 1<<1,

	Fascii	= 1<<2,		/* printable ascii */
	Flatin	= 1<<3,
	Fbinary	= 1<<4,
	Feascii	= 1<<5,		/* including extended */
	Fnull	= 1<<6,
};

void	type(char*, int);
long	lendian(uchar*);

int
main(int argc, char *argv[])
{
	int i, l;

	l = 0;
	for(i=1; i<argc; i++)
		if(strlen(argv[i]) > l)
			l = strlen(argv[i]);
	for(i=1; i<argc; i++)
		type(argv[i], l);
	exit(0);
}

void
type(char *file, int nlen)
{
	int i, f, l, m, c;
	char *p, *ep, word[20];

	print("%s:%*s", file, nlen-strlen(file)+1, "");
#ifdef plan9
	if(dirstat(file, &mbuf) < 0) {
		print("cannot stat\n");
		return;
	}
	if(mbuf.mode & CHDIR) {
		print("directory\n");
		return;
	}
	if(mbuf.type != 'M') {
		print("special file #%c\n", mbuf.type);
		return;
	}
#else
	if(stat(file, &mbuf) < 0) {
		print("cannot stat\n");
		return;
	}
	switch(mbuf.st_mode&S_IFMT) {
	case S_IFDIR:
		print("directory\n");
		return;
	case S_IFCHR:
		print("character special file\n");
		return;
	case S_IFBLK:
		print("block special file\n");
		return;
	}
#endif

	f = open(file, OREAD);
	if(f < 0) {
		print("cannot open\n");
		return;
	}
	nbuf = read(f, buf, sizeof(buf));
	close(f);

	if(nbuf < 0) {
		print("cannot read\n");
		return;
	}
	if(nbuf == 0) {
		print("empty\n");
		return;
	}

	/*
	 * build histogram table
	 */
	memset(cfreq, 0, sizeof(cfreq));
	flag = 0;
	if(nbuf > 100)
		flag |= Long;
	else
		flag |= Short;

	for(i=0; i<nbuf; i++) {
		f = buf[i] & 0xff;
		if(f >= 128) {
			if(f >= 128+32)
				f = Clatin;	/* latin */
			else
				f = Cbinary;	/* not latin */
		} else
		if(!isprint(f) && !isspace(f))
			if(f == 0)
				f = Cnull;
			else
				f = Ceascii;
		cfreq[f]++;
	}

	/*
	 * gross classify
	 */
	if(cfreq[Cbinary])
		flag |= Fbinary;
	else
	if(cfreq[Clatin])
		flag |= Flatin;
	else
	if(cfreq[Ceascii])
		flag |= Feascii;
	else
	if(cfreq[Cnull])
		flag |= Fnull;
	else
		flag |= Fascii;

	if(flag & Fnull) {
		print("null\n");
		return;
	}

	/*
	 * lookup dictionary words
	 */
	memset(wfreq, 0, sizeof(wfreq));
	if(flag & Fascii) {
		ep = word+sizeof(word)-2;
		for(i=0; i<nbuf; i++) {
			f = buf[i];
			if(!isalpha(f))
				continue;
			p = word;
			for(; i<nbuf; i++) {
				f = buf[i];
				if(!isalnum(f))
					break;
				*p++ = f;
				if(p >= ep)
					break;
			}
			*p = 0;
			f = 0;
			l = sizeof(dict)/sizeof(dict[0]);
			for(;;) {
				if(f >= l)
					break;
				m = (f+l)/2;
				c = strcmp(dict[m].word, word);
				if(c == 0) {
					wfreq[dict[m].flag]++;
					break;
				}
				if(c < 0)
					f = m+1;
				else
					l = m;
			}
		}
	}

	/*
	 * call individual classify routines
	 */
	for(i=0; call[i]; i++)
		if((*call[i])())
			return;

	/*
	 * if all else fails,
	 * print out gross classification
	 */
	if(flag & Short)
		print("short ");
	if(flag & Fascii)
		print("ascii\n");
	else
	if(flag & Feascii)
		print("extended ascii\n");
	else
	if(flag & Flatin)
		print("latin ascii\n");
	else
		print("binary\n");
}

long
lendian(uchar *p)
{

	return (p[0]) |
		(p[1] << 8) |
		(p[2] << 16) |
		(p[3] << 24);
}

int
long0(void)
{

	switch((unsigned)lendian(buf)) {
	default:
		return 0;

	case 0413:
		print("demand paged ");

	case 0410:
		print("pure ");
		goto exec;

	case 0406:
		print("mpx 68000 ");
		goto exec;

	exec:
	case 0407:
		print("unix vax executable");
		if(lendian(buf+4) != 0)
			print(" not stripped");
		print("\n");
		break;

	case 0411:
		print("jfr 411 executable\n");
		break;

	case 0177555:
		print("very old archive\n");
		break;

	case 0177545:
		print("old archive\n");
		break;

	case 0135246:		/* andrew/ehg */
		print("view2d input file\n");
		break;

	case 0135256:		/* andrew */
		print("apl file\n");
		break;

	case 0164200:		/* td */
		print("Lucasfilm picture\n");
		break;

 	case 0600560:
		print("mux downloadable file\n");
		break;

	case 0x07010000:
		print("68020 plan9 executable\n");
		break;

	case 0x07040000:
		print("mips plan9 executable\n");
		break;

	case 0x97010000:
		print("hobbit plan9 executable\n");
		break;

	case 0xab020000:
		print("sparc plan9 executable\n");
		break;

	case 0xeb010000:
		print("386 plan9 executable\n");
		break;
	case 0x0b1f1bdc:
		print("daisy\n");
		break;
	case 0x64205300:
		print("S data object\n");
		break;
	}
	return 1;
}

int
short0(void)
{

	switch(lendian(buf) & 0xffff) {
	default:
		return 0;

	case 070707:
		print("cpio archive\n");
		break;

	case 0x02f7:
		print("tex dvi\n");
		break;

	case 0405:
	case 0407:
	case 0410:
	case 0411:
		print("pdp-11 executable\n");
		break;
	case 0x0000:
		print("bitmap\n");
		break;
	}
	return 1;
}

/*
 * initial words to classify file
 */
char*	iwords[] =
{
	"!<arch>\n__.SYMDEF",
		"archive random library",
	"!<arch>\n",
		"archive",
	"070707",
		"cpio archive - ascii header",
	"#FIG",
		"fig ouput",
	"#!/bin/echo",
		"cyntax object file",
	"#!/bin/rc",
		"rc executable file",
	"#!/bin/sh",
		"sh executable file",
	"%!",
		"postscript",
	"@document(",
		"imagen",
	"x T i300",
		"troff output for i300",
	"x T im300",
		"troff output for im300",
	"x T post",
		"troff output for post",
	"x T opost",
		"troff output for opost",
	"x T Latin1",
		"troff output for Latin1",
	"x T 202",
		"troff output for 202",
	"x T aps",
		"troff output for aps",
	0,0
};

int
istring(void)
{
	int i, n;
	char *p;

	for(i=0; p=iwords[i]; i+=2) {
		n = strlen(p);
		if(nbuf >= n && !strncmp((char*)buf, p, n)) {
			print("%s\n", iwords[i+1]);
			return 1;
		}
	}
	if(strncmp((char*)buf, "TYPE=", 5) == 0) {	/* td */
		for(i=5; i<nbuf; i++)
			if(buf[i] == '\n')
				break;
		print("%.*s picture\n", i-5, buf+5);
		return 1;
	}
	return 0;
}

/*
 * low entropy means encrypted
 */
int
ismung(void)
{
	int i, bucket[8];
	float cs;

	if(nbuf < 64)
		return 0;
	memset(bucket, 0, sizeof(bucket));
	for(i=0; i<64; i++)
		bucket[(buf[i]>>5)&07] += 1;

	cs = 0.;
	for(i=0; i<8; i++)
		cs += (bucket[i]-8)*(bucket[i]-8);
	cs /= 8.;
	if(cs <= 24.322) {
		if(buf[0]==037 && buf[1]==0235)
			print("compressed\n");
		else
			print("encrypted\n");
		return 1;
	}
	return 0;
}

/*
 * english by punctuation and frequencies
 */
int
isenglish(void)
{
	int i, vow, comm, rare, badpun, punct;
	char *p;

	if(!(flag & (Fascii|Feascii)))
		return 0;
	badpun = 0;
	punct = 0;
	for(i=0; i<nbuf-1; i++)
		switch(buf[i]) {
		case '.':
		case ',':
		case ')':
		case '%':
		case ';':
		case ':':
		case '?':
			punct++;
			if(buf[i+1] != ' ' && buf[i+1] != '\n')
				badpun++;
		}
	if(badpun*5 > punct)
		return 0;
	if(cfreq['>']+cfreq['<']+cfreq['/'] > cfreq['e'])	/* shell file test */
		return 0;
	if(2*cfreq[';'] > cfreq['e'])
		return 0;

	vow = 0;
	for(p="AEIOU"; *p; p++) {
		vow += cfreq[*p];
		vow += cfreq[tolower(*p)];
	}
	comm = 0;
	for(p="ETAION"; *p; p++) {
		comm += cfreq[*p];
		comm += cfreq[tolower(*p)];
	}
	rare = 0;
	for(p="VJKQXZ"; *p; p++) {
		rare += cfreq[*p];
		rare += cfreq[tolower(*p)];
	}
	if(vow*5 >= nbuf-cfreq[' '] && comm >= 10*rare) {
		print("English text\n");
		return 1;
	}
	return 0;
}

int
isc(void)
{
	int n;

	n = wfreq[I1];
	/*
	 * includes
	 */
	if(n >= 2 && wfreq[I2] >= n && wfreq[I3] >= n && cfreq['.'] >= n)
		goto yes;
	/*
	 * declarations
	 */
	if(wfreq[Cword] >= 5 && cfreq[';'] >= 5)
		goto yes;
	/*
	 * assignments
	 */
	if(cfreq[';'] >= 10 && cfreq['='] >= 10)
		goto yes;
	return 0;

yes:
	print("c program text\n");
	return 1;
}

int
isas(void)
{

	/*
	 * includes
	 */
	if(wfreq[Aword] >= 2)
		goto yes;
	return 0;

yes:
	print("assembler program text\n");
	return 1;
}

int
iscint(void)
{

	if(buf[0] == 0x3a)			/* as = ANAME */
	if(buf[1] == 0x11)			/* type = D_FILE */
	if(buf[2] == 1)				/* sym */
	if(buf[3] == '<') {			/* name of file */
		print("mips .v intermediate\n");
		return 1;
	}

	if(buf[0] == 0x4d)			/* aslo = ANAME */
	if(buf[1] == 0x01)			/* ashi = ANAME */
	if(buf[2] == 0x32)			/* type = D_FILE */
	if(buf[3] == 1)				/* sym */
	if(buf[4] == '<') {			/* name of file */
		print("68020 .2 intermediate\n");
		return 1;
	}

	if(buf[0] == 0x43)			/* as = ANAME */
	if(buf[1] == 0x0d)			/* type */
	if(buf[2] == 1)				/* sym */
	if(buf[3] == '<') {			/* name of file */
		print("hobbit .z intermediate\n");
		return 1;
	}

	if(buf[0] == 0x74)			/* as = ANAME */
	if(buf[1] == 0x10)			/* type */
	if(buf[2] == 1)				/* sym */
	if(buf[3] == '<') {			/* name of file */
		print("sparc .k intermediate\n");
		return 1;
	}

	if(buf[0] == 0x7e)			/* aslo = ANAME */
	if(buf[1] == 0x00)			/* ashi = ANAME */
	if(buf[2] == 0x45)			/* type = D_FILE */
	if(buf[3] == 1)				/* sym */
	if(buf[4] == '<') {			/* name of file */
		print("386 .8 intermediate\n");
		return 1;
	}

	return 0;
}

/*
 * pick up a number with
 * syntax _*[0-9]+_
 */
#define	P9BITLEN	12
int
p9bitnum(uchar *bp)
{
	int n, c, len;

	len = P9BITLEN;
	while(*bp == ' ') {
		bp++;
		len--;
		if(len <= 0)
			return -1;
	}
	n = 0;
	while(len > 1) {
		c = *bp++;
		if(!isdigit(c))
			return -1;
		n = n*10 + c-'0';
		len--;
	}
	if(*bp != ' ')
		return -1;
	return n;
}

int
isp9bit(void)
{
	int ldep, lox, loy, hix, hiy;
	long len;

	ldep = p9bitnum(buf + 0*P9BITLEN);
	lox = p9bitnum(buf + 1*P9BITLEN);
	loy = p9bitnum(buf + 2*P9BITLEN);
	hix = p9bitnum(buf + 3*P9BITLEN);
	hiy = p9bitnum(buf + 4*P9BITLEN);

	if(ldep < 0 || lox < 0 || loy < 0 || hix < 0 || hiy < 0)
		return 0;

	len = (hix-lox) * (1<<ldep);	/* row length */
	len = (len + 7) / 8;		/* rounded to bytes */
	len *= (hiy-loy);		/* col length */
	len += 60;			/* size of initial ascii */

	/*
	 * for regular file length is non-zero and must match calculation above
	 * for /dev/window and /dev/screen the length is always zero
	 */
#ifdef plan9
	if(mbuf.length != len && mbuf.length != 0)
#else
	if(mbuf.st_size != len && mbuf.st_size != 0)
#endif	
		return 0;
	print("plan 9 bitmap\n");
	return 1;
}

int	(*call[])(void) =
{
	long0,		/* recognizable by first 4 bytes */
	short0,		/* recognizable by first 2 bytes */
	istring,	/* recognizable by first string */
	iscint,		/* c intermediate */
	isc,		/* c compiler key words */
	isas,		/* assembler key words */
	ismung,		/* entropy compressed/encrypted */
	isenglish,	/* char frequency English */
	isp9bit,	/* plan 9 bitmap (as from /dev/window) */
	0
};
