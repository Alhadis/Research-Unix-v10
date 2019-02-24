/*
 * write config tables and addresses
 */

#include <libc.h>
#include "mkconf.h"

writconf(f)
char *f;
{
	int fd;
	register Dev *dp;
	register int i;
	register int max;
	register Include *ip;
	Dev *putaddr();

	if ((fd = creat(f, 0666)) < 0) {
		perror(f);
		exit(1);
	}
	fprint(fd, "/*\n * config data\n */\n\n");
	fprint(fd, "#include \"sys/param.h\"\n");
	fprint(fd, "#include \"sys/conf.h\"\n");
	for (ip = inclist; ip; ip = ip->next)
		if (ip->inuse) {
			fprint(fd, "#include \"%s\"\n", ip->incl);
			while (ip->next && ip->next->incl == ip->incl)
				ip = ip->next;
		}
	/*
	 * device switches
	 */
	max = 0;
	for (i = 0; i <= nblk; i++)
		if (bdevsw[i].used) {
			fprint(fd, "extern struct bdevsw %sbdev;\n", bdevsw[i].dev->tag);
			max = i;
		}
	fprint(fd, "int nblkdev = %d;\n", max + 1);
	nblk = max;
	max = 0;
	for (i = 0; i <= nchr; i++)
		if (cdevsw[i].used) {
			fprint(fd, "extern struct cdevsw %scdev;\n", cdevsw[i].dev->tag);
			max = i;
		}
	fprint(fd, "int nchrdev = %d;\n", max + 1);
	nchr = max;
	max = 0;
	for (i = 0; i <= nfs; i++)
		if (fstypsw[i].used) {
			fprint(fd, "extern struct fstypsw %sfs;\n", fstypsw[i].dev->tag);
			max = i;
		}
	fprint(fd, "int nfstyp = %d;\n", max + 1);
	nfs = max;
	max = 0;
	for (i = 0; i <= nld; i++)
		if (ldispsw[i].used) {
			fprint(fd, "extern struct streamtab %sstream;\n", ldispsw[i].dev->tag);
			max = i;
		}
	fprint(fd, "int nstreamtab = %d;\n", max + 1);
	nld = max;
	fprint(fd, "\nstruct bdevsw *bdevsw[] = {\n");
	for (i = 0; i <= nblk; i++)
		if (bdevsw[i].used)
			fprint(fd, "\t&%sbdev,\t/* %d */\n", bdevsw[i].dev->tag, i);
		else
			fprint(fd, "\tNULL,\n");
	fprint(fd, "};\nstruct cdevsw *cdevsw[] = {\n");
	for (i = 0; i <= nchr; i++)
		if (cdevsw[i].used)
			fprint(fd, "\t&%scdev,\t/* %d */\n", cdevsw[i].dev->tag, i);
		else
			fprint(fd, "\tNULL,\n");
	fprint(fd, "};\nstruct fstypsw *fstypsw[] = {\n");
	for (i = 0; i <= nfs; i++)
		if (fstypsw[i].used)
			fprint(fd, "\t&%sfs,\t/* %d */\n", fstypsw[i].dev->tag, i);
		else
			fprint(fd, "\tNULL,\n");
	fprint(fd, "};\nstruct streamtab *streamtab[] = {\n");
	for (i = 0; i <= nld; i++)
		if (ldispsw[i].used)
			fprint(fd, "\t&%sstream,\t/* %d */\n", ldispsw[i].dev->tag, i);
		else
			fprint(fd, "\tNULL,\n");
	fprint(fd, "};\n");
	/*
	 * put out addresses and data structures
	 */
	for (dp = dlist; dp; dp = advtype(dp))
		(*acode[dp->mdev->atype].putaddr)(fd, dp);
	/*
	 * other stuff
	 */
	fprint(fd, "int rootfstyp = %d;\n", rootfs);
	fprint(fd, "dev_t rootdev = makedev(%d, %d);\n", rootmaj, rootmin);
	fprint(fd, "dev_t swapdev = makedev(%d, %d);\n", swapmaj, swapmin);
	fprint(fd, "struct swdevt swdevt[] = {\n");
	for (i = 0; i < nswapdev; i++)
		fprint(fd, "\t{makedev(%d, %d), %ldL},\n",
			swaptab[i].major, swaptab[i].minor, swaptab[i].size);
	fprint(fd, "};\nint nswdevt = %d;\n", nswapdev);
	if (dumprout == NULL)
		dumprout = "nulldev";
	fprint(fd, "extern int %s();\nint (*dumprout)() = %s;\n", dumprout, dumprout);
	fprint(fd, "int dumpunit = %d;\nlong dumplow = %ld;\nlong dumpsize = %ld;\n",
		dumpunit, dumplow, dumpsize);
	close(fd);
}
