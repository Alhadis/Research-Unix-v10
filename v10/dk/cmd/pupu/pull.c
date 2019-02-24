
	static char	SCCSID[] = "@(#)pull.c	2.1 DKHOST 85/01/15";

#include	<errno.h>
#include	"string.h"
#include	<varargs.h>
#include	"pupu.h"
#include	"ndir.h"

	Efn	takefile, fetchrequest, setdirectory, terminate;
	Efn	takelink, confirmed, remoteerror, alldone;

	Sexp	pulltable[] = {
		'F',	takefile,
		'L',	takelink,
		'S',	fetchrequest,
		'D',	setdirectory,
		'X',	terminate,
		0,	0
	};

	Sexp	fetchtable[] = {
		'F',	takefile,
		'L',	takelink,
		'c',	confirmed,
		'e',	remoteerror,
		'X',	alldone,
		0,	0
	};

	Eexp	justquit[];

	Sint	dochown;

	EcharP	below();
	EpwP	getpwnam();

pull(fd, names, directory, errfcn, options)
	PfnP	errfcn;
	PcharP	*names, directory, options;
{
	begin(fd, errfcn, "local", ".");

	dochown = (geteuid() == 0);

	if(setjmp(giveup))
		return(lasterror);

	sendinit(options);

	setbase(directory);

	while(*names)
		fetchfile(*names++);

	msginit('X');
	msgsend();
	msgwait(justquit);

	signal(SIGALRM, alarmwas);
	return(lasterror);
}

fetchfile(name)
	PcharP	name;
{
	msginit('S');

	msgfmt("%s", name);

	msgconf(fetchtable);
}

fetchrequest()
{
	Achar	name[NAMELEN];

	msgtake("%s", name);

	sendfile(name);

	reply('c');

	return(0);
}

ppremote(fd)
{
	begin(fd, (TfnP) 0, "remote", ".");

	dochown = (geteuid() == 0);

	if(setjmp(giveup))
		return(lasterror);

	msgwait(inittable);

	msgwait(pulltable);

	signal(SIGALRM, alarmwas);
	return(lasterror);
}

setdirectory()
{
	setbase(&buf[H_DATA]);

	reply('c');

	return(0);
}

setbase(p)
	RcharP	p;
{
	RcharP	q;
	RpwP	pw;

	if(*p == '/')
		strcpy(basedir, p);
	else if(*p == '~'){
		if(q = strchr(++p, '/'))
			*q++ = '\0';

		if(*p){
			if(pw = getpwnam(p))
				strcpy(basedir, pw->pw_dir);
			else
				err("No passwd entry for", p, -EX_NOUSER);
		}else{
			curdir(basedir);
			strcpy(wdir, basedir);
		}

		if(q){
			strcat(basedir, "/");
			strcat(basedir, q);
		}
	}else{
		curdir(basedir);
		strcpy(wdir, basedir);

		if(strcmp(p, ".") != 0){
			strcat(basedir, "/");
			strcat(basedir, p);
		}
	}

db(stderr, "setdirectory %s\n", basedir);
	changedir(basedir);

	return(0);
}

changedir(newdir)
	PcharP	newdir;
{
	RcharP	p;

	if(!(p = below(wdir, newdir)))
		p = newdir;

	if(*p){
		if(chdir(p) < 0 && (makedirectory(p), chdir(p) < 0))
			err("Can't chdir to", p, EX_NOINPUT);

		strcpy(wdir, newdir);
	}
db(stderr, "changedir newdir %s p %s wdir %s\n", newdir, p, wdir);
}

	PcharP
below(ref, check)
	RcharP	ref, check;
{
	Rint	len;

	len = strlen(ref);

	if(strncmp(ref, check, len) != 0)
		return(NIL);

	check += len;

	if(*check == '/')
		return(check+1);
	if(*check)
		return(NIL);
	return(check);
}

