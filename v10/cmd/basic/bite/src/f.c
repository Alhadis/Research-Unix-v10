/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	******************************************
 *	******************************************
 *	*** SIMILAR TO UNIX EDITOR 'f' COMMAND ***
 *	****** R. B. Drake WH 8C-005 X4163 *******
 *	******** Fri Aug 24 17:23:16 1979 ********
 *	******************************************
 *	******************************************

*/
/*   "@(#) f.c:  V 1.1  12/21/80" */

/* print the current default filename */
extern char filnam[];
f()
{
	if(filnam[0] == '\0')
		printf("No current filename exists\n");
	else
	printf("Current filename is %s\n",filnam);
	return(0);
}
