/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	****** STRING CONCATINATION *******
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Sun Aug 26 11:42:03 1979 *****
 *	***********************************
 *	***********************************
 * This routine is similar to the UNIX 'strcat' except that the pointer
 * it returns is to the end of the resultant string instead of the
 * beginning. This is useful when several 'strcat' calls are used in
 * succession to build a string. With the UNIX version the entire string
 * must be scanned from the beginning to find the end. With this one
 * we already have the end after the first call.

*/
/*   "@(#) strcat.c:  V 1.1  12/21/80" */

char *Strcat(s,t)
char *s,*t;
{
	register char *s1,*t1;
	s1=s;t1=t;
	while(*s1) s1++;
	while(*t1)
		*s1++= *t1++;
	*s1='\0';
	return(s1);
}
/* add a char to a string and return ptr to end of string */
char *carcat(s,t)
char *s,t;
{
	char register *s1;
	s1=s;
	while(*s1) s1++;
	*s1++ = t;
	*s1='\0';
	return(s1);
}
