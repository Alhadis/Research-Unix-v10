main(argc,argv)
char **argv;
{
	char *p, *q;
	if(argc < 2) {
		printf("\n");
		return 0;
	}
	for(p = argv[1]; *p; ) 
		if(*p++ == '/')
			argv[1] = p;
	if(argc > 2) {
		for(q = argv[2]; *q; q++) ;
		while(--q>=argv[2] && --p>=argv[1] && *p==*q) ;
		if(q < argv[2])
			*p = 0;
	}
	printf("%s\n", argv[1]);
	return 0;
}
