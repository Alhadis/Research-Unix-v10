/*
  makedev:
	read text info about a particular device
	(e.g., cat, 202, aps5) from file, convert
	it into internal (binary) form suitable for
	fast reading by troff initialization (ptinit()).

	Usage:

	makedev DESC [ F ... ]
		uses DESC to create a description file
		using the information therein.
		It creates the file DESC.out.

	makedev F ...
		makes the font tables for fonts F only,
		creates files F.out.

	DESC.out contains:
	dev structure with fundamental sizes
	list of sizes (nsizes+1) terminated by 0, as short's
	indices of char names (nchtab * sizeof(short))
	char names as hy\0em\0... (lchname)
	nfonts occurrences of
		widths (nwidth)
		kerning (nwidth) [ascender+descender only so far]
		codes (nwidth) to drive actual typesetter
		fitab (nchtab+128-32)
	each of these is an array of char.

	dev.filesize contains the number of bytes
	in the file, excluding the dev part itself.

	F.out contains the font header, width, kern, codes, and fitab.
	Width, kern and codes are parallel arrays.
	(Which suggests that they ought to be together?)
	Later, we might allow for codes which are actually
	sequences of formatting info so characters can be drawn.
*/

#include	"stdio.h"
#include	"dev.h"

#define	BMASK	0377

#define	SLANT_BIT	1					/* slant flag is bit 0 */
#define	FONT_BIT	2					/* font flag is bit 1 */
#define	RANGE_BIT	4					/* range flag is bit 2 */

#define	SLANT_VAL	3					/* slant angle starts in bit 3 */
#define	RANGE_VAL	5					/* max range starts in bit 5 */

#define	skipline(f)	while(getc(f) != '\n')

struct	dev	dev;
struct	font	font;

#define	NSIZE	100	/* maximum number of sizes */
short	size[NSIZE];
#define	NCH	256	/* max number of characters with funny names */
char	chname[5*NCH];	/* character names, including \0 for each */
short	chtab[NCH];	/* index of character in chname */

#define	NFITAB	(NCH + 128-32)	/* includes ascii chars, but not non-graphics */
char	fitab[NFITAB];	/* font index table: position of char i on this font. */
			/* zero if not there */

#define	FSIZE	200	/* size of a physical font (e.g., 102 for cat) */
char	width[FSIZE];	/* width table for a physical font */
char	kern[FSIZE];	/* ascender+descender info */
char	code[FSIZE];	/* actual device codes for a physical font */
char	alt_code[FSIZE];				/* code for alternate font */
int		alt_font[FSIZE];				/* alternate font to use */

int		dbg = 0;						/* debug flag */


#define	NFONT	10	/* max number of default fonts */
char	fname[NFONT][10];	/* temp space to hold default font names */

int	fflag	= 0;	/* on if font table to be written */
int	fdout;	/* output file descriptor */
char	*fout	= "DESC.out";

