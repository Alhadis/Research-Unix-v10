/*
 * login  [ -f  name ] [ -p passwdline ] [ command ]
 *    -f:  if su, log in with no password
 *    -p:  if su, use entire password line.
 *    command: if given, just execute command
 */

#include <sys/param.h>
#include <sys/ttyio.h>
#include <utmp.h>
#include <signal.h>
#include <pwd.h>
#include <libc.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/vlimit.h>

#define SCPYN(a, b)	strncpy(a, b, sizeof(a))

#define ISIZE 32
#define POSTMKSIZ sizeof "From  Sun Jan 00 00:00:00 1979"
char	maildir[30] =	"/usr/spool/mail/";
struct	passwd nouser = {"", "nope"};
struct	utmp utmp, xtmp;
char	minusnam[16] = "-";
char	homedir[64] = "HOME=";
char	path[] = "PATH=:/bin:/usr/bin";
char	**env;
int	nenv = 0;
char	nolog[] = "/etc/nologin";
char	motd[] = "/etc/motd";
struct	passwd *pwd;
struct	passwd *pwdecode();
char	*cmd;

extern	char **environ;

main(argc, argv)
char **argv;
{
	register char *namep;
	register int i, f;
	char input[ISIZE];
	char *ttyn;
	int neednopass = 0;
	int hangitup = 0;
	int ntries = 0;
	struct ttydevb tb;

	alarm(60);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	nice(-100);
	nice(20);
	for (i=NSYSFILE; i<NOFILE; i++)
		close(i);
	ttyn = ttyname(0);
	if (ttyn==0)
		SCPYN(utmp.ut_line, "tty??");
	else if (strncmp(ttyn, "/dev/", 5) == 0)
		SCPYN(utmp.ut_line, ttyn+5);
	else
		SCPYN(utmp.ut_line, ttyn);	/* shouldn't happen */
	SCPYN(input, "");
	switch(argc) {
	case 0:
	case 1:
		break;

	case 2:
		SCPYN(input, argv[1]);
		break;

	default:
		if (strcmp(argv[1], "-f")==0 || strcmp(argv[1], "-p")==0) {
			if (getuid()!=0) {
				printf("login: not super-user\n");
				exit(1);
			}
			neednopass++;
			if (strcmp(argv[1], "-f")==0)
				SCPYN(input, argv[2]);
			else {
				pwd = pwdecode(argv[2]);
				SCPYN(input, pwd->pw_name);
			}
			if (argc>3)
				cmd = argv[3];
		} else
			exit(1);
	}
    loop:
	if (ntries) {
		if (ntries > 5 || hangitup) {
			ioctl(0, TIOCGDEV, &tb);
			tb.ispeed = tb.ospeed = 0;
			ioctl(0, TIOCSDEV, &tb);
			sleep(5);
			exit(1);
		}
		neednopass = 0;
		pwd = NULL;
		SCPYN(input, "");
	}
	ntries++;
	while (input[0] == '\0') {
		namep = input;
		printf("login: ");
		while ((i = getchar()) != '\n') {
			if(i == ' ')
				i = '_';
			if (i == EOF)
				exit(0);
			if (namep < input + ISIZE - 1)
				*namep++ = i;
		}
		*namep = NULL;
	}
	SCPYN(utmp.ut_name, input);
	utmp.ut_time = 0;
	if (pwd == NULL) {
		setpwent();
		if ((pwd = getpwnam(input)) == NULL)
			pwd = &nouser;
		endpwent();
	}
	if (namep = strchr(utmp.ut_name, '\001'))
		if (namep[1]=='L' && namep[2]=='\002')	/* loopback? */
			hangitup++;
	time(&utmp.ut_time);
	if (*pwd->pw_passwd != '\0' && !neednopass) {
		namep = crypt(getpass("Password:"), pwd->pw_passwd);
		if (strcmp(namep, pwd->pw_passwd)) {
			/* magic string detects loopbacks */
			printf("\001L\002ogin incorrect\n");
			f = open("/usr/adm/xtmp", 1);
			if (f > 0) {
				lseek(f, 0L, 2);
				write(f, (char *)&utmp, sizeof(utmp));
				close(f);
			}
			goto loop;
		}
	}
	if(pwd->pw_uid != 0 && access(nolog, 0) == 0){
		showmotd(nolog);
		exit(0);
	}
	if(chdir(pwd->pw_dir) < 0) {
		printf("No directory\n");
		if(pwd->pw_uid != 0 || (access(nolog, 0) < 0))
			goto loop;
	}
	setlogname(utmp.ut_name);
	if(setupshares(pwd->pw_uid, printf))
		goto loop;
	if (cmd) {		/* remote exec */
		i = strlen(utmp.ut_name);
		if (i < sizeof(utmp.ut_name))
			utmp.ut_name[i] = '*';
	}
	if (ttyn && cmd == 0)
		setut(&utmp);
	if ((f = open("/usr/adm/wtmp", 1)) >= 0) {
		lseek(f, 0L, 2);
		write(f, (char *)&utmp, sizeof(utmp));
		close(f);
	}
	if (ttyn) {
		chown(ttyn, pwd->pw_uid, pwd->pw_gid);
		chmod(ttyn, 0622);
	}
	setgid(pwd->pw_gid);
	setuid(pwd->pw_uid);
	if (*pwd->pw_shell == '\0')
		pwd->pw_shell = "/bin/sh";
	strncat(homedir, pwd->pw_dir, sizeof(homedir)-6);
	nenv = 0;
	for(i = 0; environ[i]; i++)
		;
	env = (char **) malloc(sizeof(char *) * (i + 10));
	if (env == NULL) {
		printf("No memory for environment.\n");
		exit(1);
	}
	for (i = 0; environ[i]; i++) {
		if (strncmp(environ[i], "HOME=", 5) == 0)
			continue;
		if (strncmp(environ[i], "PATH=", 5) == 0)
			continue;
		env[nenv++] = environ[i];
	}
	if(homedir[0])
		env[nenv++] = homedir;
	if(path[0])
		env[nenv++] = path;
	env[nenv] = NULL;
	if ((namep = strrchr(pwd->pw_shell, '/')) == NULL)
		namep = pwd->pw_shell;
	else
		namep++;
	strcat(minusnam, namep);
	alarm(0);
	umask(02);
	vlimit(LIM_CORE, 1024*1024);
	if (cmd==NULL) {
		showmotd(motd);
		strcat(maildir, pwd->pw_name);
		if(access(maildir,4)==0) {
			struct stat statb;
			stat(maildir, &statb);
			if (statb.st_size > POSTMKSIZ)
				printf("You have mail.\n");
		}
	}
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGHUP, SIG_DFL);
	environ = env;
	fflush(stdout);
	if (cmd==NULL)
		execlp(pwd->pw_shell, minusnam, 0);
	else {
		env[nenv++] = "REXEC=1";
		ioctl(0, TIOCEXCL, (void *)NULL);
		env[nenv] = 0;
		execlp(pwd->pw_shell, minusnam, "-c", cmd, (char *)0);
	}
	printf("No shell\n");
	exit(0);
}

