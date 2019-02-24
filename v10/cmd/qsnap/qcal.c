main()
{	int oval;
	extern int flag;


	qreset();
	oval = flag;
	qcmd("\047", 1);
	while (flag == oval) sleep(100);
}
