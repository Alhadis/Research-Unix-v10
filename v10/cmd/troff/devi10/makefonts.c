
/*
 *
 *  This program was written with tabs set to every fourth column,
 *  so things will look funny unless you have your tabs set the same way.
 *  I haven't really finished the program, but it works and I doubt I'll
 *  do anything else with it.
 *
 */


#include <stdio.h>
#include <ctype.h>

/*
#include "glyph.h"
*/
#include "dev.h"


#define	NON_FATAL	0
#define	FATAL		1

#define	FALSE		0
#define	TRUE		1

#define	NOT_SET		-1					/* variable has not been initialized */

#define	RES			0					/* used in new_value() switch stmt */
#define	UNIT		1
#define	HOR			2
#define	VERT		3
#define	WIDTH		4
#define	LENGTH		5

#define	BMASK		0377				/* mask for characters */

#define	CNAME		2					/* max length of special characters */
#define	DNAME		10					/* max length of device name */
#define	FNAME		10					/* max length of font name */
#define	PNAME		50					/* longest allowed pathname */

#define	MAX_BUFF	30					/* input strings are put here */
#define	LINE_LENGTH	100					/* longest allowed line */
#define	MAX_CHARS	255					/* most allowed chars per font */

#define	SIZE_LENGTH	150					/* space allowed for new point sizes */
#define	FONT_LENGTH	150					/* space for new default font list */



int		use_rast = FALSE;				/* get widths from raster tables? */

char	path[PNAME+1];					/* path names built up here */

char	*srcdir = "/usr/lib/font";		/* ASCII font files are kept here */
char	*fontdir = "/usr/lib/font";		/* binary font and device files */
char	*bitdir = "/usr/lib/font";		/* raster tables are located here */
char	*changes = "FONTCHANGES";		/* interpret this file */

char	mdev[DNAME+1] = "";				/* use this device's source files */
char	tdev[DNAME+1] = "";				/* target device - for raster tables */
char	cur_font[FNAME+1] = "";			/* font we are currently working on */

int		unitwidth = NOT_SET;			/* widths hold for this point size */
int		res = NOT_SET;					/* and this target device resolution */
int		hor = NOT_SET;					/* minimum horizontal step */
int		vert = NOT_SET;					/* minimum vertical step */
char	fonts[FONT_LENGTH] = {0};		/* new default font list */
char	sizes[SIZE_LENGTH] = {0};		/* and point size list */

float	incr;							/* add this to width after expansion */
float	scale = NOT_SET;				/* scale factor for width changes */
float	dflt_chng = NOT_SET;			/* default width change */
float	font_chng = NOT_SET;			/* width change for current font */
float	dflt_incr = .5;					/* add to width after expansion */

int		dcount;							/* index into following arrays */
char	names[MAX_CHARS][CNAME+1];		/* special character strings go here */
float	deltas[MAX_CHARS];				/* corresponding width changes */
float	incrs[MAX_CHARS];				/* character increments */

struct dev	dev;						/* *mdev's DESC.out header goes here */

int		nchtab;							/* number of special chars in chtab */
short	*pstab;							/* point size table - not needed */
short	*chtab;							/* char's index in chname array */
char	*chname;						/* special character strings */


FILE	*fp_change;						/* file pointer for *changes */
FILE	*fp_rast;						/* raster table used for widths */
FILE	*fp_in;							/* input file for current font */
FILE	*fp_out;						/* output file for current font */

char	**argv;							/* let everyone use them */
int		argc;




/*****************************************************************************/


main(agc, agv)


	int		agc;
	char	*agv[];


{


	/********************************************************************
	 *																	*
	 *		This program reads the file *changes in the current			*
	 *	directory which contains instructions on how to expand or		*
	 *	contract the character widths for each of the fonts mentioned	*
	 *	in the file. These changes will hopefully make the output for	*
	 *	*tdev more readable. Admittedly there's quite a bit of overkill	*
	 *	in this program, but I'm not going to change it.				*
	 *																	*
	 *		There are several different methods that can be used to		*
	 *	make the desired width changes. Probably the simplest is to		*
	 *	adjust the device resolution in the new DESC file up or down	*
	 *	an appropriate ammount and leave the widths in the font tables	*
	 *	unchanged. This approach results in a uniform width change for	*
	 *	all the characters on every available font. This type of change	*
	 *	can be obtained by specifying a default width change without	*
	 *	defining a new device resolution.								*
	 *																	*
	 *		Since a uniform expansion may not always be appropriate		*
	 *	you can also specify a width change for each for the new fonts.	*
	 *	This type of expansion will replace any default font change		*
	 *	that may have been specified.									*
	 *																	*
	 ********************************************************************/



	argv = agv;							/* global so everyone can use them */
	argc = agc;

	get_options();						/* process any command line options */
	startchanges();						/* start reading the *changes file */
	checkvalues();						/* make sure everything's defined */
	getdesc();							/* read *mdev's DESC.out file */
	makedesc();							/* make *tdev's new DESC file */
	finishchanges();					/* read rest of the *changes file */

}	/* End of main */


