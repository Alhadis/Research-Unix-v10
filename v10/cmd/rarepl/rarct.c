/*% cc -O -o rarct %
 *
 * read the rct and other stuff
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/udaioc.h>
#include "rct.h"

struct ud_unit ud_unit;

main(argc, argv)
int argc;
char **argv;
{
	int hdr = 0;
	int stat = 0;
	int nd = 0;
	int errs = 0;

	while (--argc > 0) {
		if (**++argv == '-')
			switch (argv[0][1]) {
			case 'h':
				hdr++;
				continue;

			case 'c':
				stat++;
				continue;

			default:
				fprintf(stderr, "%s ignored\n", *argv);
				continue;
			}
		nd++;
		errs += dodev(*argv, hdr, stat);
	}
	if (nd == 0) {
		fprintf(stderr, "usage: %s dev\n", argv[0]);
		exit(1);
	}
	exit(errs);
}

dodev(dev, hdr, stat)
char *dev;
int hdr, stat;
{
	int fd;
	daddr_t size;
	char buf[RBNSEC];
	register daddr_t i;
	daddr_t rctmax();

	if ((fd = open(dev, 0)) < 0) {
		perror(dev);
		return (1);
	}
	/*
	 * hack to force driver to read status
	 */
	if (read(fd, buf, RBNSEC) != RBNSEC)
		perror("read first block");
	if (ioctl(fd, UIOCHAR, &ud_unit) < 0) {
		perror("ioctl");
		close(fd);
		return (1);
	}
	if (stat)
		statput();
	if (hdr) {
		rctread(fd, buf, RCTHDR);
		hdrput(buf);
	}
	if (hdr || stat) {
		close(fd);
		return (0);
	}
	size = rctmax();
	for (i = RCTTAB; i < size; i++) {
		rctread(fd, buf, i);
		rctput(buf, i);
	}
	close(fd);
	return (0);
}

daddr_t
rctmax()
{
	register daddr_t nrbns;

	nrbns = (ud_unit.radsize / ud_unit.tracksz) * ud_unit.rbns;
	nrbns = ((nrbns+RBNPB-1) / RBNPB) + RCTTAB;
	if (nrbns >= ud_unit.rctsize)
		return ((daddr_t)ud_unit.rctsize);
	return (nrbns);
}

rctread(fd, buf, bno)
char *buf;
daddr_t bno;
{
	struct ud_rctbuf rb;
	register int i;

	rb.lbn = bno;
	rb.buf = buf;
	if (ioctl(fd, UIORRCT, &rb) < 0) {
		perror("read rct");
		fprintf(stderr, "block %d\n", bno);
		for (i = 0; i < RBNSEC; i++)
			buf[i] = 0;
	}
}

rctput(buf, bno)
char *buf;
int bno;
{
	register long rbn;
	register struct rbd *rp;
	register int i;

	rbn = (bno - RCTTAB) * RBNPB;
	rp = (struct rbd *)buf;
	for (i = 0; i < RBNPB; i++, rp++, rbn++) {
		if (rp->rb_code == RFREE || rp->rb_code == RNULL)
			continue;
		printf("%ld: %o: %ld\n", rbn, rp->rb_code, rp->rb_lbn);
	}
}

hdrput(buf)
char *buf;
{
	register struct rct *rc;

	rc = (struct rct *)buf;
	printf("volser x%lx x%lx\n", rc->rc_volser[0], rc->rc_volser[1]);
	printf("flags 0%x lbn %ld rbn %ld badrbn %ld\n",
		rc->rc_flags, rc->rc_lbn, rc->rc_rbn, rc->rc_badrbn);
}

char *
mtoa(m)
	long m;
{
	static char buf[10];
	char *s = buf;
	register k;

#define	M5(b, s)	if(k = ((m>>(b-4))&0x1F)) *s++ = 'A'-1+k

	M5(31, s);
	M5(26, s);
	M5(21, s);
	M5(16, s);
	M5(11, s);
	m &= 0x7F;
	*s++ = '0'+(m/10);
	*s++ = '0'+(m%10);
	*s = 0;
	return(buf);
}

statput()
{

	printf("size %ld\nrctsize %ld\n", ud_unit.radsize, ud_unit.rctsize);
	printf("medium x%lx='%s'\n", ud_unit.medium, mtoa(ud_unit.medium));
	printf("tracks %d groups %d cyls %d\n",
		ud_unit.tracksz, ud_unit.groupsz, ud_unit.cylsz);
	printf("rbns/track %d\ncopies %d\n", ud_unit.rbns, ud_unit.copies);
}