main(argc, argv)
char *argv[];
{
	FILE *fin;
	char cmd[100], *p;
	int i, totfont, v;

	if ((fin = fopen("DESC", "r")) == NULL) {
		fprintf(stderr, "makedev: can't open %s\n", argv[1]);
		exit(1);
	}
	while (fscanf(fin, "%s", cmd) != EOF) {
		if (cmd[0] == '#')	/* comment */
			skipline(fin);
		else if (strcmp(cmd, "debug") == 0)  
			dbg++;
		else if (strcmp(cmd, "res") == 0) {
			fscanf(fin, "%hd", &dev.res);
		} else if (strcmp(cmd, "hor") == 0) {
			fscanf(fin, "%hd", &dev.hor);
		} else if (strcmp(cmd, "vert") == 0) {
			fscanf(fin, "%hd", &dev.vert);
		} else if (strcmp(cmd, "unitwidth") == 0) {
			fscanf(fin, "%hd", &dev.unitwidth);
		} else if (strcmp(cmd, "sizescale") == 0) {
			fscanf(fin, "%hd", &dev.sizescale);
		} else if (strcmp(cmd, "paperwidth") == 0) {
			fscanf(fin, "%hd", &dev.paperwidth);
		} else if (strcmp(cmd, "paperlength") == 0) {
			fscanf(fin, "%hd", &dev.paperlength);
		} else if (strcmp(cmd, "spare1") == 0) {
			fscanf(fin, "%hd", &dev.spare1);
		} else if (strcmp(cmd, "spare2") == 0) {
			fscanf(fin, "%hd", &dev.spare2);
		} else if (strcmp(cmd, "sizes") == 0) {
			dev.nsizes = 0;
			while (fscanf(fin, "%d", &v) != EOF && v != 0)
				size[dev.nsizes++] = v;
			size[dev.nsizes] = 0;	/* need an extra 0 at the end */
		} else if (strcmp(cmd, "fonts") == 0) {
			fscanf(fin, "%hd", &dev.nfonts);
			for (i = 0; i < dev.nfonts; i++)
				fscanf(fin, "%s", fname[i]);
		} else if (strcmp(cmd, "charset") == 0) {
			p = chname;
			dev.nchtab = 0;
			while (fscanf(fin, "%s", p) != EOF) {
				chtab[dev.nchtab++] = p - chname;
				while (*p++)	/* skip to end of name */
					;
			}
			dev.lchname = p - chname;
			chtab[dev.nchtab++] = 0;	/* terminate properly */
		} else
			fprintf(stderr, "makedev: unknown command %s\n", cmd);
	}
	if (argc > 0 && strcmp(argv[1], "DESC") == 0) {
		fdout = creat(fout, 0666);
		if (fdout < 0) {
			fprintf(stderr, "makedev: can't open %s\n", fout);
			exit(1);
		}
		write(fdout, &dev, sizeof(struct dev));
		write(fdout, size, (dev.nsizes+1) * sizeof(size[0]));	/* we need a 0 on the end */
		write(fdout, chtab, dev.nchtab * sizeof(chtab[0]));
		write(fdout, chname, dev.lchname);
		totfont = 0;
		for (i = 0; i < dev.nfonts; i++) {
			totfont += dofont(fname[i]);
			write(fdout, &font, sizeof(struct font));
			write(fdout, width, font.nwfont & BMASK);
			write(fdout, kern, font.nwfont & BMASK);
			write(fdout, code, font.nwfont & BMASK);
			write(fdout, fitab, dev.nchtab+128-32);
		}
		lseek(fdout, 0L, 0);	/* back to beginning to install proper size */
		dev.filesize =		/* excluding dev struct itself */
			(dev.nsizes+1) * sizeof(size[0])
			+ dev.nchtab * sizeof(chtab[0])
			+ dev.lchname * sizeof(char)
			+ totfont * sizeof(char);
		write(fdout, &dev, sizeof(struct dev));
		close(fdout);
		argc--;
		argv++;
	}
	for (i = 1; i < argc; i++)
		dofont(argv[i]);
	exit(0);
}


/*****************************************************************************/


dofont(name)


	char	*name;						/* string containing name of font */


