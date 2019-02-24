/*
 * enter a password in the password file
 * this program should be suid with owner
 * with an owner with write permission on /etc/passwd
 */
#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <errno.h>
#include <ctype.h>
#include <libc.h>

char	passwd[] = "/etc/passwd";
char	npasswd[] = "/etc/npasswd";
char	tpasswd[] = "/etc/pwXXXXXX";
char	stdprof[] = "/etc/stdprofile";
int	newmode = 0755;
char	*stdgroup = "unknown";
char	*group;
int	flags;
char	*tempfile;
struct	passwd *pwd;
long	time();
char	*mktemp();
char	*readpasswd();
struct	passwd *getpwent();
int	endpwent();
char	*strcpy();
char	*crypt();
char	*getpass();
char	*getlogin();
extern	int	optind;
extern	int	errno;
int	newuser;
int	alter;
char	*username;
struct	passwd	pw;

char	*getstring();
char	*newstr();
int	rootid	= 0;

main(argc, argv)
char *argv[];
{
	char *pwp;
	int c, u, maxuid;
	FILE *tf;

	while ((c = getopt(argc, argv, "na")) != EOF) {
		switch(c) {
		case 'n':
			newuser++;
			break;

		case 'a':
			alter++;
			break;

		default:
			exit(1);
		}
	}
	if (optind >= argc) {
		if (newuser==0) {
			if ((username = getlogin()) == NULL) {
				printf ("Usage: passwd [-na] [user]\n");
				exit(1);
			} else
				printf("Changing password for %s\n", username);
		}
	} else
		username = argv[optind];
	if (username==NULL) {
		username = getstring("User: ");
		if (username==NULL || *username=='\0') {
			printf("Cannot default user\n");
			exit(1);
		}
	}
	maxuid = -1;
	while ((pwd = getpwent()) != NULL) {
		if (strcmp(pwd->pw_name, username) == 0) {
			pw.pw_name = username;
			pw.pw_passwd = newstr(pwd->pw_passwd);
			pw.pw_uid = pwd->pw_uid;
			pw.pw_gid = pwd->pw_gid;
			pw.pw_gecos = newstr(pwd->pw_gecos);
			pw.pw_dir = newstr(pwd->pw_dir);
			pw.pw_shell = newstr(pwd->pw_shell);
		}
		if (pwd->pw_uid > maxuid)
			maxuid = pwd->pw_uid;
	}
	endpwent();
	if (pw.pw_name==NULL && newuser==0) {
		printf("Cannot find password entry for %s\n", username);
		exit(1);
	}
	if (newuser) {
		if (pw.pw_name) {
			printf("user %s already exists\n", username);
			exit(1);
		}
		pw.pw_name = username;
	}
	u = getuid();
	if (u!=rootid && (newuser || pw.pw_uid!=u)) {
		printf("Permission denied.\n");
		exit(1);
	}
	pwp = readpasswd(u, pw.pw_passwd);
	if (pwp==NULL) {
		if (newuser==0) {
			printf("Password unchanged.\n");
			if (alter==0)
				exit(0);
			pwp = pw.pw_passwd;
		} else
			pwp = "";
	}
	pw.pw_passwd = pwp;
	if (alter!=0 || newuser!=0) 
		otherfields(maxuid+1, u);
	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	tempfile = mktemp(tpasswd);
	if((tf=fopen(tempfile,"w")) == NULL) {
		printf("Cannot create temporary file\n");
		exit(1);
	}
	chmod(tempfile, 0644);
	while((pwd=getpwent()) != NULL) {
		if(strcmp(pwd->pw_name,username) == 0)
			pwd = &pw;
		fprintf(tf,"%s:%s:%d:%d:%s:%s:%s\n",
			pwd->pw_name,
			pwd->pw_passwd,
			pwd->pw_uid,
			pwd->pw_gid,
			pwd->pw_gecos,
			pwd->pw_dir,
			pwd->pw_shell);
	}
	endpwent();
	if (newuser) {
		pwd = &pw;
		fprintf(tf,"%s:%s:%d:%d:%s:%s:%s\n",
			pwd->pw_name,
			pwd->pw_passwd,
			pwd->pw_uid,
			pwd->pw_gid,
			pwd->pw_gecos,
			pwd->pw_dir,
			pwd->pw_shell);
	}
	fclose(tf);
/*
 *	move temp back to passwd file
 *	(carefully)
 */
	while (link(tempfile, npasswd) < 0) {
		if (errno == EEXIST) {
			if (*getstring("Password file busy... shall I wait?") == 'y')
				sleep(5);
			else
				exit(1);
		} else {
			printf("Cannot link to temp\n");
			exit(1);
		}
	}
	if (unlink(passwd) < 0) {
		printf("Cannot unlink old passwd\n");
		exit(1);
	}
	if (link(npasswd, passwd) < 0) {
		printf("Cannot link in new passwd.\n");
		exit(1);
	}
	unlink(npasswd);
	unlink(tempfile);
	return 0;
}

