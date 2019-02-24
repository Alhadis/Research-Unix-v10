#define RESET_MR 	0x10
#define RESET_REC  	0x20
#define RESET_TRANS  	0x30
#define RESET_ERR  	0x40
#define RESET_BCH	0x50

#define ENB_TX		0x04
#define DIS_TX		0x08
#define ENB_RX		0x01
#define DIS_RX		0x02

#define CHAR_ERR	0x00
#define PAR_ENB		0x00
#define EVEN_PAR	0x00
#define ODD_PAR		0x04
#define NO_PAR		0x10
#define CBITS8		0x03
#define CBITS7		0x02
#define CBITS6		0x01
#define CBITS5		0x00

#define NORM_OP		0x00
#define LCL_LOOP	0x80
#define TWOSTOPB	0x0F
#define ONENHALF	0x08
#define ONESTOPB	0x07

#define BD19200BPS	0xCC
#define BD9600BPS	0xBB
#define BD4800BPS	0x99
#define BD2400BPS	0x88
#define BD1200BPS	0x66
#define BD300BPS	0x44
#define BD110BPS	0x11

#define RCV_RDY		0x01
#define REV_RDY		0x01
#define XMT_EMT		0x08
#define XMT_RDY		0x04
#define PAR_ERR		0x20
#define RCVD_BRK	0x80
#define FRM_ERR		0x40
#define OVER_ERR	0x10
#define FIFOFULL	0x02
#define STRT_BRK	0x60
#define STOP_BRK	0x70
