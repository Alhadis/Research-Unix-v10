
#include	"complex.h"

complex polar(double r, double theta)
{
	return complex(r * cos(theta), r * sin(theta) );
}

