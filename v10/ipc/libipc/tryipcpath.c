extern char *ipcpath();

main()
{
	char buf[128];

	while(gets(buf)!=0)
		printf("%s -> %s\n", buf, ipcpath(buf, "dk", "tcp.25"));
}
