#include <ipc.h>
main()
{
	int fd;

	fd = ipcopen("/cs/default!astro/forbesC/mesgdcon", "");
	if (fd<0){
/*		fd = ipcopen("/cs/default!astro/forbesC/mesgdcon", "");*/
		if (fd<0)
			exit(1);
	}
	printf("opened!\n");
	close(fd);
	
}
