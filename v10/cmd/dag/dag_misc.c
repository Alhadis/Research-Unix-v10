#include	"draw_dag.h"


/*
	Miscellaneous routines
*/

int longedge(register int v, register int w)
{
	register int t = Level[v]-Level[w];
	return (t > 1 || t < -1) ? 1 : 0;
}

void deledges(int n_nodes, edge_t **edges)
{
	for(int v = 0; v < n_nodes; ++v)
	{
		edge_t *e, *enext;
		for(e = edges[v]; e; e = enext)
		{
			enext = e->next;
			delete e;
		}
	}
	delete edges;
}

void panic(char *s)
{
	fprintf(stderr,"draw_dag: %s\n",s);
	(void) abort();
}

void * operator new(long nbytes)
{
	if (!nbytes) return (void*)0;
	char *rv = malloc(nbytes);
	if(!rv)
		panic("out of memory");
	memset(rv,0,nbytes);
	return (void*)rv;
}
