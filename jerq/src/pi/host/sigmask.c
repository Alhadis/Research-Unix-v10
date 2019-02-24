#include "hostcore.h"
#include "process.pri"
#include "srctext.pub"
#include "sigmask.h"
SRCFILE("sigmask.c")

char *SignalName(int sig)
{
	switch( sig ){
		case 0:		return "0 (no signal)";
		case SIGHUP:	return "hangup";
		case SIGINT:	return "interrupt";
		case SIGQUIT:	return "quit";
		case SIGILL:	return "illegal instruction";
		case SIGTRAP:	return "trace/BPT";
		case SIGIOT:	return "IOT instruction";
		case SIGEMT:	return "EMT instruction";
		case SIGFPE:	return "floating exception";
		case SIGKILL:	return "kill";
		case SIGBUS:	return "bus error";
		case SIGSEGV:	return "memory fault";
		case SIGSYS:	return "bad system call";
		case SIGPIPE:	return "broken pipe";
		case SIGALRM:	return "alarm call";
		case SIGTERM:	return "terminated";
		case SIGSTOP:	return "stop";
		case SIGTSTP:	return "stop tty";
		case SIGCONT:	return "continue";
		case SIGCHLD:	return "child termination";
		case SIGTTIN:	return "stop tty input";
		case SIGTTOU:	return "stop tty output";
		case SIGTINT:	return "input available";
		case SIGXCPU:	return "cpu timelimit";
		case SIGXFSZ:	return "file sizelimit";
	}
	return Name( "signal %d", sig );
}

void SigMask.banner()
{
	trace( "%d.banner()", this );	VOK;
	if( pad ){
		pad->banner( "Signals: %s", core->procpath() );
		pad->name( "Signals" );
	}
}

SigMask.SigMask(HostCore *c)
{
	trace( "%d.SigMask(%d)", this, c );	VOK;
	core = c;
	pad = 0;
	mask =	 bit(SIGILL)
		|bit(SIGINT)
		|bit(SIGTRAP)
		|bit(SIGIOT)
		|bit(SIGEMT)
		|bit(SIGFPE)
		|bit(SIGBUS)
		|bit(SIGSEGV)
		|bit(SIGSYS)
		|bit(SIGPIPE)
		|bit(SIGSTOP);
	exechang = 0;
	updatecore();
}

const long EXECKEY = 33;

void SigMask.open()
{
	Menu m;
	int i;
	trace( "%d.open()", this );	VOK;
	if( !pad ){
		pad = new Pad( (PadRcv*) this );
		pad->lines(EXECKEY);
		banner();
		m.last( "clear pending signal", (Action)&SigMask::clrcurrsig, 0 );
		m.last( "clear pending and go", (Action)&SigMask::clrcurrsig, 1 );
		pad->menu(m);
		pad->options(TRUNCATE);
		for( i = 1; i <= EXECKEY; ++i ) linereq( i, 0 );
	}
	pad->makecurrent();
}

void SigMask.updatecore(char *error)
{
	if( !error ) error = core->exechang(exechang);
	if( !error ) error = core->signalmask(mask);
	core->process()->pad->error(error);
}

void SigMask.execline(long e)
{
	static char *i[] = { "clear", "hang" };
	static char *l[] = { "", ">>> " };
	Menu m;
	trace( "%d.execline(%d,ox%X)", this, e, a );		VOK;
	exechang = e &= 1;
	updatecore();
	long comp = e^1;
	m.last( i[comp], (Action) &SigMask::execline, comp );
	Attrib a = 0;
	pad->insert( EXECKEY, a, (PadRcv*)this, m, "%sexec()", l[e] );
}

void SigBit.set(SigMask *s)		{ s->setsig(bit); }
void SigBit.clr(SigMask *s)		{ s->clrsig(bit); }
void SigBit.send(SigMask *s)		{ s->sendsig(bit); }

void SigMask.linereq(long sig, Attrib a )
{
 	Menu m;
	static SigBit *sigbit;
	if( !sigbit )
		sigbit = new SigBit[33];
	trace( "%d.linereq(%d,0x%X)", this, sig, a );	VOK;
	if( sig == EXECKEY ){
		execline(exechang);
		return;
	}
	char *on = mask&bit(sig) ? ">>> " : "";
 	m.last( "send signal", (Action)&SigBit::send, (long)this );
	if( on[0] == '>' )
		m.first("trace off", (Action)&SigBit::clr, (long)this);
	else
		m.first("trace on",  (Action)&SigBit::set, (long)this);
	sigbit[sig].bit = sig;
	pad->insert(sig, a, (PadRcv*)(sigbit+sig), m, "%s%s", on, SignalName(sig));
}

void SigMask.signalmask(long sig)
{
	trace( "%d.signalmask(%d)", this, sig );	VOK;
	linereq(sig, SELECTLINE);
	updatecore();
}

void SigMask.sendsig(long sig)
{
	trace( "%d.sendsig(%d)", this, sig );	VOK;
	updatecore(core->sendsig(sig));
}

void SigMask.setsig(long sig)
{
	trace( "%d.setsig(%d)", this, sig );	VOK;
	mask |= bit(sig);
	signalmask( sig );
}

void SigMask.clrsig(long sig)
{
	trace( "%d.clrsig(%d)", this, sig );	VOK;
	mask &= ~bit(sig);
	if( sig==SIGSTOP || sig==SIGTRAP )
		updatecore( "warning: debugging signal disabled" );
	signalmask( sig );
}

void SigMask.hostclose()
{
	trace( "%d.hostclose()", this );	VOK;
	if( pad ){
		delete pad;
		pad = 0;
	}
}

void SigMask.clrcurrsig(long andgo)
{
	trace( "%d.clrcurrsig()", this );	VOK;
	updatecore(core->clrcurrsig());
	if( andgo ) core->process()->go();
	else core->process()->cycle();
}
