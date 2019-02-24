/* C compiler: string & list management */

#include "c.h"

#define TABLESIZE 1024
static struct string {	/* strings: */
	char *str;		/* pointer to the string */
	int len;		/* its length */
	struct string *link;	/* next one on this chain */
} *buckets[TABLESIZE];	/* the string table */

static char *next;	/* slot for next allocated string */
static char *strlimit;	/* 1 past end of current string region */
static int scatter[] = {	/* map characters to random values */
	2078917053, 143302914, 1027100827, 1953210302, 755253631,
	2002600785, 1405390230, 45248011, 1099951567, 433832350,
	2018585307, 438263339, 813528929, 1703199216, 618906479,
	573714703, 766270699, 275680090, 1510320440, 1583583926,
	1723401032, 1965443329, 1098183682, 1636505764, 980071615,
	1011597961, 643279273, 1315461275, 157584038, 1069844923,
	471560540, 89017443, 1213147837, 1498661368, 2042227746,
	1968401469, 1353778505, 1300134328, 2013649480, 306246424,
	1733966678, 1884751139, 744509763, 400011959, 1440466707,
	1363416242, 973726663, 59253759, 1639096332, 336563455,
	1642837685, 1215013716, 154523136, 593537720, 704035832,
	1134594751, 1605135681, 1347315106, 302572379, 1762719719,
	269676381, 774132919, 1851737163, 1482824219, 125310639,
	1746481261, 1303742040, 1479089144, 899131941, 1169907872,
	1785335569, 485614972, 907175364, 382361684, 885626931,
	200158423, 1745777927, 1859353594, 259412182, 1237390611,
	48433401, 1902249868, 304920680, 202956538, 348303940,
	1008956512, 1337551289, 1953439621, 208787970, 1640123668,
	1568675693, 478464352, 266772940, 1272929208, 1961288571,
	392083579, 871926821, 1117546963, 1871172724, 1771058762,
	139971187, 1509024645, 109190086, 1047146551, 1891386329,
	994817018, 1247304975, 1489680608, 706686964, 1506717157,
	579587572, 755120366, 1261483377, 884508252, 958076904,
	1609787317, 1893464764, 148144545, 1415743291, 2102252735,
	1788268214, 836935336, 433233439, 2055041154, 2109864544,
	247038362, 299641085, 834307717
};
static List freenodes;		/* free list nodes */

char *stringn(str, n) char *str; {
	int i;
	unsigned int h;
	char *end;
	struct string *p;

	assert(str);
	for (h = 0, i = n, end = str; i > 0; i--)
		h = (h<<1) + scatter[*(unsigned char *)end++];
	h &= TABLESIZE-1;
	for (p = buckets[h]; p; p = p->link)
		if (n == p->len) {
			char *s1 = str, *s2 = p->str;
			do {
				if (s1 == end)
					return p->str;
			} while (*s1++ == *s2++);
		}
	if (next + n + 1 >= strlimit) {
		int m = roundup(n, 8) + BUFSIZE;
		next = alloc(m);
		strlimit = next + m;
	}
	p = (struct string *) alloc(sizeof *p);
	p->len = n;
	for (p->str = next; str < end; )
		*next++ = *str++;
	*next++ = 0;
	p->link = buckets[h];
	buckets[h] = p;
	return p->str;
}
char *string(str) char *str; {
	char *s;

	for (s = str; *s; s++)
		;
	return stringn(str, s - str);
}
char *stringd(n) {
	char str[30], *s = &str[30];
	unsigned m;

	if (n == INT_MIN)
		m = (unsigned)INT_MAX + 1;
	else if (n < 0)
		m = -n;
	else
		m = n;
	do
		*--s = m%10 + '0';
	while (m /= 10);
	if (n < 0)
		*--s = '-';
	return stringn(s, &str[30] - s);
}
/* append - append x to list, return new list */
List append(x, list) Generic x; List list; {
	List new;

	if (new = freenodes)
		freenodes = freenodes->link;
	else
		new = (List)alloc(sizeof *new);
	if (list) {
		new->link = list->link;
		list->link = new;
	} else
		new->link = new;
	new->x = x;
	return new;
}

/* length - # elements in list */
int length(list) List list; {
	int n = 0;

	if (list) {
		List lp = list;
		do n++;
		while ((lp = lp->link) != list);
	}
	return n;
}

/* ltoa - convert list to an 0-terminated array in a[0..length(list)] */
Generic *ltoa(list, a) List list; Generic a[]; {
	int i = 0;

	if (a == 0)
		a = (Generic *)talloc((length(list) + 1)*sizeof a[0]);
	if (list) {
		List lp = list;
		do {
			lp = lp->link;
			a[i++] = lp->x;
		} while (lp != list);
		lp = list->link;
		list->link = freenodes;
		freenodes = lp;
	}
	a[i] = 0;
	return a;
}

