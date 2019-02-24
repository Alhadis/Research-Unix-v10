static char ID[] = "@(#) lists.c: 1.3 5/27/83";
#include "system.h"

#include "list.h"
#include "structs.h"
#include "sgsmacros.h"

#define NULL 0
/*eject*/
listadd(seqid, ownerp, memp)
int seqid;			/* sequence id */
LISTOWN *ownerp;		/* ptr to owner structure */
char *memp;			/* ptr to member structure */
{

/*
 * Add a member structure to a linked list.
 *
 * This function is implemented as one function in case a general
 * Storage Management System capability ever becomes available.
 *
 * Currently, the l_AI, l_IF, l_IS, l_OS, and l_INC lists are
 * implemented as queues. The l_REG, l_MEM, and l_ADR lists are
 * ordered lists.
 */

	switch (seqid) {

	case l_AI:
		if (ownerp->tail == NULL)
			ownerp->head = memp;
		else
			((ACTITEM *) ownerp->tail)->ldlbry.ainext = (ACTITEM *) memp;
		ownerp->tail = memp;
		((ACTITEM *) memp)->ldlbry.ainext = NULL;
		break;
	case l_IF:
		if (ownerp->tail == NULL)
			ownerp->head = memp;
		else
			((INFILE *) ownerp->tail)->flnext = (INFILE *) memp;
		ownerp->tail = memp;
		((INFILE *) memp)->flnext = NULL;
		break;
	case l_IS:
		if (((INFILE *) ownerp)->flishead == NULL)
			((INFILE *) ownerp)->flishead = (INSECT *) memp;
		else
			((INFILE *) ownerp)->flistail->isnext = (INSECT *) memp;
		((INFILE *) ownerp)->flistail = (INSECT *) memp;
		((INSECT *) memp)->isnext = NULL;
		((INSECT *) memp)->isfilptr = (INFILE *) ownerp;
		break;
	case l_OS:
		if (ownerp->head == NULL)
			ownerp->head = memp;
		else
			((OUTSECT *) ownerp->tail)->osnext = (OUTSECT *) memp;
		ownerp->tail = memp;
		((OUTSECT *) memp)->osnext = NULL;
		break;
	case l_INC:
		if (((OUTSECT *) ownerp)->osinclhd == NULL)
			((OUTSECT *) ownerp)->osinclhd = (INSECT *) memp;
		else
			((OUTSECT *) ownerp)->osincltl->isincnxt = (INSECT *) memp;
		((OUTSECT *) ownerp)->osincltl = (INSECT *) memp;
		((INSECT *) memp)->isincnxt = NULL;
		((INSECT *) memp)->isoutsec = (OUTSECT *) ownerp;
		break;
	case l_GRP:
		if (((OUTSECT *) ownerp)->osinclhd == NULL)
			((OUTSECT *) (((OUTSECT *) ownerp)->osinclhd)) = (OUTSECT *) memp;
		else
			((OUTSECT *) (((OUTSECT *) ownerp)->osincltl))->osnext = (OUTSECT *) memp;
		((OUTSECT *) ownerp)->osincltl = (INSECT *) memp;
		((OUTSECT *) memp)->osnext = NULL;
		break;
	case l_REG:
		/*
		 * REGIONS list is ordered on address
		 */
		if (ownerp->head == NULL) {
			ownerp->head = memp;
			ownerp->tail = memp;
			}
		else {
			REGION *rp, *prevrp;
			rp = (REGION *) ownerp->head;
			prevrp = NULL;
			while( rp ) {
				if(rp->rgorig > ((REGION *) memp)->rgorig){
					if (prevrp)
						prevrp->rgnext = (REGION *) memp;
					else
						ownerp->head = memp;
					((REGION *) memp)->rgnext = rp;
					return;
					}
				prevrp = rp;
				rp = rp->rgnext;
				}
			prevrp->rgnext = (REGION *) memp;
			ownerp->tail = memp;
			}
		break;
	case l_MEM:
		/*
		 * MEMORY list is ordered on address
		 */
		if (ownerp->head == NULL)  {
			ownerp->head = memp;
			ownerp->tail = memp;
			((MEMTYPE *) memp)->mtnext = NULL;
			}
		else {
			MEMTYPE *mp, *prevmp;
			mp = (MEMTYPE *) ownerp->head;
			prevmp = NULL;
			while( mp ) {
				if(mp->mtorig > ((MEMTYPE*)memp)->mtorig) {
					if( prevmp )
						prevmp->mtnext = (MEMTYPE *) memp;
					else
						ownerp->head = memp;
					((MEMTYPE *) memp)->mtnext = mp;
					return;
					}
				prevmp = mp;
				mp = mp->mtnext;
				}
			prevmp->mtnext = (MEMTYPE *) memp;
			ownerp->tail = memp;
			}
		break;
	case l_ADR:
		/*
		 * ADDR NODE list is ordered on address
		 */
		if(ownerp->head == NULL) {
			ownerp->head = ownerp->tail = memp;
			((ANODE *) memp)->adprev = ((ANODE *) memp)->adnext = NULL;
			}
		else {
			ANODE *p;
			p = (ANODE *) ownerp->head;
			while( p ) {
				if(p->adpaddr >= ((ANODE *) memp)->adpaddr){
					if( p->adprev )
						p->adprev->adnext = (ANODE *) memp;
					else
						ownerp->head = memp;
					((ANODE *) memp)->adprev = p->adprev;
					((ANODE *) memp)->adnext = p;
					p->adprev = (ANODE *) memp;
					return;
					}
				p = p->adnext;
				}
			p = (ANODE *) ownerp->tail;
			ownerp->tail = memp;
			((ANODE *) memp)->adnext = NULL;
			((ANODE *) memp)->adprev = p;
			p->adnext = (ANODE *) memp;
			}
		break;
	case l_DS:
		if (ownerp->tail == NULL)
			ownerp->head = memp;
		else
			((OUTSECT *) ownerp->tail)->osdsnext = (OUTSECT *) memp;
		ownerp->tail = memp;
		((OUTSECT *) memp)->osdsnext = NULL;
		break;
	}
}
/*eject*/
listins(seqid, ownerp, basep, memp)
int seqid;			/* sequence id */
LISTOWN *ownerp;		/* ptr to owner structure */
char *basep;			/* ptr to insertion point in sequence */
char *memp;			/* ptr to member structure */
{

/*
 * Insert a member structure into a linked list, after a specified
 * existing list member.
 *
 * If the insertion member pointer (= basep) is NULL, then the list
 * is empty, and the new member will become the first list element
 *
 * This function is implemented as one function in case a general
 * Storage Management System capability ever becomes available.
 *
 * Currently, the l_AI, l_IF, l_IS, l_OS, and l_INC lists are
 * implemented as queues. The l_REG, l_MEM, and l_ADR lists are
 * ordered lists.
 */

	if( basep == NULL ) {
		listadd(seqid, ownerp, memp);
		return;
		}

	switch (seqid) {

	case l_AI:
		((ACTITEM *) memp)->ldlbry.ainext = ((ACTITEM *) basep)->ldlbry.ainext;
		((ACTITEM *) basep)->ldlbry.ainext = (ACTITEM *) memp;
		if( ownerp->tail == basep )
			ownerp->tail = memp;
		break;
	case l_IF:
		((INFILE *) memp)->flnext = ((INFILE *) basep)->flnext;
		((INFILE *) basep)->flnext = (INFILE *) memp;
		if( ownerp->tail == basep )
			ownerp->tail = memp;
		break;
	case l_IS:
		((INSECT *) memp)->isnext = ((INSECT *) basep)->isnext;
		((INSECT *) basep)->isnext = (INSECT *) memp;
		if( ((INFILE *) ownerp)->flistail == ((INSECT *) basep) )
			((INFILE *) ownerp)->flistail = (INSECT *) memp;
		break;
	case l_OS:
		((OUTSECT *) memp)->osnext = ((OUTSECT *) basep)->osnext;
		((OUTSECT *) basep)->osnext = (OUTSECT *) memp;
		if( ownerp->tail == basep )
			ownerp->tail = memp;
		break;
	case l_INC:
		((INSECT *) memp)->isincnxt = ((INSECT *) basep)->isincnxt;
		((INSECT *) basep)->isincnxt = (INSECT *) memp;
		if( ((OUTSECT *) ownerp)->osincltl == ((INSECT *) basep) )
			((OUTSECT *) ownerp)->osincltl = (INSECT *) memp;
		break;
	case l_GRP:
		((OUTSECT *) memp)->osnext = ((OUTSECT *) basep)->osnext;
		((OUTSECT *) basep)->osnext = ((OUTSECT *) memp);
		if( ((OUTSECT *) ownerp)->osincltl == ((INSECT *) basep) )
			((OUTSECT *) ownerp)->osincltl = ((INSECT *) memp);
		break;
	case l_REG:
		((REGION *) memp)->rgnext = ((REGION *) basep)->rgnext;
		((REGION *) basep)->rgnext = ((REGION *) memp);
		if( ownerp->tail == basep )
			ownerp->tail = memp;
		break;
	case l_MEM:
		((MEMTYPE *) memp)->mtnext = ((MEMTYPE *) basep)->mtnext;
		((MEMTYPE *) basep)->mtnext = ((MEMTYPE *) memp);
		if( ownerp->tail == basep )
			ownerp->tail = memp;
		break;
	case l_ADR:
		((ANODE *) memp)->adnext = ((ANODE *) basep)->adnext;
		((ANODE *) basep)->adnext = ((ANODE *) memp);
		((ANODE *) memp)->adprev = ((ANODE *) basep);
		if (((ANODE *) memp)->adnext)
			((ANODE *) memp)->adnext->adprev = ((ANODE *) memp);
		if( ownerp->tail == basep )
			ownerp->tail = memp;
		break;
	case l_DS:
		((OUTSECT *) memp)->osdsnext = ((OUTSECT *) basep)->osdsnext;
		((OUTSECT *) basep)->osdsnext = (OUTSECT *) memp;
		if( ownerp->tail == basep )
			ownerp->tail = memp;
		break;
	}
}
/*eject*/
listdel(seqid, headp, prevp, memp)
int seqid;		/* sequence id */
LISTOWN *headp;		/* ptr to head-of-list structure */
char *prevp;		/* ptr to member preceeding the member to delete +/
char *memp;		/* ptr to member to delete */
{

/*
 * Delete a member structure from a linked list
 *
 * If the member structure is null, this function is a noop
 *
 * This function is implemented as one function in case a general
 * Storage Management System capability ever becomes available.
 *
 * Currently, the l_AI, l_IF, l_IS, l_OS, and l_INC lists are
 * implemented as queues. The l_REG, l_MEM, and l_ADR lists are
 * ordered lists.
 */

	if( memp == NULL )
		return;

	switch( seqid ) {

	case l_AI:
		if( prevp )
			((ACTITEM *) prevp)->ldlbry.ainext = ((ACTITEM *) memp)->ldlbry.ainext;
		else
			headp->head = (char *) ((ACTITEM *) memp)->ldlbry.ainext;
		if( headp->tail == (char *) memp )
			headp->tail = prevp;
		break;
	case l_IF:
		if( prevp )
			((INFILE *) prevp)->flnext = ((INFILE *) memp)->flnext;
		else
			headp->head = (char *) ((INFILE *) memp)->flnext;
		if( headp->tail == (char *) memp )
			headp->tail = prevp;
		break;
	case l_IS:
		if( prevp )
			((INSECT *) prevp)->isnext = ((INSECT *) memp)->isnext;
		else
			((INFILE*) headp)->flishead = ((INSECT *) memp)->isnext;
		if( ((INFILE *) headp)->flistail == ((INSECT *) memp) )
			((INFILE *) headp)->flistail = (INSECT *) prevp;
		break;
	case l_OS:
		if( prevp )
			((OUTSECT *) prevp)->osnext = ((OUTSECT *) memp)->osnext;
		else
			headp->head = (char *) ((OUTSECT *) memp)->osnext;
		if( headp->tail == (char *) memp )
			headp->tail = prevp;
		break;
	case l_INC:
		if( prevp )
			((INSECT *) prevp)->isincnxt = ((INSECT *) memp)->isincnxt;
		else
			((OUTSECT *) headp)->osinclhd = ((INSECT *) memp)->isincnxt;
		if( ((OUTSECT *) headp)->osincltl == ((INSECT *) memp) )
			((OUTSECT *) headp)->osincltl = (INSECT *) prevp;
		break;
	case l_GRP:
		if( prevp )
			((OUTSECT *) prevp)->osnext = ((OUTSECT *) memp)->osnext;
		else
			((OUTSECT *) headp)->osinclhd = ((INSECT *) ((OUTSECT *) memp)->osnext);
		if( ((OUTSECT *) headp)->osincltl == ((INSECT *) memp) )
			((OUTSECT *) headp)->osincltl = ((INSECT *) prevp);
		break;
	case l_REG:
		if( prevp )
			((REGION *) prevp)->rgnext = ((REGION *) memp)->rgnext;
		else
			headp->head = (char *) ((REGION *) memp)->rgnext;
		if( headp->tail == (char *) memp )
			headp->tail = prevp;
		break;
	case l_MEM:
		if( prevp )
			((MEMTYPE *) prevp)->mtnext = ((MEMTYPE *) memp)->mtnext;
		else
			headp->head = (char *) ((MEMTYPE *) memp)->mtnext;
		if( headp->tail == (char *) memp )
			headp->tail = prevp;
		break;
	case l_ADR:
		if( prevp )
			((ANODE *) prevp)->adnext = ((ANODE *) memp)->adnext;
		else
			headp->head = (char *) ((ANODE *) memp)->adnext;
		if( headp->tail == (char *) memp )
			headp->tail = prevp;
		else
			((ANODE *) memp)->adnext->adprev = (ANODE *) prevp;
		break;
	case l_DS:
		if( prevp )
			((OUTSECT *) prevp)->osdsnext = ((OUTSECT *) memp)->osdsnext;
		else
			headp->head = (char *) ((OUTSECT *) memp)->osdsnext;
		if( headp->tail == (char *) memp )
			headp->tail = prevp;
		break;
	}
}
