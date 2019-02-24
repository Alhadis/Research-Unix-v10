/* machdep.c */
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
int	hz;			
int	timezone;		
int	dstflag;		
char	canonb[256		];	
extern char	version[];		
int	lbolt;			
time_t	time;			
time_t	bootime;		
long	trannum;		
int	hand;			
int	nblkdev;
int	nchrdev;
int	nstream;		
int	nswdev;			
int	mpid;			
char	runin;			
char	runout;			
int	runrun;			
char	kmapwnt;		
char	curpri;			
int	maxmem;			
int	physmem;		
int	nswap;			
int	updlock;		
daddr_t	rablock;		
extern	int intstack[];		
dev_t	rootdev;		
dev_t	dumpdev;		
long	dumplo;			
dev_t	swapdev;		
dev_t	argdev;			
dev_t	pipedev;		
extern	int icode[];		
extern	int szicode;		
dev_t	getmdev();
daddr_t	bmap();
caddr_t	calloc();
unsigned max();
unsigned min();
int	memall();
int	uchar(), schar();
int	vmemall();
char	*wmemall();
swblk_t	vtod();
extern struct sysent
{
	int	sy_narg;		
	int	(*sy_call)();		
} sysent[];
extern char	vmmap[];		
int	umbabeg,umbaend;	
int	noproc;			
extern	int catcher[256];
char	*panicstr;
int	wantin;
int	boothowto;		
struct	direct
{
	ino_t	d_ino;
	char	d_name[14		];
};
struct pcb
{
	int	pcb_ksp; 	
	int	pcb_esp; 	
	int	pcb_ssp; 	
	int	pcb_usp; 	
	int	pcb_r0; 
	int	pcb_r1; 
	int	pcb_r2; 
	int	pcb_r3; 
	int	pcb_r4; 
	int	pcb_r5; 
	int	pcb_r6; 
	int	pcb_r7; 
	int	pcb_r8; 
	int	pcb_r9; 
	int	pcb_r10; 
	int	pcb_r11; 
	int	pcb_r12; 
	int	pcb_r13; 
	int	pcb_pc; 	
	int	pcb_psl; 	
	struct  pte *pcb_p0br; 	
	int	pcb_p0lr; 	
	struct  pte *pcb_p1br; 	
	int	pcb_p1lr; 	
	int	pcb_szpt; 	
	int	pcb_cmap2;
	int	*pcb_sswap;
	int	pcb_sigc[3];
};
struct	dmap
{
	swblk_t	dm_size;	
	swblk_t	dm_alloc;	
	swblk_t	dm_map[		32	];	
};
struct	dmap zdmap;
struct	dblock
{
	swblk_t	db_base;	
	swblk_t	db_size;	
};
struct vtimes {
	int	vm_utime;		
	int	vm_stime;		
	
