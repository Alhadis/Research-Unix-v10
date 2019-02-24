/*
/   This header file defines structures used by the Macro SPITBOL compiler
/   that are passed into the OS interface.
*/

/*
/   First, define the C type WORD to be the same size as a word used
/   by the Macro SPITBOL compiler.  The type of a WORD is a signed
/   integer for now.
*/

typedef int	WORD;

/*
/   CHFCB - chain of FCBs block
/
/   For every FCB created by OSINT, the compiler creates a CHFCB pointing
/   to the FCB and links it onto a chain of CHFCBs.  At EOJ the head of this
/   CHFCB chain is passed to the interface function SYSEJ so that all files
/   can be closed.
*/

struct	chfcb
{
		WORD	typ;		/*  type word			*/
		WORD	len;		/*  block length		*/
		struct	chfcb	*nxt;	/*  pointer to next chfcb	*/
		struct	fcblk	*fcp;	/*  pointer to fcb		*/
};

/*
/   ICBLK - integer block
/
/   Integer values are stored in ICBLKs.  Field icval should be defined
/   to be the appropriate type for the implementation.
*/

struct	icblk
{
		WORD	typ;		/*  type word - b$icl		*/
		WORD	val;		/*  integer value		*/
};

/*
/   SCBLK - string block
/
/   String values are stored in SCBLKs.  Notice that the scstr field
/   is defined as an array of characters of length 1.  This is a slight
/   kludge to side-step C's lack of support for varying length strings.
/
/   The actual length of a SCBLK is 2 words + the number of words necessary
/   to hold a string of length sclen.
*/

struct	scblk
{
		WORD	typ;		/*  type word - b$scl		*/
		WORD	len;		/*  string length		*/
		char	str[1];		/*  string characters		*/
};
