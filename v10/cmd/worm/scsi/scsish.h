struct ClientData
{
	char err[256];
};
typedef struct ClientData *ClientData;
#define	_CLIENTDATA

#define		ERR_RETURN	{ it->result = cd->err; return(TCL_ERROR); }
#define		USAGE_RETURN	{ it->result = "usage error"; return(TCL_ERROR); }

struct Tcl_Interp;
typedef int (*Functionfn)(ClientData, struct Tcl_Interp *, int, char **);

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

/*
 * argument processing
 */
#define	ARGBEGIN	for(argv0? 0: (argv0=*argv++,argc--);\
			    argv[0] && argv[0][0]=='-' && argv[0][1];\
			    argc--, argv++) {\
				char *_args, *_argt, _argc;\
				_args = &argv[0][1];\
				if(_args[0]=='-' && _args[1]==0){\
					argc--; argv++; break;\
				}\
				while(*_args) switch(_argc=*_args++)
#define	ARGEND		}
#define	ARGF()		(_argt=_args, _args="",\
				(*_argt? _argt: argv[1]? (argc--, *++argv): 0))
#define	ARGC()		_argc
extern char *argv0;
