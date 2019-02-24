char	xxxmakei10[] ="@(#)makei10.c	1.1";

#include	<stdio.h>
#include	"dev.h"


#define	NON_FATAL	0
#define	FATAL		1
#define	NO			0
#define	YES			1

#define	FNAME		4					/* longest allowed font name - at least 3 */
#define	DNAME		11					/* longest accepted device name */
#define	CLENGTH		2					/* max length of special char strings */
#define	FONTDIR		100					/* max length of *fontdir string */
#define	LINE_LENGTH	100					/* longest line in ASCII tables */
#define	MAXWIDTH	255					/* maximum number of width entries */
#define	MAXSYNS		75					/* no more synonyms than this */
#define	MAXBUILD	75					/* largest 'build' list */
#define	MAXFONTS	60					/* can map this many fonts */
#define	NFONT		10					/* search at most this many fonts */
#define	BMASK		0377				/* mask for lower eight bits */

#define	FONTSIZE	3 * MAXWIDTH + nchtab + (128 - 32) + sizeof(struct Font)
#define	PATHLENGTH	FONTDIR + 2 * DNAME + FNAME + 5
#define	TARGDIR		FONTDIR + DNAME + 5



char	mdev[DNAME+1] = "";				/* raster tbls assume this device */
char	tdev[DNAME+1] = "i10";			/* target typesetter - Imagen */
char	*ndev;							/* make ASCII tables for this device */

struct dev	dev;						/* *mdev DESC.out header goes here */
struct Font	*fontbase[NFONT];			/* each font's .out header stuff */

int		nchtab;							/* number of special chars in chtab */
short	*pstab;							/* point size table - not needed */
short	*chtab;							/* char's index in chname array */
char	*chname;						/* special character strings */
char	*fontstart;						/* can load fonts starting here */
char	*widthtab[NFONT];				/* font's width table - not needed */
char	*codetab[NFONT];				/* *mdev's codes for characters */
char	*fitab[NFONT];					/* position of characters on a font */

char	mapped[MAXFONTS][FNAME+1];		/* mapped these fonts - ASCII names */
char	fmap[NFONT+1][FNAME+1];			/* names of *mdev's fonts that we search */
char	new[MAXSYNS][CLENGTH+1];		/* synonym for corresponding char in old[][] */
char	old[MAXSYNS][CLENGTH+1];		/* old character name */
char	bldlist[MAXBUILD+1][CLENGTH+1];	/* these chars will be built up */

int		mapcount;						/* mapped this many of *ndev's fonts */
int		syncount = 0;					/* number of synonyms in *fontmap */
int		bldcount = 0;					/* number of build chars in *fontmap */
int		fcount;							/* number of original default fonts */
int		acount = 0;						/* add this many fonts to default */
int		iname;							/* internal name for 'related' fonts */
int		nfonts;							/* number of *mdev's fonts to check */
int		fsize;							/* all fonts given this much room */

char	*fontdir = "..";				/* troff's font directory */
char	*srcdir = "..";					/* ASCII font files kept here */
char	targdir[TARGDIR] = ".";			/* target device's font directory */
char	*rastdev = "RASTERDEV";			/* device name is in this file */
char	*fontmap = "FONTMAP";			/* font map file for device *ndev */

char	addfont[(NFONT-1)*MAXFONTS*(FNAME+1)] = "";	/* additional default fonts */

char	path[PATHLENGTH];				/* build file pathnames up here */

FILE	*descin;						/* *ndev's real ASCII DESC file */
FILE	*descout;						/* new ASCII DESC file for *ndev */
FILE	*fontin;						/* read font data from this ASCII file */
FILE	*fontout[NFONT+1];				/* ASCII output files for this font */
FILE	*fp_map;						/* file pointer for *fontmap file */
FILE	*fp_error = stderr;				/* error messages */

char	**argv;							/* so everyone can use them */
int		argc;

int		silent = NO;



/*****************************************************************************/


main(agc, agv)


	int		agc;
	char	*agv[];


