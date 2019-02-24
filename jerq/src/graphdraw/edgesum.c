#include "defs.h"
#include <font.h>
extern char *labsmenutext[];

int strtoint(str)
char str[]; {
	int i=0, j=0;

	while (str[j] != '\0') 
		if ((str[j] < '0') || (str[j] > '9'))
			/* then it's not a digit, nullify...*/
			return(-1);
		else
			i = (str[j++] - '0') + i*10;
	return(i);
}

edgesum() {
	edgesumflag *= -1;
	if (edgesumflag == 1) {
		labsmenutext[6] = "hide sum of edges";
		Esum();
		showEsum();
	}
	else {
		labsmenutext[6] = "show sum of edges";
		rectf(&display, box, f1);
	}
}

addtoEsum(wt)
char wt[]; {
	int num;

	if ((num = strtoint(wt)) != -1)
		edgewtsum += num; 
	else
	/* not all edge wts are nos. */
		edgewtsum = -1;
	showEsum();
}
	

subtfromEsum(wt)
char wt[]; {
	int num;

	if ((num = strtoint(wt)) != -1)
		edgewtsum -= num; 
	else
	/* not all edge wts are nos. */
		edgewtsum = -1;
	showEsum();
}

	
/* calculate edgewtsum */
Esum() {
	int i, num, strtoint();
	NODE *edgeptr;

	edgewtsum = 0; /*reinit*/
	for (i = 1; i < last; i++) {
		if (list[i].status == 1) {
			edgeptr = list[i].next;
			while (edgeptr != 0) {
				if (edgeptr->ind >= i)
					if ((num = strtoint(edgeptr->wt)) != -1)
						edgewtsum += num; 
					else {
					/* not all edge wts are nos. */
						edgewtsum = -1;
						return;
					}
				edgeptr = edgeptr->next;
			}
		}
	}

}

showEsum() {
	char str[MXLINE];

	rectf(&display, box, f1);
	if (edgewtsum == -1)
		sprintf(str, "can't compute sum of edges");
	else
		sprintf(str, "sum of edges is %d", edgewtsum);
	string(&defont, str, &display, boxstart, f2);
}
