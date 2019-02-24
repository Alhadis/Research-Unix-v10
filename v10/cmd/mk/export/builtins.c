#include	"mk.h"

char *bltin[] =
{
	"AS", "as",
	"CC", "cc",
	"CFLAGS", "",
	"FC", "f77",
	"FFLAGS", "",
	"LDFLAGS", "",
	"LEX", "lex",
	"LFLAGS", "",
	"NPROC", "1",
	"NREP", "1",
	"YACC", "yacc",
	"YFLAGS", "",
	"BUILTINS", "%.o:	%.c\n\
	$CC $CFLAGS -c $stem.c\n\
%.o:	%.s\n\
	$AS -o $stem.o $stem.s\n\
%.o:	%.f\n\
	$FC $FFLAGS -c $stem.f\n\
%.o:	%.y\n\
	$YACC $YFLAGS $stem.y && $CC $CFLAGS -c y.tab.c && mv y.tab.o $stem.o; rm y.tab.c\n\
%.o:	%.l\n\
	$LEX $LFLAGS -t $stem.l > /tmp/$$.c && $CC $CFLAGS -c /tmp/$$.c && mv /tmp/$$.o $stem.o; rm /tmp/$$.c\n",
	"ENVIRON", "",
	0
};

builtins()
{
	register char **b;

	for(b = bltin; *b; b += 2)
		setvar(b[0], strdup(b[1]));
}

parsebuiltins()
{
	char *s, *file;
	int fd;
	int n;

	s = symlook("BUILTINS", S_VAR, (char *)0)->value;
	if(*s == 0)
		return;
	file = mktemp("/tmp/mkXXXXXX");
	if((fd = creat(file, 0600)) < 0){
		fprint(2, "mk: builtins in ");
		perror(file);
		Exit();
	}
	if(n = strlen(s)){
		write(fd, s, n);
		if(s[n-1] != '\n')
			write(fd, "\n", 1);
	}
	close(fd);
	parse("BUILTINS", open(file, 0), 0, 0);
	unlink(file);
}
