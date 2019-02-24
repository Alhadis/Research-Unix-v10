/*
 *	static	char	dis_utls_ID[] = "@(#)dis_utls.c	1.25 12/15/83";
 */
#define	DEBUG
#include	<stdio.h>
#include	<ar.h>
#include	"dis.h"
#include	"filehdr.h"
#include	"ldfcn.h"
#include	"sgs.h"
#include	"scnhdr.h"
#include	"linenum.h"
#include	"syms.h"
#include	"structs.h"
#ifdef OLIST
#include	"olist_defs.h"
#endif

#ifndef OLIST
#define TOOL	"dis"
#endif
/*
 * The following #define is necessary so that the disassembler
 * will run on UNIX 370 3.0 and UNIX 370 4.0 (printf was changed).
 *
 */
#ifndef UNIX
#define UNIX 3
#endif

#define		FUNC_NM	0	/* for line_nums func, indicates the	*/
				/* line number entry value for the 	*/
				/* beginning of a function		*/
#define		BADADDR	-1L	/* used by the resynchronization	*/
				/* function to indicate that a restart	*/
				/* candidate has not been found		*/

#ifdef OLIST
extern	struct	files	*curfile;
extern	char		linebuf[];
#endif

/*
 *	This structure is used for storing data needed for the looking up
 *	of symbols for labels or functions.  It consists of a pointer to a
 *	structure describing a symbol table entry, and a pointer to another
 *	structure of its own type (a forward linked list will be created).
 */

struct	SYMLINK {
	SYMENT	*element;
	struct	SYMLINK	*next;
};

/*
 *	To look up labels, a linked list of 'struct SYMLINK' will be
 *	searched.  'num_labels' will be set to the number of label
 *	entries that exist, and 'head_ptr' will point to the head
 *	of this list.
 */

static	int	num_labels = 0;
static	struct	SYMLINK	*head_ptr;
#ifdef OLIST
static	int	firsttime = 1;
#endif

/*
 *	To store the names and addresses of all the functions in the symbol
 *	table, SYMLINK is used so that all symbol information is collected
 *	for flexname purposes.
 *	In the previous version, only the name and address of the symbol
 *	was saved.
 */


extern	unsigned short	num_funcs;
static	struct SYMLINK	*func_ptr;

/*
 *	To store the name of each section and associate it with its section
 *	number, an array of pointers is used.
 */

static	char	**section;

/*
 *	The following declarations provide space for the current
 *	lint number entry and the number of line numbers that have
 *	been read
 */
static	LINENO	line_ent;
static	unsigned short	lnno_cnt;

FUNCLIST	*currfunc;

/*
 *	build_labels ()
 *
 *	Construct an array containing all the label entries in the 
 *	symbol table. (This will be a forward linked structure.)
 *	Then construct an array containing each section name,
 *	associating the section name with its section number by its 
 *	index.  This is needed in looking up the labels.
 */

build_labels()
{
	/* the following items are from _extn.c */
	extern	FILHDR	filhdr;
	extern	LDFILE	*f_ptr,
			*symb;
	extern	char	*fname;
	extern	int	trace,
			Lflag,
			save_Lflag;
	SCNHDR	sect_hdr;
	LDFILE	*sect_ptr;
	unsigned short	j;
	long	limit;
	struct	SYMLINK	*current;
	char	*calloc();
	SYMENT	sym_ent;
	SYMENT	*sym_ptr = &sym_ent;
	long	sym_cnt;

	if ((filhdr.f_flags & F_LSYMS) || (filhdr.f_symptr == NULL)) {
		printf("\nfile %s has been stripped of labels\n",fname);
		/* if labels are stripped off save the value of Lflag */
		save_Lflag = Lflag;
		Lflag = -1;
		return;
	}

	limit = filhdr.f_nsyms;		/* # of symbol table entries	*/

	/*
	 * Allocate for link structure of first element.
	 */

	if ((head_ptr = (struct SYMLINK *) calloc(1,sizeof(struct SYMLINK))) == NULL) {
		fprintf( stderr, "memory allocation failure on call to calloc");
		exit(0);
	}

	current = head_ptr; /* point to first entry */

	/*
	 * Allocate for symbol structure of first element.
	 */

	if ((current->element = (SYMENT *) calloc(1,SYMESZ)) == NULL) {
		fprintf( stderr, "memory allocation failure on call to calloc");
		exit(0);
	}

	/*
	 * initialize all needed for get_sym()
	 */

	ldtbseek(symb);

	/*
	 * Search the whole symbol table looking for elements
	 * with a storage class of label (C_LABEL)
	 */

	for( sym_cnt = 0; sym_cnt < limit; sym_cnt++ ) {
		if (ldtbread( symb, sym_cnt, sym_ptr ) == FAILURE)
		{
			fprintf( stderr, "%s%s: %s: cannot read symbol table\n",
				SGS, TOOL, fname );
			exit(1);
		}
		if (trace > 0) {
			printf("\nsym name %s\tsec # %d\tstor cl %d\n",
				sym_ptr->n_name,
				sym_ptr->n_scnum,
				sym_ptr->n_sclass);
		}
		if (sym_ptr->n_sclass == C_LABEL)  {

			/* if it's a label add it to the list */

#ifdef	FLEXNAMES
			current->element->n_zeroes=sym_ptr->n_zeroes;
			current->element->n_offset=sym_ptr->n_offset;
#else
			strcpy(current->element->n_name,sym_ptr->n_name);
#endif
			current->element->n_value = sym_ptr->n_value;
			current->element->n_scnum = sym_ptr->n_scnum;
			current->element->n_type = sym_ptr->n_type;
			current->element->n_sclass = sym_ptr->n_sclass;
			current->element->n_numaux = sym_ptr->n_numaux;

			/*
			 * Obtain space for another link structure.
			 */

			if ((current->next = (struct SYMLINK *) calloc(1,sizeof(struct SYMLINK))) == NULL) {
				fprintf( stderr, "memory allocation failure on call to calloc");
				exit(0);
			}

			current = current->next;

			/*
			 * Obtain space for another symbol element.
			 */

			if ((current->element = (SYMENT *) calloc(1,SYMESZ)) == NULL) {
				fprintf( stderr, "memory allocation failure on call to calloc");
				exit(0);
			}
			num_labels++;
		}
		sym_cnt += sym_ptr->n_numaux;
	}
	current->next = NULL;

	sect_ptr = NULL;
	sect_ptr = ldaopen(fname,f_ptr);

	/*
	 * now build an array to store the pointers to the names of
	 * each section.  First allocate a pointer for each section.
	 */

	if ((section = (char **) calloc(filhdr.f_nscns + 1, sizeof(section))) == NULL) {
		fprintf( stderr, "memory allocation failure on call to calloc");
		exit(0);
	}

	/*
	 * now have section[0] point to a block of memory big enough to
	 * hold all the section names (their size has 1 added to
	 * account for a trailing '\0').
	 */

	if ((*section = calloc ( filhdr.f_nscns + 1, (sizeof(sect_hdr.s_name)+1))) == NULL) {
		fprintf( stderr, "memory allocation failure on call to calloc");
		exit(0);
	}

	/*
	 * now read and assign the section names.  One will note that
	 * the allocated memory for the first name is not used.  This
	 * does not cause us to run out of space, though, since '1'
	 * was added above in allocating space.
	 */

	for (j = 1; j < filhdr.f_nscns; j++) {
		ldshread(sect_ptr, j, &sect_hdr);
		section[j] = section[j-1] + sizeof(sect_hdr.s_name) + 1;
		sprintf(section[j],"%-.8s",sect_hdr.s_name);
	}
	ldaclose(sect_ptr);
}

