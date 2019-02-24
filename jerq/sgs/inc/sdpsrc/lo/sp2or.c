/*
 * static char ID_sp2rc[] = "@(#) sp2or.c: 1.1 1/8/82";
 */

sp2or(spacenumber)
int spacenumber;
{
	int scan;

	if( ((spacenumber != EMPTY) && (spacenumber<0)) || (spacenumber>MAXSPNUMB) )
		return(fatal("SP01 -- illegal address space number"));

	for(scan=0;scan<MAXATTACH;scan++)
		if(open_spacenumbers[scan] == spacenumber)
			return(scan);

	return(NOTFOUND);
}
