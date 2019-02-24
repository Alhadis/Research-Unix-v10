/*
 * Print system stuff
 */

#define mask(x) (x&0377)
#define	clear(x) ((int)x&0x7fffffff)

#include <sys/param.h>
#include <sys/file.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/text.h>
#include <sys/inode.h>
#include <sys/conf.h>
#include <nlist.h>
#include <sys/vm.h>
#include <sys/pte.h>
#include <stdio.h>

char	*fcore	= "/dev/mem";
char	*fnlist	= "/unix";
int	fc;

struct nlist nl[] = {
#define	SINODE	0
	{ "_inode" },
#define	STEXT	1
	{ "_text" },
#define	SPROC	2
	{ "_proc" },
#define	SFIL	3
	{ "_file" },
#define	USRPTMA	4
	{ "_Usrptmap" },
#define	USRPT	5
	{ "_usrpt" },
#define	SNPROC	6
	{ "_proccnt" },
#define	SNTEXT	7
	{ "_textcnt" },
#define	SNFILE	8
	{ "_filecnt" },
#define	SNINODE	9
	{ "_inodecnt" },
#define	SNSWAPMAP 10
	{ "_swmapcnt" },
	0,
};

int	inof;		/* inode table */
int	txtf;		/* text table */
int	prcf;		/* proc table */
int	usrf;		/* user struct */
long	ubase;
int	filf;		/* file table */
int	totflg;		/* total table usage */
char	partab[1];
struct	cdevsw	cdevsw[1];
struct	bdevsw	bdevsw[1];
int	allflg;
int	kflg;
struct	pte *Usrptma;
struct	pte *usrpt;
long	seekmask = 0xffffffff;

main(argc, argv)
char **argv;
{
	register char *argp;

	argc--, argv++;
	while (argc > 0 && **argv == '-') {
		argp = *argv++;
		argp++;
		argc--;
		while (*argp++)
		switch (argp[-1]) {

		case 'T':
			totflg++;
			break;

		case 'a':
			allflg++;
			break;

		case 'i':
			inof++;
			break;

		case 'k':
			kflg++;
			fcore = "/vmcore";
			seekmask = 0x7fffffff;
			break;

		case 'x':
			txtf++;
			break;

		case 'p':
			prcf++;
			break;

		case 'u':
			if (argc == 0)
				break;
			argc--;
			usrf++;
			sscanf( *argv++, "%x", &ubase);
			break;

		case 'f':
			filf++;
			break;
		}
	}
	if (argc>0) {
		fcore = argv[0];
		seekmask = 0x7fffffff;
	}
	if ((fc = open(fcore, 0)) < 0) {
		perror(fcore);
		exit(1);
	}
	if (argc>1)
		fnlist = argv[1];
	nlist(fnlist, nl);
	if (nl[0].n_type == 0) {
		fprintf(stderr, "no namelist\n");
		exit(1);
	}
	usrpt = (struct pte *)nl[USRPT].n_value;
	Usrptma = (struct pte *)nl[USRPTMA].n_value;
	if (filf||totflg)
		dofile();
	if (inof||totflg)
		doinode();
	if (prcf||totflg)
		doproc();
	if (txtf||totflg)
		dotext();
	if (usrf)
		dousr();
	exit(0);
}

doinode()
{
	register struct inode *ip;
	struct inode *xinode, *ainode;
	register int nin;
	int ninode;

	nin = 0;
	ninode = getw(nl[SNINODE].n_value);
	xinode = (struct inode *)calloc(ninode, sizeof (struct inode));
	xxseek(fc, (int)(ainode = (struct inode *)(nl[SINODE].n_value)), 0);
	read(fc, xinode, ninode * sizeof(struct inode));
	for (ip = xinode; ip < &xinode[ninode]; ip++)
		if (ip->i_count)
			nin++;
	printf("%5d/%5d inodes\n", nin, ninode);
	if (totflg)
		return;
	printf("   LOC    FLAGS  CNT FS  DEVICE  INO   MODE  NLN UID   SPTR   SIZE/DEV MROOT\n");
	for (ip = xinode; ip < &xinode[ninode]; ip++) {
		if (ip->i_count == 0)
			continue;
		printf("%8.1x ", ainode + (ip - xinode));
		putf(ip->i_flag&ILOCK, 'L');
		putf(ip->i_flag&IUPD, 'U');
		putf(ip->i_flag&IACC, 'A');
		putf(ip->i_flag&IOPEN, 'O');
		putf(ip->i_flag&IWANT, 'W');
		putf(ip->i_flag&ITEXT, 'T');
		printf("%4d", ip->i_count&0377);
		printf("%3d", ip->i_fstyp);
		printf("%4d,%3d", major(ip->i_dev), minor(ip->i_dev));
		printf("%6d", ip->i_number);
		printf("%7o", ip->i_mode & 0xffff);
		printf("%4d", ip->i_nlink);
		printf("%4d", ip->i_uid);
		printf("%8x", (int)ip->i_sptr&0xffffff);
		if ((ip->i_mode&IFMT)==IFBLK || (ip->i_mode&IFMT)==IFCHR)
			printf("%6d,%3d", major(ip->i_un.i_rdev), minor(ip->i_un.i_rdev));
		else
			printf("%10ld", ip->i_size);
		printf("%8x", (int)ip->i_mroot&0xfffffff);
		printf("\n");
	}
	free(xinode);
}

