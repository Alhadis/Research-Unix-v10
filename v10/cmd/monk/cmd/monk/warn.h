#define	WARN_ME		"/tmp/mesgs.monk."	/* messages for monk manager */
#define	WARN_DB		"/tmp/mesgs.db."	/* messages for db manager */

#define	PR_NOTHING	0000
#define	PR_FILENAME	0001
#define	PR_LINENUMBER	0002

#define	INCREMENT	1
#define	DECREMENT	0

struct filestack {
	char	*name;
	short	line_number;
	struct filestack	*previous;
};

extern struct filestack	*filestack;
