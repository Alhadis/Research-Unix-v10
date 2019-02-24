main(argc, argv)
char **argv;
{
	chroot("/usr/v7");
	chdir(argc>1? argv[1]: "/");
	umask(02);
	setgid(3);
	setuid(3);
	execl("/bin/sh", "-", 0);
}
