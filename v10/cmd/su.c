#include <stdio.h>
#include <pwd.h>
#include <time.h>
#include <signal.h>

struct	passwd *pwd,*getpwnam();
char	*crypt();
char	*getpass();
char	**environ;
char *name = "root";
char *shell = "/bin/sh";
char *sulog = "/dev/console";

main(argc,argv)
int	argc;
char	**argv;
{
	register char **p;
	char *password;
	int newgid, newuid;
	if(argv[0][0] != '/'){
		fprintf(stderr, "su must be invoked with a full path\n");
		exit(1);
	}

	if(argc > 1)
		name = argv[1];
	if((pwd=getpwnam(name)) == NULL) {
		printf("Unknown id: %s\n",name);
		exit(1);
	}
	if(pwd->pw_passwd[0] != '\0' && getuid() != 0) {
		password = getpass("Password:");
		if(strcmp(pwd->pw_passwd,crypt(password,pwd->pw_passwd))!=0) {
			logit(sulog,0);
			printf("Sorry\n");
			exit(2);
		}
	}

	newgid = pwd->pw_gid;
	newuid = pwd->pw_uid;
	logit(sulog,1);
	setgid(newgid);
	setuid(newuid);
	if (pwd->pw_shell && *pwd->pw_shell)
		shell = pwd->pw_shell;
	if (newuid == 0)
		for (p=environ; *p; p++) {
			if (strncmp("PS1=", *p, 4) == 0)
				*p = "PS1=# ";
			else if (strncmp("PATH=", *p, 5) == 0)
				*p = "PATH=/bin:/usr/bin:/etc";
		}
	execl(shell, "su", "-p", 0);
	perror(shell);
	exit(3);
}
logit(logfile,how)
char *logfile;
{
	int catch();
	long now, time();
	char *cuserid(), *strrchr();
	char *ttyn, *ttyname();
	FILE *logf = fopen(logfile,"a");
	struct tm *tmp, *localtime();

	if(logf == NULL) return;
	if((ttyn=ttyname(3))==NULL)
		ttyn="/dev/tty??";
	now = time((long *)0);
	tmp = localtime(&now);
	signal(SIGALRM, catch);
	alarm(5);
	fprintf(logf,"\r\nSU %.2d/%.2d %.2d:%.2d %c %s %s-%s\r\n",
		tmp->tm_mon+1,tmp->tm_mday,tmp->tm_hour,tmp->tm_min,
		how?'+':'-',(strrchr(ttyn,'/')+1),cuserid(),name);
	fclose(logf);
	alarm(0);
}
char *cuserid()
{
	static char s[20];
	struct passwd *getpwuid();
	int uid = getuid();
	struct passwd *pwd = getpwuid(uid);

	if(pwd && pwd->pw_name && *pwd->pw_name)
		return pwd->pw_name;
	sprintf(s, "%d", uid);
	return s;
}
catch(){}
