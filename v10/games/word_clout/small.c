#define AMBIGUOUS 1
#define STDERR stdout
#include <stdio.h>
#include <ctype.h>
#include "./regexp.h"
regexp *regcomp(),*rs;
int nwanted,wantps;
int accept();
char *malloc();
#define freecol(x) (col[x].next=colfree,colfree=x)
#define getcol(x) ((x=colfree)?(colfree=col[colfree].next):(x=morecol()))
#define NHS 114723	/* number of headwords and senses */
#define N 400	/* max line length */
#define INITTEXT (516673+1)	/* bytes of headword text */
#define INITSQ 1980	/* bytes of sense qualifier text */
#define INITSQTAB 1063	/*  #qual sense addr with text addr */
#define NWINDEX 514104	/* number of 17 bit references */
#define X 1	/* extra bit for ref qualified */
#define WADDR 17	/* bits in headword or sense addr */
#define WMASK ((1<<WADDR)-1)
#define WQ (1<<WADDR)
#define WBITS (WADDR+X)
#define WQTEXT 2518	/* bytes of ref qualifier text */
#define NWQ 11582	/* #qual ref addr with text addr */
#define WQT 207	/* #tree entries for ref qual sort */
#define NCOL 1024	/* intermediate senses in path search */
struct col {
	unsigned int entry:17;
	unsigned int next:15;
} *col;
int colfree;
FILE *f;
FILE *wpackfile;
union x {
	struct headword {
		unsigned int type:1;
		unsigned int tt:19;
		unsigned int v:12;
	} h;
	struct sense {
		unsigned int type:1;
		unsigned int st:19;
		unsigned int p:4;
		unsigned int q:1;
		unsigned int v:7;
	} s;
} p[NHS+2];
char tbase[INITTEXT];
int text;
int z;
char in[N];
struct squal {
	unsigned int saddr:17;
	unsigned int qaddr:11;
	unsigned int v:4;
} sqtab[INITSQTAB];
int nsq;
char sqbase[INITSQ];
int sqtext;
int windex;
unsigned char arena[(NWINDEX*WBITS)/8+1];
struct wqual {
	unsigned int waddr:19;
	unsigned int qaddr:12;
} wqtab[NWQ];
int nwq;
char wqbase[WQTEXT];
int wqtext;
struct wqt {
	unsigned left:8,right:8,qaddr:12;
} wqtree[WQT];
int wqt;
#define NSET 32
int set[NSET];
int ncol;
char *parts[] = {
	"","n","v","adj","adv","pron","conj","prep","interj"
};
#define NP 9
inset(t)
char *t;
{
	char y[128];
	int i,j,k;
	if(*t!='&') {
		i=headw(t);
		if(i==0)return 0;
		for(j=0; j<NSET; j++)if(p[i+j+1].s.type==0||i+j+1>=z)break;
			else set[j]=i+j+1;
		return j;
	}
	if(isdigit(*++t)) {
		for(k=0; isdigit(*t); t++)k=k*10+*t-'0';
		i=headw(t);
		if(i==0)return 0;
		for(j=0; j<NSET; j++)if(p[i+j+1].s.type==0||i+j+1>=z)break;
			else if(j+1==k) {
				set[0]=i+j+1;
				return 1;
			}
		return 0;
	}
	if(strlen(t)>126)err("string to long in inset()");
	strcpy(y,t);
	for(j=0; y[j]&&y[j]!='.'; j++);
	if(y[j]==0)return 0;
	y[j]=0;
	for(k=1; k<NP; k++)if(strcmp(y,parts[k])==0)break;
	if(k>=NP)return 0;
	i=headw(y+j+1);
	if(i++==0)return 0;
	for(j=0; j<NSET&&i<=z&&p[i].s.type==1; i++) {
		if(p[i].s.p==k)set[j++]=i;
	}
	return j;
}
readnum()
{
	int j,k;
	for(k=0; isdigit(j=getc(f)); k=k*10+j-'0');
	if(j!='\n')err("expect number");
	fprintf(STDERR,"readnum()=%d\n",k);
	return k;
}
sensequal(zz,t)
char *t;
{
	int i;
	if(p[zz].h.type!=1)err("squal on headword");
	if(*t==0)return;
	p[zz].s.q=1;
	sqtab[nsq].saddr=zz;
	for(i=0; i<nsq; i++)if(strcmp(t,sqbase+sqtab[i].qaddr)==0)break;
	if(i<nsq) {
		sqtab[nsq].qaddr=sqtab[i].qaddr;
	}
	else {
		sqtab[nsq].qaddr=sqtext;
		strcpy(sqbase+sqtext,t);
		sqtext+=strlen(t)+1;
		if(sqtext>=INITSQ)err("sqtext overflow");
	}
	if(++nsq>INITSQTAB)err("nsq overflow");
}
err(s)
char *s;
{
	int i;
	fprintf(STDERR,"err: %s\n",s);
	fprintf(STDERR,"type return:"); getchar();
	fprintf(STDERR,"input buffer segments:\n");
	for(i=0; i<N; i++) {
		if(in[i]==0)continue;
		fprintf(STDERR,"'%s'\n",in+i);
		while(i<N&&in[i])i++;
	}
	exit(1);
}
char *squal(zz)
{
	int il,im,iu;
	if(p[zz].h.type==0) {
		err("ask for sensqual on headword");
	}
	if(p[zz].s.q==0)return "";
	il=0; iu=nsq;
	while(il<iu-1) {
		im=(il+iu)/2;
		if(sqtab[im].saddr>zz) {
			iu=im;
		}
		else il=im;
	}
	if(sqtab[il].saddr!=zz)err("binary botch in squal()");
	return sqbase+sqtab[il].qaddr;
}
char *wqual(wi)
{
	int il,im,iu;
	if((unpackw(wi)&WQ)==0)return "";
	il=0; iu=nwq;
	while(il<iu-1) {
		im=(il+iu)/2;
		if(wqtab[im].waddr>wi) {
			iu=im;
		}
		else il=im;
	}
	if(wqtab[il].waddr!=wi)err("binary botch wqual");
	return wqbase+wqtab[il].qaddr;
}
opackw(index,value)
register value;
{
	int i,j;
	register int mask=(1<<WBITS)-1;
	i=index*WBITS;
	value<<=i&7;
	mask<<=i&7;
	j=i>>3;
	while(mask&0xff) {
		arena[j]&=~mask;
		arena[j]|=value;
		value>>=8;
		mask>>=8;
		j++;
	}
}
packw(index,value)
register value;
{
	register i;
	register unsigned char *p;
	i=index*18;
	value<<=i&7;
	p=arena+(i>>3);
	*p++|=value; value>>=8; *p++|=value; value>>=8;
	*p++|=value;
}
unpackw(index)
{
	register i,k;
	register unsigned char *p;
	i=index*18;
	p=arena+(i>>3)+2;
	k=*p; k=(k<<8)|*--p; k=(k<<8)|*--p; 
	return (k>>(i&7))&0x3ffff;
}
ounpackw(index)
{
	int i,j,k;
	int x;
	register int mask=(1<<WBITS)-1;
	i=index*WBITS;
	j=i>>3;
	mask<<=i&7;
	x=k=0;
	while(mask&0xff) {
		k|=(arena[j]&mask)<<x;
		mask>>=8;
		j++;
		x+=8;
	}
	return k>>(i&7);
}
dumps(x)
{
	int i,j,k,l,m,n;
	if(x==0) {
		fprintf(STDERR,"not found\n");
		return;
	}
	for(i=x; i>0&&p[i].h.type!=0; i--);
	if(i==0) {
		fprintf(STDERR,"dump of %d no headword\n",x);
		return;
	}
	fprintf(STDERR,"\n&%d%s ",x-i,p[i].h.tt+tbase);
	if(p[x].h.type==0) {
		fprintf(STDERR,"\n");
		while(++x<z&&p[x].h.type!=0)dumps(x);
		return;
	}
	fprintf(STDERR," (%s.)",parts[p[x].s.p]);
	if(*squal(x))fprintf(STDERR," [%s]",squal(x));
	fprintf(STDERR,":\n");
	for(i=p[x].s.st; l=unpackw(i++); ) {
		for(j=l&WMASK; j>0&&p[j].h.type!=0; j--);
		fprintf(STDERR," &%d%s",(l&WMASK)-j,p[j].h.tt+tbase);
		if(l&WQ)fprintf(STDERR," [%s]",wqual(i-1));
		fprintf(STDERR,",");
	}
	fprintf(STDERR,"\n");
}
char *ma(x)
{
	char *p;
	p=(char*)calloc(x,1);
	if(p==0) {
		fprintf(STDERR,"can't calloc %d bytes\n",x);
		err("quit");
	}
	return p;
}
wsym(q,node)
char *q;
{
	int i;
	if(wqt==0) {
		strcpy(wqbase,q);
		wqtext=strlen(q)+1;
		wqt++;
		return 0;
	}
	i=strcmp(q,wqbase+wqtree[node].qaddr);
	if(i==0)return wqtree[node].qaddr;
	if(i<0) {
		if(wqtree[node].left)return wsym(q,wqtree[node].left);
		wqtree[node].left=wqt;
	}
	else {
		if(wqtree[node].right)return wsym(q,wqtree[node].right);
		wqtree[node].right=wqt;
	}
	strcpy(wqbase+wqtext,q);
	i=wqtext;
	wqtext+=strlen(q)+1;
	if(wqtext>=WQTEXT)err("wqtext ovfl");
	wqtree[wqt++].qaddr=i;
	if(wqt>=WQT)err("wqt ovfl");
	return i;
}
headw(text)
char *text;
{
	int il,iu,im;
	il=1; iu=z;
	while(il<iu+1) {
		im=(il+iu)/2;
		while(im>il&&p[im].h.type!=0)im--;
		if(im<=il) {
			while(++im<iu&&p[im].h.type!=0);
			if(im>=iu)break;
		}
		if(strcmp(p[im].h.tt+tbase,text)>0) {
			iu=im;
		}
		else {
			il=im;
		}
	}
	if(strcmp(p[il].h.tt+tbase,text))return 0;
	return il;
}
morecol()
{
	int i;
	if(col==0)col=(struct col*)ma(NCOL*sizeof*col);
	else col=(struct col*)realloc(col,(ncol+NCOL)*sizeof*col);
	for(i=NCOL+ncol-1; i>ncol; i--)col[i].next=i-1;
	col[ncol].next=0;
	ncol+=NCOL;
	colfree=ncol-2;
	return ncol-1;
}
path(a,b)
char *a,*b;
{
	int i,j,k;
	int old,new;
	int e;
	unsigned char psa[NP],psb[NP];
	int it;
	int pf;
	for(i=0; b[i]; i++)if(b[i]=='/') {
		nwanted=10;
		rsparse(b);
		qpath(accept,a);
		fprintf(STDERR,"\n");
		return;
	}
#ifdef AMBIGUOUS
	if(strncmp(a,"ambiguous ",10)==0) {
		ambpath(a+10,b);
		fprintf(STDERR,"\n");
		return;
	}
#endif
	old=new=0;
	it=1;
	pf=0;
	e=inset(a);
	if(e==0) {
		fprintf(STDERR,"can't find %s\n",a);
		return;
	}
	for(i=0; i<NP; i++)psa[i]=psb[i]=0;
	for(i=0; i<e; i++)psa[p[set[i]].s.p]++;
	k=inset(b);
	if(k==0) {
		fprintf(STDERR,"%s not found\n",b);
		return;
	}
	for(i=0; i<z; i++)if(p[i].s.type)p[i].s.v=0;
	for(i=0; i<k; i++) {
		if(psa[p[set[i]].s.p]==0)continue;
		psb[p[set[i]].s.p]++;
		getcol(j);
		col[j].entry=set[i];
		col[j].next=old;
		old=j;
		p[set[i]].s.v=it;
	}
	if(old==0) {
		fprintf(STDERR,"%s and %s: no part of speech in common\n",
			a,b);
		return;
	}
	e=inset(a);
	for(i=j=0; i<e; i++) {
		set[j]=set[i];
		if(psb[p[set[i]].s.p])j++;
	}
	e=j;
	if(e==0)err("path 1");
loop:
	if(old==0) {
		if(pf)fprintf(STDERR,"\n");
		else fprintf(STDERR," no path from %s to %s\n",a,b);
		goto pathx;
	}
	it++;
	while(old) {
		i=p[col[old].entry].s.st;
		j=old;
		old=col[old].next;
		freecol(j);
		while(j=(unpackw(i++)&WMASK)) {
			if(p[j].s.type&&p[j].s.v==0) {
				getcol(k);
				col[k].next=new;
				col[k].entry=j;
				new=k;
				p[j].s.v=it;
			}
		}
	}
	old=new;
	new=0;
	for(i=j=0; i<e; i++) {
		set[j]=set[i];
		if(p[set[i]].s.v) {
			pfound(set[i]);
			pf++;
		}
		else j++;
	}
	if(e=j)goto loop;
	fprintf(STDERR,"\n");
pathx:
	freeall(old);
	freeall(new);
}
freeall(x)
{
	int i;
	while(x) {
		i=col[x].next;
		freecol(x);
		x=i;
	}
}
pfound(x)
{
	int i,j;
	do {
		for(i=x; i>0&&p[i].h.type; i--);
		if(i==0)err("no hw");
		fprintf(STDERR," &%d%s",x-i,p[i].h.tt+tbase);
		i=p[x].s.st;
		while(j=(WMASK&unpackw(i++))) {
			if(p[j].s.type&&p[j].s.v==p[x].s.v-1)break;
		}
	} while(x=j);
	fprintf(STDERR,"\n");
}
char *avdummy[2];
main(ac,av)
char *av[];
{
	int i,j,k,l,m,n;
	int nhw,nhs;
	int backref;
	char *q;
	int zz;
	int w;
	int nw;
	char y[128];
	if(ac<2) {
		fprintf(stderr,"word_clout: need data file");
		return 1;
	}
	f=fopen(av[1],"r");
	for(i=0; (j=getc(f))!=EOF&&j!='\n'&&i<N-1; i++)in[i]=j;
	if(j==EOF)err("immediate eof");
	in[i]=0;
	if(strcmp(in,"-huffman")==0) {
		goto unpackh;
	}
	else if(strcmp(in,"-pack")) {
		err("expect -huffman or -pack");
	}
	nhw=readnum();
	nhs=readnum();
/* now preallocated:
*	p=(struct x*)ma((nhs+2)*sizeof*p);
*	tbase=(char*)ma(INITTEXT);
*	sqbase=(char*)ma(INITSQ);
*	sqtab=(struct squal*)ma(INITSQTAB*sizeof*sqtab);
*	arena=(unsigned char*)ma(i=(NWINDEX*sizeof*arena*WBITS)/8+1);
*	wqtab=(struct wqual*)ma(NWQ*sizeof*wqtab);
*	wqbase=(char*)ma(WQTEXT*sizeof*wqbase);
*	wqtree=(struct wqt*)ma(WQT*sizeof*wqtree);
*/
readhead:
	for(i=0; i<N&&(j=getc(f))!=EOF&&j!='\n'; i++)in[i]=j;
	if(i>=N)err("line too long");
	if(j==EOF)err("eof in headwords");
	in[i]=0;
	if(i==0)goto hwdone;
	for(j=i-1; j>0&&in[j]!=' '; j--);
	in[j++]=0;
	p[++z].h.tt=text;
	zz=z;
	strcpy(text+tbase,in);
	text+=strlen(in)+1;
	if(text>=INITTEXT)err("inittext too small");
	p[z].h.type=0;
	for( ; in[j]; j++) {
		p[++z].s.type=1;
		p[z].s.q=0;
		p[z].s.st=0;
		k=in[j];
		if(k<='z'&&k>='a')p[z].s.p=k-'a';
		else p[z].s.p=k-'A';
	}
	if((j=getc(f))==':') {
		for(i=0; i<N&&(j=getc(f))!=EOF&&j!='\n'; i++)in[i]=j;
		if(j==EOF)err("eof in sense qual");
		if(i>=N)err("squal too long");
		in[i]=0;
		for(i=0; in[i]; i++)if(in[i]==':'&&in[i+1]==' ')in[i]='-';
		for(i=0; in[i]; i++) {
			if(++zz>z)err("too many squal fields");
			for(j=i; in[j]&&in[j]!=':'; j++);
			if(in[j]==0)err("squal not ending :");
			in[j]=0;
			sensequal(zz,in+i);
			i=j;
		}
		if(zz<z)err("missing squal field");
		goto readhead;
	}
	ungetc(j,f);
	goto readhead;
hwdone:
	fprintf(STDERR,"text=%d\n",text);
	z++;
	p[z].h.type=0; /* extra slot on end */
	for(i=1,j=0; i<z; i++)if(p[i].h.type==1&&p[i].s.q)j++;
	fprintf(STDERR,"%d entries %d qual senses\n",i,j);
	fprintf(STDERR,"sqtext=%d, nsq=%d\n",sqtext,nsq);
	zz=1;
readwords:
	for(i=0; i<N&&(j=getc(f))!=EOF&&j!='\n'; i++)in[i]=j;
	if(j==EOF)goto done;
	if(i>=N)err("line too long");
	in[i]=0;
	while(zz<z&&p[zz].h.type==0)zz++;
	for(i=nw=0; in[i]<='z'&&in[i]>='a'; i++)nw=nw*26+in[i]-'a';
	w=windex;
	p[zz].s.st=w;
	windex+=nw+1;
	if(windex>NWINDEX)err("windex too big");
	while(in[i]==':') {
		q=0;
		if(in[++i]=='{') {
			for(j=i+1; in[j]&&in[j]!='}'; j++);
			if(in[j]==0)err("missing }");
			in[j]=0;
			q=in+i+1;
			i=j+1;
		}
		for(k=0; in[i]>='a'&&in[i]<='z'; i++)k=k*26+in[i]-'a';
		++k; /* offset 1 so preserve 0=0 */
		if(k!=(k&WMASK))err("exceeds wmask");
		if(p[k].h.type==1&&p[k].s.p!=p[zz].s.p) {
			dumps(k);
			dumps(zz);
			err("part bad");
		}
		if(q) {
			k|=WQ;
			wqtab[nwq].waddr=w;
			wqtab[nwq].qaddr=wsym(q,0);
			nwq++;
			if(nwq>=NWQ)err("nwq ovfl");
		}
		packw(w++,k);
	}
	zz++;
	goto readwords;
done:
	fprintf(STDERR,"wqtext=%d nwq=%d windex=%d\n",wqtext,nwq,windex);
	fprintf(STDERR,"wqt=%d\n",wqt);
	fprintf(STDERR,"zz=%d z=%d\n",zz,z);
	m=0;
	if(ac>2&&*av[2]=='c') {
		for(w=0; w<windex; w++)if(unpackw(w))m++;
		fprintf(STDERR,"%d words set\n",m);
	}
	m=0;
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type==0)continue;
		i=p[zz].s.st;
		while(j=unpackw(i++)) {
			j&=WMASK;
			if(p[j].h.type==0)continue;
			k=p[j].s.st;
			while(l=unpackw(k++)) {
				l&=WMASK;
				if(l==zz)goto backed;
			}
			if(unpackw(k))err("oversubscribed");
			packw(k-1,zz);
			m++;
		backed:;
		}
	}
	fprintf(STDERR,"backrefs added %d\n",m);
	if(ac>2&&*av[2]=='c')for(zz=1; zz<z; zz++) {
		if(p[zz].h.type==0) {
			j=zz;
			continue;
		}
		for(k=p[zz].s.st; unpackw(k)&WMASK; k++);
		if(k+1<z&&(unpackw(k+1)&WMASK)==0) {
			fprintf(STDERR,"%s is missing words\n",
				tbase+p[j].h.tt);
		}
	}
	if(ac>3&&(wpackfile=fopen(av[3],"w")))packit();
	goto ahem;
