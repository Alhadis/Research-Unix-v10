/*ident	"@(#)ctrans:demangler/dem.c	1.1"*/
/*
 * C++ Demangler Source Code
 * @(#)master	1.5
 * 7/27/88 13:54:37
 */
/* Demangle a single command line argument */
main(argc,argv)
int argc;
char **argv;
{
	int i;
	for(i=1;i<argc;i++)
		printf("%s == %s\n",argv[i],demangle(argv[i]));
}
