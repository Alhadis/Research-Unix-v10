#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

#define	DEBUG

static unsigned char *eg_slowmatch(Br *, unsigned char *, unsigned char *, int);
static unsigned char *wholeb, *wholee;
static unsigned char *start[10];
static int len[10];
static void undobr(Br *);	/* undo group assignements */

eg_match(register re_re *r, register unsigned char *b, register unsigned char *e, unsigned char **rb, unsigned char **re)
{
	int i, ret;

#ifdef	DEBUG
	if(TRACE(2)){
		PR "eg_match(%d->%d, %d, %d)\n", (int)r, (int)r->br, (int)b, (int)e);
		if(r->br)
			eg_brpr(r->br);
	}
#endif
	if((rb == 0) != (re == 0)){
		re_error("must supply both or none of group pointers");
		return(0);
	}
	if(r->backref || r->parens || rb){
		wholeb = e;
		for(i = 1; i < 10; i++)
			start[i] = 0;
		if(r->br == 0)
			egbr(r);
		ret = (wholee = eg_slowmatch(r->br, b, e, RE_BEG|RE_END)) != 0;
		if(rb && ret){
			rb[0] = wholeb;
			re[0] = wholee;
			for(i = 1; i < 10; i++){
				rb[i] = start[i];
				re[i] = rb[i]+len[i];
			}
#ifdef	DEBUG
			if(TRACE(1)){
				PR "eg_match groups:");
				for(i = 0; i < 10; i++)
					if(rb[i])PR " %d: %d@%d", i, rb[i], re[i]-rb[i]);
				PR "\n");
			}
#endif
		}
#ifdef	DEBUG
		 else {
			if(TRACE(1)){
				PR "eg_match groups: [%d - %d]\n", wholeb, wholee);
				for(i = 1; i < 10; i++)
					if(start[i])PR " %d: %d@%d", i, start[i], len[i]);
				PR "\n");
			}
		}
#endif
	} else
		ret = eg_quickmatch(r, b, e, RE_BEG|RE_END) != 0;
	return(ret);
}

static unsigned char *
eg_slowmatch(Br *br, unsigned char *b, unsigned char *e, int endpts)
{
	int i;
	unsigned char *me, *end;
	unsigned char *beg, *lbeg, *llbeg, *rbeg, *rend, *lm, *rm;
#ifdef	DEBUG
	char buf[EPRINTSIZE];
	static id = 1;
	int myid = id++;
#endif

#define		BOFF(x)		((x)? (endpts&~RE_BEG):endpts)
#define		EOFF(x)		((x)? (endpts&~RE_END):endpts)

	if(br == 0)	/* nothing to match - we won! */
		return(b);
#ifdef	DEBUG
	if(TRACE(3))
		PR "slowmatch(br=%d, [b,e]=%d,%d id=%d, endpt=%d)\n", br, b, e, myid, endpts);
#endif
	switch(br->type)
	{
	case br_br:
		i = br->group;
#ifdef	DEBUG
		if(TRACE(3))
			PR "br[%d]: %d,%d b=%d,e=%d\n", i, (int)start[i], len[i], b, e);
#endif
		if(start[i] == 0)
			return(0);
		if((len[i] > e-b) || memcmp((char *)b, (char *)start[i], len[i]))
			return(0);
		if(wholeb > b) wholeb = b;
#ifdef	DEBUG
		if(TRACE(3))
			PR "br[%d]: matched\n", i);
#endif
		return(b+len[i]);

	case br_re:
#ifdef	DEBUG
		if(TRACE(3)){
			eg_epr(br->e, buf, 0);
			PR "matching RE(%s)@%d against '", buf, br->r);
			WR((char *)b, e-b);
			PR "' id=%d\n", myid);
		}
#endif
		if((me = eg_lquickmatch(br->r, b, e, endpts)) == 0)
			return(0);
#ifdef	DEBUG
		if(TRACE(3)){
			PR "--%s matched '", buf);
			WR((char *)b, me-b);
			PR "'[%d %d] id=%d\n", (int)b, (int)me, myid);
		}
#endif
		if(wholeb > b)
			wholeb = b;
		return(me);

	case br_group:
#ifdef	DEBUG
		if(TRACE(3)){
			PR "matching GROUP%d against '", br->group);
			WR((char *)b, e-b);
			PR "' id=%d\n", myid);
		}
#endif
		if((me = eg_slowmatch(br->lb, b, e, endpts)) == 0){
			undobr(br->lb);
			return(0);
		}
#ifdef	DEBUG
		if(TRACE(3)){
			PR "--G%d matched '", br->group);
			WR((char *)b, me-b);
			PR "'[%d %d]\n", (int)b, (int)me);
		}
#endif
		if(wholeb > b)
			wholeb = b;
		start[br->group] = b;
		len[br->group] = me-b;
		return(me);

	case br_quest:
#ifdef	DEBUG
		if(TRACE(3)){
			PR "matching BR? against '", buf);
			WR((char *)b, e-b);
			PR "'\n");
		}