{


	/********************************************************************
	 *																	*
	 *		This program takes troff's ASCII font and device tables for	*
	 *	one or more typesetters and converts them into a form that can	*
	 *	be used with the raster tables located in the target device's	*
	 *	font directory. These new tables are written into appropriate	*
	 *	'dev' directories located in the *targdir directory.			*
	 *																	*
	 *		The program expects to find a file called RASTERDEV in		*
	 *	*targdir. The first string in this file is the name of the		*
	 *	device whose tables were used to make the raster files - the	*
	 *	rest of the file is skipped. In addition each new device must	*
	 *	have an appropriate 'dev' directory in *targdir, and each of	*
	 *	these directories must contain a file called FONTMAP, that		*
	 *	tells the program how to map *ndev's fonts.						*
	 *																	*
	 *		After the command line options are processed, the name of	*
	 *	the master device is read from the file *rastdev, provided the	*
	 *	string *mdev hasn't already been set. Then this device name is	*
	 *	used, in combination with *fontdir, to locate *mdev's binary	*
	 *	'.out' tables. Routine getdesc() reads the entire DESC.out		*
	 *	into memory, and then alloctes enough additional memory to hold	*
	 *	NFONT binary font files. Once this initialization stuff is done	*
	 *	the program enters a loop that treats the rest of the command	*
	 *	line arguments as device names. Each of these devices will have	*
	 *	a new set of ASCII font tables written out to the appropriate	*
	 *	'dev' directory in *targdir.									*
	 *																	*
	 *		The first step for each new device is to essentially copy	*
	 *	the first part of its real ASCII DESC file from *fontdir to the	*
	 *	appropriate 'dev' directory in *targdir. Routine startdesc()	*
	 *	handles this job, but it only reads up to the default font		*
	 *	count in the real DESC file. Once it has the default font count	*
	 *	startdesc() has really finished its job. It doesn't even write	*
	 *	this number out to the new DESC file, because we are probably	*
	 *	going to be adding some default fonts to the new DESC file.		*
	 *																	*
	 ********************************************************************/


	argv = agv;							/* global so everyone can use them */
	argc = agc;

	getoptions();
	getmdev();							/* get 'master' device name */
	getdesc();							/* get *mdev's DESC.out file */

	if ( silent == NO )
		printf("master device = %s\n\n", mdev);

	while ( --argc > 0 )  {				/* got another device */
		ndev = *++argv;					/* make tables for this guy */
		if ( silent == NO )
			printf("making tables for device %s\n", ndev);
		startdesc();					/* up to default font list */
		makefonts();					/* for all fonts listed in FONTMAP */
		finishdesc();					/* complete *ndev's new DESC file */
	}	/* End while */

}	/* End of main */


/*****************************************************************************/


getoptions()


{


	/********************************************************************
	 *																	*
	 *		Called from the main program to handle the command line		*
	 *	options.														*
	 *																	*
	 ********************************************************************/


	while ( argc > 1  &&  argv[1][0] == '-' )  {

		switch ( argv[1][1] )  {

			case 's':
						silent = YES;
						break;

			case 'm':
						strcpy(mdev, &argv[1][2]);
						if ( strlen(mdev) > DNAME )
							error(FATAL, "device name %s too long", mdev);
						break;

			case 'F':
						fontdir = &argv[1][2];
						break;

			case 'S':
						srcdir = &argv[1][2];
						break;

			default:
						error(NON_FATAL, "illegal option %c", argv[1][1]);
						break;

		}	/* End switch */

		argv++;
		argc--;

	}	/* End while */

}	/* End of getoptions */


/*****************************************************************************/


getmdev()


{


	FILE	*fd;						/* used for *rastdev file */


	/********************************************************************
	 *																	*
	 *		Called from the main program to get the name of the device	*
	 *	that was used to make the raster tables for the target device.	*
	 *	If the string *mdev has already been set, possibly by an		*
	 *	option, nothing is done. Otherwise we expect to find the name	*
	 *	as the first string in the file *targdir/*rastdev.				*
	 *																	*
	 ********************************************************************/


	if ( *mdev != '\0' )  return;		/* got the name somewhere else */

	sprintf(path, "%s/%s", targdir, rastdev);
	if ( (fd = fopen(path, "r")) == NULL )
		error(FATAL, "can't open file %s", path);

	if ( fscanf(fd, "%14s", mdev) != 1 )
		error(FATAL, "no device name in file %s", rastdev);

	if ( strlen(mdev) > DNAME )
		error(FATAL, "master device name %s too long", mdev);

	fclose(fd);							/* won't need this file again */

}	/* End of getmdev */


