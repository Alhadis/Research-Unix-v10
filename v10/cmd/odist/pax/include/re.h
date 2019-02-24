/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * regular expression library definitions
 */

#ifndef RE_ALL

#define RE_ALL		(1<<0)	/* substitute all occurrences		*/
#define RE_EDSTYLE	(1<<1)	/* ed(1) style magic characters		*/
#define RE_LOWER	(1<<2)	/* substitute to lower case		*/
#define RE_MATCH	(1<<3)	/* record matches in reprogram.match	*/
#define RE_UPPER	(1<<4)	/* substitute to upper case		*/
#define RE_EXTERNAL	8	/* first external flag bit		*/

#define RE_NMATCH	('9'-'0'+1)

typedef struct			/* sub-expression match			*/
{
	char*	sp;		/* start in source string		*/
	char*	ep;		/* end in source string			*/
} rematch;

/*
 * NOTE: reprogram is a pun for the interface routines
 *	 allowing the library to change without forcing
 *	 users to recompile
 */

typedef struct			/* compiled regular expression program	*/
{
	rematch	match[RE_NMATCH+1];/* sub-expression match table	*/
} reprogram;

/*
 * interface routines
 */

#if __STDC__ || __cplusplus || c_plusplus
#if __cplusplus
extern "C" {
#endif
extern reprogram*	recomp(const char*, int);
extern int		reexec(reprogram*, const char*);
extern void		refree(reprogram*);
extern void		reerror(const char*);
extern char*		resub(reprogram*, const char*, const char*, char*, int);
#if __cplusplus
}
#endif
#else
extern reprogram*	recomp();
extern int		reexec();
extern void		refree();
extern void		reerror();
extern char*		resub();
#endif

#endif
