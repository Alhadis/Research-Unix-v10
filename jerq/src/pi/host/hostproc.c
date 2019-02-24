#include "univ.h"
#include "process.pri"
#include "sigmask.h"
#include "hostcore.h"
#include "expr.pub"
#include "master.pub"
#include "bpts.pri"
#include "frame.pri"
#include "memory.pub"
#include "symtab.pub"
#include "symbol.h"
#include "srcdir.h"
#include "asm.pub"
#include <CC/stdio.h>
SRCFILE("hostproc.c")

void HostProcess.takeover()
{
	trace("%d.takeover()", this);			VOK;
	if( pad ){
		open();
		insert(ERRORKEY, "take over: already open");
		return;
	}
	Pick( "take over", (Action)&HostProcess::substitute, (long) this );
}

void HostProcess.reopendump()
{
	char *error;

	trace( "%d.reopendump(%d)", this );		VOK;
	insert(ERRORKEY, 0);
	if( error = core->reopen(procpath, stabpath) ){
		insert(ERRORKEY, error);
		return;
	}
	docycle();
}

void HostProcess.substitute(HostProcess *t)
{
	char *error, *oldprocpath, *oldstabpath, *oldcomment;

	trace( "%d.substitute(%d)", this, t );		VOK;
	insert(ERRORKEY, 0);
	if( !core ){
		insert(ERRORKEY, "that ought to work - but it doesn't");
		return;
	}
	if( !core->online() ){
		insert(ERRORKEY, "cannot take over a coredump");
		return;
	}
	_bpts->lift();
	if( error = core->reopen(t->procpath, t->stabpath) ){
		_bpts->lay();
		insert(ERRORKEY, error);
		return;
	}
					/*	t->kill();	*/
	oldprocpath = procpath;
	oldstabpath = stabpath;
	oldcomment = comment;
	procpath = t->procpath;
	stabpath = t->stabpath;
	comment = t->comment;
	master->makeproc( oldprocpath, oldstabpath, oldcomment );
	master->insert(t);
	master->insert(this);
	banner();
	if( _asm ) _asm->banner();
	if( _bpts ) _bpts->banner();
	if( memory ) memory->banner();
	if( globals ) globals->banner();
	if( sigmask ){
		sigmask->banner();
		sigmask->updatecore();
	}
	if( srcdir ) srcdir->banner();
	core->symtab()->banner();
	pad->clear();
	_bpts->lay();
	docycle();
}

int HostProcess.accept( Action a )
{
	trace( "%d.accept(%d)", this, a );		OK(0);
	return a == (Action)&HostProcess::substitute;
}

void HostProcess.imprint()
{
	trace( "%d.imprint()", this ); VOK;
	char *parentpath = sf("%s%d", slashname(procpath), hostcore()->ppid() );
	insert(ERRORKEY, "parent=%s", parentpath);
	Process *p = master->search(parentpath);
	if( !p ){
		insert(ERRORKEY, "parent (%d) not opened", hostcore()->ppid());
		return;
	}
	_bpts->liftparents(p->_bpts);
}

void HostProcess.userclose()
{
	trace( "%d.userclose()", this );	VOK;
	if( sigmask ){
		sigmask->hostclose();
		delete sigmask;
		sigmask = 0;
	}
	Process::userclose();
	Wait3();
}

