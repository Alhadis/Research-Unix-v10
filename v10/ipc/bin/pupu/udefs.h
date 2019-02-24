/*
 *		@(#)udefs.h	2.1 DKHOST 85/01/15
 */

#include	<setjmp.h>
#include	<pwd.h>
#include	<sys/stat.h>
#define	S_IFIFO	010000	/* UNIX-V FIFO file type, not V8 valid */

	typedef jmp_buf	Tjmp;

#	define Mjmp	/* mos, mou */ Tjmp
#	define Ajmp	auto Tjmp
#	define Ejmp	extern Tjmp
#	define Rjmp	register Tjmp
#	define Sjmp	static Tjmp
#	define Pjmp	/* parameter, value */ Tjmp
#	define Xjmp	/* extdef */ Tjmp

	typedef struct passwd		*TpwP;

#	define MpwP	/* mos, mou */ TpwP
#	define ApwP	auto TpwP
#	define EpwP	extern TpwP
#	define RpwP	register TpwP
#	define SpwP	static TpwP
#	define PpwP	/* parameter, value */ TpwP
#	define XpwP	/* extdef */ TpwP

	typedef struct stat		Tstat, *TstatP;

#	define Mstat	/* mos, mou */ Tstat
#	define Astat	auto Tstat
#	define Estat	extern Tstat
#	define Rstat	register Tstat
#	define Sstat	static Tstat
#	define Pstat	/* parameter, value */ Tstat
#	define Xstat	/* extdef */ Tstat
#	define MstatP	/* mos, mou */ TstatP
#	define AstatP	auto TstatP
#	define EstatP	extern TstatP
#	define RstatP	register TstatP
#	define SstatP	static TstatP
#	define PstatP	/* parameter, value */ TstatP
#	define XstatP	/* extdef */ TstatP
