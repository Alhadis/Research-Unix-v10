#define		SIDEA		0
#define		SIDEB		1

typedef struct {
	int nshelves;
	int nluns;		/* address range of luns */
	int ndrives;		/* physical drives */
	int nworms;		/* number of drives visible in namespace */
	struct lun {
		char occupied;
		char spunup;
		int shelf;
		int side;
		char *desc;
	} *luns;
	char *shelves;		/* set or not depending if a disk is there */
	char **names;
} Jukebox;
extern j_config(Jukebox *, char *);
extern j_drstatus(Jukebox *, char *);
extern j_shstatus(Jukebox *, char *);

/* general */
extern j_eject(int, char *);
extern j_sh_to_dr(int, int, int, char *);
extern j_dr_to_sh(int, int, int, char *);
extern j_start(int, char *);
extern j_stop(int, char *);
extern j_read(int, unsigned long, char *, int, char *);
extern j_write(int, unsigned long, char *, int, char *);
extern j_capacity(int, unsigned long *, unsigned long *);
extern j_load_unloaded(int, char *);
extern void j_reset(void);