unpackh:
	unpackit();
ahem:	fprintf(STDERR,"ready.\n");
loop:
	for(i=0; i<N&&(j=getchar())!=EOF&&j!='\n'; i++)in[i]=j;
	if(i<N)in[i]=0;
	if(j==EOF||strcmp(in,"q")==0) {
		exit(0);
	}
	for(i=0; in[i]&&in[i]!='/'; i++)if(in[i]=='^') {
		in[i]=0;
		path(in,in+i+1);
		goto loop;
	}
	if(in[i]=='/') {
		strcpy(y,in+i+1);
		in[i]=0;
		if(i>0&&in[i-1]=='.')in[i-1]=0;
		for(i=0; y[i]&&y[i]!='/'; i++);
		y[i]=0;
		k=0;
		if(*in=='&')for(k=0; k<NP&&strcmp(in+1,parts[k]); k++);
		if(rs)free(rs);
		rs=regcomp(y);
		for(zz=1; zz<z; zz++) {
			if(p[zz].h.type)continue;
			if(k) {
				for(i=1; p[zz+i].s.type; i++)
					if(p[zz+i].s.p==k)goto ok;
				continue;
			}
		ok:
			if(regexec(rs,p[zz].h.tt+tbase,0,0)) {
				fprintf(stderr," %s,",p[zz].h.tt+tbase);
			}
		}
		fprintf(stderr,"\n");
		goto loop;
	}
	i=inset(in);
	for(j=0; j<i; j++)dumps(set[j]);
	if(i==0)fprintf(STDERR,"%s not found\n",in);
	goto loop;
}
/* funct == 0 reject
   funct == -1 stop
   funct == otherwise, continue
*/
qpath(funct,b)
char *b;
int (*funct)();
{
	int i,j,k;
	int old,new;
	int it;
	old=new=0;
	it=1;
	k=inset(b);
	if(k==0) {
		fprintf(STDERR,"%s not found\n",b);
		return;
	}
	for(i=0; i<z; i++)if(p[i].s.type)p[i].s.v=0;
	for(i=0; i<k; i++) {
		getcol(j);
		col[j].entry=set[i];
		col[j].next=old;
		old=j;
		p[set[i]].s.v=it;
	}
loop:
	if(old==0) {
		goto pathx;
	}
	it++;
	while(old) {
		i=p[col[old].entry].s.st;
		j=old;
		old=col[old].next;
		freecol(j);
		while(j=(unpackw(i++)&WMASK)) {
			if(p[j].s.type&&p[j].s.v==0) {
				p[j].s.v=it;
				switch((*funct)(j)) {
				case 0:
					p[j].s.v=0;
					continue;
				case -1: goto pathx;
				}
				getcol(k);
				col[k].next=new;
				col[k].entry=j;
				new=k;
			}
		}
	}
	old=new;
	new=0;
	if(old)goto loop;
pathx:
	freeall(old);
	freeall(new);
}
rsparse(b)
char *b;
{
	int i,j;
	wantps=0;
	for(i=0; b[i]&&b[i]!='/'; i++);
	if(i&&b[i-1]=='.') {
		b[i-1]=0;
		for(j=0; j<NP; j++)if(strcmp(parts[j],b+1)==0)wantps=j;
	}
	if(b[i])i++;
	for(j=i; b[j]; j++);
	while(j&&b[j-1]!='/')j--;
	if(j)b[j-1]=0;
	if(atoi(b+j))nwanted=atoi(b+j);
	if(rs) {
		free(rs);
	}
	rs=regcomp(b+i);
}
accept(zz)
{
	int i;
	for(i=zz; i&&p[i].h.type; i--);
	if(regexec(rs,p[i].h.tt+tbase,0,0)) {
		fprintf(stderr," &%d%s,",zz-i,p[i].h.tt+tbase);
		if(--nwanted<=0)return -1;
		return 1;
	}
	return 1;
}
#define NCOMM 100
#define NHASCII 128
#define NNSEN 50
#define NNWDS 100
#define NSQR 256
#define NWQR 256
#define ALLOC(base,num) (base.freq=(long*)ma(num*sizeof(long)),\
			base.code=(long*)ma(num*sizeof(long)),\
			base.tree=(short*)ma(2*num*sizeof(short)))
