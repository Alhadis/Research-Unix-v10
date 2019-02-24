main(int argc, char **argv)
{
	char cansys[256];

	while (--argc) {
		canonsys(*++argv, cansys);
		puts(cansys);
	}
	return 0;
}
