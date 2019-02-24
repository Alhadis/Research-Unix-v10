#include "defs.h" 
extern char *labsmenutext[];

char inttostr(i)
int i; {
	char tmpstr[5];
	static char str[5];
	int j=0, k=0;

	while (i != 0) {
		tmpstr[j++] = (i%10) + '0';
		i /= 10;
	}
	while (j>0)
		str[k++] = tmpstr[--j];
	str[k] = '\0';
	return(str);
}

euc() {
	eucflag *= -1;
	if (eucflag == 1)
		labsmenutext[5] =  "turn euclidean edge labels off";
	else
		labsmenutext[5] =  "turn euclidean edge labels on";

	if (eucflag == 1)
		euclabs();
}


euclabs() {
	int eucdist(), i, ind, show = 0, d;
	NODE *edgeptr, *endptr;
	char inttostr(), str[5];


	if (showingedgelabs == 1) {
		showedgelabels();
		show = 1;
	}
	for (i=1; i < last; i++) { 
		if (list[i].status == 1) {
			edgeptr = list[i].next;
			while (edgeptr != 0) {
				ind = edgeptr->ind;
				if (ind >= i) {
					d = eucdist(list[i].pt, list[ind].pt);
					strcpy(str, inttostr(d));
					strcpy(edgeptr->wt, str);
				/* put same wt in for edge from other pt */
					endptr = list[ind].next;
					while (endptr != 0 && endptr->ind != i)
						endptr = endptr->next;
					strcpy(endptr->wt, edgeptr->wt);	
					
				}
				edgeptr = edgeptr->next;
				
			}
		}
	}
	if (edgesumflag == 1) {
		Esum();
		showEsum();
	}
	if (show == 1)
		showedgelabels();
}


/* Newton's method */
/*
int eucdist (p1,p2)
Point p1, p2; {

	register int k, l, u, v;

	u = p1.x - p2.x;
	v = p1.y - p2.y;
	k = u*u + v*v;  
	l = u + v;
	l = (l + k/l)>>1;    
	l = (l + k/l)>>1;
	l = (l + k/l)>>1;
	l = (l + k/l)>>1;
	return ((l*l<k) ? ++l : l);

}
*/


int eucdist (p1,p2)
Point p1,p2; {
	register int k, l, u, v;

	u = p1.x - p2.x;
	v = p1.y - p2.y;
	k = u*u + v*v; /* k = hyp ** 2 */
	if (k==0) return(0);
	if (u<0) u = -u;
	if (v<0) v = -v;
	l = u + v;
	l = (l + k/l)>>1; /* rt. shift; divide by 2 */
	l = (l + k/l)>>1;
	l = (l + k/l)>>1;
	l = (l + k/l)>>1;
	return ((l*l<k) ? ++l : l);
}

