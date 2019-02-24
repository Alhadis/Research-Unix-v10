main(argc, argv)
char **argv;
{
	if (chroot("/v8") < 0) {
		printf("v8: can't chroot\n");
		exit(1);
	}
	chdir(argc>1? argv[1]: "/");
	umask(02);
	setgid(4);
	setuid(3);
	execl("/bin/sh", "-", 0);
}
