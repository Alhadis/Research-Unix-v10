#include <sgtty.h>

main()
{
	int fd, x;

	fd = open("/dev/dk/dk01", 2);
	if(fd < 0){
		perror("dk01");
		exit(1);
	}
	printf("dk01 opened %d\n", fd);
	while((x = ioctl(fd, FIOLOOKLD, 0)) != -1){
		printf("popping %d\n", x);
		if(ioctl(fd, FIOPOPLD, 0) < 0){
			perror("pop");
			exit(1);
		}
	}
	printf("done\n");
	exit(0);
}
