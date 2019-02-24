#include <time.h>
#include <libc.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <ctype.h>

static
reduce(fp,n)
int *fp;
{
	int val = 0;
	for( ; *fp<0;  *fp+=n)
		val--;
	val += *fp/n;
	*fp %= n;
	return val;
}

static char Month[][3] = {"jan","feb","mar","apr","may","jun",
		"jul","aug","sep","oct","nov","dec","\0"};
static char Zone[][3] = {"gmt","est","edt","cst","cdt",
		"mst","mdt","pst","pdt","\0"};
static int Diff[] = {0,5*3600,4*3600,6*3600,5*3600,7*3600,6*3600,8*3600,7*3600};
static int mday[] = {0,31,59,90,120,151,181,212,243,273,304,334};

long timegm(tp)
struct tm *tp;
{
	int n;
	struct tm tm;
	tm = *tp;
	tm.tm_year += reduce(&tm.tm_mon, 12);
	tm.tm_yday += mday[tm.tm_mon] + tm.tm_mday
		- ((tm.tm_year%4!=0)|(tm.tm_mon<2));
	tm.tm_min += reduce(&tm.tm_sec, 60);
	tm.tm_hour += reduce(&tm.tm_min, 60);
	tm.tm_yday += reduce(&tm.tm_hour, 24);
	tm.tm_year += 4*reduce(&tm.tm_yday, 4*365+1);
	while(tm.tm_yday >= (n = 365+((tm.tm_year%4)==0)))
		tm.tm_yday -= n, tm.tm_year++;
	return	tm.tm_sec + 60*(tm.tm_min +
		60*(tm.tm_hour + 24L*(tm.tm_yday +
		365*(tm.tm_year-70) + (tm.tm_year-69)/4)));
}

long timelocal(tp,zone)
struct tm *tp;
char *zone;
{
	struct timeb systime;
	long date;
	date = timegm(tp);
	if(zone==0) {
		ftime(&systime);
		date += systime.timezone*60L;
		if(systime.dstflag &&
		   localtime(&date)->tm_isdst)
			date -= 3600;
	} else 
		date += Diff[serialno(zone,Zone)];
	return date;
}

static char *breaks = "\t ,:\n";

static char *
skipfld(s)
char *s;
{
	char *t = strpbrk(s,breaks);
	return t!=0? t: s+strlen(s);
}

static char*
skipbrk(s)
char *s;
{
	return s + strspn(s,breaks);
}

long
timec(s)
char *s;
{
	char *t;
	struct tm tm;
	char *zone = 0;
	tm.tm_sec = tm.tm_min = tm.tm_hour = 0;
	tm.tm_mon = tm.tm_mday = tm.tm_yday = 0;
	for(s=skipbrk(s); isalpha(*s); s=skipbrk(skipfld(s)))
		tm.tm_mon = serialno(s,Month);
	tm.tm_mday = atoi(s);
	s = skipbrk(skipfld(s));
	t = skipfld(s);
	if(*t==':') {
		tm.tm_hour = atoi(s);
		tm.tm_min = atoi(++t);
		t = skipfld(t);
		if(*t==':') {
			tm.tm_sec = atoi(++t);
			t = skipfld(t);
		}
		s = skipbrk(t);
	}
	tm.tm_year = atoi(s);
	if(tm.tm_year!=0)
		s = skipbrk(skipfld(s));
	if(isalpha(*s)) {
		zone = s;
		s = skipbrk(skipfld(s));
	}
	if(tm.tm_year==0)
		tm.tm_year = atoi(s);
	tm.tm_year -= 1900;
	if(tm.tm_year < 0) {
		long clock;
		time(&clock);
		tm.tm_year = localtime(&clock)->tm_year;
		if(timelocal(&tm,zone) > clock)
			tm.tm_year--;
	}
	return timelocal(&tm,zone);
}

static
serialno(item,list)
char *item, list[][3];
{
	register i,j;
	for(i=0; list[i][0]; i++)
		for(j=0; j<3; j++)
			if(tolower(item[j])!=list[i][j])
				break;
			else if(j==2)
				return i;
	return 0;
}
