#include <jerq.h>
#include <queue.h>
#include <kbd.h>
#include <setup.h>

int	kbdrepeat;
int	rptcount;
int	kbdstatus;
int	latin1;
int	latinchar;

/*
 * All characters with high bits set are used to index this table to yield
 * the generated character. This makes TAB==^I, etc, and removes the silly
 * mappings like ^@==0xba instead of NUL.  However, ^, is still NUL.
 * For now, shifted keys are the same as unshifted, if mapped in this table;
 * for example shift RETURN is RETURN.  This may change....
 * Characters that 'cannot happen' are mapped to 'X'==0x58, to ferret out bugs.
 * Break is well known as 0x80, disconnect as 0x81.
 * This map is done at interrupt level time, so is always applied.
 */
unsigned char defkeymap[]={
/*80*/	0x58,	0x58,	0x58,	0x58,	0x58,	0x58,	0x58,	0x58,
	0x58,	0x58,	0x58,	0x58,	0x58,	0x58,	0x8e,	0x81,
/*90*/	0x90,	0x91,	0x92,	0x93,	0x94,	0x95,	0x96,	0x97,
	0x98,	0x99,	0x9a,	0x9b,	0x58,	0x58,	0x58,	0x58,
/*A0*/	0x58,	0xa1,	0xa2,	0xa3,	0xa4,	0xa5,	0xa6,	0xa7,
	0x58,	0x58,	0x58,	0x58,	0x58,	0x58,	0xae,	0x80,
/*B0*/	0xb0,	0xb1,	0x8f,	0xb3,	0xb4,	0xb5,	0x0a,	0xb7,
	0xb8,	0xb9,	0x00,	0xbb,	0x1e,	0xbd,	0x60,	0x1f,
/*C0*/	0xc0,	0xc1,	0xc2,	0xc3,	0xc4,	0x58,	0xc6,	0x0d,
	0xc8,	0xc9,	0xca,	0xcb,	0xcc,	0xcd,	0xce,	0xcf,
/*D0*/	0x09,	0x08,	0xd2,	0xd3,	0xd4,	0xd5,	0xd6,	0xd7,
	0x58,	0x58,	0x58,	0x58,	0x58,	0x58,	0x7f,	0x58,
/*E0*/	0x58,	0x58,	0xe2,	0x1b,	0x58,	0xe5,	0x58,	0x0d,
	0xe8,	0xe9,	0xea,	0xeb,	0xec,	0xed,	0xee,	0xef,
/*F0*/	0x09,	0x08,	0xb2,	0x1b,	0x58,	0xf5,	0x0a,	0x58,
	0x58,	0x58,	0x58,	0x58,	0x58,	0x58,	0x7f,	0x58,
};