/******************************************************************************/


get_options()


{


	/********************************************************************
	 *																	*
	 *		Called from the main program to read and process any		*
	 *	command line options.											*
	 *																	*
	 ********************************************************************/


	while ( argc > 1  &&  argv[1][0] == '-' )  {

		switch ( argv[1][1] )  {

			case 'S':					/* font source directory */
						srcdir = &argv[1][2];
						break;

			case 'F':					/* binary font directory */
						fontdir = &argv[1][2];
						break;

			case 'R':					/* raster table directory */
						bitdir = &argv[1][2];
						break;

			case 'm':					/* get master device name */
						if ( strlen(&argv[1][2]) > DNAME )
							error(FATAL, "device name %s too long", &argv[1][2]);
						strcpy(mdev, &argv[1][2]);
						break;

			case 't':					/* set target device name */
						if ( strlen(&argv[1][2]) > DNAME )
							error(FATAL, "device name %s too long", &argv[1][2]);
						strcpy(tdev, &argv[1][2]);
						break;

			default:
						error(FATAL, "illegal option %c", argv[1][1]);

		}	/* End switch */

		argv++;							/* next argument */
		argc--;

	}	/* End while */

}	/* End of get_options */


/*****************************************************************************/


startchanges()


{


	char	buff[MAX_BUFF];				/* input buffer */
	int		ch;							/* used to skip comments */


	/********************************************************************
	 *																	*
	 *		Called from the main program to read the font change file	*
	 *	*changes. If there's still one command line argument left when	*
	 *	we get here, we will use it as the name of the input file,		*
	 *	otherwise the default file *changes is used.					*
	 *																	*
	 ********************************************************************/


	if ( argc > 1 )
		changes = &argv[1][0];

	if ( (fp_change = fopen(changes, "r")) == NULL )
		error(FATAL, "can't open file %s", changes);

	while ( fscanf(fp_change, "%s", buff) != EOF )  {

		if ( strcmp(buff, "font") == 0 )
			return;
		else if ( strcmp(buff, "master") == 0 )
			get_mdev();
		else if ( strcmp(buff, "target") == 0 )
			get_tdev();
		else if ( strcmp(buff, "unitwidth") == 0 )
			get_int(fp_change, &unitwidth);
		else if ( strcmp(buff, "res") == 0 )
			get_int(fp_change, &res);
		else if ( strcmp(buff, "hor") == 0 )
			get_int(fp_change, &hor);
		else if ( strcmp(buff, "vert") == 0 )
			get_int(fp_change, &vert);
		else if ( strcmp(buff, "sizes") == 0 )
			get_sizes();
		else if ( strcmp(buff, "fonts") == 0 )
			get_fonts();
		else if ( strcmp(buff, "default") == 0 )  {
			fscanf(fp_change, "%s", buff);
			if ( strcmp(buff, "change") != 0 )
				error(FATAL, "syntax error - bad default change statement");
			get_float(fp_change, &dflt_chng);
		} else if ( strcmp(buff, "round") == 0 )  {
			fscanf(fp_change, "%s", buff);
			if ( strcmp(buff, "up") == 0 )
				dflt_incr = 1.0;
			else if ( strcmp(buff, "down") == 0 )
				dflt_incr = 0.0;
			else error(FATAL, "syntax error - bad round statement");
		} else if ( strcmp(buff, "use") == 0 )  {
			fscanf(fp_change, "%s", buff);
			if ( strcmp(buff, "raster") != 0 )
				error(FATAL, "syntax error - bad use statement");
			fscanf(fp_change, "%s", buff);
			if ( strcmp(buff, "tables") == 0 )
				error(FATAL, "syntax error - bad use statement");
			use_rast = TRUE;
		} else if ( buff[0] == '#' )
			while ( (ch = getc(fp_change)) != EOF  &&  ch != '\n' ) ;
		else error(FATAL, "don't know command %s", buff);

	}	/* End while */

	error(FATAL, "no font commands in file %s", changes);

}	/* End of startchanges */


