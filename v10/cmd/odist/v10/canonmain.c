main(int argc, char **argv)
{
	char cansys[256];

	while (--argc) {
		canon(*++argv, cansys);
		puts(cansys);
	}
	return 0;
}
