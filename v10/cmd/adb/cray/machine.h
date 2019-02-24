#define	MAXFILE 0xffffffff

#define	watoba(x)	((x)*8)		/* word addr to byte addr */
#define	patoba(x)	((x)*2)		/* parcel addr to byte addr */
#define	batowa(x)	((x)/8)		/* byte addr to parcel addr */
#define	batopa(x)	((x)/2)		/* byte addr to parcel addr */
#define	waoff(x)	((x)&07)	/* byte offset */
