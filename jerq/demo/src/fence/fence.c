#include <jerq.h>
#include <jerqio.h>
#include <font.h>

#define LMAXPT	40
#define RMAXPT	90
#define INC	6
#define Y	8
#define NUMPTS	5
#define NICONS	4
#define HEIGHT	48	/* bits */
#define WIDTH	48	/* bits */
#define	MAXRAND	32767L
#define BITBLT(b, p, f)	bitblt(&b, b.rect, &display, p, f);

Point lpt[NUMPTS], rpt[NUMPTS];
Bitmap lbmap[NICONS], rbmap[NICONS];

short licon[NICONS][HEIGHT*WIDTH] =
   {
       {
#include "fence1l"
       },
       {
#include "fence2l"
       },
       {
#include "fence3l"
       },
       {
#include "fence4l"
       },
   },
ricon[NICONS][HEIGHT*WIDTH] =
   {
       {
#include "fence1r"
       },
       {
#include "fence2r"
       },
       {
#include "fence3r"
       },
       {
#include "fence4r"
       },
   };

struct sequence
   {
    int i;
    int p;
   } lseq[] =
       {
	{ 0, 0 }, { 1, 1 }, { 2, 1 }, { 3, 1 }, { 3, 2 }, { 3, 3 }, { 3, 4 },
	{ 0, 3 }, { 0, 2 }, { 0, 1 },
       },
     rseq[] =
       {
	{ 3, 0 }, { 2, 1 }, { 1, 1 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 },
	{ 3, 3 }, { 3, 2 }, { 3, 1 },
       };

main(argc, argv)
   int argc;
   char *argv[];
   {
    init();
    if (argc > 1)
       {
	srand(*XMOUSE);
	doit(1);
       }
    else
	doit(0);
   }

init()
   {
    int i;

    request(KBD);

    rectf(&display,Drect,F_CLR);
    string(&defont, " Touche!  ", &display, Drect.origin, F_STORE);
    for (i = 0; i < NUMPTS; i++)
       {
	lpt[i] = add(Drect.origin, Pt(LMAXPT - i*INC, Y));
    	rpt[i] = add(Drect.origin, Pt(RMAXPT - i*INC, Y));
       }

    for (i = 0; i < NICONS; i++)
       {
	lbmap[i].base = (Word *) licon[i];
    	lbmap[i].width = WIDTH % WORDSIZE ? WIDTH/WORDSIZE+1 : WIDTH/WORDSIZE;
    	lbmap[i].rect.origin.x = 0;
    	lbmap[i].rect.origin.y = 0;
    	lbmap[i].rect.corner.x = WIDTH;
   	lbmap[i].rect.corner.y = HEIGHT;
	rbmap[i].base = (Word *) ricon[i];
    	rbmap[i].width = WIDTH % WORDSIZE ? WIDTH/WORDSIZE+1 : WIDTH/WORDSIZE;
    	rbmap[i].rect.origin.x = 0;
    	rbmap[i].rect.origin.y = 0;
    	rbmap[i].rect.corner.x = WIDTH;
   	rbmap[i].rect.corner.y = WIDTH;
       }
}

doit(r)
   {
    int i;

    request(KBD);
    while (kbdchar() != 'q')
       {
	if (P->state&RESHAPED)
	    init();
	for (i = 0; i < (sizeof lseq)/(sizeof lseq[0]); i++)
	    if (r)
	        draw(roll(NICONS), roll(NICONS), roll(NUMPTS), roll(NUMPTS));
	    else
	    	draw(lseq[i].i, rseq[i].i, lseq[i].p, rseq[i].p);
       }
   }

draw(l, r, lp, rp)
   int l, r, lp, rp;
   {
    BITBLT(lbmap[l], lpt[lp], F_OR);
    BITBLT(rbmap[r], rpt[rp], F_OR);
    sleep(15);
    BITBLT(lbmap[l], lpt[lp], F_CLR);
    BITBLT(rbmap[r], rpt[rp], F_CLR);
   }

roll(nsides)
   int nsides;
   {
    return (int) ((rand() * (long) nsides) / ((long) MAXRAND + 1));
   }
