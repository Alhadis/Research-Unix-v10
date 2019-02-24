#include	<fio.h>
#include	"backup.h"
#include	<signal.h>

#define		DONE		fprint(2, "X")
#define		Exit(n)		(sleep(5), exit(n))

char outfile[256], who[256], bdir[FNAMELEN];

siggy(sn)
{
	char buf[256];

	signal(sn, SIG_IGN);
	strcpy(buf, bdir);
	logprint("rcv.%s: signal %d!\n", who, sn);
	exit(0);
}

main(argc, argv)
	char *argv[];
{
	int n, nn;
	int ofd;
	off_t size;
	struct backfile h;
	char buf[512];
	long nf, nb, t1, t2;
	int cfd = 0;		/* because we are a server */
	int cnt;
	unsigned short csum, ocsum;
	int erred;
	int i;

	chdir("/tmp");
	backupdir(bdir);
	for(n = 1; n < NSIG; n++)
		signal(n, siggy);
	signal(SIGHUP, SIG_IGN);
	dup2(cfd, 2);		/* for perror */
	ainit();
	nb = nf = 0;
	(void)time(&t1);
	for(i = 0; i < sizeof who; i++){
		if(Fread(cfd, &who[i], 1L) != 1){
			who[i] = 0;
			break;
		}
		if(who[i] == 0) break;
	}
	while((i = Fread(cfd, (char *)&h, (long)sizeof h)) == sizeof h){
		if(h.version < 0){
			i = 0;
			break;
		}
/*
logprint("rcv.%s: file %d; %d bytes\n", who, nf, h.sbuf.st_size);
write(creat("/tmp/agh", 0666), &h, sizeof h);
if(h.sbuf.st_size > 1000000)h.sbuf.st_size = 2184;
*/
		erred = 0;
		if((ofd = filefor(&h)) < 0){
			strcpy(buf, bdir);
			(void)time(&t2);
			logprint("rcv.%s: out of space during %ldth file, %ld bytes (%lds)\n", who, nf, nb, t2-t1);
			fprint(cfd, "*out of space after %d files\n", nf-1);
			erred = 1;
			ofd = open("/dev/null", 1);
		}
		csum = sum(((unsigned char *)&h)+2, sizeof h - 2, 0);
		if(strncmp(h.oname, "/n/", 3) == 0){
			nf++;
			nb += h.sbuf.st_size+sizeof h;
		}
		sprint(argv[0], "%ld", nf);
		for(size = h.sbuf.st_size, n = sizeof buf; size > 0; size -= n){
			if(n > size)
				n = size;
			if((nn = Fread(cfd, buf, (long)n)) != n){
				fprint(cfd, "read(%d)=%d", n, nn);
				perror("");
				DONE;
				Exit(1);
			}
			csum = sum((unsigned char *)buf, n, csum);
			if(write(ofd, buf, n) != n){
				if(erred == 0){
					erred = 1;
					perror(outfile);
				}
			}
		}
		if((cnt = Fread(cfd, (char *)&ocsum, 2L)) != 2){
			fprint(2, "wanted 2, got %d; ", cnt);
			perror("read 2");
			DONE;
			Exit(2);
		}
		if(csum != ocsum){
			fprint(cfd, "%s: csum=%d ocsum=%d (file=%s)\n", h.oname, csum, ocsum, outfile);
			DONE;
			Exit(3);
		}
		lseek(ofd, 0L, 0);
		if(write(ofd, (char *)&csum, 2) != 2){
			perror(outfile);
			DONE;
			Exit(3);
		}
		if(chmod(outfile, 0600) < 0){
			if(!erred)
				perror(outfile);
			DONE;
			Exit(4);
		}
		close(ofd);
		DONE;
	}
	(void)time(&t2);
	strcpy(buf, bdir);
	if(i > 0)
		logprint("rcv.%s: bad loop read (%d)\n", who, i);
	logprint("rcv.%s: %ld files, %ld bytes (%lds)\n", who, nf, nb, t2-t1);
	fprint(cfd, "%ld files, %ld bytes (%lds)\n", nf, nb, t2-t1);
	sleep(5);	/* give time for things to settle */
	exit(0);
}

char *dirs[100];
int ndir;
int nfile;

ainit()
{
	char buf[512];
	int fd, i;
	char *s, *strdup();

	sprint(buf, "%s/%s", bdir, RCVDIRS);
	if((fd = open(buf, 0)) < 0){
		perror(buf);
		dirs[0] = "/unix";
		dirs[1] = 0;
	}
	i = 0;
	while(s = Frdline(fd))
		dirs[i++] = strdup(s);
	dirs[i] = 0;
	close(fd);
	ndir = 0;
	nfile = 0;
}

filefor(h)
	struct backfile *h;
{
	int fd, i;
	char buf[4096];

	i = ndir;
	do {
		if((fd = probe(dirs[i], h->sbuf.st_size+sizeof h)) >= 0)
			break;
		if(dirs[++i] == 0)
			i = 0;
		nfile = 0;
	} while(i != ndir);
	ndir = i;
	if(fd < 0){
		/*fprint(2, "No space left to rcv\n");*/
		return(-1);
	}
	lseek(fd, 0L, 0);
	if(write(fd, (char *)h, sizeof *h) != sizeof *h){
		perror(outfile);
		close(fd);
		return(-1);
	}
	return(fd);
}

probe(dir, size)
	char *dir;
	off_t size;
{
	static i = 0;
	char buf[4096];
	int n, nn, fd = -1, pid = getpid();
	off_t s;

	for(n = 0; n < 20; n++){
		sprint(outfile, "%s/rcv%d/%d-%d", dir, (pid+i)%32, pid, i);
		i++;
		if((fd = creat(outfile, 0)) >= 0){
			for(s = 0, nn = sizeof buf; s < size; s += nn){
				if(size-s < nn)
					nn = size-s;
				if(write(fd, buf, nn) != nn){
					unlink(outfile);
					close(fd);
					return(-1);
				}
			}
			break;
		}
	}
	return(fd);
}
