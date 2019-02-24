char *
strcat2(str1,str2)	/* returns str2 with str1 prepended */
char *str1, *str2;
{
	register char c, *src, *dst;

	src=str2; dst=str2+strlen(str1);
	do { *dst++ = c = *src++; } while (c != 0);

	src=str1; dst=str2;
	while ((c = *src++)!= 0) { *dst++ = c; };

	return str2;
}
