/*
 * ptrace goo for reading and writing
 * cray version: io is addressed by words.  careful.
 */

#include "defs.h"
#include "space.h"
#include "ptrace.h"
#include "machine.h"

#define	min(a, b)	(((a) < (b)) ? (a) : (b))
extern int errno;

ptri(addr, space, buf, size)
ADDR addr;
int space;
register char *buf;
int size;
{
	PTWORD w;
	register char *p, *q;
	register int n;
	register int off;
	int rf;

	switch (space & SPTYPE) {
	default:
	case INSTSP:
		rf = P_RDI;
		break;

	case DATASP:
		rf = P_RDD;
		break;

	case UBLKSP:
		rf = P_RDU;
		break;
	}
	for (p = buf, n = size; n > 0; n--)
		*p++ = 0;
	off = waoff(addr);
	while (size) {
		errno = 0;
		w = ptrace(rf, pid, batowa(addr), 0);
		if (errno) {
			errflg = "process address not found";
			return (0);
		}
		p = (char *)&w + off;
		for (n = min(size, sizeof(w) - off); n > 0; size--, n--)
			*buf++ = *p++;
		addr += sizeof(w);
		off = 0;
	}
	return (1);
}

ptro(addr, space, buf, size)
ADDR addr;
int space;
register char *buf;
int size;
{
	PTWORD w;
	register char *p;
	register int n;
	register int off;
	int rf, wf;

	switch (space & SPTYPE) {
	default:
	case INSTSP:
		rf = P_RDI;
		wf = P_WRI;
		break;

	case DATASP:
		rf = P_RDD;
		wf = P_WRD;
		break;

	case UBLKSP:
		rf = P_RDU;
		wf = P_WRU;
		break;
	}
	off = waoff(addr);
	while (size) {
		errno = 0;
		w = ptrace(rf, pid, batowa(addr), 0);
		if (errno) {
			errflg = "process address not found";
			return (0);
		}
		p = (char *)&w + off;
		for (n = min(size, sizeof(w) - off); n > 0; size--, n--)
			*p++ = *buf++;
		errno = 0;
		ptrace(wf, pid, batowa(addr), w);
		if (errno) {
			errflg = "process address not found";
			return (0);
		}
		addr += sizeof(w);
	}
	return (1);
}
