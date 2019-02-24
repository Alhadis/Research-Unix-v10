#include <stdio.h>
#include <signal.h>
#include <time.h>

inter()
{
	printf("interrupted - no orders sent\n");
	exit(0);
}

struct Data {
	char *prompt;
	char store[512];
} data[] = {
	{ "Book title", "" },
	{ "Author(s)",	"" },
	{ "Publisher", "" },
	{ "Publication Date", "" },
	{ "ISBN",	"" },
	{ "Other info (e.g. volume,edition,paperback)", "" },
	{ "Estimated price",	"" },
	{ "Number of copies",	"" },
	{ 0, 0 }
};

main(argc, argv)
	char *argv[];
{
	int i, cnt=0; char More[8];
	FILE *fd, *fdtmp;
	char Tmp[L_tmpnam], User[64], Log1[512], Log2[512], Sharon[512];
	long clock = time(0L);
	struct tm *tm = localtime(&clock);

	signal(SIGINT, inter);
	printf("1127 - Fast(er) Book Ordering System\n");
	printf("====================================\n");
	fflush(stdout);

	if ((fd = fopen("/lib/book_orders", "a")) == NULL)
	{	printf("error: cannot open /lib/book_orders\n");
		exit(1);
	}
	tmpnam(Tmp);
	if ((fdtmp = fopen(Tmp, "w")) == NULL)
	{	printf("error: cannot create temporary file\n");
		exit(1);
	}

	getlogname(User);
	sprintf(Log1, "Book Order by %s, %s", User, asctime(tm));

again:	cnt++;
	for (i = 0; data[i].prompt; i++)
	{	printf("%s: ", data[i].prompt);
		fflush(stdout);
		fgets(data[i].store, 510, stdin);
	}
	fprintf(fdtmp, "\n%s", Log1);
	fprintf(fdtmp, "================================================\n");
	fprintf(fd, "%s", data[0].store); fflush(fd);

	for (i = 0; data[i].prompt; i++)
	{	fprintf(fdtmp, "%s: %s", data[i].prompt, data[i].store);
		memset(data[i].store, 0, 512);
	}
	fprintf(fdtmp, "      ==forwarded to sales@clbooks.com==        \n");
	fflush(fdtmp);

	printf("More (y/n)? ");
	fflush(stdout);
	fgets(More, 6, stdin);
	if (strcmp(More, "y\n") == 0)
		goto again;

	sprintf(Log2, "%d x %s", cnt, Log1);
	fprintf(fd, "%s", Log2); fflush(fd);
	fclose(fd);
	sprintf(Sharon, "/bin/mail %s sharon sales@clbooks.com < %s; rm -f %s",
		User, Tmp, Tmp);
	if (system(Sharon) == 127)
	{	printf("error on logfile - mail sharon\n");
		exit(1);
	}
	printf("\n%d order%s sent\n", cnt, (cnt>1)?"s":"");
}
