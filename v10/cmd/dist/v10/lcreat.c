int
lcreat(char *name, int mode)
{
	char temp[14];
	int fd;

	sprintf(temp, "lcreat.%d", getpid());	/* only works in same file system */
	fd = creat(temp, mode);
	if (fd < 0)
		return fd;
	if (link(temp, name) < 0) {
		close(fd);
		unlink(temp);
		return -1;
	}
	unlink(temp);
	return fd;
}
