
typedef struct {
	int weight;
	char *string;
} Object;

#define Lt(a,b)	((a).weight < (b).weight)

#define sort(a,n)	ptsort(a, a+n-1)

ptsort(l, m)
Object *l, *m;
{
	register Object *i, *j; Object x, t;
	while (l < m) {
		i = l; j = m;
		x = i[(j-i)/2];
		while (i <= j) {
			while (Lt(*i, x)) i++;
			while (Lt(x, *j)) j--;
			if (i <= j) {
				t = *i;
				*i++ = *j;
				*j-- = t;
			}
		}
		if (j-l <= m-i) {
			ptsort(l, j);
			l = i;
		} else {
			ptsort(i, m);
			m = j;
		}
	}
}
