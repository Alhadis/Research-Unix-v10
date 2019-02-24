#include <stdio.h>
#include "graphtypes.h"
#define CIRCLERAD .06
main (argc, argv)

int argc;
char *argv[];
{
	GRAPH graph;
	int size,i,j,l=0, end;
	int labelvert = 0, labeledge = 0, imagen = 0; /* option flags */
	float chop1 = 0, chop2 = 0; /* for endpoints of edges */
	char *s, c;
	edge_entry currentedge;
	float  x, y;

	float pagewidth = 600, pageheight = 600; /* in 100ths of an inch */

	while (--argc > 0 && (*++argv)[0] == '-')
		for (s = argv[0]+1; *s != '\0'; s++)
			switch (*s) {
			case 'v': /* label the vertices */
				labelvert = 1;
				break;
			case 'e': /* label the edges */
				labeledge = 1;
				break;
			case 'i': /* format for the imagen */
				imagen = 1;
				break;
			default:
				/*if option is no legal print an error & continue*/
				printf(stderr,"graphpic: illegal option %c\n", *s);
				break;
			}

	if (argc == 0) /* no filename given=> error */	{
		fprintf(stderr,
		       "graphpic: must supply file name to graphpic command\n");
		exit(1);
		}
	
	while (argc-- > 0) { /* loop over all filenames given */
		graph = Readgraph(&size, *argv);
		Scalegraph(graph, size, pagewidth, pageheight);

		/* define the different types of vertices */
		printf (".PS\n");
		printf ("circlerad = %.2f \n",CIRCLERAD);
		printf ("define vertex X circle X\n");

		if (imagen == 1) {
			printf ("define bullet X \"\\s14\\v\'1.5p\'\\(bu\\v\'-1.5p\'\\s10\"  X\n"); 
			printf ("define full X vertex at ($1, $2) invis \"\\s24\\v\'3.5p\'\\h\'-.5p\'\\(bu\\v\'-3.5p\'\\h\'.5p\'\\s10\" X\n");
		}
		else { 
			printf ("define bullet X \"\\v\'1.5p\'\\h\'-.5p\'\\(bu\\v\'-1.5p\'\\h\'.5p\'\"  X\n"); 	
			printf ("define full X vertex at ($1, $2) invis \"\\s18\\v\'3.5p\'\\h\'-.5p\'\\(bu\\v\'-3.5p\'\\h\'.5p\'\\s10\" X\n");
		}

		printf ("define empty X vertex at ($1, $2) X\n");
		printf ("define invbl X vertex at ($1, $2) invis X\n");
		printf ("define light X vertex at ($1, $2) invis bullet X\n");
		printf ("define heavy X vertex at ($1, $2) bullet X\n");
		printf ("define edge X line chop $1 chop $2 X\n");

		for (i = 1; i<= size ; i++) {
			x = Xcoord(graph,i);
			y = Ycoord(graph,i); 
			x = x/100 - 1; /* convert coor's to inches & center */
			y = 3 - y/100; /* put in PIC's coord system */

			switch (VertGraphics (graph, i)) {
			   case 'H':
				printf ("heavy(%.2f,%.2f)\n",x,y);
				break;
			   case 'F':
				printf ("full(%.2f,%.2f)\n",x,y);
				break;
			   case 'E':
				printf ("empty(%.2f,%.2f)\n", x, y);
				break;
			   case 'I':
				printf ("invbl(%.2f,%.2f)\n",x,y);
				break;
			   default:
				printf ("light(%.2f,%.2f)\n", x, y);
				break;
			}
			currentedge = FirstEdge(graph,i);

			for (j = 1; j<=Degree(graph,i); j++) {
				if ((end=EndPoint(currentedge)) < i) {
					/* draw edge */
					if (EdgeGraphics(currentedge) == 'H')
						/* heavy edge */
						printf(".ps 36\n");
 					if ((c=VertGraphics(graph,i))=='H' || c=='E')
						chop1 = CIRCLERAD;
					else
						chop1 = 0;
					if ((c=VertGraphics(graph,end))=='H' || c=='E')
						chop2 = CIRCLERAD;
					else
						chop2 = 0;
					printf("edge(%.2f,%.2f) from %dth vertex to %dth vertex \n", chop1, chop2, i, end);
					printf(".ps 10\n");
					}
				currentedge = NextEdge(currentedge);
			}
		}
		/* label the vertices */
		if (labelvert == 1)
			for (i=1; i<=size; i++) {
				printf ("\"\" \"%s\" below rjust at %dth vertex\n", NLabel(graph,i), i);
			}
		/* label the edges */
		if (labeledge == 1) 
			for (i=1; i<=size; i++) {
				currentedge = FirstEdge(graph,i);
				for (j=1; j<=Degree(graph,i); j++) {
					if ((end = EndPoint(currentedge)) < i)
						printf ("\"  %s\" below ljust at 1/2 <%dth vertex , %dth vertex>\n",ELabel(currentedge),i,EndPoint(currentedge));

					currentedge = NextEdge(currentedge);
				}
			}
		printf (".PE\n");
		++argv;


	}
}