setut(u)
register struct utmp *u;
{
	register struct utmp *v, *ev;
	struct utmp xtmp[200];
	register int slot;
	register int n, i;
	int fd;

	if ((fd = open("/etc/utmp", 2)) < 0)
		return;
	slot = -1;
	i = 0;
	while ((n = read(fd, (char *)xtmp, sizeof(xtmp))) > 0) {
		ev = &xtmp[n/sizeof(struct utmp)];
		for (v = xtmp; v < ev; i++, v++) {
			if (slot < 0 && v->ut_line[0] == 0) {
				slot = i;
				continue;
			}
			if (strncmp(u->ut_line, v->ut_line, sizeof(v->ut_line))==0) {
				slot = i;
				goto found;
			}
		}
	}
	if (slot < 0)		/* no empties found */
		slot = i;
found:
	lseek(fd, (off_t)slot*sizeof(struct utmp), 0);
	write(fd, (char *)u, sizeof(*u));
	close(fd);
}

int	stopmotd;
catch()
{
	signal(SIGINT, SIG_IGN);
	stopmotd++;
}

showmotd(f)
char *f;
{
	register FILE *mf;
	register c;

	signal(SIGINT, catch);
	if((mf = fopen(f, "r")) != NULL) {
		while((c = getc(mf)) != EOF && stopmotd == 0)
			putchar(c);
		fclose(mf);
		fflush(stdout);
	}
	signal(SIGINT, SIG_IGN);
}
