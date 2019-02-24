
struct tm { /* see ctime(3) */
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
};

extern long time(long*);
extern char* ctime(long*);
extern tm* localtime(long*);
extern tm* gmtime(long*);
extern char* asctime(tm*);
/*
	extern long timezone;
	extern int daylight;
	extern char* tzname[2];
	void tzset();
*/