/*****************************************************************************/


getdesc()


{


	int		fin;						/* DESC.out's file descriptor */
	char	*filebase;					/* memory block for DESC.out file */
	char	*malloc();					/* memory allocation routine */


	/********************************************************************
	 *																	*
	 *		Called from the main program to read in the DESC.out file	*
	 *	for typesetter *mdev. Even though we only will use a small part	*
	 *	of this file, the whole thing is read into memory. I also		*
	 *	decided not to use the font space that is already allocated in	*
	 *	the DESC.out file. If you want to use this space just change	*
	 *	the initialization of fontstart.								*
	 *																	*
	 ********************************************************************/


	sprintf(path, "%s/dev%s/DESC.out", fontdir, mdev);
	if ( (fin = open(path, 0)) < 0 )
		error(FATAL, "can't open file %s", path);

	read(fin, &dev, sizeof(struct dev));	/* struct dev comes first */

	nchtab = dev.nchtab;					/* FONTSIZE needs this number */
	fsize = FONTSIZE;						/* space given to each font */

	if ( (filebase = malloc(dev.filesize)) == NULL )
		error(FATAL, "no memory available");

	read(fin, filebase, dev.filesize);		/* get rest of the DESC.out file */
	pstab = (short *) filebase;				/* this table is never used */
	chtab = pstab + dev.nsizes + 1;
	chname = (char *) (chtab + dev.nchtab);

	if ( (fontstart = malloc(NFONT * fsize)) == NULL )
		error(FATAL, "no memory available");

	close(fin);								/* won't need this guy again */

}	/* End of getdesc */


/*****************************************************************************/


startdesc()


{


	char	cmd[LINE_LENGTH];			/* DESC file commands go here */
	int		c;							/* used as a character */


	/********************************************************************
	 *																	*
	 *		Called from main() to begin making *ndev's ASCII DESC file.	*
	 *	The real DESC file is read up to the default font list. Later	*
	 *	routine finishdesc() is called to complete the new DESC file.	*
	 *	I did things this way because we need to know the original		*
	 *	number of default fonts for device *ndev before we make the		*
	 *	new font tables, and we can't write the default font list out	*
	 *	to the new DESC file until we have built all the new font		*
	 *	tables for *ndev.												*
	 *																	*
	 *		Since I have claimed dev.spare2 in routine finishdesc() we	*
	 *	should probably check if the command we just read was 'spare2'.	*
	 *	I'll take care of this later on.								*
	 *																	*
	 ********************************************************************/


	sprintf(path, "%s/dev%s/DESC", srcdir, ndev);
	if ( (descin = fopen(path, "r")) == NULL )
		error(FATAL, "can't open file %s", path);

	sprintf(path, "%s/dev%s/DESC", targdir, ndev);
	if ( (descout = fopen(path, "w")) == NULL )
		error(FATAL, "can't write file %s", path);

	while ( fscanf(descin, "%s", cmd) != EOF )  {
		fprintf(descout, "%s", cmd);
		if ( strcmp(cmd, "fonts") == 0 )  {
			fscanf(descin, "%s", cmd);
			fcount = atoi(cmd);
			return;
		} else if ( strcmp(cmd, "charset") == 0 )
			error(FATAL, "bad DESC file format");
		else while ( putc(c=getc(descin), descout) != '\n'  &&  c != EOF ) ;
	}	/* End while */

	error(FATAL, "bad DESC file");		/* shouldn't get here! */

}	/* End of startdesc */


/*****************************************************************************/


finishdesc()


