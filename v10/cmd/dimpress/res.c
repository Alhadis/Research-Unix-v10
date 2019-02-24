

/*
 *
 * A few routines used to handle printer resident fonts. They cause us
 * some problems because there's no guaranteed way the post-processor
 * can be sure what fonts are resident on a particular printer. It's
 * certainly possible that two otherwise identical printers could
 * support different resident fonts, and we need to be able to deal
 * with that situation. The way I've decided to handle things is have
 * the post-processor look for special files in *bitdir/RESIDENT as
 * part of the startup procedure. If the file's found and can be
 * read it's used to get all the required resident font data for
 * the printer. If we can't read the control file we'll just assume
 * there aren't any resident fonts. Usually the file we're looking for
 * will have the same name as the target printer, although that can
 * be changed by an option - so we can handle different sets of
 * resident fonts.
 *
 * The control file is ASCII right now and names the resident fonts,
 * the available sizes, family tables, and the member maps. All the
 * required maps are read and immediately written to the output file.
 * Everything else is saved in resdata[] and only used when needed.
 * See the comments at the start of resfonts() for a more complete
 * description of the file.
 *
 * The member maps and family tables come from Imagen, although not
 * directly. I got the info for the lucida fonts by taking one of
 * their programs (mkres) and forcing routine dump_data() to run.
 * The output from mkres was then used to build the control file.
 * You may have to do something similiar if you've got a new set
 * of printer resident fonts.
 *
 * There are other problems, most caused by Imagen's resident fonts.
 * The most annoying is the need to use member maps and family tables.
 * As it turns out the available member codes aren't good enough to
 * access all the required characters in a resident font. That means
 * we can't just use map 0 and the character code available in the
 * font files. Things sure would have been simpler if resident fonts
 * were more logical. Actually I'm not convinced resident fonts are
 * that great an idea.
 *
 */


#include <stdio.h>

#include "gen.h"			/* definitions used by everyone */
#include "ext.h"			/* external variable declarations */
#include "init.h"			/* just used to get RESDIR */
#include "rast.h"			/* raster file definitions */
#include "impcodes.h"			/* Impress 2.0 opcodes */


Resdata	resdata[MAXRESIDENT];		/* used to manage resident fonts */
int	maxres = 0;			/* entries so far in resdata[] */


/*
 *
 * We'll want to add size list data for the resident fonts to the
 * sizedata[] structure so mapsize() works properly.
 *
 */


extern Sizedata	sizedata[];		/* so we can add resident font sizes */
extern int	maxrast;


/*****************************************************************************/


resfonts(tf)


    FILE	*tf;			/* Impress commands go here */


{


    char	name[60];		/* control file pathname */
    char	cmd[20];		/* commands read from *name */
    int		i;			/* fill in resdata[i] next */
    int		member;			/* starting member for triple */
    int		symbol;			/* starting symbol */
    int		count;			/* members mapped by this triple */
    int		j, n;			/* used for a bunch of things */
    FILE	*fp;


/*
 *
 * Resident fonts require some rather special attention. We'll need to
 * keep track of all the resident font names and their associated
 * family tables. In addition we'll need to download all the required
 * member maps. All this stuff is expected to be in file *resfile
 * which is located in *bitdir/RESIDENT. If the file can't be read
 * we'll just assume there aren't any printer resident fonts.
 *
 * For now the control file is ASCII, even though there are more
 * efficient ways of doing things. A decent alternative would be
 * to have all the required Impress commands in the file. That
 * way the file could just be copied directly to the output file.
 * Member maps work nicely that way, but the family tables pose
 * some problems because we've got to add the size to the font
 * name string. Anyway there other choices and most any of them
 * would cause the post-processor to run faster.
 *
 * For now the control file format looks something like,
 *
 *
 *	font <name1>
 *	sizes <size list> 0
 *	families <n>
 *		<map1>	<font1>
 *		      .
 *		      .
 *                    .
 *		<mapn>	<fontn>
 *	font <name2>
 *	    .
 *	    .
 *	    .
 *	font <namen>
 *	map <mapname1> <n>
 *		<mem1> <symbol1> <count1>
 *			   .
 *			   .
 *			   .
 *		<memn> <symboln> <countn>
 *	map <mapname2> <n>
 *
 *		.
 *		.
 *		.
 *
 *	map <mapnamen> <n>
 *		.
 *		.
 *		.
 *	      (EOF)
 *
 *
 * The <size list> data must be given in increasing order and terminated
 * with a 0 entry. Comments can be put in the file, although I don't
 * recommend it. They're lines that have '#' as the first non white
 * space character.
 *
 * The font names and family tables are saved in resdata[], size info
 * is added to sizedata[], and the member maps are immediately downloaded.
 *
 * I'm not responsible for the member maps or family tables. I got the
 * ones for the lucida font by running Imagen's mkres program with the
 * dump_data() routine working (just took a few ifdefs out). The output
 * from that routine was used to build the ASCII control file.
 *
 */


    sprintf(name, "%s/%s/%s", bitdir, RESDIR, resfile);
    if ( (fp = fopen(name, "r")) == NULL )
	return;

    i = maxres - 1;			/* incremented when "font" is read */

    while ( fscanf(fp, "%s", cmd) != EOF )  {
	if ( strcmp(cmd, "font") == 0 )
	    fscanf(fp, "%10s", resdata[++i].name);
	else if ( strcmp(cmd, "sizes") == 0 )  {
	    strncpy(sizedata[maxrast].name, resdata[i].name, L_FNAME);
	    j = 0;
	    while ( fscanf(fp, "%d", &n) != EOF  &&  n != 0 )
		sizedata[maxrast].sizes[j++] = n;
	    sizedata[maxrast].sizes[j] = 999;
	    if ( ++maxrast >= MAXFONTS )
		error(FATAL, "too many fonts in %s", name);
	} else if ( strcmp(cmd, "families") == 0 )  {
	    fscanf(fp, "%d", &resdata[i].count);
	    for ( j = 0; j < resdata[i].count; j++ )
		fscanf(fp, "%d %10s", &resdata[i].table[j].map, resdata[i].table[j].name);
	} else if ( strcmp(cmd, "map") == 0 )  {
	    fscanf(fp, "%d %d", &n, &j);
	    putc(ACM, tf);
	    putc(n, tf);
	    putc(j, tf);
	    for ( ; j > 0; j-- )  {
		fscanf(fp, "%d %d %d", &member, &symbol, &count);
		putc(member, tf);
		putint(symbol, tf);
		putc(count, tf);
	    }	/* End for */
	} else if ( cmd[0] == '#' )
	    while ( (n = getc(fp)) != EOF  &&  n != '\n' ) ;
	else error(FATAL, "bad command %s in file %s", cmd, name);
    }	/* End while */

    maxres = i + 1;

    fclose(fp);

}   /* End of resfonts */


