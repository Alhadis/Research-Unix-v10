/*% cyntax -j % && 3cc -go #.m %
 * movy -- jerq movie editor
 */
#include <jerq.h>
#include <jerqio.h>
#define	NWIN	40
struct win{
	Bitmap *image;
	struct winfns *f;
	void *private;
}win[NWIN];
int nwin=0;
struct winfns{
	int (*input)();
	int (*output)();
};
int nframe=0;
main(){
	register i;
	request(KBD|MOUSE);
	for(;;){
		if(button3())
			menu3();
		if(nwin)
			(*win[0].f->input)(&win[0]);
		for(i=0;i!=nwin;i++)
			(*win[i].f->output)(&win[i]);
		wait(CPU);
	}
}
#include <menu.h>
/*
 * Button 3 menu:
 *	M3		M3new		M3frame
 *	+-------+	+--------+
 *	|New  ->|----->	|playback|
 *	|Delete |	|exposure|	+---+
 *	|Move   |	|frame ->|---->	| 1 |
 *	|Exit   |	+--------+	| 2 |
 *	+-------+			| 3 |
 *					| 4 |
 *					| . |
 *					| . |
 *					| . |
 *					+---+
 */
M3framegen(n){
	static NMitem item;
	static char name[10]={0, 0, 0, 0, 0, HitFrame};
	if(++n<1 || nframe<n)
		item.text=0;
	else{
		sprintf(name, "%d", n);
		item.text=name;
		item.data=n;
	}
	return &item;
}
NMenu M3frame={0, M3framegen};
NMitem M3newitems={
{"playback", 0, 0, 0, 0, HitPlayback},
{"exposure", 0, 0, 0, 0, HitExposure},
{"frame, 0, M3frame},
0
};
NMenu M3new={M3newitems};
NMitem M3items[]={
	{"New",	0, M3new},
	{"Delete", 0, 0, 0, 0, HitDelete},
	{"Move", 0, 0, 0, 0, HitMove},
	{"Exit", 0, 0, 0, 0, HitExit},
	0
};
NMenu M3={M3items};
menu3(){
	mhit(&M3, 3);
}
	cursswitch(t);
	return button(b);
}
