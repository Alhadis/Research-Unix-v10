typedef int	(*SIG_TYP)();
SIG_TYP signal();
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
struct map {
	struct	mapent *m_limit;	
	char	*m_name;		
};
struct mapent
{
	int	m_size;		
	int	m_addr;		
};
struct	nexus {
	union nexcsr {
		long	nex_csr;
		u_char	nex_type;
	} nexcsr;
	long	nex_pad[0x2000 / sizeof (long) - 1];
};
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
struct mba_regs
{
	int	mba_csr;		
	int	mba_cr;			
	int	mba_sr;			
	int	mba_var;		
	int	mba_bcr;		
	int	mba_dr;
	int	mba_pad1[250];
	struct mba_drv {		
		int	mbd_cs1;		
		int	mbd_ds;			
		int	mbd_er1;		
		int	mbd_mr1;		
		int	mbd_as;			
		int	mbd_da;			
		int	mbd_dt;			
		int	mbd_la;			
		int	mbd_sn;			
		int	mbd_of;			
		int	mbd_fill[22];
	} mba_drv[8];
	struct	pte mba_map[256];	
	int	mba_pad2[256*5];	
};
struct	mba_hd {
	short	mh_active;		
	short	mh_ndrive;		
	struct	mba_regs *mh_mba;	
	struct	mba_regs *mh_physmba;	
	struct	mba_device *mh_mbip[8];	
	struct	mba_device *mh_actf;	
	struct	mba_device *mh_actl;	
};
struct	mba_device {
	struct	mba_driver *mi_driver;
	short	mi_unit;	
	short	mi_mbanum;	
	short	mi_drive;	
	short	mi_dk;		
	short	mi_alive;	
	short	mi_type;	
	struct	buf mi_tab;	
	struct	mba_device *mi_forw;
	struct	mba_regs *mi_mba;
	struct	mba_drv *mi_drv;
	struct	mba_hd *mi_hd;
};
struct	mba_slave {
	struct	mba_driver *ms_driver;
	short	ms_ctlr;		
	short	ms_unit;		
	short	ms_slave;		
	short	ms_alive;
};
struct mba_driver {
	int	(*md_attach)();		
	int	(*md_slave)();		
	int	(*md_ustart)();		
	int	(*md_start)();		
	int	(*md_dtint)();		
	int	(*md_ndint)();		
	short	*md_type;		
	char	*md_dname, *md_sname;	
	struct	mba_device **md_info;	
};
					
					
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
struct	forkstat
{
	int	cntfork;
	int	cntvfork;
	int	sizfork;
	int	sizvfork;
};
struct	swptstat
{
	int	pteasy;		
	int	ptexpand;	
	int	ptshrink;	
	int	ptpack;		
};
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
struct scb {
	int	(*scb_stray)();		
	int	(*scb_machchk)();	
	int	(*scb_kspinval)();	
	int	(*scb_powfail)();	
	int	(*scb_resinstr)();	
	int	(*scb_custinst)();	
	int	(*scb_resopnd)();	
	int	(*scb_resaddr)();	
	int	(*scb_acv)();		
	int	(*scb_tnv)();		
	int	(*scb_tracep)();	
	int	(*scb_bpt)();		
	int	(*scb_compat)();	
	int	(*scb_arith)();		
	int	(*scb_stray2)();
	int	(*scb_stray3)();
	int	(*scb_chmk)();		
	int	(*scb_chme)();		
	int	(*scb_chms)();		
	int	(*scb_chmu)();		
	int	(*scb_sbisilo)();	
	int	(*scb_cmrd)();		
	int	(*scb_sbialert)();	
	int	(*scb_sbiflt)();	
	int	(*scb_wtime)();		
	int	(*scb_stray4[8])();
	int	(*scb_soft[15])();	
	int	(*scb_timer)();		
	int	(*scb_stray5[7])();
	int	(*scb_stray6[4])();
	int	(*scb_csdr)();		
	int	(*scb_csdx)();		
	int	(*scb_ctr)();		
	int	(*scb_ctx)();		
	int	(*scb_ipl14[16])();	
	int	(*scb_ipl15[16])();	
	int	(*scb_ipl16[16])();	
	int	(*scb_ipl17[16])();	
	int	(*scb_ubaint[128])();	
};
struct	mcr {
	int	mc_reg[3];
};
unifind(vubp, pubp, vumem, pumem, memmap)
	struct uba_regs *vubp, *pubp;
	caddr_t vumem, pumem;
	struct pte *memmap;
{
		



	struct uba_hd *uhp;

	int i, haveubasr = 0;
	for (i = 0; i < 128; i++)
		uhp->uh_vec[i] =
		    		((int (*)())(((int)&catcher[i*2])+ 1));
}
