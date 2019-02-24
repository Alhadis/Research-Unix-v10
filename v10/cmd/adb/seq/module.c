/*
 * code to hide the stupid module structure
 * trivialized version for new sequent system
 */

#include "defs.h"
#include <a.out.h>

static struct modtbl mod;

/*
 * init stuff
 * store the single module entry from the a.out
 */

modinit(h)
register struct exec *h;
{

	mod = h->a_modtbl;
}

/*
 * link index to addr, data version
 * link entry is just an address
 */

ADDR
mtodata(d, sp)
int d;
int sp;
{
	ADDR a;

	a = (ADDR)(mod.m_linkbase + (d * sizeof(long)));
	return ((ADDR)ltow(lget(a, sp)));

}

/*
 * link index to addr, text version
 * link entry is offset,,module
 */

ADDR
mtotext(d, sp)
int d;
int sp;
{
	register long x;

	x = ltow(lget((ADDR)(mod.m_linkbase + (d * sizeof(long))), sp));
	return (mod.m_programbase + ((x >> 16) & 0xffff));
}