{


	char	fname[FNAME+1];				/* names of the default fonts */
	int		i, j;						/* for loop indices */
	int		c;							/* used as a char with getc() */


	/********************************************************************
	 *																	*
	 *		Called from the main program after all the fonts for *ndev	*
	 *	have been mapped. Routine startdesc() read *descin up to the	*
	 *	first default font name, and it wrote *descout up to the count	*
	 *	part of the default font list. Most likely we will now have		*
	 *	more default fonts than we started with, so dev.spare2 can be	*
	 *	used by post-processors to get at the original number.			*
	 *																	*
	 ********************************************************************/


	fprintf(descout, " %d", fcount + acount);	/* new default font count */

	for ( i = 0; i < fcount; i++ )  {			/* check each default font */
		fscanf(descin, "%s", fname);
		fprintf(descout, " %s", fname);
		for ( j = 0; j < mapcount; j++ )		/* did we map this font? */
			if ( strcmp(fname, &mapped[j][0]) == 0 )
				break;
		if ( j >= mapcount )					/* didn't find it */
			error(NON_FATAL, "missing default font file %s", fname);
	}	/* End for */

	fprintf(descout, "%s", addfont);			/* finish the default font list */
	fprintf(descout, "\nspare2 %d", fcount);	/* post-processor can use it */

	while ( (c = getc(descin)) != EOF ) putc(c, descout);

	fclose(descin);								/* may have more devices */
	fclose(descout);

}	/* End of finishdesc */


/*****************************************************************************/


makefonts()


{


	char	cmd[LINE_LENGTH];			/* font file commands go here */
	int		c;							/* used as a character */
	int		i;							/* for loop index */


	/********************************************************************
	 *																	*
	 *		Called from main() to make all the ASCII font files listed	*
	 *	in *ndev's FONTMAP file. All the stuff up to the charset part	*
	 *	of each ASCII font file is essentially copied into the new		*
	 *	font file. The name field is changed to the name of the second	*
	 *	font from appropriate lines in the FONTMAP file. All 'related'	*
	 *	fonts are given the same internal number. The lower eight bits	*
	 *	of this number contains the font position where post-processors	*
	 *	can expect to find related fonts to search if a character is	*
	 *	not found on the primary font. This additional search should be	*
	 *	done before checking the special fonts. The ligature stuff is	*
	 *	not really needed - leave it in for now.						*
	 *																	*
	 ********************************************************************/


	mapcount = 0;						/* fonts mapped so far for *ndev */

	sprintf(path, "%s/dev%s/%s", targdir, ndev, fontmap);
	if ( (fp_map = fopen(path, "r")) == NULL )
		error(FATAL, "can't open file %s", path);

	while ( readmap() != 0 )  {			/* not done with FONTMAP file yet */

		if ( silent == NO )
			printf("font %s\n", &mapped[mapcount][0]);

		for ( i = 1; i < NFONT + 1; i++ )
			fontout[i] = NULL;

		sprintf(path, "%s/dev%s/%s", srcdir, ndev, &mapped[mapcount][0]);
		if ( (fontin = fopen(path, "r")) == NULL )
			error(FATAL, "can't open file %s", path);

		sprintf(path, "%s/dev%s/%s", targdir, ndev, &mapped[mapcount][0]);
		if ( (fontout[0] = fopen(path, "w")) == NULL )
			error(FATAL, "can't open file %s", path);

		for ( i = 0; i < nfonts; i++ )		/* get the '.out' font files */
			loadfont(&fmap[i][0], i);

		iname = (mapcount * (BMASK + 1)) + fcount + acount + 1;

		while ( fscanf(fontin, "%s", cmd) == 1)  {
			fprintf(fontout[0], "%s", cmd);
			if ( strcmp(cmd, "name") == 0 )  {
				 fprintf(fontout[0], " %s\n", &fmap[0][0]);
				 while ( (c = getc(fontin)) != '\n'  &&  c != EOF ) ;
			} else if ( strcmp(cmd, "internalname") == 0 )  {
				 fprintf(fontout[0], " %d\n", iname);
				 while ( (c = getc(fontin)) != '\n'  &&  c != EOF ) ;
			} else if ( strcmp(cmd, "ligatures") == 0 )  {
				 get_ligs();
				 while ( (c = getc(fontin)) != '\n'  &&  c != EOF ) ;
			} else if ( strcmp(cmd, "charset") == 0 )
				 copy_chars();
			else while ( putc(c=getc(fontin), fontout[0]) != '\n' && c != EOF ) ;
		}	/* End while */

		mapcount++;						/* finished mapping this font */

		fclose(fontin);
		for ( i = 0; i < NFONT + 1; i++ )
			if ( fontout[i] != NULL )
				fclose(fontout[i]);

	}	/* End while */

	fclose(fp_map);

}	/* End of makefonts */


/*****************************************************************************/


readmap()


