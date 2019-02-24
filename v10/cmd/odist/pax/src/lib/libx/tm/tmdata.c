/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support readonly data
 */

#include <tm.h>

/*
 * default format strings -- must agree with TM_* indices
 */

static char*		format[] =
{
	"Jan",		"Feb",		"Mar",		"Apr",
	"May",		"Jun",		"Jul",		"Aug",
	"Sep",		"Oct",		"Nov",		"Dec",

	"January",	"February",	"March",	"April",
	"May",		"June",		"July",		"August",
	"September",	"October",	"November",	"December",

	"Sun",		"Mon",		"Tue",		"Wed",
	"Thu",		"Fri",		"Sat",

	"Sunday",	"Monday",	"Tuesday",	"Wednesday",
	"Thursday",	"Friday",	"Saturday",

	"%H:%M:%S",	"%m/%d/%y",	"%a %b %e %T %Z %Y",

	"AM",		"PM",

	"GMT",		"UTC",		"UCT",		"CUT",

	"DST",		"",		"",		"",

	"s",		"es",		"",		"",

	"second",	"minute",	"hour",		"day",
	"week",		"month",	"year",

	"midnight",	"morning",	"noon",		"evening",

	"yesterday",	"today",	"tomorrow",

	"last",		"ago",		"past",
	"this",		"now",		"current",
	"next",		"hence",	"coming",
	"exactly",	"",		"",

	"at",		"on",		"in",		"",
};

/*
 * format[] lex type classes
 */

static char		lex[] =
{
	TM_MONTH_3,	TM_MONTH_3,	TM_MONTH_3,	TM_MONTH_3,
	TM_MONTH_3,	TM_MONTH_3,	TM_MONTH_3,	TM_MONTH_3,
	TM_MONTH_3,	TM_MONTH_3,	TM_MONTH_3,	TM_MONTH_3,

	TM_MONTH,	TM_MONTH,	TM_MONTH,	TM_MONTH,
	TM_MONTH,	TM_MONTH,	TM_MONTH,	TM_MONTH,
	TM_MONTH,	TM_MONTH,	TM_MONTH,	TM_MONTH,

	TM_DAY_3,	TM_DAY_3,	TM_DAY_3,	TM_DAY_3,
	TM_DAY_3,	TM_DAY_3,	TM_DAY_3,

	TM_DAY,		TM_DAY,		TM_DAY,		TM_DAY,
	TM_DAY,		TM_DAY,		TM_DAY,

	TM_TIME,	TM_DATE,	TM_DEFAULT,

	TM_MERIDIAN,	TM_MERIDIAN,

	TM_UT,		TM_UT,		TM_UT,		TM_UT,
	TM_DT,		TM_DT,		TM_DT,		TM_DT,

	TM_SUFFIXES,	TM_SUFFIXES,	TM_SUFFIXES,	TM_SUFFIXES,

	TM_PARTS,	TM_PARTS,	TM_PARTS,	TM_PARTS,
	TM_PARTS,	TM_PARTS,	TM_PARTS,

	TM_HOURS,	TM_HOURS,	TM_HOURS,	TM_HOURS,

	TM_DAYS,	TM_DAYS,	TM_DAYS,

	TM_LAST,	TM_LAST,	TM_LAST,
	TM_THIS,	TM_THIS,	TM_THIS,
	TM_NEXT,	TM_NEXT,	TM_NEXT,
	TM_EXACT,	TM_EXACT,	TM_EXACT,

	TM_NOISE,	TM_NOISE,	TM_NOISE,	TM_NOISE,
};

/*
 * output format digits
 */

static char	digit[] = "0123456789";

/*
 * number of days in month i
 */

static short	days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*
 * sum of days in months before month i
 */

