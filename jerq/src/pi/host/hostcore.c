#include "process.pub"
#include "frame.pri"
#include "symtab.pri"
#include "symbol.h"
#include "hostcore.h"
#include <a.out.h>
#include <sys/reg.h>
#include "asm.pri"
#include "format.pub"
#include "bpts.pri"
#include "master.pri"
SRCFILE("hostcore.c")
#define	TXTOFF(magic)	(magic==ZMAGIC ? 1024 : sizeof (struct exec))

char *HostCore.read(long l,char *b,int n)	{ return readwrite(l,b,n,0); }
char *HostCore.write(long l,char *b,int n)	{ return readwrite(l,b,0,n); }
char *HostCore.eventname()			{ return SignalName(event()); }
char *HostCore.pokedbl(long l,double d,int n)	{ return write(l,(char*)&d,n); }
char *HostCore.poke(long l,long d,int n)	{ return write(l,(char*)&d,n); }
char *HostCore.run()				{ return ioctl(PIOCRUN);  }
char *HostCore.stop()				{ return sendsig(SIGSTOP); }
int HostCore.REG_AP()				{ return 12; }
int HostCore.REG_FP()				{ return 13; }
int HostCore.REG_PC()				{ return 15; }
int HostCore.REG_SP()				{ return 14; }
int HostCore.event()				{ return pr.p_cursig; }

#define VAXBPT 3
char *HostCore.laybpt(Trap *t)
{
	trace( "%d.laybpt(%d)", this, t );	OK("HostCore.laybpt");
	t->saved = peek(t->stmt->range.lo)->chr;
	return poke(t->stmt->range.lo, VAXBPT, 1);
}

char *HostCore.liftbpt(Trap *t)	
{
	trace( "%d.liftbpt(%d)", this, t );	OK("HostCore.liftbpt");
	if( behavs() == ERRORED ) return 0;
	return poke(t->stmt->range.lo, t->saved, 1);
}

#define REGBIT(r) (1 << (16+r))
long HostCore.saved(Frame *f, int r, int)	/* ignore size */
{
	if( !(f->regsave & REGBIT(r)) )
		return 0;
	long loc = f->regbase;
	while( --r >= 0 )
		if( f->regsave & REGBIT(r) ) loc += 4;
	return loc;
}

Asm *HostCore.newAsm()	{ return new VaxAsm(this); }

Behavs HostCore.behavs()
{
	trace( "%d.behavs()", this );	OK(ERRORED);
	if( !online() )
		return PENDING;
	char *error = readcontrol();
	if( error ){
		behavs_problem = sf( "readcontrol(): %s", error );
		return ERRORED;
	}
	switch( pr.p_stat ){
	case SSLEEP:
	case SRUN:
		return ACTIVE;
	case SSTOP:
		switch( pr.p_cursig ){
		case SIGSTOP:
			return HALTED;
		case SIGTRAP:
			return BREAKED;
		default:
			return PENDING;
		}
	}
	behavs_problem = sf( "pr.p_stat=%d", pr.p_stat);
	return ERRORED;
}

char *HostCore.regname(int r)
{
	switch( r ){
	case 12:	return "ap";
	case 13:	return "fp";
	case 14:	return "sp";
	case 15:	return "pc";
	default:	return sf( "r%d", r );
	}
}

long HostCore.regloc(int r, int)
{
	long offset = 0;

	switch( r ){
	case  0:	offset = R0; break;
	case  1:	offset = R1; break;
	case  2:	offset = R2; break;
	case  3:	offset = R3; break;
	case  4:	offset = R4; break;
	case  5:	offset = R5; break;
	case  6:	offset = R6; break;
	case  7:	offset = R7; break;
	case  8:	offset = R8; break;
	case  9:	offset = R9; break;
	case 10:	offset = R10; break;
	case 11:	offset = R11; break;
	case 12:	offset = AP; break;
	case 13:	offset = FP; break;
	case 14:	offset = SP; break;
	case 15:	offset = PC; break;
	case 16:	offset = PS; break;	/* non-standard */
	default:	IF_LIVE(1) return 0;
	}
	return (long) (offset + u()->u_ar0);
}

char *HostCore.problem()
{
	if( corefstat() )
		return "core image has gone away";
	return Core::problem();
}

