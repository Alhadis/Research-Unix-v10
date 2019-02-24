#include "jerq.h"

main() {
	if(VALSCREENCOLOR)
		TVIDEO;
	else
		RVIDEO;
	VALSCREENCOLOR ^= 1;
}