#define FREE(base) (free(base.freq),free(base.code),free(base.tree))
#define GEN(base,n) (gencodes(base.freq,n,base.code),flipcodes(n,base.code),\
			entree(base.code,n,base.tree))
#define INBIT() (ibuffc?(ibuff>>--ibuffc)&1:(ibuffc=7,((ibuff=getc(f))>>7)&1))
struct huff {
	long *freq,*code;
	short *tree;
};
struct alpha {
	unsigned int k,wqtab;
};
alphacmp(a,b)
struct alpha *a,*b;
{
	if(p[a->k&WMASK].h.type) {
		if(p[b->k&WMASK].h.type)return (a->k&WMASK)-(b->k&WMASK);
		return 1;
	}
	if(p[b->k&WMASK].h.type==0)return (a->k&WMASK)-(b->k&WMASK);
	return -1;
}
wqcmp(a,b)
struct wqual *a,*b;
{
	return a->waddr-b->waddr;
}
int ibuffc,ibuff;
packit()
{
	struct huff comm,qascii,hascii,nsen,parsp,nwds,nwdsback;
	int i,j,k,l,m,n;
	int ii,jj,il,iu,im;
	int zz;
	int sqref[NSQR],wqref[NWQR];
	int medians,medianw;
	struct alpha wtemp[NNWDS];
	/* s & w qualifier text addr from s & w qualifier index */
	for(i=j=0; i<sqtext; i++) {
		if(i==0||sqbase[i-1]==0) {
			sqref[j++]=i;
			if(j>=NSQR)err("NSQR too small");
		}
	}
	for(i=j=0; i<wqtext; i++) {
		if(i==0||wqbase[i-1]==0) {
			wqref[j++]=i;
			if(j>=NWQR)err("NWQR too small");
		}
	}
	ALLOC(comm,NCOMM);
	ALLOC(qascii,NHASCII);
	ALLOC(hascii,NHASCII);
	ALLOC(nsen,NNSEN);
	ALLOC(parsp,NP);
	ALLOC(nwds,NNWDS);
	ALLOC(nwdsback,NNWDS);
	/* common substring, number of senses, parts of speech */
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type)continue;
		j=k,k=p[zz].h.tt;
		if(k==0)l=0;
		else for(l=0; tbase[k+l]==tbase[j+l]; l++);
		if(l>=NCOMM)err("NCOMM too small");
		comm.freq[l]++;
		for(i=l; tbase[i+k]; i++)hascii.freq[tbase[i+k]]++;
		hascii.freq[0]++;
		for(n=zz+1; p[n].s.type; n++);
		if(n-zz>=NNSEN)err("NNSEN too small");
		nsen.freq[n-zz]++;
		for(n=zz+1; p[n].s.type; n++)parsp.freq[p[n].s.p]++;
	}
	/* number of words */
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type) {
			for(i=p[zz].s.st,j=0; k=unpackw(i); i++)if((k&WMASK)<zz)j++;
			i-=p[zz].s.st;
			if(i>=NNWDS)err("NNWDS too small");
			nwds.freq[i]++;
			nwdsback.freq[j]++;
		}
	}
	/* ascii in qualifiers */
	for(i=0; i<sqtext; i++)qascii.freq[sqbase[i]]++;
	for(i=0; i<wqtext; i++)qascii.freq[wqbase[i]]++;

	GEN(qascii,NHASCII);
	GEN(hascii,NHASCII);
	GEN(comm,NCOMM);
	GEN(nsen,NNSEN);
	GEN(parsp,NP);
	GEN(nwds,NNWDS);
	GEN(nwdsback,NNWDS);

	/* median sense qualifier */
	for(i=j=0,zz=1; zz<z; zz++) {
		if(p[zz].s.type) {
			i++;
			if(p[zz].s.q)j++;
		}
	}
	medians=(7*i)/(10*j);
	/* median word qualifier runs */
	for(i=j=0,zz=1; zz<z; zz++) {
		if(p[zz].h.type) {
			for(ii=p[zz].s.st; jj=unpackw(ii); ii++) {
				i++;
				if(jj&WQ)j++;
			}
		}
	}
	medianw=(7*i)/(10*j);

	/* Start packed output */
	fprintf(wpackfile,"-huffman\n");

	/* OUTPUT trees */
	outtree(qascii);
	outtree(hascii);
	outtree(comm);
	outtree(nsen);
	outtree(parsp);
	outtree(nwds);
	outtree(nwdsback);

	/* OUTPUT headers */
	writesim(500000,text);
	writesim(100000,z);
	writesim(1000,nsq);
	writesim(500000,windex);
	writesim(10000,nwq);
	writesim(2000,wqtext);
	writesim(2000,sqtext);
	writesim(100,medians);
	writesim(100,medianw);

	/* OUTPUT sense qualifier text */
	for(i=0; i<sqtext; i++)outbit(qascii.code[sqbase[i]]);
	/* OUTPUT word qualifier text */
	for(i=0; i<wqtext; i++)outbit(qascii.code[wqbase[i]]);
	/* OUTPUT hw ncommon, text \0, number of senses, part of speech */
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type)continue;
		j=k,k=p[zz].h.tt;
		if(k==0)l=0;
		else for(l=0; tbase[k+l]==tbase[j+l]; l++);
		outbit(comm.code[l]);
		for(i=l; tbase[i+k]; i++)outbit(hascii.code[tbase[i+k]]);
		outbit(hascii.code[0]);
		for(n=zz+1; p[n].s.type; n++);
		outbit(nsen.code[n-zz]);
		for(n=zz+1; p[n].s.type; n++)outbit(parsp.code[p[n].s.p]);
	}
	/* OUTPUT qualified sense run, qualifier reference number */
	m=n=0;
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type) {
			n++;
			if(p[zz].s.q) {
				writesim(medians,n-m-1);
				m=n;
				j=squal(zz)-sqbase;
				for(i=0; i<NSQR; i++)if(sqref[i]==j)break;
				if(sqref[i]!=j)err("NSQR search botch");
				outbitf(NSQR+i);
			}
		}
	}
	writesim(medians,n-m);
	/* sort words into alphabetical order */
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type==0)continue;
		for(i=p[zz].s.st,j=0; k=unpackw(i); i++) {
			wtemp[j].wqtab=0;
			if(k&WQ) {
				il=0; iu=nwq;
				while(il<iu-1) {
					im=(il+iu)/2;
					if(wqtab[im].waddr>i)iu=im;
					else il=im;
				}
				if(wqtab[il].waddr!=i)err("bin botch");
				wtemp[j].wqtab=il;
			}
			wtemp[j++].k=k;
		}
		qsort(wtemp,j,sizeof*wtemp,alphacmp);
		for(i=p[zz].s.st,j=0; unpackw(i); i++) {
			if(wtemp[j].k&WQ) {
				wqtab[wtemp[j].wqtab].waddr=i;
			}
			opackw(i,wtemp[j++].k);
		}
	}
	qsort(wqtab,nwq,sizeof*wqtab,wqcmp);
	/* OUTPUT # words, # words back, back references */
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type==0)continue;
		for(i=p[zz].s.st,j=0; k=unpackw(i)&WMASK; i++) {
				if(k<zz||p[k].s.type==0)j++;
		}
		outbit(nwds.code[i-p[zz].s.st]);
		outbit(nwdsback.code[j]);
		for(i=p[zz].s.st; k=unpackw(i)&WMASK; i++) {
			if(k<zz||p[k].s.type==0)outbitf(WQ|k);
		}
	}
	/* OUTPUT word run, word qualifier index */
	m=n=0;
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type) {
			for(ii=p[zz].s.st; jj=unpackw(ii); ii++) {
				n++;
				if(jj&WQ) {
					writesim(medianw,n-m-1);
					m=n;
					j=wqual(ii)-wqbase;
					for(i=0; i<NWQR; i++)if(wqref[i]==j)break;
					if(wqref[i]!=j)err("NWQR search botch");
					outbitf(NWQR+i);
				}
			}
		}
	}
	writesim(medianw,n-m);
	FREE(qascii);
	FREE(hascii);
	FREE(comm);
	FREE(nsen);
	FREE(parsp);
	FREE(nwds);
	FREE(nwdsback);
	outbitf(0xff);
	fclose(wpackfile);
}
outbit(x)
register x;
{
	static obuff,b;
	while(x>1) {
		obuff=(obuff<<1)+(x&1);
		x>>=1;
		if(++b>=8) {
			b=0;
			putc(obuff,wpackfile);
		}
	}
}
outbitf(x)
register x;
{
	register y;
	for(y=1; x>1; x>>=1)y=(y<<1)+(x&1);
	outbit(y);
}
inbit() { return INBIT(); }
char *rhalloc(x) { return malloc(x); }
char *rhfree(x) char *x; { free(x); }
wbytes(n) {
	putc(n>>8,wpackfile);
	putc(n,wpackfile);
}
rbytes() {
	int i;
	i=getc(f);
	return (i<<8)+getc(f);
}
decode(tree)
register short *tree;
{
	register int i;
	i=0;
	if(*tree++<2)return *tree&077777;
	do {
		if(INBIT())i=tree[i+1];
		else i=tree[i];
	} while((i&0100000)==0);
	return i&077777;
}
outtree(x)
struct huff x;
{
	int i;
	for(i=0; i<=x.tree[0]; i++)wbytes(x.tree[i]);
}
struct huff intree()
{
	struct huff x;
	int i;
	x.code=0;
	x.freq=0;
	i=rbytes();
	x.tree=(short*)ma((i+1)*sizeof(short));
	x.tree[0]=i;
	for(i=1; i<=x.tree[0]; i++)x.tree[i]=rbytes();
	return x;
}
rfixlen(code)
register code;
{
	register i=0;
	if(code<=1)return 0;
	do {
		i=(i<<1)+INBIT();
	} while((code>>=1)>1);
	return i;
}
unpackit()
{
	int i,j,k,l,m,n;
	int sqref[NSQR],wqref[NWQR];
	struct huff qascii,hascii,comm,nsen,parsp,nwds,nwdsback;
	int medians,medianw;
	int zz;
	int ii,jj;
	fprintf(STDERR,"wait for 'ready.'\n");
	fflush(STDERR);
	qascii=intree();
	hascii=intree();
	comm=intree();
	nsen=intree();
	parsp=intree();
	nwds=intree();
	nwdsback=intree();
	text=readsim(500000);
	z=readsim(100000);
	nsq=readsim(1000);
	windex=readsim(500000);
	nwq=readsim(10000);
	wqtext=readsim(2000);
	sqtext=readsim(2000);
	medians=readsim(100);
	medianw=readsim(100);

	for(i=0; i<sqtext; i++)sqbase[i]=decode(qascii.tree);
	for(i=0; i<wqtext; i++)wqbase[i]=decode(qascii.tree);

	for(i=j=0; i<sqtext; i++) {
		if(i==0||sqbase[i-1]==0) {
			sqref[j++]=i;
		}
	}
	for(i=j=0; i<wqtext; i++) {
		if(i==0||wqbase[i-1]==0) {
			wqref[j++]=i;
		}
	}
	for(k=0,zz=1; zz<z; zz=n) {
		l=decode(comm.tree);
		for(i=0; i<l; i++)tbase[i+k]=tbase[i+j];
		while(tbase[k+l++]=decode(hascii.tree));
		p[zz].h.tt=k;
		j=k;
		k+=l;
		l=decode(nsen.tree);
		for(n=zz+1; n<zz+l; n++) {
			p[n].s.p=decode(parsp.tree);
			p[n].s.type=1;
		}
	}
	if(zz!=z)fprintf(STDERR,"botch zz!=z %d %d\n",zz,z);
	if(k!=text)fprintf(STDERR,"botch k!=text %d %d\n",k,text);
	p[z].h.type=0;
	m=n=0;
	l=readsim(medians);
	k=0;
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type) {
			n++;
			if(l==n-m-1) {
				p[zz].s.q=1;
				m=n;
				sqtab[k].saddr=zz;
				sqtab[k++].qaddr=sqref[rfixlen(NSQR)];
				l=readsim(medians);
			}
		}
	}
	if(k!=nsq)fprintf(STDERR,"k!=nsq %d %d\n",k,nsq);
	j=0;
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type==0)continue;
		l=decode(nwds.tree);
		p[zz].s.st=j;
		m=decode(nwdsback.tree);
		for(i=0; i<m; i++) {
			packw(j+i,n=rfixlen(WQ));
			if(p[n].s.type) {
				packw(p[n].s.st+p[n].s.v++,zz);
			}
		}
		p[zz].s.v=m;
		j+=l+1;
	}
	if(j!=windex)fprintf(STDERR,"j!=windex %d %d\n",j,windex);
	m=n=0;
	l=readsim(medianw);
	j=0;
	for(zz=1; zz<z; zz++) {
		if(p[zz].h.type) {
			for(ii=p[zz].s.st; jj=unpackw(ii); ii++) {
				n++;
				if(n-m-1==l) {
					m=n;
					opackw(ii,WQ+jj);
					i=wqref[rfixlen(NWQR)];
					wqtab[j].waddr=ii;
					wqtab[j++].qaddr=i;
					l=readsim(medianw);
				}
			}
		}
	}
	if(j!=nwq)fprintf(STDERR,"j!=nwq %d %d\n",j,nwq);
	free(qascii.tree);
	free(hascii.tree);
	free(comm.tree);
	free(nsen.tree);
	free(parsp.tree);
	free(nwds.tree);
	free(nwdsback.tree);
}
char *rhalloc();
typedef long huffreq;
typedef long huffcode;
typedef unsigned short hufftree;
typedef int huffnum;

