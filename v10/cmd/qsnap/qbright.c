#include <stdio.h>

char nq[9] = { 36, 58, 81, 103, 126, 148, 171, 193, 220 };
	      
main(argc, argv)
	char **argv;
{
	char buf[256];
	int i=1, j, k=0;
	int oval;
	extern int flag;

	qreset();
	qpause();
	if (argc > 1)
	{	while (argc-- > 1 && k < 9)
		{	sscanf(argv[i++], "%d", &j);
			nq[k++] = (j<36)?36:(j>220)?220:j;	
		}
	qpause();
		qcmd("\072", 1);
	qpause();
		qpause();
		qwrite(nq,   9);
	qpause();
		qpause();
		printf("N.B. new values take effect after the recalibration\n");
		printf("     completes: wait for unix prompt or ready light\n");
		fflush(stdout);
		oval = flag;
		qcmd("\047", 1);
	qpause();
		qpause();
		while (flag == oval)
			sleep(30);
		printf("ready\n");
	}
	qcmd("\073", 1);
	qpause();
	qread(buf, 10);
	printf("Levels:");
	for(i = 0; i < buf[0]; i++)
		printf("  %d~%d", i, buf[i+1]&0xFF);
	printf("\n");

}
