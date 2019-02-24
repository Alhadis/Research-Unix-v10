/*
 * endstr --- return a pointer to the end of a string
 */

char *endstr(s)
register char	*s;
{

	while (*s++)
		;
	return (--s);
}


/*
 * move --- copy "length" bytes from "from" to "to"
 */

move(length, from, to)
register int	length;
register char	*from, *to;
{

	while (--length >= 0)
		*to++ = *from++;
}


/*
 * smartmove --- move "length" bytes from "from" to "to"; handle overlap
 */

smartmove(length, from, to)
register int	length;
register char	*from, *to;
{

	if (from < to && from + length > to) {
		from += length;
		to += length;
		while (--length >= 0)
			*--to = *--from;
		}
	else
		while (--length >= 0)
			*to++ = *from++;
}


#define	READ	0
#define	WRITE	1

int _outpid;		/* the child's process id */

/*
 * pipeout --- open an output pipe to the specified program
 */

pipeout(pgm, args)
char *pgm;
char *args;
{
	register int i;
	int pipedes[2];
	
	pipe(pipedes);
	if ((i = fork()) == 0) {
		close(READ);
		dup(pipedes[READ]);
		closeall();
		execv(pgm, &args);
		write(2, "no ", 3);
		write(2, pgm, strlen(pgm));
		write(2, "\n", 1);
		exit(1);
		}
	_outpid = i;
	close(pipedes[READ]);
	return(pipedes[WRITE]);
}


/*
 * clear --- write zeros to the specified block of memory
 */

clear(bp, len)
register char	*bp;
register int	len;
{
	register long	*lp;

	while (((unsigned)bp & (sizeof(long)-1)) && --len >= 0)
		*bp++ = 0;
	lp = (long *)bp;
	while (len >= sizeof(long)) {
		*lp++ = 0;
		len -= sizeof(long);
		}
	bp = (char *)lp;
	while (--len >= 0)
		*bp++ = 0;
}


/*
 * closeall --- close all open files (except stdin, stdout, stderr)
 */

closeall()
{
	extern errno;
	register int i;
	
	for (i=3; i<15; ++i)
		close(i);
	errno = 0;
}