/* gencodes((huffreq*)freqtab, (huffnum)n, (huffcode*)output);
   flipcodes((huffnum)n,(huffcode*)output);
   entree((huffcodes*)output.flipped,(huffnum)n,(hufftree*)treearray)
*/
#define ENDFLAG 0100000
/*	decoding tree is (hufftree) array; start at 0, add next bit to index;
	if ENDFLAG is set in entry, everything else is the wanted code;
	if not, take the next offset from the entry;

	first entry in tree is number of entries in rest of tree;
	offset 0 corresponds to tr[1].
*/
entree(cd,ncd,tr)
huffcode *cd;
huffnum ncd;
hufftree *tr;
{
	long i,j,k;
	long x;
	for(i=0; i<2*ncd; i++)tr[i]=0;
	tr++;
	k=2;

	for(i=0; i<ncd; i++) {
		j=0;
		for(x=cd[i]; x>1; x>>=1) {
			j+=x&1;
			if(tr[j]==0) {
				if(x>3) {
					tr[j]=k;
					k+=2;
				}
				else {
					tr[j]=i|ENDFLAG;
				}
			}
			j=tr[j];
			if(x>3&&(j&ENDFLAG)) {
				err("botched huffman code");
			}
		}
	}
	*--tr=k;
}
/* Huffman codes from counts */

