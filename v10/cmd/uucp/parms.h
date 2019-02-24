/*	%W%	*/
#ifndef lint
static	char	h_parms[] = "%W%";
#endif

/* go through this carefully, configuring for your site */

/* One of the following four lines should not be commented out.
 * The other three should be unless you are running a unique hybrid.
 */

/* #define	ATTSV	/* System III or System V */
/* #define	V7	/* Version 7 systems (32V, Berkeley 4BSD, 4.1BSD) */
/* #define	BSD4_2	/* Berkeley 4.2BSD */
#define	V8	/* Research some edition */


/* Owner of setud files running on behalf of uucp.  Needed in case
 * root runs uucp and euid is not honored by kernel.
 * GID is needed for some chown() calls.
 * Also used if guinfo() cannot find the current users ID in the
 * password file.
 */
#define UUCPUID		48	/* */
#define UUCPGID		1	/* */

/* define ATTSVKILL if your system has a kill(2) that accepts kill(0, pid)
 * as a test for killability.  If ATTSV is defined this will automatically
 * be defined anyway.
 */
#define ATTSVKILL	/* */

/*
 * the next two lines control high resolution sleeps, called naps.
 *
 * most UNIX versions have no nap() system call; they want NONAP defined,
 * in which case one is provided in the code.  this includes all standard
 * versions of UNIX.
 *
 * some sites use a fast timer that reads a number of clock ticks and naps
 * for that interval; they want NONAP defined, and FASTTIMER defined as
 * the name of the device, e.g., /dev/ft.
 *
 * repeating, NONAP should be disabled *only* if your standard library has a
 * function called nap.
 */


/* #define NONAP	/* nominal case -- no nap() in the standard library */
/* #define FASTTIMER "/dev/ft"   /* identify the device used for naps */

/*
 * we use ustat to decide whether there's enough space to receive a
 * file.  if you're not ATTSV, you can use a setgid program to read the
 * number of free blocks and free inodes directly off the disk.  if you
 * choose this course, do not define NOUSTAT; rather, define V7USTAT to
 * be the name of that program.  be sure it accepts 2 args, major and minor
 * device numbers, and returns two numbers, blocks and inodes, in
 * "%d %d" format, or you'll never receive another file.
 */
/* #define V7USTAT  "/usr/local/lib/ustat" /* */
#define NOUSTAT  /* define NOUSTAT if you don't have ustat */

/* define GRPCHK if you want to restrict the ability to read */
/* Systems file stuff by way of the DEBUG flags based on a group id range */
/* ex: if (GRPCHK(getgid()) no_secrets(); */
#define GRPMIN	2	/* */
#define GRPMAX	4	/* */
#define GRPCHK(gid)	( gid >= GRPMIN && gid <= GRPMAX ? 1 : 0 )	/* */
/* #define GRPCHK(gid)	1	/* Systems info is not protected from DEBUG */

/*
 * protocols
 * some are specific to particular networks
 */

#define	PROTO_D		/* datakit block mode -- assumes datakit */
#define	PROTO_E		/* general block mode -- assumes error-free line */
#define	PROTO_G		/* ordinary phone line */
/* #define	PROTO_X		/* X25 */
/*
 * types of networks
 * ACUs are always assumed, stupid though that be
 */

/* #define DATAKIT	/* define DATAKIT for plain (tdkdial) datakit */
#define	CSERV		/* define CSERV for connection server */
/* #define UNET		/* define UNET for 3com ethernet */
/* #define TCP		/* define TCP for TCP/IP */
/* #define SYTEK	/* define SYTEK for sytek network */

#define MAXCALLTRIES	2	/* maximum call attempts per Systems file line */

/* #define DIAL801	/* 801/212-103 auto dialers */
/* #define X25 /* define X25 if you want to use the xio protocol */

/* define DUMB_DN if your dn driver (801 acu) is too dumb to handle '=' */
/* #define DUMB_DN /*  */

/* define DEFAULT_BAUDRATE to be the baud rate you want to use when both */
/* Systems file and Devices file allow Any */
#define DEFAULT_BAUDRATE "9600"	/* */

