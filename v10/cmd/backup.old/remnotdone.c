#include	<fio.h>
#include	<cbt.h>
#include	<regexp.h>
#include	"backup.h"
#include	<string.h>

extern long atol();
char bdir[FNAMELEN];
char tmpfile[] = "/tmp/remnotdXXXXXX";

main(argc, argv)
	char **argv;
{
	int ifd, ofd, tfd;
	register n;
	register char *s;
	unsigned short csum, tsum;
	long nchars, tnchars;
	char *sys, *service;

	backupdir(bdir);
	mktemp(tmpfile);
	for(s = bdir; *s++; )
		;
	if(rx(sys = s, service = "bkpnotdone", (char *)0, "heavy", &ifd, &ofd))
		exit(1);
	if((tfd = creat(tmpfile, 0666)) < 0)
		perror(tmpfile);
	Finit(tfd, (char *)0);
	Finit(ofd, (char *)0);
	Fprint(ofd, "%s\n", argv[1]? argv[1]:"??");
	/* write chars, csum, nchars */
	csum = 0;
	nchars = 0;
	while(s = Frdline(0)){
		n = FIOLINELEN(0);	
		s[n] = '\n';
		nchars += ++n;
		csum = sum((unsigned char *)s, n, csum);
		Fwrite(ofd, s, (long)n);
		Fwrite(tfd, s, (long)n);
	}
	Fprint(ofd, "%c%-6d%ld\n", 0, csum, nchars);
	Fprint(tfd, "%c%-6ud%ld\n", 0, csum, nchars);
	Fflush(ofd);
	Fflush(tfd);
	/* read back results */
	sleep(1);
	csum = 0;
	nchars = 0;
	Finit(ifd, (char *)0);
	lseek(tfd, 0L, 0);
	Finit(tfd, (char *)0);
	while(s = Frdline(ifd)){
		if(*s == 0)
			break;
		n = FIOLINELEN(ifd);	
		s[n] = '\n';
		nchars += ++n;
		csum = sum((unsigned char *)s, n, csum);
		Fwrite(1, s, (long)n);
		Fwrite(tfd, s, (long)n);
	}
	if(!s){
		fprint(2, "unexpected eof from %s!%s; see /n/%s/usr/backup/log\n", sys, service, sys);
		exit(1);
	}
	tsum = atoi(s+1);
	tnchars = atol(s+7);
	if((tsum != csum) || (tnchars != nchars)){
		fprint(2, "remnotdone: rcverr: rcv=(#%ux,%ld) sent=(#%ux,%ld)\n",
			csum&0xFFFF, nchars, tsum&0xFFFF, tnchars);
		logprint("remnotdone: rcverr: rcv=(#%ux,%ld) sent=(#%ux,%ld)\n",
			csum&0xFFFF, nchars, tsum&0xFFFF, tnchars);
		exit(1);
	}
	unlink(tmpfile);
	if(argc > 2){
		for(s += 7; *s;)
			if(*s++ == ' ') break;
		tsum = atoi(s);
		while(*s && (*s != ' '))
			s++;
		tnchars = atoi(s);
		fprint(2, "considering %d files, %d already backed up\n",
			tsum, tsum-tnchars);
	}
	exit(0);
}
