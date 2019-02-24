#include <jerq.h>
#include <jerqio.h>

struct	_iobuf	_iob[_NFILE] ={
	{ 0, 0, 0, _IOREAD, 0},
	{ BUFSIZ, 0, 0, _IOWRT+_IOTTY, 1},
	{ BUFSIZ, 0, 0, _IOWRT+_IOTTY, 2},
};
/*
 * Ptr to end of buffers
 */
struct	_iobuf	*_lastbuf ={ &_iob[_NFILE] };
