#define _RESEARCH_SOURCE
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <libv.h>
#include "../lib/lib.h"

int qflag;
int tflag;
int vflag;

int nerr;
struct sub *pathsublist;

int
filter(FILE *ifp, FILE *ofp)
{
	struct tarbuf buf;
	struct tarhdr hdr;
	char *ptr;
	int blocks;
	static char z[2 * TSIZE];
	char name[4 * TNAMEMAX + 1];

	if (fread(&buf, sizeof buf, 1, ifp) != 1) {
		++nerr;
		fprintf(stderr, "tarf: premature eof on input\n");
		return 0;
	}
	if (memcmp(&buf, z, TSIZE) == 0) {
		if (!tflag && fwrite(z, 1, sizeof z, ofp) != sizeof z) {
			++nerr;
			fprintf(stderr, "tarf: error writing eof header\n");
		}
		return 0;
	}
	if (thdrget(&hdr, &buf) != 0) {
		++nerr;
		fprintf(stderr, "tarf: bad input header\n");
		return 0;
	}
	if (!sub(&pathsublist, 1, hdr.name, TNAMEMAX)) {
		++nerr;
		fprintf(stderr, "tarf: can't expand '%s'\n", hdr.name);
		return 0;
	}
	if (hdr.typeflag == LNKTYPE || hdr.typeflag == SYMTYPE)
		sub(&pathsublist, 1, hdr.linkname, TLINKMAX);
	blocks = (hdr.size + TSIZE - 1) / TSIZE;
	if (qflag)
		quote(name, hdr.name);
	else
		strcpy(name, hdr.name);
	if (vflag)
		fprintf(stderr, "%s\n", name);
	if (tflag) {
		fprintf(ofp, "%s\n", name);
		if (hdr.typeflag == REGTYPE)
			discard(ifp, blocks * TSIZE);
	} else {
		if (thdrput(&buf, &hdr) != 0) {
			++nerr;
			fprintf(stderr, "tarf: error building output header\n");
			return 0;
		}
		if (fwrite(&buf, sizeof buf, 1, ofp) != 1) {
			++nerr;
			fprintf(stderr, "tarf: error writing output header\n");
			return 0;
		}
		if (hdr.typeflag == REGTYPE)
			if (fpcopy(ofp, ifp, blocks * TSIZE) != blocks * TSIZE) {
				++nerr;
				fprintf(stderr, "tarf: error copying data\n");
				return 0;
			}
	}
	return 1;
}

void
usage(void)
{
	fprintf(stderr, "usage: tarf [-qtv] [-Dold=new ...]\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	int c;
	char *nstr;

	prog = argv[0];
	while ((c = getopt(argc, argv, "qtvD:")) != EOF)
		switch (c) {
		case 'q':
			++qflag;
			break;
		case 't':
			++tflag;
			break;
		case 'v':
			++vflag;
			break;
		case 'D':
			nstr = strchr(optarg, '=');
			if (nstr) {
				*nstr = '\0';
				addsub(&pathsublist, optarg, nstr + 1);
			} else
				usage();
			break;
		default:
			usage();
			break;
		}
	if (argc > 1 && optind != argc)
		usage();
	while (filter(stdin, stdout))
		;
	if (ferror(stdin) || !feof(stdin))
		return 1;
	return nerr ? 1 : 0;
}
