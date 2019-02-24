#define IDTOCORE
#include "termcore.h"
#include "symtab.pri"
#include "process.pub"		/* needed because of stabpath */
#include "master.pri"		/* remote */
#include "frame.pub"
#include "symbol.h"
#include "asm.pri"
#include "bpts.pri"
SRCFILE("termcore.c")

void CoreVersion(Remote *r,char *comment)
{
	r->pktstart(C_VERSION);
	r->sendlong(PI_VERSION);
	r->sendstring(comment);
	r->pktflush();
}

char *MuxName(short r)
{
	switch( r ){
		case MUX_RUN:		return "RUN";
		case MUX_BUSY:		return "BUSY";
		case MUX_BLOCKED:	return "BLKD";
		case MUX_USER:		return "USER";
		case MUX_KBDLOCAL:	return "KBDL";
		case MUX_MOUSELOCAL:	return "MOUSL";
		case MUX_GOTMOUSE:	return "GOTM";
		case MUX_WAKEUP:	return "WAKE";
		case MUX_MOVED:		return "MOVD";
		case MUX_UNBLOCKED:	return "UNBLK";
		case MUX_ZOMBIE:	return "ZOMB";
		case MUX_RESHAPED:	return "RSHPD";
		case MUX_ZOMBOOT:	return "ZBOOT";
		case MUX_ALARMREQD:	return "ALMRQ";
	}
	return sf( "0x%X", r );
}

int TermCore.REG_AP()			{ return 10; }
int TermCore.REG_FP()			{ return  9; }
int TermCore.REG_PC()			{ return 15; }
int TermCore.REG_SP()			{ return 12; }

char *TermCore.step(long lo, long hi)
{
	trace( "%d.step(0x%X,0x%X)", this, lo, hi ); OK("TermCore.step");
	pktstart( CP_STEP );
	sendlong(P);
	sendlong(lo);
	sendlong(hi);
	pktflush();
	P_state = rcvlong();
	P_pc = rcvlong();
	P_psw = rcvlong();
	return rcverror("step: ");
}

char *TermCore.resources()
{
	short r;
	Bls res;

	for( r = 1; r; r <<= 1 ){
		if( P_state & r )
			res.af( "%s ", MuxName(r) );
	}
	return sf( "%s", res.text );
}

#define LASTREG 8
#define SAVE_INST 0x10		/* SAVE Rn saves n..LASTREG */
#define REGDATABYTE(s) ( size == 1 ? 3 : size == 2 ? 2 : 0 )

long TermCore.regloc(int r, int size)
{
	long loc, i;
	trace("%d.regloc(%d,%d,%d)", this, f, r, size); OK(0);

	if( cache )
		for( i = lo; i <= hi; ++i )
			if( loc = saved(cache+i, r, size) )
				return loc;
	pktstart(CP_REGLOC);
	sendlong(P);
	senduchar(r);
	pktflush();
	loc = rcvlong();
	if( rcverror() ) return 0;
	return loc + REGDATABYTE(size);
}

long TermCore.saved(Frame *f, int r, int size)	/* search symtab, then RAM */
{
	trace( "%d.saved(%d,%d,%d)", this, f, r, size ); OK(0);
	Func *func;

	if( !f || !f->regbase ) return 0;
	if( !(func = f->func) ){
		f->addsymbols();
		if( !(func = f->func) ) return 0;
	}
	if( func->regsave == -1 ){
		if( peek(func->range.lo)->chr != SAVE_INST )
			func->regsave = LASTREG+1;
		else
			func->regsave = 0xF & peek(func->range.lo+1)->chr;
	}
	if( r>LASTREG || r<func->regsave || !f->regbase )
		return 0;
	if( size != 1 && size != 2 ) size = 4;
	return f->regbase+(r-func->regsave)*4 + REGDATABYTE(size);
}

#define ADDW2_INST 0x9C
char *TermCore.stepprolog()
{
	char *error = 0;
	trace( "%d.stepprolog(%d)", this ); OK("TermCore.stepprolog");
	if( peek(pc())->chr == SAVE_INST )
		error = step();
	if( !error && (unsigned char)peek(pc())->chr == ADDW2_INST )
		error = step();
	return error;
}

