#include	<string.h>
#include	<fio.h>

#define		MAXARG		256		/* no sys name is longer than this */

#define		DK	"/usr/lib/uucp/Systems.dk"

main(argc, argv)
	char **argv;
{
	int fd;
	char *s, *ss, *sss;
	register char **ap;

	argv++;
	if(--argc <= 0){
		Fprint(2, "Usage: dkname sysname ...\n");
		exit(1);
	}
	/* make sure we get plausible length args */
	for(ap = argv; *ap; ap++)
		if(strlen(*ap) > MAXARG)
			(*ap)[MAXARG] = 0;
	if((fd = open(DK, 0)) < 0){
		perror(DK);
		exit(1);
	}
	while(s = Frdline(fd)){
		if((ss = strchr(s, ' ')) == 0)
			continue;	/* bad line */
		*ss++ = 0;
		if((ss = strchr(ss, ' ')) == 0)
			continue;	/* bad line */
		if((ss = strchr(ss+1, ' ')) == 0)
			continue;	/* bad line */
		if((ss = strchr(ss+1, ' ')) == 0)
			continue;	/* bad line */
		for(ap = argv; *ap; ap++)
			if(strcmp(*ap, s) == 0){
				**ap = 0;
				argc--;
				sss = ss+1;
				if(ss = strchr(sss, ' '))
					*ss = 0;
				if(ss = strchr(sss, '.'))
					*ss = 0;
				Fprint(1, "%s\n", sss);
			}
		if(argc == 0) break;
	}
	for(fd = 0, ap = argv; *ap; ap++)
		if(**ap)
			Fprint(2, "`%s' not found\n", *ap), fd = 1;
	exit(fd);
}
