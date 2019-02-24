static	char sccsid[] = "@(#)file.c 4.1 10/1/80";
/*
 * file - determine type of file
 */

#include <pagsiz.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ctype.h>
#include <a.out.h>
int in;
int i  = 0;
unsigned char buf[BUFSIZ];
char *troff[] = {	/* new troff intermediate lang */
	"x","T","res","init","font","202","V0","p1",0};
char *fort[] = {
	"function","subroutine","common","dimension","block","integer",
	"real","data","double",0};
char *asc[] = {
	"chmk","mov","tst","clr","jmp",0};
char *c[] = {
	"int","char","float","double","struct",
	"extern","static","typedef",0};
char *as[] = {
	"globl","byte","align","text","data","comm",0};
int	ifile;

main(argc, argv)
char **argv;
{
	FILE *fl;
	register char *p;
	char ap[128];
	extern char _sobuf[];

	if (argc>1 && argv[1][0]=='-' && argv[1][1]=='f') {
		if ((fl = fopen(argv[2], "r")) == NULL) {
			printf("Can't open %s\n", argv[2]);
			exit(2);
		}
		while ((p = fgets(ap, 128, fl)) != NULL) {
			int l = strlen(p);
			if (l>0)
				p[l-1] = '\0';
			printf("%s:	", p);
			type(p);
			if (ifile>=0)
				close(ifile);
		}
		exit(1);
	}
	while(argc > 1) {
		printf("%s:	", argv[1]);
		type(argv[1]);
		fflush(stdout);
		argc--;
		argv++;
		if (ifile >= 0)
			close(ifile);
	}
}

