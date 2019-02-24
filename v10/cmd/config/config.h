/*
 * config.h	1.5	81/05/22
 * Definitions that everybody needs to know
 */

#define eq(a,b) (strcmp(a,b) == 0)
#define TRUE 1
#define FALSE 0
#define DRIVER 1
#define NORMAL 2
#define INVISIBLE 3

#define TO_NEXUS (struct device *)-1

#define STREAM_DEVICE 1
#define BLOCK_DEVICE 2
#define CHAR_DEVICE 3
#define LINE_DISC 4
#define FILE_SYS 5
#define NDEVTYPE 5

struct file_list {
	char *f_fn;
	int f_type;
	struct file_list *f_next;
	char *f_needs;
};

struct dev_list {
	char *dev_name;
	int dev_standard;
	int dev_type;
	int dev_num;
	long dev_mask;
	char *dev_if;
	char *dev_prefix;
	struct dev_list *dev_next;
};

struct	idlst {
	char *id;
	struct idlst *id_next;
};

struct	litlst {
	char *line;
	struct litlst *lit_next;
};

typedef char bool;

struct device {
	int d_type;			/* CONTROLLER, DEVICE, UBA or MBA */
	struct device *d_conn;		/* What it is connected to */
	char *d_name;			/* Name of device (e.g. rk11) */
	struct idlst *d_vec;		/* Interrupt vectors */
	int d_addr;			/* Address of csr */
	int d_unit;			/* Unit number */
	int d_drive;			/* Drive number */
	int d_slave;			/* Slave number */
#define QUES	-1	/* -1 means '?' */
#define	UNKNOWN -2	/* -2 means not set yet */
	bool d_dk;			/* if init 1 set to number for iostat */
	int d_flags;			/* Flags for device init */
	int d_count;			/* # for pseudo devs */
	struct device *d_next;		/* Next one in list */
};

struct config {
	char *c_dev;
	char *s_sysname;
};

struct cputype {
	char *cpu_name;
	struct cputype *cpu_next;
} *cputype;
struct opt {
	char *op_name;
	struct opt *op_next;
} *opt;
char *ident, *mkfile, *ns(), *malloc(), *tc(), *qu();
bool do_trace, seen_mba, seen_uba;
struct device *connect();
struct device *dtab;
char errbuf[80];
int yyline;
struct file_list *ftab, *conf_list, *confp;
int hz, timezone, hadtz, maxusers, dst;
struct dev_list *devtab;
struct litlst *littab;
int max_num[NDEVTYPE+1];

#define	WEOF	(char *)EOF
char *get_word();

#define	GLOBAL(file)	gpath(file)	/* file for all systems */
#define	LOCAL(file)	file		/* file for just our system */
char *gpath();
