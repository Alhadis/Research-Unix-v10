#include <stdio.h>
#include "defines.h"

static char Mode = 2;	/* remembers resolution for check in window() */

advance(how)
{
	if (how)
		qcmd("\020", 1);	/* enable frame advance */
	else
		qcmd("\017", 1);	/* disable frame advance */
	qpause();
}

shutter(how)
{
	if (how)
		qcmd("\033", 1);	/* open shutter */
	else
		qcmd("\032", 1);	/* close and advance film */
	qpause();
	qpause();
	qpause();
	qpause();
	qpause();
	qpause();
}

beep()
{
	qcmd("\021", 1);	/* beep */
	qpause();
}

loadluts(wbuf)		/* 8 bit wide lookup tables */
	char *wbuf;
{	char rbuf[768];
	register int i;

	qcmd("\000", 1);	/* load the rgb LUT's */
	qpause();
	qwrite(wbuf, 768);
	qpause();
	qcmd("\001", 1);	/* read back rgb LUT's */
	qpause();

	if (qread(rbuf, 768) != 768)
		return 0;
	else
		for(i = 0; i < 768; i++)
			if (rbuf[i] != wbuf[i])
				return 0;
	qpause();
	return 1;
}

load12luts(wbuf)	/* 12 bit wide lookup tables */
	short *wbuf;
{
	register int i;
	short rbuf[768];

	printf("loading 12 bit lookup tables\n"); fflush(stdout);
	qpause();
	qcmd("\094", 1);	/* load the rgb LUT's */
	qpause();
	qwrite((char *)wbuf, 1536);	/* sizeof(short)*768 */
	qpause();
	qcmd("\096", 1);	/* read back rgb LUT's */
	qpause();

	if (qread((char *)rbuf, 1536) != 1536)
		return 0;
	else
		for(i = 0; i < 768; i++)
			if (rbuf[i] != wbuf[i])
				return 0;
	qpause();
	return 1;
}

singlepass(how)
{	/* set single pass pixel mode */
	switch (how) {
	default     :
	case NEUTRAL:	qcmd("\002", 1); printf("singlepass neutral\n"); break;
	case RED    :	qcmd("\003", 1); printf("singlepass red\n"); break;
	case GREEN  :	qcmd("\004", 1); printf("singlepass green\n"); break;
	case BLUE   :	qcmd("\005", 1); printf("singlepass blue\n"); break;
	}
	qpause();
}

threepass()
{	/* set three pass pixel mode */
	qcmd("\006", 1);
	qpause();
	printf("3pass mode\n");
}

nocalibs()
{	/* disable automatic calibrations */
	qcmd("\037", 1);
	qpause(); qpause();
}

testpattern(which)
{
	if (which)
		qcmd("\012", 1);	/* test pattern 1 */
	else
		qcmd("\013", 1);	/* test pattern 0 */
	qpause();
}

linedelay(n)
	unsigned char n;
{	/* interline time delay - to match host speed to qcr scanrate */

	qcmd("\014", 1);
	qpause();
	qwrite(n, 1);		/* max delay n msec per line */
	qpause();
}

version(data)
	char *data;
{
	qcmd("\015", 1);
	qpause();
	qread(data, 14);
	qpause();
}

window(x, y, w, h)
{
	unsigned char buf[8];
	int maxsz = (Mode == 2)?1536:3072;	/* 1536 = 2*768 = 2*3/4*1024 */

	if (w+x > maxsz || h-y > maxsz)
	{	printf("error: bad image size\n");
		if (Mode == 2)
			printf("\tmax: -1024,  768, 1024,  -768\n");
		else
			printf("\tmax: -2048, 1536, 2048, -1536\n");
		printf("\thave: %4d, %4d, %4d, %4d\n", x, y, x+w, y-h);
		return;
	}
	buf[0] = (x>>8)&255; buf[1] = x&255;
	buf[2] = (w>>8)&255; buf[3] = w&255;
	buf[4] = (y>>8)&255; buf[5] = y&255;
	buf[6] = (h>>8)&255; buf[7] = h&255;
	qpause();
	qcmd("\016", 1);
	qpause();
	qwrite(buf, 8);
	qpause();
	printf("window (%dx%d): %d %d %d %d\n", w, h, x, y, x+w, y-h);
}

