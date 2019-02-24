#include <jerq.h>
#include "event.h"
extern Bitmap *buildimage();
extern Rectangle scaler();
char expl4[]={
	'R', -4, -4, 4+1, 4+1,
	'p', 0, -1,
	'p', 3, 0,
	'p', 0, 0,
	'p', 1, 3,
	'p', 0, 0,
	'p', 1, -1,
	'p', 3, 0,
	'p', 1, 0,
	'p', 4, 1,
	'p', 3, 2,
	'p', 2, 0,
	'p', -3, 0,
	'p', -1, 3,
	'p', 1, 0,
	'p', -1, -1,
	'p', 0, 1,
	'p', 0, 0,
	'p', 3, -1,
	'p', 0, 0,
	'p', 1, 3,
	'p', 3, 1,
	'p', -1, 1,
	'p', 2, 3,
	'p', 0, 3,
	'p', 3, 2,
	0};
char expl8[]={
	'R', -8, -8, 8+1, 8+1,
	'p', -1, -3,
	'p', 7, 1,
	'p', -1, 0,
	'p', 3, 7,
	'p', 0, -1,
	'p', 2, -3,
	'p', 6, 0,
	'p', 2, -1,
	'p', 8, 3,
	'p', 7, 5,
	'p', 5, 0,
	'p', -6, 1,
	'p', -3, 7,
	'p', 2, 0,
	'p', -2, -3,
	'p', 0, 3,
	'p', 1, 1,
	'p', 6, -3,
	'p', 0, 0,
	'p', 2, 6,
	'p', 7, 2,
	'p', -2, 2,
	'p', 4, 6,
	'p', -1, 6,
	'p', 6, 4,
	0};
char expl12[]={
	'R', -12, -12, 12+1, 12+1,
	'p', -2, -4,
	'p', 10, 2,
	'p', -2, 1,
	'p', 5, 11,
	'p', 0, -2,
	'p', 3, -5,
	'p', 9, 1,
	'p', 3, -2,
	'p', 12, 4,
	'p', 10, 8,
	'p', 8, 0,
	'p', -9, 2,
	'p', -5, 11,
	'p', 4, 0,
	'p', -4, -4,
	'p', 1, 5,
	'p', 1, 2,
	'p', 10, -5,
	'p', 0, 0,
	'p', 3, 9,
	'p', 11, 3,
	'p', -3, 3,
	'p', 7, 9,
	'p', -1, 9,
	'p', 9, 6,
	0};
char expl16[]={
	'R', -16, -16, 16+1, 16+1,
	'p', -3, -6,
	'p', 14, 3,
	'p', -3, 1,
	'p', 7, 14,
	'p', 0, -3,
	'p', 4, -7,
	'p', 12, 1,
	'p', 4, -3,
	'p', 16, 6,
	'p', 14, 10,
	'p', 11, 0,
	'p', -12, 3,
	'p', -7, 14,
	'p', 5, 1,
	'p', -5, -6,
	'p', 1, 7,
	'p', 2, 3,
	'p', 13, -7,
	'p', 0, -1,
	'p', 5, 12,
	'p', 15, 5,
	'p', -4, 4,
	'p', 9, 12,
	'p', -2, 12,
	'p', 12, 8,
	0};
char expl20[]={
	'R', -20, -20, 20+1, 20+1,
	'p', -4, -8,
	'p', 17, 4,
	'p', -4, 2,
	'p', 8, 18,
	'p', 0, -4,
	'p', 5, -9,
	'p', 15, 2,
	'p', 5, -4,
	'p', 20, 8,
	'p', 17, 13,
	'p', 14, 0,
	'p', -16, 4,
	'p', -8, 18,
	'p', 7, 1,
	'p', -7, -8,
	'p', 2, 8,
	'p', 3, 4,
	'p', 16, -9,
	'p', 0, -1,
	'p', 6, 15,
	'p', 19, 6,
	'p', -5, 5,
	'p', 12, 16,
	'p', -3, 15,
	'p', 15, 11,
	0};
char expl25[]={
	'R', -25, -25, 25+1, 25+1,
	'p', -6, -10,
	'p', 22, 6,
	'p', -6, 3,
	'p', 11, 23,
	'p', -1, -6,
	'p', 7, -12,
	'p', 19, 3,
	'p', 7, -6,
	'p', 25, 10,
	'p', 22, 17,
	'p', 18, 0,
	'p', -20, 5,
	'p', -11, 23,
	'p', 9, 2,
	'p', -9, -10,
	'p', 3, 11,
	'p', 4, 6,
	'p', 21, -12,
	'p', 1, -2,
	'p', 8, 19,
	'p', 24, 8,
	'p', -7, 7,
	'p', 15, 20,
	'p', -4, 19,
	'p', 19, 14,
	0};
char *exim[]={
	expl4, expl8, expl12, expl16, expl20, expl25, 0
};
#define	NIMAGE	(sizeof exim/sizeof (char *) -1)
Bitmap *exseq[sizeof exim/sizeof (char *)];
typedef struct{
	Point	p;
	char	image;
	char	allocated;
}Explosion;
#define	NEXPL	16	/* known in event type */
#define	EXPLTICKS	32
Explosion ex[NEXPL];
int	exadvance();
initexplosion()
{
	register i;
	for(i=0; i<NIMAGE; i++)
		exseq[i]=buildimage(exim[i]);
	for(i=0; i<NEXPL; i++)
		ex[i].allocated=0;
}
exfree()
{
	register Bitmap **b;
	for(b=exseq; b<&exseq[NIMAGE]; b++)
		bfree(*b);
}
explode(p)
	Point p;
{
	register i;
	register Explosion *e;
	for(i=0, e=ex; i<NEXPL; i++, e++)
		if(e->allocated==0){
			if(addevent(newevent(), EXPLTICKS, exadvance, EEXPL|i)){
				e->p=p;
				e->allocated=1;
				e->image=0;
				exdraw(i);
			}
			break;
		}
}
exdraw(i)
{
	register Bitmap *b=exseq[ex[i].image];
	bitblt(b, b->rect, &display, add(transform(ex[i].p), b->rect.origin), F_XOR);
}
exadvance(x)
{
	register Explosion *e= &ex[x&=~EEXPL];
	extern notdrawn;
	exdraw(x);
	if(++e->image >= NIMAGE){
		e->allocated=0;
		return 0;
	}
	exdraw(x);
	--notdrawn;	/* minor kludge to keep time smooth */
	return 1;
}
