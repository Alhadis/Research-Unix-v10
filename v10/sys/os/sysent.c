/*
 * This table is the switch used to transfer
 * to the appropriate routine for processing a system call.
 * Each row contains the number of arguments expected
 * and a pointer to the routine.
 */

#include "sys/param.h"
#include "sys/systm.h"

int	alarm();
int	biasclock();
int	chdir();
int	chmod();
int	chown();
int	chroot();
int	close();
int	creat();
int	dirread();
int	dup();
int	exece();
int	fchmod();
int	fchown();
int	fmount();
int	fork();
int	fstat();
int	ftime();
int	funmount();
int	getgid();
int	getgroups();
int	getlogname();
int	getpid();
int	getuid();
int	gtime();
int	ioctl();
int	kill();
int	link();
int	lstat();
int	lseek();
int	mkdir();
int	mknod();
int	nap();
int	nice();
int	nosys();
int	nullsys();
int	open();
int	pause();
int	pipe();
int	profil();
int	read();
int	readlink();
int	rexit();
int	rmdir();
int	saccess();
int	sbreak();
int	seek();
int	select();
int	setgid();
int	setgroups();
int	setpgrp();
int	setruid();
int	setuid();
int	ssig();
int	stat();
int	stime();
int	symlink();
int	sync();
int	sysacct();
int	sysboot();
int	syslock();
int	times();
int	umask();
int	unlink();
int	utime();
int	vadvise();		/* later, segadvise */
int	vlimit();
int	vswapon();
int	vtimes();
int	wait();
int	write();

