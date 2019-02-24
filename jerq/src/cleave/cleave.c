#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
FILE *popen();

int Argc; char **Argv;
char Doth[128],Dotu[128],Dotf[128],Dotut[128],Dotft[128],Dotui[128],Dotfi[128];

char Cfront[128] = "cfront";
char Cpp[128]	 = "/lib/cpp";
char *Base = 0;

main( argc, argv )
char **argv;
{
	int i;

	setbuf( stderr, malloc(BUFSIZ) );
	Argc = argc; Argv= argv;
	if( argc<2 ){
Argerr:
		fprintf( stderr, "use: cleave [-I] [+VE] base\n");
		exit(1);
	}
	for( i = 1; argv[i]; ++i ) switch( argv[i][0] ){
		case '+':
			strcat( Cfront, " " );
			strcat( Cfront, argv[i] );
			break;
		case '-':
			strcat( Cpp, " " );
			strcat( Cpp, argv[i] );
			break;
		default:
			if( Base ) goto Argerr;
			Base = argv[i];
	}
	if( !Base ) goto Argerr;
	sprintf( Doth,  "%s.h",   Base );
	sprintf( Dotu,  "%s.pub",   Base );
	sprintf( Dotf,  "%s.pri",   Base );
	sprintf( Dotut, "%s.pub.t", Base );
	sprintf( Dotft, "%s.pri.t", Base );
	sprintf( Dotui, "%s.pub.i", Base );
	sprintf( Dotfi, "%s.pri.i", Base );
	UpToDate();
	Split(0);
	Compare();
	Split(0);
	Install(Dotut,Dotu);
	Install(Dotft,Dotf);
	exit(0);
}

char *GetLine(s, n, f)
char *s;
int n;
FILE *f;
{
	char *e, l[256];
	int d;

	do e = fgets(s, n, f);
	while( e && sscanf(s, "# %s %d", l, &d)==2 && !strcmp(l, "line") );
	return e;
}

Same(a,b)
char *a, *b;
{
	struct stat unused;
	char al[1024], bl[1024], same = 1;
	FILE *ap, *bp;

	if( !(bp = fopen(b,"r")) ) return 0;
	if( !(ap = fopen(a,"r")) ) return 0;
	while( same && GetLine(al, 1024, ap) )
		same = GetLine(bl, 1024, bp) && !strcmp(al,bl);
	same = same && !GetLine(bl, 1024, bp);
	fclose(ap); fclose(bp);
	return same;
}

Install(from,to)
char *from, *to;
{
	if( Same(from,to) ){
		unlink(from);
		fprintf( stderr, "%s unchanged\n", to );
		return;
	}
	unlink(to);
	if( link(from,to) )
		Exit(1, "cannot link %s %s", from, to );
	if( unlink(from) )
		Exit(1, "cannot unlink %s", from );
	fprintf( stderr, "%s modified\n", to ); return;
}

Compile( from, to )
char *from, *to;
{
	char cpp[256], cfront[256], *cpped = "huf.tmp";

	sprintf( cpp, "%s <%s >%s", Cpp, from, cpped );
	if( system( cpp ) ) Exit( 1, "cpp failed: %s", from );
	sprintf( cfront, "%s +f%s <%s >%s", Cfront, from, cpped, to );
	if( system( cfront ) ) Exit( 1, "cfront failed: %s", from );
	unlink( cpped );
}

struct Context {
	FILE	*fp;
	char	line[1024];
	int	n;
	char	path[256];
};

int LineNo(c)
struct Context *c;
{
	if( c->line[0] == '#' ){
		if( 2 != sscanf(c->line, "# %d %[^\n]", &c->n, c->path) )
			Exit( 1, ".i # syntax error: %s", c->line );
		--c->n;
	} else ++c->n;
	return c->line[0] == '#';
}

ShowContext(c)
struct Context *c;
{
	fprintf( stderr, "%s ~%d >C++> %s", c->path, c->n, c->line );
}

