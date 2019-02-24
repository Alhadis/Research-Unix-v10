#ifndef LAYER_H
#define	LAYER_H	LAYER_H
typedef struct Layer{
	/* The first part looks like (is) a Bitmap */
	Word *base;
	unsigned width;
	Rectangle rect;	/* bounding box of layer */
/* The next fields give us compatability with Rob's layers, and with
   bitmaps, and let us perform efficiency hacks.  They can safely be
   set to true, false with no chance of error.  Note that
   nonevis ==> someobs; all we really care is to find out one of
   three states: all visible (in which case the following word should
   be identically zero), all obscured (so that we can skip layerop),
   and partially obscured and partially visible. */
	short someobs, nonevis;
	struct Layer *front;	/* adjacent layer in front */
	struct Layer *back;	/* adjacent layer behind */
	Bitmap *obs;		/* Storage for obscured bits */
}Layer;

typedef Layer Obscured;

Rectangle rsubp();
Layer *newlayer();
#ifndef	MUX
extern Layer *lfront, *lback;
#endif
#endif
