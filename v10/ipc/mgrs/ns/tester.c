#include <sys/types.h>
#include <signal.h>
#include <sysent.h>
#include <libc.h>
#include <fio.h>
#include <ipc.h>
#include <string.h>
#include <errno.h>
#include "dbtypes.h"

/*
 *  global
 */
Ordered *o;
Tuple *dblist;
char *errstr;

main()
{
	int i;
	Tuple *t;
	char buf[512];
	char outbuf[sizeof(Fbuffer)];

	Finit(2, outbuf);
	for(;;){
		o = new Ordered(12);
		for(i=0; i<1650; i++){
			strcpy(buf, "att,org att,org att,org att,org att,org att,org");
			t = new Tuple(buf, o, &dblist);
		}
		fprint(2, "after creating %x\n", sbrk(0));
		delete o;
		freetuplelist(&dblist);
		fprint(2, "after deleting %x\n", sbrk(0));
	}
}
