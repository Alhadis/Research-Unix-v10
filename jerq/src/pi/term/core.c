#include "term.h"
#include "../protocol.h"

Read()
{
	register char *loc = (char*) RcvLong(), c, *error = 0;
	register long bytes =  RcvLong();

	journal( "Read", "loc=%x bytes=%d", loc, bytes );
	while( bytes-- > 0 ){
		if( !error && !(error = ReadErr(loc,1)) )
			c = *loc++;
		SendUChar(c);
	}
	SendError(error);
}

Write()
{
	register char *loc = (char*) RcvLong(), c, *error = 0;
	register long bytes =  RcvLong();

	journal( "Write", "loc=%x bytes=%d", loc, bytes );
	while( bytes-- > 0 ){
		c = RcvUChar();
		if( !error && !(error = WriteErr(loc,1)) )
			*loc++ = c;
	}
	SendError(error);
}

String()
{
	register char *s = (char*) RcvLong(), *error;
	journal( "String", "%x", s );
	if( (error = ReadErr(s,1)) || (error = ReadErr(s+260,1)) )
		s = "";
	SendString(s);
	SendError(error);
}

#define BPT_INST 0x2E

LayBpt()
{
	register char *loc = (char*) RcvLong(), save, *error;

	error = WriteErr(loc,1);
	if( !error ){
		save = *loc;
		if( save == BPT_INST )
			error = "BPT already there";
		else
			*loc = BPT_INST;
	}
	journal( "LayBpt", "loc=%x save=%x %s", loc, save, error );
	SendUChar(save);
	SendError(error);
}

LiftBpt()
{
	register char *loc = (char*) RcvLong();
	register char save = RcvUChar(), *error;

	error = WriteErr(loc,1);
	if( !error ){
		if( *loc != BPT_INST )
			error = "BPT not found";
		else
			*loc = save;
	}
	journal( "LiftBpt", "loc=%x save=%x %s", loc, save, error );
	SendError(error);
}

char *RegLoc(p,error)
register Proc *p; register char *error;
{
	register short r = RcvUChar();
	register long loc = 0;

	if( !error ){
		if( NormalISC(p) && r>=0 && r<=12 )	/* mux/term/pl.s */
			loc = 4*(r+1) + (long) p->traptype;
		else
			switch( r ){
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					loc = (long) &p->pcb.reg[r];
					break;
				case 9:
				case 10:
					loc = (long) &p->pcb.reg[8-r];
					break;
				case 11:
					loc = (long) &p->pcb.psw;
					break;
				case 12:
					break;
				default: error = "bad register";
			}
	}
	journal( "RegLoc", "P=%x r=%d loc=%x %s", p, r, loc, error );
	SendLong(loc);
	return error;
}

long NormalISC(p)
register Proc *p;
{
	register long psw, *pswptr = p->traptype;

	if( ReadErr(pswptr,4) || ReadErr(p->traploc,4) )
		return 0;
	psw = *pswptr;
	if( psw&FT_MASK != FT_NORMAL )
		return 0;
	return psw & ISC_MASK;
}

typedef struct ap_fp_pc {
	long	ap;
	long	fp;
	long	pc;
} ap_fp_pc;

ap_fp_pc Cache[FRAME_CACHE];		/* [0]=main   ... [i]=callps	*/
long HostCache;

char *Stack(p,error)
register Proc *p; register char *error;
{
	ap_fp_pc build[FRAME_CACHE];	/* [0]=callps ... [i]=main	*/
	register ap_fp_pc *f, *c;
	register long fp, lo, hi;
	register long hostcache = RcvLong();

	journal( "Stack", "p=%x %s", p, error );
	if( error ){
		SendLong(0L);
		SendLong(-1L);
		return error;
	}
	f = build;
	f->fp = p->pcb.fp;
	f->ap = p->pcb.ap;
	f->pc = (long) p->pcb.pc;
	for( ;; ){
		journal("Stack","%d fp=%X ap=%X pc=%X",f-build,f->fp,f->ap,f->pc);
		fp = f->fp;
		if( f >= &build[FRAME_CACHE-1] )
			break;
		if( error = ReadErr(fp,4) ){
			PutTextf( "callstack fp=%x: %s", fp, error );
			break;
		}
		if( fp-36 < p->pcb.lower )
			break;
		++f;
		f->fp = *(long*)(fp-28);
		f->ap = *(long*)(fp-32);
		f->pc = *(long*)(fp-36);
	}
							/* f-> main */
	lo = 0;
	hi = -1;
	for( c = Cache; f >= build; --f, ++c ){	/* 0..hi exist; 0..lo-1 agree */
		++hi;
		if( lo == hi
		 && c->fp == f->fp
		 && c->ap == f->ap
		 && c->pc == f->pc )
			lo = hi+1;
		*c = *f;
	}
	if( NormalISC(p) && hi > 2 )
		hi -= 2;
	if( HostCache != hostcache )
		lo = 0;
	HostCache = hostcache;
	journal( "Stack","lo=%d hi=%d %s", lo, hi, error );
	SendUChar(lo);
	SendUChar(hi);
	for( f = Cache+lo; lo <= hi; ++lo, ++f ){
		SendLong(f->fp);
		SendLong(f->ap);
		SendLong(f->pc);
	}
	return error;
}	

char *Control(p,error)
register Proc *p; register char *error;
{
	register int state = 0;
	register char *text = 0;
	register long psw = 0, pc = 0, isc;

	if( !error ){
		state = p->state;
		text = p->text;
		isc = NormalISC(p);
		if( isc ){
			psw = *p->traptype;
			pc = (long) *p->traploc;
			if( isc == NORMAL_BPT )
				--pc;
		}
	}
	SendLong(text);
	SendLong(state);
	SendLong(pc);
	SendLong(psw);
	journal( "Control", "p=%x state=%x text=%x pc=%x psw=%x %s",
			     p,	  state,   text,   pc, 	 psw,	error );
	return error;
}

char *Run(p,error)
register Proc *p; register char *error;
{
	journal( "Run", "p=%x %s", p, error );
	if( !error ){
		p->state &= ~MUX_ZOMBIE;
		p->state |= MUX_RUN;
		p->traploc = 0;
		p->traptype = 0;
	}
	return error;
}

char *Stop(p,error)
register Proc *p; register char *error;
{
	journal( "Stop", "p=%x %s", p, error );
	if( !error )
		p->state |= MUX_ZOMBIE;
	return error;
}

char *Flash(p,error)
register Proc *p; register char *error;
{
	if( !error ) PiTexture(p);
	return error;
}

char *Step();

CoreP(pkt)
{
	register Proc *p = (Proc*) RcvLong();
	register char *error = 0;

	journal( "CoreP", "pkt=%x p=%x", pkt, p );
	if( !IsProcPtr(p) )
		error = "not a process";
	else if( !p->state&MUX_BUSY )
		error = "process not live";
	switch( pkt ){
	case CP_STACK:	error = Stack(p,error);		break;
	case CP_CONTROL:	error = Control(p,error);	break;
	case CP_FLASH:	error = Flash(p,error);		break;
	case CP_REGLOC:	error = RegLoc(p,error);	break;
	case CP_RUN:		error = Run(p,error);		break;
	case CP_STEP:	error = Step(p,error);		break;
	case CP_STOP:	error = Stop(p,error);		break;
	default:		assert(0);
	}
	SendError(error);
}