{


	char	buff[20];
	int		c;							/* used as a character */


	/********************************************************************
	 *																	*
	 *		This routine continues reading the FONTMAP file until it	*
	 *	has processed the next font mapping instruction. Any 'build'	*
	 *	lists that are encountered replace the last list, and will		*
	 *	therefore apply to the next font map. The first font mentioned	*
	 *	in a font map line is the name of the primary font for the		*
	 *	current mapping instruction. This is *ndev's ASCII font file	*
	 *	that we will be reading and usually writing. The rest of the	*
	 *	fonts following the 'into' are the names of *mdev's fonts that	*
	 *	are searched when we do the character lookups. These searches	*
	 *	are done using the '.out' font files, which are read in from	*
	 *	*mdev's normal font directory.									*
	 *																	*
	 ********************************************************************/


	if ( mapcount >= MAXFONTS )
		error(FATAL, "too many fonts in %s file", fontmap);

	while ( fscanf(fp_map, "%19s", buff) == 1 )  {
		if ( strcmp(buff, "synonyms") == 0 )
			readsyns();
		else if ( strcmp(buff, "build") == 0 )
			readbuild();
		else if ( strcmp(buff, "nobuild") == 0 )
			bldcount = 0;
		else if ( strcmp(buff, "map") == 0 )
			return(readfonts());
		else if ( buff[0] = '#' )
			while ( (c = getc(fp_map)) != '\n'  &&  c != EOF ) ;
		else error(FATAL, "don't know command %s", buff);
	}	/* End while */

	return(0);

}	/* End of readmap */


/*****************************************************************************/


readfonts()


{


	int		c;							/* character used with getc() */


	/********************************************************************
	 *																	*
	 *		Called from readmap() to read the next font map line for	*
	 *	device *ndev from the file *fp_map.								*
	 *																	*
	 ********************************************************************/


	nfonts = 0;

	if ( fscanf(fp_map, "%s into", &mapped[mapcount][0]) != 1 )
		error(FATAL, "map file synatx error");

	if ( FNAME < 3 )					/* we may have trouble later on */
		error(FATAL, "constant FNAME less than 3");

	if ( strlen(&mapped[mapcount][0]) > FNAME )
		error(FATAL, "font name %s too long", &mapped[mapcount][0]);

	while ( nfonts < NFONT )  {
		while ( (c = getc(fp_map)) == ' '  ||  c == '\t') ;
		if ( c == '\n'  ||  c == EOF )
			break;
		ungetc(c, fp_map);
		fscanf(fp_map, "%s", &fmap[nfonts][0]);
		if ( strlen(&fmap[nfonts++][0]) > FNAME )
			error(FATAL, "font name %s too long", &fmap[nfonts-1][0]);
	}	/* End while */

	if ( nfonts >= NFONT )
		while ( (c = getc(fp_map)) != '\n'  &&  c != EOF ) ;

	strcpy(&fmap[nfonts][0], "bld");

	return(nfonts);

}	/* End of readfonts */


/*****************************************************************************/


readbuild()


{


	/********************************************************************
	 *																	*
	 *		Called from readmap() to read a 'build' list from *fontmap.	*
	 *	This list consists of those characters that will be built up	*
	 *	by the post-processor. They will be written out to a special	*
	 *	font file named *fmap[0].bld with a character code of 0. The	*
	 *	list is terminated by the string "end".							*
	 *																	*
	 ********************************************************************/


	bldcount = 0;						/* throw the last copy list out */

	while ( bldcount < MAXBUILD )  {
		if ( fscanf(fp_map, "%s", &bldlist[bldcount][0]) != 1 )
			error(FATAL, "bad build list format");

		if ( strcmp(&bldlist[bldcount][0], "end") == 0 )
			return;

		if ( strlen(&bldlist[bldcount][0]) > CLENGTH )
			error(FATAL, "copy name %s too long", &bldlist[bldcount][0]);

		bldcount++;
	}	/* End while */

	error(FATAL, "too many build characters");

}	/* End of readbuild */


/*****************************************************************************/


readsyns()


