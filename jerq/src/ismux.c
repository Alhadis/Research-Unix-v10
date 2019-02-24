#include "/usr/jerq/include/jioctl.h"
int	print=1;
main(argc, argv)
	char *argv[];
{
	if(argc>1)
		print=0;
	if(ioctl(0, JMUX, 0)==0){
		if(print)
			printf("yes\n");
		return 0;
	}else{
		if(print)
			printf("no\n");
		return 1;
	}
}