/* gencodes(huffreq freq table, huffnum length, huffcode output code table)
	output code words have leading 1-bit set, rest is huffman code.
	program aborts if a code is more than 30 bits -- try increasing
	the minimum counts.
	zero frequency counts produce zero code-words (are ignored).

	flipcodes(huffnum length, huffcode code table) flips the bit order of
		the huffman code bits.
*/

struct code {
	struct code *left,*right;
	huffreq count;
	huffnum val;
};

struct code *getcode()
{
	struct code *h;
	h=(struct code*)rhalloc(sizeof*h);
	if(h==0)err("getcode: can't rhalloc code");
	h->left=h->right=0;
	h->val=h->count=0;
	return h;
}

tcompare(a,b)
register struct code **a,**b;
{
	if(a[0]->count<b[0]->count)return -1;
	if(a[0]->count>b[0]->count)return 1;
	if(a[0]->val<b[0]->val)return -1;
	if(a[0]->val>b[0]->val)return 1;
	return 0;
}

gencodes(infreq,nel,outcodes)
huffreq *infreq;
huffnum nel;
huffcode *outcodes;
{
	struct code **tp,**a,*p,*s;
	long i,j,k;
	long na;
	tp=(struct code**)rhalloc(nel*sizeof*tp);
	if(tp==0)err("gencodes: can't rhalloc tp");
	for(i=0; i<nel; i++) {
		outcodes[i]=0;
		tp[i]=getcode();
		tp[i]->count=infreq[i];
		tp[i]->val=i;
	}
	qsort(tp,nel,sizeof*tp,tcompare);
	for(k=0; k<nel-1; k++) {
		if(tp[k]->count==0)rhfree(tp[k]);
		else break;
	}
	a=&tp[k];
	na=nel-k;
	/* add smallest two and replace in heap till heap gone */
	for(k=na; k>2; ) {
		j=a[1]->count<a[2]->count?1:2;
		p=getcode();
		p->count=a[0]->count+a[j]->count;
		p->left=a[0];
		p->right=a[j];
		a[j]=p;
		for(i=j; (i+=i+1)<k; j=i) {
			if(i+1<k&&a[i]->count>a[i+1]->count)i++;
			if(a[j]->count<=a[i]->count)break;
			p=a[j];
			a[j]=a[i];
			a[i]=p;
		}
		a[0]=a[--k];
		for(i=j=0; (i+=i+1)<k; j=i) {
			if(i+1<k&&a[i]->count>a[i+1]->count)i++;
			if(a[j]->count<=a[i]->count)break;
			p=a[j];
			a[j]=a[i];
			a[i]=p;
		}
	}
	if(k==2) {
		p=getcode();
		p->count=a[0]->count+a[1]->count;
		p->left=a[0];
		p->right=a[1];
	}
	else p=a[0];
	rhfree(tp);
	ascodes(p,1L,outcodes);
}

