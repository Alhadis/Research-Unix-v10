/*
 *	return codes from tdkdial subroutine
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
#define	TDKERR	010	/* controller went wrong somewhere */
#define	BUSY	011	/* destination is there but busy now */
#define	NONET	012	/* network resource is currently unavailable */
#define	NODEST	013	/* destination is currently unavailable */
#define	INTERT	014	/* illegal destination name */
#define	NETBUSY	015	/* network overload */
#define	EXISTS	016	/* server already exists */
#define	NOPERM	017	/* permission denied */
