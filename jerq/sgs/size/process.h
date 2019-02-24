    /*  process.h contains format strings for printing size information
     *
     *  The output of size differs for Basic-16 and for New 3b:  20 bits
     *  of information are printed for Basic-16 while 32 bits are printed
     *  for new 3b.
     *
     *  In addition different format strings are used for hex, octal and decimal
     *  output.  The appropriate string is chosen by the value of numbase:
     *  pr???[0] for hex, pr???[1] for octal and pr???[2] for decimal.
     */


#ifdef B16
    /*  BASIC-16 FORMAT STRINGS */

static char	*prhead = 
    "\n\tSection       Size      Physical Address    Virtual Address\n\n";
/*	 12345678    12345678        12345678            1234567	*/
/*		 1234        12345678        123456789012		*/

static char	*prsect[3] = {
		"\t%-8.8s    0x%.5lx        0x%.5lx              0x%.4lx\n",
		"\t%-8.8s    0%.7lo        0%.7lo            0%.6lo\n",
		"\t%-8.8s    %7ld         %7ld              %5ld\n"
		};
#else
	/* ALL OTHER GENERICS */

static char	*prhead = 
    "\n\tSection         Size      Physical Address    Virtual Address\n\n";
/*	 12345678    123456789012    123456789012       123456789012	*/
/*		 1234            1234            1234567		*/

static char	*prsect[3] = {
		"\t%-8.8s     0x%.8lx      0x%.8lx         0x%.8lx\n",
		"\t%-8.8s    0%.11lo    0%.11lo       0%.11lo\n",
		"\t%-8.8s     %10ld      %10ld         %10ld\n"
		};

#endif

#ifdef UNIX
static char *prusect[3] = {
	"%lx",
	"%lo",
	"%ld"
	};

static char *prusum[3] = {
	" = 0x%lx\n",
	" = 0%lo\n",
	" = %ld\n"
	};
#endif

/*
 *	static char ID_proch[] = "@(#) process.h: 1.4 3/5/83";
 */
