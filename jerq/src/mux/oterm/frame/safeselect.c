#include <jerq.h>
#include <layer.h>
#include <queue.h>
#include "jerqproc.h"
#include "frame.h"

select(f, pt)
	register Frame *f;
	Point pt;
{
	register fix, var, oldvar;
	if(f->s1 != f->s2)
		selectf(f, F_XOR);
	fix=charofpt(f, pt);	/* fixed point */
	oldvar=fix;		/* moving point tracks mouse */
	var=fix;
	while (button1()) {
		if(var != oldvar){
			f->s1=oldvar; f->s2=var;
			order(f);
			selectf(f, F_XOR);
			oldvar=var;
		}
		var=charofpt(f, mouse.xy);
	}
	f->s1=fix; f->s2=oldvar;
	order(f);
}
order(f)
	register Frame *f;
{
	register a;
	if(f->s1 > f->s2){
		a=f->s1;
		f->s1=f->s2;
		f->s2=a;
	}
}