void HostProcess.open(long ischild)
{
	Menu m, s;
	char *error;

	trace( "%d.open(%d)", this, ischild );	VOK;
	Process::openpad();
	if( core ) return;
	insert(ERRORKEY, "Checking process and symbol table...");
	core = (Core*) new HostCore(this, master);
	if( error = core->open() ){
		delete core;
		core = 0;
		if( ischild )
			m.last( "open child", (Action)&HostProcess::open, 1 );
		else
			m.last( "open process", (Action)&HostProcess::open, 0 );
		pad->menu( m );
		insert(ERRORKEY, error);
		return;
	}
	insert(ERRORKEY, core->symtab()->warn());
	globals = new Globals(core);
	_asm = core->newAsm();
//			HostCore.reopen() doesn't handle dumps
//			if( !core->online() )
//				m.last( "reopen dump", (Action)&reopendump );
	if( core->online() ){
		m.last( "stop", (Action)&HostProcess::stop );
		m.last( "run",  (Action)&Process::go  );
	}
	m.last( "src text",  (Action)&Process::srcfiles    );	/* should check */
	m.last( "Globals",   (Action)&Process::openglobals );
	m.last( "RawMemory", (Action)&Process::openmemory  );
	s.last( "Assembler", (Action)&Process::openasm     );
	s.last( "User Types",(Action)&Process::opentypes   );
	if( core->online() ){
		s.last("Journal", (Action)&Process::openjournal);
		s.last("Signals", (Action)&HostProcess::opensigmask);
		s.last("Bpt List", (Action)&HostProcess::openbpts);
		_bpts = new Bpts(core);
		_bpts->lay();
		if( ischild ) imprint();
		sigmask = new SigMask(hostcore());
		m.last("kill?",   (Action)&HostProcess::destroy     );
	}
	m.last(s.index("more"));
	pad->menu(m);
	pad->makecurrent();
	docycle();
}

char *UnixStateName(int s)
{
	switch( s ){
		case 0:		return "process access error";
		case SSLEEP:	return "sleeping";
		case SWAIT:	return "p_stat=SWAIT";
		case SRUN:	return "running";
		case SIDL:	return "p_stat=SIDL";
		case SZOMB:	return "p_stat=SZOMB";
		case SSTOP:	return "stopped";
		default:	return	Name( "p_stat=%d", s );
	}
}

void HostProcess.opensigmask()
{
	trace( "%d.opensigmask()", this );	VOK;
	if( sigmask ) sigmask->open();
}

void HostProcess.destroy()
{
	trace( "%d.destroy()", this );		VOK;

	IF_LIVE( !core->online() ) return;
	insert(ERRORKEY, core->destroy());
	docycle();
}

void HostProcess.stop()
{
	trace( "%d.stop()", this );		VOK;

	IF_LIVE( !core->online() ) return;
	if( !(sigmask->mask&sigmask->bit(SIGSTOP)) ) sigmask->setsig(SIGSTOP);
	Process::stop();
}

Index HostProcess.carte()
{
	Menu m;
	trace( "%d.carte(%d)", this ); OK(ZIndex);
	if( !strcmp(procpath,"!") ){
		m.last( "hang & open proc", (Action)&HostProcess::hangopen );
		m.last( "hang & take over", (Action)&HostProcess::hangtakeover );
	} else if( !strcmp(basename(procpath), "core") )
		m.last( "open coredump",(Action)&HostProcess::open );
	else {
		m.last( "open process",  (Action)&HostProcess::open, 0 );
		m.last( "take over",    (Action)&HostProcess::takeover );
		m.last( "open child", (Action)&HostProcess::open, 1 );
	}
	return m.index();
}

void HostProcess.hang()
{
	trace( "%d.hang(%s)", this ); VOK;
	int pid = ::fork();
	if( !pid ){
		int fd;
		for( fd = 0; fd < _NFILE; ++fd ) ::close(fd);
		::open("/dev/null", 2);
		::dup2(0, 1);
		::dup2(0, 2);
		::setpgrp(::getpid(), ::getpid());
		::execl("/bin/sh","sh","-c",sf("exec hang %s",stabpath),0);
		::exit(0);
	}		
	procpath = sf("/proc/%05d", pid);
	master->makeproc("!", stabpath);
	master->insert(this);
	comment = stabpath;
	stabpath = 0;
	WaitForExecHang(procpath);
}

void HostProcess.hangopen()
{
	trace( "%d.hangopen()", this ); VOK;
	hang();
	open();
}

void HostProcess.hangtakeover()
{
	trace( "%d.hangtakeover()", this ); VOK;
	hang();
	takeover();
}
