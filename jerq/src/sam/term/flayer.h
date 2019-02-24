#ifndef	FLAYER_H
#define	FLAYER_H
#include "frame.h"


typedef enum Vis{
	None=0,
	Some,
	All,
}Vis;
enum{
	Clicktime=60,		/* one second */
};
typedef struct Flayer Flayer;
struct Flayer{
	Frame		f;
	long		origin;	/* offset of first char in flayer */
	long		p0, p1;
	long		click;	/* time at which selection click occurred, in HZ */
	uchar		**(*textfn)();
	int		user0;
	char		*user1;
	Rectangle	entire;
	Rectangle	scroll;
	Vis		visible;
};

Flayer	*flwhich();
long	scrtotal();
#define	FLMARGIN	3
#define	FLSCROLLWID	12
#define	FLGAP		4
#endif FLAYER_H
