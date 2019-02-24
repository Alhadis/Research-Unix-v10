/* with luck, protocol can be used in either direction */
/* for now, all ascii, for readability */
/*
	x		character x
	nnn		integer nnn, followed by blank
			NEW: last digit is now a letter, no blank
	snn abcdef	string abcdef, length nn, term. by \n
			NEW: use new integer encoding

  Changing the integer encoding reduced the time for data transfer in
	/u/j/anim/sort/a1.i from 58 seconds to 42 seconds
*/

char	protobuf[1024];
char	*protop	= protobuf;

flushproto()
{
	if (protop > protobuf) {
		sendbuf(protobuf, protop);
		protop = protobuf;
	}
}

send1char(c)	/* send one character */
	int c;
{
	if (protop >= protobuf + sizeof protobuf)
		flushproto();
	*protop++ = c;
}

readchar()	/* read one character */
{
	int c;

	flushproto();
	return readbyte();
}

sendint(n)	/* send an integer */
	int n;
{
	char buf[20], *p;

	sprintf(buf, "%d", n);
	for (p = buf; *(p+1); p++)
		send1char(*p);
	send1char(*p - '0' + 'a');
}

readint()	/* read an integer */
{
	int n, c, sign;

	flushproto();
	while ((c = readbyte()) == ' ' || c == '\n')
		;
	assert(c == '-' || isdigit(c) || (c >= 'a' && c <= 'j'),
		"c is not a digit");
	if (c == '-') {
		sign = -1;
		c = readbyte();
	} else
		sign = 1;
	for (n = 0; c >= '0' && c <= '9'; c = readbyte())
		n = 10 * n + c - '0';
	assert(c >= 'a' && c <= 'j', "bad string terminator");
	n = 10 * n + c - 'a';
	return sign * n;
}

sendstring(s)	/* send a string */
	char *s;
{
	send1char('s');
	sendint(strlen(s));
	while (*s)
		send1char(*s++);
	send1char('\n');
}

readstring(s)	/* read string into s (better be big enough) */
	char *s;
{
	int c, n, m;
	char *p = s;

	flushproto();
	c = readbyte();
	assert(c == 's', "readstring: c == 's'");
	m = n = readint();
	while (n--)
		*p++ = readbyte();
	*p = '\0';
	c = readbyte();
	assert(c == '\n', "c == '\\n'");
	return m;
}

isdigit(c)
{
	return c >= '0' && c <= '9';
}
