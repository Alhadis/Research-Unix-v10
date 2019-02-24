#include	"draw_dag.h"


/*
	Turn long edges into sequences of length-1 edges
*/

void dag_unitedges()
{
	// compute the thickness for each level
	Levelheight = new int[Maxlevel+1];
	for(int v = 0; v < N_real_nodes; ++v)
		if(Height[v] > Levelheight[Level[v]])
			Levelheight[Level[v]] = Height[v];

	// count the number of new edges, and nodes
	int n_dummies = 0;
	for(v = 0; v < N_real_nodes; v++)
	for(edge_t *e = Outedges[v]; e; e = e->next)
	{
		if(Level[e->node] <= Level[v])
			panic("Bad level assignment");
		n_dummies += (Level[e->node] - Level[v]) - 1;
	}

	// reset N_nodes
	N_nodes += n_dummies;

#ifndef TIMING
	if(Verbose)
		fprintf(stderr,"Dummy nodes = %d\n",n_dummies);
#endif

	// get more space for Level[], Outedges[] and Inedges[]
	Width = (int *) realloc((char *)Width,N_nodes*sizeof(Width[0]));
	Height = (int *) realloc((char *)Height,N_nodes*sizeof(Height[0]));
	Level = (int *) realloc((char *)Level,N_nodes*sizeof(Level[0]));
	Inedges = (edge_t **) realloc((char *)Inedges,N_nodes*sizeof(Inedges[0]));
	Outedges = (edge_t **) realloc((char *)Outedges,N_nodes*sizeof(Outedges[0]));
	if(!Width || !Height || !Level || !Inedges || !Outedges)
		panic("out of memory");
	for(v = N_real_nodes; v < N_nodes; v++)
	{
		Inedges[v] = Outedges[v] = (edge_t *)(NULL);
		Level[v] = Width[v] = Height[v] = 0;
	}

	// create new dummy nodes and edges
	int dummy = N_real_nodes;
	for(v = 0; v < N_real_nodes; v++)
	for(e = Outedges[v]; e; e = e->next)
	{
		if(Level[v]+1 == Level[e->node])
			continue;

		// attributes of new edges
		int	node = e->node;
		int	weight = e->weight;
		int	count = e->count;
		int	width = e->width;

		// delete corresponding Inedge 
		edge_t *lastf = (edge_t*)0;
		for(edge_t *f = Inedges[node]; f; lastf = f, f = f->next)
			if(f->node == v)
				break;
		if(lastf)
			lastf->next = f->next;
		else	Inedges[node] = f->next;

		// make these edges short
		f->next = NULL;
		Inedges[dummy] = f;
		e->node = dummy++;

		// make the rest of the short edges
		int	lev = Level[v]+1;
		int	maxlev = Level[node];
		int	tail = e->node;
		int	head = lev == maxlev-1 ? node : dummy++;
		edge_t	*ochain = e, *ichain = f;
		while(lev < maxlev)
		{
			Level[tail] = lev;
			Width[tail] = width;
			Height[tail] = Levelheight[lev];

			N_edges++;
			Outedges[tail] =
				new edge_t(head,weight,width,1,Outedges[tail]);
			Outedges[tail]->count = count;
			Inedges[head] =
				new edge_t(tail,weight,width,1,Inedges[head]);
			Inedges[head]->count = count;
			Outedges[tail]->link = Inedges[head];
			Inedges[head]->link = Outedges[tail];

			// chain the broken edges
			ochain->chain = Outedges[tail];
			Inedges[head]->chain = ichain;
			ochain = Outedges[tail];
			ichain = Inedges[head];

			if(++lev < maxlev)
			{
				tail = head;
				head = lev == maxlev-1 ? node : dummy++;
			}
		}
	}
}


#ifdef DEBUG
static void d_chk_level()
{
	for(int i = 0; i < N_nodes; ++i)
		if(Level[i] > Maxlevel || Level[i] < 0)
			(void) abort();
}
#endif
