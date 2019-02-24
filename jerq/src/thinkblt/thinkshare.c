#include <jerq.h>
#define	THINKSHARE_C
#include <thinkclient.h>

extern int thinkchar(), thinknchars(), thinkflush(), thinkmap(), thinkabort();

static struct Thinksubs thinksubs = {
	thinkchar, thinknchars, thinkflush, thinkmap, thinkabort,
};

thinkshare()
{
	putname("thinksubs", &thinksubs);
}