/*****************************************************************************/


checkvalues()


{



	/********************************************************************
	 *																	*
	 *		Called from the main program to make sure all the required	*
	 *	parameters have been properly defined.							*
	 *																	*
	 ********************************************************************/


	if ( *mdev == '\0' )
		error(FATAL, "missing master device name");

	if ( use_rast == TRUE )
		if ( *tdev == '\0' )
			error(FATAL, "missing target device name");
		else if ( res == NOT_SET )
			error(FATAL, "missing target device resolution");

}	/* End of checkvalues */


/*****************************************************************************/


get_mdev()


{


	char	buff[MAX_BUFF];				/* put the device name here */


	/********************************************************************
	 *																	*
	 *		Read the name of the master device from *fp_change, and		*
	 *	save the name in array mdev[]. If *mdev has already been set,	*
	 *	possibly by an option, it won't be changed again.				*
	 *																	*
	 ********************************************************************/


	if ( fscanf(fp_change, " device %s", buff) != 1 )
		error(FATAL, "missing master device name");

	if ( strlen(buff) > DNAME )
		error(FATAL, "master device name %s too long", buff);

	if ( *mdev == '\0' )
		strcpy(mdev, buff);

}	/* End of get_mdev */


/*****************************************************************************/


get_tdev()


{


	char	buff[MAX_BUFF];				/* target device name put here */


	/********************************************************************
	 *																	*
	 *		Called from startchanges() to read the name of the target	*
	 *	typesetter from *fp_change, and store the result in the array	*
	 *	tdev[]. If the device has already been specified nothing is		*
	 *	done.															*
	 *																	*
	 ********************************************************************/


	if ( fscanf(fp_change, " device %s", buff) != 1 )
		error(FATAL, "missing target device name");

	if ( strlen(buff) > DNAME )
		error(FATAL, "target device name %s too long", buff);

	if ( *tdev != '\0' )
		strcpy(tdev, buff);

}	/* End of get_tdev */


/*****************************************************************************/


get_int(fp, num)


	FILE	*fp;						/* read an integer from this file */
	int		*num;						/* and store it here */


{


	/********************************************************************
	 *																	*
	 *		Called to read an integer from the input file and store the	*
	 *	result in num - this routine and get_float() really should be	*
	 *	macros.															*
	 *																	*
	 ********************************************************************/


	if ( fscanf(fp, "%d", num) != 1 )
		error(FATAL, "syntax error - integer not found");

}	/* End of get_int */


/*****************************************************************************/


get_float(fp, num)


	FILE	*fp;						/* read a float from this file */
	float	*num;						/* and store it here */


{


	/********************************************************************
	 *																	*
	 *		Called to read a floating point number from *fp_change and	*
	 *	store the result in num.										*
	 *																	*
	 ********************************************************************/


	if ( fscanf(fp, "%f", num) != 1 )
		error(FATAL, "syntax error - floating point number not found");

}	/* End of get_float */


/*****************************************************************************/


get_sizes()


{


	int		i;							/* index into sizes[] array */


	/********************************************************************
	 *																	*
	 *		Called from startchanges() to read the new default point	*
	 *	size list for the DESC file. The format expected for the size	*
	 *	list in the *fp_change file is the same as in the ASCII DESC	*
	 *	file.															*
	 *																	*
	 ********************************************************************/


	i = 0;

	while ( i < SIZE_LENGTH )  {

		while ( isspace(sizes[i++] = getc(fp_change)) ) ;

		if ( !isdigit(sizes[i-1]) )
			error(FATAL, "syntax error - bad point size list");

		if ( sizes[i-1] == '0'  &&  (isspace(sizes[i++] = getc(fp_change))) )  {
			sizes[i-1] = '\0';
			return;
		}	/* End if */

		while ( isdigit(sizes[i++] = getc(fp_change)) ) ;

	}	/* End while */

	error(FATAL, "point size list too long");	/* shouldn't get here */

}	/* End of get_sizes */


/*****************************************************************************/


get_fonts()


