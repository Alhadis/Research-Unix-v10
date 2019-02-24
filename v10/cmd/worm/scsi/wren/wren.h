typedef int (*wfn)(ClientData, int, char **);

typedef struct
{
	char *ident;
	char *desc;
	wfn msense;
	wfn mselect;
	wfn diag;
	wfn lsense;
	wfn lselect;
} Wren;

extern Wren wr_elite;
extern Wren wr_wren5;

typedef struct
{
	char *name;
	int byteoff;
	int bitoff;
	int len;		/* in bits */
	long nval;		/* new value */
} Field;

extern int wr_mpage(int pcf, int page, Field **fields, char *err);

typedef struct
{
	char *name;
	int page;
	Field fields[10];
} Page;
