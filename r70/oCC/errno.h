/*
 * Error codes
 */

#define	EPERM	1
#define	ENOENT	2
#define	ESRCH	3
#define	EINTR	4
#define	EIO	5
#define	ENXIO	6
#define	E2BIG	7
#define	ENOEXEC	8
#define	EBADF	9
#define	ECHILD	10
#define	EAGAIN	11
#define	ENOMEM	12
#define	EACCES	13
#define	EFAULT	14
#define	ENOTBLK	15
#define	EBUSY	16
#define	EEXIST	17
#define	EXDEV	18
#define	ENODEV	19
#define	ENOTDIR	20
#define	EISDIR	21
#define	EINVAL	22
#define	ENFILE	23
#define	EMFILE	24
#define	ENOTTY	25
#define	ETXTBSY	26
#define	EFBIG	27
#define	ENOSPC	28
#define	ESPIPE	29
#define	EROFS	30
#define	EMLINK	31
#define	EPIPE	32

/* math software */
#define	EDOM	33
#define	ERANGE	34

/* complex */
#define C_COSH_RE	40
#define C_COSH_IM	41
#define C_EXP_RE_POS	42
#define C_EXP_RE_NEG	43
#define C_EXP_IM	44
#define C_LOG_0		45
#define C_SINH_RE	46
#define C_SINH_IM	47

extern int errno;
extern int sys_nerr;
extern char *sys_errlist[];
