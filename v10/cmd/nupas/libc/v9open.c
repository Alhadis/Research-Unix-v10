/*
 *  open a file for appending.  if the file doesn't exist, create it.
 *  return the file descriptor.
 */
appendopen(file)
	char *file;
{
}

/*
 *  lock and open file.
 *  return the file descriptor.
 */
lockopen(file, mode)
	char *file;
{
}

/*
 *  close and unlock a file.
 */
lockclose(fd)
	int fd;
{
}

/*
 *  remove ALL locks
 */
cleanlocks()
{
}
