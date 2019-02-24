main()
{	char buf[256];

	qreset();
	qcmd("\015", 1);
	qpause();
	qread(buf, 14);
	qpause();
	printf("Reset: %s\n", buf);
	qcmd("\021", 1);	/* beep */
	qpause();
	printf("Reset: %s\n", buf);
	exit(0);
}