{


	int		count;						/* number of default fonts */
	int		i;							/* index used for fonts[] array */


	/********************************************************************
	 *																	*
	 *		Called from startchanges() to read a new default font list	*
	 *	for the new DESC file. Again the format expected for the font	*
	 *	list is the same as in the ASCII DESC file.						*
	 *																	*
	 ********************************************************************/


	count = -1;							/* real value is the first string */
	i = 0;								/* next character goes here */

	do {

		while ( isspace(fonts[i++] = getc(fp_changes)) ) ;

		while ( !isspace(fonts[i++] = getc(fp_changes)) ) ;

		if ( count < 0 )				/* string is the default font count */
			count = atoi(fonts);

	} while ( count-- > 0 );

	fonts[i-1] = '\0';

}	/* End of get_fonts */


/*****************************************************************************/


getdesc()


{


	int		fin;						/* DESC.out's file descriptor */
	char	*filebase;					/* memory block for DESC.out file */
	char	*malloc();					/* memory allocation routine */


	/********************************************************************
	 *																	*
	 *		Called from the main program to read the DESC.out file for	*
	 *	*mdev. Most of the file is only needed if we are going to do 	*
	 *	the width lookups in the raster tables, but anyway we'll read	*
	 *	the whole thing in.												*
	 *																	*
	 ********************************************************************/


	sprintf(path, "%s/dev%s/DESC.out", fontdir, mdev);
	if ( (fin = open(path, 0)) < 0 )
		error(FATAL, "can't open file %s", path);

	read(fin, &dev, sizeof(struct dev));	/* struct dev comes first */

	nchtab = dev.nchtab;

	if ( (filebase = malloc(dev.filesize)) == NULL )
		error(FATAL, "no memory available");

	read(fin, filebase, dev.filesize);		/* get rest of the DESC.out file */
	pstab = (short *) filebase;				/* this table is never used */
	chtab = pstab + dev.nsizes + 1;
	chname = (char *) (chtab + dev.nchtab);

	close(fin);								/* won't need this guy again */

}	/* End of getdesc */


/*****************************************************************************/


makedesc()


{


	char	buff[MAX_BUFF];				/* input strings are put here */
	int		ch;							/* used in skipping over comments */
	int		temp;						/* used to skip font and point sizes */


	/********************************************************************
	 *																	*
	 *		Called from the main program to make the new ASCII DESC		*
	 *	file for *tdev.													*
	 *																	*
	 ********************************************************************/


	sprintf(path, "%s/dev%s/DESC", srcdir, mdev);
	if ( (fp_in = fopen(path, "r")) == NULL )
		error(FATAL, "can't read file %s", path);

	if ( (fp_out = fopen("DESC", "w")) == NULL )
		error(FATAL, "can't write new DESC file");

	while ( fscanf(fp_in, "%s", buff) != EOF )  {

		fprintf(fp_out, "%s", buff);
		if ( strcmp(buff, "res") == 0 )
			new_value(RES);
		else if ( strcmp(buff, "hor") == 0 )
			new_value(HOR);
		else if ( strcmp(buff, "vert") == 0 )
			new_value(VERT);
		else if ( strcmp(buff, "unitwidth") == 0 )
			new_value(UNIT);
		else if ( strcmp(buff, "paperwidth") == 0 )
			new_value(WIDTH);
		else if ( strcmp(buff, "paperlength") == 0 )
			new_value(LENGTH);
		else if ( strcmp(buff, "fonts") == 0  &&  fonts[0] )  {
			fprintf(fp_out, "%s\n", fonts);
			get_int(fp_in, &temp);
			while ( temp-- > 0 )
				fscanf(fp_in, "%s", buff);
		} else if ( strcmp(buff, "sizes") == 0  &&  sizes[0] )  {
			fprintf(fp_out, "%s\n", sizes);
			do {
				get_int(fp_in, &temp);
			} while ( temp != 0 );
		} else if ( strcmp(buff, "charset") == 0 )
			while ( (ch = getc(fp_in)) != EOF ) putc(ch, fp_out);
		else copy_line();

	}	/* End while */

	fclose(fp_in);
	fclose(fp_out);

}	/* End of makedesc */


/*****************************************************************************/


new_value(value)


	int		value;						/* used in the switch statement */


