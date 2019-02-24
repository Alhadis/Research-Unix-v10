#include	"stdio.h"

errexit(ret_code,s1,s2,s3)
{
	fprintf(stderr,s1,s2,s3);
	exit(ret_code);
}
