static char *classes[] = {

 "`a\310e\311i\331o\312u\313A\241E\243I\346O\350U\255",

 "'a\304e\305i\325o\306u\307A\340E\334I\345O\347U\355",

 "^a\300e\301i\321o\302u\303A\242E\244I\246O\337U\256s\354S\353",

"\"a\314e\315i\335o\316u\317A\330E\245I\247O\332U\333y\357Y\356",

 "~a\342o\352n\267A\341O\351N\266!\270?\271",

 ",c\265C\264", "/o\326O\322", "oa\324A\320",

 "ae\327", "AE\323", "<<\373", ">>\375",

 "-L\273", "=L\257Y\274",

 0 };

main()
{
	register unsigned char **l, *p;
	register unsigned c;
	for (l=(unsigned char **)&classes[0]; *l; l++) {
		c = *(p = *l);
		while (*++p) {
			printf("%c%c 1 %%\\%o\n", c, p[0], p[1]);
			p++;
		}
	}
	exit(0);
}
