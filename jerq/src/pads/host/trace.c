#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define INTERVAL	60
#define STRSIZE		64
#define TABLESIZE	32
#define PRINTFARGS	fmt, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10
#define IDLECYCLE	1000

static char *ControlFile = "TRACE";

static struct TraceRange {
	char	srcfile[STRSIZE];
	int	from;
	int	to;
};
static int TableSize;
static struct TraceRange RangeTable[TABLESIZE];
static struct TraceRange Context[16];
static int ContextIndex;
static Calls;

static FILE *Tty;
static char Device[STRSIZE];

static void ifprintf( PRINTFARGS )
{
	if( Tty ){
		fprintf( Tty, PRINTFARGS );
		fflush( Tty );
	}
}

#define SEMANTICS(text) { ifprintf( "Error: %s\n", text ); return 1; }

static int RangeCmd( cmd )
char *cmd;
{
	struct TraceRange r, x;

	switch( sscanf( cmd, "range %s %d %d", r.srcfile, &r.from, &r.to ) ){
		case 2:	r.to = r.from;
			break;
		case 3: break;
		default:
			return 0;
	}
	if( r.to < r.from ) { x = r; x.from = r.to; x.to = r.from; r = x; }
	if( TableSize >= TABLESIZE ) SEMANTICS( "table overflow" );
	RangeTable[TableSize++] = r;
	return 1;
}

static int DeviceCmd( cmd )
char *cmd;
{
	if( sscanf( cmd, "device %s", Device ) != 1 ) return 0;
	return 1;
}

static void Commands(f)
FILE *f;
{
	char cmd[STRSIZE];

	TableSize = 0;
	strcpy( Device, "/dev/null" );
	while( fgets( cmd, STRSIZE, f ) ){
		ifprintf( "%s", cmd );
		if(	!RangeCmd(cmd)	&&
			!DeviceCmd(cmd) 	) ifprintf( "Syntax Error\n" );
	}
}

static void Update()
{
	static time_t modified;
	FILE *f;
	struct stat buf;
	char shcmd[STRSIZE];

	ifprintf( "trace 831120\n" );
	if( !(f = fopen( ControlFile, "r" ))) return;
	fstat( fileno(f), &buf );
	if( buf.st_mtime != modified ) Commands(f);
	modified = buf.st_mtime;
	fclose(f);
	if( !strcmp( Device, "/dev/null") ){
		if( Tty ) fclose( Tty );
		Tty = 0;
		return;
	}
	if( f = fopen( Device, "w" ) ){
		if( Tty ) fclose( Tty );
		Tty = f;
	}
	else ifprintf( "Cannot open: %s\n", Device );
}
		
static void CheckTime(t)
long t;
{
	static timestamp;

	if( t > timestamp+INTERVAL ){
		Update();
		timestamp = t;
	}
}

static int ContextSelected()
{
	int i, line, from, to;

	for( i = 0; i < TableSize; ++i )
		if( !strcmp(Context[ContextIndex].srcfile, RangeTable[i].srcfile) ){
			line =  Context[ContextIndex].from;
			from = RangeTable[i].from;
			to = RangeTable[i].to;
			if( line>=from && line<=to ) return 1;
			if( -line>=from && -line<=to ) return 0;
		}
	return 0;
}

static TraceArgs( PRINTFARGS )
{
	char mmss[32];
	long time(), t = time(0L);
	char *ctime();

	--ContextIndex;
	CheckTime(t);
	if( !Tty || !ContextSelected() ) return 0;
	strcpy( mmss, &ctime(&t)[14]);	/* "Sun Sep 16 01:03:52 1973\n\0" */
	mmss[5] = '\0';
	fprintf(Tty, "%d %s %s:%d ",
	 Calls, mmss, Context[ContextIndex].srcfile, Context[ContextIndex].from);
	fprintf( Tty, PRINTFARGS );
	fputc( '\n', Tty );
	fflush( Tty );
	return 1;
}

typedef int (*PFI)();

static long *watchloc, watchval;

PFI trace_fcn( srcfile, line )
char *srcfile;
{
	static idle;

	++Calls;
	if( !Tty && idle ){
		idle = (idle+1)%IDLECYCLE;
		return 0;
	}
	if( watchloc && watchval!=*watchloc ){
		ifprintf(Tty, "!!!!!!! %s:%d *%d: %d -> %d\n",
			srcfile, line, watchloc, watchval, *watchloc );
		watchval = *watchloc;
	}
	strncpy( Context[ContextIndex].srcfile, srcfile, STRSIZE );
	Context[ContextIndex].to = Context[ContextIndex].from = line;
	++ContextIndex;
	return TraceArgs;
}

watch(loc)
long *loc;
{
	if( watchloc = loc ){
		watchval = *watchloc;
		ifprintf(Tty, "watching *%d = %d\n", watchloc, watchval );
	}
}