{


	/********************************************************************
	 *																	*
	 *		Called from readmap() to read a synonym list from the		*
	 *	*fontmap. The list is terminated by any pair of identical		*
	 *	character strings whose length is no more than CLENGTH.			*
	 *	This synonym stuff is probably not needed. It was added to the	*
	 *	program early on.												*
	 *																	*
	 ********************************************************************/


	syncount = 0;						/* throw the last list out */

	while ( syncount < MAXSYNS )  {
		if ( fscanf(fp_map, "%s %s", &new[syncount][0], &old[syncount][0]) != 2 )
			error(FATAL, "bad synonym list format");

		if ( strlen(&new[syncount][0]) > CLENGTH )
			error(FATAL, "synonym name %s too long", &new[syncount][0]);

		if ( strlen(&old[syncount][0]) > CLENGTH )
			error(FATAL, "synonym name %s too long", &old[syncount][0]);

		if ( strcmp(&new[syncount][0], &old[syncount][0]) == 0 )
			return;

		syncount++;						/* its a real synonym */
	}	/* End while */

	error(FATAL, "too many synonyms");

}	/* End of readsyns */


/*****************************************************************************/


get_ligs()


{


	char	lig;


	/********************************************************************
	 *																	*
	 *		Called from makefonts() to build up an appropriate ligature	*
	 *	list for the current font. This routine should not be needed	*
	 *	because the new tables will probably only be used by post-		*
	 *	processors, and they usually won't look at the font.ligfont		*
	 *	field. It is the right way to do things, so I decided to leave	*
	 *	the routine in for now - just in case.							*
	 *																	*
	 ********************************************************************/


	lig = fontbase[0]->ligfont;

	if ( lig & LFI )
		fprintf(fontout[0], " fi");
	if ( lig & LFL )
		fprintf(fontout[0], " fl");
	if ( lig & LFF )
		fprintf(fontout[0], " ff");
	if ( lig & LFFI )
		fprintf(fontout[0], " ffi");
	if ( lig & LFFL )
		fprintf(fontout[0], " ffl");

	fprintf(fontout[0], " 0\n");

}	/* End of get_ligs */


/*****************************************************************************/


copy_chars()


{


	char	ch[10];						/* standard stuff from makedev */
	char	width[10];
	char	kern[10];
	char	code[10];
	char	nch[10];					/* look this char up - synonym */
	char	buff[LINE_LENGTH];			/* next line from charset part */
	int		index;						/* found character on this font */
	int		i;							/* for loop index */


	/********************************************************************
	 *																	*
	 *		Called from makefonts() to copy the charset portion of the	*
	 *	current font file to the appropriate place in the *tdev			*
	 *	directory.														*
	 *																	*
	 ********************************************************************/


	index = -1;							/* needed because of synonyms */
	putc('\n', fontout[0]);				/* comes after 'charset' command */

	while ( fgets(buff, sizeof(buff), fontin) != NULL )  {

		if ( sscanf(buff, "%s %s %s %s", ch, width, kern, code) < 2 )
			continue;

		if ( width[0] == '"' )  {
			if ( index >= 0 )
				fprintf(fontout[index], "%s\t%s\n", ch, width);
			continue;
		}	/* End if */

		strcpy(nch, ch);
		for ( i = 0; i < syncount; i++ )
			if ( strcmp(ch, &old[i][0]) == 0 )  {
				strcpy(nch, &new[i][0]);
				break;
			}	/* End if */

		if ( (index = lookup(code, nch)) < 0 )  {
			if ( silent == NO )
				printf("  char %s not found\n", ch);
			continue;
		} else if ( fontout[index] == NULL )
			startfont(index);

		fprintf(fontout[index], "%s\t%s\t%s\t%s\n", ch, width, kern, code);

	}	/* End while */

}	/* End of copy_chars */


/*****************************************************************************/


startfont(index)


	int		index;						/* font's name is in fmap[index] */


