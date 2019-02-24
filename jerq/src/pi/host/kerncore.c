#include "process.pri"
#include "frame.pri"
#include "symtab.pri"
#include "symbol.h"
#include "hostcore.h"
#include <sys/reg.h>
#include "master.pri"
SRCFILE("kerncore.c")

#define P0P1_PROG	0x00000000
#define P0P1_STCK	0x40000000
#define P0P1_SYST	0x80000000
#define P0P1_MASK	0xC0000000
#define VPN_MASK	0x3FFFFE00
#define PAGE_MASK	0x000001FF
#define PAGE_SHIFT	9
#define PFN_MASK	0x001FFFFF
#define FOD_MASK	0x02000000

class Pte {
public:
	long	e;
	long	pfn()		{ return e&PFN_MASK;			}
	int	fod()		{ return e&FOD_MASK;			}
	int	valid()		{ return e<0 || (!fod() && pfn());	}
	long	pfntob()	{ return pfn()<<PAGE_SHIFT;		}
};

char *KernCore.specialop(char *s)
{
	trace( "%d.specialop(%s)", this, s?s:"0" ); OK(0);
	if( !strcmp(s, "pte") )
		return "pcb = $";
	return 0;
}

char *KernCore.special(char *s, long addr)
{
	trace("%d.special(%s,0x%X)", this, s?s:"0", loc); OK(0);
	IF_LIVE(strcmp(s, "pte")) return 0;
	char *error = getpcb(peek(addr)->lng);
	if( !error )
		_process->habeascorpus(PENDING, 0);
	return error;
}

char *KernCore.getpcb(long addr)
{
	trace("%d.getpcb(0x%X)", this, addr);	OK("KernCore.getpcb");
	lseek(corefd, pcb_loc = (addr&PFN_MASK)<<PAGE_SHIFT, 0);
	if( !ReadOK(corefd, (char*)&pcb_copy, sizeof pcb_copy) )
		return "can't read pcb";
	pcb_copy.pcb_p0lr &= ~AST_CLR;
	return 0;
}

char *KernCore.open()
{
	trace("%d.open() %d", this, _process); OK("KernCore.open");
	int mode = 2;
	while( corefd<0 && mode>=0 )
		corefd = ::open(procpath(), mode--);
	if( corefstat() )
		return SysErr("core image: " );
	if( stabpath() ){
		stabfd = ::open(stabpath(),0);
		if( stabfd<0 ) return SysErr( "symbol tables: " );
	}
	_online = 0;
	stabfstat();
	if( stabfd >= 0 ) _symtab = 0;		/* don't inherit /unix */
	_symtab = new Ed8SymTab(this, stabfd, _symtab);
	_symtab->read();
	static char *id[4] = { "Sysmap",  "Syssize",  "masterpaddr",  "intstack" };
	static char *_id[4]= { "_Sysmap", "_Syssize", "_masterpaddr", "_intstack" };
	Symbol *glb[4];
	SSet glbfun(U_GLB,  U_FUNC);
	for( int i = 0; i <= 3; ++i )
		if( !(glb[i] = _symtab->idtosym(glbfun,  id[i]))
		 && !(glb[i] = _symtab->idtosym(glbfun, _id[i])) )
			return sf("not found: %s", id[i]);
	sbr = glb[0]->range.lo;
	slr = glb[1]->range.lo;
	intstack = glb[3]->range.lo;
	char *error = getpcb(peek(glb[2]->range.lo&~P0P1_SYST)->lng);
	if( error ) return error;
	return readcontrol();
}

char *KernCore.readcontrol()
{
	trace( "%d.readcontrol()", this ); OK("KernCore.readcontrol()");
	if( corefstat() )
		return "cannot fstat core image";
	return 0;
}

Behavs KernCore.behavs()
{
	trace( "%d.behavs()", this );	OK(ERRORED);
	char *error = readcontrol();
	if( error ){
		behavs_problem = sf( "readcontrol(): %s", error );
		return ERRORED;
	}
	return PENDING;
}

char *KernCore.eventname()
{
	OK("eventname");
	return
	sf("sbr=0x%X slr=0x%X p0br=0x%X p0lr=0x%X p1br=0x%X p1lr=0x%X",
	sbr, slr,
	pcb_copy.pcb_p0br, pcb_copy.pcb_p0lr, pcb_copy.pcb_p1br, pcb_copy.pcb_p1lr
	);
}

int KernCore.event()		{ return 0; }		/* ever called? */

char *KernCore.seekto(int&, long &addr, int&)
{
	trace("%d.seekto(%d,0x%X,%d)", this, addr); OK("seekto");
	Pte p;
	long page = (addr&VPN_MASK)>>PAGE_SHIFT, po = addr&PAGE_MASK;
	switch( addr & P0P1_MASK ){
	case P0P1_STCK:
		if( page < pcb_copy.pcb_p1lr )
			return "page < p1lr";
		addr = (long)(pcb_copy.pcb_p1br+page);
		if( !(addr&P0P1_SYST) )
			return "user stack page table error";
		seekto(0, addr, 0);
		p.e = peek(addr)->lng;
		if( !p.valid() )
			return "invalid page table entry";
		addr = p.pfntob() + po;
		break;
	case P0P1_SYST:
		if( page > slr )
			return "page > slr";
		p.e = peek((sbr+page*4)&~P0P1_SYST)->lng;
		if( !p.valid() )
			return "invalid page table entry";
		addr = p.pfntob() + po;
		break;
	default:
		return 0;
	}
	return 0;
}

long KernCore.regloc(int r,int)
{
	return pcb_loc + ((long)(&pcb_copy.pcb_r0) - (long)&pcb_copy) + 4*r;
}

long KernCore.pc()
{
	return pcb_copy.pcb_pc;
}

long KernCore.fp()
{
	return cs_fp ? cs_fp : peek(intstack-4)->lng;
}

int KernCore.instack(long curfp, long prevfp )
{
	return curfp && (curfp>prevfp);
}