/*
 *	compoff (lng, temp)
 *
 *	This routine will compute the location to which control is to be
 *	transferred.  'lng' is the number indicating the jump amount
 *	(already in proper form, meaning masked and negated if necessary)
 *	and 'temp' is a character array which already has the actual
 *	jump amount.  The result computed here will go at the end of 'temp'.
 *	(This is a great routine for people that don't like to compute in
 *	hex arithmetic.)
 */

compoff(lng, temp)
long	lng;
char	*temp;
{
	extern	int	oflag;	/* from _extn.c */
	extern	long	loc;	/* from _extn.c */

	lng += loc;
	if (oflag)
		sprintf(temp,"%s <%lo>",temp,lng);
	else
		sprintf(temp,"%s <%lx>",temp,lng);
}
/*
 *	convert (num, temp, flag)
 *
 *	Convert the passed number to either hex or octal, depending on
 *	the oflag, leaving the result in the supplied string array.
 *	If  LEAD  is specified, preceed the number with '0' or '0x' to
 *	indicate the base (used for information going to the mnemonic
 *	printout).  NOLEAD  will be used for all other printing (for
 *	printing the offset, object code, and the second byte in two
 *	byte immediates, displacements, etc.) and will assure that
 *	there are leading zeros.
 */

convert(num,temp,flag)
unsigned	num;
char	temp[];
int	flag;

{
	extern	int	oflag;		/* in _extn.c */

#if UNIX < 4
#ifdef B16
	if (flag == NOLEAD) 
		(oflag) ?	sprintf(temp,"%03o",num):
				sprintf(temp,"%02x",num);
#else
	if (flag == NOLEAD)
		(oflag) ?	sprintf(temp,"%06o",num):
				sprintf(temp,"%04x",num);
#endif	/* B16 */
#else
#ifdef B16
	if (flag == NOLEAD) 
		(oflag) ?	sprintf(temp,"%.3o",num):
				sprintf(temp,"%.2x",num);
#else
	if (flag == NOLEAD)
		(oflag) ?	sprintf(temp,"%.6o",num):
				sprintf(temp,"%.4x",num);
#endif	/* B16 */
#endif	/* UNIX < 4 */
	if (flag == LEAD)
		(oflag) ?	sprintf(temp,"0%o",num):
				sprintf(temp,"0x%x",num);
}
/*
 *	dis_data ()
 *
 *	the routine to disassemble a data section,
 *	which consists of just dumping it with byte offsets
 */

dis_data( sectp )
SCNLIST	*sectp;

