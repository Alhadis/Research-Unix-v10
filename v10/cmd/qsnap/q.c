#include <stdio.h>

int flag = 0;
int Errors=0;

static
srq()
{	signal(2, srq);
	flag = 1 - flag;
	printf("<SRQ>");
	fflush(stdout);
}

qreset()
{	register i;

	printf("waiting for reset.."); fflush(stdout);
	signal(2, srq);
	if ((i = gpib(4)) < 0)
		iberr(i, "Gpib Clear");
	if ((i = gpib(5)) < 0)
		iberr(i, "Gpib Remote");
	sleep(12);
	printf("done\n"); fflush(stdout);
	Errors=0;
}

qcmd(cmd, len)
	char *cmd;
{
	register i;
	if (Errors) return;
	/* unlisten, untalk, interface talk, qcr listen */
	if ((i = gpib(0, "\077\137\043\125", 4)) < 0)
	{	iberr(i, "cmd setup");
		Errors++;
		return;
	}
	if ((i = gpib(1, cmd, len, 2)) < 0)
	{	iberr(i, "cmd send");
		Errors++;
		return;
	}
}

qwrite(cmd, len)
	char *cmd;
{
	register i;

	if (Errors) return;
	/* unlisten, untalk, interface talk, qcr listen */
	if ((i = gpib(0, "\077\137\042\125", 4)) < 0)
	{	iberr(i, "write setup");
		Errors++;
		return 0;
	}
	if ((i = gpib(1, cmd, len, 2)) < 0)
	{	iberr(i, "write cmd");
		Errors++;
		return 0;
	}
	return i;
}

qread(cmd, len)
	char *cmd;
{
	register i;

	if (Errors) return;
	/* unlisten, untalk, interface talk, qcr listen */
	if ((i = gpib(0, "\077\137\102\065", 3)) < 0)
	{	iberr(i, "read setup");
		Errors++;
		return 0;
	}
	if ((i = gpib(2, cmd, len, 0)) < 0)
	{	iberr(i, "read cmd");
		Errors++;
		return 0;
	}
	return i;
}

qpause()
{	int i;

	for (i=1000*500; i>0; i--)	/* was 1000*200 */
		;
}
