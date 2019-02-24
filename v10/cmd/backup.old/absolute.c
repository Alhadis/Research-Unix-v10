#include	<fio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<libc.h>

#define 	IEQ(a,b)	((a.st_dev == b.st_dev) && (a.st_ino == b.st_ino))

main()
{
	int nerrs = 0;
	char dot[4096], pwd[4096];
	char *home, *s, c;
	struct stat hbuf, pbuf;

	/* figure out where we are */
	dot[0] = 0;
	if(getwd(pwd)){
		if(strncmp("/usr/", pwd, 5) == 0){
			strcpy(dot, pwd);
			goto doit;
		}
		home = getenv("HOME");
		if(home == 0){
			strcpy(dot, pwd);
			goto doit;
		}
		if(stat(home, &hbuf) < 0){
			strcpy(dot, pwd);
			goto doit;
		}
		/* can we convert safely to /usr */
		strcpy(dot, pwd);
		for(s = pwd+1; *s;){
			c = '/';
			while(*s != '/')
				if(*s++ == 0){
					s--;
					c = 0;
					break;
				}
			if(c)
				*s = 0;
			if(stat(pwd, &pbuf) < 0)
				goto doit;
			if(IEQ(pbuf, hbuf)){	/* bonzer ! */
				if(c)
					sprint(dot, "%s/%s", home, s+1);
				else
					strcpy(dot, home);
				goto doit;
			}
			if(c)
				*s++ = c;
		}
	}
doit:
	while(s = Frdline(0)){
		if(*s == '/'){
			Fwrite(1, s, (long)FIOLINELEN(0));
			Fputc(1, '\n');
			continue;
		}
		if(dot[0] == 0){
			nerrs++;
			fprint(2, "absolute: %s: don't know where '.' is: pwd\n", s, pwd);
			continue;
		}
		Fprint(1, "%s/%s\n", dot, s);
	}
	exit(nerrs != 0);
}