{
	extern	SCNHDR	scnhdr;	 /* from _extn.c */
	extern	int	oflag;	 /* from _extn.c */
	extern	int	trace;	 /* from _extn.c */
	extern	short	aflag;	 /* from _extn.c */
	extern	long	loc;	 /* from _extn.c */
	extern	char	mneu[];	 /* from _extn.c */
	extern	char	object[];/* from _extn.c */
	extern	unsigned short cur2bytes; /* from _extn.c */
	extern	LDFILE	*f_ptr;
	extern	char	*fname;

	LDFILE		*d_ptr;
	short		count;
	long		last_addr;

	scnhdr = sectp->shdr;
	d_ptr = ldaopen( fname, f_ptr );
	ldsseek( d_ptr, sectp->scnum );

	/* Blank out mneu so the printline routine won't print extraneous
	 * garbage.
	 */

	sprintf(mneu,"");
	if (scnhdr.s_nlnno  != 0){
		fprintf(stderr,"\n%sdis: QUIRK--data section has line number entries",SGS);
		printf("\n        Are you sure this was a data section?!\n");
	}

	if (trace > 0){
		printf("\ntrace: section name %s\n",scnhdr.s_name);
		printf("\ntrace: section size %x\n",scnhdr.s_size);
	}

	for (loc = aflag? scnhdr.s_paddr: 0, last_addr = loc + scnhdr.s_size;
	    loc < last_addr; printline()) {
		/* if -da flag specified, actual adress will be printed
		   if -d flag specified, offset within section will be printed */
		printf("\t");
		prt_offset();
#ifdef B16
		for (count=0; (count<6) && (loc<last_addr); count++)
			getbyte(d_ptr);
#endif
#ifdef MC68
		for (count=0; (count<8) && (loc<last_addr); count+=2)
			get2bytes(d_ptr);
#endif
#ifdef N3B
		for (count=0; (count<8) && (loc<last_addr); count+=2)
			get2bytes(d_ptr);
#endif
#ifdef M32
		for (count=0; (count<8) && (loc<last_addr); count+=2)
			/* bytes of data are swapped so they must be swapped back */
			getswapb2(d_ptr);
#endif /* M32 */
	}

}
#if U3B | N3B | U3B5 | M32
/*
 *	dfpconv(fpword1, fpword2, fpdoub, fpshort)
 *
 *	This routine will convert the 2 longs (64 bit) "fpword1 fpword2" double  
 *	precision floating point representation of a number into its decimal 
 *	equivalent. The result will be stored in *fpdoub. The routine will
 *	return a value indicating what type of floating point number was
 *	converted.
 *	*NOTE*	The conversion routine will calculate a decimal value
 *		if the disassembler is to run native mode on the 3B.
 *		If the 3B disassembler is to be run on a DEC processor
 *		(pdp11 or vax) the routine will store the exponent in
 *		*fpshort. The mantissa will be stored in the form:
 *		"T.fraction" where "T" is the implied bit and the
 *		fraction is of radix 2. The mantissa will be stored
 *		in *fpdoub. This is due to the difference in range
 *		of floating point numbers between the 3B and DEC
 *		processors.
 */
dfpconv(fpword1, fpword2, fpdoub, fpshort)
long	fpword1, fpword2;
double	*fpdoub;
short	*fpshort;
{
	unsigned short exponent;
	short	leadbit, signbit;
	double	dtemp, dec2exp, mantissa;
	long	ltemp;

	exponent = (unsigned short)((fpword1>>20) & 0x7ffL);
	/* exponent is bits 1-11 of the double	*/

	ltemp = fpword1 & 0xfffffL;	/* first 20 bits of mantissa */
	mantissa = ((double)ltemp * TWO_32) + (double)fpword2;
	/* mantissa is bits 12-63 of the double	*/

	signbit = (short)((fpword1>>31) & 0x1L);
	/* sign bit (1-negative, 0-positive) is bit 0 of double	*/

	leadbit = 1;
	/* implied bit to the left of the decimal point	*/

	if (exponent==2047)
		if (mantissa)
			return(NOTANUM);
		else
			return((signbit)? NEGINF: INFINITY);

	if (exponent==0)
		if (mantissa)
		/* This is a denormalized number. The implied bit to 
		 * the left of the decimal point is 0.
		 */
			leadbit = 0;
		else
			return((signbit)? NEGZERO: ZERO);

	/*
	 * Convert the 52 bit mantissa into "I.fraction" where
	 * "I" is the implied bit. The 52 bits are divided by
	 * 2 to the 52rd power to transform the mantissa into a
	 * fraction. Then the implied bit is added on.
	 */
	dtemp = (double)(leadbit + (mantissa/TWO_52));

#if U3B | U3B5
	/* Calculate 2 raised to the (exponent-BIAS) power and
	 * store it in a double.
	 */
	if (exponent < DBIAS)
		for(dec2exp=1; exponent < DBIAS; ++exponent)
			dec2exp /= 2;
	else
		for(dec2exp=1; exponent > DBIAS; --exponent)
			dec2exp *= 2;

	/* Multiply "I.fraction" by 2 raised to the (exponent-BIAS)
	 * power to obtain the decimal floating point number.
	 */
	*fpdoub = dtemp *dec2exp;

	if (signbit)
		*fpdoub = -(*fpdoub);
	return(FPNUM);
#else
	*fpshort = exponent - DBIAS;
	*fpdoub = ((signbit)? (-dtemp): dtemp);
	return(FPBIGNUM);
#endif /* U3B | U3B5 */
}
#endif /* U3B | N3B | U3B5 | M32 */

/*
 *	getbyte (ptr)
 *
 *	read a byte, mask it, then return the result in 'curbyte'.
 *	The getting of all single bytes is done here.  The 'getbyte[s]'
 *	routines are the only place where the global variable 'loc'
 *	is incremented.
 */
int
getbyte(ptr)
LDFILE	*ptr;

