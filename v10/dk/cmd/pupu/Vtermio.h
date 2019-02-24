/* @(#)termio.h	6.2 */
#define	V__NCC	8

/* control characters */
#define	V__VINTR	0
#define	V__VQUIT	1
#define	V__VERASE	2
#define	V__VKILL	3
#define	V__VEOF	4
#define	V__VEOL	5
#define	V__VEOL2	6
#define	V__VMIN	4
#define	V__VTIME	5
#define	V__VSWTCH	7



/* input modes */
#define	V__IGNBRK	0000001
#define	V__BRKINT	0000002
#define	V__IGNPAR	0000004
#define	V__PARMRK	0000010
#define	V__INPCK	0000020
#define	V__ISTRIP	0000040
#define	V__INLCR	0000100
#define	V__IGNCR	0000200
#define	V__ICRNL	0000400
#define	V__IUCLC	0001000
#define	V__IXON	0002000
#define	V__IXANY	0004000
#define	V__IXOFF	0010000

/* output modes */
#define	V__OPOST	0000001
#define	V__OLCUC	0000002
#define	V__ONLCR	0000004
#define	V__OCRNL	0000010
#define	V__ONOCR	0000020
#define	V__ONLRET	0000040
#define	V__OFILL	0000100
#define	V__OFDEL	0000200
#define	V__NLDLY	0000400
#define	V__NL0	0
#define	V__NL1	0000400
#define	V__CRDLY	0003000
#define	V__CR0	0
#define	V__CR1	0001000
#define	V__CR2	0002000
#define	V__CR3	0003000
#define	V__TABDLY	0014000
#define	V__TAB0	0
#define	V__TAB1	0004000
#define	V__TAB2	0010000
#define	V__TAB3	0014000
#define	V__BSDLY	0020000
#define	V__BS0	0
#define	V__BS1	0020000
#define	V__VTDLY	0040000
#define	V__VT0	0
#define	V__VT1	0040000
#define	V__FFDLY	0100000
#define	V__FF0	0
#define	V__FF1	0100000

/* control modes */
#define	V__CBAUD	0000017
#define	V__B0	0
#define	V__B50	0000001
#define	V__B75	0000002
#define	V__B110	0000003
#define	V__B134	0000004
#define	V__B150	0000005
#define	V__B200	0000006
#define	V__B300	0000007
#define	V__B600	0000010
#define	V__B1200	0000011
#define	V__B1800	0000012
#define	V__B2400	0000013
#define	V__B4800	0000014
#define	V__B9600	0000015
#define	V__EXTA	0000016
#define	V__EXTB	0000017
#define	V__CSIZE	0000060
#define	V__CS5	0
#define	V__CS6	0000020
#define	V__CS7	0000040
#define	V__CS8	0000060
#define	V__CSTOPB	0000100
#define	V__CREAD	0000200
#define	V__PARENB	0000400
#define	V__PARODD	0001000
#define	V__HUPCL	0002000
#define	V__CLOCAL	0004000
#define	V__LOBLK	0010000

/* line discipline 0 modes */
#define	V__ISIG	0000001
#define	V__ICANON	0000002
#define	V__XCASE	0000004
#define	V__ECHO	0000010
#define	V__ECHOE	0000020
#define	V__ECHOK	0000040
#define	V__ECHONL	0000100
#define	V__NOFLSH	0000200

#define	V__SSPEED	7	/* default speed: 300 baud */

/*
 * Ioctl control packet
 */
struct Vtermio {
	unsigned short	c_iflag;	/* input modes */
	unsigned short	c_oflag;	/* output modes */
	unsigned short	c_cflag;	/* control modes */
	unsigned short	c_lflag;	/* line discipline modes */
	char	c_line;		/* line discipline */
	unsigned char	c_cc[V__NCC];	/* control chars */
};
#define	IOCTYPE	0xff00

#define	TIOC	('T'<<8)
#define	TCGETA	(TIOC|1)
#define	TCSETA	(TIOC|2)
#define	TCSETAW	(TIOC|3)
#define	TCSETAF	(TIOC|4)
#define	TCSBRK	(TIOC|5)
#define	TCXONC	(TIOC|6)
#define	TCFLSH	(TIOC|7)
#define	TCDSET	(TIOC|32)

/* @(#)ttold.h	6.1 */
/* modes */
#define	VO_HUPCL	01
#define	VO_XTABS	02
#define	VO_LCASE	04
#define	VO_ECHO	010
#define	VO_CRMOD	020
#define	VO_RAW	040
#define	VO_ODDP	0100
#define	VO_EVENP	0200
#define	VO_NLDELAY	001400
#define	VO_NL1	000400
#define	VO_NL2	001000
#define	VO_TBDELAY	002000
#define	VO_NOAL	004000
#define	VO_CRDELAY	030000
#define	VO_CR1	010000
#define	VO_CR2	020000
#define	VO_VTDELAY	040000
#define	VO_BSDELAY	0100000
