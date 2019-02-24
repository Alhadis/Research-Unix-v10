rfpanic(s, d1, d2, d3, d4, d5, d6, d7, d8, d9)
char *s;
int d1, d2, d3, d4, d5, d6, d7, d8, d9;
{
	rflog(s, d1, d2, d3, d4, d5, d6, d7, d8, d9);
	exit(1);
}
