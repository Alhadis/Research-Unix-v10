/*
 * static char ID_ftlc[] = "@(#) fatal.c: 1.1 1/7/82";
 */

fatal(msg)
char *msg;
{

	lderror(2, 0, NULL, msg);

	return( -1 );
}