char *HostCore.readcontrol()
{
	char *error;

	trace( "%d.readcontrol()", this ); OK("Core.readcontrol()");
	if( corefstat() )
		return "cannot fstat core image";
	if( online() ){
		if( ::ioctl( corefd, PIOCGETPR, &pr ) < 0 )
			return SysErr("fetching proc struct:");
		if( error = read( USRADR, uarea(), ctob(UPAGES)) )
			return error;
	} else {
		if( error = read( USRADR, uarea(), ctob(UPAGES)) )
			return error;
		pr = *(struct proc *)(uarea() + u()->u_stack[0] - USRADR);
	}
	return 0;
}

char *HostCore.open()
{
	trace("%d.open() %d", this, _process); OK("HostCore.open");
	int mode = 2;
	while( procpath() && corefd<0 && mode>=0 )
		corefd = ::open(procpath(), mode--);
	if( procpath() && corefd<0 ) return SysErr("core image: " );
	if( procpath() && corefstat() )
		return SysErr("core image: " );
	if( corestat.st_mode & S_IEXEC )
		return "executable - should be process (/proc/123) or dump (core)";
	if( stabpath() ){
		stabfd = ::open(stabpath(),0);
		if( stabfd<0 ) return SysErr( "symbol tables: " );
	}
	proc dummy_pr;
	if( ::ioctl(corefd, PIOCGETPR, &dummy_pr) == 0 ){
		_online = 1;
		if( stabpath() == 0 )
			stabfd = ::ioctl(corefd, PIOCOPENT, 0);
	}
	stabfstat();
	_symtab = new Ed8SymTab(this, stabfd, _symtab);
	_symtab->read();
	return procpath() ? readcontrol() : 0;
}

char *HostCore.reopen(char *newprocpath, char *newstabpath)
{
	int mode = 2, newcorefd = -1, compstabfd = -1;

	trace("%d.reopen()", this); OK("HostCore.reopen");
	while( newprocpath && newcorefd<0 && mode>=0 )
		newcorefd = ::open(newprocpath, mode--);
	if( newcorefd<0 )
		return "cannot open process";
	proc dummy_pr;
	if( ::ioctl(newcorefd, PIOCGETPR, &dummy_pr) )
		return "not a live process";
	if( newstabpath )
		compstabfd = ::open(newstabpath, 0);
	else
		compstabfd = ::ioctl(newcorefd, PIOCOPENT, 0);
	struct stat compstabstat;
	if( compstabfd < 0 || ::fstat(compstabfd, &compstabstat) ){
		::close(newcorefd);
		return "symbol table error";
	}
	if( compstabstat.st_mtime != stabstat.st_mtime )
		return "symbol tables differ (modified time)";
	if( compstabstat.st_size != stabstat.st_size )
		return "symbol tables differ (file size)";
	::close(corefd);
	corefd = newcorefd;
	::close(compstabfd);
	return readcontrol();
}

#define ATTEMPTS 9		/* core file i/o */
#define SLEEP	2

char *HostCore.seekto(int &fd, long &addr, int &whence)
{
	long maxadr;

	trace( "%d.seekto(%d,0x%X,%d)", this, fd, addr, whence ); OK("seekto");
	if( !online() ) {
		if( (u_long)addr >= (u_long)(maxadr = SYSADR) )
			return "offset in system space";
		else if( addr >= USRADR )
			addr -= USRADR;
		else if( addr >= (maxadr = USRADR) - ctob(u()->u_ssize) )
			{ addr -= USRADR; whence = 2; }
		else if( addr < (maxadr = ctob(u()->u_tsize))){
			addr += TXTOFF(_symtab->magic());
			fd = stabfd;
		} else if (addr < (maxadr = ctob(u()->u_tsize+u()->u_dsize)))
			addr = addr - ctob(u()->u_tsize) + ctob(UPAGES);
		else
			return "offset beyond data segment";
	} else if( addr & SYSADR ){
			extern char *DEVKMEM;
			if( kmemfd == -1 )		// kmemfd per Core??
				kmemfd = ::open(DEVKMEM, 0);
			fd = kmemfd;
	}
	return 0;
}

