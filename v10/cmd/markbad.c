/*
 * markbad name b1 ...
 * make a file with blocks b1 ... in it,
 * escaping the ordinary allocation mechanism.
 * Used to capture bad disk blocks.
 */


main(argc, argv)
char **argv;
{
	if (argc != 3) {
		printf("markbad filename block\n");
		exit(1);
	}
	if(mknod(argv[1], 0, atoi(argv[2]))<0) {
		printf("refused\n");
		exit(1);
	}
	exit(0);
}