{


	FILE	*fin;						/* input file descriptor */
	int		fdout;						/* output file descriptor */
	int		i, nw, spacewidth, n, v;
	char 	buf[100], ch[10], s1[10], s2[10], s3[10], cmd[30];
	char	s4[10];						/* used to check for extra info */
	int		count;						/* value returned from sscanf() */
	int		dflt_range;					/* default maximum range for font */
	int		dflt_slant;					/* default slant for this font */



	/********************************************************************
	 *																	*
	 *		This routine is responsible for making the '.out' file for	*
	 *	the font specified by the parameter name. It also sets up some	*
	 *	of the data structures needed to make the DESC.out file in the	*
	 *	main routine. In addition I have changed this routine so that	*
	 *	we can add some typesetter dependent parameters to the font		*
	 *	files and have this information dumped out to a 'add' file		*
	 *	for this font, which can be read as needed by the aps driver.	*
	 *																	*
	 ********************************************************************/



	if ((fin = fopen(name, "r")) == NULL) {
		fprintf(stderr, "makedev: can't open font %s\n", name);
		exit(2);
	}	/* End if */

	sprintf(cmd, "%s.out", name);		/* output file is name.out */
	fdout = creat(cmd, 0666);			/* create the '.out' font file */

	font.specfont = 0;					/* by default it isn't special font */
	font.ligfont = 0;					/* it also has no ligatures either */
	font.spare1 = 0;					/* all the flags are initially off */
	spacewidth = 0;						/* tables may specify a spacewidth */
	dflt_range = 3;						/* max range available on the APS-5 */
	dflt_slant = 0;						/* no slant for this font yet */

	for (i = 0; i < NFITAB; i++)		/* initialize font index table */
		fitab[i] = 0;

	for (i = 0; i < FSIZE; i++)  {		/* initialize character tables */
		width[i] = kern[i] = code[i] = 0;
		alt_font[i] = 0;
		alt_code[i] = 0;
	}	/* End for */

	while (fscanf(fin, "%s", cmd) != EOF)  {	/* read the font file */

		if (strcmp(cmd, "name") == 0)
			fscanf(fin, "%s", font.namefont);
		else if (strcmp(cmd, "internalname") == 0)
			fscanf(fin, "%s", font.intname);
		else if (strcmp(cmd, "special") == 0)
			font.specfont = 1;
		else if (strcmp(cmd, "ligatures") == 0) {
			skipline(fin);	/* skip over any names for now */
			font.ligfont = 1;
		} else if (strcmp(cmd, "spacewidth") == 0) {
			fscanf(fin, "%d", &spacewidth);
			width[0] = spacewidth;	/* width of space on this font */
		} else if (strcmp(cmd, "charset") == 0) {
			skipline(fin);
			nw = 0;

			/* widths are origin 1 so fitab==0 can mean "not there" */

			while (fgets(buf, 100, fin) != NULL)  {		/* read next line */

				count = sscanf(buf, "%s %s %s %s %s", ch, s1, s2, s3, s4);

				if (s1[0] != '"')  {	/* it's a genuine new character */

					nw++;
					width[nw] = atoi(s1);
					kern[nw] = atoi(s2);

					/* temporarily, pick up one byte as code */

					if ( s3[0] == '0' )			/* code is in octal */
						sscanf(s3, "%o", &i);
					else
						sscanf(s3, "%d", &i);	/* otherwise it is decimal */
					code[nw] = i;

					if ( count > 4  &&  font.spare1 )	/* process the rest */
						finish_line(buf, nw);	

				}	/* End if */

				/* otherwise it's a synonym for previous character,
				/* so leave previous values intact
				*/

				if (strlen(ch) == 1)	/* it's ascii */
					fitab[ch[0] - 32] = nw;	/* fitab origin omits non-graphics */
				else {		/* it has a funny name */
					for (i = 0; i < dev.nchtab; i++)
						if (strcmp(&chname[chtab[i]], ch) == 0) {
							fitab[i + 128-32] = nw;	/* starts after the ascii */
							break;
						}	/* End if */

					if (i >= dev.nchtab)
						fprintf(stderr, "makedev: font %s: %s not in charset\n", name, ch);
				}	/* End else */

			}	/* End while */
			nw++;
			font.nwfont = n = nw;
		} else if ( strcmp(cmd, "alternate_font") == 0 )
			font.spare1 |= FONT_BIT;		/* set alternate font bit */
		else if ( strcmp(cmd, "default_slant") == 0 )  {
			font.spare1 &= ~(3 << SLANT_VAL);	/* clear two slant val bits */
			font.spare1 |= SLANT_BIT;			/* set font slant bit */
			fscanf(fin, "%d", &dflt_slant);	/* read the default slant value */
			if ( dflt_slant > 0 )			/* encode it as 01 */
				font.spare1 |= (1 << SLANT_VAL);
			else if ( dflt_slant < 0 )		/* encode it as 10 */
				font.spare1 |= (1 << (SLANT_VAL + 1));
		} else if ( strcmp(cmd, "max_range") == 0 )  {
			font.spare1 |= RANGE_BIT;		/* set range bit in spare1 */
			fscanf(fin, "%d", &dflt_range);	/* read default font slant */
			if ( dflt_range < 1  ||  dflt_range > 4 )  {
				fprintf(stderr, "makedev: illegal default range %d\n",dflt_range);
				exit(1);
			}	/* End if */
			font.spare1 |= (dflt_range << RANGE_VAL);
		} else if ( strcmp(cmd, "debug") == 0 )
			dbg++;

	}	/* End while */

	if (spacewidth == 0)
		width[0] = dev.res * dev.unitwidth / 72 / 3;
	fclose(fin);

	write(fdout, &font, sizeof(struct font));
	write(fdout, width, font.nwfont & BMASK);
	write(fdout, kern, font.nwfont & BMASK);
	write(fdout, code, font.nwfont & BMASK);
	write(fdout, fitab, dev.nchtab+128-32);
	close(fdout);

	if ( font.spare1 ) add_font(name);

	if ( dbg ) dump_font();

	v = sizeof(struct font) + 3 * n + dev.nchtab + 128-32;
	fprintf(stderr, "%3s: %3d chars, width %3d, size %3d\n",
		font.namefont, nw, width[0], v);
	return v;

}	/* End of dofont */


