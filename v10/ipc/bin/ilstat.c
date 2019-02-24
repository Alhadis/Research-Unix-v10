#include <sys/param.h>
#include <sys/types.h>
#include <sys/stream.h>
#include <nlist.h>
#include <stdio.h>

struct nlist nl[] = {
	{"_il", 0},
	{0, 0},
};

int kern_fd;

main(argc, argv)
char *argv[];
{
	char *nuxi, *core;
	struct block b;
	struct il{
		int attached, active, rcvpending;
		int ipackets, opackets;
		int ierrors, oerrors;
		int collisions;
		int bp;
		int nbp;
		int len;
		int tq;
		int freebp;
		unsigned char addr[6];
	} il;

	nuxi = "/unix";
	core = "/dev/kmem";
	if(argc > 1)
		nuxi = argv[1];
	if(argc > 2)
		core = argv[2];
	if((kern_fd = open(core, 0)) < 0){
		perror(core);
		exit(1);
	}
	nlist(nuxi, nl);
	if(nl[0].n_value == 0){
		printf("nlist %s error\n", nuxi);
		exit(1);
	}
	if(strcmp(core, "/dev/kmem"))
		nl[0].n_value &= 0xffffff;
	lseek(kern_fd, nl[0].n_value, 0);
	if(read(kern_fd, &il, sizeof(il)) != sizeof(il)){
		perror("read");
		exit(1);
	}
	printf("%7d input\n", il.ipackets);
	printf("%7d input errors\n", il.ierrors);
	printf("%7d output\n", il.opackets);
	printf("%7d output errors\n", il.oerrors);
	printf("%7d collisions\n", il.collisions);
}