/* permission modes for the device uucico is calling the remote machine on */
#define DEVICEMODE 0600

/* NO_MODEM_CTRL - define this if you have very old hardware
 * that does not know how to correctly handle modem control
 * Some old pdp/11 hardware such as dk, dl
 * If you define this, and have DH devices for direct lines,
 * the ports will often hang and be unusable.
*/
/*#define NO_MODEM_CTRL		/* */


/* UUSTAT_TBL - this is the maximum number of machines that
 * status may be needed at any instant.
 * If you are not concerned with memory for a seldom used program,
 * make it very large.
 * This number is also used in uusched for its machine table -- it has
 * the same properties as the one in uustat.
 */

/* #define UUSTAT_TBL 1000		/* big machine with lots of traffic */
#define UUSTAT_TBL 200

/* define UNAME if uname() should be used to get uucpname
 * This will be defined automatically if ATTSV is defined
 */
/* #define UNAME /*  */

/* initial wait time after failure before retry */
#define RETRYTIME 300		/* 5 minutes */
/* MAXRETRYTIME is for exponential backoff  limit.
 * NOTE - this should not be 24 hours so that
 * retry is not always at the same time each day
 */
#define MAXRETRYTIME 82800	/* 23 hours */
#define ASSERT_RETRYTIME 86400	/* retry time for ASSERT errors */

/*  This is the path that will be used for uuxqt command executions */
#define PATH	"PATH=/bin:/usr/bin " /* */

/*  This is the set of default commands that can be executed */
/*  if none is given for the system name in PERMISSIONS file */
/*  It is a colon separated list as in PERMISSIONS file */
#define DEFAULTCMDS	"rmail"	/* standard default command list */

/* define HZ to be the number of clock ticks per second */
#define HZ 60 /* not needed for ATTSV or above */

/*
 * put in local uucp name of this machine if there is no "/etc/whoami"
 * and no uname() (this is a last resort)
 */
#define MYNAME		"greg"	/* */

/*
 * define READDIR if there are useful readdir/opendir routines in the library
 */

#define	READDIR	1		/* */

/* define NOSTRANGERS if you want to reject calls from systems which
 * are not in your Systems file.   If defined, NOSTRANGERS should be the name
 * of the program to execute when such a system dials in.  The argument
 * to said program will be the name of said system.  Typically this is a shell
 * procedure that sends mail to the uucp administrator informing them of an
 * attempt to communicate by an unknown system.
 * NOTE - if this is defined, it can be overridden by the administrator
 * by making the command non-executable.  (It can be turned on and off
 * by changing the mode of the command.)
 */
#define NOSTRANGERS	"/usr/lib/uucp/remote.unknown"	/* */

/* define LMTUUXQT to be the name of a file which contains the number
 * (in ascii) of simultaneous uuxqt's which you will permit.  If it is
 * not defined, there may be "many" uuxqt's running. 5 is reasonable number.
 */
#define LMTUUXQT	"/usr/lib/uucp/Maxuuxqts"	/* */

/* define LMTUUSCHED to be the name of a file which contains the number
 * (in ascii) of simultaneous uusched's which you will permit.  If it is
 * not defined, there may be "many"  uusched's running. 2 is reasonable number.
 */
#define LMTUUSCHED	"/usr/lib/uucp/Maxuuscheds"	/* */

/* define USRSPOOLLOCKS if you like your lock files in /usr/spool/locks
 * be sure other programs such as 'cu' and 'ct' know about this
 */
/* #define USRSPOOLLOCKS  /* define to use /usr/spool/locks for LCK files */

/* define ASCIILOCKS if you like your lock files to contain ascii pids.
 * rather than binary representations.  Rmemeber cu and ct and whatever
 * else might lock devices need to understand this
 */
#define ASCIILOCKS  /* define for ascii lck pids */

/* define PKSPEEDUP if you want to try the recommended speedup in pkcget.
 * this entails sleeping between reads at low baud rates.
 */
#define PKSPEEDUP	/* */

/*
 * no vfork buggery, please
 * see cpmv.c and gwd.c
 */

#define	uidstat	stat
#define	uidxcp	xcp
