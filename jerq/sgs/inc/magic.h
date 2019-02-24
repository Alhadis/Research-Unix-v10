/*
 * static char ID_magich[] = "@(#) magic.h: 1.1 1/7/82";
 */

/* normal unix user, text and data in separate segments = 410		*/
#define	NMAGIC		0410
/* text and data squashed together = 407				*/
#define	OMAGIC		0407
/* public library created by ldp					*/
#define	PLIBMAGIC	0370
/* data library created by ldp						*/
#define	DLIBMAGIC	0371
/* ldp created kernel process						*/
#define	KMAGIC		0401
/* ldp created supervisor process					*/
#define	SMAGIC		0402
/* ldp created user process						*/
#define	USRMAGIC	0403
/* sgen created boot process						*/
#define	BOOTMAGIC	0400
