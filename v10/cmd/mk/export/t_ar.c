#include	"mk.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<ar.h>

static atimes();

long
atimeof(force, name, ar, mem)
	char *name, *ar, *mem;
{
	struct stat sbuf;
	Symtab *sym;

	if(stat(ar, &sbuf) < 0)
		sbuf.st_mtime = 0;
	if(sym = symlook(ar, S_AGG, (char *)0)){
		if(force || (sbuf.st_mtime > (long)sym->value)){
			atimes(ar);
			symlook(ar, S_AGG, ar)->value = (char *)sbuf.st_mtime;
		}
	} else {
		atimes(ar);
		symlook(strdup(ar), S_AGG, ar)->value = (char *)sbuf.st_mtime;
	}
	if(sym = symlook(name, S_TIME, (char *)0))
		sbuf.st_mtime = (long)sym->value;
	else
		sbuf.st_mtime = 0;
	return((long)sbuf.st_mtime);
}

void
atouch(name, ar, mem)
	char *name, *ar, *mem;
{
	int fd;
	struct ar_hdr hdr;
	char *s;
	long t;
	char buf[NAMEBLOCK];

	strcpy(buf, mem);
#ifdef	SYSV
	strcat(buf, "/");
#endif
	if((fd = open(ar, 2)) < 0){
		if((fd = creat(ar, 0666)) < 0){
			perror(ar);
			Exit();
		}
		write(fd, ARMAG, SARMAG);
	}
	if(symlook(name, S_TIME, (char *)0)){
		/* hoon off and change it in situ */
		lseek(fd, (long)SARMAG, 0);
		while(read(fd, (char *)&hdr, sizeof(hdr)) == sizeof(hdr)){
			for(s = &hdr.ar_name[sizeof(hdr.ar_name)]; *--s == ' ';);
			s[1] = 0;
			if(strcmp(buf, hdr.ar_name) == 0){
				t = sizeof(hdr.ar_name)-sizeof(hdr);
				lseek(fd, t, 1);
				fprint(fd, "%-12ld", time((long *)0));
				break;
			}
			t = atol(hdr.ar_size);
#ifdef	CRAY
			t = (t+7)&~7;
			t -= sizeof(hdr)-60;
#else
			if(t&01) t++;
#endif
			lseek(fd, t, 1);
		}
	} else {
		lseek(fd, 0L, 2);
		fprint(fd, "%-16s%-12ld%-6d%-6d%-8lo%-10ld%2s", buf, time((long *)0),
			getuid(), getgid(), 0100666L, 0L, ARFMAG);
	}
	close(fd);
}


void
adelete(name, ar, mem)
	char *name, *ar, *mem;
{
	Fprint(2, "sorry; mk doesn't know how to delete archive members yet\n");
}

static
atimes(ar)
	char *ar;
{
	struct ar_hdr hdr;
	long t;
	int fd;
	char buf[BIGBLOCK];
	register char *s;

	if((fd = open(ar, 0)) < 0)
		return;
	if(read(fd, buf, SARMAG) != SARMAG){
		close(fd);
		return;
	}
	while(read(fd, (char *)&hdr, sizeof(hdr)) == sizeof(hdr)){
		for(s = &hdr.ar_name[sizeof(hdr.ar_name)]; *--s == ' ';);
#ifdef	SYSV
		if(*s == '/')	/* Damn you Sytem V */
			s--;
#endif
		s[1] = 0;
		t = atol(hdr.ar_date);
		sprint(buf, "%s(%s)", ar, hdr.ar_name);
		symlook(strdup(buf), S_TIME, (char *)t)->value = (char *)t;
		t = atol(hdr.ar_size);
		if(t&01) t++;
		lseek(fd, t, 1);
	}
	close(fd);
}