kbdchar()
{
	return qgetc(&KBDQUEUE);
}
kbdinit()
{
	/* init	the keyboard */
	DUART->b_cmnd=RESET_RECV|DIS_TX|DIS_RX;
	DUART->b_cmnd=RESET_TRANS;
	DUART->b_cmnd=RESET_ERR;
	DUART->b_cmnd=RESET_MR;
	DUART->mr1_2b=CHAR_ERR|PAR_ENB|EVN_PAR|CBITS8;
	DUART->mr1_2b=NRML_MOD|ONEP000SB;
	DUART->b_sr_csr=BD4800BPS;
	DUART->b_cmnd=RESET_MR|ENB_TX|ENB_RX;
	DUART->scc_ropbc=0x08; /* set output pins for kbd tx port*/
	/* turn chirps on/off depending on BRAM */
	if(VALKEYTONE)
		kbdstatus=0;	/* no chirp */
	else
		kbdstatus=TTY_CHIRP;	/* chirp, chirp */
	DUART->b_data=kbdstatus|0x02; /* request status */
	latin1=latinchar=0;		/* latin-1 escape state */
}
auto2(){
	register unsigned s, c;
	s=DUART->b_sr_csr;
	c=DUART->b_data;
	if(s&(FRM_ERR|OVR_RUN)) 
		return;
	if(s&PAR_ERR){	/* control word: caps lock or repeat */
		checkbram();
		VALCAPS=(c&0x04)? 0 : 1; /* set the caps lock flag */
		setbram();
		if(c&0x10)	/* turn repeat off */
			kbdrepeat=rptcount=0;
		else		/* the next character is to be repeated */
			kbdrepeat=RPTON;
		/*
		 * Don't actually set the repeat bit until the character
		 * after the control code
		 */
	}else{		/* ordinary character */
		rptcount=0;	/* new char so restart repeat timer */
		if(c==0xF2){	/* shift to Latin-1 charset */
			latin1=1;
			latinchar=0;
			return;
		}
		if(c&0x80)
			c=defkeymap[c&0x7f];
		if(c==0x8E)	/* SHIFT-SETUP; show what's up */
			KBDQUEUE.state |= QPRIORITY;
		if(latin1&&latinchar) {
			int c1=mappair(latinchar,c);
			if(c1==c)
				qputc(&KBDQUEUE, latinchar);
			else
				c=c1;
			latin1=latinchar=0;
		}
		if(latin1){
			latinchar=c;
			return;
		}
		qputc(&KBDQUEUE, (int)c);
		KBDQUEUE.state &= ~QPRIORITY;
		if((kbdrepeat&RPTMASK) == RPTON){
			kbdrepeat=RPTHAVECHR|RPTON;
			kbdrepeat|=c;
		}
	}
}
kbdrpt(){
	qputc(&KBDQUEUE, kbdrepeat&0xff);
}
/*
 * table that maps character-pairs to extended characters in
 * Latin-1 character set
 */