{
	extern	long	loc;		/* from _extn.c */
	extern	unsigned short curbyte;	/* from _extn.c */
	extern	char	object[];	/* from _extn.c */
	char	temp[NCPS+1];
	char	byte;

	if ( FREAD(&byte,sizeof(byte),1,ptr) == NULL){
		fprintf(stderr,"\n%s%s: premature EOF\n",SGS,TOOL);
		exit(4);
	}
	loc++;
	curbyte = byte & 0377;
	convert(curbyte, temp, NOLEAD);
	sprintf(object,"%s%s ",object,temp);
}
/*
 *	lookbyte (ptr)
 *
 *	read a byte, mask it, then return the result in 'curbyte'.
 *	The byte is not immediately placed into the string object[].
 *	is incremented.
 */

int
lookbyte(ptr)
LDFILE	*ptr;

{
	extern	long	loc;		/* from _extn.c */
	extern	unsigned short curbyte;	/* from _extn.c */
	char	byte;

	if ( FREAD(&byte,sizeof(byte),1,ptr) == NULL){
		fprintf(stderr,"\n%s%s: premature EOF\n",SGS,TOOL);
		exit(4);
	}
	loc++;
	curbyte = byte & 0377;
}

/*
 *	get2bytes(ptr)
 *
 *	This routine will get 2 bytes, print them in the object file
 *	and place the result in 'cur2bytes'.
 */

int
get2bytes(ptr)
LDFILE	*ptr;
{
	extern	long	loc;		/* from _extn.c */
	extern	unsigned short cur2bytes; /* from _extn.c */
	extern	char	object[];	/* from _extn.c */
	char	temp[NCPS+1];
	extern	int	trace;

	if ( FREAD(&cur2bytes,sizeof(cur2bytes),1,ptr) == NULL){
		fprintf(stderr,"\n%s%s: premature EOF\n",SGS,TOOL);
		exit(4);
	}
	loc += 2;
	convert( (cur2bytes & 0xffff), temp, NOLEAD);
	sprintf(object,"%s%s ",object, temp);
	if (trace > 1)
		printf("\nin get2bytes object<%s>\n",object);
}
#ifdef M32
/*
 *	getswapb2(ptr)
 *
 *	This routine is used only for m32 and m32a. It will get and
 *	swap 2 bytes, print them in the object file and place the
 *	result in 'cur2bytes'.
 */

int
getswapb2(ptr)
LDFILE	*ptr;
{
	extern	long	loc;		/* from _extn.c */
	extern	unsigned short cur2bytes; /* from _extn.c */
	extern	char	object[];	/* from _extn.c */
	char	temp[NCPS+1];
	extern	int	trace;

	if ( FREAD(&cur2bytes,sizeof(cur2bytes),1,ptr) == NULL){
		fprintf(stderr,"\n%s%s: premature EOF\n",SGS,TOOL);
		exit(4);
	}
	loc += 2;
	/* swap the 2 bytes contained in 'cur2bytes' */
	cur2bytes = ((cur2bytes>>8) & (unsigned short)0x00ff) |
		((cur2bytes<<8) & (unsigned short)0xff00);
	convert( (cur2bytes & 0xffff), temp, NOLEAD);
	sprintf(object,"%s%s ",object, temp);
	if (trace > 1)
		printf("\nin getswapb2 object<%s>\n",object);
}
#endif
/*
 *	get4bytes(ptr)
 *
 *	This routine will bet 4 bytes, print them in the object and
 *	place the results in 'cur4bytes'.
 */

get4bytes(ptr)
LDFILE	*ptr;
{
	extern	long	loc;		/* from _extn.c */
#ifdef AR32WR
	extern	unsigned long	cur4bytes;	/* from _extn.c */
#else
	extern	long		cur4bytes;
#endif
	extern	unsigned short cur2bytes;	/* from _extn.c */
	extern	char	object[];	/* from _extn.c */
	char	temp[NCPS+1];
	unsigned	send;
	extern	int	trace;			/* in _extn.c	*/
#ifdef AR32WR
	unsigned long	tempvax;
#endif

	if ( FREAD(&cur4bytes,sizeof(cur4bytes),1,ptr) == NULL){
		fprintf(stderr,"\n%s%s: premature EOF\n",SGS,TOOL);
		exit(4);
	}

	loc += 4;
#ifdef AR32WR
	tempvax = (( cur4bytes >>16 ) & 0xffffL);
	cur4bytes = (( cur4bytes <<16 ) | tempvax);
#endif
	send = (cur4bytes >> 16) & 0xffffL;
	convert(send, temp, NOLEAD);
	sprintf(object,"%s%s ",object,temp);
	cur2bytes = cur4bytes & 0xffffL;
	send = cur4bytes  & 0xffffL;
	convert(send, temp, NOLEAD);
	sprintf(object,"%s%s ",object,temp);
	if (trace > 1)
		printf("\nin get4bytes object<%s>\n",object);
}
/*
 *	label_free ()
 *
 *	Free all the space that has been allocated which would be the space
 *	for section names plus those label entries that are not already free.
 */

label_free()

{
	struct	SYMLINK	*temp;
	int	i;

	cfree(section[0]);
	cfree(section);

	for (i = 0; i < num_labels; i++) {
		temp = head_ptr;
		head_ptr = head_ptr->next;
		free(temp->element);
		free(temp);
	}

}
/*
 *	lib_free ()
 *
 *	After all the files have been processed, free the space that was
 *	allocated for libraries that were disassembled.
 */

