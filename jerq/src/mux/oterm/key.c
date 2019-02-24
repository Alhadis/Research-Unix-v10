#include	"map.h"
#include	"/usr/jerq/include/keymap.h"

static kmap[256] =
{
	0,	1,	2,	3,	4,	5,	6,	7,	/* 7 */
	8,	9,	10,	11,	12,	13,	14,	15,	/* 15 */
	16,	17,	18,	19,	20,	21,	22,	23,	/* 23 */
	24,	25,	26,	27,	28,	29,	30,	31,	/* 31 */
	' ',	UMLAUT,	')',	'<',	LE,	'=',	'>',	']',	/* ' */
	OR,	AND,	NE,	DIVIDE,	',',	'+',	'.',	'/',	/* / */
	'0',	'1',	'2',	'3',	'4',	'5',	'6',	'7',	/* 7 */
	'8',	'9',	'(',	'[',	';',	TIMES,	':',	'\\',	/* ? */
	'_',	ALPHA,	BASE,	CAP,	FLOOR,	EPSILON, '_',	DEL,	/* G */
	DELTA,	IOTA,	JOT,	'\'',	QUAD,	'|',	TOP,	CIRCLE,	/* O */
	'*',	'?',	RHO,	CEILING, '~',	DARROW,	CUP,	OMEGA,	/* W */
	RSHOE,	UARROW,	LSHOE,	LARROW,	'}',	LTACK,	GE,	'-',	/* _ */
	DIAMOND, 'a',	'b',	'c',	'd',	'e',	'f',	'g',	/* g */
	'h',	'i',	'j',	'k',	'l',	'm',	'n',	'o',	/* o */
	'p',	'q',	'r',	's',	't',	'u',	'v',	'w',	/* w */
	'x',	'y',	'z',	RARROW,	'{',	RTACK,	'$',	'',	/*  */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 135 */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 143 */
	0,	0,	0,	0,	LDEL,	IBEAM,	0,	SLASHB,	/* 151 */
	BSLASHB, DOMINO, 0,	0,	0,	0,	0,	0,	/* 159 */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 167 */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 175 */
	0,	0,	254,	0,	0,	0,	0,	0,	/* 183 */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 191 */
	THETA,	PHI,	NAND,	QQUAD,	NOR,	0,	ON,	0,	/* 199 */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 207 */
	0,	0,	LAMP,	HYDRANT, CENT,	THORN,	MATCH,	OVER,	/* 215 */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 223 */
	0,	0,	0,	0,	0,	64,	0,	0,	/* 231 */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 239 */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 247 */
	0,	0,	0,	0,	0,	0,	0,	0,	/* 255 */
};

mapchar(c)
{
	static lower = 1;

	if(c == 033)		/* escape */
	{
		lower = !lower;
		return(255);
	}
	if((c >= 'a') && (c <= 'z'))
		return(c - (lower? 0:32));
	return(kmap[c]);
}
