/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>
#include <ctype.h>
#include <namval.h>

#define TM_type		(-1)

static struct namval	options[] =
{
	"adjust",	TM_ADJUST,
	"format",	TM_DEFAULT,
	"leap",		TM_LEAP,
	"type",		TM_type,
	"utc",		TM_UTC,
	0,		0
};

struct tminfo		tm_info;

char*			tzname[2];	/* POSIX time zone names	*/

#if __STDC__ || __cplusplus
#if __cplusplus
extern "C" {
#endif
static int		tzwest(time_t*, int*);
static int		tmopt(char*, struct namval*, int, const char*);

extern char*		getenv(const char*);
extern char*		strchr(const char*, int);
extern char*		strdup(const char*);
extern void		stropt(char*, struct namval*, int (*)(char*, struct namval*, int, const char*), char*);
#if __cplusplus
}
#endif
#else
static int		tzwest();
static int		tmopt();

extern char*		getenv();
extern char*		strchr();
extern char*		strdup();
#endif

/*
 * initialize tm data
 */

void
tminit()
{
	register struct tz*	zp;
	register int		n;
	register char*		s;
	int			i;
	int			m;
	int			isdst;
	char*			t;
	struct tm*		tp;
	time_t			now;
	char			buf[20];

	/*
	 * only do this stuff once
	 */

	if (tm_info.local.standard) return;

	/*
	 * tm_info.format language support goes here
	 */

	tm_info.format = tm_data.format;
	tm_info.deformat = tm_data.format[TM_DEFAULT];

	/*
	 * tm_info.local
	 */

	(void)time(&now);
	n = tzwest(&now, &isdst);

	/*
	 * compute local DST offset by roaming
	 * through the last 12 months until tzwest() changes
	 */

	for (i = 0; i < 12; i++)
	{
		now -= 31 * 24 * 60 * 60;
		if ((m = tzwest(&now, &isdst)) != n)
		{
			if (!isdst)
			{
				isdst = n;
				n = m;
				m = isdst;
			}
			m -= n;
			break;
		}
	}
	tm_info.local.west = n;
	tm_info.local.dst = m;

	/*
	 * now get the time zone names
	 */

	if (tzname[0])
	{
		/*
		 * POSIX
		 */

		tm_info.local.standard = tzname[0];
		tm_info.local.daylight = tzname[1];
	}
	else if ((s = getenv("TZNAME")) && *s && (s = strdup(s)))
	{
		/*
		 * BSD
		 */

		tm_info.local.standard = s;
		if (s = strchr(s, ',')) *s++ = 0;
		else s = "";
		tm_info.local.daylight = s;
	}
	else if ((s = getenv("TZ")) && *s && *s != ':' && (s = strdup(s)))
	{
		/*
		 * POSIX style but skipped by localtime()
		 */

		tm_info.local.standard = s;
		if (*++s && *++s && *++s)
		{
			*s++ = 0;
			(void)tmgoff(s, &t, 0);
			s = t;
		}
		else s = "";
		tm_info.local.daylight = s;
	}
	else
	{
		/*
		 * tm_data.zone table lookup
		 */

		t = 0;
		for (zp = &tm_data.zone[0]; zp->standard; zp++)
		{
			if (zp->type) t = zp->type;
			if (zp->west == n && zp->dst == m)
			{
				tm_info.local.type = t;
				tm_info.local.standard = zp->standard;
				if (!(s = zp->daylight))
				{
					s = tmpoff(buf, zp->standard, 0, 0);
					*s++ = ' ';
					(void)tmpoff(s, tm_info.format[TM_DT], m, TM_DST);
					s = strdup(buf);
				}
				tm_info.local.daylight = s;
				break;
			}
		}
		if (!zp->standard)
		{
			/*
			 * not in the table
			 */

			s = tmpoff(buf, tm_info.format[TM_UT], n, 0);
			tm_info.local.standard = strdup(buf);
			*s++ = ' ';
			(void)tmpoff(s, tm_info.format[TM_UT], m, TM_DST);
			tm_info.local.daylight = strdup(buf);
		}
	}

	/*
	 * set the options
	 */

	stropt(getenv("TM_OPTIONS"), options, tmopt, (char*)0);

	/*
	 * the time zone type is probably related to the locale
	 */

	if (!tm_info.local.type)
	{
		s = tm_info.local.standard;
		t = 0;
		for (zp = &tm_data.zone[0]; zp->standard; zp++)
		{
			if (zp->type) t = zp->type;
			if (tmword(s, (char**)0, zp->standard, (char**)0, 0))
			{
				tm_info.local.type = t;
				break;
			}
		}
	}

	/*
	 * tm_info.flags
	 */

	if (!(tm_info.flags & TM_ADJUST))
	{
		now = (time_t)78811200;		/* Jun 30 1972 23:59:60 */
		tp = (struct tm*)localtime(&now);
		if (tp->tm_sec != 60) tm_info.flags |= TM_ADJUST;
	}
	if (!(tm_info.flags & TM_UTC))
	{
		s = tm_info.local.standard;
		for (zp = &tm_data.zone[0]; !zp->type && zp->standard; zp++)
			if (tmword(s, (char**)0, zp->standard, (char**)0, 0))
			{
				tm_info.flags |= TM_UTC;
				break;
			}
	}
}

/*
 * stropt() option handler
 */

/*ARGSUSED*/
static int
#if __STDC__ || __cplusplus
tmopt(char* a, struct namval* p, int n, const char* v)
#else
tmopt(a, p, n, v)
char*		a;
struct namval*	p;
int		n;
char*		v;
#endif
{
	char*	s;

	switch (p->value)
	{
	case TM_DEFAULT:
		tm_info.deformat = (n && (n = strlen(v)) > 0 && (n < 2 || v[n-2] != '%' || v[n-1] != '?')) ? strdup(v) : tm_info.format[TM_DEFAULT];
		break;
	case TM_type:
		tm_info.local.type = (n && *v) ? ((s = tmtype(v, (char**)0)) ? s : strdup(v)) : 0;
		break;
	default:
		if (n) tm_info.flags |= p->value;
		else tm_info.flags &= ~p->value;
		break;
	}
	return(0);
}

/*
 * return minutes west of GMT for local time clock
 *
 * isdst will point to non-zero if DST is in effect
 * this routine also kicks in the local initialization
 */

static int
#if __STDC__ || __cplusplus
tzwest(time_t* clock, int* isdst)
#else
tzwest(clock, isdst)
time_t*	clock;
int*	isdst;
#endif
{
	register struct tm*	tp;
	register int		n;
	register int		m;
	int			h;

	/*
	 * convert to GMT assuming local time
	 */

	tp = (struct tm*)gmtime(clock);
	n = tp->tm_yday;
	h = tp->tm_hour;
	m = tp->tm_min;

	/*
	 * localtime() handles DST and GMT offset
	 */

	tp = (struct tm*)localtime(clock);
	if (n = tp->tm_yday - n)
	{
		if (n > 1) n = -1;
		else if (n < -1) n = 1;
	}
	*isdst = tp->tm_isdst;
	return((h - tp->tm_hour - n * 24) * 60 + m - tp->tm_min);
}