lib_free()

{
	/* the following array is from _extn.c */
	extern	char	*libs[];
	extern	int	lflag;		/* from _extn.c */
	int	i;

	for (i = lflag; i > 0; i--)
		free(libs[i]);

}

/*
 *	line_nums ()
 *
 *	This function prints out the names of functions being disassembled
 *	and break-pointable line numbers.  First it checks the address
 *	of the next function in the list of functions; if if matches
 *	the current location, it prints the name of that function.
 *
 *	It then examines the line number entries. If the address of the
 *	current line number equals that of the current location, the
 *	line number is printed.
 */
line_nums()
{
	/* the following items are from _extn.c */
	extern	LDFILE	*l_ptr,
			*t_ptr,
			*symb,
			*symb_ptr;

	extern	SCNHDR	scnhdr;
	extern	FILHDR	filhdr;
	extern	int	trace;
	extern	int	oflag;
#ifndef OLIST
	extern	int	fflag;
#endif
	extern	long	loc;
	extern	FUNCLIST	*next_function;
	extern	FUNCLIST	*currfunc;
#ifdef OLIST
	extern	int	nosource;
	extern	int	funcs_listed;
#endif

	SYMENT	sym_ent;
	SYMENT	*sym_ptr = &sym_ent;
	long	sym_cnt;
	char	*flexname, /* pointer to flexname returned by ldgetname */
		*ldgetname();
	LINENO	*lnno_ptr = &line_ent;

#ifdef OLIST
	linebuf[0] = '\0';
#endif

	while (next_function != NULL)
	{
		/* not there yet */
		if (loc < next_function->faddr)
			break;

		if (loc > next_function->faddr)
		{
			/* this is an error condition */
			fflush( stdout );
			fprintf( stderr, "\nWARNING: Possible strings in text or bad physical address before location ");
			if (oflag) 
				fprintf( stderr, "0%lo\n", loc );
			else
				fprintf( stderr, "0x%lx\n", loc );
			FSEEK( t_ptr, (next_function->faddr - scnhdr.s_paddr) + scnhdr.s_scnptr, 0);
			loc = next_function->faddr;
		}

		/* loc == next_function->addr, so print the function name */
#if OLIST
		funcs_listed++;
		if (nosource == TRUE)
			printf( "\t" );
		else if (curfile->f_nam[ strlen( curfile->f_nam ) -1] == 's')
			printf( "\n\@FUNCTION: %s\n\n", next_function->funcnm );
#endif
		printf("%s()\n", next_function->funcnm );
		currfunc = next_function;
		next_function = next_function->nextfunc;
	}

	if (scnhdr.s_lnnoptr != NULL)
	{
#if OLIST
		/* read the first one for the section */
		if (loc == scnhdr.s_paddr || firsttime)
		{
			firsttime = 0;
			FSEEK( l_ptr, scnhdr.s_lnnoptr, 0 );
			lnno_cnt = 0;
			if (FREAD( (char *) lnno_ptr, LINESZ,1, l_ptr ) != 1)
				fatal( "cannot read line numbers" );
		}
#else
		if (loc == scnhdr.s_paddr)
		{
			if (fflag)  {
				if ((ldlinit(l_ptr, currfunc->fcnindex)) == FAILURE)
					return;
			}  else
				FSEEK( l_ptr, scnhdr.s_lnnoptr, 0 );
			lnno_cnt = 0;
			if (FREAD( (char *) lnno_ptr, LINESZ,1, l_ptr ) != 1)
				fatal( "cannot read line numbers" );
		}
#endif

		for ( ; lnno_cnt < scnhdr.s_nlnno; )
		{
			if (lnno_ptr->l_lnno == FUNC_NM)
			{
				if (currfunc && lnno_ptr->l_addr.l_symndx == currfunc->fcnindex)
				{
					/* if the function matches the last one
					 * printed, skip the line number entry
					 */
					if (FREAD((char *) lnno_ptr, LINESZ,1, l_ptr ) !=1 )
						fatal( "cannot read line numbers" );
					lnno_cnt++;
				}
				else
				{
					/* should never get here.  All of the
					 * functions should be in the function
					 * list, but the code is here just
					 * in case
					 */
					if (ldtbread( symb_ptr, lnno_ptr->l_addr.l_symndx, sym_ptr ) == FAILURE)
						fatal( "cannot read symbol table");
					if (sym_ptr->n_value > loc)
						break;
					if (sym_ptr->n_value < loc)
#if OLIST
						if (sym_ptr->n_value < curfile->tbegin)
						{
							lnno_cnt += 1;
							if (FREAD ((char *) lnno_ptr, LINESZ, 1, l_ptr ) != 1)
								fatal("cannot read line numbers");
							continue;
						}
						else
#endif
						{
							fflush( stdout );
							fprintf( stderr, "\nWARNING: Possible strings in text or bad physical address before location " );
							if (oflag)
								fprintf( stderr, "0%lo\n", loc);
							else
								fprintf(stderr, "0x%lx\n", loc);
							FSEEK(t_ptr, (sym_ptr->n_value - scnhdr.s_paddr) + scnhdr.s_scnptr, BEGINNING);
							loc = sym_ptr->n_value;
						}

					/* the address of the function matches so print its name */
					if(!(flexname = ldgetname(symb_ptr, sym_ptr)))
						fprintf(stderr, "ldgetname returned 0\n");
#ifdef OLIST
					if (nosource == TRUE )
						printf("\t");
					else if (curfile->f_nam[strlen(curfile->f_nam)-1] =='s')
						printf("\n\\@FUNCTION: %s\n\n", flexname);
#endif
					printf("%s()\n",flexname);
					lnno_cnt += 1;
					if (FREAD( lnno_ptr, LINESZ, 1, l_ptr ) != 1)
						fatal( "cannot read line numbers\n");
				}
			}
			else
			{
				/* this is just a line number entry */
#ifdef OLIST
				if ((loc == lnno_ptr->l_addr.l_paddr) && (nosource != TRUE))
					sprintf(linebuf, "%s[%-d%]", linebuf, lnno_ptr->l_lnno);
#else
				if (loc == lnno_ptr->l_addr.l_paddr)
					printf("[%d]",lnno_ptr->l_lnno);
#endif
				else if (loc < lnno_ptr->l_addr.l_paddr)
					break;
				if (FREAD( lnno_ptr, LINESZ, 1, l_ptr ) != 1)
					fatal( "cannot read line numbers\n" );
				lnno_cnt++;
			}
		}
	}

#ifndef OLIST
	printf("\t");
#endif
}

