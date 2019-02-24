/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	************************************
 *	************************************
 *	*** Initiate Auto Line Numbering ***
 *	*** R. B. Drake WH 8C-005 X4163 ****
 *	***** Sat Sep 22 17:58:51 1979 *****
 *	************************************
 *	************************************

*/
/*   "@(#) auto.c:  V 1.1  12/21/80" */

int aut,inc,autflg;
autnum()
{
	extern char *expr;
	char *prncpy();
	char txtbuf[80]; /* temporary buffer */
	autflg = 1;
	if(!inc) inc = 10; /*default values */
	aut += inc;
	if(*expr == '\0') return(0); /* no expression keep the defaults */
	expr = prncpy(txtbuf,expr); /* copy to first comma if there is one */
	if(txtbuf[0] != '\0') /* if a start num was specified */
	aut = atoi(txtbuf);
	if(*expr++ == ',') /* if an increment has been specified */
	prncpy(txtbuf,expr);
	inc = atoi(txtbuf);
	return(0);
}
