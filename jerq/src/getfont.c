#include <jerq.h>
#include <font.h>
#include <jerqio.h>
#include <msgs.h>
#include "/usr/jerq/icon/16x16/coffeecup"

#define NPROC ((long*)getproctab())[-1]
char s[100] = "/usr/jerq/font/";
extern Font *getfont(), *realdefont();
extern char *strchr();
Texture *icon;
int mflag, dflag, lflag;

/* dirt: getfont uses "Font.unused" for a chain link
*  and knows the structure of alloc and gcalloc blocks
*/

main(argc,argv)
char **argv;
{
	Font *f, *g;
	struct Proc *p;
	int i;
	request(MOUSE);
	icon = cursswitch(&coffeecup);
	for( ; argc>1&&argv[1][0]=='-'; argc--, argv++) {
		for( ; argv[1][1]; argv[1]++) {
			switch(argv[1][1]) {
			case 'm':
				mflag++;
				break;
			case 'd':
				dflag++;
				break;
			case 'l':
				lflag++;
				break;
			}
		}
	}
	if(dflag) print("before");
	if(argc < 2)
		done();
	f = getfont(argv[1]);
	if(f == 0)
		f = getfont(strcat(s, argv[1]));
	if(f == 0)
		error("can't read font",s);
	P->defaultfont = f;
		/*make alloc permanent*/
	((char**)f)[-1] = 0;
	((char**)f->bits)[-1] = 0;
	((long*)((long*)f->bits->base)[-1])[1] = 0;/*gcalloc*/
	if(!lflag) {
		f->unused = (long)Sys[1];
		Sys[1] = (ptr_fint)f;
	} else {
		f->unused = ((Font*)Sys[1])->unused;
		((Font*)Sys[1])->unused = (long)f;
	}
	if(mflag)
		getproctab()[1].defaultfont = f;	/* main menu */
	done();
}

error(s, f)
char *s, *f;
{
	printf("getfont: %s %s\n", s, f);
	done();
}

discard()	/* discard inaccessible fonts */
{
	Font *f = (Font*)Sys[1], *g;
	for(;;) {
		g = (Font*)f->unused;
		if(g==0 || g->unused==0)	/* real defont */
			break;
		if(!busy(g)) {
			f->unused = g->unused;
			bfree(g->bits);
			free((char*)g);
		} else
			f = g;
	}
}

busy(f)		/* is the font used by a live process? */
Font *f;
{
	struct Proc *p = getproctab();
	int i = NPROC;
	for( ; --i>=0; p++)
		if(p->state && p->defaultfont==f)
			return 1;
	return 0;
}

done()
{
	discard();
	if(dflag) print("after");
	cursswitch((Texture*)0);
	muxnewwind(P, C_RESHAPE);
	exit();
}

print(s)
char *s;
{
	struct Proc *p = getproctab();
	int n = NPROC;
	Font *f = (Font*)Sys[1];
	int i;
	printf("%s:\n", s);
	printf("  fonts ");
	for( ; f; f=(Font*)f->unused)
		printf("%D ",f);
	printf("\n");
	printf("  procs and defaultfonts\n  ");
	for(i=0; i<n; i++)
		if(p[i].state)
			printf("%d %D ",i,p[i].defaultfont);
	printf("\n");
}
