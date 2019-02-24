#include <stdio.h>
#include <errno.h>
#include <nlist.h>
#include "defs.h"
#include "load.h"

/* globals */
struct nlist nl[] ={
    {"_intrtime",0},
    {"_cp_time",0},
    {"_avenrun",0},
    { 0,0 },
};
char *sys = "/unix";
char *core = "/dev/kmem";
int mem;

/* imported */
extern int errno;
extern long lseek();
extern long time();

/* initialize */
initload()
{
    nlist(sys, nl);

    mem = open(core, 0);
    if (mem<0) {
	printf("can't open %s\n", core);
	printf("errno = %d\n", errno);
	exit(1);
    }

}

/* generate the current information */
genload()
{
    char buf[16];
    double fabs();

    lseek(mem, (long)nl[1].n_value, 0);
    read(mem, (char *)load.l_cp, sizeof(load.l_cp));
    lseek(mem, (long)nl[2].n_value, 0);
    read(mem, (char *)&(load.l_runq), sizeof(load.l_runq));
    load.l_time = time ((long *)0);
}