char *HostCore.readwrite(long offset, char *buf, int r, int w)
{
	int fd = corefd, whence = 0, attempts = ATTEMPTS;
	extern int errno;
	char *msg = "core image:";

	trace( "%d.readwrite(0x%X,%d,%d,%d)", this, offset, buf, r, w ); OK("r/w");
	if( corefstat() )
		return "core image fstat error";
	char *error = seekto(fd, offset, whence);
	if( error )
		return sf("core image: %s", error);
	while(--attempts>=0){
		if( lseek(fd, offset, whence) == -1 )
			return sf("lseek(%d,0x%X,%d)", fd, offset, whence);
		if( r ){
			int got = ::read(fd, buf, r);
			for( int i = got; i < r; ++i ) buf[i] = 0;
			if( got > 0 ) return 0;
		}
		if( w && ::write(fd, buf, w) == w ) return 0;
		if( errno != EBUSY ) break;
		if( online() ){
			const SPRBUSY = (SLOCK|SSWAP|SPAGE|SKEEP|SDLYU|SWEXIT
					|SVFORK|SVFDONE|SNOVM|SPROCIO);
			msg = sf("core p_flag=0x%x: ", pr.p_flag);
			if( !(pr.p_flag&SPRBUSY) ) break;
			if( pr.p_flag&SPAGE ) ++attempts;
		}
		sleep(SLEEP);
	}
	return SysErr(msg);
}

#define CYCLE 4
Cslfd *HostCore.peek(long loc, Cslfd *fail)
{
	static i;
	static Cslfd *c;
	UCslfd u;

	trace( "%d.peek(0x%X,%d)", this, loc, fail ); OK(fail);
	if( read(loc, (char*)&u, 8) )
		return Core::peek(loc, fail);
	if( !c ) c = new Cslfd[CYCLE];
	Cslfd *p = c+(i++,i%=CYCLE);
	p->chr = u.chr;
	p->sht = u.sht;
	p->lng = u.lng;
	if( (u.sht&0xFF00) != 0x8000 ){	/* VAX reserved operand faults */
		p->flterr = 0;
		p->flt = u.flt;
		p->dbl = u.dbl;
	} else {
		p->flterr = "not a float";
		p->flt = 0.0;
		p->dbl = 0.0;
	}
	return p;
}

char *HostCore.peekstring(long loc, char *fail)
{
	static char buf[256];
	trace( "%d.peekstring(0x%X,%s)", this, loc, fail?fail:"0" ); OK(fail);
	char *error = read(loc, buf, 250);
	if( error )
		return fail ? fail : strcpy(buf,error);
	return buf;
}

#define STACK(a) ((a&0xC0000000)==0x40000000)
int HostCore.instack(long curfp, long prevfp )
{
	return ((curfp&0xC0000000)==0x40000000) && (curfp>prevfp);
}

CallStk *HostCore.callstack()	// do we have to peek everything twice?
{
	trace( "%d.callstack()", this );	OK(0);
	long size;
	long _fp = fp();
	for( size = 1; size<1000; ++size ){
		long __fp = peek(_fp+12)->lng;
		if( !instack(__fp, _fp) )
			break;
		_fp = __fp;
	}
	CallStk *c = new CallStk(size, this);
	_fp = fp();
	long _pc = pc();
	for( long i = 0; i < size; ++i ){
		c->fpf[i].fp = _fp;
		c->fpf[i].func = (Func*) _symtab->loctosym(U_FUNC, _pc);
		_pc = peek(_fp+16)->lng;
		_fp = peek(_fp+12)->lng;
	}
	return c;
}

Frame HostCore.frameabove(long _fp)
{
	Frame f(this);
	trace("%d.frameabove(%d)", this, _fp); OK(f);
	if( _fp ){
		f.ap = peek(_fp+ 8)->lng;
		f.pc = peek(_fp+16)->lng;
		f.fp = peek(_fp+12)->lng;
	} else {
		f.ap = ap();
		f.pc = pc();
		f.fp = fp();
	}	
	f.nargs = peek(f.ap)->lng;
	f.regsave = peek(f.fp+4)->lng;
	f.regbase = f.fp+20;
	return f;
}

char *HostCore.ioctl(int p)
{
	trace( "%d.ioctl(0x%X)", this, p );	OK("Core.ioctl");
	if( !online() )
		return "process: not live";
	if( ::ioctl( corefd, p, 0 )< 0 )
		return SysErr( "process control: " );
	return 0;
}

char *HostCore.signalmask(long mask)
{
	trace( "%d.signalmask(0x%X)", this, mask );	OK("Core.signalmask");

	if( ::ioctl(corefd,PIOCSMASK,&mask)<0 )
		return SysErr( "setting trace mask:" );
	return 0;
}

#define PSW_T   0x00000010
char *HostCore.pswT(long psw_loc, int t)
{
	long ps;

	trace( "%d.setpswT(0x%X, %d)", this, psw_loc, t );	OK("Core.setpswT");
	char *error = read(psw_loc, (char*) &ps, 4 );
	if( error ) return error;
	trace( "oldpsw=0x%X", ps );
	if( t )
		ps |= PSW_T;
	else
		ps &= ~PSW_T;
	return write(psw_loc, (char*)&ps, 4);
}

