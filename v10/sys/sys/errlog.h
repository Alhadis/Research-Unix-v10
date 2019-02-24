/*
 * error log record
 */

#define	MAXEDATA	112

struct errhdr {
	char e_1magic, e_2magic;
	char e_len;	/* how many bytes of data are valid */
	char e_unit;	/* subdevice number */
	time_t e_time;	/* when error happened */
	char e_dev[8];	/* device (or other) name */
};

struct errlog {
	struct errhdr e_hdr;
	char e_data[MAXEDATA];
};

#define	E_HARD	0200	/* e_len -- unrecovered error */
#define	E_LEN	0177	/* e_len -- the real length */
#define	E_1MAGIC 0247
#define	E_2MAGIC 0145
