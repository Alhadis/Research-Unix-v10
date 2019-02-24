#include <pads.pub>
#include <stdio.h>
#include <dk.h>

int Remfd;
#define RCVSIZE 8192
char *SyncOK  = "SYNCOK";
char *SyncErr = "SYNCERR";
char *Slugs = "slugs";
//void fprintf(FILE*,char* ...);

void RequestSlugs()
{
	write( Remfd, Slugs, strlen(Slugs) );
}

char *Aphost[] = { "nj/astro/alice", "nj/astro/research",  0 };

void CreateServer(int i = 0)
{
	char buf[RCVSIZE];
	char *apserver = "/usr/jerq/lib/apserver";

	if( !Aphost[i] ){
		fprintf( stderr, "can't find an AP server; waiting 10 mins " );
		fflush(stderr);
		for( int i = 9; i>=0; --i ){
			sleep(60);
			fprintf( stderr, "%d ", i );
			fflush(stderr);
		}
		fprintf( stderr, "\n" );
		CreateServer(0);
		return;
	}
	fprintf(stderr, "waiting for AP server on %s (5-10 secs) ... ", Aphost[i]);
	fflush(stderr);
	Remfd = ipcexec(Aphost[i], "light", apserver);
	if( Remfd<0 ) {
		extern char *errstr;
		if( errstr )
			fprintf(stderr, "%s", errstr);
		fprintf(stderr, "\ncan't run %s on %s\n", apserver, Aphost[i]);
		CreateServer(i+1);
		return;
	}
	write( Remfd, SyncOK, strlen(SyncOK) );
	write( Remfd, SyncErr, strlen(SyncErr) );
	for( ;; ){
		int got = read( Remfd, buf, sizeof buf );
		buf[got] = 0;
		if( !strcmp(buf, SyncOK) ){
			fprintf( stderr, " connected\n" );
			fflush(stderr);
			return;
		}
		if( !strcmp(buf, SyncErr)
		 || !strcmp(buf, "No directory\n") )
			break;
	}
	printf( " failed\n" );
	CreateServer(i+1);
}

long time(long*);

char *hhmm(long t)
{
	static char text[32];
	char *ctime(long*);
	strcpy(text,ctime(&t));
	text[16] = 0;
	return text+11;
}

class Story : public PadRcv {
	friend	ApNews;
	char	file[16];
	char	*slug;
	char	*body;
	char	**edge;
	char	*sent;
	Pad	*pad;
	long	lines;
	long	lastreq;
	Story	*link;
	long	key;
	char	*kbd(char*);
	char	*help();
	void	cycle();
	void	linereq(long,Attrib=0);
	void	open(int=0);
		Story(char*, char*, Story*, long);
	void	userclose();
};

class ApNews  : public PadRcv {
	Story	*stories;
	Pad	*pad;
	long	key;
	void	insert(Story*);
public:
		ApNews();
	void	cycle();
	void	readslugs(int=0);
};

void main(int argc, char **argv)
{
//	void setbuf(FILE*,char*);
	char *load = 0, *getenv(char*);
	int i, help = 1, stats = 0;

	setbuf( stderr, new char[BUFSIZ] );
	CreateServer();
	RequestSlugs();
	for( i = 1; i < argc; ++i ){
		if( !strcmp( argv[i], "-h" ) ) help = 0;
		else if( !strcmp( argv[i], "-s" ) ) stats = 1;
		else load = argv[i];
	}
	if( PadsInit( load ) ){
		fprintf( stderr, "can't load terminal\n" );
		exit(1);
	}
	ApNews *ap = new ApNews;
	ap->readslugs();
	if( help ) NewHelp();
	if( stats ) NewPadStats();
	PadsServe();
}

ApNews.ApNews()
{
	pad = new Pad( (PadRcv *) this );
	stories = 0;
	key = 0x80000000;
	pad->name( "Slugs" );
	pad->banner( "Slugs" );
	pad->makecurrent();
	pad->alarm(60);	
}

void ApNews.cycle()
{
	RequestSlugs();
	readslugs(1);
	pad->alarm(60);
}

void ApNews.readslugs(int)
{
	char buf[RCVSIZE], file[128], slug[128];
	int i;
	Story *s;

	pad->banner( "new slugs ..." );
	key -= 128;
	pad->options(TRUNCATE); 
	int size = read( Remfd, buf, RCVSIZE );
	for( i = 0; i < size; ++i ){
		if( sscanf(buf+i, "%s %s", file, slug) == 2 ){
			for( s = stories; s; s = s->link )
				if( !strcmp( s->file, file ) )
					pad->removeline( s->key );
			stories = new Story( file, slug, stories, ++key );
			insert(stories);
			/* if( o ) stories->open(1); */
		}
		while( buf[i] ) ++i;
	}
	pad->banner("Slugs %s", hhmm(time(0)));
}
			
void ApNews.insert(Story *s)
{
	Menu m( "open story", (Action) &Story::open );
	pad->insert( s->key, (Attrib) DONT_CUT, (PadRcv*) s, m, s->slug );
}

Story.Story(char *f, char *s, Story *l, long k)
{
	strcpy(file, f);
	slug = sf(s);
	link = l;
	key = k;
	edge = 0;
	sent = 0;
	body = 0;
	lines = 0;
	pad = 0;
	lastreq = 0;
}

void Story.userclose()
{
	delete edge;
	delete body;
	delete sent;
	delete pad;
	edge = 0;
	body = 0;
	sent = 0;
	lines = 0;
	pad = 0;
	lastreq = 0;
}

#define LINES 150
void Story.open(int dont)
{
	char buf[RCVSIZE], *p, *q;

	if( pad && edge ){
		pad->makecurrent();
		pad->alarm();
		return;
	}
	if( !pad ){
		pad = new Pad((PadRcv*) this);
		pad->banner("%s %s:", file, slug);
		pad->name("%s", slug);
		pad->options(NO_TILDE);
	}
	write( Remfd, file, strlen(file) );
	int size = read( Remfd, buf, RCVSIZE );
	if( size <= 0 ){
		strcpy( buf, "Story delivery failed - AP server died?\n" );
		size = strlen(buf);
	}
	body = new char[size+1];
	edge = new char* [LINES];
	sent = new char [LINES];
	p = body;
	q = buf;
	for( lines = 1; q < buf+size && lines < 150; ++lines ){
		edge[lines] = p;
		while( *q && *q!='\n' ) *p++ = *q++;
		*p++ = '\0';
		++q;
	}
	--lines;
	pad->lines(lines);
	int i;
	for( i = 1; i <= 5; ++i ) linereq( i );
	if(!dont){
		pad->makecurrent();
		pad->alarm();
	}	
}

void Story.linereq(long i, Attrib a)
{
	if( i<1 || i>lines ) return;
	if( !sent[i] ){
		pad->insert(i, a|DONT_CUT, "%s ", edge[i]);
		sent[i] = 1;
	}
	lastreq = i;
}

int FIVE = 5;
void Story.cycle()
{
	long lr = lastreq;
	for( int i = 1; i <= FIVE; ++i )
		linereq(lr+i, DONT_DIRTY);
	if( lastreq<lines ) pad->alarm(1);
	
}

char *Story.kbd(char *s)
{
	FILE *fp, *popen(char*,char*);
	long i;

	if( !s[0] ) return 0;
	if( fp = popen( s, "w" ) ){
		for( i = 1; i <= lines; ++i )
			fprintf( fp, "%s\n", edge[i] );
	}
	if( fclose(fp) ) return sf( "%s:failed", s );
	return 0;
}

char *Story.help()
{
	return "shell_cmd { shell_cmd < story_text }";
}
