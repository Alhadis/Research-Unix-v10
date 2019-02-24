typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	struct	_physadr { int r[1]; } *physadr;
typedef	int	daddr_t;
typedef	char *	caddr_t;
typedef	u_short ino_t;
typedef	int	swblk_t;
typedef	int	size_t;
typedef	int	time_t;
typedef	int	label_t[14];
typedef	short	dev_t;
typedef	int	off_t;
typedef long	portid_t;
typedef struct	fd_set { int fds_bits[1]; } fd_set;
struct ar_hdr {
	char	ar_name[16];
	char	ar_date[12];
	char	ar_uid[6];
	char	ar_gid[6];
	char	ar_mode[8];
	char	ar_size[10];
	char	ar_fmag[2];
};
extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;
	char	*_base;
	short	_flag;
	char	_file;
} _iob[20];
struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*fgets();
extern char	_ctype[];
struct exec {
	long	a_magic;	
unsigned long	a_text;		
unsigned long	a_data;		
unsigned long	a_bss;		
unsigned long	a_syms;		
unsigned long	a_entry;	
unsigned long	a_trsize;	
unsigned long	a_drsize;	
};
struct relocation_info {
	int	r_address;	
unsigned int	r_symbolnum:24,	
		r_pcrel:1, 	
		r_length:2,	
		r_extern:1,	
		:4;		
};
struct	nlist {
	union {
		char	*n_name;	
		long	n_strx;		
	} n_un;
unsigned char	n_type;		
	char	n_other;	
	short	n_desc;		
unsigned long	n_value;	
};
struct	stat
{
	dev_t	st_dev;
	ino_t	st_ino;
	unsigned short 	st_mode;
	short  	st_nlink;
	short  	st_uid;
	short  	st_gid;
	dev_t	st_rdev;
	off_t	st_size;
	time_t	st_atime;
	time_t	st_mtime;
	time_t	st_ctime;
};
int	aflg, gflg, nflg, oflg, pflg, uflg; 
int	rflg = 1;
char	**xargv;
int	archive;
struct	ar_hdr	archdr;
union {
	char	mag_armag[8+1];
	struct	exec mag_exp;
} mag_un;
struct _iobuf	*fi;
off_t	off;
off_t	ftell();
char	*malloc();
char	*realloc();
char	*strp;
char	*stab();
off_t	strsiz;
int	compare();
int	narg;
int	errs;
main(argc, argv)
char **argv;
{
	if (--argc>0 && argv[1][0]=='-' && argv[1][1]!=0) {
		argv++;
		while (*++*argv) switch (**argv) {
		case 'n':
			nflg++;
			continue;
		case 'g':
			gflg++;
			continue;
		case 'u':
			uflg++;
			continue;
		case 'r':
			rflg = -1;
			continue;
		case 'p':
			pflg++;
			continue;
		case 'o':
			oflg++;
			continue;
		case 'a':
			aflg++;
			continue;
		default:
			fprintf((&_iob[2]), "nm: invalid argument -%c\n",
			    *argv[0]);
			exit(2);
		}
		argc--;
	}
	if (argc == 0) {
		argc = 1;
		argv[1] = "a.out";
	}
	narg = argc;
	xargv = argv;
	while (argc--) {
		++xargv;
		namelist();
	}
	exit(errs);
}
namelist()
{
	register int j;
	archive = 0;
	fi = fopen(*xargv, "r");
	if (fi == 0) {
		error(0, "cannot open");
		return;
	}
	off = 8;
	fread((char *)&mag_un, 1, sizeof(mag_un), fi);
	if (mag_un.mag_exp.a_magic == 0177545) {
		error(0, "old archive");
		return;
	}
	if (strncmp(mag_un.mag_armag, "!<arch>\n", 8)==0)
		archive++;
	else if (     (((mag_un.mag_exp).a_magic)!=0407		 && ((mag_un.mag_exp).a_magic)!=0410		 && ((mag_un.mag_exp).a_magic)!=0413		)) {
		error(0, "bad format");
		return;
	}
	fseek(fi, 0L, 0);
	if (archive) {
		nextel(fi);
		if (narg > 1)
			printf("\n%s:\n", *xargv);
	}
	do {
		off_t o;
		register i, n;
		struct nlist *symp = 0;
		struct nlist sym;
		struct stat stb;
		fread((char *)&mag_un.mag_exp, 1, sizeof(struct exec), fi);
		if (     (((mag_un.mag_exp).a_magic)!=0407		 && ((mag_un.mag_exp).a_magic)!=0410		 && ((mag_un.mag_exp).a_magic)!=0413		))
			continue;
		if (archive == 0)
			fstat(	((fi)->_file), &stb);
		o =  	( 	((mag_un.mag_exp).a_magic==0413		 ? 1024 : sizeof (struct exec)) + (mag_un.mag_exp).a_text+(mag_un.mag_exp).a_data + (mag_un.mag_exp).a_trsize+(mag_un.mag_exp).a_drsize) - sizeof (struct exec);
		fseek(fi, o, 1);
		n = mag_un.mag_exp.a_syms / sizeof(struct nlist);
		if (n == 0) {
			error(0, "no name list");
			continue;
		}
		if ( 	( 	( 	((mag_un.mag_exp).a_magic==0413		 ? 1024 : sizeof (struct exec)) + (mag_un.mag_exp).a_text+(mag_un.mag_exp).a_data + (mag_un.mag_exp).a_trsize+(mag_un.mag_exp).a_drsize) + (mag_un.mag_exp).a_syms) + sizeof (off_t) >
		    (archive ? off : stb.st_size))
			error(1, "old format .o (no string table) or truncated file");
		i = 0;
		if (strp)
			free(strp), strp = 0;
		while (--n >= 0) {
			fread((char *)&sym, 1, sizeof(sym), fi);
			if (gflg && (sym.n_type&01		)==0)
				continue;
			if ((sym.n_type&0xe0		) && (!aflg||gflg||uflg))
				continue;
			if (symp==0)
				symp = (struct nlist *)
				    malloc(sizeof(struct nlist));
			else
				symp = (struct nlist *)
				    realloc(symp,
					(i+1)*sizeof(struct nlist));
			if (symp == 0)
				error(1, "out of memory");
			symp[i++] = sym;
		}
		if (archive && ftell(fi)+sizeof(off_t) >= off) {
			error(0, "no string table (old format .o?)");
			continue;
		}
		if (fread((char *)&strsiz,sizeof(strsiz),1,fi) != 1) {
			error(0, "no string table (old format .o?)");
			goto out;
		}
		strp = (char *)malloc(strsiz);
		if (strp == 0)
			error(1, "ran out of memory");
		if (fread(strp+sizeof(strsiz),strsiz-sizeof(strsiz),1,fi) != 1)
			error(1, "error reading string table");
		for (j = 0; j < i; j++)
			if (symp[j].n_un.n_strx)
				symp[j].n_un.n_name =
				    symp[j].n_un.n_strx + strp;
			else
				symp[j].n_un.n_name = "";
		if (pflg==0)
			qsort(symp, i, sizeof(struct nlist), compare);
		if ((archive || narg>1) && oflg==0)
			printf("\n%s:\n", archive ? archdr.ar_name : *xargv);
		psyms(symp, i);
		if (symp)
			free((char *)symp), symp = 0;
		if (strp)
			free((char *)strp), strp = 0;
	} while(archive && nextel(fi));
out:
	fclose(fi);
}
psyms(symp, nsyms)
	register struct nlist *symp;
	int nsyms;
{
	register int n, c;
	for (n=0; n<nsyms; n++) {
		c = symp[n].n_type;
		if (c & 0xe0		) {
			if (oflg) {
				if (archive)
					printf("%s:", *xargv);
				printf("%s:", archive ? archdr.ar_name : *xargv);
			}
			printf("%08x - %02x %04x %5.5s %s\n",
			    symp[n].n_value,
			    symp[n].n_other & 0xff, symp[n].n_desc & 0xffff,
			    stab(symp[n].n_type & 0xff),
			    symp[n].n_un.n_name);
			continue;
		}
		switch (c&0x1e		) {
		case 0x0		:
			c = 'u';
			if (symp[n].n_value)
				c = 'c';
			break;
		case 0x2		:
			c = 'a';
			break;
		case 0x4		:
			c = 't';
			break;
		case 0x6		:
			c = 'd';
			break;
		case 0x8		:
			c = 'b';
			break;
		case 0x1f		:
			c = 'f';
			break;
		}
		if (uflg && c!='u')
			continue;
		if (oflg) {
			if (archive)
				printf("%s:", *xargv);
			printf("%s:", archive ? archdr.ar_name : *xargv);
		}
		if (symp[n].n_type&01		)
			c = toupper(c);
		if (!uflg) {
			if (c=='u' || c=='U')
				printf("        ");
			else
				printf("%08x", symp[n].n_value);
			printf(" %c ", c);
		}
		printf("%s\n", symp[n].n_un.n_name);
l1:		;
	}
}
compare(p1, p2)
struct nlist *p1, *p2;
{
	if (nflg) {
		if (p1->n_value > p2->n_value)
			return(rflg);
		if (p1->n_value < p2->n_value)
			return(-rflg);
	}
	return (rflg * strcmp(p1->n_un.n_name, p2->n_un.n_name));
}
nextel(af)
struct _iobuf *af;
{
	register char *cp;
	register r;
	long arsize;
	fseek(af, off, 0);
	r = fread((char *)&archdr, 1, sizeof(struct ar_hdr), af);
	if (r != sizeof(struct ar_hdr))
		return(0);
	for (cp = archdr.ar_name; cp < &archdr.ar_name[sizeof(archdr.ar_name)]; cp++)
		if (*cp == ' ')
			*cp = '\0';
	arsize = atol(archdr.ar_size);
	if (arsize & 1)
		++arsize;
	off = ftell(af) + arsize;	
	return(1);
}
error(n, s)
char *s;
{
	fprintf((&_iob[2]), "nm: %s:", *xargv);
	if (archive) {
		fprintf((&_iob[2]), "(%s)", archdr.ar_name);
		fprintf((&_iob[2]), ": ");
	} else
		fprintf((&_iob[2]), " ");
	fprintf((&_iob[2]), "%s\n", s);
	if (n)
		exit(2);
	errs = 1;
}
struct	stabnames {
	int	st_value;
	char	*st_name;
} stabnames[] ={
	0x20		, "GSYM",
	0x22		, "FNAME",
	0x24		, "FUN",
	0x26		, "STSYM",
	0x28		, "LCSYM",
	0x40		, "RSYM",
	0x44		, "SLINE",
	0x60		, "SSYM",
	0x64		, "SO",
	0x80		, "LSYM",
	0x84		, "SOL",
	0xa0		, "PSYM",
	0xa4		, "ENTRY",
	0xc0		, "LBRAC",
	0xe0		, "RBRAC",
	0xe2		, "BCOMM",
	0xe4		, "ECOMM",
	0xe8		, "ECOML",
	0xfe		, "LENG",
	0x30		, "PC",
	0, 0
};
char *
stab(val)
{
	register struct stabnames *sp;
	static char prbuf[32];
	for (sp = stabnames; sp->st_name; sp++)
		if (sp->st_value == val)
			return (sp->st_name);
	sprintf(prbuf, "%02x", val);
	return (prbuf);
}
