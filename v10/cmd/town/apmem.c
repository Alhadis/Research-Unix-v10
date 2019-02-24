# include "stdio.h"
# include "ctype.h"
# include "assert.h"
# include "cbt.h"
# define SAME 0
bfile *townfile;
bfile *happened;
main(argc,argv)
	char *argv[];
{
char x[100], y[500], z[100];
char city[100];
int dbgflg=0;
if (isatty(1)) dbgflg=1;
argc--; argv++;
townfile = bopen("/usr/spool/town/ustowns", 0);
happened = bopen("/usr/spool/town/happen", 2);
assert(townfile!=NULL);
while (argc--)
	{
	if (dbgflg) printf("story %s\n",*argv);
	if (getstory(*argv++, x, y, z)==0)
		continue;
	if (dbgflg) printf("got story from %s, len %d start %.20s\n", x, strlen(y), y);
	where (x, city);
	if (nothing(y)) continue;
	if (dbgflg) printf("approaching update city %s\n",city);
	update (city, z, y);
	if (dbgflg) printf("past update\n");
	}
bclose(happened);
}
getstory(name, dateline, sentence, date)
	char *name, *dateline, *sentence, *date;
{
char *s, line[100], *p, *t, *strchr(), *ctime();
int k;
long tn, time();
FILE *f;
f = fopen(name, "r");
if (f==NULL) return(0);
fgets(line, 100, f);
if (line[2]!='a')
	{
	fclose(f);
	return(0);
	}
while (fgets(line, 100, f))
	if (line[0]=='\t') break;
for(p=line; isspace(*p); p++)
	;
sentence[0]=0;
s = strchr(p, '-');
if (s==NULL) return(0);
t = strchr(p, '(');
if (t==NULL || t>s) t=s;
*--t=0;
while (isspace(*--t))
	*t=0;
strcpy(dateline, p);
while (isspace(*++s));
do {
	trimnl(s);
	k = sentend(s);
	strcat(sentence, s);
	if (k>0) break;
	strcat(sentence, " ");
	} while (fgets(s=line, 100, f));
tn = time(0);
p = ctime(&tn);
sprintf(date, "%.6s %.4s", p+4, p+20);
fclose(f);
return(1);
}
sentend(s)
	char *s;
{
for( s+=2; *s; s++)
	if (s[0]=='.')
		{
		char *p = s;
		if (s[-2]=='.' || s[-2]==' ')
			continue;
		if (s[-3]==' ' &&strncase(s-2, "st", 2)==0)
			continue;
		if (strncase(s-2, "Mr", 2)==0)
			continue;
		if (strncase(s-3, "Mrs", 3)==0)
			continue;
		if (strncase(s-2, "Ms", 2)==0)
			continue;
		if (strncase(s-3, "Sen", 3)==0)
			continue;
		if (strncase(s-4, "Capt", 4)==0)
			continue;
		if (strncase(s-3, "Col", 3)==0)
			continue;
		if (strncase(s-3, "Gov", 3)==0)
			continue;
		if (strncase(s-3, "Rep", 3)==0)
			continue;
		if (strncase(s-2, "Co", 2)==0)
			continue;
		if (strncase(s-2, "Dr", 2)==0)
			continue;
		if (strncase (s-3, "Rev", 3)==0)
			continue;
		while (*++p ==' ' || *p == '\t');
		if (p[0]=='\'' && p[1]=='\'')
			{
			p[2]=0;
			return(1);
			}
		if (*p==0 || isupper(*p) )
			{
			s[1]=0;
			return(1);
			}
		}
return(0);
}
strncase(s, t, n)
	char *s, *t;
{
while (n--)
	{
	if ( (*s ^ *t) & 0337)
		return(1);
	if (*s==0) return(0);
	s++; t++;
	}
return(0);
}
trimnl(s)
	char *s;
{
while (*s)s++;
if (*--s=='\n') *s=0;
}
struct {
	char *old, *new;
	} *st, stnames [] = {
{"Ala.", "al"},
{"Alaska", "ak"},
{"Ariz.", "az"},
{"Ark.", "ak"},
{"Calif.", "ca"},
{"Colo.", "co"},
{"Conn.", "ct"},
{"Del.", "de"},
{"Fla.", "fl"},
{"Ga.", "ga"},
{"Hawaii", "hi"},
{"Ida.", "id"},
{"Idaho", "id"},
{"Ill.", "il"},
{"Ind.", "in"},
{"Iowa", "ia"},
{"Kan.", "ks"},
{"Ky.", "ky"},
{"La.", "la"},
{"Mass.", "ma"},
{"Mass", "ma"},
{"Md.", "md"},
{"Me.", "me"},
{"Maine", "me"},
{"Mich.", "mi"},
{"Mich", "mi"},
{"Minn.", "mn"},
{"Miss.", "ms"},
{"Mo.", "mo"},
{"Mont.", "mt"},
{"N.C.", "nc"},
{"N.D.", "nd"},
{"N.H.", "nh"},
{"N.J.", "nj"},
{"N.M.", "nm"},
{"N.Y.", "ny"},
{"Neb.", "ne"},
{"Nev.", "nv"},
{"Ohio", "oh"},
{"OHIO", "oh"},
{"Okla.", "ok"},
{"Ore.", "or"},
{"Pa.", "pa"},
{"R.I.", "ri"},
{"S.C.", "sc"},
{"S.D.", "sd"},
{"Puerto Rico", "pr"},
{"Tenn.", "tn"},
{"Texas", "tx"},
{"Utah", "ut"},
{"Va.", "va"},
{"Vt.", "vt"},
{"W.Va.", "wv"},
{"Wash.", "wa"},
{"Wis.", "wi"},
{"Wyo.", "wy"},
{NULL, NULL},
};
where (dateline, city)
	char *dateline, *city;
{
char *state, *p, *s;
state = strchr(dateline, ',');
if (state!=NULL)
	{
	*state++=0;
	while (isspace(*state))state++;
	for(st=stnames; st->old; st++)
		if (strcmp(state, st->old)==0)
			break;
	if (st->old)
		strcpy(state, st->new);
	else
		fprintf(stderr, "State? %s\n",state);
	}
lcase(dateline);
find (dateline, state, city);
}
find (request, state, city)
	char *request, *state, *city;
{
mbuf key, rkey, rrec;
int btype, bsize;
char rka[100], rra[100], best[100], *s;
	best[0]=0;
	btype='3'; bsize=' ';
	key.mdata = request; key.mlen = strlen(request);
	bseek(townfile, key);
	rkey.mdata = rka; rrec.mdata = rra;
	while (bread(townfile, &rkey, &rrec)==NULL)
		{
		rkey.mdata[rkey.mlen]=0;
		rrec.mdata[rrec.mlen]=0;
		if (strncmp(rkey.mdata, request, key.mlen)!=SAME)
			break;
		if (state)
			{
			s = strchr(rkey.mdata, ',');
			if (strncmp(state, s+2, 2))
				continue;
			}
		for(s=rrec.mdata; *s; s++)
			;
		--s;
		if (*s < btype || (*s == btype && s[-2] > bsize) )
			{
			if (state || (s[-2]>'7'))/* don't take small
				american cities for Ottawa, Toronto...*/
				{
				btype = *s;
				bsize = s[-2];
				strcpy(best, rkey.mdata);
				}
			}
		}
	strcpy(city, best);
}
lcase(s)
	char *s;
{
for (; *s; s++)
	if (isupper(*s))
		*s = tolower(*s);
}
update (city, date, text)
	char *city, *date, *text;
{
mbuf key, val;
char buff[1000];
if (city[0]==0 || city[0]==' '|| city[0]=='\t') return;
sprintf(buff, "(%s) %s", date, text);
val.mdata = buff;
val.mlen = strlen(buff);
key.mdata = city;
key.mlen = strlen(city);
bwrite(happened, key, val);
printf("%s\n", city);
}
nothing(s)
	char *s;
{
while (isspace(*s)) s++;
if (strlen(s)<20) return(1);
return(0);
}
