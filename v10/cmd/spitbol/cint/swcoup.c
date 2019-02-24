
/*
	swcoup( oupptr ) switches between 2 output files:  the
	standard output file provided by the shell and the optional
	output file provided by the -o option.

	the switching is as follows:

	program listing, compilation statisitics, execution statistics,
	and dump of variables at termination go to the -o file, if
	specified.

	standard output produced by the executing program goes to the
	standard output file provided by the shell.
*/

int swcoup( oupptr )
char	*oupptr;

{
	static	int	errflag = 0,
			origoup = 0,
			state	= 0;
		int	fd,retval;

	retval	= 0;
	if (oupptr == 0)
		return	0;

	if (errflag != 0)
		return	0;

	switch ( state ) {

		/*	1st switch:  standard output -> -o file */

		case 0: origoup = dup( 1 );
				  close( 1 );
			fd	= creat( oupptr,0666 );
			if ( fd < 0 ) {
				write( 2,"-o file open error\n",19 );
				++errflag;
				fd	= dup( origoup );
				close( origoup );
				retval	= -1;
			}
			break;

		/*	2nd switch:  standard output -> shell output file  */

		case 1: close( 1 );
			fd	= dup( origoup );
			close( origoup );
			break;

		/*	3rd switch:  standard output -> -o file */

		case 2: close( 1 );
			fd	= open( oupptr,1 );
			if ( fd < 0 ) {
				write( 2,"error reopening\n",16 );
			}
				lseek( 1,0,2 );
			break;

		default:
			write( 2,"swcinp too many times\n",22 );
	}
	++state;
	return	retval;
}

