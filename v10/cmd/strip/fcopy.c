#define BSIZE	4096
#define min(a,b)	((a)<(b) ? (a) : (b))

fcopy(to, from, nbytes)
int to, from; register int nbytes;
{
	char *malloc();
	register char *buf; register int n;

	if (nbytes && (buf = malloc(min(nbytes, BSIZE)))) {
		do {
			n = min(nbytes, BSIZE);
			if (read(from, buf, n) != n) break;
			if (write(to, buf, n) != n) break;
			nbytes -= n;
		} while (nbytes);
		free(buf);
	}
	return nbytes;
}
