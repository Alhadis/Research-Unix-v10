/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * generate libx header files
 * usually controlled by mkhdr
 *
 * genhdr hdr
 *
 * NOTE: two's complement binary integral representation assumed
 */

#if !lint
static char id[] = "\n@(#)genhdr (ulysses!gsf) 12/11/90\0\n";
#endif

#include "FEATURE/dir"
#include "FEATURE/getdents"

#include <lcllimits.h>
#include <lclparam.h>
#include <lcldirent.h>
#include <lclstdio.h>

#if !defined(_lcl_dirent) && defined(_sys_dir)
#include <sys/dir.h>
#endif

#ifndef SIG_IGN
#include <signal.h>
#endif

union _u_
{
	long		u1;
	char*		u2;
	double		u3;
	char		u4[1024];
};

struct _s_
{
	char		s1;
	union _u_	s2;
};

struct _m_
{
	char*		text;
	char*		name;
	int		value;
};

#define elements(x)	(sizeof(x)/sizeof(x[0]))
#define round(x,y)	(((x)+((y)-1))&~((y)-1))

static struct _m_ map[] =
{
#ifdef SIGABRT
"Abort",			"ABRT",		SIGABRT,
#endif
#ifdef SIGALRM
"Alarm call",			"ALRM",		SIGALRM,
#endif
#ifdef SIGAPOLLO
"Apollo",			"APOLLO",	SIGAPOLLO,
#endif
#ifdef SIGBUS
"Bus error",			"BUS",		SIGBUS,
#endif
#ifdef SIGCHLD
"Child status change",		"CHLD",		SIGCHLD,
#endif
#ifdef SIGCLD
"Death of child", 		"CLD",		SIGCLD,
#endif
#ifdef SIGCONT
"Stopped process continued",	"CONT",		SIGCONT,
#endif
#ifdef SIGDEBUG
"Debug trap",			"DEBUG",	SIGDEBUG,
#endif
#ifdef SIGEMT
"EMT trap",			"EMT",		SIGEMT,
#endif
#ifdef SIGERR
"ERR trap",			"ERR",		SIGERR,
#endif
#ifdef SIGEXIT
"Exit",				"EXIT",		SIGEXIT,
#endif
#ifdef SIGFPE
"Floating exception",		"FPE",		SIGFPE,
#endif
#ifdef SIGHUP
"Hangup",			"HUP",		SIGHUP,
#endif
#ifdef SIGILL
"Illegal instruction",		"ILL",		SIGILL,
#endif
#ifdef SIGINT
"Interrupt",			"INT",		SIGINT,
#endif
#ifdef SIGIO
"IO possible",			"IO",		SIGIO,
#endif
#ifdef SIGIOT
"IOT trap",			"IOT",		SIGIOT,
#endif
#ifdef SIGKILL
"Killed",			"KILL",		SIGKILL,
#endif
#ifdef SIGLAB
"LAB trap",			"LAB",		SIGLAB,
#endif
#ifdef SIGKILL
"Killed",			"KILL",		SIGKILL,
#endif
#ifdef SIGLOST
"Resources lost",		"LOST",		SIGLOST,
#endif
#ifdef SIGPHONE
"Phone status change",		"PHONE",	SIGPHONE,
#endif
#ifdef SIGPIPE
"Broken Pipe",			"PIPE",		SIGPIPE,
#endif
#ifdef SIGPOLL
"Poll event",			"POLL",		SIGPOLL,
#endif
#ifdef SIGPROF
"Profiling time alarm",		"PROF",		SIGPROF,
#endif
#ifdef SIGPWR
"Power fail",			"PWR",		SIGPWR,
#endif
#ifdef SIGQUIT
"Quit",				"QUIT",		SIGQUIT,
#endif
#ifdef SIGSEGV
"Memory fault",			"SEGV",		SIGSEGV,
#endif
#ifdef SIGSSTOP
"Sendable stop",		"SSTOP",	SIGSSTOP,
#endif
#ifdef gould
"Stack overflow",		"STKOV",	28,
#endif
#ifdef SIGSTOP
"Stopped (signal)",		"STOP",		SIGSTOP,
#endif
#ifdef SIGSYS
"Bad system call", 		"SYS",		SIGSYS,
#endif
#ifdef SIGTERM
"Terminated",			"TERM",		SIGTERM,
#endif
#ifdef SIGTINT
"Interrupt (terminal)",		"TINT",		SIGTINT,
#endif
#ifdef SIGTRAP
"Trace/BPT trap",		"TRAP",		SIGTRAP,
#endif
#ifdef SIGTSTP
"Stopped",			"TSTP",		SIGTSTP,
#endif
#ifdef SIGTTIN
"Stopped (tty input)",		"TTIN",		SIGTTIN,
#endif
#ifdef SIGTTOU
"Stopped (tty output)",		"TTOU",		SIGTTOU,
#endif
#ifdef SIGURG
"Urgent IO",			"URG",		SIGURG,
#endif
#ifdef SIGUSR1
"User signal 1",		"USR1",		SIGUSR1,
#endif
#ifdef SIGUSR2
"User signal 2",		"USR2",		SIGUSR2,
#endif
#ifdef SIGVTALRM
"Virtual time alarm",		"VTALRM",	SIGVTALRM,
#endif
#ifdef SIGWINCH
"Window change", 		"WINCH",	SIGWINCH,
#endif
#ifdef SIGWIND
"Window change",		"WIND",		SIGWIND,
#endif
#ifdef SIGWINDOW
"Window change",		"WINDOW",	SIGWINDOW,
#endif
#ifdef SIGXCPU
"CPU time limit",		"XCPU",		SIGXCPU,
#endif
#ifdef SIGXFSZ
"File size limit",		"XFSZ",		SIGXFSZ,
#endif
0
};