/*
 *	looklabel (addr, sect_num)
 *
 *	This function will look in the symbol table to see if
 *	a label exists which may be printed.
 */

looklabel(addr, sect_num)
long	addr;
unsigned short	sect_num;

{
	struct	SYMLINK	*current,
			*previous;
	int	i;
	extern	LDFILE	*symb_ptr;
	char	*flexname,
		*ldgetname();

	current = head_ptr;
	previous = current;

	for (i = 0; i < num_labels; i++) {

		/* both the address and section number must match	*/
		if (current->element->n_value == addr &&
			current->element->n_scnum == sect_num) {

			/* found the label so print it	*/
			if(!(flexname = ldgetname(symb_ptr, current->element))) {
				fprintf(stderr, "ldgetname returned 0\n");
			}
			printf("%s:\n",flexname);

			/* now free up the space occupied by the	*/
			/* found element				*/

			if (current == head_ptr)
				head_ptr = head_ptr->next;

			previous->next = current->next;
			free (current->element);
			free(current);
			num_labels--;
			return;
		}
		/* didn't find a label yet, so go to the next label	*/
		previous = current;
		current = current->next;
	}
}

/*
 *	printline ()
 *
 *	Print the disassembled line, consisting of the object code
 *	and the mnemonics.  The breakpointable line number, if any,
 *	has already been printed, and 'object' contains the offset
 *	within the section for the instruction.
 */

printline()

{
	extern	int	oflag;		/* in _extn.c */
	extern  char	object[];
	extern	char	mneu[];
#ifdef OLIST
	extern	int	nosource;
#endif

#ifdef B16
	(oflag > 0) ?
		printf("%-36s%s\n",object,mneu):	/* to print octal */
		printf("%-30s%s\n",object,mneu);	/* to print hex */
#endif
#ifdef MC68
	(oflag > 0) ?
		printf("%-36s%s\n",object,mneu):	/* to print octal */
		printf("%-37s%s\n",object,mneu);	/* to print hex */
#endif
#ifdef N3B
#ifdef OLIST
	if (nosource == TRUE) return;
	printf("%-46s%-8s%s\n", object, linebuf, mneu);
#else
	(oflag > 0) ?
		printf("%-36s%s\n",object,mneu):	/* to print octal */
		printf("%-37s%s\n",object,mneu);	/* to print hex */
#endif
#endif
#ifdef M32
	(oflag > 0) ?
		printf("%-36s%s\n",object,mneu):	/* to print octal */
		printf("%-37s%s\n",object,mneu);	/* to print hex */
#endif /* M32 */
}
/*
 *	prt_offset ()
 *
 *	Print the offset, right justified, followed by a ':'.
 */


prt_offset()

{
	extern	long	loc;	/* from _extn.c */
	extern	int	oflag;
	extern	char	object[];

#ifdef OLIST
	if (oflag)
		sprintf(object,"0%.7lo  ", loc);
	else
		sprintf(object, "0x%.6lx  ", loc);
#else
	if (oflag)
		sprintf(object,"%6lo:  ",loc);
	else
		sprintf(object,"%4lx:  ",loc);
#endif
}
/*
 *	resync ()
 *
 *	If a bad op code is encountered, the disassembler will attempt
 *	to resynchronize itself. The next line number entry and the
 *	next function symbol table entry will be found. The restart
 *	point will be the smaller of these two addresses and bytes
 *	of object code will be dumped (not disassembled) until the
 *	restart point is reached.
 */


resync()

