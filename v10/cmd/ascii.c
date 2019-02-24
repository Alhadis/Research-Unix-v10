#include <stdio.h>
char *str[128]={
	"nul",	"soh",	"stx",	"etx",	"eot",	"enq",	"ack",	"bel",
	"bs ",	"ht ",	"nl ",	"vt ",	"np ",	"cr ",	"so ",	"si ",
	"dle",	"dc1",	"dc2",	"dc3",	"dc4",	"nak",	"syn",	"etb",
	"can",	"em ",	"sub",	"esc",	"fs ",	"gs ",	"rs ",	"us ",
	"sp ",	" ! ",	" \" ",	" # ",	" $ ",	" % ",	" & ",	" ' ",
	" ( ",	" ) ",	" * ",	" + ",	" , ",	" - ",	" . ",	" / ",
	" 0 ",	" 1 ",	" 2 ",	" 3 ",	" 4 ",	" 5 ",	" 6 ",	" 7 ",
	" 8 ",	" 9 ",	" : ",	" ; ",	" < ",	" = ",	" > ",	" ? ",
	" @ ",	" A ",	" B ",	" C ",	" D ",	" E ",	" F ",	" G ",
	" H ",	" I ",	" J ",	" K ",	" L ",	" M ",	" N ",	" O ",
	" P ",	" Q ",	" R ",	" S ",	" T ",	" U ",	" V ",	" W ",
	" X ",	" Y ",	" Z ",	" [ ",	" \\ ",	" ] ",	" ^ ",	" _ ",
	" ` ",	" a ",	" b ",	" c ",	" d ",	" e ",	" f ",	" g ",
	" h ",	" i ",	" j ",	" k ",	" l ",	" m ",	" n ",	" o ",
	" p ",	" q ",	" r ",	" s ",	" t ",	" u ",	" v ",	" w ",
	" x ",	" y ",	" z ",	" { ",	" | ",	" } ",	" ~ ",	"del",
};
#define	MAXBASE	36
char Ncol[]={
    0,0,7,5,4,4,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
};
int 	base=8;
int 	ncol;
int 	text=1;
int	strip=0;
extern	int	optind;
extern	int	opterr;
extern	char	*optarg;
extern	int	optopt;
main(argc, argv)
	char *argv[];
{
	register i;
	opterr = 0;
	for(;;)
		switch(getopt(argc, argv, "xodntceb:")){
		case 'x':
			base=16;
			break;
		case 'o':
			base=8;
			break;
		case 'd':
			base=10;
			break;
		case 'b':
			base=atoi(optarg);
			if(base<2||base>MAXBASE)
				usage();
			break;
		case 'n':
			text=0;
			break;
		case 't':
			strip=1;
		case 'c':
			text=2;
			break;
		case 'e':
		case EOF:
			argc-=optind;
			argv+=optind;
			goto Done;
		default:
			usage();
		}
 Done:
	ncol=Ncol[base];
	if(argc==0){
		for(i=0;i<128;i++){
			put(i);
			if((i&7)==7)
				puts("|\n");
		}
	} else {
		if(text==1)
			text=isnum(argv[0]);
		while(argc--)
			if(text)
				puttext(*argv++);
			else
				putnum(*argv++);
	}
	if(strip)
		putchar('\n');
}
usage(){
	fprintf(stderr, "usage: ascii [-xod | -b8] [-nct] [-e] [text]\n");
	exit(1);
}
put(i){
	putchar('|');
	putn(i, ncol);
	putchar(' ');
	puts(str[i]);
}
char dig[]="0123456789abcdefghijklmnopqrstuvwxyz";
putn(n, ndig)
	register n;
{
	if(ndig==0)
		return;
	putn(n/base, ndig-1);
	putchar(dig[n%base]);
}
puts(s)
	register char *s;
{
	fputs(s, stdout);	/* no newline!! */
}
puttext(s)
	register char *s;
{
	register n;
	n=btoi(s)&0177;
	if(strip)
		putchar(n);
	else{
		puts(str[n]);
		putchar('\n');
	}
}
putnum(s)
	register char *s;
{
	while(*s){
		putn(*s++&0177, ncol);
		putchar('\n');
	}
}
btoi(s)
	register char *s;
{
	register n;
	n=0;
	while(*s)
		n=n*base+value(*s++, 0);
	return(n);
}
value(c, f)
	register c;
{
	register char *s;
	for(s=dig; s<dig+base; s++)
		if(*s==c)
			return(s-dig);
	if(f)
		return(-1);
	fputs("ascii: bad input char ", stderr);
	putc(c, stderr);
	putc('\n', stderr);
	exit(1);
}
isnum(s)
	register char *s;
{
	while(*s)
		if(value(*s++, 1)==-1)
			return(0);
	return(1);
}