static int	index[64];

main(argc, argv)
int	argc;
char**	argv;
{
	register int	i;
	register int	j;
	register int	k;

	int		align0;
	int		align1;
	int		align2;
	unsigned long	bit1;
	unsigned long	bit2;
	unsigned long	bits0;
	unsigned long	bits1;
	unsigned long	bits2;
	union _u_	u;
	union _u_	v;

	char		c;
	unsigned char	uc;
	unsigned short	us;
	unsigned int	ui;
	unsigned long	ul;
	unsigned long	val;

	char*		hdr;

	if (argc != 2 || !(hdr = *++argv)) hdr = "UNKNOWN";
	if (!strcmp(hdr, "align"))
	{
		u.u2 = u.u4;
		v.u2 = u.u2 + 1;
		bit1 = u.u1 ^ v.u1;
		v.u2 = u.u2 + 2;
		bit2 = u.u1 ^ v.u1;
		align0 = sizeof(struct _s_) - sizeof(union _u_);
		bits0 = 0;
		k = 0;
		for (j = 0; j < align0; j++)
		{
			u.u2 = u.u4 + j;
			bits1 = 0;
			for (i = 0; i < align0; i++)
			{
				v.u2 = u.u2 + i;
				bits1 |= u.u1 ^ v.u1;
			}
			if (!bits0 || bits1 < bits0)
			{
				bits0 = bits1;
				k = j;
			}
		}
		align1 = round(align0, 2);
		u.u2 = u.u4 + k;
		for (bits1 = bits0; i < align1; i++)
		{
			v.u2 = u.u2 + i;
			bits1 |= u.u1 ^ v.u1;
		}
		align2 = round(align0, 4);
		for (bits2 = bits1; i < align2; i++)
		{
			v.u2 = u.u2 + i;
			bits2 |= u.u1 ^ v.u1;
		}
		printf("typedef unsigned %s INTEGRAL;\n", sizeof(char*) >= sizeof(long) ? "long" : sizeof(char*) >= sizeof(int) ? "int" : "short");
		printf("\n");
		printf("#define CHUNK		%d\n", sizeof(char*) >= 4 ? 8192 : 1024);
		printf("#define INTEGRAL	%s\n", sizeof(char*) >= sizeof(long) ? "long" : sizeof(char*) >= sizeof(int) ? "int" : "short");
		printf("#define INTEGER(x)	((INTEGRAL)(x))\n");
		printf("#define POINTER(x)	((char*)(x))\n");
		if (bits2 == (align2 - 1)) printf("#define ROUND(x,y)	POINTER(INTEGER((x)+(y)-1)&~((y)-1))\n");
		else printf("#define ROUND(x,y)	POINTER(INTEGER(ALIGN(x)+(((y)+%d)/%d)-1)&~((((y)+%d)/%d)-1))\n", align0, align0, align0, align0);
		printf("\n");
		if (align0 == align2)
		{
			printf("#define BOUND		BOUND2\n");
			printf("#define ALIGN(x)	ALIGN2(x)\n");
			printf("#define TRUNC(x)	TRUNC2(x)\n");
		}
		else if (align0 == align1)
		{
			printf("#define BOUND		BOUND1\n");
			printf("#define ALIGN(x)	ALIGN1(x)\n");
			printf("#define TRUNC(x)	TRUNC1(x)\n");
		}
		else
		{
			printf("#define BOUND		1\n");
			printf("#define ALIGN(x)	POINTER(x)\n");
			printf("#define TRUNC(x)	POINTER(x)\n");
		}
		printf("\n");
		printf("#define BIT1		0x%lx\n", bit1);
		if (align1 == align2)
		{
			printf("#define BOUND1		BOUND2\n");
			printf("#define ALIGN1(x)	ALIGN2(x)\n");
			printf("#define TRUNC1(x)	TRUNC2(x)\n");
		}
		else
		{
			printf("#define BOUND1		%d\n", align1);
			printf("#define ALIGN1(x)	TRUNC1((x)+%d)\n", align1 - 1);
			printf("#define TRUNC1(x)	POINTER(INTEGER((x)+%d)&0x%lx)\n", align1 - 1, ~(bits0|bits1));
		}
		printf("#define CLRBIT1(x)	POINTER(INTEGER(x)&0x%lx)\n", ~bit1);
		printf("#define SETBIT1(x)	POINTER(INTEGER(x)|0x%lx)\n", bit1);
		printf("#define TSTBIT1(x)	POINTER(INTEGER(x)&0x%lx)\n", bit1);
		printf("\n");
		printf("#define BIT2		0x%lx\n", bit2);
		printf("#define BOUND2		%d\n", align2);
		printf("#define ALIGN2(x)	TRUNC2((x)+%d)\n", align2 - 1);
		printf("#define TRUNC2(x)	POINTER(INTEGER(x)&0x%lx)\n", ~(bits0|bits1|bits2));
		printf("#define CLRBIT2(x)	POINTER(INTEGER(x)&0x%lx)\n", ~bit2);
		printf("#define SETBIT2(x)	POINTER(INTEGER(x)|0x%lx)\n", bit2);
		printf("#define TSTBIT2(x)	POINTER(INTEGER(x)&0x%lx)\n", bit2);
		printf("\n");
	}
	else if (!strcmp(hdr, "dirent"))
	{
#ifdef _lib_getdents
		printf("#define _dir_getdents	1	/* getdents() in library */\n");
#endif
#ifdef rewinddir
		printf("#define _dir_rewinddir	1	/* rewinddir locally defined */\n");
#endif
#ifdef _sys_dir
		printf("#define _dir_sys	1	/* #include <sys/dir.h> ok */\n");
#endif
		printf("\n");
	}
	else if (!strcmp(hdr, "fsinfo"))
	{
#ifdef BUFFERSIZE
		val = BUFFERSIZE;
#else
#ifdef MAXBSIZE
		val = MAXBSIZE;
#else
#ifdef SBUFSIZE
		val = SBUFSIZE;
#else
#ifdef BUFSIZ
		val = BUFSIZ;
#else
		if (sizeof(char*) > 4) val = 8192;
		else if (sizeof(char*) < 4) val = 512;
		else val = 4096;
#endif
#endif
#endif
#endif
		printf("#define BUFFERSIZE	%lu\n", val);
		printf("\n");
	}
	else if (!strcmp(hdr, "limits"))
	{
		/*
		 * <limits.h> with *constant* valued macros
		 */

#ifdef CHAR_BIT
		val = CHAR_BIT;
		printf("#undef	CHAR_BIT\n");
#else
		uc = 0;
		uc = ~uc;
		val = 1;
		while (uc >>= 1) val++;
#endif
		printf("#define CHAR_BIT	%lu\n", val);

		c = 0;
		c = ~c;
		uc = 0;
		uc = ~uc;
		us = 0;
		us = ~us;
		ui = 0;
		ui = ~ui;
		ul = 0;
		ul = ~ul;

#ifdef UCHAR_MAX
		val = UCHAR_MAX;
		printf("#undef	UCHAR_MAX\n");
#else
		val = uc;
#endif
		printf("#if __STDC__\n");
		printf("#define UCHAR_MAX	%luU\n", val);
		printf("#else\n");
		printf("#define UCHAR_MAX	%lu\n", val);
		printf("#endif\n");

#ifdef SCHAR_MIN
		val = -(SCHAR_MIN);
		printf("#undef	SCHAR_MIN\n");
#else
		val = (unsigned char)(uc >> 1) + 1;
#endif
		printf("#define SCHAR_MIN	-%lu\n", val);

#ifdef SCHAR_MAX
		val = SCHAR_MAX;
		printf("#undef	SCHAR_MAX\n");
#else
		val = (unsigned char)(uc >> 1);
#endif
		printf("#define SCHAR_MAX	%lu\n", val);

		if (c < 0)
		{
#ifdef CHAR_MIN
			printf("#undef	CHAR_MIN\n");
#endif
			printf("#define CHAR_MIN	SCHAR_MIN\n");

#ifdef CHAR_MAX
			printf("#undef	CHAR_MAX\n");
#endif
			printf("#define CHAR_MAX	SCHAR_MAX\n");
		}
		else
		{
#ifdef CHAR_MIN
			printf("#undef	CHAR_MIN\n");
#endif
			printf("#define CHAR_MIN	0\n");

#ifdef CHAR_MAX
			printf("#undef	CHAR_MAX\n");
#endif
			printf("#define CHAR_MAX	UCHAR_MAX\n");
		}

#ifdef USHRT_MAX
		val = USHRT_MAX;
		printf("#undef	USHRT_MAX\n");
#else
		val = us;
#endif
		printf("#if __STDC__\n");
		printf("#define USHRT_MAX	%luU\n", val);
		printf("#else\n");
		printf("#define USHRT_MAX	%lu\n", val);
		printf("#endif\n");

#ifdef SHRT_MIN
		val = -(SHRT_MIN);
		printf("#undef	SHRT_MIN\n");
#else
		val = (unsigned short)(us >> 1) + 1;
#endif
		printf("#define SHRT_MIN	-%lu\n", val);

#ifdef SHRT_MAX
		val = SHRT_MAX;
		printf("#undef	SHRT_MAX\n");
#else
		val = (unsigned short)(us >> 1);
#endif
		printf("#define SHRT_MAX	%lu\n", val);

		if (ui == us)
		{
#ifdef UINT_MAX
			printf("#undef	UINT_MAX\n");
#endif
			printf("#define UINT_MAX	USHRT_MAX\n");

#ifdef INT_MIN
			printf("#undef	INT_MIN\n");
#endif
			printf("#define INT_MIN		SHRT_MIN\n");

#ifdef INT_MAX
			printf("#undef	INT_MAX\n");
#endif
			printf("#define INT_MAX		SHRT_MAX\n");
		}
		else
		{
#ifdef UINT_MAX
			val = UINT_MAX;
			printf("#undef	UINT_MAX\n");
#else
			val = ui;
#endif
			printf("#if __STDC__\n");
			printf("#define UINT_MAX	%luU\n", val);
			printf("#else\n");
			printf("#define UINT_MAX	%lu\n", val);
			printf("#endif\n");

#ifdef INT_MIN
			val = -(INT_MIN);
			printf("#undef	INT_MIN\n");
#else
			val = (unsigned int)(ui >> 1) + 1;
#endif
			printf("#define INT_MIN		-%lu\n", val);

#ifdef INT_MAX
			val = INT_MAX;
			printf("#undef	INT_MAX\n");
#else
			val = (unsigned int)(ui >> 1);
#endif
			printf("#define INT_MAX		%lu\n", val);
		}

		if (ul == ui)
		{
#ifdef ULONG_MAX
			printf("#undef	ULONG_MAX\n");
#endif
			printf("#define ULONG_MAX	UINT_MAX\n");

#ifdef LONG_MIN
			printf("#undef	LONG_MIN\n");
#endif
			printf("#define LONG_MIN	INT_MIN\n");

#ifdef LONG_MAX
			printf("#undef	LONG_MAX\n");
#endif
			printf("#define LONG_MAX	INT_MAX\n");
		}
		else
		{
#ifdef ULONG_MAX
			val = ULONG_MAX;
			printf("#undef	ULONG_MAX\n");
#else
			val = ui;
#endif
			printf("#if __STDC__\n");
			printf("#define ULONG_MAX	%luU\n", val);
			printf("#else\n");
			printf("#define ULONG_MAX	%lu\n", val);
			printf("#endif\n");

#ifdef LONG_MIN
			val = -(LONG_MIN);
			printf("#undef	LONG_MIN\n");
#else
			val = (unsigned long)(ul >> 1) + 1;
#endif
			printf("#define LONG_MIN	-%lu\n", val);

#ifdef LONG_MAX
			val = LONG_MAX;
			printf("#undef	LONG_MAX\n");
#else
			val = (unsigned long)(ul >> 1);
#endif
			printf("#define LONG_MAX	%lu\n", val);
		}

		/*
		 * POSIX values
		 */

		printf("\n");
		printf("/*\n");
		printf(" * system implementation limits\n");
		printf(" */\n");
		printf("\n");

#ifdef ARG_MAX
		val = ARG_MAX;
		printf("#undef  ARG_MAX\n");
#else
#ifdef NCARGS
		val = NCARGS;
#else
		val = 5120;
#endif
#endif
		printf("#define ARG_MAX		%lu\n", val);

#ifdef MAX_CANON
		val = MAX_CANON;
		printf("#undef  MAX_CANON\n");
#else
#ifdef CANBSIZ
		val = CANBSIZ;
#else
		val = 256;
#endif
#endif
		printf("#define MAX_CANON	%lu\n", val);

#ifdef LINK_MAX
		val = LINK_MAX;
		printf("#undef  LINK_MAX\n");
#else
#ifdef MAXLINK
		val = MAXLINK;
#else
#ifdef SHRT_MAX
		val = SHRT_MAX;
#else
		val = (unsigned short)(us >> 1);
#endif
#endif
#endif
		printf("#define LINK_MAX	%lu\n", val);

#ifdef NGROUPS_MAX
		val = NGROUPS_MAX;
		printf("#undef  NGROUPS_MAX\n");
#else
#ifdef NGROUPS
		val = NGROUPS;
#else
		val = 0;
#endif
#endif
		printf("#define NGROUPS_MAX	%lu\n", val);

#ifdef PATH_MAX
		val = PATH_MAX;
		printf("#undef  PATH_MAX\n");
#else
#ifdef MAXPATHLEN
		val = MAXPATHLEN;
#else
		val = 1024;
#endif
#endif
		printf("#define PATH_MAX	%lu\n", val);

#ifdef OPEN_MAX
		val = OPEN_MAX;
		printf("#undef  OPEN_MAX\n");
#else
		val = 2;
		while ((i = dup(0)) >= 0)
			if (i > val) val = i;
#endif
		printf("#define OPEN_MAX	%lu\n", val);

#ifdef OPEN_MAX_CEIL
		val = OPEN_MAX_CEIL;
		printf("#undef  OPEN_MAX_CEIL\n");
		printf("#define OPEN_MAX_CEIL	%lu\n", val);
#else
		printf("#define OPEN_MAX_CEIL	OPEN_MAX\n");
#endif
		printf("\n");
	}
	else if (!strcmp(hdr, "sigdata"))
	{
		/*
		 * sigdata.h for sigdata.c
		 */

		k = 0;
		for (i = 0; map[i].name; i++)
			if ((j = map[i].value) > 0 && j < elements(index))
			{
				if (j > k) k = j;
				index[j] = i;
			}
		printf("int	sig_max = %d;\n", k);
		printf("\n");
		printf("char*	sig_name[] =\n");
		printf("{\n");
		for (i = 0; i <= k; i++)
			if (j = index[i]) printf("	\"%s\",\n", map[j].name);
			else printf("	\"%d\",\n", i);
		printf("};\n");
		printf("\n");
		printf("char*	sig_text[] =\n");
		printf("{\n");
		for (i = 0; i <= k; i++)
			if (j = index[i]) printf("	\"%s\",\n", map[j].text);
			else printf("	\"Signal %d\",\n", i);
		printf("};\n");
	}
	else
	{
		fprintf(stderr, "Usage: genhdr align ^ dirent ^ fsinfo ^ limits ^ sigdata\n");
		exit(1);
	}
	exit(0);
}
