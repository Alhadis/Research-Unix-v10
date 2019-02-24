#ifndef LAYER_H
#define	LAYER_H	LAYER_H
typedef struct Obscured{
	Rectangle rect;	/* obscured rectangle, screen coords */
	Bitmap *bmap;	/* where the obscured data resides */
	struct Layer *lobs;	/* layer currently in rect on screen */
	struct Obscured *next;	/* chaining */
	struct Obscured *prev;
}Obscured;

typedef struct Layer{
	/* The first part looks like (is) a Bitmap */
	Word *base;
	unsigned width;
	Rectangle rect;	/* bounding box of layer */
	struct Obscured *obs;	/* linked list of obscured rectangles */
	struct Layer *front;	/* adjacent layer in front */
	struct Layer *back;	/* adjacent layer behind */
}Layer;

Rectangle rsubp();
Layer *newlayer();
#ifndef	MUX
extern Layer *lfront, *lback;
#endif
#endif