CallStk *TermCore.callstack()
{
	trace( "%d.callstack()", this ); OK(0);
	if( !cache )
		cache = new Frame[FRAME_CACHE];
	pktstart(CP_STACK);
	sendlong(P);
	sendlong((long)cache);
	pktflush();
	lo = rcvuchar();
	hi = rcvuchar();			// [hi]=deepest, [0]=start
	for( long i = lo; i <= hi; ++i ){
		cache[i].fp = rcvlong();
		cache[i].ap = rcvlong();
		cache[i].pc = rcvlong();
		cache[i].regbase = cache[i].fp-24;
		cache[i].core = this;
	}
	if( rcverror() ){
		lo = 0;
		hi = -1;
		return 0;
	}
	for( lo = hi; lo >= 0; --lo )
		if( cache[lo].pc >= relocate )
			break;
	long lopc = cache[lo].pc;		// [lo]=deepest user
	Behavs b = behavs();
	if( (b == BREAKED || b == PENDING) && lo < hi ){
		++lo;
		lopc = cache[lo].pc;
		cache[lo].pc = P_pc;
		hi = -1;
	}
	CallStk *cs;
	if( lo<=0 ){
		cs = new CallStk(1, this);
		cs->fpf[0].fp = cache[0].fp;
		cs->fpf[0].func = (Func*) _symtab->loctosym(U_FUNC, cache[0].pc);
	} else {
		cs = new CallStk(lo, this);
		for( i = lo; i>0; --i ){
			cs->fpf[lo-i].fp = cache[i].fp;
			cs->fpf[lo-i].func = (Func*) _symtab->loctosym(U_FUNC, cache[i].pc);
		}
	}
	cache[lo].pc = lopc;
	++lo;					// [lo]=sw
	return cs;
}

Frame TermCore.frameabove(long _fp)
{
	trace("%d.frameabove(0x%X)", this, _fp);
	long i = lo-1;
	if( !_fp ){
		Frame f = cache[lo-1];
		Behavs b = behavs();
		if( b == BREAKED || b == PENDING )
			f.pc = P_pc;
		return f;
	}
	for( i = lo-1; i>1; --i )
		if( cache[i].fp == _fp )
			break;
	return cache[i-1];
}

char *TermCore.rcverror(char *s)
{
	char error[256];
	return rcvstring(error)[0] ? sf("%s %s", s, error) : 0;
}

char *TermCore.laybpt(Trap *t)
{
	trace( "%d.laybpt(%d)", this, t ); OK("TermCore.laybpt");
	pktstart(C_LAYBPT);
	sendlong(t->stmt->range.lo);
	pktflush();
	t->saved = rcvuchar();
	return rcverror("lay bpt: ");
}

char *TermCore.liftbpt(Trap *t)
{
	trace( "%d.liftbpt(%d)", this, t ); OK("TermCore.liftbpt");
	pktstart(C_LIFTBPT);
	sendlong(t->stmt->range.lo);
	senduchar(t->saved);
	pktflush();
	return rcverror("lift bpt: ");
}

char *TermCore.run()
{
	trace( "%d.run()", this ); OK("TermCore.run");
	pktstart(CP_RUN);
	sendlong(P);
	pktflush();
	return rcverror("run: ");
}

char *TermCore.stop()
{
	trace( "%d.run()", this ); OK("TermCore.run");
	pktstart(CP_STOP);
	sendlong(P);
	pktflush();
	return rcverror("stop: ");
}

char *TermCore.specialop(char *s)
{
	extern char *COREHOST;
	trace( "%d.specialop(%s)", this, s?s:"0" );
	if( COREHOST ) return 0;
	for( int i = 0; DisplayTable[i].id; ++i )
		if( eqstr(s, DisplayTable[i].id ) )
			return "graphics $";
	return 0;
}

char *TermCore.special(char *s, long loc)
{
	char op = 0;
	trace( "%d.special(%s,0x%X)", this, s?s:"0", loc );
	for( int i = 0; DisplayTable[i].id; ++i )
		if( eqstr(s, DisplayTable[i].id ) )
			op = DisplayTable[i].op;
	if( !op ) return "not implemented";
	pktstart(op);
	sendlong(loc);
	pktflush();
	return rcverror();
}

char *TermCore.regname(int r)
{
	switch( r ){
	case  9:	return "fp";
	case 10:	return "ap";
	case 11:	return "psw";
	case 12:	return "sp";
	case 13:	return "pcbp";
	case 14:	return "isp";
	case 15:	return "pc";
	default:	return sf( "r%d", r );
	}
}