/*****************************************************************************/


finish_line(buf, nw)


	char	*buf;						/* contains current input line */
	int		nw;							/* current index in char tables */


{


	char	s[3][20];					/* used for special char commands */
	char	v[3][20];					/* used to hold values */
	int		count;						/* number of vals read in sscanf() */
	int		range;						/* value of max range for character */
	int		angle;						/* value of slant angle for char */
	int		i;							/* for loop index */



	/********************************************************************
	 *																	*
	 *		This routine is called from dofont() to finish scanning an	*
	 *	input line which has additional character information on it.	*
	 *	The line was read in dofont() and put in the buffer buf, while	*
	 *	the current index that we are using for the character tables is	*
	 *	the value of the parameter nw. The format of the additional		*
	 *	information that may appear in a line in the character set part	*
	 *	of a font file is shown below,									*
	 *																	*
	 *				 slant l  max_range m  font n						*
	 *																	*
	 *	where l, m, and n are integers. Any or all of these commands	*
	 *	may appear on a given line after the normal code entry for the	*
	 *	current character.												*
	 *																	*
	 ********************************************************************/



	count = sscanf(buf,"%*s %*s %*s %*s %s %s %s %s %s %s",s[0],v[0],s[1],v[1],s[2],v[2]);

	if ( (count % 2 != 0) || (count == 0) )  {
		fprintf(stderr, "makedev: format error in charset table\n");
		exit(1);
	}	/* End if */

	alt_code[nw] = code[nw];			/* store real code in alt_code array */
	code[nw] = 128;						/* turn bit 7 on */

	for ( i = 0; i < count/2; i++ )  {		/* process the strings */

		if ( strcmp(s[i], "font") == 0 )  {		/* have an alternate font */
			code[nw] |= FONT_BIT;		/* turn font bit on */
			alt_font[nw] = atoi(v[i]);	/* save this font number */
		} else if ( strcmp(s[i], "max_range") == 0 )  {
			code[nw] &= ~(3 << RANGE_VAL);
			code[nw] |= RANGE_BIT;
			range = atoi(v[i]);
			if ( range < 1  ||  range > 4 )  {
				fprintf(stderr, "makedev: illegal range\n");
				exit(1);
			}	/* End if */
			code[nw] |= (range << RANGE_VAL);
		} else if ( strcmp(s[i], "slant" ) == 0 )  {
			code[nw] &= ~(3 << SLANT_VAL);
			code[nw] |= SLANT_BIT;
			angle = atoi(v[i]);
			if ( angle > 0 )			/* set angle bits to 01 */
				code[nw] |= (1 << SLANT_VAL);
			else if ( angle < 0 )		/* set angle bits to 10 */
				code[nw] |= (1 << (SLANT_VAL + 1));
		} else {						/* illegal command - abort */
			fprintf(stderr, "makedev: illegal command\n");
			exit(1);
		}	/* End else */

	}	/* End for */
	if ( code[nw] & BMASK == 128 )  {
		fprintf(stderr, "makedev: char code %d less than 129\n", code[nw]);
		exit(1);
	}	/* End if */

}	/* End of finish_line */