char *
readpasswd(uid, opass)
char *opass;
{
	static char minlen[] =
	  { 8, 8, 6, 5, 6, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4};
	register char *p, *npass;
	register c, flags;
	int ok, tries, pwlen;
	long salt;
	char saltc[2];
	char *rev(), *multi();
	register i;

	if (uid!=0 && opass && *opass) {
		npass = getpass("Old password:");
		if (strcmp(crypt(npass, opass), opass)) {
			printf("Sorry.\n");
			exit(1);
		}
	}
	opass = NULL;
	for (ok=0, tries=0;;) {
		if (ok==0 && tries) {
			printf("Password too simple.  Try again.\n");
			opass = 0;
		}
		npass = newstr(getpass(ok?"Reconfirm password:":"New password:"));
		if (opass && strcmp(opass, npass)) {
			printf("Password mismatch.\n");
			exit(1);
		}
		if (strcmp(npass, "sorry")==0 && tries==0) {
			if (*getstring("You want an unusable password, right? ") == 'y')
				return("sorry");
		}
		if (ok)
			break;
		pwlen = strlen(npass);
		if (pwlen == 0)
			return(NULL);
		flags = 0;
		p = npass;
		while (c = *p++) {
			if (islower(c))
				flags |= 02;
			else if (isupper(c))
				flags |= 04;
			else if (isdigit(c))
				flags |= 01;
			else
				flags |= 010;
		}
		if (tries>2 || pwlen>=minlen[flags])
			ok++;
		tries++;

		if((strcmp(npass, pw.pw_name) == 0)
		|| (strcmp(npass, rev(pw.pw_name)) == 0)
		|| (strcmp(npass, multi(pw.pw_name)) == 0)){

			ok = 0;
			tries = 1;
		}


		opass = npass;
	}
	time(&salt);
	salt += getpid();
	saltc[0] = salt & 077;
	saltc[1] = (salt>>6) & 077;
	for(i=0; i<2; i++) {
		c = saltc[i] + '.';
		if(c>'9') c += 7;
		if(c>'Z') c += 6;
		saltc[i] = c;
	}
	return(newstr(crypt(npass, saltc)));
}

char *
getstring(mesg, arg)
char *mesg, *arg;
{
	char buf[128], *fgets();

	printf(mesg, arg);
	if (fgets(buf, 128, stdin) == NULL)
		exit(1);
	buf[strlen(buf)-1] = '\0';
	return(newstr(buf));
}

char *
newstr(s)
char *s;
{
	char *strcpy();
	return(strcpy(malloc(strlen(s)+1), s));
}

otherfields(uid, myuid)
{
	char *s;
	int id;

	if (newuser) {
		s = getstring("UID: ");
		while (!isnum(s)) {
			printf ("Not numeric\n");
			s = getstring ("Enter numeric user ID: ");
		}
		if (*s != '\0')
			uid = atoi (s);
		pw.pw_uid = uid;
		pw.pw_gid = 1;
	}
	if (newuser || alter) {
		char defdir[128];

		strcpy (defdir, "/usr/");
		strcat (defdir, pw.pw_name);

		s = getstring("GCOS acct,box: ");
		if (*s)
			pw.pw_gecos = s;
		if (pw.pw_gecos==NULL)
			pw.pw_gecos = "";
		s = getstring("Directory: ");
		if (*s)
			pw.pw_dir = s;
		if (pw.pw_dir==NULL)
			pw.pw_dir = newstr(defdir);
		if (newuser) {
			char *realdir;
			realdir = pw.pw_dir;
			if (realdir[0] == '*') {
				realdir++;
				if (realdir[0] != '/') {
					char x[256];
					strcpy (x, "/");
					strcat (x, realdir);
					strcat (x, "/");
					strcat (x, pw.pw_name);
					realdir = newstr (x);
				}
				pw.pw_dir = newstr(defdir);
				symlink (realdir, defdir);
			}
			mkdir(realdir, newmode);
			chown(realdir, pw.pw_uid, pw.pw_gid);
		}
		s = getstring("Shell: ");
		if (*s)
			pw.pw_shell = s;
		if (pw.pw_shell == NULL)
			pw.pw_shell = "";

		/* Create .profile for the new user */
		if (newuser) {
			FILE *inf, *outf;

			for (;;) {
				s = getstring("Profile: ");
				if (*s == '\0')
					s = stdprof;
				inf = fopen (s, "r");
				if (inf != NULL || s == stdprof)
					break;
				printf ("Can't open %s\n", s);
			}
			if (inf != NULL) {
				char pname[128];
				register int c;

				(void) strcpy (pname, pw.pw_dir);
				(void) strcat (pname, "/.profile");
				outf = fopen (pname, "w");
				if (outf == NULL) {
					printf ("can't create %s\n", pname);
					exit(1);
				}
				(void) chown (pname, pw.pw_uid, pw.pw_gid);
				(void) chmod (pname, newmode);

				/*
				 *	copy the profile file to the new
				 *	user's .profile, transforming \N into
				 *	the user's name, \D into his home
				 *	directory, and \\ into \.
				 */
				while ((c = getc (inf)) != EOF) {
					if (c == '\\') {
						c = getc (inf);
						switch (c) {

						case 'D':
							fputs (pw.pw_dir, outf);
							break;

						case 'N':
							fputs (pw.pw_name,outf);
							break;
						
						case EOF:
							break;

						default:
							putc ('\\', outf);
							/* No break */
						case '\\':
							putc (c, outf);
							break;
						}
					} else
						putc (c, outf);
				}
				if (ferror (inf) || ferror (outf))
					printf ("I/O error copying %s to %s\n",
					    s, pname);
				(void) fclose (inf);
				(void) fclose (outf);
			}
		}
	}
}

int
isnum(s)
	register char *s;
{
	while (*s)
		if (!isdigit(*s++))
			return 0;
	return 1;
}

char *rev(s)
char *s;
{

	static char t[200];
	char *p, *q;

	p = s;
	q = &t[199];
	*q-- = '\0';

	while(*q-- = *p++);

	return q+2;
}

char *multi(s)
char *s;
{

	int i, x;
	char *p=s;
	static char w[9];

	while(*p++); x = p-s-1;

	if(x > 4) return "password";

	*w = '\0';

	for(i=0;i<2*x*x-(x*x*x+47*x)/6+12;i++)

	   strcat(w,s);

	return w;
}

