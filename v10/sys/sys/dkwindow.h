/*  URP window-size negotiation.
 *
 *  The 16 bits of the window-size argument are defined as:
 *
 *  -----------------------------------------------------------------
 *  |valid|   spare   |   originator   |   destination  |   traffic |
 *  |     |           |   window size  |   window size  |           |
 *  -----------------------------------------------------------------
 *     1        3              4                4              4
 *
 *  The "valid" bit equals 1 to indicate that the window size values
 *  are specified.  Zero means they should be ignored and that default
 *  values must be used instead.  Old controllers that do not have
 *  window size negotiation happen to set this bit to zero, fortunately.
 *
 *  The actual window size is computed from the 4-bit window size 
 *  parameter (n) as 2**(n+4).  Thus, window sizes range from 16 to
 *  512K bytes.
 *
 *  The originator of a call sets his "originator window size" to be
 *  less than or equal to the size of his receive buffer, and he
 *  requests through the "destination window size" the size of the
 *  buffer that he would like to use for transmission.  As the call
 *  is being set up, trunk processes and the destination line process
 *  may lower either or both window sizes.  The resulting window sizes
 *  are returned to the originator in the call ANSWER message.
 */

#ifndef MIN
#define MIN(x,y)  (x < y ? x : y)
#endif
/* Format originator, destination, and traffic values */
#define WINDOW(o,d,t)  ((o<<8) | (d<<4) | t | 0100000)

#define W_VALID(x)  (x & 0100000)

#define W_ORIG(x)  ((x>>8) & 017)

#define W_DEST(x)  ((x>>4) & 017)

#define W_TRAF(x)  (x & 017)

#define W_DESTMAX(x,y)  (WINDOW(W_ORIG(x),MIN(W_DEST(x),y),W_TRAF(x)))

#define W_LIMIT(x,y)  (WINDOW(MIN(W_ORIG(x),y),MIN(W_DEST(x),y),W_TRAF(x)))

#define	W_VALUE(x)	(1<<(x+4))


#define WS_16	0
#define WS_32	1
#define WS_64	2
#define WS_128	3
#define WS_256	4
#define WS_512	5
#define WS_1K	6
#define WS_2K	7
#define WS_4K	8
#define WS_8K	9
#define WS_16K	10
#define WS_32K	11
#define WS_64K	12
#define WS_128K	13
#define WS_256K	14
#define WS_512K	15
