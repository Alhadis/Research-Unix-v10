#include	<libc.h>
#include	"worm.h"
#include	"sym.h"
#include	<fio.h>
#include	"backup.h"

int nb = 0;
typedef struct
{
	long b, t;
} tb;
tb *allblks;
long nallblk;
extern long numinodes;
char *timenow();

#define		N		63
tb blks[N];

cmp(a, b)
	tb *a, *b;
{
	return(a->b - b->b);
}

main(argc, argv)
	char **argv;
{
	char *dev = "/dev/worm0", *e, *vol_id;
	int c, i, j;
	Superblock sb;
	void blkfn();
	extern char *optarg;
	extern int optind;
	extern long atol();

	while((c = getopt(argc, argv, "f:")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case '?':	usage();
		}
	if(optind != argc-1)
		usage();
	vol_id = argv[optind];
	dev = mapdev(dev);
	if((sb.fd = open(dev, 0)) < 0){
		perror(dev);
		exit(1);
	}
	fprint(2, "%s: reading inodes\n", timenow());
	if(e = openinode(&sb, DO_INODE|SPIN_DOWN)){
		fprint(2, "%s: %s\n", dev, e);
		exit(1);
	}
	if(strcmp(vol_id, sb.vol_id)){
		fprint(2, "wanted volid '%s'; got '%s'\n", vol_id, sb.vol_id);
		exit(1);
	}
	if((allblks = (tb *)malloc(sizeof(tb)*(int)numinodes)) == 0){
		fprint(2, "out of memory (%d)\n", numinodes);
		exit(1);
	}
	nallblk = 0;
	inodetraverse(blkfn);
	fprint(2, "%s: sorting inodes\n", timenow());
	qsort((char *)allblks, (int)nallblk, sizeof allblks[0], cmp);
	fprint(2, "%s: reading blocks\n", timenow());
	for(j = 0; j < nallblk;){
		for(i = 0; (i < N) && (j < nallblk); i++){
			blks[i] = allblks[j++];
			if(blks[i].b >= blks[0].b+N){
				block(sb.fd, i);
				blks[0] = blks[i];
				i = 0;
			}
		}
		block(sb.fd, i);
	}
	fprint(2, "%ld blocks\n", nb);
	fprint(2, "%s: done reading blocks\n", timenow());
	exit(0);
}

usage()
{
	fprint(2, "Usage: wormupd [-fdevice] vol_id\n");
	exit(2);
}

block(fd, n)
{
	register i;
	register struct backfile *h;
	long t;
	int nr;
	int zero = 0;
	struct backfile buf[N];

	nb++;
	lseek(fd, blks[0].b*1024, 0);
	i = 1024*(int)(blks[n-1].b-blks[0].b+1);
	nr = read(fd, (char *)buf, i);
	if(nr < 0){
		perror("wormupd read");
		fprint(2, "trying for %d blocks:", n);
		for(i = 0; i < n; i++) fprint(2, " %ld", blks[i].b);
		fprint(2, "\n");
		return;
	}
	nr /= 1024;
	for(i = 0; i < n; i++){
		t = blks[i].b-blks[0].b;
		if(t >= nr){
			fprint(2, "read short(%ld) for block %ld\n", nr, blks[i].b);
			continue;
		}
		h = &buf[t];
		t = blks[i].t;
#ifdef	USR1_USR2
	{
	register char *p;

	p = strchr(h->oname, '/');		/* / */
	if (p) p = strchr(p+1, '/');		/* /n/ */
	if (p) p = strchr(p+1, '/');		/* /n/system/ */
	if (p && (strncmp(p+1, "usr1", 4) == 0 || strncmp(p+1, "usr2", 4) == 0))
		strcpy(p+4, p+5);
	}
#endif	/* USR1_USR2 */
		Fprint(1, "%s%c%s%c%ld%c%d%c%ld%c%s%c%s%c%ld\n", h->oname, 0, h->bname,
			0, h->sbuf.st_ctime, 0, h->sbuf.st_mode, 0, h->sbuf.st_size,
			0, h->uname, 0, h->gname, 0, t);
		if((h->sbuf.st_mode&S_IFMT) == S_IFDIR)
			Fwrite(1, (char *)&zero, 4L);	/* cop out for now */
	}
}

void
blkfn(i)
	Inode *i;
{
	if(strchr(i->name.n, '/')){
		allblks[nallblk].t = i->ctime;
		allblks[nallblk].b = i->block;
		nallblk++;
	}
}

char *
timenow()
{
	long tim;
	char *tims;

	time(&tim);
	tims = ctime(&tim);
	tims[19] = 0;
	return(tims);
}