	unsigned vm_idsrss;		
	unsigned vm_ixrss;		
	int	vm_maxrss;		
	int	vm_majflt;		
	int	vm_minflt;		
	int	vm_nswap;		
	int	vm_inblk;		
	int	vm_oublk;		
};
struct vtimes zvms;		
 
 
struct	user
{
	struct	pcb u_pcb;
	int	u_arg[5];		
	label_t	u_qsav;			
	char	u_segflg;		
	char	u_error;		
	short	u_uid;			
	short	u_gid;			
	short	u_ruid;			
	short	u_rgid;			
	struct	proc *u_procp;		
	int	*u_ap;			
	union {				
		struct	{
			int	R_val1;
			int	R_val2;
		} u_rv;
		off_t	r_off;
		time_t	r_time;
	} u_r;
	caddr_t	u_base;			
	unsigned int u_count;		
	off_t	u_offset;		
	struct	inode *u_cdir;		
	struct	inode *u_rdir;		
	char	u_dbuf[14		];		
	caddr_t	u_dirp;			
	struct	direct u_dent;		
	struct	inode *u_pdir;		
	struct	file *u_ofile[20		];	
	char	u_pofile[20		];	
	label_t u_ssav;			
	int	(*u_signal[32])();	
	int	u_code;			
	int	*u_ar0;			
	struct uprof {			
		short	*pr_base;	
		unsigned pr_size;	
		unsigned pr_off;	
		unsigned pr_scale;	
	} u_prof;
	char	u_eosys;		
	char	u_sep;			
	dev_t	u_ttydev;		
	ino_t	u_ttyino;
	union {
	   struct {			
		int	Ux_mag;		
		unsigned Ux_tsize;	
		unsigned Ux_dsize;	
		unsigned Ux_bsize;	
		unsigned Ux_ssize;	
		unsigned Ux_entloc;	
		unsigned Ux_unused;
		unsigned Ux_relflg;
	   } Ux_A;
	   char ux_shell[32];	
	} u_exdata;
	char	u_comm[14		];
	time_t	u_start;
	char	u_acflag;
	short	u_fpflag;		
	short	u_cmask;		
	size_t	u_tsize;		
	size_t	u_dsize;		
	size_t	u_ssize;		
	struct	vtimes u_vm;		
	struct	vtimes u_cvm;		
	struct	dmap u_dmap;		
	struct	dmap u_smap;		
	struct	dmap u_cdmap, u_csmap;	
	time_t	u_outime;		
	size_t	u_odsize, u_ossize;	
	size_t	u_vrpages[20		];	
	int	u_limit[8];		
	int	u_stack[1];
					
};
extern	struct user u;
extern	struct user swaputl;
extern	struct user forkutl;
extern	struct user xswaputl;
extern	struct user xswap2utl;
extern	struct user pushutl;
extern	struct user vfutl;
extern	struct user prusrutl;
struct map {
	struct	mapent *m_limit;	
	char	*m_name;		
};
struct mapent
{
	int	m_size;		
	int	m_addr;		
};
struct	map *swapmap;
int	nswapmap;
struct	map *argmap;
struct	map *kernelmap;
	
