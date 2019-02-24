/*
 *
 * Rune declarations - for supporting UTF encoding.
 *
 */

#define RUNELIB		1

#ifdef RUNELIB
typedef unsigned short	Rune;

enum
{
	UTFmax		= 3,		/* maximum bytes per rune */
	Runesync	= 0x21,		/* cannot represent part of a utf sequence (<) */
	Runeself	= 0xA0,		/* rune and utf sequences are the same (<) */
	Runeerror	= 0x80,		/* decoding error in utf */
};
#endif