#define VAXRET 4
#define VAX_CHMK 0xBC
char *HostCore.popcallstack()
{
	trace("%d.popcallstack()", this, lo, hi); OK("HostCore.popcallstack");
	if( pr.p_stat != SSTOP )
		return "pop callstack: process not stopped";
	long savepc = pc();
	if( peek(savepc-2)->chr == VAX_CHMK )
		return "pop callstack: process in system call";
	savepc = pc();
	Cslfd *c = peek(savepc, 0);
	if( !c ) return "cannot pop callstack";
	char saveinst = c->chr;
	char *error = poke(savepc, VAXRET, 1);
	if( !error ) error = step();
	if( !error ) error = poke(savepc, saveinst, 1);
	return error;
}

char *HostCore.step(long lo, long hi)
{
	trace( "%d.step(%d,%d)", this, lo, hi ); OK("HostCore.step");
	static Trap *t;
	if(!t) t = new Trap(new Stmt(0,0,0),0);
	t->stmt->range.lo = hi;
	char *error = 0;
	if( hi && (error = laybpt(t)) ) return error;
	error = dostep(lo,hi,t);
	if( hi ){
		if( !error ) error = liftbpt(t);
		else liftbpt(t);
	}
	return error;
}

static Alarm;
static void SigCatch(int)
{
	extern int errno;
	trace("SigCatch()");
	Alarm = 1;
	errno = 0;
}

const char VAX_JSB = 0x16, VAX_RSB = 0x5;
char *HostCore.stepoverVAXJSB()
{
	char *error = 0;
	int depth = 0;

	trace( "%d.stepJSB()", this );	OK("Core.stepJSB");
	do {
		switch( peek(pc())->chr ){
			case VAX_JSB: ++depth; break;
			case VAX_RSB: --depth; break;
		}
		error = dostep(0,0,0);		/* ??? */
	} while( !error && depth <= 0 );
	return error;
}

const int STEPWAIT = 15;

char *HostCore.waitstop(long RETpsw)
{
	int oldalarm;
	char *error = 0;

	trace("%d.waitstop()", this);	OK("HostCore.waitstop");
	oldalarm = alarm(STEPWAIT);
	SIG_TYP oldsig = signal(SIGALRM, (SIG_ARG_TYP)&SigCatch);
	Alarm = 0;
	error = ioctl(PIOCWSTOP);
	signal(SIGALRM, (SIG_ARG_TYP)oldsig);
	alarm(oldalarm);
	if( Alarm ){
		ioctl(PIOCSTOP);
		sleep(2);
		if( RETpsw ) pswT(RETpsw,0);
		return sf("timeout (%d secs)", STEPWAIT);
	}
	return error;
}

char *HostCore.dostep(long lo, long hi, Trap *t)
{
	char *error = 0;
	long fp0, time0, time(long), RETpsw;

	trace( "%d.dostep(%d,%d,%d)", this, lo, hi, t );	OK("Core.dostep");
	time0 = ::time(0L);
	fp0 = fp();
	for(;;){
		if( hi && fp()<fp0 ){
			RETpsw = fp()+4;
			error = pswT(RETpsw,1);
		} else if( hi && peek(pc())->chr == VAX_JSB ){
			error = stepoverVAXJSB();
		} else {
			RETpsw = 0;
			error =	pswT((long)(PS + u()->u_ar0), 1);
		}
		if( !error && (event()==SIGTRAP || event()==SIGSTOP) )
			error = clrcurrsig();
		if( !error ) error = run();
		if( !error ) error = waitstop(RETpsw);
		if( !error ) error = readcontrol();
		if( !error && event()!=SIGTRAP )
			error = sf( "single step error. signal=%d", event() );
		if( !error ) error = clrcurrsig();
		if( error ) return error;
		trace( "pc=%d", pc() );
		if( hi && fp()<fp0 && pc()==hi ){
			if( error = liftbpt(t) ) return error;
			if( error = dostep(0,0,t) ) return error;
			if( error = laybpt(t) ) return error;
		}
		if( !hi 
		 || fp()>fp0
		 || (fp()==fp0 && (pc()<lo || pc()>=hi) ) )
			return 0;
		if( ::time(0L) > time0+STEPWAIT )
			return sf("single step timeout (%d secs)",STEPWAIT);
	}
}

char *HostCore.resources()
{
	static char buf[64];

	trace( "%d.resources()", this );	OK("resources");
	sprintf( buf, "%.1fu %.1fs",
		(double)u()->u_vm.vm_utime/60, (double)u()->u_vm.vm_stime/60 );
	return buf;
}