#endif
		if(lbeg = eg_slowmatch(br->lb, b, e, endpts)){
			return(lbeg);
		}
		undobr(br->lb);
		return(b);

	case br_plus:
#ifdef	DEBUG
		if(TRACE(3)){
			PR "matching BR+ against '", buf);
			WR((char *)b, e-b);
			PR "' id=%d\n", myid);
		}
#endif
		if((lbeg = eg_slowmatch(br->lb, b, e, endpts)) == 0){
			undobr(br->lb);
			return(0);
		}
		llbeg = b;
		while(beg = eg_slowmatch(br->lb, lbeg, e, BOFF(lbeg != b))){
			llbeg = lbeg, lbeg = beg;
		}
#ifdef	DEBUG
		if(TRACE(3)){
			PR "--+ matched [%d %d]'", (int)llbeg, (int)lbeg);
			WR((char *)llbeg, lbeg-llbeg);
			PR "' id=%d\n", myid);
		}
#endif
		return(eg_slowmatch(br->lb, llbeg, e, BOFF(llbeg != b)));

	case br_star:
#ifdef	DEBUG
		if(TRACE(3)){
			PR "matching BR* against '", buf);
			WR((char *)b, e-b);
			PR "'\n");
		}
#endif
		llbeg = lbeg = b;
		while(beg = eg_slowmatch(br->lb, lbeg, e, BOFF(lbeg != b)))
			llbeg = lbeg, lbeg = beg;
#ifdef	DEBUG
		if(TRACE(3)){
			PR "--* matched '");
			WR((char *)lbeg, lbeg-llbeg);
			PR "'[%d %d]\n", (int)llbeg, (int)lbeg);
		}
#endif
		if(beg = eg_slowmatch(br->lb, llbeg, e, BOFF(llbeg != b)))
			return(beg);
		undobr(br->lb);
		return(b);

	case br_cat:
#ifdef	DEBUG
		if(TRACE(3)){
			PR "matching BRcat against '", buf);
			WR((char *)b, e-b);
			PR "' id=%d\n", myid);
		}
#endif
		/*
			this is not so hard.
			we try all possible matches of the left half,
			and record the match that gave the longest
			valid match on the right half
		*/
		rend = 0;
		for(end = e; b <= e; e = beg-1){
			if((beg = eg_slowmatch(br->lb, b, e, EOFF(e != end))) == 0){
				break;
			}
#ifdef	DEBUG
			if(TRACE(3)){
				PR "--cat matched '");
				WR((char *)b, beg-b);
				PR "'[%d %d] id=%d\n", (int)b, (int)beg, myid);
			}
#endif
			if((me = eg_slowmatch(br->rb, beg, end, BOFF(beg != b))) == 0){
				continue;	/* no match of right half */
			}
#ifdef	DEBUG
			if(TRACE(3)){
				PR "----cat matched '");
				WR((char *)b, beg-b);
				PR "'[%d %d] id=%d\n", (int)b, (int)beg, myid);
			}
#endif
			if(me > rend){
				rend = me;
				rbeg = beg;
#ifdef	DEBUG
				if(TRACE(3)){
					PR "--++-- cat new max rb=%d re=%d\n", (int)rbeg, (int)rend);
				}
#endif
			}
		}
		if(rend == 0){
			undobr(br->lb);
			undobr(br->rb);
			return(0);
		}
		(void)eg_slowmatch(br->lb, b, rbeg, EOFF(rbeg != end));
		return(eg_slowmatch(br->rb, rbeg, end, BOFF(rbeg != b)));

	case br_alt:
#ifdef	DEBUG
		if(TRACE(3)){
			PR "matching BR| against '", buf);
			WR((char *)b, e-b);
			PR "'\n");
		}
#endif
		if(lm = eg_slowmatch(br->lb, b, e, endpts)){
#ifdef	DEBUG
			if(TRACE(3)){
				PR "--|L matched '");
				WR((char *)b, lm-b);
				PR "'[%d %d]\n", (int)b, (int)lm);
			}
#endif
		}
		if(rm = eg_slowmatch(br->rb, b, e, endpts)){
#ifdef	DEBUG
			if(TRACE(3)){
				PR "--|R matched '");
				WR((char *)b, rm-b);
				PR "'[%d %d]\n", (int)b, (int)rm);
			}
#endif
		}
		if(lm > rm){
			undobr(br->rb);
			return(eg_slowmatch(br->lb, b, e, endpts));
		} else {
			if(rm == 0){
				undobr(br->lb);
				undobr(br->rb);
				return(0);
			} else {
				undobr(br->lb);
				return(beg);
			}			
		}
	}
	abort();
	return(0);
}

static void
undobr(register Br *br)
{
	switch(br->type)
	{
	case br_group:
		start[br->group] = 0;
		undobr(br->lb);
		break;
	case br_star:
	case br_plus:
	case br_quest:
		undobr(br->lb);
		break;
	case br_cat:
	case br_alt:
		undobr(br->lb);
		undobr(br->rb);
		break;
	}
}