struct pte
{
unsigned int	pg_pfnum:21,		
		:2,
		pg_vreadm:1,		
		pg_swapm:1,		
		pg_fod:1,		
		pg_m:1,			
		pg_prot:4,		
		pg_v:1;			
};
struct hpte
{
unsigned int	pg_pfnum:21,
		:2,
		pg_high:9;		
};
struct fpte
{
unsigned int	pg_blkno:20,		
		pg_fileno:5,		
		pg_fod:1,		
		:1,
		pg_prot:4,
		pg_v:1;
};
struct	pte *vtopte();
extern	struct pte Sysmap[];
extern	struct pte Usrptmap[];
extern	struct pte usrpt[];
extern	struct pte Swapmap[];
extern	struct pte Forkmap[];
extern	struct pte Xswapmap[];
extern	struct pte Xswap2map[];
extern	struct pte Pushmap[];
extern	struct pte Vfmap[];
extern	struct pte mmap[];
extern	struct pte msgbufmap[];
extern	struct pte camap[];
extern	struct pte Nexmap[][16];
extern	struct pte Prusrmap[];
					
					
int	klseql;
int	klsdist;
int	klin;
int	kltxt;
int	klout;
struct vmmeter
{
	unsigned v_swtch;	
	unsigned v_trap;	
	unsigned v_syscall;	
	unsigned v_intr;	
	unsigned v_pdma;	
	unsigned v_pswpin;	
	unsigned v_pswpout;	
	unsigned v_pgin;	
	unsigned v_pgout;	
	unsigned v_pgpgin;	
	unsigned v_pgpgout;	
	unsigned v_intrans;	
	unsigned v_pgrec;	
	unsigned v_xsfrec;	
	unsigned v_xifrec;	
	unsigned v_exfod;	
	unsigned v_zfod;	
	unsigned v_vrfod;	
	unsigned v_nexfod;	
	unsigned v_nzfod;	
	unsigned v_nvrfod;	
	unsigned v_pgfrec;	
	unsigned v_faults;	
	unsigned v_scan;	
	unsigned v_rev;		
	unsigned v_seqfree;	
	unsigned v_dfree;	
	unsigned v_swpin;	
	unsigned v_swpout;	
};
struct	vmmeter cnt, rate, sum;
struct vmtotal
{
	short	t_rq;		
	short	t_dw;		
	short	t_pw;		
	short	t_sl;		
	short	t_sw;		
	int	t_vm;		
	int	t_avm;		
	short	t_rm;		
	short	t_arm;		
	int	t_vmtxt;	
	int	t_avmtxt;	
	short	t_rmtxt;	
	short	t_armtxt;	
	short	t_free;		
};
struct	vmtotal total;
int	freemem;		
int	avefree;		
int	avefree30;		
int	deficit;		
int	nscan;			
int	multprog;		
int	desscan;		
int	maxpgio;		
int	maxslp;			
int	lotsfree;		
int	minfree;		
int	desfree;		
int	saferss;		
struct	forkstat
{
	int	cntfork;
	int	cntvfork;
	int	sizfork;
	int	sizvfork;
};
struct	forkstat forkstat;
struct	swptstat
{
	int	pteasy;		
	int	ptexpand;	
	int	ptshrink;	
	int	ptpack;		
};
struct	swptstat swptstat;
struct	proc
{
	struct	proc *p_link;	
	struct	proc *p_rlink;	
	struct	pte *p_addr;	
	char	p_usrpri;	
	char	p_pri;		
	char	p_cpu;		
	char	p_stat;
	char	p_time;		
	char	p_nice;		
	char	p_slptime;	
	char	p_cursig;
	long	p_sig;		
	long	p_siga0;	
	long	p_siga1;	
	int	p_flag;
	short	p_uid;		
	short	p_pgrp;		
	short	p_pid;		
	short	p_ppid;		
	short	p_poip;		
	short	p_szpt;		
	size_t	p_tsize;	
	size_t	p_dsize;	
	size_t	p_ssize;	
	size_t 	p_rssize; 	
	size_t	p_maxrss;	
	size_t	p_swrss;	
	swblk_t	p_swaddr;	
	caddr_t p_wchan;	
	struct	text *p_textp;	
	u_short	p_clktim;	
	u_short	p_tsleep;	
	struct	pte *p_p0br;	
	struct	proc *p_xlink;	
	short	p_cpticks;	
	float	p_pctcpu;	
	short	p_ndx;		
	short	p_idhash;	
	struct	proc *p_pptr;	
	struct	inode *p_trace;	
};
short	pidhash[	63];
struct	proc *pfind();
struct	proc *proc, *procNPROC;	
int	nproc;
struct	prochd {
	struct	proc *ph_link;	
	struct	proc *ph_rlink;
} qs[32		];
int	whichqs;		
struct	xproc
{
	struct	proc *xp_link;
	struct	proc *xp_rlink;
	struct	pte *xp_addr;
	char	xp_usrpri;
	char	xp_pri;		
	char	xp_cpu;		
	char	xp_stat;
	char	xp_time;	
	char	xp_nice;	
	char	xp_slptime;
	char	p_cursig;
	int	xp_sig;		
	int	xp_siga0;
	int	xp_siga1;
	int	xp_flag;
	short	xp_uid;		
	short	xp_pgrp;	
	short	xp_pid;		
	short	xp_ppid;	
	short	xp_xstat;	
	struct	vtimes xp_vm;
};
struct bufhd
{
	long	b_flags;		
	struct	buf *b_forw, *b_back;	
};
struct buf
{
	long	b_flags;		
	struct	buf *b_forw, *b_back;	
	struct	buf *av_forw, *av_back;	
	long	b_bcount;		
	short	b_error;		
	dev_t	b_dev;			
	union {
	    caddr_t b_addr;		
	    int	*b_words;		
	    struct filsys *b_filsys;	
	    struct dinode *b_dino;	
	    daddr_t *b_daddr;		
	} b_un;
	daddr_t	b_blkno;		
	long	b_resid;		
	struct  proc *b_proc;		
};
struct	buf *buf;		
char	*buffers;
int	nbuf;
struct	buf *swbuf;		
int	nswbuf;
short	*swsize;
int	*swpf;
struct	buf bfreelist[	3		];	
struct	buf bswlist;		
struct	buf *bclnlist;		
struct	buf *alloc();
struct	buf *baddr();
struct	buf *getblk();
struct	buf *geteblk();
struct	buf *bread();
struct	buf *breada();
unsigned minphys();
struct	uba_hd {
	struct	uba_regs *uh_uba;	
	struct	uba_regs *uh_physuba;	
	int	(**uh_vec)();		
	struct	uba_device *uh_actf;	
	struct	uba_device *uh_actl;	
	short	uh_mrwant;		
	short	uh_bdpwant;		
	int	uh_bdpfree;		
	int	uh_hangcnt;		
	int	uh_zvcnt;		
	int	uh_errcnt;		
	int	uh_lastiv;		
	short	uh_users;		
	short	uh_xclu;		
	struct	map *uh_map;		
};
struct uba_ctlr {
	struct	uba_driver *um_driver;
	short	um_ctlr;	
	short	um_ubanum;	
	short	um_alive;	
	int	(**um_intr)();	
	caddr_t	um_addr;	
	struct	uba_hd *um_hd;
	int	um_cmd;		
	int	um_ubinfo;	
	struct	buf um_tab;	
};
struct uba_device {
	struct	uba_driver *ui_driver;
	short	ui_unit;	
	short	ui_ctlr;	
	short	ui_ubanum;	
	short	ui_slave;	
	int	(**ui_intr)();	
	caddr_t	ui_addr;	
	short	ui_dk;		
	int	ui_flags;	
	short	ui_alive;	
	short	ui_type;	
	caddr_t	ui_physaddr;	
	struct	uba_device *ui_forw;
	struct	uba_ctlr *ui_mi;
	struct	uba_hd *ui_hd;
};
struct uba_driver {
	int	(*ud_probe)();		
	int	(*ud_slave)();		
	int	(*ud_attach)();		
	int	(*ud_dgo)();		
	u_short	*ud_addr;		
	char	*ud_dname;		
	struct	uba_device **ud_dinfo;	
	char	*ud_mname;		
	struct	uba_ctlr **ud_minfo;	
	short	ud_xclu;		
};
int	numuba;					
extern struct	uba_hd uba_hd[];
extern	struct	uba_ctlr ubminit[];
extern	struct	uba_device ubdinit[];
extern	struct pte UMEMmap[][16];	
extern	char umem[][16*	512];		
extern	int (*UNIvec[])();			
struct uba_regs
{
	int	uba_cnfgr;		
	int	uba_cr;			
	int	uba_sr;			
	int	uba_dcr;		
	int	uba_fmer;		
	int	uba_fubar;		
	int	pad1[2];
	int	uba_brsvr[4];
	int	uba_brrvr[4];		
	int	uba_dpr[16];		
	int	pad2[480];
	struct	pte uba_map[496];	
	int	pad3[16];		
};
 
 
struct bdevsw
{
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_strategy)();
	int	(*d_dump)();
	int	d_flags;
};
extern struct	bdevsw bdevsw[];
extern struct cdevsw
{
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_read)();
	int	(*d_write)();
	int	(*d_ioctl)();
	int	(*d_reset)();
	struct	streamtab *qinfo;
};
extern struct cdevsw cdevsw[];
struct fstypsw {
	int		(*t_put)();
	struct inode	*(*t_get)();
	int		(*t_free)();
	int		(*t_updat)();
	int		(*t_read)();
	int		(*t_write)();
	int		(*t_trunc)();
	int		(*t_stat)();
	int		(*t_nami)();
	int		(*t_mount)();
	int		(*t_ioctl)();
};
extern struct fstypsw fstypsw[];
extern nfstyp;
extern	struct streamtab {
	struct	qinit	*rdinit;
	struct	qinit	*wrinit;
} *streamtab[];
struct swdevt
{
	dev_t	sw_dev;
	int	sw_freed;
};
extern struct	swdevt swdevt[];
struct	mcr {
	int	mc_reg[3];
};
int	nmcr;
struct	mcr *mcraddr[	1];
union cpusid {
	int	cpusid;
	struct cpuany {
		u_int	:24,
			cp_type:8;
	} cpuany;
	struct cpu780 {
		u_int	cp_sno:12,		
			cp_plant:3,		
			cp_eco:9,		
			cp_type:8;		
	} cpu780;
	struct cpu750 {
		u_int	cp_hrev:8,		
			cp_urev:8,		
			:8,
			cp_type:8;		
	} cpu750;
	
};
struct	percpu {
	short	pc_cputype;		
	short	pc_nnexus;		
	struct	nexus *pc_nexbase;	
	caddr_t	*pc_umaddr;		
	short	pc_nubabdp;		
	short	pc_haveubasr;		
	short	*pc_nextype;		
};
int	cpu;
extern struct	percpu percpu[];
struct	inode
{
	short	i_flag;
	u_char	i_count;	
	char	i_fstyp;	
	dev_t	i_dev;		
	long	i_number;	
	unsigned short i_mode;
	short	i_nlink;	
	short	i_uid;		
	short	i_gid;		
	off_t	i_size;		
	struct	stdata *i_sptr;	
	union {
		struct {
			daddr_t	I_addr[13];	
			daddr_t	I_lastr;	
		} i_f;
		struct {
			daddr_t	I_rdev;		
			long	I_key;		
		} i_d;
		struct {
			long I_tag;
			struct inode *I_cip;	
		} i_a;		
		struct {
			struct proc *I_proc;	
			int	    I_sigmask;	
		} i_p;
	} i_un;
	short	i_hlink;	
};
struct	inode *inode, *inodeNINODE;
int	ninode;
struct	inode *rootdir;		
struct	inode *ialloc();
struct	inode *ifind();
struct	inode *iget();
struct	inode *owner();
struct	inode *maknode();
struct	inode *namei();
struct argnamei {	
	short flag;	
	ino_t ino;	
	dev_t idev;
	short mode;	
};
struct nx {	
	struct inode *dp;
	char *cp;
	struct buf *nbp;
	int nlink;
};
struct	file
{
	short	f_flag;
	short	f_count;		
	struct inode *f_inode;		
	off_t	f_offset;		
};
struct	file *file, *fileNFILE;	
int	nfile;
struct	file *getf();
struct	file *falloc();
struct text
{
	swblk_t	x_daddr[12		];	
	swblk_t	x_ptdaddr;	
	size_t	x_size;		
	struct proc *x_caddr;	
	struct inode *x_iptr;	
	short	x_rssize;
	short	x_swrss;
	char	x_count;	
	char	x_ccount;	
	char	x_flag;		
	char	x_slptime;
	short	x_poip;		
};
struct	text *text, *textNTEXT;
int	ntext;
struct	callout {
	int	c_time;		
	caddr_t	c_arg;		
	int	(*c_func)();	
	struct	callout *c_next;
};
struct	callout *callfree, *callout, calltodo;
int	ncallout;
struct cmap
{
unsigned int 	c_next:13,	
		c_prev:13,	
		c_lock:1,	
		c_want:1,	
		c_page:16,	
		c_hlink:13,	
		c_intrans:1,	
		c_free:1,	
		c_gone:1,	
		c_type:2,	
		c_blkno:20,	
		c_ndx:10,	
		c_mdev:6;	
};
struct	cmap *cmap;
struct	cmap *ecmap;
int	ncmap;
struct	cmap *mfind();
int	firstfree, maxfree;
int	ecmx;			
short	cmhash[512		];
struct frame {
	int	fr_handler;
	u_int	fr_psw:16,		
		fr_mask:12,		
		:1,
		fr_s:1,			
		fr_spa:2;		
	int	fr_savap;		
	int	fr_savfp;		
	int	fr_savpc;		
};
struct rpb {
	struct	rpb *rp_selfref;	
	int	(*rp_dumprout)();	
	long	rp_checksum;		
	long	rp_flag;		
};
extern	struct rpb rpb;
int	icode[] =
{
	0x9f19af9f,	
	0x02dd09af,	
	0xbc5c5ed0,	
	0x2ffe110b,	
	0x2f637465,	
	0x74696e69,	
	0x00000000,	
	0x00000014,	
	0x00000000,	
};
int	szicode = sizeof(icode);
 
