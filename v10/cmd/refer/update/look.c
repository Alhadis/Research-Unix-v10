# include "stdio.h"
# include "assert.h"
# define mv(f,g) {close(g); dup(f); close(f);}
/* send everything through chkbib & lhunt */
char * indexes= "/usr/dict/papers/Ind";
lookup (fin, fout)
	FILE *fin, *fout;
{
	struct pipe {int fr, fw;};
	struct pipe p1, p2, p3;
pipe(&p1); pipe(&p2); pipe(&p3);
if (fork()==0)
	{
	/* chkbib process; read p1.fr, write p2.fw */
	close(p1.fw); close(p2.fr); close(p3.fr); close(p3.fw);
	mv (p2.fw, 1);
	mv (p1.fr, 0);
	execl("/usr/lib/refer/mkey", "mkey", "-s", 0);
	_assert(0);
	}
close(p1.fr);
if (fork()==0)
	{
	/* lhunt reads p2, writes p3 */
	close(p1.fw); close(p1.fr); close(p2.fw); close(p3.fr);
	mv (p3.fw, 1);
	mv (p2.fr, 0);
	execl("/usr/lib/refer/hunt", "hunt", "-Ty", "-Fn",  /*"-C1",*/
		indexes, 0);
	_assert(0);
	}
close(p3.fw);
close(p2.fr); close(p2.fw);
/* now get some files */
close (fin->_file); fin->_file = p1.fw;
close(fout->_file); fout->_file=p3.fr;
return;
}
