#define HEADSIZE 160
#define UJN 0			/* unique job name - 14 bytes */
#define ULOG 15			/* user login - 8 bytes */
#define MHACCT 24		/* mh account number - 6 bytes */
#define BIN 31			/* bin or mail info - 34 bytes */
#define SGRADE 66		/* service grade - 3 bytes - currently unused */
#define TODAPS 70		/* TOD of apsend - 10 bytes */
#define BCBPP 81		/* byte count before post-processing - 10 bytes */
#define TODAPP 92		/* TOD of post-processing - 10 bytes */
#define BCAPP 103		/* byte count after post-processing - 10 bytes */
#define CPUTIM 114		/* floating point seconds for daps - user + sys time - 6 bytes */
#define PAPER 121		/* floating-point page count from daps - 6 bytes */
#define TODFIN 128		/* TOD of apsrun - 10 bytes */
#define VERSION 139		/* troff font table version (drexler) - 6 bytes*/
#define AVERSION 153		/* apsend version (vbt) - 6 bytes */
#define HOST "none"		/* for PDP-11/34 - HOST is "mhuxm" */
				/* for Janus - HOST is "none" */
