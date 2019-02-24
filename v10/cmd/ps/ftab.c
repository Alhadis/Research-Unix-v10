/*
 * data-fetching function tables
 * (0) read through /proc and /dev/kmem
 * (1) read /dev/mem (or crash dump)
 */

#include "ps.h"

List *getprlista(); char *getuareaa(); int getstacka(), getproca(), kreada();
List *getprlistk(); char *getuareak(); int getstackk(), getprock(), kreadk();
int inita(), initk();

struct ftab ftab[] = {
	{ getproca, getprlista, getuareaa, getstacka, kreada, inita },
	{ getprock, getprlistk, getuareak, getstackk, kreadk, initk },
};
