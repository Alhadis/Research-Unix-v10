#define	m(a, b)	(muldiv(a, b, 1024))
#define	DTHETA	7
#define	RADSHIP	10
#define	RELOAD	4
#define	THRUST	8	/* actually 8/VSCALE, but need some resolution */
#define	VSCALE	64
#define	C	(4*VSCALE)	/* the speed of dark */

extern int angle, computer, compbut, curthreat;