{


	int		temp;						/* next number read from *fp_in */


	/********************************************************************
	 *																	*
	 *		Called from makedesc() to write a new value out to the DESC	*
	 *	file for the parameter specified by value.						*
	 *																	*
	 ********************************************************************/


	get_int(fp_in, &temp);

	switch ( value )  {

		case RES:
					if ( res == NOT_SET )
						if ( dflt_chng == NOT_SET )  {
							res = temp;
							dflt_chng = 0.0;
						} else res = temp / (1 + dflt_chng);
					temp = res;
					break;

		case UNIT:
					if ( unitwidth == NOT_SET )
						unitwidth = temp;
					temp = unitwidth;
					break;

		case HOR:
					if ( hor == NOT_SET )
						hor = temp;
					temp = hor;
					break;

		case VERT:
					if ( vert == NOT_SET )
						vert = temp;
					temp = vert;
					break;

		case WIDTH:
		case LENGTH:
					temp = (temp * res) / dev.res;
					break;

	}	/* End switch */

	fprintf(fp_out, " %d\n", temp);

}	/* End of new_value */


/*****************************************************************************/


finishchanges()


{


	char	buff[MAX_BUFF];
	int		ch;
	int		i;


	/********************************************************************
	 *																	*
	 *		Called from startchanges() to process all the new font		*
	 *	commands. 														*
	 *																	*
	 ********************************************************************/


	nextfont();							/* do initialization for next font */

	while ( fscanf(fp_change, "%s", buff ) != EOF )  {

		if ( strcmp(buff, "font") == 0 )  {
			makefont();
			nextfont();
		} else if ( strcmp(buff, "increment") == 0 )  {
			if ( fscanf(fp_change, "%s by ", buff) != 1 )
				error(FATAL, "syntax error - bad file format");
			if ( strlen(buff) > CNAME )
				error(FATAL, "special char %s too long", buff);
			i = lookup(buff);
			get_float(fp_change, &incrs[i]);
			if ( i == dcount )  {
				deltas[dcount] = NOT_SET;
				strcpy(&names[dcount++][0], buff);
			}	/* End if */
		} else if ( strcmp(buff, "change") == 0 )  {
			if ( fscanf(fp_change, "%s by ", buff) != 1 )
				error(FATAL, "syntax error - bad file format");
			if ( strcmp(buff, "widths") == 0 )
				get_float(fp_change, &font_chng);
			else {
				if ( strlen(buff) > CNAME )
					error(FATAL, "special char %s too long", buff);
				i = lookup(buff);
				get_float(fp_change, &deltas[i]);
				if ( i == dcount )  {
					incrs[dcount] = 0;
					strcpy(&names[dcount++][0], buff);
				}	/* End if */
			}	/* End else */
		} else if ( buff[0] == '#' )
			while ( (ch = getc(fp_change)) != '\n' && ch != EOF ) ;

	}	/* End while */

	makefont();

}	/* End of finishchanges */


/*****************************************************************************/


nextfont()


{


	/********************************************************************
	 *																	*
	 *		Called from finishchanges() to get the name of the next		*
	 *	font and do any other initialization that may be required.		*
	 *																	*
	 ********************************************************************/


	fscanf(fp_change, "%s", cur_font);		/* name of the next font */

	font_chng = NOT_SET;
	incr = dflt_incr;
	dcount = 0;

	if ( scale == NOT_SET )
		if ( use_rast == TRUE )
			scale = 1.0;
		else scale = ( float )(res * unitwidth) / (dev.res * dev.unitwidth);

}	/* End of nextfont */


/*****************************************************************************/


makefont()


{


	char	buff[MAX_BUFF];				/* temp buffer for *fp_change reads */


	/********************************************************************
	 *																	*
	 *		This routine makes all the new ASCII font files.			*
	 *																	*
	 ********************************************************************/


	sprintf(path, "%s/dev%s/%s", srcdir, mdev, cur_font);
	if ( (fp_in = fopen(path, "r")) == NULL )
		error(FATAL, "can't open font file %s", path);

	if ( (fp_out = fopen(cur_font, "w")) == NULL )
		error(FATAL, "can't open output font file %s", cur_font);

	while ( fscanf(fp_in, "%s", buff) != EOF )  {

		fprintf(fp_out, "%s", buff);	/* echo what we just read in */
		if ( strcmp(buff, "charset") == 0 )
			copy_chars();
		else copy_line();

	}	/* End while */

	fclose(fp_in);						/* done with these files for now */
	fclose(fp_out);

}	/* End of makefont */


/*****************************************************************************/


copy_chars()