char *TermCore.eventname()
{
	switch( P_psw&FT_MASK ){
	case FT_RESET:			return "reset fault";
	case FT_PROCESS:		return "process fault";
	case FT_STACK:			return "stack fault";
	case FT_NORMAL: switch( P_psw&ISC_MASK ){
		case NORMAL_ZERODIV:	return "zero divide";
		case NORMAL_TRACE:	return "trace";
		case NORMAL_ILLOP:	return "illegal opcode";
		case NORMAL_RESOP:	return "reserved opcode";
		case NORMAL_INVDESC:	return "invalid descriptor";
		case NORMAL_EXTMEM:	return "external memory fault";
		case NORMAL_GATE:	return "gate vector fault";
		case NORMAL_LEVEL:	return "illegal level change";
		case NORMAL_RESDATA:	return "reserved data type";
		case NORMAL_OFLOW:	return "overflow";
		case NORMAL_PRIVOP:	return "privileged opcode";
		case NORMAL_BPT:	return "breakpoint";
		case NORMAL_PRIVREG:	return "privileged register";
		}
	}
	return sf( "PSW=0x%X", P_psw );
}

Behavs TermCore.behavs()
{
	trace( "%d.behavs()", this );	OK(ERRORED);
	if( readcontrol() ) return ERRORED;
	if( !(P_state&MUX_ZOMBIE) ) return ACTIVE;
	if( P_psw & (FT_MASK|ISC_MASK) ){
		if( (P_psw&FT_MASK) != FT_NORMAL ) return PENDING;
		switch( P_psw&ISC_MASK ){
			case NORMAL_BPT:   return BREAKED;
		}
		return PENDING;
	}
	return HALTED;
}

char *TermCore.problem()
{
	trace( "%d.problem()", this );	OK("TermCore.problem");
	char *error = readcontrol();
	if( error ) return error;
	return Core::problem();
}

TermCore.TermCore(Process *p, TermMaster *m):(p,m)
{
	remote = m->remote;
	_online = 1;
	argarea = 0;
}

long TermCore.rcvlong()				{ return remote->rcvlong();	}
short TermCore.rcvshort()			{ return remote->rcvshort();	}
unsigned char TermCore.rcvuchar()		{ return remote->rcvuchar();	}
char *TermCore.rcvstring(char *x)		{ return remote->rcvstring(x);	}

void TermCore.sendlong(long  x)			{ remote->sendlong(x);		}
void TermCore.sendshort(short x)		{ remote->sendshort(x); 	}
void TermCore.senduchar(unsigned char x)	{ remote->senduchar(x); 	}
void TermCore.sendstring(char *x)		{ remote->sendstring(x);	}

void TermCore.pktstart(char c)			{ remote->pktstart(c);		}
void TermCore.pktend()				{ remote->pktend();		}
void TermCore.pktflush()			{ remote->pktflush();		}

char *TermCore.open()
{
	char *error;

	trace("%d.open() %d %s", this, _process, procpath()); OK("Core.open");
	P = 0;
	if( procpath() ){
		if( sscanf(procpath(), "%*[Pp=0xX]%x", &P) != 1 || !P )
			return "invalid process name - should be P=0Xnnnnn";
	}
	relocate = 0;
	if( P ){				/* relocate for symtab->read */
		if( error = readcontrol() )
			return error;
	}
	if( stabpath() )
		stabfd = ::open( stabpath(), 0 );
	if( _symtab )		/* for rob - only works for one process */
		_symtab->_core = this;
	_symtab = (SymTab*) new CoffSymTab(this, stabfd, _symtab, relocate);
	_symtab->read();
	if( P ){
		Var *v = new Var(_symtab, 0, 0, U_GLB, "P" );
		UType *t = (UType*) _symtab->idtosym(U_UTYPE, "Proc");
		if( t ){
			v->range.lo = P;
			v->range.hi = 0;
			v->type = t->type;
		}
	}
	return 0;
}

long TermCore.pc()
{
	return P_pc;
}

char *TermCore.readcontrol()
{
	char error[256];
	long r;
	trace( "%d.readcontrol()", this ); OK("Core.readcontrol()");
	pktstart(CP_CONTROL); sendlong(P); pktflush();
	r = rcvlong();
	P_state = rcvlong();
	P_pc = rcvlong();
	P_psw = rcvlong();
	if( rcvstring(error)[0] )
		return sf("process access: %s", error);
	if( !(P_state&MUX_BUSY) )
		return "process has gone away";
	if( relocate == 0 ? r == 0 : relocate != r )
		return "process deleted/reloaded?";
	relocate = r;
	return 0;
}

