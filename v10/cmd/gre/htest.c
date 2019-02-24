#include	<stdio.h>
#include	"re.h"

#ifdef	PROFILING
short profb[50000];
#endif

main(argc, argv)
	char **argv;
{
	re_re *r;
	char *pat, *s;
	char buf[4096];
	unsigned char map[256];
	FILE *exprs[1024];
	char *match[10][2];
	int n, i, bufc;
	extern re_debug;

#ifdef	PROFILING
	{ extern etext(); monitor((int (*)())2, etext, profb, ((int)etext) - 2+12+2400, 300); }
#endif
	if((argc%2) || (argc < 2)){
		fprintf(stderr, "Usage: %s {pattern file}* sequence\n", argv[0]);
		fprintf(stderr, "\tEg: %s '^(a|b)+$' temp0 '(gh+|hb)' temp1  01101\n", argv[0]);
		exit(1);
	}
	for(n = 0; n < 256; n++)
		map[n] = n;
	for(n = 0, i = 1; i < argc-1; i++, n++){
		pat = argv[i++];
		r = re_recomp(pat, pat+strlen(pat), map);
		if(r == 0)
			exit(1);
		if((exprs[n] = fopen(argv[i], "w+r")) == NULL){
			perror(argv[i]);
			exit(1);
		}
		re_refile(r, exprs[n]);
	}
	bufc = fread(buf, 1, sizeof buf, stdin);
	for(s = argv[i]; *s; s++){
		rewind(exprs[n = *s-'0']);
		r = re_filere(exprs[n]);
re_debug = 20;
		if(re_reexec(r, buf, buf+bufc, match)){
			printf("%s matched:", argv[1+2*n]);
			for(i = 0; i < 10; i++)
				if(match[i][0]){
					printf(" G%d='", i);
					fwrite(match[i][0], 1, match[i][1]-match[i][0], stdout);
					putchar('\'');
				}
			putchar('\n');
		}
		re_refree(r);
	}
	exit(0);
}

void
re_error(char *s)
{
	fprintf(stderr, "pattern error: %s\n", s);
	/* NOTREACHED */
}
