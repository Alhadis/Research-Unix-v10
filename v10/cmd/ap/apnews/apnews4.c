# include "stdio.h"
# include "cbt.h"
# include "assert.h"
# include "ctype.h"
# include "time.h"
# include "sys/types.h"
# include "sys/stat.h"
extern int day, yesterday;
# define NST 200
extern int stc, inolim;
extern char storylist[NST][12];
extern char nowfile[];
extern char *storydir;
extern char names[20][14];
extern char heads[20][50];
char *index();
extern int inodes[NST+5];

getkeys(firsen)
{
# define NWS 50
char lb[200], *srchvec[NWS+1], *v[40], *s, *sufstr();
char wb[NWS+1][15];
int i, n, ng;
FILE *f;
f = fopen(nowfile, "r");
n=0;
if (f==NULL) return;
while (fgets(lb, 200, f))
	{
	if (lb[0]=='^') continue;
	trimnl(lb);
	ng = getargs(lb, v);
	for(i=0; i<ng; i++)
		{
		if (common(v[i])) continue;
		s = sufstr(v[i]);
		if (strlen(s)>= 14) s[14]=0;
		strcpy(srchvec[n]=wb[n], s);
		if (n++ >NWS) break;
		}
	if (n>NWS) break;
	}
fclose(f);
if (n<=0) return;
lkkeys( srchvec, n);
}
wdsearch(s, firsen)
	char *s;
{
char *srchvec[50];
int n;
n=mksrch (s, srchvec);
lkkeys( srchvec, n);
if (firsen) printf("----\n");
}
mksrch (s, v)
	char *s, *v[];
{
char *g[20];
int i, n, ng;
static char wb[20][20];
lcase(s);
ng = getargs(s, g);
for (n=i=0; i<ng; i++)
	{
	if (common(g[i])) continue;
	strcpy(v[n] = wb[n], sufstr(g[i]));
	n++;
	}
return(n);
}
bfile *bf=NULL;
bfile *bfy=NULL;
long ytim = 0L;
# define ND 50
struct dst {
	char dn[6];
	int dc;
	};
struct dst docs[ND];
struct dst *pnd = docs;
lkkeys (srchvec, n)
	char *srchvec[];
{
mbuf key, rkey, rrec;
char ka[100], kb[100], kc[100], *dn;
int i;
key.mdata =ka; rkey.mdata=kb; rrec.mdata=kc;
if (bf==NULL)
	{
	sprintf(ka, "%s/.I%d", storydir, day);
	bf = bopen(ka, 0);
	if (bf==NULL) return;
	}
if (bfy==NULL)
	{
	sprintf(ka, "%s/.I%d", storydir, yesterday);
	bfy = bopen(ka,0);
	strcat(ka, ".T");
	ytim = filtim(ka);
	}
pnd = docs;
for(i=0; i<n; i++)
	{
	sprintf(key.mdata, "%s.", srchvec[i]);
	key.mlen = strlen(key.mdata);
	bseek (bf, key);
	while (bread(bf, &rkey, &rrec)==NULL)
		{
		rkey.mdata[rkey.mlen]=0;
		if (strncmp(rkey.mdata, key.mdata, key.mlen)!=0)
			break;
		docfnd(index(rkey.mdata, '.'));
		}
	if (bfy==NULL) continue;
	bseek(bfy, key);
	while (bread(bfy, &rkey, &rrec)==NULL)
		{
		rkey.mdata [rkey.mlen]=0;
		if (strncmp(rkey.mdata, key.mdata, key.mlen)!=0)
			break;
		dn = index(rkey.mdata, '.');
		/* check that dn has not been overwritten */
		if (dn && filtim(dn+1) <= ytim)
			docfnd(dn);
		}
	}
docsrt();
n = pnd-docs;
if (n>20) n=20;
for(i=0; i<n; i++)
	strcpy(names[i], docs[i].dn);
for (; i<20; i++)
	names[i][0]=0;
for(i=0; i<20; i++)
	heads[i][0]=0;
disprint();
}
docfnd(s)
	char *s;
{
int i;
struct dst *p;
if (s==NULL) return;
s++;
assert(strlen(s)<=5);
for(p=docs; p<pnd; p++)
	{
	if (strcmp(s, p->dn)==0)
		break;
	}
if (p<pnd)
	p->dc++;
else
	{
	if (pnd>=docs+ND) return;
	strcpy(pnd->dn, s);
	pnd->dc=1;
	pnd++;
	}
}
docsrt()
{
int dcmp();
struct dst *p;
qsort( docs, pnd-docs, sizeof(*pnd), dcmp);
}
dcmp(p1, p2)
	struct dst *p1, *p2;
{
return (p2->dc - p1->dc);
}
menu(lc)
{
char *m;
tpos(10,0);
if (lc==1)
	m="titles";
else if (lc==0)
	m="of this story";
else if (lc==2)
	m="first sentences";
printf("CR: more %s; t: top titles; m: more titles; t>N: top, rated >= N;\n", m);
printf("s: search words; y: search story; c: copy; q: exit\n");
}
extend(n)
{
/* make sure we have n stories */
int i, ino;
FILE *f;
char line[20];
struct stat stbuff;
if (stc>=n) return;
for (i=inolim; i<stc; i++)
	{
	stat (storylist[i], &stbuff);
	inodes[i]= stbuff.st_ino;
	}
if (access("..slist", 04)!=0 || (stc==0 &&
	filtim(".") > filtim("..slist")))
		{
		if (fork()==0)
			{
			close(1);
			assert( creat("..slist", 0666) == 1);
			chmod("..slist", 0666);
			execl("/usr/bin/ls", "ls", "-t", 0);
			execl("/bin/ls", "ls", "-t", 0);
			assert(0);
			}
		wait(0);
		}
f = fopen("..slist", "r");
assert(f!=NULL);
while (fgets(line, 20, f))
	{
	trimnl(line);
	/* acceptable lines are a9999 or mondy.a9999 */
	if (strlen(line)==5 && line[0]=='a')
		;
	else
	if (strlen(line)==11 && line[6]=='a')
		;
	else
		continue;
	i=stat (line, &stbuff);
	if (i!=0) continue;
	ino = stbuff.st_ino;
	for(i=0; i<stc; i++)
		if (ino == inodes[i])
			break;
	if (i<stc)
		continue;
	inodes[inolim=stc]=ino;
	strcpy(storylist[stc++], line);
	if (stc>=100) break;
	}
fclose(f);
return;
}
long
filtim( f)
	char *f;
{
struct stat stbuff;
int k;
k = stat (f, &stbuff);
if (k!=0) return(0L);
return(stbuff.st_mtime);
}
/* the next routine picks out longest word */
pickbig (wv, nw)
	char **wv;
{
int mlen, mat, i, k, c;
for(mlen=mat=i=0; i<nw; i++)
	{
	k = strlen(wv[i]);
	if (k>mlen)
		{
		mlen=k;
		mat=i;
		}
	}
c = wv[mat][0];
if (islower(c)) wv[mat][0]=toupper(c);
return(mat);
}
lcase(s)
	char *s;
{
for ( ; *s; s++)
	{
	if (isupper(*s))
		*s = tolower(*s);
	}
}
