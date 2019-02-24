#include <string.h>
#include <stdio.h>
#include <ipc.h>

extern char *getkey();
extern char *pexpw();
extern char *xs();
extern void exit();

char *
getkey(s)
char *s;
{
	s = pexpw(3, s);
	if(s==0) {
		printf("cannot pex for key; quitting\n");
		exit(1);
	}
	return s;
}

static pexipc(name)
char *name;
{
	char *x;
	int fd;
	/* struct pexclude pexbuf; */

	x = ipcpath("astro/giacobini", "dk", name);
	if((fd = ipcopen(x, "light"))==-1){
		return -1;
	}
	/*
	if(pex(fd, 60, &pexbuf)!=0
	    ||pexbuf.farpid<=0
	    ||pexbuf.farcap==0) {
		close(fd);
		return -1;
	}
	*/
	return fd;
}

static yesno(fd)
{
	char buf[4];
	int i;

	strncpy(buf,"",4);
	for(i=0; i<4; i++) {
		if(read(fd,&buf[i],1)!=1)break;
		if(strncmp(buf, "yes\n",4)==0)return 1;
		if(strncmp(buf, "no\n",3)==0)return 0;
	}
	return -1;
}

#ifndef SIGN
keynotary(k1, k2)
char *k1, *k2;
{
	int fd = ipcopen("/cs/mcert!key","light");
	if(fd != -1)
		fprint(fd, "%s\n%s\n", k1, k2);
	return fd;
}

rverify(name, xsum, body, n)
char *name, *xsum, *body;
{
	static int vfd = 0;
	
	if(name==0) {
		if(vfd>0)close(vfd);
		vfd = 0;
		return -1;
	}
	if(vfd == 0)
		vfd = pexipc("mcert");
	if(vfd == -1) return -1;

	fprint(vfd, "%s\n%s\n%d\n", name, xsum, n);
	if(write(vfd, body,n)!=n)
		return -1;
	return yesno(vfd);
}

#endif
verify(name, xsum, body, n)
char *name, *xsum, *body;
{
	char s[100];
	int r, fd = pexipc("mcert");
	
	if(fd==-1)return -1;
	sprintf(s, "%s\n%s\n%d\n", name, xsum, n);
	if(write(fd,s,strlen(s))!=strlen(s))
		return -1;
	if(write(fd, body,n)!=n)
		return -1;
	r = yesno(fd);
	close(fd);
	return r;
}

enroll(name, opw, npw) 
char *name, *opw, *npw;
{
	char buf[9], s[100];
	int r, fd, n = strlen(npw);

	strncpy(buf,opw,9);
	fd = pexipc("mcert!register");
	if(fd==-1)return -1;
	sprintf(s, "%s\n%s\n%d\n", name, xs(buf,npw,n), n);
	if(write(fd,s,strlen(s))!=strlen(s))
		return -1;
	if(write(fd, npw,n)!=n)
		return -1;
	r = yesno(fd);
	close(fd);
	return r;
}