filmtype(n)
{
	if (n == NONE || n == TMAX_100)
	{	customluts(n);
		return;
	}
	qcmd("\043", 1);
	qpause();
	switch (n) {
	default:
	case LINEAR    : qwrite("\001", 1); printf("linear lut's\n"); break;
	case POLA559   : qwrite("\002", 1); printf("filmtype pola559\n"); break;
	case EKTA100   : qwrite("\003", 1); printf("filmtype ekat100\n"); break;
	case EKTA100_4k: qwrite("\004", 1); printf("filmtype ekta100_4k\n"); break;
	case POLA559_4k: qwrite("\005", 1); printf("filmtype pola559_4k\n"); break;
	case POLA809_4k: qwrite("\007", 1); printf("filmtype pola809_4k\n"); break;
	}
	qpause();
}

moduleno()
{	char c;

	qcmd("\045", 1);
	qpause();
	qread(&c, 1);
	qpause();
	switch (c) {
	case 240: printf("no module\n"); break;
	case  16: printf("35mm module\n"); break;
	case  32: printf("4x5\" module\n"); break;
	case  64: printf("8x10\" module\n"); break;
	default : printf("bad module number %d\n", c);
	}
	return c;	/*  (none, m35, m120, or m240) */
}

setreso(n)
{
	if (n == '2')
	{	qcmd("\077", 1);
		printf("2k mode set\n"); 
	} else if (n == '4')
	{	qcmd("\078", 1);
		printf("4k mode set \n"); 
	} else
	{	printf("resolution must be either 2 or 4\n");
		return;
	}
	qpause();
	fflush(stdout);
}

resolution()
{
	Mode = qreso();
	switch (Mode) {
	case 2:  printf("2k mode\n"); break;
	case 4:  printf("4k mode\n"); break;
	default: printf("qcr: bad resolution %d\n", Mode);
	}
	fflush(stdout);
	qpause();
	return Mode;
}

handshake(how)
{
	if (how)
		qcmd("\063", 1);	/* enable full handshake */
	else
		qcmd("\062", 1);	/* disable (default) */
	qpause();
}

rotate(how)
{
	if (how)
		qcmd("\067", 1);	/* enable 90 deg rotation */
	else
		qcmd("\066", 1);	/* disable (default) */
	qpause();
}

brightness(r,g,b,n)
{
	char buf[4]; int oval;

	buf[0] = r; buf[1] = g; buf[2] = b; buf[3] = n;
	qcmd("\055", 1); qpause();
	qwrite(buf, 4);  qpause();
}

getbright(nq)
	char *nq;
{	char buf[10]; int i;

	qcmd("\073", 1); qpause();
	qread(buf, 10);  qpause();
	for (i = 0; i < 9; i++)
		nq[i] = buf[i+1]&0xFF;
	qpause();
}

whaterror(n)
{	/* primary erro code, as shown on display lights:
		128	64	32	16
		  8	 4	 2	 1
	*/
	switch (n) {
	case   1:	printf("out of film\n"); break;
	case 129:	printf("unexpected command byte\n"); break;
	case 130:	printf("unexpected data byte\n"); break;
	case 131:	printf("illegal command byte\n"); break;
	case 132:	printf("illegal data byte\n"); break;
	case 133:	printf("command interrupt\n"); break;
	case 134:	printf("unimplemented function\n"); break;
	case 145:	printf("memory fault\n"); break;
	case 146:	printf("gpib interface fault\n"); break;
	case 147:	printf("crt beam calibration fault\n"); break;
	case 161:	printf("module fault\n"); break;
	case 162:	printf("filter wheel fault\n"); break;
	default:	printf("-bad primary code (%d)-\n", n); break;
	}
}

