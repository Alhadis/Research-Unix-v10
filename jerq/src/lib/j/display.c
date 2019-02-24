#include <jerq.h>
Bitmap display = { 
	(Word *)(0x700000), 			/* bitmap.base */
	25, 					/* width in 32 bit words */
	0, 					/* bitmap.rect.origin.x */
	0, 					/* bitmap.rect.origin.y */
	XMAX, 					/* bitmap.rect.corner.x */
	YMAX 					/* bitmap.rect.corner.y */
};
