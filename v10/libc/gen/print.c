#define	SIZE	1024
extern int	printcol;

char	*doprint();

print(fmt, args)
char *fmt;
{
	char buf[SIZE], *out;

	out = doprint(buf, fmt, (char *)&args);
	return write(1, buf, (int)(out-buf));
}

fprint(f, fmt, args)
char *fmt;
{
	char buf[SIZE], *out;

	out = doprint(buf, fmt, (char *)&args);
	return write(f, buf, (int)(out-buf));
}

sprint(buf, fmt, args)
char *buf;
char *fmt;
{
	char *out;
	int scol;

	scol = printcol;
	out = doprint(buf, fmt, (char *)&args);
	printcol = scol;
	return out-buf;
}
