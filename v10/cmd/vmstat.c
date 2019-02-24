#include <stdio.h>
#include <libc.h>
#include <sys/param.h>
#include <sys/vm.h>
#include <nlist.h>
#include <sys/meter.h>

struct nlist nl[] = {
#define	X_CPTIME	0
	{ "_cp_time" },
#define	X_RATE		1
	{ "_rate" },
#define X_TOTAL		2
	{ "_total" },
#define	X_DEFICIT	3
	{ "_deficit" },
#define X_SUM		4
	{ "_sum" },
#define	X_FIRSTFREE	5
	{ "_firstfree" },
#define	X_MAXFREE	6
	{ "_maxfree" },
#define	X_BOOTIME	7
	{ "_bootime" },
#define X_REC		8
	{ "_rectime" },
#define X_PGIN		9
	{ "_pgintime" },
	{ 0 },
};

double	stat1();
int	firstfree, maxfree;
struct
{
	int	busy;
	long	time[CPUSTATES];
	struct	vmmeter Rate;
	struct	vmtotal	Total;
	struct	vmmeter Sum;
	unsigned rectime;
	unsigned pgintime;
} s, s1, z;
#define	rate		s.Rate
#define	total		s.Total
#define	sum		s.Sum

int	zero;
int	deficit;
double	etime;
int 	mf;

main(argc, argv)
char **argv;
{
	time_t now;
	int lines;
	extern char *ctime();
	register i;
	int iter, nintv;
	time_t bootime;
	long t;
	extern char _sobuf[];

	setbuf(stdout, _sobuf);
	nlist("/unix", nl);
	if(nl[0].n_type == 0) {
		printf("no /unix namelist\n");
		exit(1);
	}
	mf = open("/dev/kmem", 0);
	if(mf < 0) {
		printf("cannot open /dev/kmem\n");
		exit(1);
	}
	iter = 0;
	argc--, argv++;
	while (argc>0 && argv[0][0]=='-') {
		char *cp = *argv++;
		argc--;
		while (*++cp) switch (*cp) {

		case 't':
			dotimes();
			exit(0);
		case 'z':
			close(mf);
			mf = open("/dev/kmem", 2);
			lseek(mf, (long)nl[X_SUM].n_value, 0);
			write(mf, &z.Sum, sizeof z.Sum);
			exit(0);

		
		case 's':
			dosum();
			exit(0);

		default:
			fprintf(stderr, "usage: vmstat [ -fs ] [ interval ] [ count]\n");
			exit(1);
		}
	}
	if(argc > 1)
		iter = atoi(argv[1]);
	lseek(mf, (long)nl[X_FIRSTFREE].n_value, 0);
	read(mf, &firstfree, sizeof firstfree);
	lseek(mf, (long)nl[X_MAXFREE].n_value, 0);
	read(mf, &maxfree, sizeof maxfree);
	lseek(mf, (long)nl[X_BOOTIME].n_value, 0);
	read(mf, &bootime, sizeof bootime);
	time(&now);
	nintv = now - bootime;
	if (nintv <= 0 || nintv > 60*60*24*365*10) {
		printf("Time makes no sense... namelist must be wrong.\n");
		exit(1);
	}
reprint:
	lines = 20;
	/* s1 = z; */
printf("\
 procs|    memory   |            page           |       |    cpu\n\
 r b w    avm    fre  re  at  pi  po  fr  de  sr  sy  cs us sy id st\n");
loop:
	lseek(mf, (long)nl[X_CPTIME].n_value, 0);
 	read(mf, s.time, sizeof s.time);
	if (nintv != 1) {
		lseek(mf, (long)nl[X_SUM].n_value, 0);
		read(mf, &rate, sizeof rate);
	} else {
		lseek(mf, (long)nl[X_RATE].n_value, 0);
		read(mf, &rate, sizeof rate);
	}
	lseek(mf, (long)nl[X_TOTAL].n_value, 0);
	read(mf, &total, sizeof total);
	lseek(mf, (long)nl[X_DEFICIT].n_value, 0);
	read(mf, &deficit, sizeof deficit);
	etime = 0;
	for (i=0; i < CPUSTATES; i++) {
		t = s.time[i];
		s.time[i] -= s1.time[i];
		s1.time[i] = t;
		etime += s.time[i];
	}
	if(etime == 0.)
		etime = 1.;
	printf("%2d %d %d", total.t_rq, total.t_dw+total.t_pw, total.t_sw);
	printf(" %6d %6d", total.t_avm/CLSIZE, total.t_free/CLSIZE);
	printf(" %3d %3d %3d",
	    (rate.v_pgrec - (rate.v_xsfrec+rate.v_xifrec))/nintv,
	    (rate.v_xsfrec+rate.v_xifrec)/nintv, rate.v_pgpgin/CLSIZE/nintv);
	printf(" %3d %3d %3d %3d", rate.v_pgpgout/CLSIZE/nintv,
	    rate.v_dfree/CLSIZE/nintv, deficit/CLSIZE, rate.v_scan/nintv);
	etime /= 60.;
	printf(" %3d", rate.v_syscall/nintv);
	printf(" %3d", rate.v_swtch/nintv);
	for(i=0; i<CPUSTATES; i++) {
		float f = stat1(i);
		if (i == 0) {		/* US+NI */
			i++;
			f += stat1(i);
		}
		printf("%3.0f", f);
	}
	printf("\n");
	fflush(stdout);
	nintv = 1;
	--iter;
	if(iter)
	if(argc > 0) {
		sleep(atoi(argv[0]));
		if (--lines <= 0)
			goto reprint;
		goto loop;
	}
}

