#include	<fio.h>
#include	<libc.h>

relay(bdir, argv, service, traffic)
	char *bdir, **argv, *service, *traffic;
{
	int pid, ifd, ofd, null;
	char *sys;
	register char *s;

	for(s = bdir; *s++; )
		;
	if(rx(sys = s, service, (char *)0, traffic, &ifd, &ofd))
		exit(1);
	Finit(ifd, (char *)0);
	Finit(ofd, (char *)0);
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	if(pid){	/* copy back output */
		null = 0;
		while(s = Frdline(ifd)){
			if(!(null = *s == 0)){
				Fwrite(1, s, (long)FIOLINELEN(ifd));
				Fputc(1, '\n');
			}
		}
		if(!null){
			fprint(2, "unexpected eof from %s!%s; see /n/%s/usr/backup/log\n", sys, service, sys);
			exit(1);
		} else
			exit(0);
	}
	for(; *argv && (**argv == '-'); argv++)
		Fwrite(ofd, *argv, (long)strlen(*argv)+1);
	if(*argv){
		for(; *argv; argv++)
			Fwrite(ofd, *argv, (long)strlen(*argv)+1);
		Fputc(ofd, '\n');
	} else {
		Fputc(ofd, '\n');
		while(s = Frdline(0))
			Fprint(ofd, "%s\n", s);
	}
	exit(0);
}