type(file)
char *file;
{
	int j,nl;
	char ch;
	struct stat mbuf;

	ifile = -1;
	if(stat(file, &mbuf) < 0) {
		if(lstat(file,&mbuf)>=0 && (mbuf.st_mode&S_IFMT)==S_IFLNK)
			printf("symbolic link, cannot stat\n");
		else
			printf("cannot stat\n");
		return;
	}
	switch (mbuf.st_mode & S_IFMT) {

	case S_IFCHR:
		printf("character");
		goto spcl;

	case S_IFDIR:
		printf("directory\n");
		return;

	case S_IFBLK:
		printf("block");

spcl:
		printf(" special (%d/%d)\n", major(mbuf.st_rdev), minor(mbuf.st_rdev));
		return;
	}

	ifile = open(file, 0);
	if(ifile < 0) {
		printf("cannot open\n");
		return;
	}
	in = read(ifile, buf, BUFSIZ);
	if(in == 0){
		printf("empty\n");
		return;
	}

	if(in>=64){

		int i, bucket[8];
		float cs;

		for(i=0; i<8; i++) bucket[i] = 0;

		for(i=0; i<64; i++) bucket[(buf[i]>>5)&07] += 1;

		cs = 0.;
		for(i=0; i<8; i++) cs += (bucket[i]-8)*(bucket[i]-8);
		cs /= 8.;

		if(cs <= 24.322){

			if(buf[0]==037 && buf[1]==0235) {
				char * p = file + strlen(file);
				if(p[-1]=='Z' && p[-2]=='.') {
					printf("compressed\n");
					return;
				}
			}
			printf("encrypted\n");
			return;
		}
	}

	switch (*(short *) buf)
	{
	case 070707:
cpio:
		printf("cpio archive\n");
		goto out;
	}

	switch(*(int *)buf)
	{
	case 0413:
		printf("demand paged ");

	case 0410:
		printf("pure ");
		goto exec;

	case 0411:
		printf("jfr 411 executable\n");
		return;
	
	case 0406:
		printf ("mpx 68000 ");
		goto exec;

	case 0407:
exec:
		printf("executable");
		if(((int *)buf)[4] != 0) {
			printf(" not stripped");
			if(oldo(buf))
				printf(" (old format symbol table)");
		}
		if(*(int *)buf==0413 && ((int *)buf)[1]-((int *)buf)[5]==0x11F)
			printf(" infected");
		printf("\n");
		goto out;

	case 0177555:
		printf("very old archive\n");
		goto out;

	case 0177545:
		printf("old archive\n");
		goto out;

	case 0135246:		/* andrew/ehg */
		printf("view2d input file\n");
		goto out;

	case 0135256:		/* andrew */
		printf("apl file\n");
		goto out;

	case 0164200:		/* td */
		printf("Lucasfilm picture\n");
		goto out;

 	case 0600560:
		printf("mux downloadable file\n");
		goto out;

	default:
		switch (*(short *) buf+1) {
		case 0405:
		case 0407:
		case 0410:
		case 0411:
			printf ("pdp-11 executable\n");
			goto out;
		}
	}

	if (in >= 32 && buf[0] == 0x00 && buf[1] == 0x00) {
		if (buf[2] == 0x01 && buf[3] == 0x07) {
			printf("68020 plan9 executable\n");
			goto out;
		}
		if (buf[2] == 0x04 && buf[3] == 0x07) {
			printf("mips plan9 executable\n");
			goto out;
		}
	}
	if(strncmp(buf, "!<arch>\n__.SYMDEF", 17) == 0 ) {
		printf("archive random library\n");
		goto out;
	}
	if (strncmp(buf, "!<arch>\n", 8)==0) {
		printf("archive\n");
		goto out;
	}
	if (strncmp(buf, "070707", 6) == 0)
	{
		printf("ascii ");
		goto cpio;
	}
	if(strncmp(buf, "#!/bin/echo ", 12)==0){
		printf("cyntax object file\n");
		goto out;
	}
	if(strncmp(buf, "#!", 2)==0) {
		if(script(buf,in))
			goto out;
	}
	if(strncmp(buf, "TYPE=", 5)==0){	/* td */
		for(i=5;i!=in && buf[i]!='\n';i++);
		printf("%.*s picture\n", i, buf);
		goto out;
	}
	if(strncmp(buf, "%!", 2)==0){
		printf("postscript\n");
		goto out;
	}
	if (strncmp(buf, "@document(", 10) == 0) {
		printf("imagen\n");
		goto out;
	}
	if (strncmp(buf, "\0334\033\037\013", 5) == 0) {
		printf("daisy\n");
		goto out;
	}
	if(buf[0] == '\0') {
		if(buf[1] == '\0'){
			printf("bitmap\n");
			goto out;
		}
		if(strncmp(buf+1,"S data\001", 7)==0){
			printf("S data object\n");
			goto out;
		}
	}
	if( (((unsigned char)buf[0])==247) && (buf[1]==2) ){
		printf("tex dvi\n");
		goto out;
	}

	i = 0;
	if(ccom() == 0)goto notc;
	while(buf[i] == '#'){
		j = i;
		while(buf[i++] != '\n'){
			if(i - j > 255){
				printf("data\n"); 
				goto out;
			}
			if(i >= in)goto notc;
		}
		if(ccom() == 0)goto notc;
	}
check:
	if(lookup(c) == 1){
		while((ch = buf[i++]) != ';' && ch != '{')if(i >= in)goto notc;
		printf("c program text");
		goto outa;
	}
	nl = 0;
	while(buf[i] != '('){
		if(buf[i] <= 0)
			goto notas;
		if(buf[i] == ';'){
			i++; 
			goto check; 
		}
		if(buf[i++] == '\n')
			if(nl++ > 6)goto notc;
		if(i >= in)goto notc;
	}
	while(buf[i] != ')'){
		if(buf[i++] == '\n')
			if(nl++ > 6)goto notc;
		if(i >= in)goto notc;
	}
	while(buf[i] != '{'){
		if(buf[i++] == '\n')
			if(nl++ > 6)goto notc;
		if(i >= in)goto notc;
	}
	printf("c program text");
	goto outa;
notc:
	i = 0;
	while(buf[i] == 'c' || buf[i] == '#'){
		while(buf[i++] != '\n')if(i >= in)goto notfort;
	}
	if(lookup(fort) == 1){
		printf("fortran program text");
		goto outa;
	}
notfort:
	i=0;
	if(ascom() == 0)goto notas;
	j = i-1;
	if(buf[i] == '.'){
		i++;
		if(lookup(as) == 1){
			printf("assembler program text"); 
			goto outa;
		}
		else if(buf[j] == '\n' && isalpha(buf[j+2])){
			printf("roff, nroff, or eqn input text");
			goto outa;
		}
	}
	while(lookup(asc) == 0){
		if(ascom() == 0)goto notas;
		while(buf[i] != '\n' && buf[i++] != ':')
			if(i >= in)goto notas;
		while(buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')if(i++ >= in)goto notas;
		j = i-1;
		if(buf[i] == '.'){
			i++;
			if(lookup(as) == 1){
				printf("assembler program text"); 
				goto outa; 
			}
			else if(buf[j] == '\n' && isalpha(buf[j+2])){
				printf("roff, nroff, or eqn input text");
				goto outa;
			}
		}
	}
	printf("assembler program text");
	goto outa;
notas:
	for(i=0; i < in; i++)if(buf[i]&0200){
		if (buf[0]=='\100' && buf[1]=='\357') {
			printf("troff (CAT) output\n");
			goto out;
		}
		if(buf[0] == 037 && buf[1] == 036){
			char *p = file;
			while(*p++);
			if(*(p-2) != 'z' || *(p-3) != '.')
				printf("probably ");
			printf("packed\n");
			goto out;
		}
		if(buf[0] == 037 && buf[1] == 0235) {
			char *p = file;
			while(*p++);
			if(*(p-2) != 'Z' || *(p-3) != '.')
				printf("probably ");
			printf("compressed\n");
			goto out;
		}
		printf("data\n"); 
		goto out; 
	}
	if (mbuf.st_mode&((S_IEXEC)|(S_IEXEC>>3)|(S_IEXEC>>6)))
		printf("shell script text");
	else if (troffint(buf, in))
		;
	else if (english(buf, in))
		printf("English text");
	else
		printf("ascii text");
outa:
	while(i < in)
		if((buf[i++]&0377) > 127){
			printf(" with garbage\n");
			goto out;
		}
	/* if next few lines in then read whole file looking for nulls ...
		while((in = read(ifile,buf,BUFSIZ)) > 0)
			for(i = 0; i < in; i++)
				if((buf[i]&0377) > 127){
					printf(" with garbage\n");
					goto out;
				}
		/*.... */
	printf("\n");
out:;
}

oldo(cp)
char *cp;
{
	struct exec ex;
	struct stat stb;

	ex = *(struct exec *)cp;
	if (fstat(ifile, &stb) < 0)
		return(0);
	if (N_STROFF(ex)+sizeof(off_t) > stb.st_size)
		return (1);
	return (0);
}

script(buf,n)
char *buf;
{
	int i,j;
	int text = 1;
	for(j=2; j<n; j++)
		if(buf[j]!=' '&&buf[j]!='\t')
			break;
	for(i=j; i<n; i++)
		if(buf[i]==' ' || buf[i]=='\n')
			goto found;
	return 0;
found:
	buf[i] = 0;
	for(i=0; i<n; i++)
		if(buf[i]>=0177)
			text = 0;
	printf("%s script%s\n",buf+j, text?" text":"");
	return 1;
}	

troffint(bp, n)
char *bp;
int n;
{
	int k, l;
	char s[14];

	i = 0;
	for (k = 0; k < 8; k++) {
		if (buf[i] == 'x' && buf[i+1] == ' ') {	/* x [T|init|font|...] */
			if (strncmp(&buf[i+2], "T ", 2) == 0) {
				l = i;
				while (l < n && buf[l++] != '\n');
				strncpy(s, &buf[i+4], l - (i+4) - 1);
				printf("troff intermediate output text for %s",s);
				i = l;
				return 1;
			}
			while (i < n && buf[i++] != '\n')
				;
		} else if ((buf[i] == 'f' || buf[i] == 's' ||
			  buf[i] == 'p' || buf[i] == 'V' || buf[i] == 'H')
				&& isdigit(buf[i+1]))	/* p1 | f2 | s3 | V4... */
			while (i < n && buf[i++] != '\n')
				;
		else
			return 0;
	}
	printf("troff intermediate output text");
	return 1;
}

lookup(tab)
char *tab[];
{
	char r;
	int k,j,l;
	while(buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')i++;
	for(j=0; tab[j] != 0; j++){
		l=0;
		for(k=i; ((r=tab[j][l++]) == buf[k] && r != '\0');k++);
		if(r == '\0')
			if(buf[k] == ' ' || buf[k] == '\n' || buf[k] == '\t'
			    || buf[k] == '{' || buf[k] == '/'){
				i=k;
				return(1);
			}
	}
	return(0);
}
ccom(){
	char cc;
	while((cc = buf[i]) == ' ' || cc == '\t' || cc == '\n')if(i++ >= in)return(0);
	if(buf[i] == '/' && buf[i+1] == '*'){
		i += 2;
		while(buf[i] != '*' || buf[i+1] != '/'){
			if(buf[i] == '\\')i += 2;
			else i++;
			if(i >= in)return(0);
		}
		if((i += 2) >= in)return(0);
	}
	if(buf[i] == '\n')if(ccom() == 0)return(0);
	return(1);
}
ascom(){
	while(buf[i] == '/'){
		i++;
		while(buf[i++] != '\n')if(i >= in)return(0);
		while(buf[i] == '\n')if(i++ >= in)return(0);
	}
	return(1);
}

english (bp, n)
char *bp;
{
# define NASC 128
	int ct[NASC], j, vow, freq, rare;
	int badpun = 0, punct = 0;
	if (n<50) return(0); /* no point in statistics on squibs */
	for(j=0; j<NASC; j++)
		ct[j]=0;
	for(j=0; j<n; j++)
	{
		if (bp[j]<NASC)
			ct[bp[j]|040]++;
		switch (bp[j])
		{
		case '.': 
		case ',': 
		case ')': 
		case '%':
		case ';': 
		case ':': 
		case '?':
			punct++;
			if ( j < n-1 &&
			    bp[j+1] != ' ' &&
			    bp[j+1] != '\n')
				badpun++;
		}
	}
	if (badpun*5 > punct)
		return(0);
	vow = ct['a'] + ct['e'] + ct['i'] + ct['o'] + ct['u'];
	freq = ct['e'] + ct['t'] + ct['a'] + ct['i'] + ct['o'] + ct['n'];
	rare = ct['v'] + ct['j'] + ct['k'] + ct['q'] + ct['x'] + ct['z'];
	if (2*ct[';'] > ct['e']) return(0);
	if ( (ct['>']+ct['<']+ct['/'])>ct['e']) return(0); /* shell file test */
	return (vow*5 >= n-ct[' '] && freq >= 10*rare);
}
