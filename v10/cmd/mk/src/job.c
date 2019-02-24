#include	"mk.h"

Job *
newjob(r, nlist, stem, match, pre, npre, tar, atar)
	Rule *r;
	Node *nlist;
	char *stem;
	regsubexp *match;
	Word *pre, *npre, *tar, *atar;
{
	register Job *j;

	j = (Job *)Malloc(sizeof(Job));
	j->r = r;
	j->n = nlist;
	j->stem = stem;
	j->match = match;
	j->p = pre;
	j->np = npre;
	j->t = tar;
	j->at = atar;
	j->nproc = -1;
	j->fd = -1;
	j->next = 0;
	return(j);
}

dumpj(s, j, all)
	char *s;
	Job *j;
{
	Fprint(1, "%s\n", s);
	while(j){
		Fprint(1, "job@%ld: r=%ld n=%ld stem='%s' nproc=%d\n",
			j, j->r, j->n, j->stem, j->nproc);
		Fprint(1, "\ttarget='%s' alltarget='%s' prereq='%s' nprereq='%s'\n",
			wtos(j->t), wtos(j->at), wtos(j->p), wtos(j->np));
		j = all? j->next : 0;
	}
}
