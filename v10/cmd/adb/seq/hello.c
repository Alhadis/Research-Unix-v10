main(argc, argv)
int argc;
char **argv;
{
	while (--argc > 0)
		printf("%s\n", *++argv);
}