{


	char	*nfont[2*FNAME+1];			/* name of the new font */


	/********************************************************************
	 *																	*
	 *		When this routine is called, we have found a character		*
	 *	from font mapped[mapcount] on fmap[index]. It is the first		*
	 *	time we needed this font so we have to create a new font file	*
	 *	with all the appropriate header stuff. This routine will never	*
	 *	be called with index equal to 0, since when a character is		*
	 *	found on fmap[0] it is written to *ndev's new ASCII font file	*
	 *	called mapped[mapcount][0]. This string is set in readfonts()	*
	 *	to the name of the first font in the current 'map' line in file	*
	 *	*fp_map, and the appropriate ASCII output file has already been	*
	 *	opened for writing in routine makefonts().						*
	 *																	*
	 ********************************************************************/


	strcat(strcat(strcpy(nfont, &mapped[mapcount][0]), "."), &fmap[index][0]);

	sprintf(path, "%s/dev%s/%s", targdir, ndev, nfont);
	if ( (fontout[index] = fopen(path, "w")) == NULL )
		error(FATAL, "can't write file %s", path);

	fprintf(fontout[index], "name %s\n", index != nfonts ? &fmap[index][0] : &fmap[0][0]);
	fprintf(fontout[index], "internalname %d\n", iname);
	fprintf(fontout[index], "charset\n");

	acount++;
	strcat(strcat(addfont, " "), nfont);	/* will be a default font */

}	/* End of startfont */


/*****************************************************************************/


lookup(code, ch)


	char	*code;						/* use this device code */
	char	*ch;						/* for this character */


{


	int		buildit;					/* did we find it in the build list? */
	int		c;							/* ch's index */
	int		j;							/* found on this font */
	int		i;							/* chars index on font j */


	/********************************************************************
	 *																	*
	 *		Called from copy_chars() to lookup the device code for the	*
	 *	character ch. If *ch isn't found we return -1, otherwise we		*
	 *	return the font's index where *ch was found.					*
	 *																	*
	 ********************************************************************/


	buildit = 0;							/* not in the build list */
	for ( i = 0; i < bldcount; i++ )		/* check the build list */
		if ( strcmp(ch, &bldlist[i][0]) == 0 )  {
			strcpy(code, "0");
			buildit = 1;
		}	/* End if */

	if ( ch[1] == '\0' )				/* ASCII character */
		c = ch[0];
	else  {								/* special troff character string */
		 for ( c = 0; c < nchtab; c++ )
			if ( strcmp(&chname[chtab[c]], ch) == 0 )
				break;
		 if ( c >= nchtab )
			return(buildit == 0 ? -1 : nfonts);
		c += 128;						/* special chars start at 128 */
	}	/* End if */

	c -= 32;							/* forget non-graphic characters */
	j = 0;

	while ( j < nfonts  &&  (i = fitab[j][c] & BMASK) == 0 )
		j++;

	if ( j >= nfonts  ||  i == 0 )		/* didn't find it on any font */
		return(buildit == 0 ? -1 : nfonts);

	sprintf(code, "%d", codetab[j][i] & BMASK);
	return(j);

}	/* End of lookup */


/*****************************************************************************/


loadfont(str, pos)


	char	*str;						/* name of the font to load */
	int		pos;						/* read font data into this position */


{


	char	*fontpos;					/* this font's position */
	int		fin;						/* font's .out file descriptor */
	int		nw;							/* number of width entries for font */


	/********************************************************************
	 *																	*
	 *		Called from makefonts() to load the .out file for font *str	*
	 *	from the 'master' device's real font directory.					*
	 *																	*
	 ********************************************************************/


	sprintf(path, "%s/dev%s/%s.out", fontdir, mdev, str);
	if ( (fin = open(path, 0)) < 0 )
		error(FATAL, "can't open file %s", path);

	fontpos = fontstart + (fsize * pos);
	read(fin, fontpos, fsize);

	fontbase[pos] = (struct Font *) fontpos;
	nw = fontbase[pos]->nwfont & BMASK;

	widthtab[pos] = (char *) fontpos + sizeof(struct Font);
	codetab[pos] = (char *) widthtab[pos] + 2 * nw;
	fitab[pos] = (char *) widthtab[pos] + 3 * nw;

	close(fin);

}	/* End of loadfont */


/*****************************************************************************/


error(kind, str, a1, a2)


	int			kind;
	char		*str;
	unsigned	a1, a2;


{


	/********************************************************************
	 *																	*
	 *		Print the error message *str, and quit if the error was		*
	 *	FATAL.															*
	 *																	*
	 ********************************************************************/


	fprintf(fp_error, "make%s: ", tdev);

	if ( kind == NON_FATAL )
		fprintf(fp_error, "warning - ");

	fprintf(fp_error, str, a1);
	putc('\n', fp_error);

	if ( kind == FATAL )
		exit(1);

}	/* End of error */


/*****************************************************************************/
