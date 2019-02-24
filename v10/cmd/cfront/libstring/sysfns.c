#include "String.h"

int
stat(const String& s1, struct stat* x)
{
	const String	x1 = s1 + (char)0;
	return stat(x1.d->start, x);
}

int
system(const String& s1)
{
	const String	x1 = s1 + (char)0;
	return system(x1.d->start);
}

int
access(const String& s1, int i1)
{
	const String	x1 = s1 + (char)0;
	return access(x1.d->start, i1);
}

int
acct(const String& s1)
{
	const String	x1 = s1 + (char)0;
	return acct(x1.d->start);
}

int
chdir(const String& s1)
{
	const String	x1 = s1 + (char)0;
	return chdir(x1.d->start);
}

int
chmod(const String& s1, int i1)
{
	const String	x1 = s1 + (char)0;
	return chmod(x1.d->start, i1);
}

int
chown(const String& s1, int i1, int i2)
{
	const String	x1 = s1 + (char)0;
	return chown(x1.d->start, i1, i2);
}

int
creat(const String& s1, int i1)
{
	const String	x1 = s1 + (char)0;
	return creat(x1.d->start, i1);
}

int
link(const String& s1, const String& s2)
{
	const String	x1 = s1 + (char)0;
	const String	x2 = s2 + (char)0;
	return link(x1.d->start, x2.d->start);
}

int
mknod(const String& s1, int i1, int i2)
{
	const String	x1 = s1 + (char)0;
	return mknod(x1.d->start, i1, i2);
}

int
open(const String& s1, int i1)
{
	const String	x1 = s1 + (char)0;
	return open(x1.d->start, i1);
}

int
unlink(const String& s1)
{
	const String	x1 = s1 + (char)0;
	return unlink(x1.d->start);
}

int
read(int fildes, String& buffer, int nbytes)
{
	int	ans;
	switch (nbytes) {
	case 0:
		if (read(fildes, "", 0))
			return -1;
		else buffer = "";
		return 0;
	case 1:	{
			char	c;
			switch (ans = read(fildes, &c, 1)) {
			case 0:
				buffer = "";
				break;
			case 1:
				buffer = c;
				break;
			}
			return ans;
		}
	}
	Rep*	temp = new Rep(nbytes);
	if ((ans = read(fildes, temp->start, nbytes)) < nbytes)
		switch (ans) {
		case 0:
			buffer = "";
		case -1:
			delete temp;
			break;
		case 1:
			buffer = *temp->start;
			delete temp;
			break;
		default:
			{
				temp->extend(ans - nbytes);
				buffer = String(*temp);
				temp->refDecr();
			}
			break;
		}
	return ans;
}

int
write(int fildes, const String& s1)
{
	return write(fildes, s1.d->start, s1.length());
}


FILE*
fopen(const String& s1, const String& s2)
{
	const String	x1 = s1 + (char)0;
	const String	x2 = s2 + (char)0;
	return fopen(x1.d->start, x2.d->start);
}

FILE*
fdopen(int i, const String& s1)
{
	const String	x1 = s1 + (char)0;
	return fdopen(i, x1.d->start);
}

FILE*
freopen(const String& s1, const String& s2, FILE* f)
{
	const String	x1 = s1 + (char)0;
	const String	x2 = s2 + (char)0;
	return freopen(x1.d->start, x2.d->start, f);
}

FILE*
popen(const String& s1, const String& s2)
{
	const String	x1 = s1 + (char)0;
	const String	x2 = s2 + (char)0;
	return popen(x1.d->start, x2.d->start);
}

int
fgets(String& s1, int n, FILE* iop)
{
	register c;
	s1 = "";
	while (--n>0 && (c = getc(iop))>=0) {
		s1 += c;
		if (c=='\n')
			break;
	}
	if (c<0 && s1.length()==0)
		return 0;
	return 1;
} 

int
gets(String& s1)
{
	register c;

	s1 = "";
	while ((c = getchar()) != '\n' && c >= 0)
		s1 += c;
	if (c<0 && s1.length()==0)
		return 0;
	return 1;
}

int
puts(const String& s1)
{
	register char*	s = s1.d->start;
	register char*	e = s + s1.d->len;
	register c;

	while (s < e) {
		c = *s++;
		putchar(c);
	}
	return(putchar('\n'));
}

int
fputs(const String& s1, FILE* iop)
{
	register char*	s = s1.d->start;
	register char*	e = s + s1.d->len;
	register r;
	register c;

	while (s < e) {
		c = *s++;
		r = putc(c, iop);
	}
	return(r);
}

