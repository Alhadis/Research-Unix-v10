/*
	openerr() returns the file descriptor for the error text file.
	this function exists so that it is easy for an installer to
	change the location of this file.
*/

int openerr()

{
	return	open( "/usr/lib/vaxspitv35.err\0",0 );
}