char *TermCore.read(long loc, char *buf, int r)
{
	trace( "%d.read(%d,%d,%d)", this, loc, buf, r ); OK("TermCore.read");
	pktstart(C_READ);
	sendlong(loc);
	sendlong(r);
	pktflush();
	while(r-->0) *buf++ = rcvuchar();
	return rcverror("read : ");
}

char *TermCore.write(long loc, char *buf, int w)
{
	trace( "%d.write(%d,%d,%d)", this, loc, buf, w ); OK("TermCore.write");
	pktstart(C_WRITE);
	sendlong(loc);
	sendlong(w);
	while(w-->0) senduchar(*buf++);
	pktflush();
	return rcverror("write : ");
}

#define CYCLE 4
Cslfd *TermCore.peek(long loc, Cslfd *fail)	/* mac32 no floats */
{
	static i;
	static Cslfd *cycle;

	trace( "%d.peek(0x%X,%d)", this, loc, fail ); OK(fail);
	if( !cycle ) cycle = new Cslfd[CYCLE];
	unsigned char raw[4];
	Cslfd c;
	if( read(loc, raw, 4) )
		return Core::peek(loc, fail);
	c.chr =  raw[0];
	c.sht = (raw[0]<<8 ) |  raw[1];
	c.lng =	(raw[0]<<24) | (raw[1]<<16) | (raw[2]<<8) | raw[3];
	c.dbl = c.flt = 0.0;
	c.flterr = "float undefined";
	cycle[(i++, i%=CYCLE)] = c;
	return cycle+i;
}

Cslfd *TermCore.peekcode(long loc)
{
	static Cslfd *c;
	UCslfd u;

	if( !c ) c = new Cslfd;
	trace( "%d.peekcode(0x%X)", this, loc ); OK(c);
	read(loc, (char*) &u, 4);
	c->chr = u.chr;
	c->sht = u.sht;
	c->lng = u.lng;
	return c;
}

Asm *TermCore.newAsm()	{ return new Mac32Asm(this); }

char *TermCore.poke(long loc, register long l, int n)
{
	trace( "%d.poke(0x%X,0x%X,%d)", this, loc, l, n ); OK("TermCore.poke");
	char raw[4];
	register char *r = raw+4;
	while( r >= raw ){
		*--r = l;
		l >>= 8;
	}
	return write( loc, raw+4-n, n );
}

char *TermCore.peekstring( long loc, char *fail )
{
	static char buf[260];
	trace( "%d.peekstring(0x%X,%s)", this, loc, fail?fail:"0" ); OK(fail);
	pktstart(C_STRING);
	sendlong(loc);
	pktflush();
	rcvstring(buf);
	char error[256];
	return !rcvstring(error)[0]
		? buf
		: fail ? fail : sf( "string error: %s", error );
}

Context* TermCore.newContext()
{
	trace( "%d.newContext()", this );	OK(0);
	TermContext *cc = new TermContext(argarea);
	argarea = 0;
	cc->core = this;
	cc->error = 0;
	if( !(P_state&MUX_ZOMBIE) )
		cc->error = "context save: process not stopped";
	return cc;
}

void TermContext.restore()
{
	trace( "%d.restore()", this );	VOK;
	core->freeargarea();
	core->argarea = argarea;
}

void TermCore.freeargarea()
{
	trace( "%d.freeargarea()", this );	VOK;
	if( argarea ){
		pktstart(CF_FREE);
		sendlong(argarea);
		pktend();
		argarea = 0;
	}
}

long TermCore.apforcall(int bytes)
{
	trace( "%d.apforcall()", this );	OK(0);
	if( argarea )
		return 0;		/* save/call/restore nesting error */
	pktstart(CF_ALLOC);
	sendshort(bytes);
	pktflush();
	return argarea = rcvlong();
}

char *TermCore.docall(long addr, int)
{
	trace( "%d.docall(0x%X)", this, addr ); OK("docall");
	if( !addr || !argarea )
		return "TermCore.docall() error";
	pktstart(CF_CALL);
	sendlong(addr);
	sendlong(argarea);
	pktflush();
	retloc = rcvlong();
	return rcverror();
}

long TermCore.returnregloc()
{
	trace( "%d.returnregloc()", this ); OK(0);
	return retloc;
}