ascodes(p,hcode,outcodes)
struct code *p;
long *outcodes;
long hcode;
{
	if(hcode<=0)err("ascodes: bitcode overflow");
	if(p->left==0) {
		if(p->right!=0)err("ascodes: tree ends wrong");
		outcodes[p->val]=hcode;
	}
	else {
		if(p->right==0)err("ascodes: tree ends oddly");
		ascodes(p->right,(hcode<<1)|1,outcodes);
		ascodes(p->left,(hcode<<1)|0,outcodes);
	}
	rhfree(p);
}

flipcodes(nel,codes)
huffnum nel;
huffcode *codes;
{
	long i,j,k;
	long x;
	for(i=0; i<nel; i++) {
		if(codes[i]==0)continue;
		x=codes[i];
		codes[i]=1;
		while(x>1) {
			codes[i]=codes[i]*2|(x&1);
			x>>=1;
		}
	}
}
/* self-similar Huffman codes for neg exp distr.
	calls outbitf(1+forward code)
	calls inbit()
*/
writesim(median,intv)
{
	int i;
	static nc,dc,ec,omedian;
	if(median!=omedian) {
		omedian=median;
		for(i=1; (1L<<i)<=median; i++);
		nc=1L<<--i;
		dc=2*nc-median;
		ec=4*nc-median;
	}
	while(intv>=median) {
		outbitf(03L);
		intv-=median;
	}
	outbitf(02L);
	if(intv<dc)outbitf(intv+nc);
	else outbitf(ec+intv);
}
readsim(median) {
	int i,j,k;
	static nc,oi,dc,omedian;
	if(median!=omedian) {
		omedian=median;
		for(i=1; (1L<<i)<=median; i++);
		nc=1L<<--i;
		oi=i;
		dc=2*nc-median;
	}
	k=0; while(INBIT())k+=median;
	for(i=j=0; i<oi; i++)j+=j+INBIT();
	if(j<dc)return k+j;
	return k-dc+j+j+INBIT();
}
#ifdef AMBIGUOUS
prsense(i)
{
	fprintf(STDERR," &%d%s",i-hwof(i),p[hwof(i)].h.tt+tbase);
}
ambpath(a,b)
char *a,*b;
{
	int i,j,k;
	int froma,fromb,nfroma,nfromb;
	int e;
	int it;
	int pf;
	pf=0;
	it=1;
	e=inset(a);
	if(e==0) {
		fprintf(STDERR,"can't find %s\n",a);
		return;
	}
	k=inset(b);
	if(k==0) {
		fprintf(STDERR,"%s not found\n",b);
		return;
	}
	for(i=0; i<z; i++)if(p[i].s.type)p[i].s.v=0; else p[i].h.v=0;
	fromb=0;
	for(i=0; i<k; i++) {
		getcol(j);
		col[j].entry=set[i];
		col[j].next=fromb;
		fromb=j;
		p[set[i]].s.v=it;
	}
	e=inset(a);
	froma=0;
	for(i=0; i<e; i++) {
		getcol(j);
		col[j].entry=set[i];
		col[j].next=froma;
		froma=j;
		p[set[i]].s.v=it;
	}
loop:
	pf=0;
	for(k=froma; k; k=col[k].next) {
		j=hwof(col[k].entry);
		p[j].h.v|=1;
		if(p[j].h.v==3)pf++;
	}
	for(k=fromb; k; k=col[k].next) {
		j=hwof(col[k].entry);
		p[j].h.v|=2;
		if(p[j].h.v==3)pf++;
	}
	if(pf>0) {
		ambiguous(froma,fromb);
		goto pathx;
	}
	if(froma==0&&fromb==0) {
		fprintf(STDERR,"no link found\n");
		return;
	}
	it++;
	nfroma=0;
	while(froma) {
		i=p[col[froma].entry].s.st;
		j=froma;
		froma=col[j].next;
		freecol(j);
		while(j=(unpackw(i++)&WMASK)) {
			if(p[j].s.type&&p[j].s.v==0) {
				getcol(k);
				col[k].next=nfroma;
				col[k].entry=j;
				nfroma=k;
				p[j].s.v=it;
			}
		}
	}
	froma=nfroma;
	nfromb=0;
	while(fromb) {
		i=p[col[fromb].entry].s.st;
		j=fromb;
		fromb=col[j].next;
		freecol(j);
		while(j=(unpackw(i++)&WMASK)) {
			if(p[j].s.type&&p[j].s.v==0) {
				getcol(k);
				col[k].next=nfromb;
				col[k].entry=j;
				nfromb=k;
				p[j].s.v=it;
			}
		}
	}
	fromb=nfromb;
	goto loop;
pathx:
	freeall(froma);
	freeall(fromb);
}
hwof(i)
{
	while(i>0&&p[i].h.type!=0)i--;
	if(i==0)err("hwof()");
	return i;
}
ambiguous(fa,fb)
{
	int f,g,i,j,k;
	int dbest;
	int enda,endb;
	int la,lb;
	int length;
	int best,a;
	dbest=9999;
	for(best=0; best<2; best++)
	for(f=fa; f; f=(f==fb)?0:fb)for(g=f; g; g=col[g].next) {
		enda=endb=0;
		la=lb=9999;
		i=hwof(col[g].entry);
		if(p[i].h.v!=3)continue;
		for(j=1; p[i+j].s.type; j++) {
			if(p[i+j].s.v==0)continue;
			a=pathtoend(i+j);
			if(enda==0)enda=a,la=p[i+j].s.v;
			else
			if(hwof(col[a].entry)==hwof(col[enda].entry)) {
				if(p[i+j].s.v<la) {
					freeall(enda);
					enda=a;
					la=p[i+j].s.v;
				}
				else freeall(a);
			}
			else if(endb==0)endb=a,lb=p[i+j].s.v;
			else
			if(hwof(col[a].entry)==hwof(col[endb].entry)) {
				if(p[i+j].s.v<lb) {
					freeall(endb);
					endb=a;
					lb=p[i+j].s.v;
				}
				else freeall(a);
			}
		}
		length=la+lb;
		if(best==0&&length<dbest)dbest=length;
		else if(best==1&&length==dbest) {
			prrev(enda);
			fprintf(STDERR,"\n");
			prrev(endb);
			fprintf(STDERR,"\n\n");
			p[i].h.v=0;
		}
		freeall(enda);
		freeall(endb);
	}
}
pathtoend(x)
{
	int i,j,k;
	for(k=0;;) {
		getcol(j);
		col[j].entry=x;
		col[j].next=k;
		k=j;
		if(p[x].s.v<=1)return k;
		i=p[x].s.st;
		while(j=(WMASK&unpackw(i++))) {
			if(p[j].s.type&&p[j].s.v==p[x].s.v-1)break;
		}
		x=j;
	}
}
prrev(i)
{
	if(i==0)return;
	prrev(col[i].next);
	prsense(col[i].entry);
}
#endif
