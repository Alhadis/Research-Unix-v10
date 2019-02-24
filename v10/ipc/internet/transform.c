#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inet/in.h>
#include <stdio.h>
#include <ctype.h>


struct namefile {
	char *name;
	char *data;
	long time;
};

static struct namefile files[] = {
	{ "/tmp/networks", 0, 0 },
	{ 0, 0, 0},
};

struct hash {
	struct hash *next;
	in_addr addr;
	char *name;
};

#define HSIZE 1023
static struct hash ht[HSIZE];

/* imported */
extern char *in_getw();
extern char *malloc();

/* exported */
int in_host_dostat=1;

static int
getfile(i)
	int i;
{
	int fd;
	struct stat sbuf;
	unsigned int x;

	if (in_host_dostat)
		stat(files[i].name, &sbuf);
	else
		sbuf.st_mtime = files[i].time;
	if (files[i].data == 0 || sbuf.st_mtime>files[i].time) {
		if (!in_host_dostat)
			stat(files[i].name, &sbuf);
		fd = open(files[i].name, 0);
		if (fd<0)
			return -1;
		if (files[i].data!=0)
			free(files[i].data);
		x = sbuf.st_size;
		files[i].data = malloc(x+2);
		if (files[i].data==0) {
			close(fd);
			return -1;
		}
		if (read(fd, files[i].data, x)!=x){
			close(fd);
			return -1;
		}
		close(fd);
		files[i].data[sbuf.st_size] = '\n';
		files[i].data[sbuf.st_size+1] = 0;
		files[i].time = sbuf.st_mtime;
		for (i=0; i<HSIZE; i++)
			ht[i].addr = 0;
	}
	return 0;
}

#define SKIPWHITE while(*p==' ' || *p=='\t') p++
#define GETBLACK	xp = b;\
			while(*p!=' ' && *p!='\t' && *p!='\n' && *p!='\0')\
				*xp++ = *p++;\
			*xp = '\0'

char *
in_host(addr)
in_addr addr;
{
	register char *p, *xp;
	unsigned char *yp;
	static char b[32];
	int i;
	in_addr x, in_netof(), in_aton();

	if (addr==0)
		return "*";
	i = addr==in_netof(addr) ? 0 : 1;
	if (getfile(i) < 0) {
		perror(files[i].name);
	} else if (addr == ht[addr%HSIZE].addr) {
		p = ht[addr%HSIZE].name;
		GETBLACK;
		return(b);
	} else for (p = files[i].data; *p; p++){
		/* get number */
		SKIPWHITE;
		GETBLACK;
		x = in_aton(b);
		SKIPWHITE;
		if (x != 0 && *p != '\n') {
			/* get name & insert into hash table */
			i = x%HSIZE;
			ht[i].addr = x;
			ht[i].name = p;
			GETBLACK;

			/* see if this is the one */
			if (x==addr)
				return b;
		}
		/* drop rest of line */
		while(*p!='\n'&&*p!='\0') p++;
	}
	yp = (unsigned char *) &addr;
	sprintf(b, "%d.%d.%d.%d", yp[3], yp[2], yp[1], yp[0]);
	return(b);
}

main()
{
	in_addr	net, addr, onet;
	char line[128];
	char *name, *cp;

	onet = -1;
	while(gets(line)!=NULL) {
		/* comments */
		if (line[0]=='#'||line[0]=='\n' || line[0]==' ' || line[0]=='\t'){
			puts(line);
			continue;
		}

		/* get addr and net */
		for (name=line; *name&&*name!=' '&&*name!='\t'; name++)
			;
		if (*name=='\0')
			continue;
		*name++ = '\0';
		if ((addr=in_aton(line))==0 || (net=in_netof(addr))==0) {
			printf("%s	%s\n", line, name);
			continue;
		}
		if (net!=onet) {
			onet = net;
			printf("%s	%s\n", in_ntoa(net), in_host(net));
		}

		/* get name */
		for (; *name==' '||*name=='\t'; name++)
			;
		if (*name=='\0')
			continue;
		for (cp=name; *cp&&*cp!=' '&&*cp!='\t'; cp++)
			;
		if (*cp!='\0')
			*cp++ = '\0';

		/* print out both addresses */
		printf("%s	%s/%s	%s %s\n", line, in_host(net),name,name,cp);
	}
}