static short	sum[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

/*
 * leap second time_t and accumulated adjustments
 * (reverse order -- biased for recent dates)
 *
 * tl.time is the seconds since the epoch for the leap event
 *
 *	adding:		the first additional second
 *	subtracting:	the first dissappearing second
 */

static struct tl	leap[] =
{
	  662688015,   16,		/* Dec 31 23:59:60 GMT 1990 */
	  631152014,   15,		/* Dec 31 23:59:60 GMT 1989 */
	  567993613,   14,		/* Dec 31 23:59:60 GMT 1987 */
	  489024012,   13,		/* Jun 30 23:59:60 GMT 1985 */
	  425865611,   12,		/* Jun 30 23:59:60 GMT 1983 */
	  394329610,   11,		/* Jun 30 23:59:60 GMT 1982 */
	  362793609,   10,		/* Jun 30 23:59:60 GMT 1981 */
	  315532808,    9,		/* Dec 31 23:59:60 GMT 1979 */
	  283996807,    8,		/* Dec 31 23:59:60 GMT 1978 */
	  252460806,    7,		/* Dec 31 23:59:60 GMT 1977 */
	  220924805,    6,		/* Dec 31 23:59:60 GMT 1976 */
	  189302404,    5,		/* Dec 31 23:59:60 GMT 1975 */
	  157766403,    4,		/* Dec 31 23:59:60 GMT 1974 */
	  126230402,    3,		/* Dec 31 23:59:60 GMT 1973 */
	   94694401,    2,		/* Dec 31 23:59:60 GMT 1972 */
	   78796800,    1,		/* Jun 30 23:59:60 GMT 1972 */
		  0,    0,		/* can reference (tl+1)     */
		  0,    0
};

/*
 * time zones
 *
 * the UTC entries must be first
 *
 * zones with the same type are contiguous with all but the
 * first entry for the type having a null type
 *
 * tz.standard is the sentinel
 */

static struct tz	zone[] =
{
 0,	"GMT",	0,	 ( 0 * 60),	     0,	/* UTC			*/
 0,	"UCT",	0,	 ( 0 * 60),	     0,	/* UTC			*/
 0,	"UTC",	0,	 ( 0 * 60),	     0,	/* UTC			*/
 0,	"CUT",	0,	 ( 0 * 60),	     0,	/* UTC			*/
 "USA",	"HST",	0,	 (10 * 60),	     0,	/* Hawaii		*/
 0,	"YST",	"YDT",	 ( 9 * 60),	TM_DST,	/* Yukon		*/
 0,	"PST",	"PDT",	 ( 8 * 60),	TM_DST,	/* Pacific		*/
 0,	"PST",	"PPET",	 ( 8 * 60),	TM_DST,	/* Pacific pres elect	*/
 0,	"MST",	"MDT",	 ( 7 * 60),	TM_DST,	/* Mountain		*/
 0,	"CST",	"CDT",	 ( 6 * 60),	TM_DST,	/* Central		*/
 0,	"EST",	"EDT",	 ( 5 * 60),	TM_DST,	/* Eastern		*/
 "CAN",	"AST",	"ADT",	 ( 4 * 60),	TM_DST,	/* Atlantic		*/
 0,	"NST",	0,	 ( 3 * 60 + 30),     0,	/* Newfoundland		*/
 "GBR",	"",	"BST",	 ( 0 * 60),	TM_DST,	/* British Summer	*/
 "EUR",	"WET",	0,	 ( 0 * 60),	TM_DST,	/* Western Eurpoean	*/
 0,	"CET",	0,	-( 1 * 60),	TM_DST,	/* Central European	*/
 0,	"MET",	0,	-( 1 * 60),	TM_DST,	/* Middle European	*/
 0,	"EET",	0,	-( 2 * 60),	TM_DST,	/* Eastern Eurpoean	*/
 "ISR",	"IST",	"IDT",  -( 3 * 60),	TM_DST,	/* Israel		*/
 "CHN",	"HKT",	0,	-( 8 * 60),	     0,	/* Hong Kong		*/
 "KOR",	"KST",	"KDT",	-( 8 * 60),	TM_DST,	/* Korea		*/
 "SNG",	"SST",	0,	-( 8 * 60),	     0,	/* Singapore		*/
 "JPN",	"JST",	0,	-( 9 * 60),	     0,	/* Japan		*/
 "AUS",	"AWST",	0,	-( 8 * 60),	     0,	/* Australia Western	*/
 0,	"WST",	0,	-( 8 * 60),	     0,	/* Australia Western	*/
 0,	"ACST",	0,	-( 9 * 60 + 30),TM_DST,	/* Australia Central	*/
 0,	"CST",	0,	-( 9 * 60 + 30),TM_DST,	/* Australia Central	*/
 0,	"AEST",	0,	-(10 * 60),	TM_DST,	/* Australia Eastern	*/
 0,	"EST",	0,	-(10 * 60),	TM_DST,	/* Australia Eastern	*/
 "NZL",	"NZST",	"NZDT",	-(12 * 60),	TM_DST,	/* New Zealand		*/
 0,	0,	0,	0,		     0
};

struct tmdata tm_data = { format, lex, digit, days, sum, leap, zone };
