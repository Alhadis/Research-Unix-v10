/* emit code like that of dave johnson's 'graphdraw' format.
 * slightly incompatible but this format can easily by
 * mangled for other purposes (UNIX TOOLS APPROACH)
 * tabs used as delimiters; spaces can appear in node labels.
 */

#include "draw_dag.h"
#include "dag.h"
#include "parsedag.h"

/* map tab to space */
static char* escape(char *s) {
	char *rv = s;
	do {
		if (*s == '\t') *s = ' ';
	} while (*s++);
	return rv;
}

void emit_graphdraw() {
	int *Degree = new int[N];
	for (int n_edges = 0, i = 0; i < N; i++)
		for (DAG_edge_t *e= Edge[i]; e; e = e->nextof()) {
			Degree[i]++;
			n_edges++;
		}
	printf("%d\t%d\td\n",N,n_edges);	// "d" for directed
	for (i = 0; i < N; i++) {
		char *printname;
		if (Node[i]->label.type) printname = Node[i]->label.value;
		else printname = Node[i]->name;

		printf("%d\t\"%s\"\t%d\t%d\tL\n",Degree[i],escape(printname),Node[i]->pos.x,Node[i]->pos.y);
		for (e = Edge[i]; e; e = e->nextof()) {
			if (e->label.type)
				printf("%d\t\"%s\"\tL",e->node,escape(e->label.value));
			else
				printf("%d\tL",e->node);
			for (int nsp = 0; e->splinept[nsp].x >= 0; nsp++);
			printf("\t%d",nsp);
			for (int sp = 0; sp < nsp; sp++)
				printf("\t%d\t%d",e->splinept[sp].x,e->splinept[sp].y);
			printf("\n");
		}
	}
	delete Degree;
}
