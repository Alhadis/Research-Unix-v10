/*% cc -O -o rarepl %
 *
 * replace a bad sector on an RA disk
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/udaioc.h>
#include "rct.h"

struct ud_unit ud_unit;
int primblk;		/* ugh */
int notprim;		/* never primary replacement */

#define	BADRBN	(-1L)

main(argc, argv)
int argc;
char **argv;
{
	int fd;
	int errs = 0;

	if (argc < 3) {
		fprintf(stderr, "usage: %s dev lbn ...\n", argv[0]);
		exit(1);
	}
	if ((fd = open(argv[1], 2)) < 0) {
		perror(argv[1]);
		exit(1);
	}
	online(fd);
	if (ioctl(fd, UIOCHAR, &ud_unit) < 0) {
		perror("unit info");
		exit(1);
	}
	argc--;
	argv++;
	while (--argc > 0)
		errs += repl(fd, atol(*++argv));
	exit(errs);
}

/*
 * read a block from the disk,
 * just to make the driver bring it online
 */

online(fd)
int fd;
{
	char buf[RBNSEC];

	read(fd, buf, RBNSEC);		/* ignore error; might be bad sector */
}

/*
 * replace a block
 */

repl(fd, bad)
int fd;
daddr_t bad;
{
	struct ud_repl r;
	char block[RBNSEC];
	struct rbd rct[RBNPB];
	daddr_t rctb;
	int rctoff;
	daddr_t rbn;
	daddr_t pickrbn();

	if (bad >= ud_unit.radsize) {
		fprintf(stderr, "%ld: out of range\n", bad);
		return (1);
	}
	clrbuf(block);
	lseek(fd, (off_t)(bad * RBNSEC), 0);
	read(fd, block, RBNSEC);	/* ignore error on purpose */
	if ((rbn = pickrbn(fd, bad)) == BADRBN) {
		fprintf(stderr, "can't find replacement for %ld\n", bad);
		return (1);
	}
	rctb = (rbn / RBNPB) + RCTTAB;
	rctoff = rbn % RBNPB;
	if (rdrct(fd, rctb, (char *)rct) == 0)
		return (1);
	if (rct[rctoff].rb_code) {
		fprintf(stderr, "rbn %ld in use or bad\n", rbn);
		return (1);
	}
	rct[rctoff].rb_code = primblk ? RALLOC : RALLOC | RALT;
	rct[rctoff].rb_lbn = bad;
	if (wrrct(fd, rctb, (char *)rct) == 0)
		return (1);
	r.lbn = bad;
	r.replbn = rbn;
	r.prim = primblk;
	if (ioctl(fd, UIOREPL, &r) < 0) {
		perror("replace ioctl");
		return (1);
	}
	lseek(fd, (off_t)(bad * RBNSEC), 0);
	if (write(fd, block, RBNSEC) != RBNSEC) {
		perror("write back");
		return (1);
	}
	return (0);
}

/*
 * find a replacement block
 * remember in primblk whether it's the primary replacement
 */

daddr_t
pickrbn(fd, lbn)
int fd;
daddr_t lbn;
{
	struct rbd rct[RBNPB];
	daddr_t bno, rbn, prbn;
	daddr_t low, high;
	register int i;
	daddr_t size;
	daddr_t rctmax(), primrbn();

	size = rctmax();
	prbn = primrbn(lbn);
	low = high = BADRBN;
	for (bno = RCTTAB, rbn = 0L; bno < size; bno++) {
		if (rdrct(fd, bno, (char *)rct) == 0) {
			rbn += RBNPB;
			continue;
		}
		for (i = 0; i < RBNPB; i++, rbn++) {
			if (rct[i].rb_code == RFREE) {
				if (rbn < prbn)
					low = rbn;
				else if (high == BADRBN)
					high = rbn;
			}
			else if (rct[i].rb_lbn == lbn
			     &&  rct[i].rb_code != RNULL
			     &&  rct[i].rb_code != RBAD) {
				rct[i].rb_code = RBAD;
				wrrct(fd, bno, (char *)rct);
			}
		}
	}
	if (low == BADRBN && high == BADRBN)
		return (BADRBN);
	else if (low == BADRBN)
		rbn = high;
	else if (high == BADRBN)
		rbn = low;
	else if (prbn - low < high - prbn)
		rbn = low;
	else
		rbn = high;
	if (notprim)
		primblk = 0;
	else
		primblk = (rbn == prbn);
	return (rbn);
}

daddr_t
rctmax()
{
	register daddr_t nrbns;

	nrbns = (ud_unit.radsize / ud_unit.tracksz) * ud_unit.rbns;
	nrbns = ((nrbns+RBNPB-1) / RBNPB) + RCTTAB;
	if (nrbns >= ud_unit.rctsize) {
		notprim = 1;
		return ((daddr_t)ud_unit.rctsize);
	}
	return (nrbns);
}

/*
 * figure out the primary RBN
 * if notprim == 0, we understand what to do
 * otherwise it's not really the primary RBN (and we won't tell the
 * controller so), but make a guess about a preferred RBN anyway
 */
daddr_t
primrbn(lbn)
daddr_t lbn;
{
	daddr_t q, csize;

	if (notprim == 0)
		return ((lbn/ud_unit.tracksz) * ud_unit.rbns);
	/*
	 * try for one in the same cylinder,
	 * or multiple thereof
	 */
	q = (ud_unit.rctsize - RCTTAB)*RBNPB;	/* max possible rbns */
	q /= ud_unit.rbns;		/* max rbn quantum */
	q = ud_unit.radsize/q;		/* max LBNs per RBN */
	csize = ud_unit.tracksz*ud_unit.groupsz*ud_unit.cylsz;
	q = (q + csize - 1)/csize;	/* cylinders per RBN */
	q *= csize;			/* probably real LBNs per RBN */
	return ((lbn + csize - 1)/q);
}

rdrct(fd, bno, buf)
int fd;
daddr_t bno;
char *buf;
{
	struct ud_rctbuf b;

	b.buf = buf;
	b.lbn = bno;
	if (ioctl(fd, UIORRCT, &b) < 0) {
		perror("rrct");
		fprintf(stderr, "can't read block %ld of rct\n", bno);
		return (0);
	}
	return (1);
}

wrrct(fd, bno, buf)
int fd;
daddr_t bno;
char *buf;
{
	struct ud_rctbuf b;

	b.buf = buf;
	b.lbn = bno;
	if (ioctl(fd, UIOWRCT, &b) < 0) {
		perror("wrct");
		fprintf(stderr, "can't write block %ld of rct\n", bno);
		return (0);
	}
	return (1);
}

clrbuf(b)
register char *b;
{
	register int i;

	for (i = 0; i < RBNSEC; i++)
		*b++ = 0;
}
