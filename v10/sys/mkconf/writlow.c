/*
 * write system control block
 */

#include <libc.h>
#include "mkconf.h"

#define	SVSIZE	4	/* size of an scb vector */

writlow(f)
char *f;
{
	int fd;
	register Scb *sp;
	register int ssize;
	register int i;
	register Dev *dp;

	if ((fd = creat(f, 0666)) < 0) {
		perror(f);
		exit(1);
	}
	/*
	 * system control block
	 */
	fprint(fd, "\t.text\n\t.align 9\n\t.set INTSTACK,1\n");
	fprint(fd, "\t.globl\tscb\nscb:\n");
	for (ssize = 0, sp = llist; sp; ssize = sp->offset+SVSIZE, sp = sp->next) {
		for (; ssize < sp->offset; ssize += SVSIZE)
			fprint(fd, "\t.long straycatch+0x%x+INTSTACK\n", ssize);
		fprint(fd, "\t.long %s%s\n", sp->name, sp->type == SINTR ? "+INTSTACK" : "");
	}
	while ((ssize & 0x1fc) != 0) {
		fprint(fd, "\t.long straycatch+0x%x+INTSTACK\n", ssize);
		ssize += SVSIZE;
	}
	/*
	 * stray catching stuff
	 */
	fprint(fd, "\t.globl\tstraycatch\nstraycatch:\n");
	for (i = 0; i < ssize; i += SVSIZE)
		fprint(fd, "\t.align 2; bsbw stray\n");
	/*
	 * trampoline, because assembler can't cope with global bsbw
	 */
	fprint(fd, "stray:\tjmp Xstray\n");
	/*
	 * interface routines
	 */
	for (dp = dlist; dp; dp = dp->next)
		(*acode[dp->mdev->atype].putvec)(fd, dp);
	close(fd);
}

stdputvec(fd, dp, name, ix)
int fd;
Dev *dp;
char *name;
int ix;
{
	Mdev *mp;

	mp = dp->mdev;
	if (mp->flags & FRAWVEC)
		return;
	fprint(fd, "\t.align 2\n.globl _%s%dint\n%s:\n", mp->tag, ix, name);
	fprint(fd, "\tpushr\t$0x3f\n");
	if (dp->id == 0)
		fprint(fd, "\tclrl\t-(sp)\n");
	else
		fprint(fd, "\tpushl\t$%d\n", dp->id);
	fprint(fd, "\tcalls\t$1,_%s%dint\n", mp->tag, ix);
	fprint(fd, "\tpopr\t$0x3f\n\trei\n");
}
