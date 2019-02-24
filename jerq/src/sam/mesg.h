#define	TBLOCKSIZE 512		  /* largest piece of text sent to terminal */
#define	DATASIZE  (TBLOCKSIZE+30) /* ... including protocol header stuff */
/*
 * Messages originating at the terminal
 */
typedef enum Tmesg{
	Tcut,
	Tpaste,
	Tsnarf,
	Ttype,		/* add some characters, but terminal already knows */
	Trequest,	/* request data to fill a hole */
	Tstartfile,	/* terminal just opened a file's frame */
	Tstartnewfile,	/* terminal just opened a new frame */
	Tstartcmdfile,	/* terminal just opened command frame */
	Tworkfile,	/* set file to which commands apply */
	Twrite,		/* write file */
	Tcheck,		/* ask host to poke with Hcheck */
	Torigin,	/* gimme an Horigin near here */
	Tclose,		/* terminal requests file close; check mod. status */
	Tsearch,	/* search for last regular expression */
	Tlook,		/* search for literal current text */
	Tsend,		/* pretend he typed stuff */
	Tunlockfile,	/* Hunlockfile me */
	Tstartsnarf,	/* initiate snarf buffer exchange */
	Tsetsnarf,	/* remember string in snarf buffer */
	Tdclick,	/* double click */
	Texit		/* exit */
}Tmesg;
/*
 * Messages originating at the host
 */
typedef enum Hmesg{
	Hnewname,	/* create "" name in menu */
	Hdelname,	/* delete file name from menu */
	Hmovname,	/* move file name in menu */
	Hbindname,	/* attach name[0] to text in terminal */
	Hcurrent,	/* make named file the typing file */
	Hgrow,		/* insert space in rasp */
	Hdata,		/* store this data in previously allocated space */
	Hgrowdata,	/* Hgrow + Hdata folded together */
	Hcut,		/* remove space from rasp */
	Hclean,		/* named file is now 'clean' */
	Hdirty,		/* named file is now 'dirty' */
	Hcheck0,	/* see below */
	Hcheck,		/* ask terminal to check whether it needs more data */
	Hmoveto,	/* scrolling, context search, etc. */
	Hsetdot,	/* set dot in terminal */
	Horigin,	/* set origin of file/frame in terminal */
	Hclose,		/* close file and remove from menu */
	Hunlock,	/* command is finished; user can do things */
	Hsetpat,	/* set remembered regular expression */
	Hunlockfile,	/* unlock file in terminal */
	Hsetsnarf,	/* remember string in snarf buffer */
	Hsnarflen,	/* report length of implicit snarf */
	Hexit
}Hmesg;
typedef struct Header{
	uchar	type;		/* one of the above */
	uchar	count0;		/* low bits of data size */
	uchar	count1;		/* high bits of data size */
	uchar	data[1];	/* variable size */
}Header;
/*
 * File transfer protocol schematic, a la Holzmann
 *	
 *	proc h
 *	{	pvar n = 0;
 *		queue h[4];
 *	
 *		do
 *		:: (n <  N)  -> n++; t!Hgrow
 *		:: (n == N)  -> n++; t!Hcheck0
 *		:: h?Trequest -> t!Hdata
 *		:: h?Tcheck  -> t!Hcheck
 *		od
 *	}
 *	proc t
 *	{	queue t[4];
 *		do
 *		:: t?Hgrow -> h!Trequest
 *		:: t?Hdata -> skip
 *		:: t?Hcheck0 -> h!Tcheck
 *		:: t?Hcheck ->
 *			if
 *			:: break
 *			:: h!Trequest; h!Tcheck
 *			fi
 *		od
 *	}
 */