{


	char	ch[10];						/* standard stuff from makedev */
	char	width[10];
	char	kern[10];
	char	code[10];
	char	buff[LINE_LENGTH];			/* next line from charset part */


	/********************************************************************
	 *																	*
	 *		Called from makefont() to copy the charset portion of the	*
	 *	current font file to the output file. The width entry for each	*
	 *	character will be adjusted by routine newwidth().				*
	 *																	*
	 ********************************************************************/


	putc('\n', fp_out);

	while ( fgets(buff, sizeof(buff), fp_in) != NULL )  {

		if ( sscanf(buff, "%s %s %s %s", ch, width, kern, code) < 2 )
			continue;

		if ( width[0] == '"' )  {		/* it's a synonym */
			fprintf(fp_out, "%s", buff);
			continue;
		}	/* End if */

		newwidth(ch, width);			/* calculate *ch's new width */

		fprintf(fp_out, "%s\t%s\t%s\t%s\n", ch, width, kern, code);

	}	/* End while */

}	/* End of copy_chars */


/*****************************************************************************/


copy_line()


{


	int		ch;


	/********************************************************************
	 *																	*
	 *		Called from makefont() to copy the current line from fp_in	*
	 *	to fp_out.														*
	 *																	*
	 ********************************************************************/


	while ( (ch = putc(getc(fp_in), fp_out)) != '\n' && ch != EOF ) ;

}	/* End of copy_line */


/*****************************************************************************/


newwidth(ch, width)


	char	*ch;						/* get the width for this character */
	char	*width;						/* and store it here */


{


	int		newwidth;					/* character's new width */
	float	cwidth;						/* char's current width */
	float	delta;						/* width change for the character */

	float	getdelta();
	float	atof();


	/********************************************************************
	 *																	*
	 *		Called from copy_chars() to get the new width for character	*
	 *	ch, and store the ASCII representation for it in array width[].	*
	 *	If we are supposed to be getting the character widths from the	*
	 *	raster tables we call getwidth() to do the work, otherwise we	*
	 *	use the value obtained from the ASCII font file.				*
	 *																	*
	 ********************************************************************/


	if ( use_rast == TRUE )
		cwidth = getwidth(ch);
	else cwidth = atof(width);

	delta = getdelta(ch);

	if ( (newwidth = cwidth * (1 + delta) * scale + incr) < 0 || newwidth > BMASK )
		error(FATAL, "bad width %d for char %s", newwidth, ch);

	sprintf(width, "%d", newwidth);

}	/* End of newwidth */


/*****************************************************************************/


getwidth(ch)


	char	*ch;						/* get raster width for this char */


{


	/********************************************************************
	 *																	*
	 *		Called from newwidth() to get the raster table width for	*
	 *	character ch form the raster table for the current font and in	*
	 *	size unitwidth.													*
	 *																	*
	 ********************************************************************/


	return(1);

}	/* End of getwidth */


/*****************************************************************************/


float getdelta(ch)


	char	*ch;						/* get change for this char */


{


	int		i;
	float	change;


	/********************************************************************
	 *																	*
	 *		This routine gets the appropriate change in width that we	*
	 *	should use for character ch in the new font table.				*
	 *																	*
	 ********************************************************************/


	change = font_chng == NOT_SET ? dflt_chng : font_chng;
	incr = dflt_incr;

	if ( (i = lookup(ch)) < dcount )  {
		change = ( deltas[i] == NOT_SET ) ? change : deltas[i];
		incr += incrs[i];
	}	/* End if */

	return(change);

}	/* End of getdelta */


/*****************************************************************************/


lookup(ch)


	char	*ch;						/* look this character string up */


{


	int		i;


	/********************************************************************
	 *																	*
	 *		Called from finishchanges() to find the character string	*
	 *	*ch in array names[]. If it isn't found dcount is returnd.		*
	 *																	*
	 ********************************************************************/


	for ( i = 0; i < dcount; i++ )
		if ( strcmp(ch, &names[i][0]) == 0 )
			break;

	return(i);

}	/* End of lookup */


/*****************************************************************************/


error(kind, mesg, arg1, arg2)


	int		kind;						/* FATAL or NON_FATAL error */
	char	*mesg;						/* error message to be printed */


{


	/********************************************************************
	 *																	*
	 *		Write out the error message *mesg, and quit the program if	*
	 *	kind is FATAL.													*
	 *																	*
	 ********************************************************************/


	fprintf(stderr, "makefonts: ");
	fprintf(stderr, mesg, arg1, arg2);
	fprintf(stderr, "\n");

	if ( kind == FATAL )
		exit(1);

}	/* End of error */


/*****************************************************************************/


