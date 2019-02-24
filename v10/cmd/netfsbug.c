/* if /n/... seems to screw up, type netfsbug >xxx; mail pjw < xxx */
#include "stdio.h"
#include "nlist.h"
#include "sys/types.h"
#include "sys/neta.h"
/*
struct	nlist {
	char	*n_name;
	unsigned char n_type;
	char	n_other;
	short	n_desc;
	unsigned long n_value;
};
*/
extern int errno;
char *cmd[] = { 0, "stat", "wrt", "read", "free", "trunc", "updat", "get",
	"nami", "put", "root", "ndel", "nlink", "creat", "nomatch", "nstart",
	"nioctl"};
struct nlist nl[] = {
	{"_netabuf"}, {0}
};
struct {
	char len, s, n, dummie;
	int b[4000];
} x;
struct rcva *rp;
struct senda *sp;
main()
{	int fd, n, i;
	nlist("/unix", nl);
	if(nl[0].n_value == 0) {
		fprintf(stderr, "nlist failed\n");
		exit(1);
	}
	fd = open("/dev/kmem", 0);
	if(fd < 0) {
		perror("/dev/kmem");
		exit(1);
	}
	lseek(fd, nl[0].n_value, 0);
	if(errno)
		perror("lseek");
	n = read(fd, (char *) &x, sizeof(x));
	sp = (struct senda *) x.b;
	rp = (struct rcva *) (x.b + x.len * sizeof(struct senda)/sizeof(int));
	for(i = x.n - 1; i >= 0; i--)
		out(i);
	for(i = x.len - 1; i >= x.n; i--)
		out(i);
}

out(i)
{
	printf("%d %s tran=%d uid=%d gid=%d maj=%d min=%d tag=0x%x mode=0%o\n",
		sp[i].version, cmd[sp[i].cmd], sp[i].trannum, sp[i].uid,
		sp[i].gid, (sp[i].dev & 0xff00) >> 8, sp[i].dev & 0xff,
		sp[i].tag, sp[i].mode & 0xffff);
	printf("\tnuid=%d ngid=%d flag=%d ino=%d cnt=%d offset=%d\n",
		sp[i].newuid, sp[i].newgid, sp[i].flags, sp[i].ino, sp[i].count,
		sp[i].offset);
	printf("tran=%d err=%d flag=%d maj=%d min=%d size=%d mode=0%o\n",
		rp[i].trannum, rp[i].errno, rp[i].flags, (rp[i].dev & 0xff00) >> 16,
		rp[i].dev & 0xff, rp[i].size, rp[i].mode & 0xffff);
	printf("\tuid=%d gid=%d tag=0x%x nlnk=%d ino=%d count=%d\n",
		rp[i].uid, rp[i].gid, rp[i].tag, rp[i].nlink, rp[i].ino,
		rp[i].count);
}
