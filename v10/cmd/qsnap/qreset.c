main()
{	char buf[256];

	qreset();
	version(buf);
	printf("Reset: %s\n", buf);
	exit(0);
}
