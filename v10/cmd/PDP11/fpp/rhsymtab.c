/*
 *  mjm: calloc() -> kalloc() so no conflict with Unix calloc();
 *	and kalloc() calls malloc; also new OUT
 */
#define UNDEF (undefin)
#define NOTFOUND (-1)
#define OUT (0)		/* mjm: out of storage for malloc() */
#define PUTOK 1
#define PUTBAD 0
#define SYNAMEL 8
#define AREASIZN (504/sizeof(dummynode))
#define AREASIZE sizeof(**ixb)
static struct node {
	int lefta,leftn;
	int righta,rightn;
	int value;
	char nn[SYNAMEL];
	} dummynode;
static struct area {
	int areaname;
	int altered;
	struct node nodes[AREASIZN];
	};

struct head {
	int nodenew,areanew,lvalid;
	int lstart;
	struct head *lnext;
	};

static struct area *buff,**ixb;
static struct head *lhead;
static int fp;
static int oldlv;
/* the value to return for undefined symbol, set in rhccomp.c */
extern int undefin;
extern char *symtout;
extern int outcore;

/* return the value of a symbol s, starting from area a */
static getval(s,a)
struct area *a;
char *s;
{
int n,c,at;
	n=0;
	while(c=sycomp(s,a->nodes[n].nn)) {
		if(c<0) {
			if((at=a->nodes[n].lefta)==0) return(NOTFOUND);
			n=a->nodes[n].leftn;
			}
		else {
			if((at=a->nodes[n].righta)==0) return(NOTFOUND);
			n=a->nodes[n].rightn;
			}
		if(at!=a->areaname) a=fetcha(at);
		}
	return(a->nodes[n].value);
}

/* define a symbol s with value v starting in area a */
static putval(s,a,v)
int v;
struct area *a;
char *s;
{
int n,c,at;
	n=0;
	while(c=sycomp(s,a->nodes[n].nn)) {
		if(c<0) {
			if((at=a->nodes[n].lefta)==0) {
				return(putlink(s,a,v,n,'l'));
				}
			n=a->nodes[n].leftn;
			}
		else {
			if((at=a->nodes[n].righta)==0) {
				return(putlink(s,a,v,n,'r'));
				}
			n=a->nodes[n].rightn;
			}
		if(at!=a->areaname) a=fetcha(at);
		}
	return(PUTBAD);
}

/* initial call here: set up size in bytes of symtab buffers */
symtab(size)
int size;
{
int i;
	buff=kalloc(size);
	ixb=kalloc(2*sizeof(buff)+(sizeof(buff)*size)/AREASIZE);	/* mjm: 2->sizeof */
	if((fp=creat(symtout,0666))<0) abt("can't create file for symtab");
	close(fp);
	if((fp=open(symtout,2))<0) abt("can't re-open file for symtab");
	lhead=kalloc(sizeof(*lhead));
	lhead->lnext = lhead->nodenew = lhead->lvalid = 0;
	lhead->lstart=lhead->areanew=1;
	oldlv=0;
	for(i=0; AREASIZE*(i+1)<=size; i++) {
		ixb[i]= &buff[i];
		buff[i].areaname=0; buff[i].altered=0;
		}
	ixb[i]=0;
}

/* define the current level of symbol definition */
definelv(lv)
int lv;
{
struct head *lnew;
if(lv>oldlv) {
	lnew=kalloc(sizeof(*lhead));
	lnew->lnext=lhead;
	if(lhead->nodenew) {
		lnew->areanew=lhead->areanew+1;
		}
	else {
		lnew->areanew=lhead->areanew;
		}
	lnew->nodenew=lnew->lvalid=0;
	lnew->lstart=lnew->areanew;
	lhead=lnew;
	}
if(lv<oldlv) {
	lnew=lhead->lnext;
	free(lhead);
	lhead=lnew;
	}
oldlv=lv;
}

/* define a symbol */
definesy(s,v)
char *s;
int v;
{
struct area *a;
	if(lhead->lvalid) return(putval(s,fetcha(lhead->lstart),v));
	else {
		lhead->lvalid=1;
		lhead->nodenew=1;
		a=fetcha(lhead->lstart);
		return(putnv(s,a,v,0));
		}
}

/* get a symbol value at current level or below */
gettype(s)
char *s;
{
struct head *h;
int i;
	h=lhead;
	while(h) {
		if(h->lvalid)
			if((i=getval(s,fetcha(h->lstart)))!=NOTFOUND)return(i);
		h=h->lnext;
		}
	return(UNDEF);
}
static putnv(s,a,v,n) /* put a new node at n in a */
int v,n;
struct area *a;
char *s;
{
struct node *nq;
int i;
char *s2;
	a->altered=1;
	s2=(nq= &a->nodes[n])->nn;
	i=0;
	while(i++<SYNAMEL&&(*s2++= *s++));
	nq->lefta=nq->righta=0;
	nq->value=v;
	return(PUTOK);
}

static putlink(s,a,v,n,lr)	/* put new node and link to ... */
char *s;
struct area *a;
int v,n,lr;
{
	a->altered=1;
	if(lr=='l') {
		a->nodes[n].leftn=lhead->nodenew;
		a->nodes[n].lefta=lhead->areanew;
		}
	else {
		a->nodes[n].rightn=lhead->nodenew;
		a->nodes[n].righta=lhead->areanew;
		}
        a=fetcha(lhead->areanew);
	putnv(s,a,v,lhead->nodenew);
	if(++lhead->nodenew>=AREASIZN) {
		lhead->nodenew=0;
		lhead->areanew++;
		}
	return(PUTOK);
}

/* check out strings */
static sycomp(s1,s2)
char *s1,*s2;
{
int i;
for(i=0; i<SYNAMEL&&(*s1||*s2); i++)
	if(*s1++!= *s2++)
		return(*--s1- *--s2);
return(0);
}

/* dynamic disk reader-writer */
static fetcha(ta)
int ta;
{
struct area *it;
int i;
long lonk;
	for(i=0; it=ixb[i]; i++)
		if(it->areaname==ta) break;
	if(it==0) {
		it= ixb[--i];
		if(it->altered) {
			lseek(fp,(lonk=it->areaname)<<9,0);
			write(fp,it,AREASIZE);
			}
		lseek(fp,(lonk=ta)<<9,0);
		read(fp,it,AREASIZE);
		it->altered=0;
		it->areaname=ta;
		}
        while(i-->0) ixb[i+1]=ixb[i];
	return(*ixb=it);
}

static kalloc(sz)
int sz;
{	/* mjm: made malloc arg always even */
if((sz=malloc((sz+1)&~01)) != OUT)
	return(sz);
abt("out of storage in symtab");
}
