/*
 * make an empty bitmapped filesystem
 */

#include <stdio.h>
#include <libc.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/filsys.h>
#include <sys/dir.h>
#include <sys/ino.h>
#include <sys/inode.h>

#define	ISBIT	0100	/* this is a bitmapped filesystem -- fake out macros */
#define	MAXSMALL	(BITMAP*BITCELL)	/* biggest in-superb fs */
#define	MAXINO	65536L	/* ino_t is a short */

#define	ICOUNT	INOPB(ISBIT)
#define	BCOUNT	BSIZE(ISBIT)

struct filsys sb;
union {
	struct dinode ib[ICOUNT];
	struct direct db[BCOUNT/sizeof(struct direct)];
	long fb[BCOUNT/sizeof(long)];
	char bb[BCOUNT];
} buf;
struct stat statbuf;
long where[NADDR];
int cylsize = 40;
int space = 4;		/* ok for comets, suboptimal elsewhere */

main(argc, argv)
char **argv;
{
	int i, j;	/* only for test read */
	daddr_t size, isize;
	register daddr_t bno;
	int fd;
	register struct dinode *ip;
	llong_t off;
	long atol();
	extern llong_t Llmul(), ltoL();

	if(argc < 3) {
		fprintf(stderr, "%s: bit-dev no-of-4k-blocks [ space cylsize ]\n", argv[0]);
		exit(1);
	}
	if (argc > 3)
		space = atoi(argv[3]);
	if (argc > 4)
		cylsize = atoi(argv[4]);
	fd = open(argv[1], 2);
	if(fd < 0) {
		perror(argv[1]);
		exit(1);
	}
	if(fstat(fd, &statbuf) < 0) {
		perror(argv[1]);
		exit(1);
	}
	if(!BITFS(statbuf.st_rdev)) {	/* doubtful */
		fprintf(stderr, "%s device %d, 0%o can't have a 4k filesystem\n",
			argv[1], major(statbuf.st_rdev), minor(statbuf.st_rdev));
		exit(1);
	}
	size = atol(argv[2]);
	if(size <= 3) {
		fprintf(stderr, "size %ld too small\n", size);
		exit(1);
	}
	off = Llmul(ltoL(size-1), BCOUNT);
	llseek(fd, off, 0);
	if((j = read(fd, buf.bb, BCOUNT)) != BCOUNT) {
		fprintf(stderr, "size %ld too large (lseek [%d,%d], read %d)\n",
			size, Lsign(off), Ltol(off), j);
		exit(1);
	}
	isize = (size - 2)/(1 + ICOUNT);	/* DOUBTFUL */
	if(isize * ICOUNT > MAXINO)
		isize = MAXINO/ICOUNT;	/* 65535 is largest short */
	fprintf(stderr, "%ld 4k blocks, %ld blocks of inodes, %d inodes\n",
		size, isize - 2, ICOUNT * (isize - 2));
	/* zero out all inodes */
	lseek(fd, (off_t)((SUPERB+1) * BCOUNT), 0);
	zerobuf(buf.bb);
	for(bno = SUPERB+1; bno < isize; bno++) {
		if (write(fd, (char *)buf.ib, BCOUNT) != BCOUNT) {
			perror("inode write");
			exit(1);
		}
	}
	/* next block has the root directory */
	where[0] = isize;
	buf.db[0].d_ino = ROOTINO;
	buf.db[0].d_name[0] = '.';
	buf.db[1].d_ino = ROOTINO;
	buf.db[1].d_name[1] = buf.db[1].d_name[0] = '.';
	if (write(fd, (char *)buf.db, BCOUNT) != BCOUNT) {
		perror("root dir write");
		exit(1);
	}
	/* now for its inode */
	zerobuf(buf.bb);
	ip = &buf.ib[itoo(ISBIT, ROOTINO)];
	ip->di_mode = IFDIR | IREAD | IWRITE | IEXEC;
	ip->di_mode |= (IREAD | IEXEC | IWRITE) >> 3;
	ip->di_mode |= (IREAD | IEXEC | IWRITE) >> 6;
	ip->di_nlink = 2;
	ip->di_uid = ip->di_gid = 0;
	ip->di_size = 2*sizeof(struct direct);
	ltol3(ip->di_addr, where, 1);
	ip->di_atime = ip->di_mtime =
		ip->di_ctime = time((long *)0);
	lseek(fd, (off_t)(itod(ISBIT, ROOTINO)*BCOUNT), 0);
	if (write(fd, (char *)buf.ib, BCOUNT) != BCOUNT) {
		perror("root inode write");
		exit(1);
	}
	/* and now the super block */
	sb.s_isize = isize;
	sb.s_fsize = size;
	sb.s_time = ip->di_atime;
	sb.s_tfree = size - isize - 1;
	sb.s_tinode = (isize - 2) * ICOUNT - 1;	/* doubtful */
	sb.s_cylsize = cylsize;
	sb.s_aspace = space;
	if (size - isize <= MAXSMALL)
		smallfree();
	else
		largefree(fd);
	sb.s_valid = 1;
	lseek(fd, (off_t)(SUPERB * BCOUNT), 0);
	if (write(fd, (char *)&sb, BCOUNT) != BCOUNT) {
		perror("superblock write");
		exit(1);
	}
	exit(0);
}

/*
 * make the bitmap, if it all fits in the superblock
 */
smallfree()
{
	register int i;
	register daddr_t bno;
	register daddr_t last;

	sb.U.N.S_flag = 0;
	for(i = 0; i < BITMAP; i++)
		sb.s_bfree[i] = 0;
	last = sb.s_fsize - sb.s_isize;
	BITALLOC(sb.s_bfree, 0);		/* root */
	for (bno = 1; bno < last; bno++)
		BITFREE(sb.s_bfree, bno);
}

/*
 * make the out-of-super-block bitmap
 */
#define	BITSPERBLK	(BCOUNT*NBBY)

largefree(fd)
int fd;
{
	daddr_t nblks, maxblk, firstblk, freeb;
	register daddr_t bno;
	register int bit;

	sb.U.N.S_flag = 1;
	sb.U.N.S_bsize = BITSPERBLK;
	nblks = (sb.s_fsize + BITSPERBLK-1)/BITSPERBLK;
	if (nblks >= sizeof(sb.U.N.S_blk)/sizeof(sb.U.N.S_blk[0])) {
		fprintf(stderr, "%ld blocks are too many\n", sb.s_fsize);
		exit(1);
	}
	sb.s_tfree -= nblks;
	firstblk = sb.s_isize + 1;	/* root has first block */
	maxblk = sb.s_fsize - nblks;
	freeb = maxblk;			/* first block of bitmap */
	zerobuf(buf.bb);
	for (bno = 0, bit = 0; bno < maxblk; bno++) {
		if (bno >= firstblk)
			BITFREE(buf.fb, bit);
		/* else alloc, but map is already cleared */
		if (++bit == BITSPERBLK) {
			lseek(fd, (off_t)freeb * BCOUNT, 0);
			if (write(fd, (char *)buf.fb, BCOUNT) != BCOUNT) {
				perror("freeblk write error");
				exit(1);
			}
			zerobuf(buf.bb);
			freeb++;
			bit = 0;
		}
	}
	if (bit) {		/* any leftovers? */
		lseek(fd, (off_t)freeb * BCOUNT, 0);
		if (write(fd, (char *)buf.fb, BCOUNT) != BCOUNT) {
			perror("freeblk write error");
			exit(1);
		}
	}
}

zerobuf(b)
char *b;
{
	memset(b, 0, BCOUNT);
}
