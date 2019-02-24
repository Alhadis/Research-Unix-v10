/*
	swcinp( filecnt,fileptr ) is responsible for switching between
	input files for standard input.  filecnt is the number of
	input filnames specified on the command line and fileptr is
	points to an array of character pointers.

	all files on the command line are read before the standard
	input provided by the shell.  after all input is exhausted
	a -1 is returned to indicate EOF.
*/

int	swcinp( filecnt,fileptr )
int	filecnt;
char	*fileptr[];

{
	static	int	curfile = -1,
			originp = 0;
	register int	fd;

	if (filecnt == 0) {
		if (originp == 0) {
			--originp;
			fd = 0;
		} else {
			fd = -1;
		}
	} else {
		if (originp == -1)
			return	originp;
		if (originp == 0)
			originp = dup( 0 );
		close( 0 );
		fd = -1;
		if (curfile < filecnt - 1) {
			do {
				fd = open( fileptr[++curfile],0 );
				if (fd != 0) {
					register int len = 0;
					write( 2,"can't open ",11 );
					while ( fileptr[curfile][len] != '\0' )
						len++;
					write( 2,fileptr[curfile],len );
					write( 2,"\n",1 );
				}
			} while ( fd != 0 && curfile < filecnt - 1 );
		}
		if ( curfile == filecnt - 1 && fd != 0 ) {
			if (originp > 0) {
				fd = dup( originp );
				close( originp );
				originp = -1;
			} else {
				fd = -1;
			}
		}
	}

	return fd;
}
