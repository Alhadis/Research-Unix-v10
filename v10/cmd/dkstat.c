#include <sys/dkstat.h>
#include <nlist.h>
#include <ctype.h>

struct nlist nl[] ={
	{"_dkstat"},
	{"_dk"},
	{"_ndkline"},
	{ "" },
};

int	mem;
int	every;

int	val;
long	total;
char	*sys = "/unix";
char	*core = "/dev/kmem";
struct	dkstat	dko, dkn;
int	cflag;

#define	R(m, f)	if (dko.f != dkn.f) printf("%8D  %s\n", (long)(dkn.f-dko.f), m)

main(argc, argv)
char **argv;
{
	int i;

	if (argc>1 && strcmp(argv[1], "-c")==0) {
		cflag++;
		argc--;
		argv++;
	}
	if (argc>1 && isdigit(argv[1][0])) {
		every = atoi(argv[1]);
		argc--;
		argv++;
	}
	if (argc > 1)
		sys = argv[1];
	if (argc > 2)
		core = argv[2];
	nlist(sys, nl);
	if (nl[0].n_value <= 0) {
		printf("No namelist in %s\n", sys);
		exit(1);
	}
	mem = open(core, 0);
	if (mem < 0) {
		printf("can't open %s\n", core);
		exit(1);
	}
	if (cflag) {
		int ndkline = 0, i;
		struct dk {
			int	*q;
			char	chan;
			char	state;
		} dk[64];

		lseek(mem, (long)nl[2].n_value, 0);
		read(mem, (char *)&ndkline, sizeof(ndkline));
		if (ndkline<=0 || ndkline>=64)
			ndkline = 32;
		lseek(mem, (long)nl[1].n_value, 0);
		read(mem, (char *)dk, sizeof(dk));
		for (i=0; i<ndkline; i++)
			if (dk[i].state)
				printf("%d	%c\n", i, "xrlo"[dk[i].state]);
	}
	for (;;) {
		lseek(mem, (long)nl[0].n_value, 0);
		read(mem, (char *)&dkn, sizeof(dkn));
		R("in", input);
		R("out", output);
		R("mark fault", markflt);
		R("mark parity", markparity);
		R("packets on closed channels", closepack);
		R("packets on channel 0", pack0);
		R("packets on peculiar channels", packstrange);
		R("short packets", shortpack);
		R("parity", parity);
		R("T_CHG on peculiar channel", chgstrange);
		R("packets on hung-up channels", notclosed);
		R("IS_CLOSED packets on open channels", isclosed);
		R("dkp retransmissions", dkprxmit);
		R("dkp trailer size rejects", dkprjtrs);
		R("dkp message size rejects", dkprjpks);
		R("dkp seq number rejects", dkprjseq);
		if (every == 0)
			break;
		printf("\n");
		sleep(every);
		dko = dkn;
	}
	return(0);
}
