/* defines (sort of) protocol between host and term */

#define	P_CLEAR	'c'	/* clear screen */

#define	P_DEFINE 'd'	/* define something */

#define	P_FILE	'f'	/* filename follows (term to host) */

#define	P_INIT	'i'	/* initialize terminal */

#define	P_OBJECT 'o'	/* object definition follows */
			/* objects are 'b', 'c', etc. */
#define	P_PRINT	'p'	/* print string */

#define	P_QUIT	'q'	/* all done */

#define	P_LINE	'L'	/* expect a sequence of pairs back from defining a line */

#define	P_ENDLINE 'e'	/* terminated by this */

#define	P_ENDFILE '.'	/* end of data */

#define P_ERROR '?'	/* error in filename, etc. */

#define	P_ERRPRINT '!'	/* an error string follows */

/* options for various commands */

#define	Tcenter	10
#define	Tljust	20
#define	Trjust	30
#define	Tabove	40
#define	Tbelow	50
#define	Tsmall	1
#define	Tmedium	2
#define	Tbig	3
#define	Tbigbig	4

int	textops[] ={	'c', Tcenter, 'l', Tljust, 'r', Trjust,
			's', Tsmall, 'm', Tmedium, 'b', Tbig, 'B', Tbigbig, 0 };

#define	Lsolid	10
#define	Lfat	20
#define	Lfatfat	30
#define	Ldotted	40
#define	Ldashed	50
#define	Lline	1
#define	Larrow1	2
#define	Larrow2	3
#define	Larrow3	4

int	lineops[] ={	's', Lsolid, 'f', Lfat, 'F', Lfatfat, 'o', Ldotted, 'a', Ldashed,
			'-', Lline, '>', Larrow1, '<', Larrow2, 'x', Larrow3, 0 };

#define	Bnofill	1
#define	Bfill	2

int	boxops[] ={	'n', Bnofill, 'f', Bfill, 0 };

#define	Cnofill	1
#define	Cfill	2

int	circops[] ={	'n', Cnofill, 'f', Cfill, 0 }; 