qerror()
{	char buf[8];

	qcmd("\044", 1);
	qpause();
	qread(buf, 8);
	whaterror(buf[0]&0xFF);
	qpause();
	printf("Error: pri=0x%02x sec=0x%02x", (buf[0]&0xFF), (buf[1]&0xFF));
	printf("\n\tH=0x%02x L=0x%02x ", (buf[2]&0xFF), (buf[3]&0xFF));
	printf("D=0x%02x \n\tE=0x%02x ", (buf[4]&0xFF), (buf[5]&0xFF));
	printf("B=0x%02x C=0x%02x\n", (buf[6]&0xFF), (buf[7]&0xFF));
}

qreso()
{	unsigned char buf[64];
	int i, j;

	qpause();
	qcmd("\074", 1);
	qpause();
	qpause();
	qread(buf, 1);
	qread(&buf[1], buf[0]);
	qpause();
	qpause();
	return (int) buf[18];
}

qstatus()
{	unsigned char buf[64];
	int i, j;

	qcmd("\074", 1);
	qpause();
	memset(buf, 0, 64);
	qread(buf, 1);
	printf("QCR status (%d bytes):\n", buf[0]);
	qread(&buf[1], buf[0]);
	for (i = 0; i < 38; i++)
	{	switch (i) {
		case  6: printf("\timage height: %d\n", qnum(buf[i],buf[i+1]));
			 i++; break;
		case  8: printf("\ty-offset    : %d\n", qnum(buf[i],buf[i+1]));
			 i++; break;
		case 10: printf("\timage  width: %d\n", qnum(buf[i],buf[i+1]));
			 i++; break;
		case 12: printf("\tx-offset    : %d\n", qnum(buf[i],buf[i+1]));
			 i++; break;
		case 26: printf("\tframe counter: %d\n", qnum(buf[i],buf[i+1]));
			 i++; break;
		case 36: printf("\tdrift value : %d\n", qnum(buf[i],buf[i+1]));
			 i++; break;
		default: whatstatus(i, buf[i]); break;
		}
	}
}

qnum(a, b)
	unsigned char a, b;
{	short n;

	n = b + a<<8;
	return (int)n;
}

whatstatus(n, m)
	unsigned char m;
{
	switch (n) {
	case  0: if (m) printf("\texternal luts loaded\n"); break;
	case  1: if (m) printf("\tneutral  luts loaded\n"); break;
	case  2: printf("\t%d msec interline time delay\n", m); break;
	case  3: if (m) printf("\tend of film\n"); break;
	case  4: printf("\tautomatic filter wheel %sabled\n",(m)?"dis":" en");break;
	case  5: printf("\tauto camera operation  %sabled\n",(m)?"dis":" en");break;
	case 14: printf("\tmodule no: %d\n", m); break;
	case 15: printf("\tautomatic calibration %sabled\n",(m)?"en":"dis");break;
	case 16: printf("\tautomatic warbling    %sabled\n",(m)?"dis":" en");break;
	case 17: printf("\trunlength black jumps %sabled\n",(m)?"dis":" en");break;
	case 18: printf("\tresolution: %dk\n", m); break;
	case 19: printf("\tmode was %sternally selected\n", (m)?"ex":"in");break;
	case 20: if (m) printf("\timage is 90 degrees rotated\n"); break;
	case 21: if (m) printf("\timage is mirrored\n"); break;
	case 22: if (m) printf("\tunbuffered raster mode\n"); break;
	case 23: if (m) printf("\treturn to no filter disabled\n"); break;
	case 24: if (m) printf("\tx and y coordinates are swapped\n"); break;
	case 25: if (m) printf("\tred repeat is enabled\n"); break;
	case 28: if (m) printf("\tfilm is in camera\n"); break;
	case 29: if (m) printf("\thalf frame mode enabled\n"); break;
	case 30: if (m) printf("\tbulk back is in use\n"); break;
	case 31: if (m) printf("\timage is 180 degrees rotated\n"); break;
	case 32: if (m) printf("\t12 bit lookup tables are loaded\n"); break;
	case 33: printf("\tinternal timing constant 1: %d\n", m); break;
	case 34: printf("\tinternal timing constant 2: %d\n", m); break;
	case 35: if (m) printf("\t'irradiance modulation' is active\n"); break;
	default: printf("\t%2d: %2d\n", n, m); break;
	}
}

