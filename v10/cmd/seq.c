#include <stdio.h>

double	min=1.0;
double	max=0.0;
double	incr=1.0;
int	const=0;
int	nsteps;
char	*format;

extern double atof();
extern char *strchr();

main(argc, argv)
	char *argv[];
{
	register i;
	char buf[BUFSIZ], ffmt[BUFSIZ];

	while(argc>1 && argv[1][0]=='-'){
		switch(argv[1][1]){
		case 'w':
			const++;
			break;
		case 'f':
			format= &argv[1][2];
			if(format[strlen(format)-1] != '\n')
				sprintf(format = ffmt, "%s\n", &argv[1][2]);
			break;
		default:
			goto out;
		}
		--argc;
		argv++;
	}
    out:
	if(argc<2 || argc>4)
		usage();
	max=atof(argv[argc-1]);
	if(argc>2)
		min=atof(argv[1]);
	if(argc>3)
		incr=atof(argv[2]);
	if(incr==0){
		fprintf(stderr, "seq: zero increment\n");
		exit(1);
	}
	nsteps=(max-min)/incr+.5;
	if(!format)
		buildfmt();
	for(i=0; i<=nsteps; i++)
		printf(format, min+i*incr);
	return 0;
}
usage(){
	fprintf(stderr, "usage: seq [-fformat] [-w] [first [incr]] last\n");
	exit(1);
}
buildfmt()
{
	register i;
	register char *dp;
	int w, p, maxw=0, maxp=0;
	static char fmt[16];
	char buf[32];

	format = "%g\n";
	if(!const)
		return;
	for(i=0; i<=nsteps; i++) {
		sprintf(buf,"%g",min+i*incr);
		if(strchr(buf,'e')!=0)
			return;
		dp = strchr(buf,'.');
		w = dp==0? strlen(buf): dp-buf;
		p = dp==0? 0: strlen(strchr(buf,'.')+1);
		if(w>maxw) maxw = w;
		if(p>maxp) maxp = p;
	}
	if(maxp>0) maxw += maxp+1;
	sprintf(fmt,"%%0%d.%df\n",maxw,maxp);
	format = fmt;
}
