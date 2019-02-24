/*
 * This file defines message structures and codes
 * used in talking to the DK controller
 */

/*
 * Principal message structure.
 */
struct	dialout {
	char	type;		/* message type */
	char	srv;		/* service requested */
	short	param0;		/* parameters */
	short	param1;
	short	param2;
	short	param3;
	short	param4;
	short	param5;
};

/*
 * dialout structure as received with
 * cmcld on (includes channel)
 */
struct	listenin {
	short	l_lchan;
	char	l_type;
	char	l_srv;
	short	l_param0;
	short	l_param1;
	short	l_param2;
	short	l_param3;
	short	l_param4;
	short	l_param5;
};


/*
 * message types
 */
#define	T_SRV		1	/* request for server process */
#define	T_CHG		3	/* status change on a channel */
#define	T_LSTNR		4	/* keep-alive */
#define	T_REPLY		10	/* reply from controller */

/*
 * T_CHG codes
 */
#define	D_CLOSE		1	/* close a channel */
#define	D_ISCLOSED	2	/* channel is closed */
#define	D_OPEN		3	/* channel is open */
#define	D_FAIL		5