struct Context ZContext;

Compare()
{
	char sed[256], i;
	struct Context u, f, save;

	u = f = save = ZContext;
	Compile( Dotut, Dotui );
	Compile( Dotft, Dotfi );
	sprintf( sed, "sed /pub_filler/d <%s", Dotui );
	u = f = save = ZContext;
	if( !(u.fp = popen(sed, "r")) )
		Exit( 1, "cannot popen %s", sed );
	if( !(f.fp = fopen(Dotfi, "r")) )
		Exit( 1, "cannot open %s", Dotfi );
	strcpy( u.path, Dotut );
	strcpy( f.path, Dotft );
	for( i = 1; i <= 5; ++i )
		fgets( u.line, sizeof u.line, u.fp );
	while( fgets( u.line, sizeof u.line, u.fp ) ){
		if( LineNo(&u) ) continue;
		while( fgets( f.line, sizeof f.line, f.fp ) ){
			if( LineNo(&f) ) continue;
			save = f;
			break;
		}
/*Bogusfgets:	if( strlen(u.line) > 510 ) continue;	*/
		while( strcmp( u.line, f.line ) ){
			if( !fgets( f.line, sizeof f.line, f.fp ) ){
				fprintf( stderr, "%s vs %s:\n", Dotui, Dotfi );
				ShowContext( &u );
				ShowContext( &save );
				exit(1);
			}
			LineNo(&f);
		}
	}
	pclose(u.fp); fclose(f.fp); unlink(Dotui); unlink(Dotfi);
}

Split(v)
{
	FILE *h, *ut, *ft;
	char text[256], u = 1, f = 1, cmd[256], direct[256];
	int n = 0, sync = 0;

	if( !(h = fopen(Doth, "r")) )
		Exit( 1, "cannot open %s", Doth );
	if( !(ut = fopen(Dotut, "w")) )
		Exit( 1, "cannot open %s", Dotut );
	if( !(ft = fopen(Dotft, "w")) )
		Exit( 1, "cannot open %s", Dotft );
	while( ++n, fgets( text, 256, h ) ){
		if( text[0] == '>' ){
			strcpy( direct, "" );
			sscanf( text, ">%s", direct );
			if(!strcmp(direct,"pub") || !strcmp(direct,"u")){
				u = 1;
				f = 0;
			} else if(!strcmp(direct,"pri") || !strcmp(direct,"f") ){
				u = 0;
				f = 1;
			} else {
				if(v) fprintf(ft, "static %s_%d;\n", Base, ++sync );
				if(v) fprintf(ut, "static %s_%d;\n", Base, sync );
				u = 1; f = 1;
			}
			if(f&&v) fprintf(ft, "# line %d \"%s(.pri)\"\n", n+1, Doth);
			if(u&&v) fprintf(ut, "# line %d \"%s(.pub)\"\n", n+1, Doth);
			continue;
		}
		if( f ) fprintf( ft, "%s", text );
		if( u ) fprintf( ut, "%s", text );
	}
	fclose(h); fclose(ut); fclose(ft);
	sprintf( cmd, "cmp -s %s %s", Dotut, Dotft );
	if( v && !system(cmd) ){
		fprintf( stderr, "warning: identical .pri and .pub\n" );
		fflush(stderr);
	}
}

UpToDate()
{
	struct stat h, f;

	if( stat(Doth, &h) )
		Exit( 1, "cannot stat %s", Doth );
	if( stat(Dotf, &f) )
		return;
	if( h.st_mtime<f.st_mtime ){
		fprintf( stderr, "%s is up to date\n", Dotf );
		exit(0);
	}
}

Exit( code, f, a1, a2, a3, a4, a5, a6 )
char *f;
{
	char msg[256];

	sprintf( msg, f, a1, a2, a3, a4, a5, a6 );
	perror( msg );
	exit(code);
}
