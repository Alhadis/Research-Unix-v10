#include <errno.h>

/* imported */
extern int errno;
extern char *errstr;

char *msgs[] = {
	"DK controller system error",
	"destination busy",
	"remote node not answering",
	"destination not answering",
	"unassigned destination",
	"DK system overload",
	"server already exists",
	"call rejected by destination",
};

int errs[] = {
	EIO,
	EBUSY,
	EIO,
	EIO,
	ENOENT,
	EBUSY,
	EEXIST,
	EACCES,
};

struct uerrs {
	int v9;
	int dk;
};

struct uerrs uerrs[] = {
	{ EIO,	  3 },
	{ EBUSY,  1 },
	{ ENOENT, 3 },
	{ EEXIST, 3 },
	{ EACCES, 7 },
	{ 0,	  3 },
};

/*
 *  Convert a Unix error code to a dk error code
 */
int
utodkerr(uerr)
{
	register struct uerrs *up;

	for(up=uerrs; up->v9; up++)
		if(up->v9 == uerr)
			return up->dk;
	return up->dk;
}

/*
 *  Convert a dk error code to a Unix errstr and errno
 */
dktouerr(dkerr)
{
	dkerr &= 0x7f;
	if (dkerr<=0 || dkerr>=sizeof(errs)/sizeof(int)) {
		errno = EGREG;
		errstr = "unknown error";
		return;
	}
	errno = errs[dkerr];
	errstr = msgs[dkerr];
}