dotimes()
{

	lseek(mf, (long)nl[X_REC].n_value, 0);
	read(mf, &s.rectime, sizeof s.rectime);
	lseek(mf, (long)nl[X_PGIN].n_value, 0);
	read(mf, &s.pgintime, sizeof s.pgintime);
	lseek(mf, (long)nl[X_SUM].n_value, 0);
	read(mf, &sum, sizeof sum);
	printf("%d reclaims, %d total time (usec)\n", sum.v_pgrec, s.rectime);
	printf("average: %d usec / reclaim\n", s.rectime/sum.v_pgrec);
	printf("\n");
	printf("%d page ins, %d total time (msec)\n",sum.v_pgin, s.pgintime/10);
	printf("average: %8.1f msec / page in\n", s.pgintime/(sum.v_pgin*10.0));
}

dosum()
{

	lseek(mf, (long)nl[X_SUM].n_value, 0);
	read(mf, &sum, sizeof sum);
	printf("%9d swap ins\n", sum.v_swpin);
	printf("%9d swap outs\n", sum.v_swpout);
	printf("%9d pages swapped in\n", sum.v_pswpin / CLSIZE);
	printf("%9d pages swapped out\n", sum.v_pswpout / CLSIZE);
	printf("%9d total address trans. faults taken\n", sum.v_faults);
	printf("%9d page ins\n", sum.v_pgin);
	printf("%9d page outs\n", sum.v_pgout);
	printf("%9d pages paged in\n", sum.v_pgpgin);
	printf("%9d pages paged out\n", sum.v_pgpgout);
	printf("%9d sequential process pages freed\n", sum.v_seqfree);
	printf("%9d total reclaims\n", sum.v_pgrec);
	printf("%9d reclaims from free list\n", sum.v_pgfrec);
	printf("%9d intransit blocking page faults\n", sum.v_intrans);
	printf("%9d zero fill pages created\n", sum.v_nzfod / CLSIZE);
	printf("%9d zero fill page faults\n", sum.v_zfod / CLSIZE);
	printf("%9d executable fill pages created\n", sum.v_nexfod / CLSIZE);
	printf("%9d executable fill page faults\n", sum.v_exfod / CLSIZE);
	printf("%9d swap text pages found in free list\n", sum.v_xsfrec);
	printf("%9d inode text pages found in free list\n", sum.v_xifrec);
	printf("%9d pages examined by the clock daemon\n", sum.v_scan);
	printf("%9d revolutions of the clock hand\n", sum.v_rev);
	printf("%9d pages freed by the clock daemon\n", sum.v_dfree / CLSIZE);
	printf("%9d cpu context switches\n", sum.v_swtch);
	printf("%9d traps\n", sum.v_trap);
	printf("%9d system calls\n", sum.v_syscall);
}

double
stat1(row)
{
	double t;
	register i;

	t = 0;
	for(i=0; i<CPUSTATES; i++)
		t += s.time[i];
	if(t == 0.)
		t = 1.;
	return(s.time[row]*100./t);
}

pct(top, bot)
{

	if (bot == 0)
		return (0);
	return ((top * 100) / bot);
}
