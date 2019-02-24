/*	static char Y2681_h[]="@(#)2681.h	2.1.1.2";	*/
/*
 * Signetics 2681 Dual UART
 */

#ifndef _2681
#define _2681

/*
 *
 *
 *                                    READ         WRITE
 *                                  7      0     7      0
 *                                 +--------+   +--------+
 *  (struct duart *) ->mr1_2a      |        |   |        |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->a_sr_csr    |  SRA   |   |  CSRA  |
 *                                 +--------+   +--------+
 *
 *                                              +--------+
 *		     ->a_cmnd       RESERVED    |   CRA  |
 *                                              +--------+
 *
 *                                 +--------+   +--------+
 *                   ->a_data      |  RHRA  |   |  TXRA  |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->ipc_acr     |  IPCR  |   |   ACR  |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->is_imr      |   ISR  |   |   IMR  |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->ctur        |   CTU  |   |  CTUR  |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->ctlr        |   CTL  |   |  CTLR  |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->mr1_2b      |        |   |        |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->b_sr_csr    |   SRB  |   |  CSRB  |
 *                                 +--------+   +--------+
 *
 *                                              +--------+
 *                   ->b_cmnd       RESERVED    |   CRB  |
 *                                              +--------+
 *
 *                                 +--------+   +--------+
 *                   ->b_data      |  RHRB  |   |  THRB  |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->ip_opcr     | In.Prt |   |  OPCR  |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->scc_sopbc   |  SCC   |   |  SOPBC |
 *                                 +--------+   +--------+
 *
 *                                 +--------+   +--------+
 *                   ->scc_ropbc   |  SCC   |   |  ROPBC |
 *                                 +--------+   +--------+
 */

struct duart
{
	int : 16; int :8; unsigned char mr1_2a;	  /* Mode Register A Channels 1 & 2 */
	int : 16; int :8; unsigned char a_sr_csr; /* Status Reg. A / Clock Select Reg. A */
	int : 16; int :8; unsigned char a_cmnd;	  /* Command Register A */
	int : 16; int :8; unsigned char a_data;   /* RX Holding / TX Holding Register A */

	int : 16; int :8; unsigned char ipc_acr;  /* Input Port Change / Aux. Control Register A */
	int : 16; int :8; unsigned char is_imr;	  /* Interrupt Status / Interrupt Mask Register A */
	int : 16; int :8; unsigned char ctur;	  /* Counter/Timer Upper Register */
	int : 16; int :8; unsigned char ctlr;	  /* Counter/Timer Lower Register */

	int : 16; int :8; unsigned char mr1_2b;	  /* Mode Register B Channels 1 & 2 */
	int : 16; int :8; unsigned char b_sr_csr; /* Status Reg. B / Clock Select Reg. B */
	int : 16; int :8; unsigned char b_cmnd;	  /* Command Register B */
	int : 16; int :8; unsigned char b_data;   /* RX Holding / TX Holding Register B */

	int : 16; int :8; int :8;		  /* RESERVED */

	int : 16; int :8; unsigned char ip_opcr;  /* Input Port / Output Port Conf. Reg. */
	int : 16; int :8; unsigned char scc_sopbc;/* Start Counter Command / Set Output Port Bits Command */
	int : 16; int :8; unsigned char scc_ropbc;/* Stop Counter Command / Reset Output Port Bits Command */
};

/* Register Bit Format Defines	*/

/*
 *	Mode Register 1 Channel A and B Defines
 */

#define	ENB_RX_RTS	0x80
#define ENB_RXINT	0x40
#define BLCK_ERR	0x20
#define CHAR_ERR	0x00
#define PAR_ENB		0x00
#define	FRC_PAR		0x08
#define	NO_PAR		0x10
#define	SPECIAL		0x18
#define EVN_PAR		0x00
#define ODD_PAR		0x04
#define CBITS5		0x00
#define	CBITS6		0x01
#define	CBITS7		0x02
#define	CBITS8		0x03

/*
 *	Mode Register 2 Channel A and B Defines
 */
 
#define	NRML_MOD	0x00
#define	ENB_ECHO	0x40
#define	LCL_LOOP	0x80
#define	REM_LOOP	0xc0

#define	ENB_TXRTS	0x20
#define	ENB_CTSTX	0x10

#define	ZEROP563SB	0x00
#define	ZEROP625SB	0x01
#define	ZEROP688SB	0x02
#define	ZEROP750SB	0x03
#define	ZEROP813SB	0x04
#define	ZEROP875SB	0x05
#define	ZEROP938SB	0x06

#define	ONEP000SB	0x07
#define	ONEP563SB	0x08
#define	ONEP625SB	0x09
#define	ONEP688SB	0x0a
#define	ONEP750SB	0x0b
#define	ONEP813SB	0x0c
#define	ONEP875SB	0x0d
#define	ONEP938SB	0x0e

#define	TWOP000SB	0x0f

/*
 *	Clock Select Register Defines
 */

#define	BD50BPS		0x00
#define	BD75BPS		0x00		/* ACR[7]=SET2	*/
#define	BD110BPS	0x11
#define	BD134BPS	0x22
#define	BD150BPS	0x33		/* ACR[7]=SET2	*/
#define	BD200BPS	0x33
#define	BD300BPS	0x44
#define	BD600BPS	0x55
#define	BD1200BPS	0x66
#define	BD1050BPS	0x77
#define	BD1800BPS	0xaa		/* ACR[7]=SET2	*/
#define	BD2000BPS	0x77		/* ACR[7]=SET2	*/
#define	BD2400BPS	0x88
#define	BD4800BPS	0x99
#define	BD7200BPS	0xaa
#define	BD9600BPS	0xbb
#define	BD19200BPS	0xcc		/* ACR[7]=SET2	*/
#define	BD38400BPS	0xcc

/*
 *	Miscellaneous Commands for CRA/CRB
 */

#define	NO_OP		0x00
#define	RESET_MR	0x10
#define	RESET_RECV	0x20
#define	RESET_TRANS	0x30
#define	RESET_ERR	0x40
#define	RST_A_BCI	0x50
#define	STRT_BRK	0x60
#define	STOP_BRK	0x70
#define	DIS_TX		0x08
#define	ENB_TX		0x04
#define	DIS_RX		0x02
#define	ENB_RX		0x01

/*
 *	Status Register Defines for SRA/SRB
 */

#define	RCVD_BRK	0x80
#define	FRM_ERR		0x40
#define	PAR_ERR		0x20
#define	OVR_RUN		0x10
#define	XMT_EMT		0x08
#define	XMT_RDY		0x04
#define	FIFOFULL	0x02
#define	RCV_RDY		0x01

/*
 *	Register Defines for IPCR/OPCR
 */

#define	DCDA		0x01
#define	DCDB		0x02
#define	DTRA		0x01
#define	DTRB		0x02

#endif
