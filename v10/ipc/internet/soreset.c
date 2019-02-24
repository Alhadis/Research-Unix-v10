#include <sys/param.h>
#include <sys/types.h>
#include <sys/stream.h>
#include <nlist.h>
#include <stdio.h>

#include <sys/inet/in.h>
#include <sys/inet/tcp.h>

struct nlist nl[] = {
	{"_tcpsocks", 0},
	{0, 0},
};

int kern_fd;

main(argc, argv)
char *argv[];
{
	char *nuxi, *core;
	struct block b;
	register i;

struct socket{
	struct		tcpcb *so_tcpcb;
	struct		queue *so_rq;
	struct		queue *so_wq;
	int		so_rcount;
	int		so_wcount;	/* because q->count is inaccurate */
	struct		socket *so_head; /* parent who listened */
	int		so_dev;
	int		so_state;
	int		so_options;
	in_addr		so_laddr;
	in_addr		so_faddr;
	tcp_port	so_lport;
	tcp_port	so_fport;
	int		so_oobmark;
	int		so_delimcnt;	/* to detect logical eof */
} so;
#define SO_DONTROUTE	0x1
#define SO_KEEPALIVE	0x2
#define SO_ACCEPTCONN	0x4	/* this is real */

#define SS_OPEN		0x1	/* by user */
#define SS_PLEASEOPEN	0x2	/* waiting for user open */
#define SS_RCVATMARK	0x4	/* some kind of OOB */
#define SS_WAITING	0x8	/* wait for user control */
#define	SS_ACTIVE	0x10	/* has tcp action */
#define SS_HANGUP	0x20	/* HANGUP on TH_FIN */
#define SS_HUNGUP	0x40	/* socket has been hung up (avoid multiple) */
#define SS_WCLOSED	0x80	/* write side is closed */

#define	STATE_BITS	"\10WCLOSED\7HUNGUP\6HANGUP\5ACTIVE\4WAITING\3RCVATMARK\2PLEASOPEN\1OPEN"
	nuxi = "/unix";
	core = "/dev/kmem";
	if(argc > 1)
		nuxi = argv[1];
	if(argc > 2)
		core = argv[2];
	if((kern_fd = open(core, 2)) < 0){
		perror(core);
		exit(1);
	}
	nlist(nuxi, nl);
	if(nl[0].n_value == 0){		printf("nlist %s error\n", nuxi);
		exit(1);
	}
	if(strcmp(core, "/dev/kmem"))
		nl[0].n_value &= 0xffffff;
	lseek(kern_fd, nl[0].n_value, 0);
	for (i=0; i<32; i++) {
		if(read(kern_fd, &so, sizeof(so)) != sizeof(so)){
			perror("read");
			exit(1);
		}
		so.so_state = 0;
		so.so_options = 0;
		so.so_laddr = so.so_faddr = 0;
		so.so_lport = so.so_fport = 0;
		if (lseek(kern_fd, -sizeof(so), 1) == -1) {
			perror("seek");
			exit(1);
		}
		if(write(kern_fd, &so, sizeof(so)) != sizeof(so)){
			perror("write");
			exit(1);
		}
	}
	
}
