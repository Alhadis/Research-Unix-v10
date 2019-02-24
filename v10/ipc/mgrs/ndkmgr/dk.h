/*
 * This file defines message structures and codes
 * used in talking to the UNIXP DK controller
 */

/*
 * Principal message structure.
 */
struct	dialout {
	unsigned char	type;
	unsigned char	srv;
	unsigned char	param0l;
	unsigned char	param0h;
	unsigned char	param1l;
	unsigned char	param1h;
	unsigned char	param2l;
	unsigned char	param2h;
	unsigned char	param3l;
	unsigned char	param3h;
	unsigned char	param4l;
	unsigned char	param4h;
};


/*
 * message types
 */
#define	T_SRV		'k'	/* request for server process */
#define	T_REPLY		2	/* reply from controller */
#define	T_CHG		3	/* status change on a channel */
#define	T_ALIVE		4	/* keep-alive */
#define	T_RESTART	8	/* controller warmstart */

/*
 * T_CHG codes
 */
#define	D_CLOSE		1	/* close a channel */
#define	D_ISCLOSED	2	/* channel is closed */
#define	D_CLOSEALL	3	/* close all channels */
#define D_REDIAL	6	/* redial a call */

#define	D_OK		1	/* server announce OK */
#define	D_OPEN		2	/* channel is open */
#define	D_FAIL		3	/* call failed */

/*
 * T_SRV codes
 */
#define D_SERV 1	/* announce a server */
#define D_DIAL 2	/* dialout from host */
