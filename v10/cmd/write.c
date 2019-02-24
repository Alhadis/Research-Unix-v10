#include <stdio.h>
#include <signal.h>
#include <utmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utsname.h>

#define	MAXPPL	15	/* maximum number of people to whom we can write */
#define	TTYSIZ	16	/* path-name size for ttys */
#define UDIRSIZ	8	/* size of fields in utmp */

int num;
int fd[MAXPPL + 1];
char *logname, *mytty, *sysname;
struct utmp ut[MAXPPL + 1];

main(argc, argv)
int argc;
char **argv;
{
	int x;
	int cleanup();
	char inp[BUFSIZ], mesg[BUFSIZ], strm[MAXPPL + 1][TTYSIZ];
	char *getlogin(), *ttyname();
	struct utsname ourname;

	signal(SIGHUP, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGQUIT, cleanup);

	logname = getlogin();
	if ((mytty = ttyname(3)) != NULL) {
		mytty += 5;		/* strip "/dev/" off of tty */
		cktty(logname, mytty, 1);
	}
	uname(&ourname);
	sysname = ourname.nodename;

	if (argc > MAXPPL + 1 || argc < 2) {
		sprintf(mesg, "usage: write person1 [... person%d]\n", MAXPPL);
		error(mesg);
	}
	num = argc - 1;
	if ((fd[0] = open("/etc/utmp", 0)) < 0)
		error("write: Cannot open /etc/utmp.\n");
	while (read(fd[0], &ut[0], sizeof(struct utmp)) == sizeof(struct utmp))
		for (x = 1; x <= num; x++)
			if (strncmp(ut[0].ut_name, argv[x], UDIRSIZ) == 0 ||
			    strncmp(ut[0].ut_line, argv[x], UDIRSIZ) == 0)
				ut[x] = ut[0];

	for (x = 1; x <= num; x++) {
		if(ut[x].ut_name[0] == 0){
			sprintf(mesg, "write: %s is not on.\n", argv[x]);
			error(mesg);
		}
		if (strcmp(ut[x].ut_line, argv[x]) == 0)
			strcpy(ut[x].ut_name, ut[x].ut_line);
		cktty(ut[x].ut_name, ut[x].ut_line, 0);
		devoftty(strm[x], ut[x].ut_line);
		if ((fd[x] = open(strm[x], 1)) < 0) {
			sprintf(mesg, "write: Cannot open %s's tty.\n", argv[x]);
			error(mesg);
		}
	}

	people(mesg);
	for (x = 1; x <= num; x++) {
		write(fd[x], "\007\007\007write: ", 10);
		write(fd[x], sysname, strlen(sysname));
		write(fd[x], "!", 1);
		write(fd[x], logname, strlen(logname));
		if (mytty) {
			write(fd[x], " on ", 4);
			write(fd[x], mytty, strlen(mytty));
		}
		write(fd[x], " to ", 4);
		write(fd[x], mesg, strlen(mesg));
		write(fd[x], ".\n", 2);
	}
	if (num > 1)
		used();
	while (fgets(inp, 512, stdin) != NULL)
		if (inp[0] == '!')
			shell(inp);
		else if (inp[0] == ':')
			colon(inp);
		else
			for (x = 1; x <= num; x++) {
				if (num != 1) {
					write(fd[x], logname, strlen(logname));
					write(fd[x], ": ", 2);
				}
				write(fd[x], inp, strlen(inp));
			}
	cleanup();
}

cleanup()
{
	int x;

	for (x = 1; x <= num; x++) {
		write(fd[x], "write: ", 7);
		write(fd[x], logname, strlen(logname));
		write(fd[x], ": EOF\n", 6);
		close(fd[x]);
	}
	close(fd[0]);
	exit(0);
}

people(list)
char *list;
{
	int x;

	strncpy(list, ut[1].ut_name, UDIRSIZ);
	list[UDIRSIZ] = '\0';
	if (num > 2) {
		for (x = 2; x < num; x++) {
			strcat(list, ", ");
			strncat(list, ut[x].ut_name, UDIRSIZ);
		}
		strcat(list, ",");
	}
	if (num > 1) {
		strcat(list, " and ");
		strncat(list, ut[num].ut_name, UDIRSIZ);
	}
}

shell(str)
char *str;
{
	int frkd;
	int cleanup();

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if ((frkd = fork()) < 0)
		printf("write: Cannot fork.\n");
	if (frkd == 0) {
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execl("/bin/sh", "sh", "-c", str + 1, 0);
		exit(0);
	}
	if (frkd > 0)
		wait(0);
	signal(SIGINT, cleanup);
	signal(SIGQUIT, cleanup);
	printf("!\n");
}

