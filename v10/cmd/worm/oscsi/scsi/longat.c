#include	"../scsi.h"

unsigned long
longat(uchar *src)
{
	unsigned long n;

	n = *src++;
	n = (n<<8) | *src++;
	n = (n<<8) | *src++;
	n = (n<<8) | *src;
	return(n);
}
