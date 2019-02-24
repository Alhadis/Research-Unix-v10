1c1
< static char junk[] = "\n@(#) LIBI77 VERSION pjw,dmg-mods 1 Feb. 1993\n";
---
> static char junk[] = "\n@(#) LIBI77 VERSION pjw,dmg-mods 23 June 1993\n";
140a141,162
> /* 5 Feb. 1993:  tweaks to NAMELIST: rsne.c: ? prints the namelist being
> 		 sought; namelists of the wrong name are skipped (after
> 		 an error message; xwsne.c: namelist writes have a
> 		 newline before each new variable.
> 		 open.c: ACCESS='APPEND' positions sequential files
> 		 at EOF (nonstandard extension -- that doesn't require
> 		 changing data structures). */
> /* 9 Feb. 1993:  Change some #ifdef MSDOS lines to #ifdef NON_UNIZ_STDIO.
> 		 err.c: under NON_UNIX_STDIO, avoid close(creat(name,0666))
> 		 when the unit has another file descriptor for name. */
> /* 4 March 1993: err.c, open.c: take declaration of fdopen from rawio.h;
> 		 open.c: always give f__w_mode[] 4 elements for use
> 		 in t_runc (in endfile.c -- for change of 1 Feb. 1993). */
> /* 6 March 1993: uio.c: adjust off-end-of-record test for sequential
> 		 unformatted reads to respond to err= rather than end=. */
> /* 12 March 1993: various tweaks for C++ */
> /* 6 April 1993: adjust error returns for formatted inputs to flush
> 		 the current input line when err=label is specified.
> 		 To restore the old behavior (input left mid-line),
> 		 either adjust the #definition of errfl in fio.h or
> 		 omit the invocation of f__doend in err__fl (in err.c).	*/
> /* 23 June 1993: iio.c: fix bug in format reversions for internal writes */
