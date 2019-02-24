main(argc,argv)
	char **argv;
{
int k;
system("pwd");
k = chdir(argv[1]);
printf("chdir returns %d, new directory ", k);
system("pwd");
}