colon(arg)
char *arg;
{
	char arg1[BUFSIZ], arg2[BUFSIZ];

	used();
	strcpy(arg1, "\0");
	strcpy(arg2, "\0");
	sscanf(arg, ":%s %s\n", arg1, arg2);
	if (strlen(arg1) <= 1) {
		if (strcmp(arg2, "\0") == 0 && (strcmp(arg1, "a") == 0 ||
		    strcmp(arg1, "d") == 0))
			printf("write: missing argument to colon escape.\n");
		else
			switch(arg1[0]) {
			case 'a':
				add(arg2);
				break;
			case 'd':
				drop(arg2);
				break;
			case 'l':
				plist();
				break;
			default:
				printf("write: unknown colon escape.\n");
			}
	}
	else
		printf("write: unknown colon escape.\n");
}

add(name)
char *name;
{
	int x;
	char dev[TTYSIZ], list[BUFSIZ];

	if (num + 1 > MAXPPL) {
		printf("write: too many people.\n");
		return;
	}

	lseek(fd[0], 0L, 0);
	while (read(fd[0], &ut[0], sizeof(struct utmp)) == sizeof(struct utmp))
		if (strcmp(ut[0].ut_name, name) == 0 ||
		    strcmp(ut[0].ut_line, name) == 0)
			ut[num + 1] = ut[0];

	if (strcmp(ut[num + 1].ut_name, "\0") == 0) {
		printf("write: %s is not on.\n", name);
		return;
	}

	if (strcmp(ut[num + 1].ut_line, name) == 0)
		strcpy(ut[num + 1].ut_name, ut[num + 1].ut_line);

	if (cktty(name, ut[num + 1].ut_line, 1))
		return;
	devoftty(dev, ut[num + 1].ut_line);
	if ((fd[num + 1] = open(dev, 1)) < 0) {
		printf("write: Cannot open %s's tty.\n", name);
		return;
	}

	num++;
	people(list);
	write(fd[num], "\007\007\007write: ", 10);
	write(fd[num], sysname, strlen(sysname));
	write(fd[num], "!", 1);
	write(fd[num], logname, strlen(logname));
	if (mytty) {
		write(fd[num], " on ", 4);
		write(fd[num], mytty, strlen(mytty));
	}
	write(fd[num], " to ", 4);
	write(fd[num], list, strlen(list));
	write(fd[num], ".\n", 2);
	for (x = 1; x < num; x++) {
		write(fd[x], "\007\007\007write: ", 10);
		write(fd[x], logname, strlen(logname));
		if (mytty) {
			write(fd[x], " on ", 4);
			write(fd[x], mytty, strlen(mytty));
		}
		write(fd[x], " added ", 7);
		write(fd[x], name, strlen(name));
		write(fd[x], ".\n", 2);
	}
}

drop(name)
char *name;
{
	int x;
	int place = 0;

	for (x = 1; x <= num; x++)
		if (strcmp(ut[x].ut_name, name) == 0)
			place = x;
	if (place == 0)
		printf("write: %s is not in the conversation.\n", name);
	else {
		write(fd[place], "\007\007\007write: ", 10);
		write(fd[place], logname, strlen(logname));
		if (mytty) {
			write(fd[place], " on ", 4);
			write(fd[place], mytty, strlen(mytty));
		}
		write(fd[place], " dropped you.\n", 14);
		close(fd[place]);
		for (x = place; x <= num; x++) {
			fd[x] = fd[x + 1];
			ut[x] = ut[x + 1];
		}
		num--;

		for (x = 1; x <= num; x++) {
			write(fd[x], "\007\007\007write: ", 10);
			write(fd[x], logname, strlen(logname));
			if (mytty) {
				write(fd[x], " on ", 4);
				write(fd[x], mytty, strlen(mytty));
			}
			write(fd[x], " dropped ", 9);
			write(fd[x], name, strlen(name));
			write(fd[x], ".\n", 2);
		}
	}
}

plist()
{
	char list[BUFSIZ];

	people(list);
	if (strlen(list) < 1)
		printf("write: You're not talking to anyone.\n");
	else
		printf("write: You're talking to %s.\n", list);
}

cktty(user, tty, flag)
int flag;
char *user, *tty;
{
	char bfr[TTYSIZ], msg[BUFSIZ];
	struct stat sbuf;

	devoftty(bfr, tty);
	if (stat(bfr, &sbuf) < 0) {
		sprintf(msg, "write: Cannot stat %s (%s's tty).\n", bfr, user);
		if (flag) {
			fprintf(stderr, msg);
			return(1);
		}
		error(msg);
	}
	if (!(sbuf.st_mode & 02)) {
		sprintf(msg, "write: %s's tty is protected.\n", user);
		if (flag) {
			fprintf(stderr, msg);
			return(1);
		}
		error(msg);
	}
}

devoftty(buf, tty)
	char *buf, *tty;
{
	sprintf(buf, "/dev/%s", tty);
	buf[13] = 0;	/* strlen("/dev/")+8 */
}

error(s)
char *s;
{
	fprintf(stderr, s);
	exit(1);
}

used()
{
	int fd;
	char buf[20];

	if ((fd = open("/tmp/wacct", 2)) < 0)
		return;
	sprintf(buf, "%s\n", logname);
	lseek(fd, 0L, 2);
	write(fd, buf, strlen(buf));
	close(fd);
}
