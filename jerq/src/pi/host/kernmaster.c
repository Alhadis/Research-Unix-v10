#include "hostcore.h"
#include "master.pri"
#include "process.pri"
#include "symtab.pri"
#include <CC/stdio.h>
SRCFILE("kernmaster.c")

Process	*KernMaster.domakeproc(char *p, char *s, char* c)
{
	return (Process*) new KernProcess(child, p, s, c);
}

extern char *UNIX;

KernMaster.KernMaster(SymTab *s)
{
	trace( "%d.KernMaster()", this );	VOK;
	pad = new Pad( (PadRcv*) this );		/* this code cannot */
	pad->options(TRUNCATE|SORTED);			/* be in base ctor */
	pad->name("kpi");
	pad->banner("Kernel pi:");
	pad->makecurrent();
	core = new KernCore(0,this);
	core->_symtab = s;
	refresh();
}

void KernMaster.refresh()
{
	trace( "%d.refresh()", this );	VOK;
	pad->clear();
	findcores("/tmp/dump");
	findcores("/u/d");
	findcores("/usr/dumps");
	findcores("/usr1/dumps");
	findcores("/usr2/dumps");
	findcores("/usrc/dumps");
	findcores(".");
	for( Process *p = child; p; p = p->sibling )
		if( p->core ) insert(p);
}

void KernMaster.findcores(char *dir)
{
	FILE *fp, *Popen(char*,char*);
	char file[16], n;
	int fd;

	trace( "%d.findcores(%s)", this, dir );	VOK;
	fd = open(dir, 0);
	if( fd < 0 ) return;
	close(fd);
	if( !(fp = Popen( sf("ls -t %s", dir), "r")) )
		return;
	for( int i = 0; i < 5; ++i ){
		if( !fgets(file, 16, fp) )
			break;
		if( 1 == sscanf(file, "vmcore.%d", &n) )
			makeproc(sf("%s/vmcore.%d",dir,n),sf("%s/unix.%d",dir,n));
	}
	Pclose(fp);
}
char *KernMaster.kbd(char *s)
{
	char *corep, core[64], syms[64], star = 0;
	KernProcess *p;

	trace( "%d.kbd(%s)", this, s );		OK("kbd");
	while( *s == ' ' ) ++s;
	while( *s==' ' ) ++s;
	switch( *s ){
	case '*':
		star = 1;
		for( ++s; *s==' '; ++s ) {}
	default:
		switch( sscanf(s, "%s %s \n", corep = core, syms) ){
		case 1: syms[0] = '\0';
		case 2:	p = (KernProcess*) makeproc(corep, syms[0]?syms:0, 0);
			if( star && p ) p->open();
			break;
		default:
		return help();
		}
	}
	return 0;
}

char *KernMaster.help()
{
	trace( "%d.help()", this );
	return ".../vmcore.N [.../unix.N] {identify system dump}";
}
