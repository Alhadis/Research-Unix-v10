#include "Regexp.h"

Regexp::Regexp(const char *p)
{
	prog = regcomp(p);
	for (int i=0; i<NSUBEXP; i++) offset[i] = len[i] = -1;
}

Regexp::Regexp(const String& s1)
{
	const String	x1 = s1 + (char)0;
	prog = regcomp(x1.d->start);
	for (int i=0; i<NSUBEXP; i++) offset[i] = len[i] = -1;
}

int
Regexp::exec(const String& s1)
{
	subject = s1;
	const String	x1 = s1 + (char)0;
	register char*	p = x1.d->start;
	if (!prog) return 0;
	int	ans = regexec(prog, p);
	for (int i=0; i<NSUBEXP; i++)
		if (prog->startp[i]) {
			offset[i] = prog->startp[i] - p;
			len[i] = prog->endp[i] - prog->startp[i];
		} else offset[i] = len[i] = -1;
	return ans;
}

int
Regexp::result(unsigned& start, unsigned& length, unsigned i)
{
	if (i>=NSUBEXP || offset[i] == -1) return 0;
	start = offset[i];
	length = len[i];
	return 1;
}

extern void free(char *);	// since prog is allocated by malloc()

Regexp::~Regexp()
{
	free((char *)prog);
}
