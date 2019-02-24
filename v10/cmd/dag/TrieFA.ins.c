/* File - TrieFA.ins.c
 *
 *    This file contains code to be included in the scanner file using a
 * generated trie-based FA.
 */

#include "TrieFA.h"

#ifdef UNDERLINE
static long CharMask[28] = {
	0x0000001,	0x0000000,	0x0000004,	0x0000008,	// mask for _, unused, a and b.
	0x0000010,	0x0000020,	0x0000040,	0x0000080,	// mask for c, d, e and f.
	0x0000100,	0x0000200,	0x0000400,	0x0000800,	// mask for g, h, i and j.
	0x0001000,	0x0002000,	0x0004000,	0x0008000,	// mask for k, l, m and n.
	0x0010000,	0x0020000,	0x0040000,	0x0080000,	// mask for o, p, q and r.
	0x0100000,	0x0200000,	0x0400000,	0x0800000,	// mask for s, t, u and v.
	0x1000000,	0x2000000,  0x4000000,  0x8000000, 	// mask for w, x, y and z.
};

#define IN_MASK_RANGE(C)	(islower(C) || ((C) == '_'))
#define MASK_INDEX(C)	((C) - '_')

#else
static long CharMask[26] = {
	0x0000001,	0x0000002,	0x0000004,	0x0000008,	// mask for a, b, c and d.
	0x0000010,	0x0000020,	0x0000040,	0x0000080,	// mask for e, f, g and h
	0x0000100,	0x0000200,	0x0000400,	0x0000800,	// mask for i, j, k and l.
	0x0001000,	0x0002000,	0x0004000,	0x0008000,	// mask for m, n, o and p.
	0x0010000,	0x0020000,	0x0040000,	0x0080000,	// mask for q, r, s and t.
	0x0100000,	0x0200000,	0x0400000,	0x0800000,	// mask for u, v, w and x.
	0x1000000,	0x2000000							// mask for y and z.
};

#define IN_MASK_RANGE(C)	islower(C)
#define MASK_INDEX(C)		((C) - 'a')

#endif

static short		TFA_State;

/* TFA_Init:
 *
 *    Initialize the trie FA.
 */
inline void TFA_Init ()
{
	TFA_State = 0;

}

/* TFA_Advance:
 *
 *    Advance to the next state (or -1) on the lowercase letter c.  This should be an
 * inline routine, but the C++ implementation isn't advanced enough so we use a macro.
 */
#define TFA_Advance(C)	{															\
	char		c = C;																\
	if (TFA_State >= 0) {															\
		if (isupper(c))																\
			c = tolower(c);															\
		else if (! IN_MASK_RANGE(c)) {												\
			TFA_State = -1;															\
			goto TFA_done;															\
		}																			\
		if (TrieStateTbl[TFA_State].mask & CharMask[MASK_INDEX(c)]) {				\
			short		i = TrieStateTbl[TFA_State].trans_base;						\
			while (TrieTransTbl[i].c != c)											\
				i++;																\
			TFA_State = TrieTransTbl[i].next_state;									\
		}																			\
		else																		\
			TFA_State = -1;															\
	}																				\
  TFA_done:;																		\
} /* end of TFA_Advance. */

/* TFA_Definition:
 *
 *    Return the definition (if any) associated with the current state.
 */
inline int TFA_Definition ()
{
	return (TFA_State < 0) ? -1 : TrieStateTbl[TFA_State].def;

}
