#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

Test()
{
	char req[128];

	printf( "testing\n" );
	while( fgets( req, 128, stdin ) ){
		req[strlen(req)-1] = 0;
		Request(req);
	}
	exit(0);
}

main( argc, argv )
char **argv;
{
	static char sync[128], req[128];
	int got;

	got = read( 0, sync, sizeof sync );			/* OK */
	if( chdir(argc>1 ? argv[1] : "/usr/spool/ap") ){
		got = read( 0, sync, sizeof sync );		/* ERR */
		write( 1, sync, got );				/* ERR */
		exit(1);
	}
	if( argc>2 ) Test();
	write( 1, sync, got );					/* OK */
	read( 0, sync, sizeof sync );				/* ERR */
	for( ;; ){
		got = read( 0, req, sizeof req );
		req[got] = '\0';
		Request( req );
	}
}

Request(r)
char *r;
{
	if( !strcmp(r, "slugs") )
		Slugs();
	else
		File(r);
}

char *Swallow(file, size)
char *file;
int *size;
{
	int fd = -1;
	struct stat st;
	char *buf = 0, *malloc();

	if( (fd = open(file, 0)) >= 0
	 && !fstat(fd, &st)
	 && (buf = malloc(st.st_size) )
	 && read(fd, buf, st.st_size) == st.st_size ){
		close(fd);
		*size = st.st_size;
		return buf;
	} else {
		if( fd>= 0 ) close(fd);
		if( buf ) free(buf);
		*size = 0;
		return 0;
	}
}

#define RCVSIZE 8192

File(f)
char *f;
{
	char *buf, *error = "cannot read file";
	int size;

	if( buf = Swallow(f, &size) ){
		if( size > RCVSIZE ) size = RCVSIZE;
		write( 1, buf, size );
		free( buf );
	} else
		write( 1, error, strlen(error) );
}
	
#define RECENT "..tlist"
#define MARGIN (RCVSIZE-256)
#define APPEND(s) { strcpy(reply+r, (s)); r += strlen(s); }
Slugs()
{
	static char Last[16];
	int size, i, r = 0, limit = 75;
	char *recent = 0, reply[RCVSIZE], *f, junk[128], slug[128], *newlast=0;
	FILE *fp;

	APPEND( "slugs_header" );
	++r;
	recent = Swallow( RECENT, &size );
	if( recent ){
		for( f = recent+size-1; r < MARGIN; --f ){
			if( limit-- < 0 ) break;
			while( !*f ) --f;
			while(  *f ) --f;
			++f;
			if( f<recent ) break;
			if( !newlast ) newlast = f;
			if( !strcmp(f, Last) ) break;
			fp = fopen( f, "r" );
			if( fp ){
				if( fscanf(fp, "%100[^-]-%100s", junk, slug) == 2 ){
					APPEND( f );
					APPEND( " " );
					APPEND( slug );
					++r;
				fclose(fp);
				}
			}
		}
		if( newlast ) strncpy( Last, newlast, 14 );
		free(recent);
	}
	write( 1, reply, r );
}

