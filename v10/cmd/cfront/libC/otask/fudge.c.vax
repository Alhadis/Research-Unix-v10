#include "task.h"

// VAX frame fudger

// STACK GROWS DOWN
/*
#define FP(p)		((int*)(&p+1))
#define OLD_AP(fp)	(*(fp+2))
#define OLD_FP(fp)	(*(fp+3))
#define OLD_PC(fp)	(*(fp+4))
*/

/*
 * fix a the frame so that when we return from it, all the registers will
 * be restored.
 * The idea is that task_fp points to the task::task() stack frame which
 * contains all the saved registers.  t_framep points to the My_task::Mytask()
 * stack frame that contains the return address (and perhaps some registers).
 * fill the frame pointed to by t_framep with the registers from task_fp.
 * do this by copying the registers up from t_framep to task_fp, then
 * copying the whole frame down and adjusting t_framep.
 * see the VAX architecture reference manual for frame layout
 * note we always use calls (not callg) and spa is always 0
 */
void
task::fudge_return(register int* task_fp, int offset, task* next)
{
	register int*	this_fp = t_framep + offset;
	task_fp += offset;		// in case this is SHARED
	// get masks of saved registers in the frames
	unsigned short	this_mask = *((short*)this_fp + 3);
	unsigned short	task_mask = *((short*)task_fp + 3);
	// copy up the always fields
	task_fp[0] = this_fp[0];	// condition handler (always zero?)
	((short*)task_fp)[2] = ((short*)this_fp)[2];	// psw but not mask
	task_fp[2] = this_fp[2];	// saved argument pointer
	task_fp[3] = this_fp[3];	// saved frame pointer
	task_fp[4] = this_fp[4];	// saved program counter
	// now copy up optional registers
	register int*	this_rp = this_fp + 5;
	register int*	task_rp = task_fp + 5;
	for (register mask = 1; mask != 0x1000; mask <<= 1) {
		if (mask & task_mask)
			if (mask & this_mask)
				*task_rp++ = *this_rp++;
			else task_rp++;
		else if (mask & this_mask) // task frame is supposed to save all
			task_error(E_FUDGE);
	}
	// next copy the whole frame down
	while (task_rp > task_fp)
		*--this_rp = *--task_rp;
	// finally fix t_framep to point to where the frame wound up
	t_framep = this_rp - offset;
	if (next) next->restore();
}

