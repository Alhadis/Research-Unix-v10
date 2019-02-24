#include <fio.h>

unsigned char	odata[16];
unsigned char	data[16];
int		ndata;
unsigned long	addr;
Fbuffer		buf;
int		repeats;
int		swizzle;
int		abase=2;

enum{
	Narg=10
};
typedef struct Arg Arg;
struct Arg{
	int	ascii;		/* 0==none, 1==ascii */
	int	loglen;		/* 0==1, 1==2, 2==4 */
	int	base;		/* 0==8, 1==10, 2==16 */
	int	(*fn)();	/* function to call with data */
	char	*afmt;		/* format to use to print address */
	char	*fmt;		/* format to use to print data */
}arg[Narg];
int	narg;

extern int	fmt0(), fmt1(), fmt2(), fmtc();

int (*fmt[3])()={
	fmt0,	fmt1,	fmt2,
};
char *dfmt[3][3]={
	" %.3uo",	" %.3ud",	" %.2ux",
	" %.6uo",	" %.5ud",	" %.4ux",
	" %.11luo",	" %.10lud",	" %.8lux",
};
char *cfmt[3][3]={
	"   %c",	"   %c", 	"  %c",
	" %.3s",	" %.3s",	" %.2s",
	" %.3uo",	" %.3ud",	" %.2ux",
};
char *afmt[2][3]={
	"%.7luo ",	"%.7lud ",	"%.7lux ",
	"%7luo ",	"%7lud ",	"%7lux ",
};
main(argc, argv)
	char *argv[];
{
	int err=0;
	register Arg *ap;
	while(argc>1 && argv[1][0]=='-' && argv[1][1]){
		--argc;
		argv++;
		argv[0]++;
		if(argv[0][0]=='r'){
			repeats=1;
			if(argv[0][1])
				goto Usage;
			continue;
		}
		if(argv[0][0]=='s'){
			swizzle=1;
			if(argv[0][1])
				goto Usage;
			continue;
		}
		if(argv[0][0]=='a'){
			argv[0]++;
			switch(argv[0][0]){
			case 'o':
				abase=0;
				break;
			case 'd':
				abase=1;
				break;
			case 'x':
				abase=2;
				break;
			default:
				goto Usage;
			}
			if(argv[0][1])
				goto Usage;
			continue;
		}
		ap=&arg[narg];
		initarg();
		while(argv[0][0]){
			switch(argv[0][0]){
			case 'c':
				ap->ascii=1;
				ap->loglen=0;
				if(argv[0][1] || argv[0][-1]!='-')
					goto Usage;
				break;
			case 'o':
				ap->base=0;
				break;
			case 'd':
				ap->base=1;
				break;
			case 'x':
				ap->base=2;
				break;
			case 'b':
			case '1':
				ap->loglen=0;
				break;
			case 'w':
			case '2':
				ap->loglen=1;
				break;
			case 'l':
			case '4':
				ap->loglen=2;
				break;
			default:
			Usage:
   fprint(2, "usage: xd [-r] [-s] [-a{odx}] [-c|{b1w2l4}{odx}] ... file ...\n");
				return 1;
			}
			argv[0]++;
		}
		if(ap->ascii)
			ap->fn=fmtc;
		else
			ap->fn=fmt[ap->loglen];
		ap->fmt=dfmt[ap->loglen][ap->base];
		ap->afmt=afmt[ap>arg][abase];
	}
	if(narg==0)
		initarg();
	if(argc==1)
		err=xd((char *)0, 0);
	else if(argc==2)
		err=xd(argv[1], 0);
	else while(argc-->1)
		err|=xd(argv[1], 1);
	return err;
}
initarg(){
	register Arg *ap;
	ap=&arg[narg++];
	if(narg>=Narg){
		fprint(2, "xd: too many formats (max %d)\n", Narg);
		exit(1);
	}
	ap->ascii=0;
	ap->loglen=2;
	ap->base=2;
	ap->fn=fmt2;
	ap->fmt=dfmt[ap->loglen][ap->base];
	ap->afmt=afmt[narg>1][abase];
}
xd(name, title)
	char *name;
	int title;
{
	register fd=name? open(name, 0) : 0;
	register i, star;
	register Arg *ap;
	if(fd<0){
		fprint(2, "xd: can't open %s\n", name);
		return 1;
	}
	if(title)
		Fprint(1, "%s\n", name);
	Finit(fd, &buf);
	addr=0;
	star=0;
	while((ndata=Fread(fd, data, 16L))>=0){
		if(ndata<16)
			for(i=ndata; i<16; i++)
				data[i]=0;
		if(swizzle)
			swizz();
		if(ndata==16 && repeats){
			if(addr>0 && data[0]==odata[0]){
				for(i=1; i<16; i++)
					if(data[i]!=odata[i])
						break;
				if(i==16){
					addr+=16;
					if(star==0){
						star++;
						Fprint(1, "*\n");
					}
					continue;
				}
			}
			for(i=0; i<16; i++)
				odata[i]=data[i];
			star=0;
		}
		for(ap=arg; ap<&arg[narg]; ap++){
			Fprint(1, ap->afmt, addr);
			(*ap->fn)(ap->fmt);
			Fprint(1, "\n");
		}
		addr+=ndata;
		if(ndata<16){
			Fprint(1, afmt[0][abase], addr);
			Fprint(1, "\n");
			break;
		}
	}
	return 0;
}
swizz(){
	unsigned char *p, *q;
	int i;
	unsigned char swdata[16];
	p=data;
	q=swdata;
	for(i=0; i<16; i++)
		*q++=*p++;
	p=data;
	q=swdata;
	for(i=0; i<4; i++, q+=4){
		*p++=q[3];
		*p++=q[2];
		*p++=q[1];
		*p++=q[0];
	}
}
fmt0(f)
	char *f;
{
	register i;
	for(i=0; i<ndata; i++)
		Fprint(1, f, data[i]);
}
fmt1(f)
	char *f;
{
	register i;
	for(i=0; i<ndata; i+=sizeof(unsigned short))
		Fprint(1, f, *(unsigned short *)&data[i]);
}
fmt2(f)
	char *f;
{
	register i;
	for(i=0; i<ndata; i+=sizeof(unsigned long))
		Fprint(1, f, *(unsigned long *)&data[i]);
}
fmtc()
{
	register i;
	for(i=0; i<ndata; i++)
		switch(data[i]){
		case '\t':
			Fprint(1, cfmt[1][2], "\\t");
			break;
		case '\r':
			Fprint(1, cfmt[1][2], "\\r");
			break;
		case '\n':
			Fprint(1, cfmt[1][2], "\\n");
			break;
		case '\b':
			Fprint(1, cfmt[1][2], "\\b");
			break;
		default:
			if(data[i]>=0x7F || ' '>data[i])
				Fprint(1, cfmt[2][2], data[i]);
			else
				Fprint(1, cfmt[0][2], data[i]);
			break;
		}
}
