#define		JUKEDIR		"/usr/worm/jukedir"
#define		UNALLOCATED	"<unallocated>"
#define		NONAME		"<no name??>"

extern int j_wrshelf;

/* general functions */
extern j_wrshelves(Jukebox *, char *);
extern j_rdshelves(Jukebox *, char *);
extern char *j_name(Jukebox *, int);
extern void j_init(Jukebox *);
extern char *strdup(char *);
extern j_cold(Jukebox *, char *, char *);
extern j_warm(Jukebox *, char *);
extern int j_rvolid(int, char *);
extern int j_wvolid(int, char *, char *);
extern getvol(int sh, int drive, char *vol_id, int *side);
extern j_shelfof(Jukebox *j, char *vol_id);
extern j_driveof(Jukebox *j, char *vol_id);
extern j_load(Jukebox *j, char *vol_id, char *buf, long tlimit);
extern allocate(Jukebox *j, char *vol_id, char *buf);
