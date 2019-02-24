# include	"trek.h"
#include	"stat.h"

/**
 **	dump game for later restart
 **/

#define	CHECK	9


inode(f)
{	STATBUF		buf;

	fstat(f,&buf);
	return(buf.i_ino);
}

long Ftime(f)
{	STATBUF		buf;

	fstat(f,&buf);
	return(buf.i_mtime);
}

dumpgame()
{
	register int	f, len;
	int		check;
	long		t;
	int		n;

	if((f=creat("trek.dump",0664))<0) {
		printf("Cannot create 'trek.dump'\n");
		return;
	}

	check=CHECK; write(f,&check,2);
	len=sbrk(0); lseek(f,8L,0);
	if(write(f,0,len)!=len) {
		printf("Failed to write dump\n");
		return;
	}
	lseek(f,2L,0);
	n=inode(f); write(f,&n,2);
	time(&t); write(f,&t,4); close(f);
	exit(0);
}

restart()
{
	register int	f, len;
	int		check;
	char		chkpass[PWDLEN];
	long		t, tfile;
	int		n, nfile;

	if((f=open("trek.dump",0))<0) {
		printf("Cannot open 'trek.dump'\n");
		return(0);
	}

	read(f,&check,2);
	read(f,&n,2); read(f,&t,4);
	nfile=inode(f); tfile=Ftime(f);
	len=sbrk(0);
	if(check!=CHECK || read(f,0,len)!=len) {
		printf("Cannot restart\n");
		exit(1);
	}
	getpasswd(chkpass);
	if (cf(chkpass, Game.passwd)) {
		printf("Incorrect password\n");
		exit(1);
	}
	unlink("trek.dump");
	if(nfile!=n || tfile>t+2)
		lose(L_CHEAT);
	return(1);
}
