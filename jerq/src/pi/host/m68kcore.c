#include "master.pri"
#include "m68kcore.h"
#include "symtab.pri"
#include "process.pub"		// needed because of stabpath
#include "frame.pri"
#include "symbol.h"
#include "asm.pri"
#include "format.pub"
#include "bpts.pri"
SRCFILE("m68kcore.c")

const int REG_A0 = 8;

int M68kCore.REG_FP()			{ return REG_A0+6; }
int M68kCore.REG_AP()			{ return REG_A0+6; }
int M68kCore.REG_SP()			{ return 15; }

char *M68kCore.step(long lo, long hi)
{
	return "step";
}

char *M68kCore.resources()
{
	return "resources";
}

long M68kCore.savearea() { return 0; }

#define REGDATABYTE(s) ( size == 1 ? 3 : size == 2 ? 2 : 0 )
long M68kCore.regloc(int r, int size)
{
	return savearea()+4*r + REGDATABYTE(size);
}

long M68kCore.pc()
{
	const PC_OFFSET = 70;
	return peek(savearea()+PC_OFFSET)->lng;
}

long M68kCore.saved(Frame *f, int r, int size)
{
	trace("%d.saved(%d,%d,%d)", this, f, r, size); OK(0);
	if( !f ) return 0;
	Func *func = f->func;
	if( !func || !func->regused(r) ) return 0;
	int higher = 0;
	for( ++r; r<16; ++r )
		if( func->regused(r) ) ++higher;
	return f->fp-(higher+1)*4 + REGDATABYTE(sise);
}

CallStk *M68kCore.callstack()
{
	long size, _fp = fp();
	for( size = 1; size<1000; ++size ){
		long __fp = peek(_fp)->lng;
		if( __fp < stack_lo
		 || __fp > stack_hi
		 || __fp < _fp ) break;
		_fp = __fp;
	}
	CallStk *c = new CallStk(size, this);
	_fp = fp();
	long _pc = pc();
	for( long i = 0; i < size; ++i ){
		c->fpf[i].fp = _fp;
		c->fpf[i].func = (Func*) _symtab->loctosym(U_FUNC, _pc);
		_pc = peek(_fp+4)->lng;
		_fp = peek(_fp)->lng;
	}
	return c;
}

Frame M68kCore.frameabove(long _fp)
{
	Frame f(this);
	trace("%d.frameabove(%d)", this, _fp); OK(f);
	if( _fp ){
		f.pc = peek(_fp+4)->lng;
		f.fp = peek(_fp)->lng;
	} else {
		f.pc = pc();
		f.fp = fp();
	}	
	f.ap = f.fp;
//	f.regbase = f.fp+20;
	return f;
}

char *M68kCore.laybpt(Trap *t)
{
	return "laybpt";
}

char *M68kCore.liftbpt(Trap *t)
{
	return "liftbpt";
}

char *M68kCore.run()
{
	return "run";
}

char *M68kCore.stop()
{
	return "stop";
}

char *M68kCore.regname(int r)
{
	char *aord;
	switch( r){
	case  0:
	case  1:
	case  2:
	case  3:
	case  4:
	case  5:
	case  6:
	case  7: aord = "d"; break;
	case  8:
	case  9:
	case 10:
	case 11:
	case 12:
	case 13: aord = "a"; r -= 8; break;
	case 14: return "fp";
	case 15: return "sp";
	default: aord = "badreg";
	}
	return sf("%s%d", aord, r);
}

char *M68kCore.eventname()
{
	long const TRAP_OFFSET = 64;
	long trap_code = peek(savearea()+TRAP_OFFSET)->lng;
	switch( trap_code ){
		case  2: return "Bus Error";
		case  3: return "Address Error";
		case  4: return "Illegal Instruction";
		case  5: return "Zero Divide";
		case  6: return "CHK Instruction";
		case  7: return "TRAPV Instruction";
		case  8: return "Privilege Violation";
		case  9: return "Trace";
		case 10: return "Line 1010 Emulator";
		case 11: return "Line 1111 Emulator";
	}
	return sf("Exception %d (prog ref man tbl 4-2)", trap_code);
}

Behavs M68kCore.behavs()
{
	return PENDING;
}

char *M68kCore.problem()
{
	return Core::problem();
}

M68kCore.M68kCore(Process *p, Master *m):(p,m)
{
	_online = 0;
}

char *M68kCore.open()
{
	char *error;

	trace("%d.open() %d %s", this, _process, procpath()); OK("M68kCore.open");
	int mode = 2;
	while( procpath() && corefd<0 && mode>=0 )
		corefd = ::open(procpath(), mode--);
	if( procpath() && corefstat() )
		return SysErr("core image: " );
	if( stabpath() ){
		stabfd = ::open(stabpath(),0);
		if( stabfd<0 ) return SysErr( "symbol tables: " );
		ReadOK(stabfd, (char*)&hdr, sizeof(exec));
	}
	_symtab = (SymTab*) new MccSymTab(this, stabfd, _symtab);
	_symtab->read();
	return 0;
}

char *M68kCore.readcontrol()
{
	return "readcontrol";
}

char *M68kCore.seekto(int &fd, long &offset)
{
	return "seekto";	
}

char *M68kCore.readwrite(long offset, char *buf, int r, int w)
{
	int fd = corefd;
	extern int errno;
	char *error;

	trace( "%d.readwrite(0x%X,%d,%d,%d)", this, offset, buf, r, w ); OK("r/w");
	if( error = seekto(fd, offset) )
		return error;
	if( lseek(fd, offset, 0) == -1 )
		return sf("lseek(%d,0x%X,0)", fd, offset);
	if( r ){
		int got = ::read(fd, buf, r);
		for( int i = got; i < r; ++i ) buf[i] = 0;
		if( got > 0 ) return 0;
	}
	if( w && ::write(fd, buf, w) == w ) return 0;
	return "read/write error";
}

char *M68kCore.read(long l,char *b,int n)	{ return readwrite(l,b,n,0); }
char *M68kCore.write(long l,char *b,int n)	{ return readwrite(l,b,0,n); }

#define CYCLE 4
Cslfd *M68kCore.peek(long loc, Cslfd *fail)	/* 68000 no floats */
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

Asm *M68kCore.newAsm()	{ return new M68kAsm(this); }

char *M68kCore.poke(long loc, register long l, int n)
{
	trace( "%d.poke(0x%X,0x%X,%d)", this, loc, l, n ); OK("M68kCore.poke");
	char raw[4];
	register char *r = raw+4;
	while( r >= raw ){
		*--r = l;
		l >>= 8;
	}
	return write( loc, raw+4-n, n );
}

char *M68kCore.peekstring(long loc, char *fail)
{
	static char buf[256];
	trace( "%d.peekstring(0x%X,%s)", this, loc, fail?fail:"0" ); OK(fail);
	char *error = read(loc, buf, 250);
	if( error )
		return fail ? fail : strcpy(buf,error);
	return buf;
}

Context* M68kCore.newContext()
{
	return 0;
}

long M68kCore.apforcall(int bytes)
{
	return 0;
}

char *M68kCore.docall(long addr, int)
{
	trace( "%d.docall(0x%X)", this, addr ); OK("docall");
	return "docall";
}

long M68kCore.returnregloc()
{
	trace( "%d.returnregloc()", this ); OK(0);
}
