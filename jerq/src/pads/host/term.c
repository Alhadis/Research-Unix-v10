#include <ctype.h>
#include <pads.pri>
#include <sys/types.h>
#include <CC/sys/stat.h>
SRCFILE("term.c")

void Pick( char *s, Action a, long o )
{
	Index ix;

	trace("Pick(%d,%d)", a, o);
	ix = ICache->place(Item(s, a, o));
	R->pktstart( P_PICK );
	R->sendshort( ix.sht() );
	R->pktend();
}

char *JerqTERM[] = {	"/usr/jerq/bin/32ld /usr/jerq/mbin/pads.m",
			"5620", "jerq", "JERQ", "Jerq", "DMD", "DMD5620",
			0 };

char *BlitTERM[] = {	"/usr/blit/bin/68ld /usr/blit/mbin/pads.m",
			"blit", "BLIT", "Blit",
			0 };

char **MapTERM[] = { JerqTERM, BlitTERM, 0 };

char *PadsInit(char *loadcmd)
{
	trace( "PadsInit(%s)", loadcmd );
	if( !loadcmd ){
		char *getenv(char*), *TERM = getenv("TERM"), ***map, **term;
		loadcmd = MapTERM[0][0];
		for( map = MapTERM; TERM && *map; ++map )
			for( term = *map, ++term; *term; ++term )
				if( !strcmp(TERM, *term) )
					loadcmd = **map;
	}
	if( system(loadcmd) ) return "terminal download failed";
	R = new Remote("/dev/tty");
	R->pktstart(P_VERSION); R->sendlong(PADS_VERSION); R->pktend();
	R->pktstart(P_BUSY); R->pktend();
	ICache = new ItemCache;
	CCache = new CarteCache;
	close(2);
	return 0;
}

char *TapTo;
void WireTap(PRINTF_ARGS)
{
	static int fd = -1;
	static long t0;
	struct stat s;
	char *ctime(long*);
	long t, time(long*);

	if( !TapTo ) return;
	t = time(0);
	if( ::stat("/usr/jerq/lib/.logpads", &s) || ctime(&t)[23]!='6' )
		goto BailOut;
	if( t0 )
		t -= t0;
	else
		t0 = t;
	char buf[256];
	sprintf(buf, "%x:", t);
	sprintf(buf+strlen(buf), PRINTF_COPY);
	if( fd < 0 ){
		if( ::stat(TapTo, &s) )
			creat(TapTo, 0777); 
		fd = open(TapTo, 1);
	}
#define PILOGSIZE 32000
	if( fd<0
	 || fstat(fd, &s)
	 || s.st_size > PILOGSIZE )
			goto BailOut;
	lseek(fd, s.st_size, 0);
	write(fd, buf, strlen(buf));
	return;
BailOut:
	TapTo = 0;
}

int BothValid(PadRcv *p, PadRcv *o)
{
	return p && o;
}

void TermAction(PadRcv *parent, PadRcv *obj, int pick)
{
	Item *item;
	Index ix(R->rcvshort());

	trace( "TermAction(%d,%d,%d)", parent, obj, pick );
	if( ix.null() ) return;
	item = ICache->take(ix);
	if( !BothValid(parent,obj)
	 || (pick && !obj->accept(item->action)) )
		return;
	Action a = item->action;
	long faddr, ixorf = (int) a;			// yuk: can't say
	if( ixorf & (~127) )				// &(obj->*item->action)
		faddr = ixorf;				//
	else						//
		faddr = ((long**) obj)[1][ixorf-1];	//
	WireTap("%x->%x(%x)\n", obj, faddr, item->opand);
	if( item->action ) (obj->*item->action)(item->opand, 0, 0);
}

char *DoKbd(PadRcv *obj, char *buf)
{
	WireTap("%x->%x(%x) %s\n", obj, &obj->kbd, strlen(buf), buf);
	char *e = obj->kbd(buf);
	if( e ) PadsWarn("%s", e);
	return e;
}

