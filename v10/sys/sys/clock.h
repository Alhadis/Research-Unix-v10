/*
 * VAX clock registers
 */

#define	ICCS_RUN	0x00000001
#define	ICCS_TRANS	0x00000010
#define	ICCS_SS		0x00000020
#define	ICCS_IE		0x00000040
#define	ICCS_INT	0x00000080
#define	ICCS_ERR	0x80000000
	
#define	SECDAY		((unsigned)(24*60*60))		/* seconds per day */
#define	SECYR		((unsigned)(365*SECDAY))	/* per common year */
/*
 * TODRZERO is the what the TODR should contain when the ``year'' begins.
 * The TODR should always contain a number between 0 and SECYR+SECDAY.
 */
#define	TODRZERO	((unsigned)(1<<28))

#define	YRREF		1970
#define	LEAPYEAR(year)	((year)%4==0)	/* good till time becomes negative */

/*
 * Start a 60 HZ clock.
 */

#define	CLK60HZ	(-16667)	/* 16.667 ms */

#define	clkstart() {\
	mtpr(NICR, CLK60HZ);\
	mtpr(ICCS, ICCS_RUN+ICCS_IE+ICCS_TRANS+ICCS_INT+ICCS_ERR);\
}
#define	clkreld()	mtpr(ICCS, ICCS_RUN|ICCS_IE|ICCS_INT|ICCS_ERR)

#define	CLKBIAS	100		/* when adjusting clock, do so .1ms at a time */

/*
 * Software clock is software interrupt level 8
 */
#define	setsoftclock()	mtpr(SIRR, 0x8)
