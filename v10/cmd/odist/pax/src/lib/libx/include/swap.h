/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * internal representation conversion support definitions
 */

#ifndef BYTE

#define BYTE		01		/* swap bytes			*/
#define HALF		02		/* swap halfwords		*/

#define getbyte(op,p)	(*((unsigned char*)(p)))
#define putbyte(op,p,n)	(*(p)=(n))

#if __STDC__ || __cplusplus || c_plusplus
#if __cplusplus
extern "C" {
#endif
extern char*		memswap(int, char*, int);
extern int		gethalf(int, const char*);
extern long		getlong(int, const char*);
extern void		puthalf(int, char*, int);
extern void		putlong(int, char*, long);
#if __cplusplus
}
#endif
#else
extern char*		memswap();
extern int		gethalf();
extern long		getlong();
extern void		puthalf();
extern void		putlong();
#endif

#endif