void Wait3()
{
#define WNOHANG		1		/* see <wait.h> */
	int wait3(int*,int,int*);
	for( int i = 0; i<10 && wait3(0,WNOHANG,0)>0; ++i ) {}		/* 10? */
}

char *HostCore.destroy()
{
	clrcurrsig();
	char *error = sendsig(SIGKILL);
	Wait3();
	return error;
}

char *HostCore.clrcurrsig()
{
	trace("%d.clrcurrsig(%d)", this);	OK("HostCore.clrcurrsig");
	char *error = ioctl(PIOCCSIG);
	if( !error )
		pr.p_cursig = 0;
	return error;
}

char *HostCore.sendsig(long sig)
{
	trace( "%d.sendsig(%d)", this, sig );	OK("HostCore.sendsig");
	if( !online() ) return "send signal: process not live";
	if( ::ioctl(corefd, PIOCKILL, &sig) >= 0 )
		return 0;
	return SysErr( "send signal (PIOCKILL): " );
}

char *HostCore.docall(long addr, int numarg)
{
	const int CALL_SEQ=2, CALL_SIZE=7, CALLS=0xFB, PCREL=0xEF;
	char save[CALL_SIZE], *error;
	int i;

	trace( "%d.docall(%d,%d)", this, numarg, addr ); OK("docall");
	if( pr.p_stat != SSTOP )
		return "process not stopped";
	if( !online() )
		return "cannot restart dump";
	for( i = 0; i < CALL_SIZE; ++i )
		save[i] = peek(CALL_SEQ+i)->chr;
	if( ( error = poke(CALL_SEQ+0, CALLS, 1) )
	 || ( error = poke(CALL_SEQ+1, numarg, 1) )
	 || ( error = poke(CALL_SEQ+2, PCREL, 1) )
	 || ( error = poke(CALL_SEQ+3, addr-(CALL_SEQ+7), 4) ) )
		return error;
	if( ( error = regpoke(15, CALL_SEQ) )
	 || ( error = step( CALL_SEQ, CALL_SEQ+CALL_SIZE) ) )
		return error;
	for( i = 0; i < CALL_SIZE; ++i )
		if( error = poke(CALL_SEQ+i, save[i], 1 ) )
			return error;
	return 0;
}

long HostCore.apforcall(int argbytes)
{
	trace( "%d.apforcall(%d)", this, argwords ); OK(0);
	regpoke(REG_SP(), sp() - argbytes );
	return sp() - 4;
}

long HostCore.returnregloc()
{
	trace( "%d.returnregloc()", this ); OK(0);
	return regloc(0);
}

Context* HostCore.newContext()
{
	trace( "%d.newContext()", this );	OK(0);
	HostContext *cc = new HostContext;
	cc->error = 0;
	cc->core = this;
	cc->pending = 0;
	if( pr.p_stat != SSTOP )
		cc->error = "context save: process not stopped";
	else if( peek(pc()-2)->chr == VAX_CHMK )
		cc->error = "context save: process in system call";
	else if( cc->pending = pr.p_cursig )
		cc->error = clrcurrsig();
	if( !cc->error )
		for( int i = 0; i <= 16; ++i )
			cc->regs[i] = regpeek(i);
	return cc;
}

void HostContext.restore()	// should use only PIOCSSIG eventually
{
	trace( "%d.restore()", this );	VOK;
	if( pending ){
		if( ::ioctl(core->corefd, PIOCSSIG, &pending) == 0 )
			core->pr.p_cursig = pending;
		else {
			static once = 0;
			if( once++ == 0 )
				PadsWarn("warning: unix out of date?: PIOCSSIG");
			if( error = core->sendsig(pending) )
				return;
			if( pending == SIGTRAP ){
				if( error = core->run() ) return;
				if( error = core->waitstop() ) return;
				error = core->readcontrol();
			} else
				error = core->step();	// other signals
			if( error  )
				return;
		}
	}
	for( int i = 0; i <= 16; ++i )
		if( error = core->regpoke(i, regs[i]) )
			return;
}

void WaitForExecHang(char *procpath)
{
	int fd = open(procpath, 0);
	for( int i = 1; i <= 15; ++i ){
		proc p;
		if( ::ioctl(fd, PIOCGETPR, &p) >= 0 ){
			if( p.p_stat == SSTOP ){
				close(fd);
				return;
			}
		}
		sleep(1);
	}
	close(fd);
}

