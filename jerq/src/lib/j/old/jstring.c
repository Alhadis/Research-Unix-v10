#include <jerq.h>
#include <font.h>

extern Font defont;
extern Point PtCurrent;
extern Point string();
Point jstring(s)
	register char *s;
{
	return PtCurrent=string(&defont,s,&display,PtCurrent,F_XOR);
}
