/* Here is a bunch of stuff used for setup mode */
#define BRAM ((struct blop *) 0x600000)	/* address of battery backed ram */

#define BRKKEY	0x80
#define FUNC1KEY	0x82
#define FUNC2KEY	0x83
#define FUNC3KEY	0x84
#define FUNC4KEY	0x85
#define FUNC5KEY	0x86
#define FUNC6KEY	0x87
#define FUNC7KEY	0x88
#define FUNC8KEY	0x89

#define BACKSPCKEY	'\b'
#define SPACEKEY	' '
#define SETUPKEY	0xae
#define TABKEY		'\t'


#define BAUD	0
#define RETKEY	1
#define NEWLINE	2
#define DUPLEX	3
#define KEYTONE 4
#define SCREENCOLOR	5
#define PFKEYS	6
#define NUMSETUPS	7
#define NUMOPTS	6

#define CHECKSUM 2044

#define NUMBAUD		6
#define NUMRETKEYS	3
#define NUMNEWLINE	2
#define NUMDUPLEX	2
#define NUMKEYTONE	2
#define NUMSCREENCOLOR	2
#define NUMPFKEYS	8
#define PFKEYSIZE	50

#define VALBAUD		BRAM->opts.opt_array[BAUD].byte
#define VALRETKEY	BRAM->opts.opt_array[RETKEY].byte
#define VALNEWLINE	BRAM->opts.opt_array[NEWLINE].byte
#define VALDUPLEX	BRAM->opts.opt_array[DUPLEX].byte
#define VALKEYTONE	BRAM->opts.opt_array[KEYTONE].byte
#define VALSCREENCOLOR	BRAM->opts.opt_array[SCREENCOLOR].byte
#define VALCHECK1	BRAM->opts.opt_array[CHECKSUM].byte
#define VALCHECK2	BRAM->opts.opt_array[CHECKSUM+1].byte
#define VALCHECK3	BRAM->opts.opt_array[CHECKSUM+2].byte
#define VALCHECK4	BRAM->opts.opt_array[CHECKSUM+3].byte
#define VALCAPS		BRAM->capslock.byte
#define VALNUM		BRAM->numlock.byte
#define VALDWNLDFLAG	BRAM->dwnldflag.byte
#define VALMAXADDR	BRAM->bmaxaddr.byte

struct bram_mem {
	int:16;		unsigned char byte;
	int:8;
	};
struct blop {
	union blip{
		struct vals {
			int:16;		unsigned char baud;
			int:8;
			int:16;		unsigned char retkey;
			int:8;
			int:16;		unsigned char newline;
			int:8;
			int:16;		unsigned char duplex;
			int:8;
			int:16;		unsigned char keytone;
			int:8;
			int:16;		unsigned char screencolor;
			int:8;
			} individ;
		struct bram_mem opt_array[NUMOPTS];
		} opts;
	struct bram_mem pfkeys[NUMPFKEYS][PFKEYSIZE+1];	/* can init partly? */
	struct bram_mem numlock;		/* num lock flag */
	struct bram_mem capslock;		/* caps lock flag */
	struct bram_mem dwnldflag;		/* 0 = stand-alone, 1=program loaded */
	struct bram_mem bmaxaddr;		/* 0 = 256K Mem, 1 = 1Meg */
	} ;
#define SETUP_HEADINGS "SPEED     CR DEF    NL DEF    DUPLEX    TONE      BACKGROUND  (Hit pfkey to edit value)"

	/* next come the displayed values... all default values are first */
#ifdef SETUP_OWN
long maxaddr[2] = {0x740000, 0x800000}; /* last valid memory address */
/*short curpos[NUMSETUPS] = {0, 10, 20, 30, 40, 52, 61};	/* cursor positions:  */
						/* if this simple use formula */

char baud_speeds[NUMBAUD] = { (char) BD1200BPS, (char) BD2400BPS, (char) BD4800BPS,
			     (char) BD9600BPS, (char) BD19200BPS, (char) BD300BPS};
char *val_baud[NUMBAUD] = {" 1200"," 2400"," 4800"," 9600","19200","  300"};
char *val_retkey[NUMRETKEYS] = { "   CR", "   LF", "CR/LF" };
char *val_newline[NUMNEWLINE] = { "INDEX", "  NL " };
char *val_duplex[NUMDUPLEX] = { " FULL", " HALF" };
char *val_keytone[NUMKEYTONE] = { " ON", "OFF" };
char *val_screencolor[NUMSCREENCOLOR] = { " DARK", "LIGHT"};

char **val_indx[NUMOPTS]  = { val_baud, val_retkey,  val_newline, val_duplex,
				 val_keytone,  val_screencolor } ;

char val_maxes[NUMOPTS] = { (char) NUMBAUD, (char) NUMRETKEYS,
				(char) NUMNEWLINE, (char) NUMDUPLEX,
				(char) NUMKEYTONE, (char) NUMSCREENCOLOR };
#else
extern char baud_speeds[];
extern char *val_baud[];
extern char *val_retkey[];
extern char *val_newline[];
extern char *val_duplex[];
extern char *val_keytone[];
extern char *val_screencolor[];
extern char *val_pfkeys[];
extern char **val_indx[];
extern char val_maxes[];
/*extern short curpos[];*/
extern long maxaddr[];
#endif

/* QUESTION???? How do we determine factory default?  Is bram pre-setup, or
   somehow initially invalid?  How do we determine this?  write a "im ready"
   pattern somewhere in cmos? 	*/

/* Font properties */
#define	CW	9	/* width of a character */
#define	NS	14	/* newline size; height of a character */

#define	XMARGIN	3	/* inset from border */
#define	YMARGIN	3
