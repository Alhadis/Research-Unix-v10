#include <values.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/filio.h>
#include <sys/inio.h>
#include <sys/enio.h>
#include <sys/inet/in.h>
#include <sys/inet/ip_var.h>
#include <nlist.h>

extern errno;
extern char *optarg;
extern int optind;
extern int ip_ld;

struct nlist nl[] = {
	{"_ip_arps", 0},
	{"_arpcnt", 0},
	{0, 0}
};
#define NL_ARP 0
#define NL_NARP 1

#define MAXARP 512
struct x{
	unsigned int inaddr;
	unsigned char enaddr[6];
} goo;
struct ip_arp arp[MAXARP];
int kern_fd;

usage()
{
	fprintf(stderr, "usage:\tarp hostname\n");
	fprintf(stderr, "\tarp -a [unix] [kmem]\n");
	fprintf(stderr, "\tarp -d hostname\n");
	fprintf(stderr, "\tarp -s hostname ether_addr\n");
	exit(1);
}

main(argc, argv)
char *argv[];
{
	int c, narp;
	int allf;
	int deletef;
	int setf;
	char *xinu;	
	char *kmem;

	allf = deletef = setf = 0;
	xinu = "/unix";
	kmem = "/dev/kmem";

	while((c = getopt(argc, argv, "ads")) != -1) {
		switch(c){
		case 'a':
			allf = 1;
			break;
		case 'd':
			deletef = 1;
			break;
		case 's':
			setf = 1;
			break;
		default:
			usage();
		}
	}

	if(allf+setf+deletef > 1)
		usage();

	if(allf){
		if (argc > optind+1)
			xinu = argv[optind++];
		if (argc > optind)
			kmem = argv[optind];
		kern_init(xinu, kmem);
		displayall();
	} else if(setf){
		if(optind != argc-2)
			usage();
		set(argv[optind], argv[optind+1]);
	} else if(deletef){
		if(optind != argc-1)
			usage();
		delete(argv[optind]);
	} else {
		if(optind >= argc)
			usage();
		kern_init(xinu, kmem);
		narp = readarps();
		for(; optind < argc; optind++)
			display(narp, argv[optind]);
	}
}

etherparse(to, from)
	unsigned char *to;
	char *from;
{
	int tdig;
	int fdig;
	int i;

	if(strlen(from) != 12){
		fprintf(stderr, "illegal ether address %s\n", from);
		exit(1);
	}

	for(i = 0; i < 6; i++){
		fdig = *from++;
		tdig = fdig > 'a' ? (fdig - 'a' + 10)
				: (fdig > 'A' ? (fdig - 'A' + 10) : (fdig - '0'));
		fdig = *from++;
		tdig <<= 4;
		tdig |= fdig > 'a' ? (fdig - 'a' + 10)
				: (fdig > 'A' ? (fdig - 'A' + 10) : (fdig - '0'));
		*to++ = tdig;
	}
}

set(host, address)
	char* host;
	char *address;
{
	int pfd[2];
	int i;

	goo.inaddr = in_address(host);
	if(goo.inaddr == 0){
		fprintf(stderr, "can't find ip address for %s\n", host);
		return;
	}
	etherparse(goo.enaddr, address);
	if(pipe(pfd) < 0){
		perror("setting");
		exit(1);
	}
	if(ioctl(pfd[0], FIOPUSHLD, &ip_ld) < 0){
		perror("setting");
		exit(1);
	}
	if(ioctl(pfd[0], IPIORESOLVE, &goo) < 0)
		perror("can't set ether address");
	close(pfd[0]);
	close(pfd[1]);
}

delete(host)
	char *host;
{
	int i;
	int pfd[2];


	goo.inaddr = in_address(host);
	if(goo.inaddr == 0){
		fprintf(stderr, "can't find ip address for %s\n", host);
		return;
	}

	if(pipe(pfd) < 0){
		perror("setting");
		exit(1);
	}
	if(ioctl(pfd[0], FIOPUSHLD, &ip_ld) < 0){
		perror("setting");
		exit(1);
	}

	/*
 	 *  this assumes the hashing algorithm is the same as that of
	 *  the system
	 */
	for(i = 0; i < 256; i++){
		goo.inaddr = i;
		if(ioctl(pfd[0], IPIORESOLVE, &goo) < 0){
			fprintf(stderr, "can't delete ether address for %s\n", host);
			break;
		}
	}
	close(pfd[0]);
	close(pfd[1]);
}

kern_init(xinu, kmem)
char *xinu, *kmem;
{
	int i;

	nlist(xinu, nl);
	if((long)nl[0].n_value == 0){
		fprintf(stderr, "nlist %s failed\n", xinu);
		exit(1);
	}
	if(strcmp(kmem, "/dev/kmem") != 0){
		for(i = 0; nl[i].n_name; i++){
			nl[i].n_value &= 0xffffff;
		}
	}
	kern_fd = open(kmem, 0);
	if(kern_fd < 0){
		perror(kmem);
		exit(1);
	}
}

doseek(nlitem)
	unsigned int nlitem;
{
	if (nl[nlitem].n_value == 0)
		return -1;
	if(lseek(kern_fd, (long)nl[nlitem].n_value, 0) == -1){
		perror("seek");
		exit(1);
	}
	return 0;
}

void
doseekoff(nlitem, offset)
	unsigned int nlitem;
	unsigned int offset;
{
	if(lseek(kern_fd, (long)(nl[nlitem].n_value+offset), 0) == -1){
		perror("seek");
		exit(1);
	}
}

void
doread(addr, size)
	char *addr;
	unsigned int size;
{
	if(read(kern_fd, addr, size) < 0){
		perror("read");
		exit(1);
	}
}

readarps()
{
	int narp;

	if (doseek(NL_NARP) < 0) {
		fprintf(stderr, "Arp not compiled into this kernel\n");
		exit(1);
	}
	doread((char *)&narp, sizeof narp);
	if (narp > MAXARP){
		fprintf(stderr, "more arps than I can handle\n");
		narp = MAXARP;
	}
	doseek(NL_ARP);
	doread((char *)arp, narp*sizeof(struct ip_arp));
	return narp;
}

displayall()
{
	int narp, i, j;

	narp = readarps();
	for(i = 0; i < narp; i++){
		if(arp[i].inaddr >= MAXARP){
			printf("%s %28.28s ", in_ntoa(arp[i].inaddr), in_host(arp[i].inaddr));
			for(j = 0; j < 6; j++){
				printf("%02x", arp[i].enaddr[j]);
			}
			printf("\n");
		}
	}
}

display(narp, host)
	int narp;
	char *host;
{
	int i, j;

	goo.inaddr = in_address(host);
	if(goo.inaddr == 0){
		fprintf(stderr, "can't find ip address for %s\n", host);
		return;
	}
	for(i = 0; i < narp; i++){
		if(arp[i].inaddr == goo.inaddr){
			printf("%.15s %.28s ", in_ntoa(arp[i].inaddr), in_host(arp[i].inaddr));
			for(j = 0; j < 6; j++){
				printf("%02x", arp[i].enaddr[j]);
			}
			printf("\n");
		}
	}
}
