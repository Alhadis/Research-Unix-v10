char	xstr[];
#line 1 "printf.c"
static	char *sccsid = (&xstr[4589]);





printf(fmt, args)
char *fmt;
{
	_doprnt(fmt, &args, 0);
}

_strout(count, string, adjust, foo, fillch)
register char *string;
register int count;
int adjust;
register struct { int a[6]; } *foo;
{

	if (foo != 0)
		abort();
	while (adjust < 0) {
		if (*string=='-' && fillch=='0') {
			putchar(*string++);
			count--;
		}
		putchar(fillch);
		adjust++;
	}
	while (--count>=0)
		putchar(*string++);
	while (adjust) {
		putchar(fillch);
		adjust--;
	}
}
