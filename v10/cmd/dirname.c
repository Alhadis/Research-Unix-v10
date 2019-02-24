main(argc, argv)
char **argv;
{
	char *p;
	if(argc<2 || *argv[1]==0)
		fini(".");
	for(p=argv[1]; *p; p++)
		continue;
	while(p>=argv[1] && *--p=='/')
		;
	if(p < argv[1])
		fini("/");
	while(p>=argv[1] && *p!='/')
		p--;
	if(p < argv[1])
		fini(".");
	while(p>=argv[1] && *p=='/')
		p--;
	if(p < argv[1])
		fini("/");
	p[1] = 0;
	fini(argv[1]);
}

fini(s)
char *s;
{
	printf("%s\n",s);
	exit(0);
}
