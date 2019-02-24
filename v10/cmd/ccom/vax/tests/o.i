m(){
	register char *s;
	register c;
	*s++ = (c&0200? c : '/');
}
typedef struct P0 {
	unsigned L9 : 8;		
} P0;
P0 x;
unsigned char sv[512];
addproc(n)
{	int j, i;
	((P0 *)(&sv[i]))->L9 = j = addqueue();
}
a()
{	int j;
	(&x)->L9 = j = addqueue();
}
