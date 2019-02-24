#include	<libc.h>
#include	<fio.h>
#include	<cbt.h>
#include	"backup.h"

bfile *db;
char bdir[FNAMELEN];
long nproc, nunl;
int statfd;
char *base;
int nbase;

main(argc, argv)
	char **argv;
{
	extern char *optarg;
	extern int optind;
	extern char *strchr();
	int c;
	char buf[1024], *s;
	long t1, t2;

	while((c = getopt(argc, argv, "")) != -1)
		switch(c)
		{
		case '?':	usage();
		}
	if(optind < argc)
		strcpy(bdir, argv[optind]);
	else
		backupdir(bdir);
	base = malloc(nbase = 10);
	sprint(buf, "%s/%s", bdir, STATLOG);
	if((statfd = open(buf, 1)) < 0){
		perror(buf);
		exit(1);
	}
	sprint(buf, "%s/%s", bdir, BDBM);
	lock(bdir);
	if((db = bopen(buf, 2)) == 0){
		perror(buf);
		unlock();
		exit(1);
	}
	Fseek(statfd, 0L, 2);
	(void)time(&t1);

	while(s = Frdline(0))
		process(s, FIOLINELEN(0));	/* not very clean, process knows about fd 0 */
	(void)time(&t2);
	bclose(db);
	unlock();
	logprint("dbupdate: %ld/%ld (%lds)\n", nunl, nproc, t2-t1);
	exit(0);
}

usage()
{
	Fprint(2, "Usage: dbupdate [dbdirectory]\n");
	exit(1);
}

process(p, len)
	register char *p;
{
	long time();
	char *s, *bname, *uname, *gname;
	char oname[4096];
	unsigned short st_mode;
	time_t st_ctime;
	off_t st_size;
	char *ptrs[9];
	int i;
	long rt;
	static derr = 0;

	for(s = p+len, i = 0; p < s; i++){
		ptrs[i] = p;
		while(*p++)
			;
		if(i >= sizeof ptrs/sizeof ptrs[0])
			break;
	}
	if(i != 8){
		Fprint(2, "data error: got '%s'\n", p);
		if(derr == 0){
			Fprint(2, "want: oname bname time mode size owner group date\n");
			derr = 1;
		}
		return;
	}
	strcpy(oname, ptrs[0]);
	bname = ptrs[1];
	st_ctime = atoi(ptrs[2]);
	st_mode = atoi(ptrs[3]);
	st_size = atoi(ptrs[4]);
	uname = ptrs[5];
	gname = ptrs[6];
	rt = atoi(ptrs[7]);
	nproc++;
	if(set(oname, st_ctime, bname, st_mode, st_size, uname, gname)){
		nunl++;
		Fprint(2, "unlinking %s; already have %s\n", ptrs[1], oname);
	} else {
		struct { long t, l; char u[12]; char s[12];} rec;
		register char *p, *pp;
		extern char *strchr();

		rec.t = rt;
		rec.l = st_size;
		strncpy(rec.u, uname, sizeof rec.u);
		rec.u[sizeof rec.u - 1] = 0;
		p = oname;
		if((p[0] == '/') && (p[1] == 'n') && (p[2] == '/'))
			p += 3;
		if(pp = strchr(p, '/'))
			*pp = 0;
		strncpy(rec.s, p, sizeof rec.s);
		rec.s[sizeof rec.s - 1] = 0;
		Fwrite(statfd, (char *)&rec, (long)sizeof rec);
		*pp = '/';
	}
	if((st_mode&S_IFMT) == S_IFDIR)
		dirsuck(oname, 0, st_ctime);
}

set(oname, t, bname, mode, size, un, gn)
	char *oname, *bname, *un, *gn;
	long t, size;
{
	mbuf d, key;
	long tt;
	char buf[512];
	char goo[4096], goo1[4096];

	d.mdata = goo;
	strcpy(key.mdata = goo1, oname);
	key.mlen = strlen(key.mdata)+1;
	if(bseek(db, key) == 1){
		bread(db, (mbuf *)0, &d);
		d.mdata[d.mlen] = 0;
		tt = atol(d.mdata);
		if(t <= tt)
			return(1);
	}
	sprint(d.mdata, "%ld", t);
	d.mlen = strlen(d.mdata)+1;
	bwrite(db, key, d);
	sprint(d.mdata, "%s %d %ld %s %s", bname, mode, size, un, gn);
	d.mlen = strlen(d.mdata)+1;
	key.mdata = goo1;
	sprint(key.mdata, "%s//%ld", oname, t);
	key.mlen = strlen(key.mdata)+1;
	if(bwrite(db, key, d) < 0)
		Fprint(2, "error %d on storing '%s'\n", errno, oname);
	return(0);
}

dirsuck(oname, fd, t)
	char *oname;
	long t;
{
	mbuf d, key;
	int len;
	char buf[512];
	char goo[4096], goo1[4096];

	key.mdata = goo;
	sprint(key.mdata, "dir%s//%ld", oname+2, t);
	key.mlen = strlen(key.mdata)+1;
	Fread(fd, (char *)&len, 4L);
	if(len <= 0)
		return(0);
	if(nbase < len)
		base = realloc(base, nbase = len);
	Fread(fd, base, (long)len);
	d.mdata = base;
	d.mlen = len;
	if(bwrite(db, key, d) < 0)
		Fprint(2, "error %d on storing '%s'\n", errno, key.mdata);
	return(0);
}
