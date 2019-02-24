/* smtp constants and the like */

/* tunable constants */
#define MAXSTR 10240			/* maximum string length */
#define NAMSIZ MAXSTR			/* max file name length */

typedef struct namelist namelist;
struct namelist {
	namelist *next;
	char *name;
};
