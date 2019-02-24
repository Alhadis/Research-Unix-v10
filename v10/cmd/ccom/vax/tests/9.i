/* vmmem.c */
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
struct	mount	
{
	dev_t	m_dev;		
	struct	buf *m_bufp;	
	struct	inode *m_inodp;	
	struct	inode *m_idev;	
	int	m_fstyp;	
};
struct	mount mount[62		];
					
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
vmemall(pte, size, p, type)
	register struct pte *pte;
	int size;
	struct proc *p;
{
	register int m;
	if (size <= 0 || size > maxmem)
		panic("vmemall size");
	while (size > 0) {
		if (freemem < desfree)
			wakeup((caddr_t)&proc[2]);	
		while (freemem == 0)
			sleep((caddr_t)&freemem, 0+2);
		m = imin(size, freemem);
		(void) memall(pte, m, p, type);
		size -= m;
		pte += m;
	}
	if (freemem < desfree)
		wakeup((caddr_t)&proc[2]);		
	
	return (1);
}
vmemfree(pte, count)
	register struct pte *pte;
	register int count;
{
	register struct cmap *c;
	register struct pte *spte;
	register int j;
	int size, pcnt, fileno;
	if (count % 2)
		panic("vmemfree");
	for (size = 0, pcnt = 0; count > 0; pte += 2, count -= 2) {
		if (pte->pg_fod == 0 && pte->pg_pfnum) {
			c = &cmap[	((((pte->pg_pfnum)-firstfree) / 2) + 1)];
			pcnt += 2;
			if (c->c_lock && c->c_type != 	0		) {
				for (j = 0; j < 2; j++)
					*(int *)(pte+j) &= (	0x78000000|0x00800000);
				c->c_gone = 1;
				goto free;
			}
			if (c->c_free) {
				pcnt -= 2;
				for (j = 0; j < 2; j++)
					*(int *)(pte+j) &= (	0x78000000|0x00800000);
				if (c->c_type == 	1		)
					distpte(&text[c->c_ndx], (int)c->c_page, pte);
				c->c_gone = 1;
				goto free;
			}
			if (size == 0)
				spte = pte;
			size += 2;
			continue;
		}
		if (pte->pg_fod) {
			fileno = ((struct fpte *)pte)->pg_fileno;
			if (fileno < 20		)
				if ((u.u_vrpages[fileno] -= 2) <= 0)
					if (u.u_vrpages[fileno] < 0)
						panic("vmemfree vrpages");
			for (j = 0; j < 2; j++)
				*(int *)(pte+j) &= (	0x78000000|0x00800000);
		}
free:
		if (size) {
			memfree(spte, size, 1);
			size = 0;
		}
	}
	if (size)
		memfree(spte, size, 1);
	return (pcnt);
}
munlink(pf)
	unsigned pf;
{
	register int next, prev;
	next = cmap[	((((pf)-firstfree) / 2) + 1)].c_next;
	prev = cmap[	((((pf)-firstfree) / 2) + 1)].c_prev;
	cmap[prev].c_next = next;
	cmap[next].c_prev = prev;
	cmap[	((((pf)-firstfree) / 2) + 1)].c_free = 0;
	if (freemem < minfree)
		wakeup((caddr_t)&proc[2]);	
	freemem -= 2;
}
memall(pte, size, p, type)
	register struct pte *pte;
	int size;
	struct proc *p;
{
	register struct cmap *c;
	register struct pte *rpte;
	register struct proc *rp;
	int i, j, next, curpos;
	unsigned pf;
	struct cmap *c1, *c2;
	if (size % 2)
		panic("memall");
	if (size > freemem)
		return (0);
	;
	for (i = size; i > 0; i -= 2) {
		curpos = cmap[0].c_next;
		c = &cmap[curpos];
		if (c->c_free == 0)
			panic("dup mem alloc");
		if (	((((curpos)-1) * 2) + firstfree) > maxfree)
			panic("bad mem alloc");
		if (c->c_gone == 0 && c->c_type != 	0		) {
			if (c->c_type == 	1		)
				rp = text[c->c_ndx].x_caddr;
			else
				rp = &proc[c->c_ndx];
			while (rp->p_flag & 0x00004000	)
				rp = rp->p_xlink;
			switch (c->c_type) {
			case 	1		:
				rpte = 		((rp)->p_p0br + ( c->c_page));
				break;
			case 	2		:
				rpte = 		((rp)->p_p0br + (rp)->p_tsize + ( c->c_page));
				break;
			case 	3		:
				rpte =  	(((rp)->p_p0br + (rp)->p_szpt*	(	512/(sizeof (struct pte))) - 8		 - 1) - ( c->c_page));
				break;
			}
				(rpte)[1]. pg_pfnum = (rpte)[0]. pg_pfnum = 0;
			if (c->c_type == 	1		)
				distpte(&text[c->c_ndx], (int)c->c_page, rpte);
		}
		switch (type) {
		case 	0		:
			c->c_ndx = p->p_ndx;
			break;
		case 	1		:
			c->c_page = 	((int)( ptetov(p, pte)));
			c->c_ndx = p->p_textp - &text[0];
			break;
		case 	2		:
			c->c_page = 	((int)(( ptetov(p, pte)) - (p)->p_tsize));
			c->c_ndx = p->p_ndx;
			break;
		case 	3		:
			c->c_page = 	((int)(		(((unsigned)((0x80000000-8		*	512))) >> 	9		) - 1 - ( ptetov(p, pte))));
			c->c_ndx = p->p_ndx;
			break;
		}
		if (c->c_blkno) {
			
			j = 	((c->c_blkno)&(512		-1));
			c1 = &cmap[cmhash[j]];
			if (c1 == c)
				cmhash[j] = c1->c_hlink;
			else {
				for (;;) {
					if (c1 == ecmap)
						panic("memall ecmap");
					c2 = c1;
					c1 = &cmap[c2->c_hlink];
					if (c1 == c)
						break;
				}
				c2->c_hlink = c1->c_hlink;
			}
			if (mfind(c->c_mdev == 15		 ?
			      swapdev : mount[c->c_mdev].m_dev,
			      (daddr_t)c->c_blkno))
				panic("memall mfind");
			c1->c_mdev = 0;
			c1->c_blkno = 0;
			c1->c_hlink = 0;
		}
		pf = 	((((curpos)-1) * 2) + firstfree);
		for (j = 0; j < 2; j++)
			*(int *)pte++ = pf++;
		c->c_free = 0;
		c->c_gone = 0;
		if (c->c_intrans || c->c_want)
			panic("memall intrans|want");
		c->c_lock = 1;
		c->c_type = type;
		freemem -= 2;
		next = c->c_next;
		cmap[0].c_next = next;
		cmap[next].c_prev = 0;
	}
	return (size);
}
memfree(pte, size, detach)
	register struct pte *pte;
	register int size;
{
	register int i, j, prev, next;
	register struct cmap *c;
	
	if (size % 2)
		panic("memfree");
	if (freemem < 2 * (32/2))
		wakeup((caddr_t)&freemem);
	while (size > 0) {
		size -= 2;
		i = pte->pg_pfnum;
		if (i < firstfree || i > maxfree)
			panic("bad mem free");
		i = 	((((i)-firstfree) / 2) + 1);
		c = &cmap[i];
		if (c->c_free)
			panic("dup mem free");
		if (detach && c->c_type != 	0		) {
			for (j = 0; j < 2; j++)
				*(int *)(pte+j) &= (	0x78000000|0x00800000);
			c->c_gone = 1;
		}
		if (detach && c->c_blkno == 0) {
			next = cmap[0].c_next;
			cmap[next].c_prev = i;
			c->c_prev = 0;
			c->c_next = next;
			cmap[0].c_next = i;
		} else {
			prev = cmap[0].c_prev;
			cmap[prev].c_next = i;
			c->c_next = 0;
			c->c_prev = prev;
			cmap[0].c_prev = i;
		}
		c->c_free = 1;
		freemem += 2;
		pte += 2;
	}
}
caddr_t
wmemall(pmemall, n)
	int (*pmemall)(), n;
{
	int npg;
	caddr_t va;
	register int a;
	npg = 	((((unsigned)(n)+511)>>9));
	a = rmalloc(kernelmap, npg);
	if (a == 0)
		return (0);
	if ((*pmemall)(&Usrptmap[a], npg, &proc[0], 	0		) == 0) {
		rmfree(kernelmap, npg, a);
		return (0);
	}
	va = (caddr_t) 	(usrpt + (a) * 	(	512/(sizeof (struct pte))));
	vmaccess(&Usrptmap[a], va, npg);
	return (va);
}
wmemfree(va, n)
	caddr_t va;
	int n;
{
	register int a;
	int npg;
	a = 	((((struct pte *) va) - usrpt) / 	(	512/(sizeof (struct pte))));
	npg = 	((((unsigned)(n)+511)>>9));
	(void) memfree(&Usrptmap[a], npg, 0);
	rmfree(kernelmap, npg, a);
}
mhash(c, dev, bn)
	register struct cmap *c;
	dev_t dev;
	daddr_t bn;
{
	register int i = 	((bn)&(512		-1));
	c->c_hlink = cmhash[i];
	cmhash[i] = c - cmap;
	c->c_blkno = bn;
	i = getfsx(dev);
	if (i == -1)
		panic("mhash");
	c->c_mdev = i;
}
munhash(dev, bn)
	dev_t dev;
	daddr_t bn;
{
	register int i = 	((bn)&(512		-1));
	register struct cmap *c1, *c2;
	c1 = &cmap[cmhash[i]];
	if (c1 == ecmap)
		panic("munhash");
	if (c1->c_blkno == bn && getfsx(dev) == c1->c_mdev)
		cmhash[i] = c1->c_hlink;
	else {
		for (;;) {
			c2 = c1;
			c1 = &cmap[c2->c_hlink];
			if (c1 == ecmap)
				panic("munhash");
			if (c1->c_blkno == bn && getfsx(dev) == c1->c_mdev)
				break;
		}
		c2->c_hlink = c1->c_hlink;
	}
	if (mfind(dev, bn))
		panic("munhash mfind");
	c1->c_mdev = 0;
	c1->c_blkno = 0;
	c1->c_hlink = 0;
}
struct cmap *
mfind(dev, bn)
	dev_t dev;
	daddr_t bn;
{
	register struct cmap *c1 = &cmap[cmhash[	((bn)&(512		-1))]];
	while (c1 != ecmap) {
		if (c1->c_blkno == bn && c1->c_mdev == getfsx(dev))
			return (c1);
		c1 = &cmap[c1->c_hlink];
	}
	return ((struct cmap *)0);
}
mpurge(mdev)
	int mdev;
{
	register struct cmap *c1, *c2;
	register int i;
	for (i = 0; i < 512		; i++) {
more:
		c1 = &cmap[cmhash[i]];
		if (c1 == ecmap)
			continue;
		if (c1->c_mdev == mdev)
			cmhash[i] = c1->c_hlink;
		else {
			for (;;) {
				c2 = c1;
				c1 = &cmap[c1->c_hlink];
				if (c1 == ecmap)
					goto cont;
				if (c1->c_mdev == mdev)
					break;
			}
			c2->c_hlink = c1->c_hlink;
		}
		c1->c_mdev = 0;
		c1->c_blkno = 0;
		c1->c_hlink = 0;
		goto more;
cont:
		;
	}
}
meminit(first, last)
	int first, last;
{
	register int i;
	register struct cmap *c;
	firstfree = 	(((first) + (2-1)) &~ (2-1));
	maxfree = 	(((last - (2 - 1)) + (2-1)) &~ (2-1));
	freemem = maxfree - firstfree;
	ecmx = ecmap - cmap;
	if (ecmx < freemem / 2)
		freemem = ecmx * 2;
	for (i = 1; i <= freemem / 2; i++) {
		cmap[i-1].c_next = i;
		c = &cmap[i];
		c->c_prev = i-1;
		c->c_free = 1;
		c->c_gone = 1;
		c->c_type = 	0		;
		c->c_mdev = 0;
		c->c_blkno = 0;
	}
	cmap[freemem / 2].c_next = 0;
	for (i = 0; i < 512		; i++)
		cmhash[i] = ecmx;
	cmap[0].c_prev = freemem / 2;
	cmap[0].c_type = 	0		;
	avefree = freemem;
	hand = 0;
}
mwait(pf)
	unsigned pf;
{
	mlock(pf);
	munlock(pf);
}
mlock(pf)
	unsigned pf;
{
	register struct cmap *c = &cmap[	((((pf)-firstfree) / 2) + 1)];
	while (c->c_lock) {
		c->c_want = 1;
		sleep((caddr_t)c, 0+1);
	}
	c->c_lock = 1;
}
munlock(pf)
	unsigned pf;
{
	register struct cmap *c = &cmap[	((((pf)-firstfree) / 2) + 1)];
	if (c->c_lock == 0)
		panic("dup page unlock");
	if (c->c_want)
		wakeup((caddr_t)c);
	c->c_lock = 0;
	c->c_want = 0;
}
vslock(base, count)
	caddr_t base;
{
	register unsigned v;
	register int npf;
	register struct pte *pte;
	u.u_procp->p_flag |= 0x00000200	;
	v = 		(((unsigned)(base)) >> 	9		);
	pte = vtopte(u.u_procp, v);
	npf = 	((((unsigned)(count + ((int)base & 	(2*	512-1)	))+511)>>9));
	while (npf > 0) {
		if (pte->pg_v) 
			mlock(pte->pg_pfnum);
		else
			if (fubyte((caddr_t)	((v)<<9)) < 0)
				panic("vslock");
		pte += 2;
		v += 2;
		npf -= 2;
	}
	u.u_procp->p_flag &= ~0x00000200	;
}
vsunlock(base, count, rw)
	caddr_t base;
{
	register struct pte *pte;
	register int npf;
	pte = vtopte(u.u_procp, 		(((unsigned)(base)) >> 	9		));
	npf = 	((((unsigned)(count + ((int)base & 	(2*	512-1)	))+511)>>9));
	while (npf > 0) {
		munlock(pte->pg_pfnum);
		if (rw == 	0x000001	)	
			pte->pg_m = 1;
		pte += 2;
		npf -= 2;
	}
}
