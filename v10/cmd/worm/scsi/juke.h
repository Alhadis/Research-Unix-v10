#define	NLUN	8
#define	NSHELF	50
extern int nlun;
extern void setnlun(void);
extern char *j_shelf[NSHELF];
extern int j_wrshelf;			/* need to write out shelves */
extern j_rdshelves(char *err);
extern j_wrshelves(char *err);
extern j_inventory(char cold, int tlim, char *err);

typedef enum { SIDEA = 0, SIDEB = 1 } Side;

struct Lunstatus
{
	unsigned int poweron:1;		/* is power on ? */
	unsigned int diskin:1;		/* is disk in drive? */
	unsigned int ready:1;		/* is disk spun up or spun down? */
	unsigned int writeprotect:1;	/* is disk write protected? */
	unsigned int diskindrive:1;	/* is driveshelf a drive number? */
	unsigned int shelfvalid:1;	/* is retshelf valid? */
	uchar driveshelf;		/* drive number */
	uchar retshelf;			/* return shelf */
};

struct Jstatus
{	
	struct Lunstatus lun[NLUN]; /* disk status */
	uchar shelf[NSHELF];	/* shelf status */
	uchar iounit;		/* I/O unit status */
	uchar carrier;		/* carrier status */
	uchar udrive;		/* upper drive status */
	uchar ldrive;		/* lower drive status */
};
extern struct Jstatus j_status;
extern int j_getstatus(char *err);
extern int j_shelfof(char *vol_id);
extern int j_driveof(char *vol_id);

extern char *strdup(char *);
extern int j_shelf_to_drive(int, Side, int, char *);
extern int j_drive_to_shelf(int, int, Side, char *);
extern int j_empty_drive(long, char *);
extern int j_rvolid(int, char *);
extern int j_wvolid(int, char *, char *);
extern void pperror(char *buf, char *mesg);
extern int reserve_drive(int, char *);
extern int release_drive(int, char *);
extern int cold_inv(char, char *);
extern int warm_inv(char *);
extern int j_load(char *vol_id, char *buf, long tlim);
extern int j_unload(char *vol_id, char *buf);

#define		JUKEDIR		"/usr/worm/jukedir"
#define		UNALLOCATED	"<unallocated>"