{
	/* the following 7 items are from _extn.c */
	extern	char	*fname;
	extern	char	*sname;
	extern	LDFILE	*f_ptr;
	extern	LDFILE	*t_ptr;
	extern	LDFILE	*l_ptr;
	extern	LDFILE	*symb;
	extern	FILHDR	filhdr;
	extern	SCNHDR	 scnhdr;
	extern	long	loc;
	extern	FUNCLIST	*next_function;
	extern	FUNCLIST	*currfunc;
	extern	char	mneu[];
	extern	int	nosyms;
#if OLIST
	extern	int	funcs_listed;
	extern	int	nosource;
#endif

#if AR16WR && !OLIST
	static	long	firstfn = -1L;
#endif

	struct	lineno	*eptr = &line_ent;
	struct	syment	sentry, stbentry, *septr;
	long	paddr, sindex, linaddr, symaddr, dumpaddr, s_end;

	linaddr = BADADDR;
	symaddr = BADADDR;
	s_end = scnhdr.s_paddr + scnhdr.s_size;
	/* Find the next line number entry if the file has not been
	 * stripped of line number information. Each entry from the
	 * beginning will be read until one having a greater address
	 * than the present location is found.
	 */
	if (nosyms == TRUE) {
		printf ("\n\t**   FILE IS STRIPPED  **\t\n");
		printf ("\t**   SKIP TWO BYTES AND TRY AGAIN  **\n");
		return(SUCCESS);
	}
	if (stripped() != TRUE)
	{
		for( ; lnno_cnt < scnhdr.s_nlnno; lnno_cnt++ )
		{
			if (eptr->l_lnno == 0) {
				if (symb == NULL)
					break;
				ldtbread(symb, eptr->l_addr.l_symndx, &sentry);
				linaddr = sentry.n_value;
				if (loc <= linaddr)
					goto symfind;
			} 
			else if (eptr->l_addr.l_paddr >= loc) {
				linaddr = eptr->l_addr.l_paddr;
				goto symfind;
			}
			FREAD(eptr, LINESZ, 1, l_ptr);
		}
		/* Cannot restart based on line number information.
		 * Try to use symbol table information to find restart
		 * point.
		 */
		if (symb == NULL)
			linaddr = BADADDR;
		else
			/* If no further line numbers have been found,
			 * the address of the section end will be a
			 * candidate for a restart point.
			 */
			linaddr = s_end;
	}
symfind:
	/* Find next function symbol table entry.	*/
	for( ; next_function; currfunc = next_function, next_function = next_function->nextfunc )
		if (next_function->faddr >= loc)
		{
			symaddr = next_function->faddr;
			break;
		}
#if OLIST
		else
			funcs_listed += 1;

	if (next_function == NULL)
		symaddr = curfile->tend;
#else
#if AR32W || AR32WR
	if (next_function == NULL)
		symaddr = scnhdr.s_paddr + scnhdr.s_size;
#else
	/* scan the symbol table	*/
	if ((filhdr.f_nsyms != 0L) && (symb != NULL) && (ldtbseek(symb) != FAILURE)) {
		/*
	 	 * set firstfn equal to the symbol table index
	 	 * of the first function symbol table entry.
		 * The search for the next function symbol
	 	 * table entry will always begin at firstfn.
			 */
		septr = &stbentry;
		if (firstfn == BADADDR) {
			ldtbread (symb, 0, septr);
			for (sindex = 1+septr->n_numaux; (sindex<filhdr.f_nsyms)
			&& ((septr->n_scnum<1) || (!ISFCN(septr->n_type)
			|| (septr->n_value < scnhdr.s_paddr)
			|| (septr->n_value > s_end))); sindex++) {
				ldtbread (symb, sindex, septr);
				sindex += septr->n_numaux;
			}
			firstfn = sindex -1 - septr->n_numaux;
		}
		ldtbread(symb, firstfn, septr);
		sindex = firstfn + 1 + septr->n_numaux;
		for (;(sindex < filhdr.f_nsyms) && ((septr->n_scnum < 1)
		|| (septr->n_numaux == 0) || ( !ISFCN(septr->n_type))
		|| (septr->n_value < scnhdr.s_paddr)
		|| (septr->n_value > s_end)
		|| (septr->n_value <= loc)); sindex++) {
			ldtbread(symb, sindex, septr);
			sindex += septr->n_numaux;
		}
		/*
	 	* If the next fuction symbol table entry has been
	 	* found, it is a candidate for a restart point.
	 	* If the entire symbol table has been read, and a
	 	* possible restart point has not been found, the end  
	 	* of the section is a candidate for a restart point.
	 	*/
		if ((sindex - septr->n_numaux) == filhdr.f_nsyms)
			symaddr = s_end;
		else
			symaddr = septr->n_value;
	}
	/* end of the section is end of the function */
#endif
#endif

	if (symaddr == BADADDR) {
		/* Restart point was not found by searching the
		 * symbol table.
		 */
		if (linaddr == BADADDR) {
			/* Restart point was not found by looking
			 * at line number information.
			 */
#ifdef OLIST
			if (nosource == FALSE)
#endif
			{
				printf ("\n\t**\tCANNOT FIND NEXT LINE NUMBER\t\t\t**\n");
				printf ("\t**  CANNOT FIND NEXT FUNCTION SYMBOL TABLE ENTRY\t**\n");
				printf ("\t**  FOLLOWING DISASSEMBLY MAY BE OUT OF SYNC\t\t**\n");
			}
			return (FAILURE);
		} 
		else {
			/* The next line number entry will be the
			 * restart point.
			 */
			paddr = linaddr;
#ifdef OLIST
			if (nosource == FALSE)
#endif
			{
				printf ("\n\t** OBJECT CODE WILL BE DUMPED UNTIL\t**\n");
				printf ("\t** THE NEXT BREAKPOINTABLE LINE NUMBER\t**\n");
			}
		}
	}
	else if (linaddr == BADADDR) {
		/* The next function symbol table entry or the end
		 * of the section will be the restart point.
		 */
		paddr = symaddr;
#ifdef OLIST
		if (nosource == FALSE)
#endif
		{
			printf ("\n\t**   OBJECT CODE WILL BE DUMPED UNTIL\t**\n");
			printf ("\t**  THE BEGINNING OF THE NEXT FUNCTION\t**\n");
			printf ("\t**    OR UNTIL THE END OF THE SECTION\t**\n");
		}
	}
	else {
		/* The smaller address of the next line number
		 * entry and the next function symbol table entry
		 * will be the restart point.
		 */
		paddr = (linaddr < symaddr)? linaddr: symaddr;
#ifdef OLIST
		if (nosource == FALSE)
#endif
		{
			printf ("\n\t**      OBJECT CODE WILL BE DUMPED UNTIL\t**\n");
			printf ("\t**           RESTART POINT IS REACHED\t\t**\n");
		}
	}
	dumpaddr = (loc < paddr - scnhdr.s_paddr)? (paddr -scnhdr.s_paddr):
						paddr;
	for (; loc < dumpaddr; ) {
		/* Dump bytes until the restart point is reached. */
#ifdef OLIST
		if (nosource == FALSE)
#endif
			printf("\t");
		prt_offset();
#ifdef B16
		getbyte( t_ptr );
		if (loc < dumpaddr)
			getbyte( t_ptr );
#endif
#ifdef MC68
		get2bytes(t_ptr);
#endif
#ifdef N3B
		get2bytes(t_ptr);
#endif
#ifdef M32
		get1byte(t_ptr);
#endif /* M32 */
		sprintf(mneu,"");
		printline();
	}	/* end of dump of unused object code	*/

#ifdef OLIST
	if ( nosource == FALSE )
#endif
		printf("\n\t** DISASSEMBLER RESYNCHRONIZED **\n");
	return(SUCCESS);

}
/*
 *	sect_name ()
 *
 *	If labels are to be looked up, the 'looklabel' routine needs to know
 *	the number of the section being processed.  Compare the section name
 *	to the stored names, created in 'build_labels', and return the
 *	appropriate section number.
 */

