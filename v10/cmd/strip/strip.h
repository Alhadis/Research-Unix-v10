#include <stdio.h>
#include <a.out.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "malloc.h"

#define Signal(s, f)	(signal(s, SIG_IGN) == SIG_IGN ? SIG_IGN : signal(s, f))

#define Read(fd,addr,nchars)	(read(fd, addr, nchars) != nchars)
#define Write(fd,addr,nchars)	(write(fd, addr, nchars) != nchars)

typedef struct Adotout {
	char *name;
	int fd;
	struct stat filstat;
	struct exec hd;
	int nsymbols;
	struct nlist *symtab, *symend;
	int strsize;
	char *strtab;
} Adotout;

Adotout *rdout();

char tmpnam[32], *prognam;

int shrinkflag, gflag, stripflag, strdebug, prtflag, copyrace;
