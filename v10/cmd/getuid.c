/*
 * getuid user group passwd uid gid acct bin gcid home shell
 */
#include <stdio.h>
extern char *user(), *group(), *passwd(), *uid(), *gid(), *acct(), *bin(), *gcid(), *home(), *shell();
struct op{
	char	*name;
	char *	(*fn)();
} op[] = {
	"user",         user,   /* login id */
	"group",        group,  /* group id */
	"passwd",       passwd, /* encrypted password */
	"uid",          uid,    /* numerical user id */
	"gid",          gid,    /* numerical group id */
	"acct",         acct,   /* GCOS account number */
	"bin",          bin,    /* GCOS output bin */
	"gcid",         gcid,   /* GCOS user id */
	"home",         home,   /* home directory */
	"shell",        shell,  /* default shell */
	0,              0,
};
int tagflag=0;
char buf[132];
main(argc, argv)
	char *argv[];
{
	register i;
	if(getpw(getuid(), buf)!=0){
		fprintf(stderr, "getuid: no line in /etc/passwd for %d\n", getuid());
		return (1);
	}
	if(argc>1 && argv[1][0]=='-'){
		tagflag++;
		--argc; argv++;
	}
	if(argc < 2)
		print(0);
	else{
		if(argc > 2)
			tagflag++;
		for(i=1; i<argc; i++)
			print(lookup(argv[i]));
	}
	return (0);
}
lookup(s)
	register char *s;
{
	register struct op *p;
	for(p=op; p->name; p++)
		if(strcmp(p->name, s)==0)
			return(p-op);
	fprintf(stderr, "getuid: unknown option %s\n", s);
	exit(1);
}
print(n)
	register n;
{
	if(tagflag)
		printf("%s=", op[n].name);
	printf("%s\n", (*op[n].fn)());
}
char *
field(b, n, delim)
	register char *b;
	register n, delim;
{
	register char *s, *t;
	static char rbuf[64];
	s=b;
	rbuf[0]='\0';
	while(n-- > 0){
		while(*s!=delim)
			if(*s++ == '\0')
				return(rbuf);
		s++;	/* Point past the delim */
	}
	t=rbuf;
	while(*t = *s++)
		if(*t++ == delim){
			*--t = '\0';
			break;
		}
	return(rbuf);
}
char *
whois(uid){
	static char buf[132];
	register char *p = buf;
	if(getpw(uid,buf)!=0)
		sprintf(buf, "%d", uid);
	else{
		while(*p!=':')
			p++;
		*p=0;
	}
	return(buf);
}
char *
user(){
	return(field(buf, 0, ':'));
}
char *
passwd(){
	return(field(buf, 1, ':'));
}
char *
uid(){
	return(field(buf, 2, ':'));
}
char *
gid(){
	return(field(buf, 3, ':'));
}
char *
group(){
#	include <grp.h>
	struct group *getgrgid();
	register struct group *g;
	g=getgrgid(atoi(field(buf, 3, ':')));
	return(g? g->gr_name : field(buf, 3, ':'));
}
char *
acct(){
	char sbuf[64];
	strcpy(sbuf, field(buf, 4, ':'));
	return(field(sbuf, 0, ','));
}
char *
bin(){
	char sbuf[64];
	strcpy(sbuf, field(buf, 4, ':'));
	return(field(sbuf, 1, ','));
}
char *
gcid(){
	char *c, sbuf[64];
	strcpy(sbuf, field(buf, 4, ':'));
	if (*(c = field(sbuf, 2, ',')) == '\0') {
	   return(field(buf, 0, ':'));
        }
        else {
           return(c);
        }
}
char *
home(){
	return(field(buf, 5, ':'));
}
char *
shell(){
	return(field(buf, 6, ':'));
}