getw(loc)
	off_t loc;
{
	int word;

	if (kflg)
		loc &= 0x7fffffff;
	xxseek(fc, loc, 0);
	read(fc, &word, sizeof (word));
	if (kflg)
		word &= 0x7fffffff;
	return (word);
}

putf(v, n)
{
	if (v)
		printf("%c", n);
	else
		printf(" ");
}

dotext()
{
	register struct text *xp;
	int ntext;
	struct text *xtext, *atext;
	int ntx;

	ntx = 0;
	ntext = getw(nl[SNTEXT].n_value);
	xtext = (struct text *)calloc(ntext, sizeof (struct text));
	xxseek(fc, (int)(atext = (struct text *)(nl[STEXT].n_value)), 0);
	read(fc, xtext, ntext * sizeof (struct text));
	for (xp = xtext; xp < &xtext[ntext]; xp++)
		if (xp->x_iptr!=NULL)
			ntx++;
	printf("%5d/%5d active texts\n", ntx, ntext);
	if (totflg)
		return;
	printf("   LOC   FLAGS DADDR      CADDR  RSS SIZE      IPTR  CNT CCNT\n");
	for (xp = xtext; xp < &xtext[ntext]; xp++) {
		if (xp->x_iptr == NULL)
			continue;
		printf("%8.1x", atext + (xp - xtext));
		printf(" ");
		putf(xp->x_flag&XPAGI, 'P');
		putf(xp->x_flag&XTRC, 'T');
		putf(xp->x_flag&XWRIT, 'W');
		putf(xp->x_flag&XLOAD, 'L');
		putf(xp->x_flag&XLOCK, 'K');
		putf(xp->x_flag&XWANT, 'w');
		printf("%5x", xp->x_daddr[0]);
		printf("%11x", xp->x_caddr);
		printf("%5d", xp->x_rssize);
		printf("%5d", xp->x_size);
		printf("%10.1x", xp->x_iptr);
		printf("%5d", xp->x_count&0377);
		printf("%5d", xp->x_ccount);
		printf("\n");
	}
	free(xtext);
}

doproc()
{
	struct proc *xproc, *aproc;
	int nproc;
	register struct proc *pp;
	register np;

	nproc = getw(nl[SNPROC].n_value);
	xproc = (struct proc *)calloc(nproc, sizeof (struct proc));
	xxseek(fc, (int)(aproc = (struct proc *)(nl[SPROC].n_value)), 0);
	read(fc, xproc, nproc * sizeof (struct proc));
	np = 0;
	for (pp=xproc; pp < &xproc[nproc]; pp++)
		if (pp->p_stat)
			np++;
	printf("%5d/%5d processes\n", np, nproc);
	if (totflg)
		return;
	printf("   LOC    S    F     ADDR PRI  SIG  UID SLP TIM  CPU  NI   PGRP    PID   PPID  RSS SRSS SIZE    WCHAN    LINK   TEXTP CLKT\n");
	for (pp=xproc; pp<&xproc[nproc]; pp++) {
		if (pp->p_stat==0 && allflg==0)
			continue;
		printf("%8x", aproc + (pp - xproc));
		printf(" %2d", pp->p_stat);
		printf(" %4x", pp->p_flag & 0xffff);
		printf(" %8x", pp->p_addr);
		printf(" %3d", pp->p_pri);
		printf(" %4x", pp->p_sig);
		printf(" %4d", pp->p_uid);
		printf(" %3d", pp->p_slptime);
		printf(" %3d", pp->p_time);
		printf(" %4d", pp->p_cpu&0377);
		printf(" %3d", pp->p_nice);
		printf(" %6d", pp->p_pgrp);
		printf(" %6d", pp->p_pid);
		printf(" %6d", pp->p_ppid);
		printf(" %4x", pp->p_rssize);
		printf(" %4x", pp->p_swrss);
		printf(" %5x", pp->p_dsize+pp->p_ssize);
		printf(" %7x", clear(pp->p_wchan));
		printf(" %7x", clear(pp->p_link));
		printf(" %7x", clear(pp->p_textp));
		printf("    %u", pp->p_clktim);
		printf("\n");
	}
}

