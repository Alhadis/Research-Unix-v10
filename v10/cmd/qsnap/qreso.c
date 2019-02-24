main(argc, argv)
	char **argv;
{
	qreset();
	if (argc > 1 && argv[1][0] == '2')
	{	qcmd("\052", 1);
		printf("2k mode\n"); 
	} else
	{	qcmd("\053", 1);
		printf("4k mode\n"); 
	}
	exit(0);
}
