/*
 *	return codes from dkdial privileged subprocess
 */

#define NORM	0	/* OK */
#define ILLEG	1	/* arg count or other syntax error */
#define NOUID	2	/* can't find /etc/passwd entry */
#define NOREP	3	/* no reply from remote */
#define	NODIAL	4	/* can't find dkdial */
#define NOOCHAN	5	/* can't find outgoing channel */
#define	NOMGR	6	/* can't open chan 1 to mgr */
#define	NODIALT	7	/* no dialtone */
#define ERRBASE	8	/* 8+n is message type n from mgr */
