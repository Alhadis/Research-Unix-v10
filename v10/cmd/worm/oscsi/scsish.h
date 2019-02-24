typedef int (*Functionfn)(int, int *, int, char **, char *);

typedef struct
{
	char *name;
	char *help;
	char *param;
	Functionfn fn;
} Function;

typedef struct
{
	char *name;
	char *verbose;
	void (*extsense)(uchar *, char *, int);
	Function *fns;
} Device;
extern void setdevice(Device *);

extern void scsi_target(int);
extern void fixedstr(uchar *src, int len, char *dest);
extern void gen_extsense(uchar *, char *, int);
extern int shelfside(char *arg, char *err);
extern void xd(uchar *base, int, FILE *fp);
