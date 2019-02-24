/* /usr/lib/calendar produces an egrep -f file
   that will select today's and tomorrow's
   calendar entries, with special weekend provisions

   used by calendar command
*/
#include <time.h>
#include <string.h>

#define DAY (3600*24L)
#define NHOL (sizeof holidays / sizeof *holidays)

char *month[] = {
	"[Jj]an",
	"[Ff]eb",
	"[Mm]ar",
	"[Aa]pr",
	"[Mm]ay",
	"[Jj]un",
	"[Jj]ul",
	"[Aa]ug",
	"[Ss]ep",
	"[Oo]ct",
	"[Nn]ov",
	"[Dd]ec"
};
/* Table of holidays gives inclusive months, inclusive
 * days of month, and inclusive days of week on which a
 * holiday may fall. Thus Thanksgiving, 4th Thursday of November,
 * may happen as early as Nov 22 and as late as Nov 28.
 * American holidays celebrated in more than two states
 * are in table, but most are commented out as irrelevant to
 * the schedule of the author's place of work
*/
struct hol {
	int h_mon0, h_mon1;
	int h_mday0, h_mday1;
	int h_wday0, h_wday1;
} holidays[] = {
	1,12,	1,31,	6,6,	/* sat */
	1,12,	1,31,	0,0,	/* sun */
	1,1,	1,1,	0,6,	/* new year's */
/*	1,1,	8,14,	0,0,	/* martin luther king's birthday */
/*	1,1,	15,15,	0,6,	/* martin luther king's birthday */
/*	1,1,	15,21,	1,1,	/* martin luther king's birthday */
/*	1,1,	19,19,	0,6,	/* robert e lee's birthday */
/*	1,1,	15,21,	1,1,	/* robert e lee's birthday */
/*	2,2,	1,7,	1,1,	/* lincoln's birthday */
/*	2,2,	12,12,	0,6,	/* lincoln's birthday */
/*	2,2,	15,21,	1,1,	/* washington's birthday */
/*	2,2,	22,22,	0,6,	/* washington's birthday */
/*	3,4,	??,??,	5,5,	/* good fri, beyond calculation */
	5,5,	25,31,	1,1,	/* memorial day */
/*	5,5,	30,30,	0,6,	/* memorial day */
/*	6,6,	1,7,	1,1,	/* jefferson davis's birthday */
/*	6,6,	3,3,	0,6,	/* jefferson davis's birthday */
	7,7,	3,3,	6,6,	/* fri before sat july 4th */
	7,7,	4,4,	0,6,	/* independence day */
	7,7,	5,5,	1,1,	/* mon after sun july 4th */
	9,9,	1,7,	1,1,	/* labor day */
/*	10,10,	8,14,	1,1,	/* columbus day */
/*	10,10,	12,12,	0,6,	/* columbus day */
/*	10,10,	22,28,	1,1,	/* veterans' day */
/*	11,11,	2,8,	2,2,	/* election day */
/*	11,11,	11,11,	0,6,	/* veterans' day */
	11,11,	22,28,	4,4,	/* thanksgiving */
	11,11,	23,29,	5,5,	/* fri after thanksgiving */
	12,12,	24,24,	1,1,	/* mon before tue christmas */
	12,12,	25,25,	0,6,	/* christmas */
	12,12,	26,26,	5,5,	/* fri after thu christmas */
	12,12,	31,31,	1,1,	/* mon before tue new years */
};
struct tm *localtime();
char *ctime();
char *date1, *date2;
#define year(date) (&date[20])
int hflg = 0;

main(argc,argv)
char **argv;
{
	long t;
	int  hidigit, lastday;
	struct tm ctm, ntm;
	int horiz = argc>1? atoi(argv[1]): 2;
	if(horiz<=0) horiz = 1;
	time(&t);
	date1 = strdup(ctime(&t));
	printf("(^|[ \t(,;])(");
	for(ctm= *localtime(&t); horiz>0; ctm=ntm) {
		hidigit = ctm.tm_mday/10;
		ntm = ctm;
		while(horiz>0) {
			lastday = ntm.tm_mday;
			if(horiz>1 || !dayoff(&ntm))
				horiz--;
			t += DAY;
			ntm = *localtime(&t);
			if(ntm.tm_mday/10!=hidigit)
				break;
		}
		printf("((%s[^ ]* *|0*%d/|\\* */?)0*",
			month[ctm.tm_mon], ctm.tm_mon+1);
		if(hidigit!=0)
			printf("%d",hidigit);
		printf("[%d-%d])", ctm.tm_mday%10, lastday%10);
		if(horiz>0)
			printf("\n");
	}
	date2 = strdup(ctime(&t));
		/* good until year 2000, but sloppy across year ends */
	printf(")([^0-9,/ ]|$|/%c%c|",year(date1)[2],year(date1)[3]);
	printf("([, ] *([^189 ]|1([^9]|$)|19[^89]|(19)?($|");
	printf("%c%c|",year(date1)[2],year(date1)[3]);
	printf("%c%c|",year(date2)[2],year(date2)[3]);
	printf("[89]($|[^0-9])))))\n");
}

dayoff(tm)
register struct tm *tm;
{
	struct hol *hol;
	for(hol= holidays; hol<&holidays[NHOL]; hol++) {
		if(hol->h_mon0 <= tm->tm_mon+1 && 
		   tm->tm_mon+1 <= hol->h_mon1 &&
		   hol->h_mday0 <= tm->tm_mday && 
		   tm->tm_mday <= hol->h_mday1 &&
		   hol->h_wday0 <= tm->tm_wday && 
		   tm->tm_wday <= hol->h_wday1 )
		return(1);
	}
	return(0);
}
