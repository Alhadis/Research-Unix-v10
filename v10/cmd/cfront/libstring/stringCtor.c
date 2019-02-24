#include "String.h"

Charfield	*currfield;
Rep	*nullRep;
Rep	*oneChar;

void
startUp()
{
	static	char	abc[256];
	currfield = new Charfield;
	currfield->next = currfield;
	String::startedUp = TRUE;
	nullRep = new Rep;
	nullRep->start = "";
	nullRep->len = 0;
	nullRep->immutable();
	nullRep->now_constant();
	oneChar = new Rep[256];
	for ( int i = 256; i--; ) {
		abc[i] = i;
		oneChar[i].start = &abc[i];
		oneChar[i].len = 1;
		oneChar[i].immutable();
		oneChar[i].now_constant();
	}
}

String::String()
{
	if (!startedUp)
		startUp();
	d = nullRep;
}

String::String(char c)
{
	if (!startedUp)
		startUp();
	d = &oneChar[(unsigned char) c];
}

String::String(char c, char cc)
{
	if (!startedUp)
		startUp();
	d = new Rep(2);
	register char *p = d->start;
	*p++ = c;
	*p = cc;
}

String::String(char c, char cc, char ccc)
{
	if (!startedUp)
		startUp();
	d = new Rep(3);
	register char *p = d->start;
	*p++ = c;
	*p++ = cc;
	*p = ccc;
}

String::String(char c, char cc, char ccc, char cccc)
{
	if (!startedUp)
		startUp();
	d = new Rep(4);
	register char *p = d->start;
	*p++ = c;
	*p++ = cc;
	*p++ = ccc;
	*p = cccc;
}

String::String(const char *s)
{
	register	len = strlen(s);
	if (!startedUp)
		startUp();
	switch (len) {
	case 0:
		d = nullRep;
		break;
	case 1:
		d = &oneChar[(unsigned char) *s];
		break;
	default:
		if (len > MAXSTRINGLENGTH) {
			error(1, "String::String(char *): overflow");
			len = MAXSTRINGLENGTH;
		}
		d = new Rep(s, len);
		break;
	}
}

String::String(const char *s, unsigned len)
{
	if (!startedUp)
		startUp();
	switch (len) {
	case 0:
		d = nullRep;
		break;
	case 1:
		d = &oneChar[(unsigned char) *s];
		break;
	default:
		if (len > MAXSTRINGLENGTH) {
			error(1, "String::String(char *, unsigned): overflow");
			len = MAXSTRINGLENGTH;
		}
		d = new Rep(s, len);
		break;
	}
}

String::String(const SubString& s)
{
	if (s.length() == 0 )
		d = nullRep;
	else if (s.length() == 1 )
		d = &oneChar[(unsigned char) s.it()->d->start[s.offset()]];
	else if ( !(d = s.it()->d->newSub(s.offset(), s.length())))
		d = new Rep(s.it()->d->start+s.offset(), s.length());
	/* delete &s;	it's used up */
}