int	nbuf = 0;
int	nswbuf = 0;
startup(firstaddr)
	int firstaddr;
{
	register int unixsize;
	register unsigned i;
	register struct pte *pte;
	register caddr_t v;
	
	maxmem -= 2;
	pte = msgbufmap;
	for (i = 0; i < 2; i++)
		*(int *)pte++ = 	0x80000000 | 	0x10000000 | (maxmem + i);
	mtpr(0x39		, 1);
	
	printf(version);
	
	
	if (nbuf == 0) {
		nbuf = (32 * physmem) / 	((((unsigned)(1024*1024)+511)>>9));
		if (nbuf < 32)
			nbuf = 32;
	}
	if (nswbuf == 0) {
		nswbuf = (nbuf / 2) &~ 1;	
		if (nswbuf > 256)
			nswbuf = 256;		
	}
	printf("real mem = %d nbuf = %d nswbuf = %d\n", 	((maxmem)<<9),
		nbuf, nswbuf);
	
	v = (caddr_t)(0x80000000 | (firstaddr * 	512));
	 	    (buffers) = ( char *)(v); (v) = (caddr_t)((buffers)+( 	4096*nbuf));
	 	    (buf) = ( struct buf *)(v); (v) = (caddr_t)((buf)+( nbuf));
	 	    (swbuf) = ( struct buf *)(v); (v) = (caddr_t)((swbuf)+( nswbuf));
	 	    (swsize) = ( short *)(v); (v) = (caddr_t)((swsize)+( nswbuf));	
	 	    (swpf) = ( int *)(v); (v) = (caddr_t)((swpf)+( nswbuf));
	 	    (inode) = ( struct inode *)(v); (v) = (caddr_t)(( inodeNINODE) = ((inode)+( ninode)));
	 	    (file) = ( struct file *)(v); (v) = (caddr_t)(( fileNFILE) = ((file)+( nfile)));
	 	    (proc) = ( struct proc *)(v); (v) = (caddr_t)(( procNPROC) = ((proc)+( nproc)));
	 	    (text) = ( struct text *)(v); (v) = (caddr_t)(( textNTEXT) = ((text)+( ntext)));
	 	    (callout) = ( struct callout *)(v); (v) = (caddr_t)((callout)+( ncallout));
	 	    (swapmap) = ( struct map *)(v); (v) = (caddr_t)((swapmap)+( nswapmap = nproc * 2));
	 	    (argmap) = ( struct map *)(v); (v) = (caddr_t)((argmap)+( 16));
	 	    (kernelmap) = ( struct map *)(v); (v) = (caddr_t)((kernelmap)+( nproc));
	
	ncmap = (physmem*	512 - ((int)v &~ 0x80000000)) /
		    (	512*2 + sizeof (struct cmap));
	 	    (cmap) = ( struct cmap *)(v); (v) = (caddr_t)(( ecmap) = ((cmap)+( ncmap)));
	if ((((int)(ecmap+1))&~0x80000000) > (32*	(	512/(sizeof (struct pte))))*	512)
		panic("sys pt too small");
	
	unixsize = 	((((unsigned)((int)(ecmap+1) &~ 0x80000000)+511)>>9));
	if (unixsize >= physmem - 8*8		)
		panic("no memory");
	pte = &Sysmap[firstaddr];
	for (i = firstaddr; i < unixsize; i++) {
		*(int *)(&Sysmap[i]) = 	0x80000000 | 	0x10000000 | i;
		clearseg(i);
	}
	mtpr(0x39		, 1);
	
	callfree = callout;
	for (i = 1; i < ncallout; i++)
		callout[i-1].c_next = &callout[i];
	
	meminit(unixsize, maxmem);
	maxmem = freemem;
	printf("avail mem = %d\n", 	((maxmem)<<9));
	rminit(kernelmap, 	(8*	(	512/(sizeof (struct pte)))), 1, "usrpt", nproc);
	
	configure();
	
	tocons(0xf03		);
	tocons(0xf04		);
}
sysphys()
{
	if(!suser())
		return;
	u.u_error = 22;
}
clkinit(base)
	time_t base;
{
	register unsigned todr = mfpr(0x1b		);
	long deltat;
	int year = 	1970;
	if (base < 5*	((unsigned)(365*	((unsigned)(24*60*60))		))	) {
		printf("WARNING: preposterous time in file system");
		time = 6*	((unsigned)(365*	((unsigned)(24*60*60))		))	 + 186*	((unsigned)(24*60*60))		 + 	((unsigned)(24*60*60))		/2;
		clkset();
		goto check;
	}
	
	if (todr < ((unsigned)(1<<28))) {
		printf("WARNING: todr too small");
		time = base;
		
		clkset();
		goto check;
	}
	
	for (time = (todr-((unsigned)(1<<28)))/100; time < base-	((unsigned)(365*	((unsigned)(24*60*60))		))	/2; time += 	((unsigned)(365*	((unsigned)(24*60*60))		))	) {
		if (	((year)%4==0)	)
			time += 	((unsigned)(24*60*60))		;
		year++;
	}
	
	deltat = time - base;
	if (deltat < 0)
		deltat = -deltat;
	if (deltat < 2*	((unsigned)(24*60*60))		)
		return;
	printf("WARNING: clock %s %d days",
	    time < base ? "lost" : "gained", deltat / 	((unsigned)(24*60*60))		);
check:
	printf(" -- CHECK AND RESET THE DATE!\n");
}
clkset()
{
	int year = 	1970;
	unsigned secyr;
	unsigned yrtime = time;
	
	for (;;) {
		secyr = 	((unsigned)(365*	((unsigned)(24*60*60))		))	;
		if (	((year)%4==0)	)
			secyr += 	((unsigned)(24*60*60))		;
		if (yrtime < secyr)
			break;
		yrtime -= secyr;
		year++;
	}
	mtpr(0x1b		, ((unsigned)(1<<28)) + yrtime*100);
}
sendsig(p, n)
	int (*p)();
{
	register int *usp, *regs;
	regs = u.u_ar0;
	usp = (int *)regs[(-5)];
	usp -= 5;
	if ((int)usp <= (0x80000000-8		*	512) - 	((u.u_ssize)<<9))
		(void) grow((unsigned)usp);
	;			
	asm("probew $3,$20,(r11)");
	asm("beql bad");
	*usp++ = n;
	if (n == 4	 || n == 8	) {
		*usp++ = u.u_code;
		u.u_code = 0;
	} else
		*usp++ = 0;
	*usp++ = (int)p;
	*usp++ = regs[(-2)];
	*usp++ = regs[(-1)];
	regs[(-5)] = (int)(usp - 5);
	regs[(-1)] &= ~(	0x80000000	|	0x08000000	);
	regs[(-2)] = (int)u.u_pcb.pcb_sigc;
	return;
asm("bad:");
bad:
	
	u.u_signal[4	] = 	(int (*)())0;
	u.u_procp->p_siga0 &= ~(1<<(4	-1));
	u.u_procp->p_siga1 &= ~(1<<(4	-1));
	psignal(u.u_procp, 4	);
}
dorti()
{
	struct frame frame;
	register int sp;
	register int reg, mask;
	extern int ipcreg[];
	(void) copyin((caddr_t)u.u_ar0[(-20)], (caddr_t)&frame, sizeof (frame));
	sp = u.u_ar0[(-20)] + sizeof (frame);
	u.u_ar0[(-2)] = frame.fr_savpc;
	u.u_ar0[(-20)] = frame.fr_savfp;
	u.u_ar0[(-21)] = frame.fr_savap;
	mask = frame.fr_mask;
	for (reg = 0; reg <= 11; reg++) {
		if (mask&1) {
			u.u_ar0[ipcreg[reg]] = fuword((caddr_t)sp);
			sp += 4;
		}
		mask >>= 1;
	}
	sp += frame.fr_spa;
	u.u_ar0[(-1)] = (u.u_ar0[(-1)] & 0xffff0000) | frame.fr_psw;
	if (frame.fr_s)
		sp += 4 + 4 * (fuword((caddr_t)sp) & 0xff);
	
	u.u_ar0[(-2)] = fuword((caddr_t)sp);
	sp += 4;
	u.u_ar0[(-1)] = fuword((caddr_t)sp);
	sp += 4;
	u.u_ar0[(-1)] |= (0x00c00000	|0x03000000	);
	u.u_ar0[(-1)] &= ~(	0x04000000	|	0x001f0000	|	0x3020ff00	);
	u.u_ar0[(-5)] = (int)sp;
}
int	memintvl = (60*60*30)		;
memenable()
{
	register struct mcr *mcr;
	register int m;
	for (m = 0; m < nmcr; m++) {
		mcr = mcraddr[m];
		switch (cpu) {
		}
	}
	if (memintvl > 0)
		timeout(memenable, (caddr_t)0, memintvl);
}
memerr()
{
	register struct mcr *mcr;
	register int m;
	for (m = 0; m < nmcr; m++) {
		mcr = mcraddr[m];
		switch (cpu) {
		}
	}
}
tbiscl(v)
	unsigned v;
{
	register caddr_t addr;		
	register int i;
	asm(".set TBIS,58");
	addr = 		((caddr_t)((v) << 	9		));
	for (i = 0; i < 2; i++) {
		asm("mtpr r11,$TBIS");
		addr += 	512;
	}
}
  
