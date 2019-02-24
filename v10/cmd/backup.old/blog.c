#include	<fio.h>
#include	"backup.h"
#include	<signal.h>

main(argc, argv)
	char **argv;
{
	if(argc != 2){
		fprint(2, "usage: blog string\n");
		exit(1);
	}
	logprint("%s\n", argv[1]);
	exit(0);
}
