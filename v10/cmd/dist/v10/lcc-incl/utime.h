struct utimbuf {
	time_t actime;
	time_t modtime;
};

extern int utime(char *, struct utimbuf *);