/*****************************************************************************/


isresident(name)


    char	*name;			/* is this a printer resident font? */


{


    int		i;			/* check res_data[i] next */


/*
 *
 * Returns TRUE if font *name is a resident font and FALSE otherwise.
 * Resident fonts are those listed in resdata[] array, which is set up
 * in routine resfonts() using file *bitdir/RESIDENT/resfile.
 *
 */


    for ( i = 0; i < maxres; i++ )
	if ( strncmp(name, resdata[i].name, L_FNAME) == 0 )
	    return(TRUE);

    return(FALSE);

}   /* End of isresident */


/*****************************************************************************/


getresdata(f, s, tf)


    char	*f;			/* name of the resident font */
    int		s;			/* in this point size */
    FILE	*tf;			/* job's output file */


{


    int		i;			/* loop index for setting famlies */


/*
 *
 * Makes sure data about resident font *f in size s is added to the
 * fam_data[] array. Essentially the same as routine getrastdata()
 * except that the chused bitmap is set to all ones so download()
 * will know the printer already has the glyph. Also don't need the
 * advance stuff saved in fam_data[].
 *
 */


    cur_fam = getfamdata(f, s);		/* using this family next */
    fam = &fam_data[cur_fam];

    if ( cur_fam >= next_fam )  {	/* first use */
	if ( next_fam > MAXFAMILY )	/* can only use 0 to 95 */
	    error(FATAL, "job uses too many font families");

	strncpy(fam->name, f, L_FNAME);	/* initialize the fields */
	fam->name[L_FNAME-1] = '\0';
	fam->size = s;
	fam->rst = NULL;
	fam->advance = NULL;
	fam->glyphdir = NULL;
	fam->first = 0;
	fam->last = 128;

	for ( i = 0; i < 4; i++ )  {
	    fam->chused[i] = (unsigned char *) rastalloc((MAXMEMBER+BYTE)/BYTE, TRUE, 0377);
	    fam->fam[i] = -1;
	}   /* End for */

	famtables(f, s, tf);		/* use resdata[] to define families */

	next_fam++;
    }	/* End if */

    return(cur_fam);			/* probably never used */

}   /* End of getresdata */


/*****************************************************************************/


famtables(f, s, tf)


    char	*f;			/* define tables for this font */
    int		s;			/* in this size */
    FILE	*tf;			/* commands written here */


{


    int		i;			/* fonts position in resdata[] */
    int		j;			/* next family pair */


/*
 *
 * Defines the family tables for font *f in size s using the data that
 * was read from the ASCII control file and saved in resdata[]. We'll
 * have to look font f up in resdata[] before doing anything. Although
 * that wastes time because it was just done in isresident() it shouldn't
 * be all that expensive because the family tables aren't downloaded
 * that often. There are easy ways to get around it if you want.
 *
 */


    for ( i = 0; i < maxres; i++ )
	if ( strncmp(f, resdata[i].name, L_FNAME) == 0 )
	    break;

    if ( i >= maxres )			/* probably can't ever happen */
	error(FATAL, "can't ffind family data for font %s", f);

    putc(ACFT, tf);
    putc(cur_fam, tf);
    putc(resdata[i].count, tf);

    for ( j = 0; j < resdata[i].count; j++ )  {
	putc(resdata[i].table[j].map, tf);
	fprintf(tf, "%.10s%d", resdata[i].table[j].name, s);
	putc('\0', tf);
    }	/* End for */

}   /* End of famtables */


/*****************************************************************************/


