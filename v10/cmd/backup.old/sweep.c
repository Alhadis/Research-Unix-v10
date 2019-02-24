#include	<fio.h>
#include	<string.h>
#include	<cbt.h>
#include	"backup.h"
#include	<utsname.h>

int nflag = 0;
int sflag = 0;
long sv, sf, sd;
extern char *strcpy();
extern char *ctime(), *date();
extern long atol();
extern int errno;
bfile *db;
char bdir[FNAMELEN];
long nno, nyes;

#define		NEXT		"@NEXT@"
#define		BLOCK		4096
#define		DEVSIZE		(10000L*1024L/BLOCK)

main(argc, argv)
	char **argv;
{
	char buf[4096];
	long t1, t2;
	char *s;
	extern char *malloc();

	for(argv++; *argv && (**argv == '-'); argv++)
		switch(argv[0][1])
		{
		case 'n':	nflag++; break;
		case 's':
			sflag = 1;
			argv++;
			if(!argv[0] || !argv[1] || !argv[2])
				usage();
			sv = atol(argv[0]);
			sf = atol(argv[1]);
			sd = atol(argv[2]);
			break;
		default:	usage();
		}
	nno = nyes = 0;
	backupdir(bdir);
	sprint(buf, "%s/%s", bdir, BDBM);
	lock(bdir);
	(void)time(&t1);
	if((db = bopen(buf, 2)) == 0){
		perror(buf);
		unlock();
		exit(1);
	}
	if(sflag)
		next(1L, buf, 0);
	if(sflag|nflag){
		next(0L, buf, 1);
		print("next file = '%s'\n", buf);
		bclose(db);
		unlock();
		exit(0);
	}
	while(s = Frdline(0))
		process(s);
	(void)time(&t2);
	bclose(db);
	unlock();
	logprint("sweep: %ld/%ld (%lds)\n", nno, nno+nyes, t2-t1);
	exit(0);
}

usage()
{
	fprint(2, "usage: sweep [-n] [files]\n");
	fprint(2, "\t-n\tnext file to be allocated\n");
	fprint(2, "\t-s v f d\tnext file to be allocated is vol v, file f and d blocks used\n");
	exit(1);
}

process(s)
	char *s;
{
	char buf[4096];
	int fd;
	struct backfile h;
	struct stat sb;
	long t, time(), nn, n, size;
	unsigned short csum;

	if(stat(s, &sb) < 0){
		perror(s);
		return;
	}
	t = time((long *)0);
	switch(sb.st_mode&0777)
	{
	case 0:
		if((sb.st_ctime+6*60*60L) < t){	/* six hours */
			Fprint(2, "unlink %s(mode=0); stale %s", s, ctime(&sb.st_ctime));
			unlink(s);
		}
		break;
	case 0400:
		if((sb.st_ctime+8*24*60*60L) < t){	/* eight days */
			Fprint(2, "%s(mode=400); stale %s", s, ctime(&sb.st_ctime));
			/*unlink(s);/**/
		}
		break;
	case 0600:
		if((fd = open(s, 2)) < 0){
			perror(s);
			return;
		}
		if(read(fd, (char *)&h, sizeof h) != sizeof h){
			perror(s);
			close(fd);
			return;
		}
		sprint(buf, "%s//%ld", h.oname, h.sbuf.st_ctime);
		if(valloc(buf, h.bname, h.sbuf.st_size+sizeof h)){
			/*Fprint(2, "unlinking %s[%s]\n", h.oname, s);/**/
			nno++;
			if(unlink(s) < 0)
				perror(s);
		} else if(h.bname[0]){
			csum = sum(((unsigned char *)&h)+2, sizeof h - 2, 0);
			for(size = h.sbuf.st_size, n = sizeof buf; size > 0; size -= n){
				if(n > size)
					n = size;
				if((nn = read(fd, buf, (int)n)) != n){
					fprint(2, "%s: ", s); perror("read");
					if(nn < 0)
						break;
				}
				csum = sum((unsigned char *)buf, (int)n, csum);
			}
			h.checksum = csum;
			lseek(fd, 0L, 0);
			write(fd, (char *)&h, sizeof h);
			/*Fprint(2, "%s: %s -> %s\n", s, h.oname, h.bname);/**/
			if(chmod(s, 0400) < 0)
				perror(s);
			nyes++;
		}
		close(fd);
		break;
	default:
		/* ignore */
		break;
	}
}

valloc(s, res, size)
	char *s, *res;
	long size;
{
	mbuf d, key, gup;
	register char *unique;
	long t1, t2;
	char buf[512];
	char goo[4096], goo1[4096];

	d.mdata = goo;
	gup.mdata = goo1;
	for(unique = s; *unique; unique++)
		if((unique[0] == '/') && (unique[1] == '/'))
			break;
	*unique++ = 0;
	*unique++ = 0;
	key.mdata = s;
	key.mlen = strlen(s)+1;
	if(bseek(db, key) == 1){
		bread(db, (mbuf *)0, &d);
		d.mdata[d.mlen] = 0;
		t1 = atol(unique);
		t2 = atol(d.mdata);
		if(t1 <= t2){
			/* old */
			return(1);
		}
	}
	next(size, res, 0);
	if(s = strchr(res, ' '))
		*s = 0;
	return(0);
}

next(size, res, old)
	long size;
	char *res;
{
	long osize = size;
	static char buf1[64], buf2[64];
	int vol, file;
	long devsize, ndevsize;
	mbuf n, d;

	n.mdata = NEXT;
	n.mlen = strlen(n.mdata)+1;
	if(sflag){
		vol = sv, file = sf, devsize = sd;
	} else if(bseek(db, n) != 1){
		Fprint(2, "no %s marker!\n", NEXT);
		unlock();
		exit(1);
	} else {
		d.mdata = buf1;
		if(bread(db, (mbuf *)0, &d))
			Fprint(2, "bread fail???\n");
		sscanf(d.mdata, "v/v%d/%d %d", &vol, &file, &devsize);
	}
	size = (size+BLOCK-1)/BLOCK;
	ndevsize = devsize+size;
	if(ndevsize > DEVSIZE){
		vol++, file = 0, devsize = size;
	}
	else
		devsize = ndevsize;
	if(size){
		d.mdata = buf1;
		sprint(d.mdata, "v/v%d/%d %d", vol, file+1, devsize);
		d.mlen = strlen(d.mdata)+1;
		if(bwrite(db, n, d) < 0){
			Fprint(2, "error %d on incrementing next!!\n", errno);
			bclose(db);
			unlock();
			exit(1);
		}
	}
	sprint(res, "v/v%d/%d %d", vol, file, old? devsize : osize);
}
