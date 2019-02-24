
	static char	SCCSID[] = "@(#)push.c	2.1 DKHOST 85/01/15";

#include	<errno.h>
#include	<varargs.h>
#include	"string.h"
#include	"pupu.h"
#include	"ndir.h"

	Efn	confirmed, remoteerror, alldone;

	Sexp	justconf[] = {
		'c',	confirmed,
		'e',	remoteerror,
		'X',	alldone,
		0,	0
	};

	Eexp	justquit[];

	EcharP	alreadydumped();

push(fd, names, directory, errfcn, options)
	PfnP	errfcn;
	PcharP	*names, directory, options;
{
	begin(fd, errfcn, "local", "");

	if(setjmp(giveup))
		return(lasterror);

	sendinit(options);

	msginit('D');
	msgfmt("%s", directory);
	msgconf(justconf);

	while(*names)
		sendfile(*names++);

	msginit('X');
	msgsend();
	msgwait(justquit);

	signal(SIGALRM, alarmwas);
	return(lasterror);
}

putfile(longname, shortname)
	RcharP	longname, shortname;
{
	Rint	ifd;
	AcharP	lp;
	Astat	statbuf;

	if((ifd = openn(shortname, 0)) < 0){
		warn("Can't open", longname, EX_NOINPUT);
		return;
	}

	fstat(ifd, &statbuf);

	if(statbuf.st_nlink > 1
	  && (statbuf.st_mode & S_IFMT) != S_IFDIR
	    && (lp = alreadydumped(longname, &statbuf)))
		dumplink(longname, lp);
	else
		switch(statbuf.st_mode & S_IFMT){
		case S_IFREG:
			dumpfile('r', ifd, &statbuf, longname);
			break;

		case S_IFDIR:
			dumpdir(ifd, longname, shortname);
			dumpfile('d', -1, &statbuf, longname);
			break;

		case S_IFCHR:
			dumpfile('c', -1, &statbuf, longname);
			break;
		
		case S_IFBLK:
			dumpfile('b', -1, &statbuf, longname);
			break;

		case S_IFIFO:
			dumpfile('p', -1, &statbuf, longname);
		}

	closen(ifd);
}

dumpdir(ifd, lname, sname)
	PcharP	lname, sname;
{
	DIR		*df;
	struct direct	*de;
	Along		mark;
	Achar		member[NAMELEN];

	if(chdir(sname) < 0){
		warn("Can't chdir to", lname, EX_NOINPUT);
		return;
	}

	df = fdopendir(ifd);

	while(de = readdir(df)){

		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;

		if(df->dd_fd > TOPFD){
			mark = telldir(df);
			openfiles[df->dd_fd] = 0;
			closedir(df);
		}else
			mark = -1;

		sprintf(member, "%s/%s", lname, de->d_name);

		putfile(member, strrchr(member, '/')+1);

		if(mark >= 0){
			df = opendir(".");
			openfiles[df->dd_fd] = 1;
			seekdir(df, mark);
		}
	}

	openfiles[df->dd_fd] = 0;
	closedir(df);

	chdir("..");
}

dumpfile(type, ifd, st, name)
	Pchar	type;
	RstatP	st;
	PcharP	name;
{
	Rint	len;

	msginit('F');

	msgfmt("%c", type);
	msgfmt("%s", name);
	msgfmt("%o", st->st_mode & 07777);
	msgfmt("%d", st->st_uid);
	msgfmt("%d", st->st_gid);
	msgfmt("%ld", st->st_mtime);

	if(type == 'r')
		msgfmt("%ld", st->st_size);
	else if(type == 'c' || type == 'b'){
		msgfmt("%d", major(st->st_rdev));
		msgfmt("%d", minor(st->st_rdev));
	}

	msgsend();

	if(type == 'r'){
		currentfile = name;
		timeoutmsg = "File transmission timed out";

		while((len = read(ifd, buf, sizeof(buf))) > 0){
			alarm(WTIMEOUT);

			if(write(cfd, buf, len) != len)
				err("Channel write error", NIL, EX_IOERR);
		}

		if(len < 0)
			err("Error reading", name, EX_IOERR);

		dkeof(cfd);

		alarm(0);
	}

	msgwait(justconf);
}

	PcharP
alreadydumped(name, st)
	PcharP	name;
	RstatP	st;
{
	RlinkP	plp, clp;
	Schar	linkname[NAMELEN];

	for(plp = &linkhead; clp = plp->next; plp = clp)
		if(clp->ino == st->st_ino && clp->dev == st->st_dev){
			strcpy(linkname, clp->name);

			if(--clp->count <= 0){
				plp->next = clp->next;
				free(clp);
			}

			return(linkname);
		}

	clp = (TlinkP) malloc(sizeof(*clp) + strlen(name));

	if(clp){
		strcpy(clp->name, name);

		clp->count = st->st_nlink - 1;
		clp->ino = st->st_ino;
		clp->dev = st->st_dev;

		clp->next = linkhead.next;
		linkhead.next = clp;
	}

	return(NIL);
}

dumplink(new, prev)
	PcharP	new, prev;
{
	msginit('L');

	msgfmt("%s", new);
	msgfmt("%s", prev);

	msgconf(justconf);
}