struct	ditab {
	unsigned char pair[2];
	unsigned char val;
} ditab[] = {
	"!!", 0241,	/* spanish initial ! */
	"c|", 0242,	/* cents */
	"c$", 0242,
	"c/", 0242,
	"l$", 0243,	/* pound sterling */
	"L-", 0243,
	"g$", 0244,	/* general currency */
	"xo", 0244,
	"y$", 0245,	/* yen */
	"Y-", 0245,
	"j$", 0245,
	"||", 0246,	/* broken vertical bar */
	"/^", 0246,
	"SS", 0247,	/* section symbol */
	"so", 0247,
	"\"\"", 0250,	/* dieresis */
	"cr", 0251,	/* copyright */
	"cO", 0251,
	"co", 0251,
	"sa", 0252,	/* super a, feminine ordinal */
	"a_", 0252,
	"<<", 0253,	/* left angle quotation */
	"no", 0254,	/* not sign, hooked overbar */
	"-,", 0254,
	"--", 0255,	/* soft hyphen */
	"rg", 0256,	/* registered trademark */
	"ro", 0256,
	"__", 0257,	/* macron */
	"-^", 0257,
	"s0", 0260,	/* degree (sup o) */
	"0^", 0260,
	"+-", 0261,	/* plus-minus */
	"s2", 0262,	/* sup 2 */
	"2^", 0262,
	"s3", 0263,	/* sup 3 */
	"3^", 0263,
	"''", 0264,	/* acute accent */
	"\\'", 0264,
	"mu", 0265,	/* mu */
	"/u", 0265,
	"pg", 0266,	/* paragraph (pilcrow) */
	"P!", 0266,
	"..", 0267,	/* centered . */
	"^.", 0267,
	",,", 0270,	/* cedilla */
	"s1", 0271,	/* sup 1 */
	"1^", 0271,
	"so", 0272,	/* sup o */
	"o_", 0272,
	">>", 0273,	/* right angle quotation */
	"14", 0274,	/* 1/4 */
	"12", 0275,	/* 1/2 */
	"34", 0276,	/* 3/4 */
	"??", 0277,	/* spanish initial ? */
	"A`", 0300,	/* A grave */
	"A'", 0301,	/* A acute */
	"A^", 0302,	/* A circumflex */
	"A~", 0303,	/* A tilde */
	"A\"", 0304,	/* A dieresis */
	"A:", 0304,
	"Ao", 0305,	/* A circle */
	"AO", 0305,
	"A*", 0305,
	"Ae", 0306,	/* AE ligature */
	"AE", 0306,
	"C,", 0307,	/* C cedilla */
	"E`", 0310,	/* E grave */
	"E'", 0311,	/* E acute */
	"E^", 0312,	/* E circumflex */
	"E\"", 0313,	/* E dieresis */
	"E:", 0313,
	"I`", 0314,	/* I grave */
	"I'", 0315,	/* I acute */
	"I^", 0316,	/* I circumflex */
	"I\"", 0317,	/* I dieresis */
	"I:", 0317,
	"D-", 0320,	/* Eth */
	"N~", 0321,	/* N tilde */
	"O`", 0322,	/* O grave */
	"O'", 0323,	/* O acute */
	"O^", 0324,	/* O circumflex */
	"O~", 0325,	/* O tilde */
	"O\"", 0326,	/* O dieresis */
	"OE", 0326,
	"Oe", 0326,
	"O:", 0326,
	"xx", 0327,	/* times sign */
	"O/", 0330,	/* O slash */
	"U`", 0331,	/* U grave */
	"U'", 0332,	/* U acute */
	"U^", 0333,	/* U circumflex */
	"U\"", 0334,	/* U dieresis */
	"U:", 0334,
	"Ue", 0334,
	"UE", 0334,
	"Y'", 0335,	/* Y acute */
	"P|", 0336,	/* Thorn */
	"Th", 0336,
	"TH", 0336,
	"ss", 0337,	/* sharp s */
	"a`", 0340,	/* a grave */
	"a'", 0341,	/* a acute */
	"a^", 0342,	/* a circumflex */
	"a~", 0343,	/* a tilde */
	"a\"", 0344,	/* a dieresis */
	"a:", 0344,
	"ao", 0345,	/* a circle */
	"a*", 0345,
	"ae", 0346,	/* ae ligature */
	"c,", 0347,	/* c cedilla */
	"e`", 0350,	/* e grave */
	"e'", 0351,	/* e acute */
	"e^", 0352,	/* e circumflex */
	"e\"", 0353,	/* e dieresis */
	"e:", 0353,
	"i`", 0354,	/* i grave */
	"i'", 0355,	/* i acute */
	"i^", 0356,	/* i circumflex */
	"i\"", 0357,	/* i dieresis */
	"i:", 0357,
	"d-", 0360,	/* eth */
	"n~", 0361,	/* n tilde */
	"o`", 0362,	/* o grave */
	"o'", 0363,	/* o acute */
	"o^", 0364,	/* o circumflex */
	"o~", 0365,	/* o tilde */
	"o\"", 0366,	/* o dieresis */
	"o:", 0366,
	"oe", 0366,
	"-:", 0367,	/* divide sign */
	"o/", 0370,	/* o slash */
	"u`", 0371,	/* u grave */
	"u'", 0372,	/* u acute */
	"u^", 0373,	/* u circumflex */
	"u\"", 0374,	/* u dieresis */
	"u:", 0374,
	"ue", 0374,
	"y'", 0375,	/* y acute */
	"p|", 0376,	/* thorn */
	"th", 0376,
	"y\"", 0377,	/* y dieresis */
	"y:", 0377,
	0,
};

mappair(c0, c1)
register c0, c1;
{
	register struct ditab *dp;

	for (dp=ditab; dp->pair[0]; dp++) {
		if (c0==dp->pair[0] && c1==dp->pair[1]
		 || c0==dp->pair[1] && c1==dp->pair[0])
			return(dp->val);
	}
	return(c1);
}
