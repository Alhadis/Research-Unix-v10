#include	<libc.h>
#include	"worm.h"
#include	"sym.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fio.h>

Inode **inos;
long nino;
int fdT, fdF;
char *inonames;
char *timenow();
char *tmp = "/tmp";

cmp(a, b)
	Inode **a, **b;
{
	return(strcmp((*a)->name.n, (*b)->name.n));
}

main(argc, argv)
	char **argv;
{
	Superblock s, news;
	char *e;
	char *dev = "/dev/worm0";
	int c, fd;
	char dbname[256];
	extern char *optarg;
	extern int optind;
	void blkfn();

	while((c = getopt(argc, argv, "t:f:")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case 't':	tmp = optarg; break;
		case '?':	usage();
		}
	dev = mapdev(dev);
	if(optind != argc-1)
		usage();
	sprint(dbname, "%s/cbt%d", tmp, getpid());
	fd = dbinit(dbname);
	if((s.fd = open(dev, 2)) < 0){
		perror(dev);
		exit(1);
	}
	if(e = openinode(&s, DO_INODE|SPIN_DOWN)){
		fprint(2, "%s: %s\n", dev, e);
		exit(1);
	}
	if(strcmp(argv[optind], s.vol_id)){
		fprint(2, "wanted volid '%s'; got '%s'\n", argv[optind], s.vol_id);
		exit(1);
	}
	if((inos = (Inode **)malloc(sizeof(inos[0])*(int)numinodes)) == 0){
		fprint(2, "out of memory (%d inodes, %d bytes)\n", numinodes, sizeof(inos[0])*(int)numinodes);
		exit(1);
	}
	nino = 0;
	inodetraverse(blkfn);
	fprint(2, "%s: sorting inodes\n", timenow());
	qsort((char *)inos, (int)nino, sizeof(inos[0]), cmp);
	news = s;
	news.ninodes = nino;
	creatdb(fd, dbname, &news);	/* fills in nF, nT, inochars */
	c = NBLKS(&news, news.nF)+NBLKS(&news, news.nT)+NBLKS(&news, news.ninochars);
	if(c > news.nfree){
		fprint(2, "%s: sorry, not enough blocks; %d+%d+%d>%d\n", news.vol_id,
			NBLKS(&news, news.nF), NBLKS(&news, news.nT),
			NBLKS(&news, news.ninochars), news.nfree);
		exit(1);
	}
	Seek(&s, news.binodes = s.nextffree);
	fdwrite(&s, fdF, (int)news.nF);
	fdwrite(&s, fdT, (int)news.nT);
	memwrite(&s, inonames, (int)news.ninochars);
	/* set up link ptrs so that zeroing block zero undoes btreeing */
	s.nfree -= c;
	s.nextffree += c;
	s.ninodes = 0;
	if(e = lkwsb(&s))
		fprint(2, "%s\n", e);
	news.nextffree = s.nextffree;
	news.nfree = s.nfree;
	news.myblock = 0;
	news.version = VBTREE;
	news.nextsb = s.myblock;
	time(&news.ctime);
	Seek(&s, news.myblock);
	free((char *)inos);
	e = malloc(news.blocksize);
	if(e == 0){
		fprint(2, "wbtree: unbelievable malloc fail of %d\n", news.blocksize);
		exit(1);
	}
	memset(e, 0, news.blocksize);
	*((Superblock *)e) = news;
	Write(&s, e, 1L);
	exit(0);
}

usage()
{
	fprint(2, "Usage: worm btree [-fdevice] [-ttmpdir] vol_id\n");
	exit(2);
}

dbinit(name)
	char *name;
{
	char buf[256];
	struct stat sbuf;
	int pid, pip[2];

	fprint(2, "%s: init db '%s'\n", timenow(), name);
	sprint(buf, "cbt creat %s", name);
	if(system(buf))
		exit(1);
	pipe(pip);
	pid = fork();
	if(pid < 0){
		perror("fork");
		exit(1);
	}
	if(pid){
		close(pip[0]);
		return(pip[1]);
	} else {
		close(pip[1]);
		dup2(pip[0], 0);
		close(pip[0]);
		execl("/usr/lib/btree/btbuild", "btbuild", name, 0);
		perror("execl");
		exit(1);
		return(0);
	}
}

creatdb(fd, name, s)
	char *name;
	Superblock *s;	/* fills in nF, nT, inochars */
{
	char buf[256];
	struct stat sbuf;
	int status, i;
	short n;
	char *np;

	fprint(2, "%s: creating db '%s'\n", timenow(), name);
	inonames = malloc((int)numnamechars+1024);
	if(inonames == 0){
		sprint(buf, "malloc(%d) namechars failed", numnamechars+1024);
		perror(buf);
		exit(1);
	}
	Finit(fd, (char *)0);
	np = inonames;
	for(i = 0; i < nino; i++){
		n = strlen(inos[i]->name.n);
		Fwrite(fd, (char *)&n, 2L);
		Fwrite(fd, inos[i]->name.n, (long)n);
		memcpy(np, inos[i]->name.n, n+1);
		inos[i]->name.o = np - inonames;
		np += n+1;
		n = sizeof(Inode);
		Fwrite(fd, (char *)&n, 2L);
		Fwrite(fd, (char *)inos[i], (long)n);
	}
	s->ninochars = np-inonames;
	Fflush(fd);
	close(fd);
	if(wait(&status) < 0){
		perror("wbtree: wait");
		exit(1);
	}
	if(status){
		fprint(2, "wbtree: bad status %d from btbuild\n", status);
		exit(1);
	}
	sprint(buf, "%s.F", name);
	if(((fdF = open(buf, 0)) < 0) || (fstat(fdF, &sbuf) < 0)){
		perror(buf);
		exit(1);
	}
	unlink(buf);
	s->nF = sbuf.st_size;
	sprint(buf, "%s.T", name);
	if(((fdT = open(buf, 0)) < 0) || (fstat(fdT, &sbuf) < 0)){
		perror(buf);
		exit(1);
	}
	unlink(buf);
	s->nT = sbuf.st_size;
	fprint(2, "%s: db done\n", timenow());
}

void
blkfn(i)
	Inode *i;
{
	inos[nino++] = i;
}

fdwrite(s, fd, cnt)
	Superblock *s;
{
	char b[BIGBLOCK];
	int n;

	lseek(fd, 0L, 0);
	while(cnt >= sizeof b){
		n = read(fd, b, sizeof b);
		if(n != sizeof b){
			perror("short read");
			exit(3);
		}
		Write(s, b, NBLKS(s, sizeof b));
		cnt -= sizeof b;
	}
	if(cnt){
		if(read(fd, b, cnt) != cnt){
			perror("short read");
			exit(4);
		}
		memset(b+cnt, 0, sizeof b - cnt);
		Write(s, b, NBLKS(s, cnt));
	}
}

memwrite(s, base, cnt)
	Superblock *s;
	char *base;
{
	int chunk = (BIGBLOCK/1024)*s->blocksize;

	while(cnt >= chunk){
		Write(s, base, NBLKS(s, chunk));
		cnt -= chunk;
		base += chunk;
	}
	if(cnt)
		Write(s, base, NBLKS(s, cnt));
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
