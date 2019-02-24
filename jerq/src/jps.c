#include <jerq.h>
#include <jerqio.h>

#define	USER		8	/* a user-68ld'd process */

struct udata {
	Rectangle Drect;
	Layer	 * Jdisplayp;
	int	argc;
	char	**argv;
	struct Mouse mouse;
};

main(argc, argv)
char **argv;
{
	register i, j;
	register struct Proc *p; register Layer *l;
	register struct udata *udp;
	struct Proc *proctab; int nproc;

	proctab = getproctab();
	nproc = *((int *)proctab - 1);

	/* printf("0x%x %d\n", proctab, nproc); */
	for (i=2; i<nproc; i++) {
		if ((p = proctab+i)->state == 0)
			continue;
		udp = (struct udata *)p->data;
		l = p->layer;
		printf("%d %d %d %d \"",
		    l->rect.origin.x, l->rect.origin.y,
		    l->rect.corner.x, l->rect.corner.y);
		if (p->state & USER) for (j=0; j<udp->argc; j++) {
			if (j > 0)
				printf(" ");
			printf("%s", udp->argv[j]);
		}
		printf("\"\n");
	}
	exit();
}