struct sysent sysent[MAXSYS] =
{
	0, nosys,			/*  0 = indir */
	1, rexit,			/*  1 = exit */
	0, fork,			/*  2 = fork */
	3, read,			/*  3 = read */
	3, write,			/*  4 = write */
	2, open,			/*  5 = open */
	1, close,			/*  6 = close */
	0, wait,			/*  7 = wait */
	2, creat,			/*  8 = creat */
	2, link,			/*  9 = link */
	1, unlink,			/* 10 = unlink */
	4, lseek,			/* 11 = lseek (64-bit) */
	1, chdir,			/* 12 = chdir */
	0, gtime,			/* 13 = time */
	3, mknod,			/* 14 = mknod */
	2, chmod,			/* 15 = chmod */
	3, chown,			/* 16 = chown */
	1, sbreak,			/* 17 = break */
	2, stat,			/* 18 = stat */
	3, seek,			/* 19 = seek */
	0, getpid,			/* 20 = getpid */
	0, nosys,			/* 21 = was mount */
	3, dirread,			/* 22 = dirread */
	1, setuid,			/* 23 = setuid */
	0, getuid,			/* 24 = getuid */
	1, stime,			/* 25 = stime */
	4, fmount,			/* 26 = mount filesystem */
	1, alarm,			/* 27 = alarm */
	2, fstat,			/* 28 = fstat */
	0, pause,			/* 29 = pause */
	2, utime,			/* 30 = utime */
	2, fchmod,			/* 31 = fchmod */
	3, fchown,			/* 32 = fchown */
	2, saccess,			/* 33 = access */
	1, nice,			/* 34 = nice */
	1, ftime,			/* 35 = ftime */
	0, sync,			/* 36 = sync */
	2, kill,			/* 37 = kill */
	4, select,			/* 38 = select */
	2, setpgrp,			/* 39 = setpgrp */
	2, lstat,			/* 40 = lstat */
	2, dup,				/* 41 = dup */
	0, pipe,			/* 42 = pipe */
	1, times,			/* 43 = times */
	4, profil,			/* 44 = prof */
	0, nosys,			/* 45 = formerly xmount */
	1, setgid,			/* 46 = setgid */
	0, getgid,			/* 47 = getgid */
	2, ssig,			/* 48 = sig */
	0, nosys,			/* 49 = was gmount */
	1, funmount,			/* 50 = unmount filesystem */
	1, sysacct,			/* 51 = turn acct off/on */
	1, biasclock,			/* 52 = bias clock */
	1, syslock,			/* 53 = lock user in core */
	3, ioctl,			/* 54 = ioctl */
	1, sysboot,			/* 55 = reboot */
	1, setruid,			/* 56 = setruid */
	2, symlink,			/* 57 = create symbolic link */
	3, readlink,			/* 58 = read symbolic link */
	3, exece,			/* 59 = exece */
	1, umask,			/* 60 = umask */
	1, chroot,			/* 61 = chroot */
	0, nosys,			/* 62 = reserved to local sites */
	0, nosys,			/* 63 = used internally */
	1, rmdir,			/* 64 +0 = rmdir */
	2, mkdir,			/* 64 +1 = mkdir */
	0, fork,			/* 64 +2 = former vfork */
	2, getlogname,			/* 64 +3 = getlogname */
	0, nosys,			/* 64 +4 = was vwrite */
	0, nosys,			/* 64 +5 = was segalloc */
	0, nosys,			/* 64 +6 = was segfree */
	0, nosys,			/* 64 +7 = was segsync */
	1, vadvise,			/* 64 +8 = vadvise */
	0, nosys,			/* 64 +9 = was splice */
	2, setgroups,			/* 64+10 = setgroups */
	2, getgroups,			/* 64+11 = getgroups */
	0, nosys,			/* 64+12 = was vhangup */
	2, vlimit,			/* 64+13 = vlimit */
	0, nosys,			/* 64+14 = nosys */
	0, nosys,			/* 64+15 = nosys */
	0, nosys,			/* 64+16 = nosys */
	0, nosys,			/* 64+17 = nosys */
	0, nosys,			/* 64+18 = nosys */
	0, nosys,			/* 64+19 = nosys */
	0, nosys,			/* 64+20 = nosys */
	1, vswapon,			/* 64+21 = vswapon */
	0, nosys,			/* 64+22 = nosys */
	0, nosys,			/* 64+23 = nosys */
	0, nosys,			/* 64+24 = nosys */
	0, nosys,			/* 64+25 = nosys */
	0, nosys,			/* 64+26 = nosys */
	0, nosys,			/* 64+27 = nosys */
	0, nosys,			/* 64+28 = nosys */
	0, nosys,			/* 64+29 = nosys */
	0, nosys,			/* 64+30 = nosys */
	0, nosys,			/* 64+31 = nosys */
	0, nosys,			/* 64+32 = nosys */
	0, nosys,			/* 64+33 = nosys */
	0, nosys,			/* 64+34 = nosys */
	0, nosys,			/* 64+35 = nosys */
	0, nosys,			/* 64+36 = nosys */
	0, nosys,			/* 64+37 = nosys */
	0, nosys,			/* 64+38 = nosys */
	0, nosys,			/* 64+39 = nosys */
	0, nosys,			/* 64+40 = nosys */
	1, nap,				/* 64+41 = nap */
	0, nosys,			/* 64+42 = was settod */
	2, vtimes,			/* 64+43 = vtimes */
	0, nosys,			/* 64+44 = nosys */
	0, nosys,			/* 64+45 = nosys */
	0, nosys,			/* 64+46 = nosys */
	0, nosys,			/* 64+47 = nosys */
	0, nosys,			/* 64+48 = nosys */
	0, nosys,			/* 64+49 = nosys */
	0, nosys,			/* 64+50 = nosys */
	0, nosys,			/* 64+51 = nosys */
	0, nosys,			/* 64+52 = nosys */
	0, nosys,			/* 64+53 = nosys */
	0, nosys,			/* 64+54 = nosys */
	0, nosys,			/* 64+55 = nosys */
	0, nosys,			/* 64+56 = nosys */
	0, nosys,			/* 64+57 = nosys */
	0, nosys,			/* 64+58 = nosys */
	0, nosys,			/* 64+59 = nosys */
	0, nosys,			/* 64+60 = nosys */
	0, nosys,			/* 64+61 = nosys */
	0, nosys,			/* 64+62 = nosys */
	0, nosys,			/* 64+63 = was limits */
};