void Shell()
{
	char cmd[256];
	R->rcvstring(cmd);
	FILE *fp = Popen(cmd, "w");
	for( long lines = R->rcvlong(); lines>0; --lines ){
		char data[256];
		if( fp ) fprintf(fp, "%s\n", R->rcvstring(data));
	}
	if( !fp ){
		PadsWarn("cannot write to pipe");
		return;
	}
	int x = Pclose(fp);
	if( x ) PadsWarn( "exit(%d): %s", x, cmd );
}

void ShKbd(PadRcv *obj, char *cmd)
{
	trace( "ShKbd(%d,%s)", obj, buf );
	FILE *fp = Popen(cmd, "r");
	if( !fp ){
		PadsWarn("cannot read from pipe");
		return;
	}
	char buf[256];
	while( fgets(buf, sizeof buf, fp) ){
		buf[strlen(buf)-1] = 0;
		if( DoKbd(obj, buf) ) break;
	}
	int x = Pclose(fp);
	if( x ) PadsWarn( "exit(%d): %s", x, cmd );
}

void Kbd(PadRcv *parent, PadRcv *obj)
{
	char buf[256];
	R->rcvstring(buf);
	trace( "Kbd %d %s", obj, buf );
	if( !BothValid(parent,obj) ) return;
	if( !strcmp( buf, "?" ) ){
		char *h = obj->help();
		PadsWarn( "%s", (h && *h) ? h : "error: null help string" );
	} else if( buf[0] == '<' ){
		ShKbd(obj, buf+1);
	} else
		DoKbd(obj, buf);
}

void TermServe()
{
	Protocol p;
	long n, to, pick = 0;

	trace("TermServe()");
	R->writesize = 0;
	R->pktstart(P_IDLE); R->pktend();	/* flush */
	p = (Protocol) R->get();
	if( p == P_PICK ) {
		pick = 1;
		p = (Protocol) R->get();
	}
	PadRcv *par = R->rcvobj();
	PadRcv *obj = R->rcvobj();
	if( p != P_CYCLE ) { R->writesize = 0; R->pktstart(P_BUSY); R->pktend(); }
	switch( (int) p ){
		case P_ACTION:
			TermAction(par, obj, pick);
			break;
		case P_KBDSTR:
			Kbd(par, obj);
			break;
		case P_SHELL:
			Shell();
			break;
		case P_NUMERIC:
		case P_CYCLE:
		case P_USERCLOSE:
		case P_USERCUT:
			n = R->rcvshort();
			if( !BothValid(par,obj) ) return;
			switch( (int) p ){
			case P_NUMERIC:
				WireTap("%x->%x(%x)\n", obj, &obj->numeric, n);
				obj->numeric(n);	break;
			case P_CYCLE:
				obj->cycle();		break;
			case P_USERCLOSE:
				WireTap("%x->%x(0)\n", obj, &obj->userclose);
				obj->userclose();	break;
			case P_USERCUT	:
				WireTap("%x->%x(0)\n", obj, &obj->usercut);
				obj->usercut();		break;
			default: R->err();
			}
			break;
		case P_LINEREQ:
			n = R->rcvlong();
			to = R->rcvlong();
			trace("P_LINEREQ %d %d %d", p, n, to);
			if( !BothValid(par,obj) ) return;
//			WireTap("%x->%x(%x)", obj, &obj->linereq, to-n+1);
			while( n <= to )
				obj->linereq((long) n++, 0);
			break;
		default:
			R->err();
	}
}

void PadsServe(long n)
{
	if( n ){
		while( n-->0 ) TermServe();
	} else {
		for( ;; )  TermServe();
	}
}

void PadsWarn(PRINTF_ARGS)
{
	char t[256];
	sprintf( t, PRINTF_COPY );
	R->pktstart( P_HELPSTR );
	R->sendstring( t );
	R->pktend();
}

void PadsError(PRINTF_ARGS)
{
	char t[256];
	sprintf(t, PRINTF_COPY);
	int fd = open("/dev/tty", 1);
	write(fd, "  ", 2);
	write(fd, t, strlen(t));
	abort(1);
}
