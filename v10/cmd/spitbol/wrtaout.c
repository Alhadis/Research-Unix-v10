/*
	wrtaout( headerp,memsize ) writes an executable a.out file containing
	the currently executing spitbol program. headerp is a pointer to
	the a.out header. memsize is the actual memory size of the executing
	program.
*/

int	wrtaout( headerp,memsize )

char	*headerp;
int	memsize;

{
	register int	excess, fd;
	long	lseek();


	unlink( "a.out" );

	if ( (fd = creat( "a.out",0777 )) < 0 )
		return	-1;

	/*
	 *	write a.out header
	 */

	if ( write( fd,headerp,32 ) != 32 )
		return	-2;

	/*
	 *	lseek out to 1024 byte boundary (full page).
	 */

	if ( lseek( fd,1024L,0 ) != 1024 )
		return -3;

	/*
	 *	write all of memory
	 */

	if ( write( fd,0,memsize ) != memsize )
		return	-4;

	/*
	 *	if memsize not multiple of 1024, force file size up to
	 *	this multiple.
	 */

	if ( (excess = memsize % 1024) != 0 ) {
		excess	= 1024 - excess;
		if ( lseek( fd,(long) excess - 1,1 ) < 0 ) 
			return	-5;
		if ( write( fd,"",1 ) != 1 )
			return	-6;
	}

	close( fd );

	return	0;

}
