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
extern struct tm *localtime();
extern struct tm *gmtime();
extern char *asctime();
extern char *ctime();
extern long timec();
extern long timegm();
extern long timelocal();
extern char *timezone();