short
sect_name()

{
	/* the following 2 items are from _extn.c */
	extern	char	*sname;
	extern	FILHDR	filhdr;

	unsigned short	i;

	for (i = 1; i < filhdr.f_nscns +1; i++) 
		if (strcmp(sname,section[i]) == STRNGEQ)
			return(i);

	return (FAILURE);
}

#if N3B | U3B | M32 | U3B5
/*
 *	sfpconv(fprep, fpdoub)
 *
 *	This routine will convert the long "fprep" single precision 
 *	floating point representation of a number into its decimal 
 *	equivalent. The result will be stored in *fpdoub. The routine
 *	will return a value indicating what type of floating point
 *	number was converted.
 */
sfpconv(fprep, fpdoub)
long fprep;
double *fpdoub;
{
	unsigned short exponent;
	short	leadbit, signbit;
	long	mantissa;
	double	dtemp, dec2exp;

	exponent = (unsigned short)((fprep>>23) & 0xffL);
	/* exponent is bits 1-8 of the long	*/

	mantissa = fprep & 0x7fffffL;
	/* mantissa is bits 9-31 of the long	*/

	signbit = (fprep>>31) & 0x1L;
	/* sign bit (1-negative, 0-positive) is bit 0 of long	*/

	leadbit = 1;
	/* implied bit to the left of the decimal point	*/

	if (exponent==255)
		if (mantissa)
			return(NOTANUM);
		else
			return((signbit)? NEGINF: INFINITY);

	if (exponent==0)
		if (mantissa)
		/* This is a denormalized number. The implied bit to 
		 * the left of the decimal point is 0.
		 */
			leadbit = 0;
		else
			return((signbit)? NEGZERO: ZERO);

	/*
	 * Convert the 23 bit mantissa into "I.fraction" where
	 * "I" is the implied bit. The 23 bits are divided by
	 * 2 to the 23rd power to transform the mantissa into a
	 * fraction. Then the implied bit is added on.
	 */
	dtemp = (double)(leadbit + (double)mantissa/TWO_23);

	/* Calculate 2 raised to the (exponent-BIAS) power and
	 * store it in a double.
	 */
	if (exponent < BIAS)
		for(dec2exp=1; exponent < BIAS; ++exponent)
			dec2exp /= 2;
	else
		for(dec2exp=1; exponent > BIAS; --exponent)
			dec2exp *= 2;

	/* Multiply "I.fraction" by 2 raised to the (exponent-BIAS)
	 * power to obtain the decimal floating point number.
	 */
	*fpdoub = dtemp *dec2exp;

	if (signbit)
		*fpdoub = -(*fpdoub);
	return(FPNUM);
}
#endif	/* N3B | U3B | M32 | U3B5 */

/*
 *	stripped ()
 *
 *	Is the file stripped (no line number entries)?
 *	If so, return TRUE else FALSE.
 */

stripped()

{
	extern	SCNHDR	 scnhdr;	/* from _extn.c */

	return( (scnhdr.s_nlnno == 0) ? TRUE : FALSE);
}

/*
 *	fatal()
 *
 *	print an error message and quit
 */

fatal( message )
char	*message;
{
	extern	char	*fname;

	fprintf( stderr, "\n%s%s: %s: %s\n", SGS, TOOL, fname, message );
	exit(1);
}
