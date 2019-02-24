/*  This program gets a histogram of elements that consist
of a pointer to a char[] and a probability or number of occurances
for each.  The size of the char[] is up to you, you pass back
a pointer to it (and you make it in the first place in the loading
routine, called getstuf(**char,*float)  At the end, you will
be presented with a NODE * pointer to the head of both the
linked list and the top of the huffman code tree.  The linked
list is in order of generation, from most likely to least,
including the internodes as well as terminal nodes.  The tree
is a tree, naturally...  

	Written by James Johnston, AT&T Bell Labs, 6/2/87.  
	Copyrighted code, all rights reserved to AT&T per
	employment agreement, etc, etc, etc    			*/

#include <stdio.h>
#include <huff.h>

static int compar();
static NODE *mknd(), *loadall(), *putone(), *taketwo();
double log();

NODE *
huff(inrout)
int ( *inrout)();
{
	NODE * active, * result, * tmp;
	NODE ** tmps, ** arhead;
	int i, j, k, max;

	active=(NODE *)0;
	result=(NODE *)0;

	active=loadall(inrout);
	/* loadall returns a pointer to the beginning */
	tmp=active;
	max=0;
	/* count how many */
	while ( tmp != (NODE *) 0 ) {
		tmp=tmp->to;
		max++;
	}
	/* load pointer array for sorting */
	arhead= (NODE **) malloc(max*sizeof(NODE *));
	if (arhead == (NODE **) 0 ) {
		fprintf(stderr,"can't malloc sorting array");
		exit();
	}
	tmp = active;
	tmps=arhead;
	while ( tmp != (NODE *) 0 ) {
		*(tmps++)=tmp;
		tmp = tmp -> to;
	}
	qsort(arhead,max,sizeof(NODE *), compar);
	/* relink list */

	active= *arhead;
	active->from=(NODE *) 0;
	active->to=arhead[1];
	for ( i=1; i<max-1; i++) {
		arhead[i]->to=arhead[i+1];
		arhead[i]->from=arhead[i-1];
	}
	arhead[max-1]->to=(NODE *)0;
	arhead[max-1]->from=arhead[max-2];
	/* now it's reloaded */

	free(arhead);

	/* this takes the first two elements of the list away, and puts
	a third one in wherever it fits, in order */
	while  (active->to != (NODE *) 0 ) {
		tmp=taketwo(&active,&result);
		active=putone(tmp,active);
	}
	/* now we're done, put the one remaining on the result list */
	result->from=active;
	active->to=result;
	active->from= (NODE *) 0;
	result=active;
	active=(NODE *) 0;



	return result;
}

static NODE *
putone(new, active)
NODE * new, *active;
{
	/* this puts tmp into place in the active list, in order of
	probability.  This relies utterly on the list being sorted already,
	and failure to do so will cause utter chaos*/

	NODE * tmp, *after, *tmpp;

	tmp=active;

	after = (NODE *) 0;
	while ( ( tmp != (NODE *)0 ) &&
	    (new->prob > tmp->prob ) ) {
		after=tmp;
		tmp=tmp->to;
	}
	/* goes until you find an element more probable or you get to the end*/
	/* now, insert tmp into the list, being careful if it's first */
	if (after == (NODE *) 0 ) {
		new->from=(NODE *) 0;
		new->to=active;
		if ( active != (NODE *) 0 ) active->from=new;
		return new;
		/* put it first */
	}
	tmp=after->to;
	/* tmp is now the one to put after the new one */
	if ( tmp != (NODE *) 0 ) tmp->from=new;
	new->to=tmp;
	/* now set up the before node, in variable after.  sheesh! */
	after->to=new;
	new->from=after;
	return active;
}

static NODE *
taketwo(actp,resp)
NODE ** actp, ** resp;
{
	NODE * active, *tmp, *result, * tmp1;

	active = *actp;
	/* temporary active pointer */
	result = *resp;
	/* temp result pointer */
	tmp=mknd();
	/* make new node */
	tmp->ldad=active;
	/* load left father */
	tmp->rdad=active->to;
	/* load right father */
	active->kid=tmp;
	active->to->kid=tmp;
	/* load kid node into fathers */
	tmp->prob=active->prob+active->to->prob;
	/* sum probabilities of fathers into the kid*/
	tmp->datump=(char *) 0;
	/* make sure that there's no character pointer in the kid */
	*actp=active->to->to;
	/* set the new head of the active list */
	if ( *actp != (NODE *) 0 ) (*actp)->from=(NODE *) 0;
	/* make the from of the new active pointer null unless there's
		no longer an active pointer due to end of algorithm*/

	*resp=active->to;
	/* remember which was second in line */
	/* and set the new result pointer */
	active->to=result;
	if ( result != (NODE *) 0 ) result->from=active;
	/* put head of active to head of result. */

	(*resp)->to=active;
	/* point next on result stack to new entry */
	active->from=(*resp);
	(*resp)->from=(NODE *)0;
	return tmp;
	/* return the new, unattached, node */
}

static NODE *
mknd()
{
	NODE *temp;
	if ( (temp=(NODE *)malloc(sizeof(NODE) ) ) == NULL )
	{	
		fprintf(stderr,"could not make NODE");
		exit();
	}
	return temp;
}
static NODE * 
loadall(inrout)
int ( *inrout)();
{
	float tprob;
	char * tdp;
	NODE * tmp, *last,* first, * backp;
	last = (NODE *)0;
	while ( ((*inrout)(&tdp,&tprob) != 0) ) {
		tmp = mknd();
		first=tmp;
		/* first keeps track of what will go into the return value,
													eventually*/
		tmp->to=last;
		if (last != ( NODE * ) 0 ) last->from=tmp;
		last=tmp;
		tmp->datump=tdp;
		tmp->prob=tprob;
		tmp->ldad=tmp->rdad=tmp->kid=(NODE *) 0;
	}
	tmp->from = ((NODE *)0);
	return first;
}
static int 
compar(a,b)
NODE ** a, ** b;
{
	if ( (*a)->prob > (*b)->prob ) return 1;
	if ( (*a)->prob < (*b)->prob ) return -1;
	return 0;
}

