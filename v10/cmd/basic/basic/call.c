bload()
{}



/* CALL statement handler */
call()
{}



/*
 *	clear out the memory taken up by the user subprograms
 */

clearsubs()
{}



/*
 *	create the ifile and call ld to create the a.out (with external
 *	references resolved back into basic when possible)
 */

do_ld(ptr,filenam)
char	*ptr,
	filenam[];
{}



/*
 *	findfuncs goes through the symbol table of an object file (.o),
 *	finds all the function names, and inserts them into BASIC's symbol
 *	table.  findfunc looks for external text symbols in the object file.
 *	This assures finding all function names defined in that file.  However
 *	it is also possible to find some data names in this manner.  This
 *	should not be a problem, since an error will result if a user tries
 *	to call the data name.
 */

findfuncs(filenam)
char	filenam[];
{}



/*
 *	assgnfuncs goes through the symbol table of the a.out that was formed
 *	by do_ld, finds function names, and hashes them into BASIC's symbol
 *	table.  If the name is found, the address associated with that name
 *	is stored in the symbol table.
 */

assgnfuncs(ldptr)
{}



do_usr(usrno)
int	usrno;
{}



defusr()
{}
