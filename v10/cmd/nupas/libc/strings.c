extern char *index(), *rindex();

char*
strchr(s,c)
char *s;
int c;
{
	return index(s,c);
}
char*
strrchr(s,c)
char *s;
int c;
{
	return rindex(s,c);
}