/*****************************************************************************/


add_font(name)


	char	*name;						/* name of currnent font */


{


	int		fdout;						/* output file descriptor */
	int		k;							/* number of width entries */
	char	cmd[30];					/* used to create output file */



	/********************************************************************
	 *																	*
	 *		This routine is called from dofont() to write out the 		*
	 *	additional information that was specified in the input font		*
	 *	file. As currently implemented the alternate code table and		*
	 *	the alternate font tables are always written to the output		*
	 *	file. 															*
	 *																	*
	 ********************************************************************/



	sprintf(cmd, "%s.add", name);		/* file will be 'name'.add */
	fdout = creat(cmd, 0666);			/* create the '.add' file */

	k = font.nwfont & BMASK;

	write(fdout, alt_font, FSIZE * sizeof(alt_font[0]));
	write(fdout, alt_code, FSIZE);

	close(fdout);

}	/* End of add_font */


/*****************************************************************************/


dump_font()


{


	int		pos;						/* position in character tables */
	int		j;							/* for loop index */
	int		code_val;					/* number in code table */



	/********************************************************************
	 *																	*
	 *		This routine is called to dump the information that we have	*
	 *	read from the current font file.								*
	 *																	*
	 ********************************************************************/



	printf("DATA FOR FONT %s:\n\n", font.namefont);

	printf("  font structure data:\n");
	printf("\t\tfont.nwfont = %d\n", font.nwfont & BMASK);
	printf("\t\tfont.specfont = %d\n", font.specfont & BMASK);
	printf("\t\tfont.ligfont = %d\n", font.ligfont & BMASK);
	printf("\t\tfont.spare1 = 0%o\n", font.spare1 & BMASK);
	printf("\t\tfont.intname = %s\n\n", font.intname);

	printf("  CHAR     WIDTH      KERN      CODE     INDEX\n");

	for ( j = 0; j < dev.nchtab + 128 - 32; j++ )  {

		if ( (pos = fitab[j] & BMASK) != 0 )  {
			if ( j >= 96 )
				printf("%5s", &chname[chtab[j-96]]);
			else printf("%5c", (j + 32) & BMASK);
			printf("%10d", width[pos] & BMASK);
			printf("%10d", kern[pos] & BMASK);
			code_val = code[pos] & BMASK;
			printf("%10d", code_val & BMASK);
			printf("%10d", j);

			if ( code_val > 128 )  {
				printf("%5d", alt_code[pos] & BMASK);
				if ( code_val & SLANT_BIT )  
					printf("  slant %d", (code_val >> SLANT_VAL) & 03);
				if ( code_val & FONT_BIT )
					printf("  font %d", alt_font[pos]);
				if ( code_val & RANGE_BIT )
					printf("  range %d", (code_val >> RANGE_VAL) & 03);
			}	/* End if */

			printf("\n");
		}	/* End if */

	}	/* End for */

	printf("\n\n");

}	/* End of dump_font */


/*****************************************************************************/


