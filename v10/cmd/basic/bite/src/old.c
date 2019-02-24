/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// old.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 06:24:20 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * CLEAR USER CORE AND LOAD BASIC PROGRAM
 */
/*   "@(#) old.c:  V 1.1  12/21/80" */
old()
{
	new();		/* clear user area */
	load();		/* load program */
}
