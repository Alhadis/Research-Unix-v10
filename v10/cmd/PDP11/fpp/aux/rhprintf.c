/*
 * mjm: originally in rhftoa.c;
 *	separate out so can use /lib/libS.a printf
 */
/* printf here to force loading of pfloat is printf is called. */

printf(arg0)
char *arg0;
{
extern int putchar();
return(format(putchar,"%r",&arg0));
}

__dummy() {
extern pfloat(); /* force pfloat and pscien to be loaded */
pfloat();
}