dousr()
{
	struct user U;
	register i, *ip;

	/* This wins only if PAGSIZ > sizeof (struct user) */
	xxseek(fc, ubase * NBPG, 0);
	read(fc, &U, sizeof(U));
	printf("pcb");
	ip = (int *)&U.u_pcb;
	while (ip < &U.u_arg[0]) {
		if ((ip - (int *)&U.u_pcb) % 4 == 0)
			printf("\t");
		printf("%x ", *ip++);
		if ((ip - (int *)&U.u_pcb) % 4 == 0)
			printf("\n");
	}
	if ((ip - (int *)&U.u_pcb) % 4 != 0)
		printf("\n");
	printf("arg\t");
	for (i=0; i<5; i++)
		printf(" %.1x", U.u_arg[i]);
	printf("\n");
	for (i=0; i<sizeof(label_t)/sizeof(int); i++) {
		if (i%5==0)
			printf("\t");
		printf("%9.1x", U.u_ssav[i]);
		if (i%5==4)
			printf("\n");
	}
	if (i%5)
		printf("\n");
	printf("segflg\t%d\nerror %d\n", U.u_segflg, U.u_error);
	printf("uids\t%d,%d,%d,%d\n", U.u_uid,U.u_gid,U.u_ruid,U.u_rgid);
	printf("procp\t%.1x\n", U.u_procp);
	printf("ap\t%.1x\n", U.u_ap);
	printf("r_val?\t%.1x %.1x\n", U.u_r.r_val1, U.u_r.r_val2);
	printf("base, count, offset %.1x %.1x %ld\n", U.u_base,
		U.u_count, U.u_offset);
	printf("cdir rdir %.1x %.1x\n", U.u_cdir, U.u_rdir);
	printf("file\t");
	for (i=0; i<10; i++)
		printf("%9.1x", U.u_ofile[i]);
	printf("\n\t");
	for (i=10; i<NOFILE; i++)
		printf("%9.1x", U.u_ofile[i]);
	printf("\n");
	printf("pofile\t");
	for (i=0; i<10; i++)
		printf("%9.1x", U.u_pofile[i]);
	printf("\n\t");
	for (i=10; i<NOFILE; i++)
		printf("%9.1x", U.u_pofile[i]);
	printf("\n");
	printf("ssav");
	for (i=0; i<sizeof(label_t)/sizeof(int); i++) {
		if (i%5==0)
			printf("\t");
		printf("%9.1x", U.u_ssav[i]);
		if (i%5==4)
			printf("\n");
	}
	if (i%5)
		printf("\n");
	printf("sigs\t");
	for (i=0; i<NSIG; i++)
		printf("%.1x ", U.u_signal[i]);
	printf("\n");
	printf("ar0\t%.1x\n", U.u_ar0);
	printf("prof\t%X %X %X %X\n", U.u_prof.pr_base, U.u_prof.pr_size,
	    U.u_prof.pr_off, U.u_prof.pr_scale);
	/*
	printf("ttyp\t%.1x\n", U.u_ttyp);
	printf("ttyd\t%d,%d\n", major(U.u_ttyd), minor(U.u_ttyd));
	*/
	printf("exdata\t");
	ip = (int *)&U.u_exdata;
	for (i = 0; i < 8; i++)
		printf("%.1D ", *ip++);
	printf("\n");
	printf("comm %.14s\n", U.u_comm);
	printf("start\t%D\n", U.u_start);
	printf("acflag\t%D\n", U.u_acflag);
	printf("cmask\t%D\n", U.u_cmask);
	printf("sizes\t%.1x %.1x %.1x\n", U.u_tsize, U.u_dsize, U.u_ssize);
	printf("vm\t");
	ip = (int *)&U.u_vm;
	for (i = 0; i < sizeof(U.u_vm)/sizeof(int); i++)
		printf("%D ", ip[i]);
	printf("\n");
	ip = (int *)&U.u_cvm;
	printf("cvm\t");
	for (i = 0; i < sizeof(U.u_vm)/sizeof(int); i++)
		printf("%D ", ip[i]);
	printf("\n");
/*
	i =  U.u_stack - &U;
	while (U[++i] == 0);
	i &= ~07;
	while (i < 512) {
		printf("%x ", 0140000+2*i);
		for (j=0; j<8; j++)
			printf("%9x", U[i++]);
		printf("\n");
	}
*/
}

oatoi(s)
char *s;
{
	register v;

	v = 0;
	while (*s)
		v = (v<<3) + *s++ - '0';
	return(v);
}

dofile()
{
	int nfile;
	struct file *xfile, *afile;
	register struct file *fp;
	register nf;

	nf = 0;
	nfile = getw(nl[SNFILE].n_value);
	xfile = (struct file *)calloc(nfile, sizeof (struct file));
	xxseek(fc, (int)(afile = (struct file *)(nl[SFIL].n_value)), 0);
	read(fc, xfile, nfile * sizeof (struct file));
	for (fp=xfile; fp < &xfile[nfile]; fp++)
		if (fp->f_count)
			nf++;
	printf("%5d/%5d open files\n", nf, nfile);
	if (totflg)
		return;
	printf("   LOC   FLG  CNT   INO    OFFS\n");
	for (fp=xfile; fp < &xfile[nfile]; fp++,afile++) {
		if (fp->f_count==0)
			continue;
		printf("%8x ", afile);
		putf(fp->f_flag&FREAD, 'R');
		putf(fp->f_flag&FWRITE, 'W');
		putf(fp->f_flag&FHUNGUP, 'H');
		printf("%4d", mask(fp->f_count));
		printf("%9.1x", fp->f_inode);
		printf("  %ld\n", fp->f_offset);
	}
}

xxseek(f, l, w)
long l;
{
	return(lseek(f, l&seekmask, w));
}
