/*
	This file holds all the Bitmap controlling information.

	Bitmaps for the moving shapes are generated on demand.

*/

#include "pengo.h"

Bitmap	bit32 = {
	(Word *) 0,
	shorts(2),
	{ { 0, 0 } , { 32, 32} }
};

Bitmap	bit16 = {
	(Word *) 0,
#ifdef	JERQ
	shorts(2),
#define	MUL	32
#else	JERQ
	shorts(1),
#define	MUL	16
#endif	JERQ
	{ { 0, 0 } , { 16, 16 } }
};

Bitmap *
bchar(ch)	/* returns 16x16 Bitmap of char 'ch' */
char ch;
{

/* all that has to be done is assign bit16.base */

	if ((ch >= '0') && (ch <= '9'))
		bit16.base = (Word *)&char_0_9[((ch - '0') * MUL)];
	else if ((ch >= 'A') && (ch <= 'Z'))
		bit16.base = (Word *)&char_A_Z[((ch - 'A') * MUL)];
	else if ((ch >= 'a') && (ch <= 'z'))
		bit16.base = (Word *)&char_A_Z[((ch - 'a') * MUL)];
	else switch(ch)
	{
	case '.':
		bit16.base = (Word *)char_dot;
		break;

	case ':':
		bit16.base = (Word *)char_colon;
		break;

	case '?':
		bit16.base = (Word *)char_question;
		break;

	case ')':
		bit16.base = (Word *)char_rbrkt;
		break;

	case '(':
		bit16.base = (Word *)char_lbrkt;
		break;

	case '_':
		bit16.base = (Word *)char_uscore;
		break;

	case '/':
		bit16.base = (Word *)char_slash;
		break;
	
	default:
		bit16.base = (Word *)char_space;
		break;
	}
	return (&bit16);
}