takefile()
{
	RcharP	p;
	Rint	haderr;
	Achar	type;
	Achar	name[NAMELEN];
	Aint	mode, uid, gid;
	Along	mtime;
	time_t	timea[2];

	msgtake("%c", &type);
	msgtake("%s", name);
	msgtake("%o", &mode);
	msgtake("%d", &uid);
	msgtake("%d", &gid);
	msgtake("%ld", &mtime);

	sprintf(wholename, "%s/%s", basedir, name);

	p = strrchr(wholename, '/');
	*p = '\0';

	changedir(wholename);

	*p++ = '/';

	switch(type){
	case 'r':
		haderr = pluckregular(p);
		break;

	case 'd':
		makedirectory(p);
		haderr = 0;
		break;

	case 'c':
		haderr = pluckspecial(p, S_IFCHR);
		break;

	case 'b':
		haderr = pluckspecial(p, S_IFBLK);
		break;

	case 'p':
		haderr = pluckspecial(p, S_IFIFO);
	}

	reply('c');

	if(haderr)
		return(0);

	chmod(p, mode);

	timea[0] = time((TlongP) 0);
	timea[1] = mtime;

	if(timea[1] >= timea[0])
		timea[1] = timea[0] - 1;

	utime(p, timea);

	if(dochown)
		chown(p, uid, gid);

	return(0);
}

pluckregular(p)
	PcharP	p;
{
	Rint	ffd, len;

	if((ffd = creatn(p, 0200)) < 0
	  && (chmod(p, 0200), (ffd = creatn(p, 0200)) < 0)){

		warn("Can't create", wholename, EX_CANTCREAT);

		while(read(cfd, buf, sizeof(buf)) > 0);

		return(BAD);
	}

	currentfile = wholename;
	timeoutmsg = "File reception timed out";

	alarm(RTIMEOUT);

	while((len = read(cfd, buf, sizeof(buf))) > 0){
db(stderr, "len %d\n", len);
		if(write(ffd, buf, len) != len){
			warn("Write failed on", wholename, EX_IOERR);
			while((len = read(cfd, buf, sizeof(buf))) > 0);
		}
		alarm(RTIMEOUT);
	}

	alarm(0);

	if(len < 0)
		stopnow(EX_IOERR);

	closen(ffd);

	return(GOOD);
}

pluckspecial(name, mode)
	PcharP	name;
{
	Rint	dev;
	Aint	maj, min;

	unlink(name);

	if(mode == S_IFIFO)
		dev = 0;
	else{
		msgtake("%d", &maj);
		msgtake("%d", &min);

		dev = makedev(maj, min);
	}

	if(mknod(name, mode, dev) < 0){
		warn("Can't mknod", wholename, EX_CANTCREAT);
		return(BAD);
	}

	return(GOOD);
}

takelink()
{
	RcharP	p;
	Achar	name[NAMELEN], prev[NAMELEN];

	msgtake("%s", name);
	msgtake("%s", prev);

	sprintf(wholename, "%s/%s", basedir, name);

	p = strrchr(wholename, '/');
	*p = '\0';
	changedir(wholename);
	*p++ = '/';

	sprintf(buf, "%s/%s", basedir, prev);

	if((link(buf, p) < 0) && (unlink(p), (link(buf, p) < 0)))
		warn("Can't link to", wholename, EX_CANTCREAT);

	reply('c');

	return(0);
}

makedirectory(p)
	PcharP	p;
{
	RcharP	q;
	Aint	pid, n, result;
	Achar	temp[NAMELEN];

db(stderr, "makedirectory %s\n", p);
	sprintf(temp, "%s/", p);

	q = temp;
	if(*q == '/')
		q++;

	while(q = strchr(q, '/')){
		*q = '\0';

		if(access(temp, 1) < 0){
db(stderr, "mkdir %s\n", temp);
			if((pid = fork()) == 0){
				freopen("/dev/null", "w", stderr);
				execl("/bin/mkdir", "mkdir", temp, 0);
				err("No mkdir", NIL, EX_UNAVAILABLE);
				/* NOTREACHED */
			}
			while((n = wait(&result)) > 0 && n != pid);

			if(result != 0)
				warn("Can't make directory", temp, -EX_CANTCREAT);
		}

		*q++ = '/';
	}
}

terminate()
{
	msginit('x');
	msgfmt("%d", lasterror);
	msgsend();

	return(1);
}