/* customized lookup tables */

#include "qlut2.h"	/* 8b  generated by /usr/gerard/CAL/QCR/qlutnew.c */

/*
#include "LUT_pola52.h"	** awful, reversed? **
*/

unsigned char lutpola52[] = {	/* pola52 lut for brightness 148 */
	   0,   46,   54,   61,   67,   68,   68,   69,   69,   70, 
	  70,   71,   71,   72,   72,   73,   73,   74,   74,   75, 
	  75,   76,   76,   77,   77,   78,   78,   79,   79,   80, 
	  80,   81,   81,   82,   82,   83,   83,   84,   84,   85, 
	  85,   85,   86,   86,   86,   86,   87,   87,   87,   88, 
	  88,   88,   88,   89,   89,   89,   90,   90,   90,   90, 
	  91,   91,   91,   92,   92,   92,   92,   93,   93,   93, 
	  94,   94,   94,   94,   95,   95,   95,   96,   96,   96, 
	  96,   97,   97,   97,   98,   98,   98,   98,   99,   99, 
	  99,  100,  100,  100,  100,  101,  101,  101,  102,  102, 
	 103,  103,  104,  104,  105,  105,  105,  106,  106,  107, 
	 107,  108,  108,  109,  109,  110,  110,  111,  111,  112, 
	 112,  113,  113,  114,  114,  115,  115,  115,  116,  116, 
	 117,  117,  118,  118,  119,  119,  120,  120,  120,  121, 
	 121,  122,  122,  122,  123,  123,  124,  124,  124,  125, 
	 125,  126,  126,  126,  127,  127,  128,  128,  128,  129, 
	 129,  130,  130,  130,  131,  131,  132,  132,  132,  133, 
	 133,  134,  134,  134,  135,  135,  136,  136,  136,  137, 
	 137,  138,  138,  139,  139,  139,  140,  140,  141,  141, 
	 141,  142,  142,  143,  143,  144,  144,  144,  145,  145, 
	 146,  146,  147,  147,  147,  148,  148,  149,  149,  150, 
	 150,  150,  151,  151,  152,  152,  153,  154,  154,  155, 
	 156,  157,  158,  159,  160,  161,  161,  162,  163,  164, 
	 165,  166,  167,  168,  169,  169,  171,  173,  175,  176, 
	 178,  180,  182,  184,  185,  188,  195,  201,  205,  208, 
	 211,  215,  218,  221,  234,  255, 
};


customluts(n)
{	short all12uts[768];
	unsigned char alluts[768];
	register int i;

	switch (n) {
	case TMAX_100:
		for (i = 0; i < 768; i++)
			alluts[i] = qcr_lut[i%256];
		if (!loadluts(alluts))
		printf("loading new lookup tables failed\n");
		break;
	default:	/* polaroid type 52 */
		for (i = 0; i < 768; i++)
			alluts[i] = lutpola52[i%256];
		if (!loadluts(alluts))
		printf("loading new lookup tables failed\n");
		break;
	}
}

/*
not implemented:
18	 n	 +	red     runlength encoded single pass pixel mode
19	 n	 +	green   runlength sppm
20	 n	 +	blue    runlength sppm
21	 n	 +	neutral runlength sppm
22	 n	 +	three pass runlength image

24	 -	 -	disable automatic filter wheel control
25	 -	 -	enable  automatic filter wheel control (default)

29	256	 -	load a neutral LUT (default for neutral is blue lut)
30	256	 -	read neutral LUT
31/32	 -	 -	disable/enable automatic calibrations
33/34	 -	 -	disable/enable warble sounds at end of image
38	 -	 +	start an intensity calibration for used levels
39	 -	 +	intensity calibration for all 9 levels

40/41	 -	 -	disable/enable black jumping (for runlength)
48/49	 -	 -	disable/enable mirror imaging
52/53	 -	 -	disable/enable end-of-image filter wheel movement
54/55	 -	 -	disable/enable switch of x/y coordinates
56/57	 -	 -	disable/enable red repeat mode
*/