int	waittime = -1;
boot(paniced, arghowto)
	int paniced, arghowto;
{
	register int howto;		
	register int devtype;		
	howto = arghowto;
	if ((howto&4	)==0 && waittime < 0 && bfreelist[0].b_forw) {
		waittime = 0;
		update();
		printf("syncing disks... ");
		while (++waittime <= 5)
			sleep((caddr_t)&lbolt, 25);
		printf("done\n");
	}
	splx(0x1f);			
	devtype = 	((int)(((unsigned)(rootdev)>>8)&0377));
	if (howto&	8	) {
		printf("halting (in tight loop); hit\n\t^P\n\tHALT\n\n");
		mtpr(0x12		, 0x1f);
		for (;;)
			;
	} else {
		if (paniced == 0	)
			doadump();
		tocons(0xf02		);
	}
	for (;;)
		asm("halt");
	
}
tocons(c)
{
	while ((mfpr(0x22		)&0x00000080	) == 0)
		continue;
	mtpr(0x23		, c);
}
dumpsys()
{
	rpb.rp_flag = 1;
	if ((	((int)((dumpdev)&0377))&07) != 1)
		return;
	printf("\ndumping to dev %x, offset %d\n", dumpdev, dumplo);
	printf("dump ");
	switch ((*bdevsw[	((int)(((unsigned)(dumpdev)>>8)&0377))].d_dump)(dumpdev)) {
	case 6:
		printf("device bad\n");
		break;
	case 14:
		printf("device not ready\n");
		break;
	case 22:
		printf("area improper\n");
		break;
	case 5:
		printf("i/o error");
		break;
	default:
		printf("succeeded");
		break;
	}
}
struct mc780frame {
	int	mc8_bcnt;		
	int	mc8_summary;		
	int	mc8_cpues;		
	int	mc8_upc;		
	int	mc8_vaviba;		
	int	mc8_dreg;		
	int	mc8_tber0;		
	int	mc8_tber1;		
	int	mc8_timo;		
	int	mc8_parity;		
	int	mc8_sbier;		
	int	mc8_pc;			
	int	mc8_psl;		
};
struct mc750frame {
	int	mc5_bcnt;		
	int	mc5_summary;		
	int	mc5_va;			
	int	mc5_errpc;		
	int	mc5_mdr;
	int	mc5_svmode;		
	int	mc5_rdtimo;		
	int	mc5_tbgpar;		
	int	mc5_cacherr;		
	int	mc5_buserr;		
	int	mc5_mcesr;		
	int	mc5_pc;			
	int	mc5_psl;		
};
struct mc7ZZframe {
	int	mc3_bcnt;		
	int	mc3_summary;		
	int	mc3_parm[2];		
	int	mc3_pc;			
	int	mc3_psl;		
};
machinecheck(cmcf)
	caddr_t cmcf;
{
	register u_int type = ((struct mc780frame *)cmcf)->mc8_summary;
	printf("machine check %x: ", type);
	switch (cpu) {
	}
	switch (cpu) {
	}
	panic("mchk");
}
