/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support definitions
 */

#ifndef TM_DEFAULT

#define tm_data		_tm_data_
#define tm_info		_tm_info_

#include <time.h>
#ifndef LS_USER
#include <sys/types.h>
#endif

#define tmset()		do{if(!tm_info.local.standard)tminit();}while(0)

#ifndef VOID
#define VOID	char
#endif

#define TM_ADJUST	(1<<0)		/* local doesn't do leap secs	*/
#define TM_LEAP		(1<<1)		/* do leap seconds		*/
#define TM_UTC		(1<<2)		/* universal coordinated ref	*/

#define TM_DST		(-60)		/* default minutes for DST	*/
#define TM_LOCALZONE	(25 * 60)	/* use local time zone offset	*/
#define TM_MAXLEAP	1		/* max leap secs per leap	*/

/*
 * these indices must agree with tm_dform[]
 */

#define TM_MONTH_3	0
#define TM_MONTH	12
#define TM_DAY_3	24
#define TM_DAY		31
#define TM_TIME		38
#define TM_DATE		39
#define TM_DEFAULT	40
#define TM_MERIDIAN	41

#define TM_UT		43
#define TM_DT		47
#define TM_SUFFIXES	51
#define TM_PARTS	55
#define TM_HOURS	62
#define TM_DAYS		66
#define TM_LAST		69
#define TM_THIS		72
#define TM_NEXT		75
#define TM_EXACT	78
#define TM_NOISE	81

#define TM_NFORM	85

struct tl				/* leap second info		*/
{
	time_t		time;		/* the leap second event	*/
	int		total;		/* inclusive total since epoch	*/
};

struct tz				/* time zone info		*/
{
	char*		type;		/* type name			*/
	char*		standard;	/* standard time name		*/
	char*		daylight;	/* daylight or summertime name	*/
	short		west;		/* minutes west of GMT		*/
	short		dst;		/* add to tz.west for DST	*/
};

struct tmdata				/* tm library readonly data	*/
{
	char**		format;		/* default TM_* format strings	*/
	char*		lex;		/* format lex type classes	*/
	char*		digit;		/* output digits		*/
	short*		days;		/* days in month i		*/
	short*		sum;		/* days in months before i	*/
	struct tl*	leap;		/* leap second table		*/
	struct tz*	zone;		/* alternate timezone table	*/
};

struct tminfo				/* tm library global info	*/
{
	char*		deformat;	/* TM_DEFAULT override		*/
	int		flags;		/* flags			*/
	char**		format;		/* current format strings	*/
	struct tz	local;		/* local timezone		*/
};

#if __cplusplus
extern "C" {
#endif

extern struct tmdata	tm_data;
extern struct tminfo	tm_info;

#if __cplusplus
}
#endif

#if __STDC__ || __cplusplus || c_plusplus
#if __cplusplus
extern "C" {
#endif
extern time_t		time(time_t*);

extern time_t		tmdate(const char*, char**, time_t*);
extern struct tm*	tmfix(struct tm*);
extern char*		tmform(char*, const char*, time_t*);
extern int		tmgoff(const char*, char**, int);
extern void		tminit(void);
extern time_t		tmleap(time_t*);
extern int		tmlex(const char*, char**, char**, int, char**, int);
extern struct tm*	tmmake(time_t*);
extern char*		tmpoff(char*, const char*, int, int);
extern time_t		tmtime(struct tm*, int);
extern char*		tmtype(const char*, char**);
extern int		tmword(const char*, char**, const char*, char**, int);
extern int		tmzone(const char*, char**, const char*, int*);
#if __cplusplus
}
#endif
#else
extern time_t		time();

extern time_t		tmdate();
extern struct tm*	tmfix();
extern char*		tmform();
extern int		tmgoff();
extern void		tminit();
extern time_t		tmleap();
extern int		tmlex();
extern struct tm*	tmmake();
extern char*		tmpoff();
extern time_t		tmtime();
extern char*		tmtype();
extern int		tmword();
extern int		tmzone();
#endif

#endif
