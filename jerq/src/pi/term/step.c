#include "term.h"
#include "../protocol.h"

long NormalISC();

Proc *StepP;
char *StepErr;
long StepTimeLimit;
#define TIMEOUT		(60*15)
#define TIMEOUT_MSG	"timeout (15 secs)"

void TimeOut()
{
	if( !StepErr && realtime() > StepTimeLimit )
		StepErr = TIMEOUT_MSG;
}

void WaitZombie()
{
	register Proc *p = StepP;

	journal( "WaitZombie", "p=%x %s", p, StepErr );
	for( ; !StepErr; TimeOut() ){
		wait(CPU);
		if( p->state & MUX_ZOMBIE )
			return;
		
	}
}

#define PSW_TRACE 0x20000
StepInstr()
{
	register Proc *p = StepP;

	journal( "StepInstr", "%x %s", p, StepErr );
	if( StepErr ) return;
	*p->traptype |= PSW_TRACE;
	Run(p,0);
	WaitZombie();
	if( StepErr ) return;
	if( NormalISC(p) == NORMAL_TRACE )
		*p->traptype &= ~PSW_TRACE;
	else
		StepErr = "psw error after instr step";
}

void BeforeStep()
{
	register Proc *p = StepP;

	journal( "BeforeStep", "p=%x %s", p, StepErr );
	if( StepErr ) return;
	switch( NormalISC(p) ){
	case NORMAL_BPT:
		*p->traploc -= 1;
		*p->traptype ^= NORMAL_BPT^NORMAL_TRACE;	/* thru */
	case NORMAL_TRACE:
		break;
	default:
		StepErr = "psw error before instr step";
	}
}

#define BPT_INST 0x2E
void StepOver()
{
	register Proc *p = StepP;
	register long sp0 = p->pcb.sp;
	register char save, *aftercall;

	journal( "StepOver", "p=%x %s", p, StepErr );
	StepInstr();
	if( StepErr ) return;
	aftercall = p->traploc[-2];	/* ret addr */
	StepErr = WriteErr(aftercall,1);
	for( ; !StepErr; TimeOut() ){
		save = *aftercall;
		*aftercall = BPT_INST;
		Run(p,0);
		WaitZombie();
		*aftercall = save;
		BeforeStep();
		if( p->pcb.sp <= sp0 )
			break;
		StepInstr();
	}
}

#define CALL_INST 0x2c
char *Step(p,error)
register Proc *p; register char *error;
{
	register long lo = RcvLong();
	register long hi = RcvLong(), state = 0, pc = 0, psw = 0;

	journal( "Step", "p=%x lo=%x hi=%x %s", p, lo, hi, error );
	StepP = p;
	StepTimeLimit = realtime()+TIMEOUT;
	StepErr = error;
	BeforeStep();
	for( ; !StepErr; TimeOut() ){
		if( hi && **p->traploc == CALL_INST )
			StepOver();
		else
			StepInstr();
		if( StepErr ) break;
		state = p->state;
		pc = (long) *p->traploc;
		psw = *p->traptype;
		journal( "Step", "pc=%x", pc );
		if( !hi || (pc<lo || pc>=hi) )
			break;
	}
	SendLong(state);
	SendLong(pc);
	SendLong(psw);
	return StepErr;
}
