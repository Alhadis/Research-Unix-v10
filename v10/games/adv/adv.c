/* Adventure (3/6/79 17:32:41 1.20)
   Current limits:
        999 lines, linsiz of message text (lines).
        750 travel options (travel, trvsiz).
        300 vocabulary words (ktab, atab, tabsiz).
        150 locations (ltext, stext, key, cond, abb, atloc, locsiz).
        100 objects (plac, place, fixd, fixed, rlink (twice), ptext,
                    prop).
         35 "action" verbs (actspk, vrbsiz).
        205 random messages (rtext, rtxsiz).
         12 different player classifications (ctext, cval, clsmax).
         20 hints, less 3 (hintlc, hinted, hints, hntsiz).
         35 magic messages (mtext, magsiz).
   there are also limits which cannot be exceeded due to the
   structure of the database.  (e.g., the vocabulary uses
   n/1000 to determine word type, so there can't be more
   than 1000 words.) these upper limits are:
        1000 non-synonymous vocabulary words
        300 locations
        100 objects
 */
#include <stdio.h>
#include <signal.h>

long longabs(x)
	long x;
{
	return x<0? -x: x;
}

long getl (file)
	register FILE *file;
{
	long l;
	l = getw (file);
	l <<= 16;
	l |= getw (file);
	return l;
}

putl (l, file)
	long l;
	register FILE *file;
{
	putw ((int) (l >> 16), file);
	putw ((int) l, file);
}

FILE *caves, *suspfd;
long tvec, xtime, time();
long ftell();
struct passwd *pwbuf;
char suspbeg;	/* Start of variables to write during suspension */
char tkword[10];
char wd2x[5];
char wd2[5];
char wd1x[5];
char wd1[5];
char atab[300+1][6];
char linebuf[100];	/* For reading cave description, etc */
char chr;
char *cp1, *cp2;
int rtext[205];
long newloc = 0L;
long lines[1000];	/* Assumed initialized to zero */
int blklin = 1;
int dseen[6];
int odloc[6];
int dloc[6];
int tk[20+1];
int hinted[20+1];
int hinted[20+1];	/* Assumed initialized to 0 */
int hints[12+1][4+1];	/* Assumed initialized to 0 */
int cval[12+1];	/* Assumed initialized to 0 */
int ctext[12+1];	/* Assumed initialized to 0 */
int actspk[35+1];	/* Assumed initialized to 0 */
int prop[100+1];	/* Assumed initialized to 0 */
int fixd[100+1];	/* Assumed initialized to 0 */
int plac[100+1];	/* Assumed initialized to 0 */
int cond[150+1];	/* Assumed initialized to 0 */
int key[150+1];	/* Assumed initialized to 0 */
int stext[150+1];	/* Assumed initialized to 0 */
int ltext[150+1];	/* Assumed initialized to 0 */
long travel[750+1];	/* Assumed initialized to 0 */
int hintlc[21+1];	/* Assumed initialized to zero */
int abb[150+1];
int holdng;
int fixed[100+1];
int place[100+1];
int rlink[200+1];
int atloc[150+1];
int tabsiz = 300;
int ktab[300+1];
int ptext[100+1];
#ifdef	INITBUG
int clssiz = 12;
int linsiz = 999;
int vrbsiz = 35;
#endif	INITBUG
int trvsiz = 750;
int locsiz = 150;
int rtxsiz = 205;
int clsmax = 12;
int hntsiz = 20;
int wzdark;
int lmwarn;
int closng;
int panic;
int closed;
int gaveup;
int scorng;
int yea;
long ll;

#ifdef unix
#include <pwd.h>
#define GETPWUID() pwbuf=getpwuid(getuid());cp2=pwbuf->pw_dir;while(*cp1++ = *cp2++);cp1--;
#define HOURS "Colossal Cave is always open\n"
#ifndef	CAVE
#define CAVE "/usr/games/lib/cave"
#endif	CAVE
#define SUSPREAD "r"
#define SUSPWRITE "w"
#endif
#ifdef gcos
#define CLOSED "Colossal Cave is closed weekdays between 8 AM and 6 PM.\n"
#define GETPWUID()  if(lmsgrd()<3){printf(CLOSED);return(0);}fprompt("\r\n");
#define HOURS CLOSED
#define CAVE "cc/adve/cave$abracadabra"
#define SUSPREAD "ri"
#define SUSPWRITE "wi"
#endif

#define abbnum	ints[0]
#define iy	ints[1]
#define axe	ints[2]
#define back	ints[3]
#define batter	ints[4]
#define bear	ints[5]
#define bird	ints[6]
#define bonus	ints[7]
#define bottle	ints[8]
#define cage	ints[9]
#define cave	ints[10]
#define ccode	ints[11]
#define ch	ints[12]
#define chain	ints[13]
#define chasm	ints[14]
#define chest	ints[15]
#define chloc	ints[16]
#define chloc2	ints[17]
#define clam	ints[18]
#define clock1	ints[19]
#define clock2	ints[20]
#define clsses	ints[21]
#define coins	ints[22]
#define daltlc	ints[23]
#define detail	ints[24]
#define dflag	ints[25]
#define dkill	ints[26]
#define door	ints[27]
#define dprssn	ints[28]
#define dragon	ints[29]
#define dtotal	ints[30]
#define dwarf	ints[31]
#define eggs	ints[32]
#define emrald	ints[33]
#define entrnc	ints[34]
#define find	ints[35]
#define fissur	ints[36]
#define foo	ints[37]
#define foobar	ints[38]
#define food	ints[39]
#define from	ints[40]
#define grate	ints[41]
#define hint	ints[42]
#define hntmax	ints[43]
#define i	ints[44]
#define inlen	ints[45]
#define invent	ints[46]
#define iwest	ints[47]
#define j	ints[48]
#define k	ints[49]
#define keys	ints[50]
#define kk	ints[51]
#define knfloc	ints[52]
#define knife	ints[53]
#define kq	ints[54]
#define k2	ints[55]
#define l	ints[56]
#define lamp	ints[57]
#define limit	ints[58]
#define linuse	ints[59]
#define hungup	ints[60]
#define loc	ints[61]
#define lock	ints[62]
#define look	ints[63]
#define m	ints[64]
#define magzin	ints[65]
#define maxdie	ints[66]
#define maxtrs	ints[67]
#define messag	ints[68]
#define mirror	ints[69]
#define mxscor	ints[70]
#define nugget	ints[71]
#define nullx	ints[72]
#define numdie	ints[73]
#define obj	ints[74]
#define oil	ints[75]
#define oldlc2	ints[76]
#define oldloc	ints[77]
#define oyster	ints[78]
#define pearl	ints[79]
#define pillow	ints[80]
#define plant	ints[81]
#define plant2	ints[82]
#define posn	ints[83]
#define pyram	ints[84]
#define rod	ints[85]
#define rod2	ints[86]
#define rug	ints[87]
#define say	ints[88]
#define score	ints[89]
#define sect	ints[90]
#define snake	ints[91]
#define spices	ints[92]
#define spk	ints[93]
#define steps	ints[94]
#define stick	ints[95]
#define tablet	ints[96]
#define tabndx	ints[97]
#define tally	ints[98]
#define tally2	ints[99]
#define temp	ints[100]
#define throw	ints[101]
#define attack	ints[102]
#define tridnt	ints[103]
#define troll	ints[104]
#define troll2	ints[105]
#define trvs	ints[106]
#define turns	ints[107]
#define vase	ints[108]
#define vend	ints[109]
#define verb	ints[110]
#define water	ints[111]
#define word	ints[112]
#define wordend	ints[113]
#define wordsize	ints[114]
#define wordstrt	ints[115]
#define logon	ints[116]
#define	srel	ints[117]
#define	slev	ints[118]
#define	tleft	ints[119]
#define	tright	ints[120]
int ints[121];

char suspend;	/* End of variables to write during suspension */

/*
   wzdark says whether the loc he's leaving was dark
   lmwarn says whether he's been warned about lamp going dim
   closng says whether its closing time yet
   panic says whether he's found out he's trapped in the cave
   closed says whether we're all the way closed
   gaveup says whether he exited via "quit"
   scorng indicates to the score routine whether we're doing
   a "score" command
   yea is random yes/no reply
   hungup says whether he hung up the phone
 */

sethup()
{
	hungup = 1;
	signal (SIGHUP, SIG_IGN);

#ifndef	INITBUG
	signal (SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
#endif	INITBUG

}

main()
{
	char suspfile[100];
	static char outbuf[BUFSIZ];
	struct passwd *getpwuid();

	setbuf(stdout, outbuf);
	setbuf(stderr, NULL);
	printf ("@(#)Adventure 1.20\n" + 4);

	cp1 = suspfile;
	GETPWUID();
	cp2 = "/adv.susp";
	while (*cp1++ = *cp2++);

#ifndef	INITBUG
	if (signal (SIGINT, SIG_IGN) != SIG_IGN)
		signal (SIGINT, sethup);
	if (signal (SIGQUIT, sethup) != SIG_IGN)
		signal (SIGQUIT, sethup);
#endif	INITBUG

	if (signal (SIGHUP, SIG_IGN) != SIG_IGN)
		signal (SIGHUP, sethup);

#ifdef LOG
	logon = 1;
#else
	logon = 0;
#endif

	if ((caves = fopen (CAVE, "r")) == NULL)
		fatal ("Couldn't open cave file\n");

	/*  Description of the database format:
	   The data file contains several sections.  Each begins
	   with a line containing a number identifying the section,
	   and ends with a line containing "-1".
	   Section 1:  long form descriptions.  Each line contains a
	       location number, a tab, and a line of text.  The set
	       of (necessarily adjacent) lines whose numbers are x
	       form the long description of location x.
	   Section 2:  Short form descriptions.  Same format as long
	       form.  Not all places have short descriptions.
	   Section 3:  Travel table.  Each line contains a location
	       number (x), a second location number (y), and a list
	       of motion numbers (see section 4).  Each motion
	       represents a verb which will go to y if currently at
	       x.  y, in turn, is interpreted as follows:  Let
	       m=y/1000, n=y mod 1000.
	               If n<=300       it is the location to go to.
	               If 300<n<=500   n-300 is used in a computed
	                               goto to a section of special
	                               code.
	               If n>500        message n-500 from section 6
	                               is printed, and he stays
	                               wherever he is.
	       Meanwhile, m specifies the conditions on the motion.
	               If m=0          it's unconditional.
	               If 0<m<100      it is done with m%
	                               probability.
	               If m=100        unconditional, but forbidden
	                               to dwarves.
	               If 100<m<=200   he must be carrying object
	                               m-100.
	               If 200<m<=300   must be carrying or in same
	                               room as m-200.
	               If 300<m<=400   prop(m mod 100) must NOT be
	                               0.
	               If 400<m<=500   prop(m mod 100) must NOT be
	                               1.
	               If 500<m<=600   prop(m mod 100) must NOT be
	                               2, etc.
	       If the condition (if any) is not met, then the next
	       DIFFERENT "destination" value is used (unless it
	       fails to meet ITS conditions, in which case the
	       next is found, etc.).  Typically, the next dest will
	       be for one of the same verbs, so that its only use is
	       as the alternate destination for those verbs.  For
	       instance:
	           15      110022  29      31      34      35      23      43
	           15      14      29
	       This says that, from loc 15, any of the verbs 29, 31,
	       etc., will take him to 22 if he's carrying object 10,
	       and otherwise will go to 14.
	               11      303008  49
	               11      9       50
	       This says that, from 11, 49 takes him to 8 unless
	       prop(3)=0, in which case he goes to 9.  Verb 50 takes
	       him to 9 regardless of prop(3).
	   Section 4:  vocabulary.  Each line contains a number (n),
	       a tab, and a five-letter word.  Call m=n/1000.  If
	       m=0, then the word is a motion verb for use in
	       travelling (see section 3).  Else, if m=1, the word
	       is an object.  Else, if m=2, the word is an action
	       verb (such as "carry" or "attack").  Else, if m=3,
	       the word is a special case verb (such as "dig") and n
	       mod 1000 is an index into section 6.  Objects from 50
	       to (currently, anyway) 79 are considered treasures
	       (for pirate, closeout).
	   Section 5:  object descriptions.  Each line contains a
	       number (n), a tab, and a message.  If n is from 1 to
	       100, the message is the "inventory" message for
	       object n.  Otherwise, n should be 000, 100, 200,
	       etc., and the message should be the description of
	       the preceding object when its prop value is n/100.
	       The n/100 is used only to distinguish multiple
	       messages from multi-line messages; the prop info
	       actually requires all messages for an object to be
	       present and consecutive.  Properties which produce no
	       message should be given the message ">$<".
	   Section 6:  arbitrary messages.  Same format as sections
	       1, 2, and 5, except the numbers bear no relation to
	       anything (except for special verbs in section 4).
	   Section 7:  object locations.  Each line contains an
	       object number and its initial location (zero (or
	       omitted) if none).  If the object is immovable, the
	       location is followed by a "-1".  If it has two
	       locations (e.g.  the grate) the first location is
	       followed with the second, and the object is assumed
	       to be immovable.
	   Section 8:  action defaults.  Each line contains an
	       "action-verb" number and the index (in section 6) of
	       the default message for the verb.
	   Section 9:  liquid assets, etc.  Each line contains a
	       number (n) and up to 20 location numbers.  Bit n
	       (where 0 is the units bit) is set in cond(loc) for
	       each loc given.  The cond bits currently assigned
	       are:
	               0       light
	               1       if bit 2 is on:  on for oil, off for
	                       water
	               2       liquid asset, see bit 1
	               3       pirate doesn't go here unless
	                       following player
	       Other bits are used to indicate areas of interest to
	       "hint" routines:
	               4       trying to get into cave
	               5       trying to catch bird
	               6       trying to deal with snake
	               7       lost in maze
	               8       pondering dark room
	               9       at Witt's end
	       cond(loc) is set to 2, overriding all other bits, if
	       loc has forced motion.
	   Section 10:  class messages.  Each line contains a number
	       (n), a tab, and a message describing a classification
	       of player.  The scoring section selects the
	       appropriate message, where each message is considered
	       to apply to players whose scores are higher than the
	       previous n but not higher than this n.  Note that
	       these scores probably change with every modification
	       (and particularly expansion) of the program.
	   Section 11:  hints.  Each line contains a hint number
	       (corresponding to a cond bit, see section 9), the
	       number of turns he must be at the right loc(s) before
	       triggering the hint, the points deducted for taking
	       the hint, the message number (section 6) of the
	       question, and the message number of the hint.  These
	       values are stashed in the "hints" array.  hntmax is
	       set to the max hint number (<= hntsiz).  Numbers 1-3
	       are unusable since cond bits are otherwise assigned,
	       so 2 is used to remember if he's read the clue in the
	       repository, and 3 is used to remember whether he
	       asked for instructions (gets more turns, but loses
	       points).
	   Section 12:  magic messages.  Identical to section 6
	       except put in a separate section for easier
	       reference.  Magic messages are used by the startup,
	       maintenance mode, and related routines.
	   Section 0:  end of database.
	 */

	/*  Read the database */

	printf("Initializing...  ");
	fflush(stdout);

	/*
	   Clear out the various text-pointer arrays.  All text is
	   stored in array lines; each line is preceded by a word
	   pointing to the next pointer (i.e.  the word following
	   the end of the line).  The pointer is negative if this is
	   first line of a message.  The text-pointer arrays contain
	   indices of pointer-words in lines.  stext(n) is short
	   description of location n.  ltext(n) is long description.
	   ptext(n) points to message for prop(n)=0.  Successive
	   prop messages are found by chasing pointers.  rtext
	   contains section 6's stuff.  ctext(n) points to a
	   player-class message.  mtext is for section 12.  we also
	   clear cond.  see description of section 9 for details.
	 */
	for (i=1; i<=300; i++) {
		if (i <= 100) ptext[i] = 0;
		if (i <= rtxsiz) rtext[i] = 0;
		if (i <= clsmax) ctext[i] = 0;
		if (i <= locsiz) {
			stext[i]=0;
			ltext[i]=0;
			cond[i]=0;
		}
	}
	/* key=0; */
	linuse=1;
	trvs=1;
	clsses=1;

	/* Start new data section.  sect is the section number. */
l1002:
	fscanf (caves, "%d", &sect);
	oldloc= -1;

#ifdef	INITBUG
	fprintf(stderr, "\nSect=%d\n", sect);
#endif	INITBUG

	switch (sect) {
	case 0:  
		goto l1100;        /* (0) */
	case 1:  
	case 2:  
	case 5:  
	case 6:  
	case 10:  
		goto l1004;        /* 1, 2, 5, 6, 10 */
	case 3:  
		goto l1030;        /* (3) */
	case 4:  
		goto l1040;        /* (4) */
	case 7:  
		goto l1050;        /* (7) */
	case 8:  
		goto l1060;        /* (8) */
	case 9:  
		goto l1070;        /* (9) */
	case 11:  
		goto l1080;       /* (11) */
	}

	bug(9);

	/* Sections 1, 2, 5, 6, 10.  Read messages and set up pointers. */
l1004:
	fscanf (caves, "%d", &loc);
#ifdef	INITBUG
	fprintf(stderr, "Sect=%d; Loc=%d\n", sect, loc);
#endif	INITBUG
	while ((chr = getc (caves)) == ' ' || chr == '\t')
		;
	ungetc (chr, caves);
	lines[linuse] = ftell (caves);
	fgets (linebuf, sizeof linebuf, caves);
	if (loc == -1) goto l1002;
	if (loc != oldloc) {
		lines[linuse] = -lines[linuse];
		if (sect == 10) {
			ctext[clsses] = linuse;
			cval[clsses] = loc;
			clsses++;
		}
		else if (sect == 6) {
			if (loc > rtxsiz) bug(6);
			rtext[loc] = linuse;
		}
		else if (sect == 5) {
			if (loc > 0 && loc <= 100) ptext[loc] = linuse;
		}
		else if (sect == 1) {
			ltext[loc] = linuse;
		}
		else
			stext[loc] = linuse;
	}
	linuse++;
	oldloc = loc;
	goto l1004;
	/*
	   The stuff for section 3 is encoded here.  Each
	   "from-location" gets a contiguous section of the "travel"
	   array.  Each entry in travel is newloc*1000 + keyword
	   (from section 4, motion verbs), and is negated if this is
	   the last entry for this location.  key[n] is the index in
	   travel of the first option at location n.
	*/
l1030:
	fscanf (caves, "%d\t%ld", &loc, &newloc);
#ifdef	INITBUG
	fprintf(stderr, "Sect=%d; Loc=%d; newloc=%d\n", sect, loc, newloc);
#endif	INITBUG
	i = 0;
	while ((chr = getc (caves)) != '\n') {
		ungetc (chr, caves);
		fscanf (caves, "%d", &tk[i++]);
	}
	if (loc == -1) goto l1002;
	if (key[loc])
		travel[trvs-1] = -travel[trvs-1];
	else
		key[loc] = trvs;
	for (l=0; l<i; l++) {
		travel[trvs] = newloc * 1000L + tk[l];
		trvs++;
		if (trvs >= trvsiz) bug(3);
	}
	travel[trvs-1]= -travel[trvs-1];
	goto l1030;
	/*
	   Here we read in the vocabulary.  ktab[n] is the word
	   number, atab[n] is the corresponding word.  The -1 at the
	   end of section 4 is left in ktab as an end-marker.
	*/
l1040:
	for (tabndx = 1; tabndx <= tabsiz; tabndx++) {
		char str[6];
		fscanf (caves, "%d\t%s", &ktab[tabndx], str);
#ifdef	INITBUG
		fprintf(stderr, "Sect=%d; k=%d; str=%s\n", sect, ktab[tabndx], str);
#endif	INITBUG
		while ((chr = getc(caves)) != '\n')
			;
		cpy (&atab[tabndx][0], str);
		if (ktab[tabndx] == -1) goto l1002;
	}
	bug(4);
	/*
	   Read in the initial locations for each object.  Also the
	   immovability info.  plac contains initial locations of
	   objects.  fixd is -1 for immovable objects (including the
	   snake), or = second loc for two-placed objects.
	*/
l1050:
	while (fscanf (caves, "%d\t%d\t%d", &obj, &j, &k), obj != -1) {
#ifdef	INITBUG
		fprintf(stderr, "Sect=%d; obj=%d; plac=%d; fixd=%d\n", sect, obj, j, k);
#endif	INITBUG
		plac[obj]=j;
		fixd[obj]=k;
	}
	goto l1002;

	/* Read default message numbers for action verbs, store in actspk. */
l1060:
	while (fscanf (caves, "%d\t%d", &verb, &j), verb != -1) {
#ifdef	INITBUG
		fprintf(stderr, "Sect=%d; verb=%d; j=%d\n", sect, verb, j);
#endif	INITBUG
		actspk[verb]=j;
	}
	goto l1002;

	/* Read info about available liquids and other conditions,
	   store in cond.  */
l1070:
	while (fscanf (caves, "%d", &k), k != -1) {
#ifdef	INITBUG
		fprintf(stderr, "Sect=%d; cond=%d\n", sect, k);
#endif	INITBUG
		i = 0;
		while ((chr = getc(caves)) != '\n') {
			ungetc(chr, caves);
			fscanf (caves, "%d", &i);
			if (bitset (i, k)) {
				int z;
				for (z=0; z<10; z++)
					printf ("cond[%d]=%d\n", z, cond[z]);
				fatal ("Duplicate bit\n");
			}
			cond[i] |= (1 << k);
		}
	}
	goto l1002;

	/* Read data for hints. */
l1080:  
	hntmax=0;

	while (fscanf (caves, "%d\t%d\t%d\t%d\t%d",
		&k, &tk[1], &tk[2], &tk[3], &tk[4]), k != -1) {
#ifdef	INITBUG
		fprintf(stderr, "Sect=%d; k=%d; tk=[%d,%d,%d,%d]\n",
			sect, k, tk[1], tk[2], tk[3], tk[4]);
#endif	INITBUG
		if (k != 0) {
			if (k < 0 || k > hntsiz) bug(7);
			for (i=1; i<=4; i++)
				hints[k][i]=tk[i];
			if (k > hntmax)
				hntmax = k;
		}
	}
	goto l1002;

	/*
	   Finish constructing internal data format
	   Having read in the database, certain things are now
	   constructed.  Props are set to zero.  We finish setting
	   up cond by checking for forced-motion travel entries.
	   The plac and fixd arrays are used to set up atloc[n] as
	   the first object at location n, and rlink[obj] as the next
	   object at the same location as obj.  (obj>100 indicates
	   that fixed[obj-100]=loc; rlink[obj] is still the correct
	   link to use.) abb is zeroed; it controls whether the
	   abbreviated description is printed.  Counts mod 5 unless
	   "look" is used.
	*/
l1100:

	printf("Linking...  ");
	fflush(stdout);

	for (i=1; i<=100; i++) {
		place[i]=0;
		prop[i]=0;
		rlink[i]=0;
		rlink[i+100]=0;
	}
	for (i=1; i<=locsiz; i++) {
		abb[i]=0;
		if (ltext[i] != 0 && key[i] != 0) {
			k=key[i];
			if (longabs(travel[k]) % 1000 == 1)
				cond[i]=2;
		}
		atloc[i]=0;
	}
	/*
	   Set up the atloc and rlink arrays as described above.
	   We'll use the drop subroutine, which prefaces new objects
	   on the lists.  Since we want things in the other order,
	   we'll run the loop backwards.  If the object is in two
	   locs, we drop it twice.  This also sets up "place" and
	   "fixed" as copies of "plac" and "fixd".  Also, since
	   two-placed objects are typically best described last,
	   we'll drop them first.
	*/
	for (i=1; i<=100; i++) {
		k=101-i;
		if (fixd[k] > 0) {
			drop (k+100,fixd[k]);
			drop (k,plac[k]);
		}
	}
	for (i=1; i<=100; i++) {
		k=101-i;
		fixed[k]=fixd[k];
		if (plac[k] != 0 && fixd[k] <= 0) drop(k,plac[k]);
	}
	/*
	   Treasures, as noted earlier, are objects 50 through
	   maxtrs (currently 79).  Their props are initially -1, and
	   are set to 0 the first time they are described.  Tally
	   keeps track of how many are not yet found, so we know
	   when to close the cave.  Tally2 counts how many can never
	   be found (e.g.  if lost bird or bridge).
	*/
	maxtrs=79;
	tally=0;
	tally2=0;
	for (i=50; i<=maxtrs; i++) {
		if (ptext[i] != 0) prop[i]= -1;
		tally -= prop[i];
	}
	/*
	   Clear the hint stuff.  hintlc[i] is how long he's been at
	   loc with cond bit i.  hinted(i) is true iff hint i has
	   been used.
	*/
	for (i = 1; i<=hntmax; i++)
		hinted[i] = hintlc[i] = 0;
	/* Define some handy mnemonics.  these correspond to object numbers. */
	keys=vocab("keys",1);
	lamp=vocab("lamp",1);
	grate=vocab("grate",1);
	cage=vocab("cage",1);
	rod=vocab("rod",1);
	rod2=rod+1;
	steps=vocab("steps",1);
	bird=vocab("bird",1);
	door=vocab("door",1);
	pillow=vocab("pillo",1);
	snake=vocab("snake",1);
	fissur=vocab("fissu",1);
	tablet=vocab("table",1);
	clam=vocab("clam",1);
	oyster=vocab("oyste",1);
	magzin=vocab("magaz",1);
	dwarf=vocab("dwarf",1);
	knife=vocab("knife",1);
	food=vocab("food",1);
	bottle=vocab("bottl",1);
	water=vocab("water",1);
	oil=vocab("oil",1);
	plant=vocab("plant",1);
	plant2=plant+1;
	axe=vocab("axe",1);
	mirror=vocab("mirro",1);
	dragon=vocab("drago",1);
	chasm=vocab("chasm",1);
	troll=vocab("troll",1);
	troll2=troll+1;
	bear=vocab("bear",1);
	messag=vocab("messa",1);
	vend=vocab("vendi",1);
	batter=vocab("batte",1);
	/* Objects from 50 through whatever are treasures.  here are a few. */
	nugget=vocab("gold",1);
	coins=vocab("coins",1);
	chest=vocab("chest",1);
	eggs=vocab("eggs",1);
	tridnt=vocab("tride",1);
	vase=vocab("vase",1);
	emrald=vocab("emera",1);
	pyram=vocab("pyram",1);
	pearl=vocab("pearl",1);
	rug=vocab("rug",1);
	chain=vocab("chain",1);
	/* These are motion-verb numbers. */
	back=vocab("back",0);
	look=vocab("look",0);
	cave=vocab("cave",0);
	nullx=vocab("null",0);
	entrnc=vocab("entra",0);
	dprssn=vocab("depre",0);
	/* And some action verbs. */
	say=vocab("say",2);
	lock=vocab("lock",2);
	throw=vocab("throw",2);
	find=vocab("find",2);
	invent=vocab("inven",2);
	/*
	   Initialize the dwarves.  dloc is loc of dwarves,
	   hard-wired in.  odloc is prior loc of each dwarf,
	   initially garbage.  daltlc is alternate initial loc for
	   dwarf, in case one of them starts out on top of the
	   adventurer.  (no 2 of the 5 initial locs are adjacent.)
	   dseen is true if dwarf has seen him.  dflag controls the
	   level of activation of all this:
	        0       no dwarf stuff yet (wait until reaches hall
	                of mists)
	        1       reached hall of mists, but hasn't met first dwarf
	        2       met first dwarf, others start moving, no
	                knives thrown yet
	        3       a knife has been thrown (first set always
	                misses)
	        3+      dwarves are mad (increases their accuracy)
	   Sixth dwarf is special (the pirate).  He always starts at
	   his chest's eventual location inside the maze.  This loc
	   is saved in chloc for ref.  The dead end in the other
	   maze has its loc stored in chloc2.
	*/
	chloc=114;
	chloc2=140;
	for (i=1; i<=6; i++)
		dseen[i] = 0;
	dflag=0;
	dloc[1]=19;
	dloc[2]=27;
	dloc[3]=33;
	dloc[4]=44;
	dloc[5]=64;
	dloc[6]=chloc;
	daltlc=18;
	/*
	   Other random flags and counters, as follows:
	        turns   tallies how many commands he's given
	                (ignores yes/no)
	        limit   lifetime of lamp (not set here)
	        iwest   how many times he's said "west" instead of
	                "w"
	        knfloc  0 if no knife here, loc if knife here, -1
	                after caveat
	        detail  how often we've said "not allowed to give
	                more detail"
	        abbnum  how often we should print non-abbreviated
	                descriptions
	        maxdie  number of reincarnation messages available
	                (up to 5)
	        numdie  number of times killed so far
	        holdng  number of objects being carried
	        dkill   number of dwarves killed (unused in scoring,
	                needed for msg)
	        foobar  current progress in saying "fee fie foe
	                foo".
	        bonus   used to determine amount of bonus if he
	                reaches closing
	        clock1  number of turns from finding last treasure
	                till closing
	        clock2  number of turns from first warning till
	                blinding flash
	        logicals were explained earlier
	*/
	turns=0;
	lmwarn=0;
	iwest=0;
	knfloc=0;
	detail=0;
	abbnum=5;
	for (i=0; i<=4; i++)
		if (rtext[2*i+81] != 0) maxdie=i+1;
	numdie=0;
	/*holdng=0; */
	dkill=0;
	foobar=0;
	bonus=0;
	clock1=30;
	clock2=50;
	closng=0;
	panic=0;
	closed=0;
	gaveup=0;
	scorng=0;
	/* Report on amount of arrays actually used,
	   to permit reductions. */
	for (kk = locsiz; kk > 0; kk--) {
		if (ltext[kk] != 0)
			break;
	}
	obj=0;
	for (k = 1; k <= 100; k++) {
		if (ptext[k] != 0) obj++;
	}
	for (k = 1; k <= tabndx; k++) {
		if (ktab[k]/1000 == 2) verb=ktab[k]-2000;
	}
	for (j = rtxsiz; j > 0; j--) {
		if (rtext[j] != 0)
			break;
	}
	k=100;
#ifdef	INITBUG
	printf ("%d of %d messages; ", linuse, linsiz);
	printf ("%d of %d travel options;\n", trvs, trvsiz);
	printf ("%d of %d vocabulary words; ", tabndx, tabsiz);
	printf ("%d of %d locations;\n", kk, locsiz);
	printf ("%d of %d objects; ", obj, k);
	printf ("%d of %d action verbs;\n", verb, vrbsiz);
	printf ("%d of %d rtext messages; ", j, rtxsiz);
	printf ("%d of %d class messages;\n", clsses, clssiz);
	printf ("%d of %d hints.\n", hntmax, hntsiz);
#endif	INITBUG
	/* Finally, since we're clearly setting things up for the first time */
	printf ("Done!\n");
	tvec = time((long *) 0);
	srand ((int) (tvec % 32768L));
	if ((suspfd = fopen (suspfile, SUSPREAD)) != NULL) {
		/* check if we suspended in this release */
		srel = getw (suspfd);
		slev = getw (suspfd);
		if (srel != 1 || slev != 20) {
			printf("I deleted a suspend file from version %d.%d\n",
				srel, slev);
			unlink (suspfile);
			hinted[3] = yes (65, 1, 0);
			loc = newloc = 1;
			limit = hinted[3]? 1000: 330;
		}
		else
		    {
			/* When did we suspend? */
			xtime = getl (suspfd);
#ifndef NOTIME
			tvec = time((long *) 0);
			if (tvec - xtime < 1800) {
				printf ("You cannot restart a suspended game");
				printf ("for at least half an hour.\n");
				exit (1);
			}
#endif

			/* Delete the suspend file */
			if (unlink (suspfile) == -1)
				fatal ("can't unlink suspend file");

			fread (&suspbeg, sizeof suspbeg, &suspend - &suspbeg, suspfd);
			fclose (suspfd);
			printf ("Restarting a suspended game...\n");
			newloc = loc;
		}
	}
	else
	    {
		hinted[3]=yes(65,1,0);
		loc=newloc=1;
		limit = hinted[3]? 1000: 330;
	}
	/*  Can't leave cave once it's closing (except by main office). */
l2:     
	if (newloc < 9 && newloc != 0 &&  closng) {
		rspeak(130);
		newloc=loc;
		if (!panic) clock2=15;
		panic=1;
	}
	/*
		   See if a dwarf has seen him and has come from where he
		   wants to go.  If so, the dwarf's blocking his way.  If
		   coming from place forbidden to pirate (dwarves rooted in
		   place) let him get out (and attacked).
		*/
	if (newloc != loc && !forced(loc) && !bitset(loc,3)) {
		for (i = 1; i <= 5; i++) {
			if (!(odloc[i] != newloc ||  !dseen[i])) {
				newloc=loc;
				rspeak(2);
				break;
			}
		}
	}
	loc=newloc;
	/*
		   Dwarf stuff.  See earlier comments for description of
		   variables.  Remember sixth dwarf is pirate and is thus
		   very different except for motion rules.
		   First off, don't let the dwarves follow him into a pit or
		   a wall.  Activate the whole mess the first time he gets
		   as far as the hall of mists (loc 15).  If newloc is
		   forbidden to pirate (in particular, if it's beyond the
		   troll bridge), bypass dwarf stuff.  That way pirate can't
		   steal return toll, and dwarves can't meet the bear.  Also
		   means dwarves won't follow him into dead end in maze, but
		   c'est la vie.  They'll wait for him outside the dead end.
		*/
	if (loc == 0 || forced(loc) || bitset((int)newloc,3)) goto l2000;
	if (dflag == 0) {
		if (loc >= 15) dflag=1;
		goto l2000;
	}
	/*
		   When we encounter the first dwarf, we kill 0, 1, or 2 of
		   the 5 dwarves.  If any of the survivors is at loc,
		   replace him with the alternate.
		*/
	if (dflag == 1) {
		if (loc < 15 || pct(95)) goto l2000;
		dflag=2;
		for (i = 1; i <= 2; i++) {
			j=1+ran(5);
			if (pct(50)) dloc[j]=0;
		}
		for (i = 1; i <= 5; i++) {
			if (dloc[i] == loc) dloc[i]=daltlc;
			odloc[i]=dloc[i];
		}
		rspeak(3);
		drop(axe,loc);
		goto l2000;
	}
	/*
		   Things are in full swing.  Move each dwarf at random,
		   except if he's seen us he sticks with us.  Dwarves never
		   go to locs <15.  If wandering at random, they don't back
		   up unless there's no alternative.  If they don't have to
		   move, they attack.  And, of course, dead dwarves don't do
		   much of anything.
		*/
	dtotal=0;
	attack=0;
	stick=0;
	for (i = 1; i <= 6; i++) {
		if (dloc[i] == 0) goto l6030;
		j=1;
		kk=dloc[i];
		kk=key[kk];
		if (kk != 0) {
			do 
			    {
				newloc=longabs(travel[kk])/1000 % 1000;
				if (newloc <= 300 && newloc >=15 && newloc != odloc[i]
				    && !(j > 1 && newloc == tk[j-1]) && j < 20
				    && newloc != dloc[i] && !forced((int)newloc)
				    && !(i == 6 && bitset((int)newloc,3))
				    && longabs(travel[kk] / 1000000) != 100) {
					tk[j]=newloc;
					j++;
				}
				kk++;
			} 
			while (travel[kk-1] >= 0);
		}
		tk[j]=odloc[i];
		if (j >= 2) j--;
		j=1+ran(j);
		odloc[i]=dloc[i];
		dloc[i]=tk[j];
		dseen[i]=(dseen[i] && loc >= 15)
			|| (dloc[i] == loc || odloc[i] == loc);
		if (!dseen[i]) goto l6030;
		dloc[i]=loc;
		if (i != 6) goto l6027;
		/*
				The pirate's spotted him.  He leaves him alone once we've
				found chest.  k counts if a treasure is here.  If not,
				and tally=tally2 plus one for an unseen chest, let the
				pirate be spotted.
				*/
		if (loc == chloc || prop[chest] >= 0) goto l6030;
		k=0;
		for (j = 50; j <= maxtrs; j++) {
			/* Pirate won't take pyramid from plover room or dark room (too easy!).  */
			if (j != pyram || !(loc == plac[pyram]
			    || loc == plac[emrald])) {
				if (toting(j)) goto l6022;
			}
			if (here(j)) k=1;
		}
		if (tally == tally2+1 && k == 0 && place[chest] == 0
		    && here(lamp) && prop[lamp] == 1) goto l6025;
		if (odloc[6] != dloc[6] && pct(20)) rspeak(127);
		goto l6030;
l6022:      
		rspeak(128);
		/* Don't steal chest back from troll! */
		if (place[messag] == 0) move(chest,chloc);
		move(messag,chloc2);
		for (j = 50; j <= maxtrs; j++) {
			if (j == pyram && (loc == plac[pyram]
			    || loc == plac[emrald])) goto l6023;
			if (at(j) && fixed[j] == 0) carry(j,loc);
			if (toting(j)) drop(j,chloc);
l6023:          
			; 
		}
l6024:      
		dloc[6]=chloc;
		odloc[6]=chloc;
		dseen[6]=0;
		goto l6030;
l6025:      
		rspeak(186);
		move(chest,chloc);
		move(messag,chloc2);
		goto l6024;
		/* This threatening little dwarf is in the room with him! */
l6027:     
		dtotal++;
		if (odloc[i] != dloc[i]) goto l6030;
		attack++;
		if (knfloc >= 0) knfloc=loc;
		if (ran(1000) < 95*(dflag-2))stick++;
l6030:      
		; 
	}
	/* Now we know what's happening.  let's tell the poor sucker about it.  */
	if (dtotal == 0) goto l2000;
	if (dtotal != 1) {
		printf ("There are %d threatening little dwarves", dtotal);
		printf (" in the room with you.\n");
	} else
		rspeak(4);
	if (attack == 0) goto l2000;
	if (dflag == 2) dflag=3;
	/*
		   Dwarves get VERY mad!
		*/
	if (attack == 1) {
		rspeak(5);
		k=52;
	}
	else {
		printf ("%d of them throw knives at you!\n", attack);
		k=6;
	}
	if (stick <= 1) {
		rspeak(k+stick);
		if (stick == 0) goto l2000;
	}
	else
	    printf ("%d of them get you!\n", stick);
	oldlc2=loc;
	goto l99;
	/* Describe the current location and (maybe) get next command. */
	/* Print text for current loc. */
l2000:  
	if (loc == 0) goto l99;
	kk=stext[loc];
	if (abb[loc] % abbnum == 0 || kk == 0) kk=ltext[loc];
	if (forced(loc) ||  ! dark()) goto l2001;
	if (wzdark && pct(35)) goto l90;
	kk=rtext[16];
l2001:  
	if (toting(bear)) rspeak(141);
	speak(kk);
	k=1;
	if (forced(loc)) goto l8;
	if (loc == 33 && pct(25) &&  ! closng) rspeak(8);
	/*
		   Print out descriptions of objects at this location.  If
		   not closing and property value is negative, tally off
		   another treasure.  Rug is special case; once seen, its
		   prop is 1 (dragon on it) till dragon is killed.
		   Similarly for chain; prop is initially 1 (locked to
		   bear).  These hacks are because prop=0 is needed to get
		   full score.
		*/
	if (dark()) goto l2012;
	abb[loc]++;
	i=atloc[loc];
	for(;;) {
		if (i == 0) goto l2012;
		obj=i;
		if (obj > 100) obj -= 100;
		if (obj == steps && toting(nugget)) goto l2008;
		if (prop[obj] < 0) {
			if (closed) goto l2008;
			prop[obj]=0;
			if (obj == rug || obj == chain) prop[obj]=1;
			tally--;
			/* If remaining treasures too elusive, zap his lamp. */
			if (tally == tally2 && tally != 0)
				limit = limit > 35? 35: limit;
		}
		kk=prop[obj];
		if (obj == steps && loc == fixed[steps]) kk=1;
		pspeak(obj,kk);
l2008:  
		i=rlink[i];
	}
l2009:  
	k=54;
l2010:  
	spk=k;
l2011:  
	rspeak(spk);
l2012:  
	verb=0;
	obj=0;
	/*
		   Check if this loc is eligible for any hints.  If been
		   here long enough, branch to help section (on later page).
		   Hints all come back here eventually to finish the loop.
		   Ignore "hints" < 4 (special stuff, see database notes).
		*/
l2600:
	for (hint = 4; hint <= hntmax; hint++) {
		if (! (hinted[hint])) {
			if (!bitset(loc,hint)) hintlc[hint]= -1;
			hintlc[hint]++;
			if (hintlc[hint] >= hints[hint][1]) goto l40000;
		}
	}
	/*
		   Kick the random number generator just to add variety to
		   the chase.  Also, if closing time, check for any objects
		   being toted with prop < 0 and set the prop to -1-prop.
		   This way objects won't be described until they've been
		   picked up and put down separate from their respective
		   piles.  Don't tick clock1 unless well into cave (and not
		   at Y2).
		*/
l2602:  
	if (!closed) goto l2605;
	if (prop[oyster] < 0 && toting(oyster)
	    ) pspeak(oyster,1);
	for (i = 1; i <= 100; i++) {
		if (toting(i) && prop[i] < 0) prop[i]= -1-prop[i];
	}
l2605:  
	wzdark=dark();
	if (knfloc > 0 && knfloc != loc) knfloc=0;
	i=ran(1);
	getin(wd1,wd1x,wd2,wd2x);
	/*
		   Every input, check "foobar" flag.  If zero, nothing's
		   going on.  If pos, make neg.  If neg, he skipped a word,
		   so make it zero.
		*/
l2608:
	foobar = foobar < 0? 0: -foobar;
	turns++;
	if (verb == say && !blankp(wd2)) verb=0;
	if (verb == say) goto l4090;
	if (tally == 0 && loc >= 15 && loc != 33) clock1--;
	if (clock1 == 0) goto l10000;
	if (clock1 < 0) clock2--;
	if (clock2 == 0) goto l11000;
	if (prop[lamp] == 1) limit--;
	if (limit <= 30 && here(batter) && prop[batter] == 0
	    && here(lamp)) goto l12000;
	if (limit == 0) goto l12400;
	if (limit < 0 && loc <= 8) goto l12600;
	if (limit <= 30) goto l12200;
l19999: 
	k=43;
	if (liqloc(loc) == water) k=70;
	if (eqp (wd1, "enter") && (eqp (wd2, "strea") || eqp (wd2, "water")))
		goto l2010;
	if (eqp (wd1, "enter") && !blankp(wd2)) goto l2800;
	if (!eqp (wd1, "water") && !eqp (wd1, "oil")
	    || (!eqp (wd2, "plant") && !eqp (wd2, "door "))) goto l2610;
	if (at(vocab(wd2,1))) cpy (wd2, "pour ");
l2610:  
	if (!eqp(wd1, "west ")) goto l2630;
	iwest++;
	if (iwest == 10) rspeak(17);
l2630:  
	i=vocab(wd1,-1);
	if (i == -1) goto l3000;
	k=i % 1000;
	kq=i/1000+1;
	switch (kq - 1) {
	case 0: 
		goto l8;
	case 1: 
		goto l5000;
	case 2: 
		goto l4000;
	case 3: 
		goto l2010;
	}
	bug(22);
	/* Get second word for analysis. */
l2800:  
	cpy (wd1, wd2);
	cpy (wd1x, wd2x);
	cpy (wd2, "     ");
	goto l2610;
	/* Gee, I don't understand. */
l3000:  
	spk=60;
	if (pct(20)) spk=61;
	if (pct(20)) spk=13;
	rspeak(spk);
	goto l2600;
	/*
		   Analyse a verb.  remember what it was, go back for object
		   if second word unless verb is "say", which snarfs
		   arbitrary second word.
		*/
l4000:  
	verb=k;
	spk=actspk[verb];
	if (!blankp(wd2) && verb != say) goto l2800;
	if (verb == say)
		if (blankp (wd2)) goto l4080;
		else goto l4090;
	if (obj != 0) goto l4090;
	/* Analyse an intransitive verb (ie, no object given yet). */
l4080:  
	switch (verb) {
	case 1: 
		goto l8010;       /* take */
	case 2: 
		goto l8000;       /* drop */
	case 3: 
		goto l8000;       /* say */
	case 4: 
		goto l8040;       /* open */
	case 5: 
		goto l2009;       /* noth */
	case 6: 
		goto l8040;       /* lock */
	case 7: 
		goto l9070;       /* on */
	case 8: 
		goto l9080;       /* off */
	case 9: 
		goto l8000;       /* wave */
	case 10: 
		goto l8000;       /* calm */
	case 11: 
		goto l2011;       /* walk */
	case 12: 
		goto l9120;       /* kill */
	case 13: 
		goto l9130;       /* pour */
	case 14: 
		goto l8140;       /* eat */
	case 15: 
		goto l9150;       /* drnk */
	case 16: 
		goto l8000;       /* rub */
	case 17: 
		goto l8000;       /* toss */
	case 18: 
		goto l8180;       /* quit */
	case 19: 
		goto l8000;       /* find */
	case 20: 
		goto l8200;       /* invn */
	case 21: 
		goto l8000;       /* feed */
	case 22: 
		goto l9220;       /* fill */
	case 23: 
		goto l9230;       /* blst */
	case 24: 
		goto l8240;       /* scor */
	case 25: 
		goto l8250;       /* foo */
	case 26: 
		goto l8260;       /* brf */
	case 27: 
		goto l8270;       /* read */
	case 28: 
		goto l8000;       /* brek */
	case 29: 
		goto l8000;       /* wake */
	case 30: 
		goto l8300;       /* susp */
	case 31: 
		goto l8310;       /* hour */
	case 32: 
		goto setlog;      /* log  */
	}
	bug(23);
	/* Analyse a transitive verb. */
l4090:  
	switch (verb) {
	case 1: 
		goto l9010;       /* take */
	case 2: 
		goto l9020;       /* drop */
	case 3: 
		goto l9030;       /* say */
	case 4: 
		goto l9040;       /* open */
	case 5: 
		goto l2009;       /* noth */
	case 6: 
		goto l9040;       /* lock */
	case 7: 
		goto l9070;       /* on */
	case 8: 
		goto l9080;       /* off */
	case 9: 
		goto l9090;       /* wave */
	case 10: 
		goto l2011;       /* calm */
	case 11: 
		goto l2011;       /* walk */
	case 12: 
		goto l9120;       /* kill */
	case 13: 
		goto l9130;       /* pour */
	case 14: 
		goto l9140;       /* eat */
	case 15: 
		goto l9150;       /* drnk */
	case 16: 
		goto l9160;       /* rub */
	case 17: 
		goto l9170;       /* toss */
	case 18: 
		goto l2011;       /* quit */
	case 19: 
		goto l9190;       /* find */
	case 20: 
		goto l9190;       /* invn */
	case 21: 
		goto l9210;       /* feed */
	case 22: 
		goto l9220;       /* fill */
	case 23: 
		goto l9230;       /* blst */
	case 24: 
		goto l2011;       /* scor */
	case 25: 
		goto l2011;       /* foo */
	case 26: 
		goto l2011;       /* brf */
	case 27: 
		goto l9270;       /* read */
	case 28: 
		goto l9280;       /* brek */
	case 29: 
		goto l9290;       /* wake */
	case 30: 
		goto l2011;       /* susp */
	case 31: 
		goto l2011;       /* hour */
	case 32: 
		goto l2011;       /* log  */
	}
	bug(24);
	/*
		   Analyze an object word.  See if the thing is here,
		   whether we've got a verb yet, and so on.  Object must be
		   here unless verb is "find" or "invent(ory)" (and no new
		   verb yet to be analyzed).  Water and oil are also funny,
		   since they are never actually dropped at any location,
		   but might be here inside the bottle or as a feature of
		   the location.
		*/
l5000:  
	obj=k;
	if (fixed[k] != loc &&  ! here(k)) goto l5100;
l5010:  
	if (!blankp(wd2)) goto l2800;
	if (verb != 0) goto l4090;
	a5toa1(wd1,wd1x,tkword);
	printf ("What do you want to do with the %s?\n", tkword);
	goto l2600;
l5100:  
	if (k != grate) goto l5110;
	if (loc == 1 || loc == 4 || loc == 7) k=dprssn;
	if (loc > 9 && loc < 15) k=entrnc;
	if (k != grate) goto l8;
l5110:  
	if (k != dwarf) goto l5120;
	for (i = 1; i <= 5; i++) {
		if (dloc[i] == loc && dflag >= 2) goto l5010;
	}
l5120:  
	if ((liq() == k && here(bottle))
	    || k == liqloc(loc)) goto l5010;
	if (obj != plant ||  ! at(plant2) || prop[plant2] == 0
	    ) goto l5130;
	obj=plant2;
	goto l5010;
l5130:  
	if (obj == knife && knfloc == loc) {
		knfloc= -1;
		spk=116;
		goto l2011;
	}
	if (obj != rod ||  !here(rod2)) goto l5190;
	obj=rod2;
	goto l5010;
l5190:  
	if ((verb == find || verb == invent) && blankp (wd2)
	    ) goto l5010;
	a5toa1(wd1,wd1x,tkword);
	printf ("I see no %s here!\n", tkword);
	goto l2012;
	/*
		   Figure out the new location
		   Given the current location in "loc", and a motion verb
		   number in "k", put the new location in "newloc".  The
		   current loc is saved in "oldloc" in case he wants to
		   retreat.  The current oldloc is saved in oldlc2, in case
		   he dies.  (if he does, newloc will be limbo, and oldloc
		   will be what killed him, so we need oldlc2, which is the
		   last place he was safe.)
		*/
l8:     
	kk=key[loc];
	newloc=loc;
	if (kk == 0) bug(26);
	if (k == nullx) goto l2;
	if (k == back) goto l20;
	if (k == look) goto l30;
	if (k == cave) goto l40;
	oldlc2=oldloc;
	oldloc=loc;
l9:     
	ll=longabs(travel[kk]);
	if (ll % 1000 == 1 || ll % 1000 == k) goto l10;
	if (travel[kk] < 0) goto l50;
	kk++;
	goto l9;
l10:    
	ll/=1000;
l11:    
	newloc=ll/1000;
	k=newloc % 100;
	if (newloc <= 300) goto l13;
	if (prop[k] != newloc/100-3) goto l16;
l12:    
	if (travel[kk] < 0) bug(25);
	kk++;
	newloc=longabs(travel[kk])/1000;
	if (newloc == ll) goto l12;
	ll=newloc;
	goto l11;
l13:    
	if (newloc <= 100) goto l14;
	if (toting(k) || (newloc > 200 && at(k))) goto l16;
	goto l12;
l14:    
	if (newloc != 0 &&  !pct((int)newloc)) goto l12;
l16:    
	newloc=ll % 1000;
	if (newloc <= 300) goto l2;
	if (newloc <= 500) goto l30000;
	rspeak((int)newloc-500);
	newloc=loc;
	goto l2;
	/*
		   Special motions come here.  Labelling convention:
		   statement numbers nnnxx (xx=00-99) are used for special
		   case number nnn (nnn=301-500).
		*/
l30000: 
	newloc=newloc-300;
	switch ((int) newloc-1) {
	case 0: 
		goto l30100;
	case 1: 
		goto l30200;
	case 2: 
		goto l30300;
	}
	bug(20);
	/*
		   Travel 301.  Plover-alcove passage.  can carry only
		   emerald.  Note:  travel table must include "useless"
		   entries going through passage, which can never be used
		   for actual motion, but can be spotted by "go back".
		*/
l30100: 
	newloc=99+100-loc;
	if (holdng == 0 || (holdng == 1 && toting(emrald))) goto l2;
	newloc=loc;
	rspeak(117);
	goto l2;
	/*
		   Travel 302.  Plover transport.  Drop the emerald (only
		   use special travel if toting it), so he's forced to use
		   the plover-passage to get it out.  Having dropped it, go
		   back and pretend he wasn't carrying it after all.
		*/
l30200: 
	drop(emrald,loc);
	goto l12;
	/*
		   Travel 303.  Troll bridge.  Must be done only as special
		   motion so that dwarves won't wander across and encounter
		   the bear.  (They won't follow the player there because
		   that region is forbidden to the pirate.) If
		   prop(troll)=1, he's crossed since paying, so step out and
		   block him.  (Standard travel entries check for
		   prop(troll)=0.) Special stuff for bear.
		*/
l30300: 
	if (prop[troll] != 1) goto l30310;
	pspeak(troll,1);
	prop[troll]=0;
	move(troll2,0);
	move(troll2+100,0);
	move(troll,plac[troll]);
	move(troll+100,fixd[troll]);
	juggle(chasm);
	newloc=loc;
	goto l2;
l30310: 
	newloc=plac[troll]+fixd[troll]-loc;
	if (prop[troll] == 0) prop[troll]=1;
	if (!toting(bear)) goto l2;
	rspeak(162);
	prop[chasm]=1;
	prop[troll]=2;
	drop(bear,(int)newloc);
	fixed[bear]= -1;
	prop[bear]=3;
	if (prop[spices] < 0)tally2++;
	oldlc2=newloc;
	goto l99;
	/* End of specials. */
	/*
		   Handle "go back".  Look for verb which goes from loc to
		   oldloc, or to oldlc2 if oldloc has forced-motion.  k2
		   saves entry -> forced loc -> previous loc.
		*/
l20:    
	k=oldloc;
	if (forced(k)) k=oldlc2;
	oldlc2=oldloc;
	oldloc=loc;
	k2=0;
	if (k != loc) goto l21;
	rspeak(91);
	goto l2;
l21:    
	ll=(longabs(travel[kk])/1000) % 1000;
	if (ll == k) goto l25;
	if (ll > 300) goto l22;
	j=key[ll];
	if (forced((int)ll) && (longabs(travel[j])/1000) % 1000 == k
	    ) k2=kk;
l22:    
	if (travel[kk] < 0) goto l23;
	kk++;
	goto l21;
l23:    
	kk=k2;
	if (kk != 0) goto l25;
	rspeak(140);
	goto l2;
l25:    
	k=longabs(travel[kk]) % 1000;
	kk=key[loc];
	goto l9;
	/*
		   Look.  Can't give more detail.  Pretend it wasn't dark
		   (though it may "now" be dark) so he won't fall into a pit
		   while staring into the gloom.
		*/
l30:    
	if (detail < 3) rspeak(15);
	detail++;
	wzdark=0;
	abb[loc]=0;
	goto l2;
	/* Cave.  Different messages depending on whether above ground. */
l40:    
	if (loc < 8) rspeak(57);
	if (loc >= 8) rspeak(58);
	goto l2;
	/* Non-applicable motion.  Various messages depending on word given.  */
l50:    
	spk=12;
	if (k >= 43 && k <= 50) spk=9;
	if (k == 29 || k == 30) spk=9;
	if (k == 7 || k == 36 || k == 37) spk=10;
	if (k == 11 || k == 19) spk=11;
	if (verb == find || verb == invent) spk=59;
	if (k == 62 || k == 65) spk=42;
	if (k == 17) spk=80;
	rspeak(spk);
	goto l2;
	/*
		   "You're dead, Jim."
		   If the current loc is zero, it means the clown got
		   himself killed.  We'll allow this maxdie times.  maxdie
		   is automatically set based on the number of snide
		   messages available.  Each death results in a message (81,
		   83, etc.) which offers reincarnation; if accepted, this
		   results in message 82, 84, etc.  The last time, if he
		   wants another chance, he gets a snide remark as we exit.
		   When reincarnated, all objects being carried get dropped
		   at oldlc2 (presumably the last place prior to being
		   killed) without change of props.  The loop runs backwards
		   to assure that the bird is dropped before the cage.
		   (This kluge could be changed once we're sure all
		   references to bird and cage are done by keywords.) The
		   lamp is a special case (it wouldn't do to leave it in the
		   cave).  It is turned off and left outside the building
		   (only if he was carrying it, of course).  He himself is
		   left inside the building (and heaven help him if he tries
		   to xyzzy back into the cave without the lamp!).  oldloc
		   is zapped so he can't just "retreat".
		   The easiest way to get killed is to fall into a pit in
		   pitch darkness.
		*/
l90:    
	rspeak(23);
	oldlc2=loc;
	/* Okay, he's dead.  Let's get on with it. */
l99:    
	if (closng) goto l95;
	yea=yes(81+numdie*2,82+numdie*2,54);
	numdie++;
	if (numdie == maxdie ||  !yea) goto l20000;
	place[water]=0;
	place[oil]=0;
	if (toting(lamp)) prop[lamp]=0;
	for (j = 1; j <= 100; j++) {
		i=101-j;
		if (!toting(i)) goto l98;
		k=oldlc2;
		if (i == lamp) k=1;
		drop(i,k);
l98:        
		;
	}
	loc=3;
	oldloc=loc;
	goto l2000;
	/* He died during closing time.  No resurrection.  tally up
		   a death and exit.  */
l95:    
	rspeak(131);
	numdie++;
	goto l20000;
	/*
		   Routines for performing the various action verbs
		   Statement numbers in this section are 8000 for
		   intransitive verbs, 9000 for transitive, plus ten times
		   the verb number.  Many intransitive verbs use the
		   transitive code, and some verbs use code for other verbs,
		   as noted below.
		   Random intransitive verbs come here.  Clear obj just in
		   case (see "attack").
		*/
l8000:  
	a5toa1(wd1,wd1x,tkword);
	printf ("%s what?\n", tkword);
	obj=0;
	goto l2600;
	/* Carry, no object given yet.  OK if only one object present. */
l8010:  
	if (atloc[loc] == 0 || rlink[atloc[loc]] != 0) goto l8000;
	for (i = 1; i <= 5; i++) {
		if (dloc[i] == loc && dflag >= 2) goto l8000;
	}
	obj=atloc[loc];
	/*
		   Carry an object.  Special cases for bird and cage (if
		   bird in cage, can't take one without the other.  Liquids
		   also special, since they depend on status of bottle.
		   Also various side effects, etc.
		*/
l9010:  
	if (toting(obj)) goto l2011;
	spk=25;
	if (obj == plant && prop[plant] <= 0) spk=115;
	if (obj == bear && prop[bear] == 1) spk=169;
	if (obj == chain && prop[bear] != 0) spk=170;
	if (fixed[obj] != 0) goto l2011;
	if (obj != water && obj != oil) goto l9017;
	if (here(bottle) && liq() == obj) goto l9018;
	obj=bottle;
	if (toting(bottle) && prop[bottle] == 1) goto l9220;
	if (prop[bottle] != 1) spk=105;
	if (!toting(bottle)) spk=104;
	goto l2011;
l9018:  
	obj=bottle;
l9017:  
	if (holdng < 7) goto l9016;
	rspeak(92);
	goto l2012;
l9016:  
	if (obj != bird) goto l9014;
	if (prop[bird] != 0) goto l9014;
	if (!toting(rod)) goto l9013;
	rspeak(26);
	goto l2012;
l9013:  
	if (toting(cage)) goto l9015;
	rspeak(27);
	goto l2012;
l9015:  
	prop[bird]=1;
l9014:  
	if ((obj == bird || obj == cage) && prop[bird] != 0
	    ) carry(bird+cage-obj,loc);
	carry(obj,loc);
	k=liq();
	if (obj == bottle && k != 0) place[k]= -1;
	goto l2009;
	/*
		   Discard object.  "Throw" also comes here for most
		   objects.  Special cases for bird (might attack snake or
		   dragon) and cage (might contain bird) and vase.  Drop
		   coins at vending machine for extra batteries.
		*/
l9020:  
	if (toting(rod2) && obj == rod && ! toting(rod)) obj=rod2;
	if (!toting(obj)) goto l2011;
	if (obj != bird ||  ! here(snake)) goto l9024;
	rspeak(30);
	if (closed) goto l19000;
	dstroy(snake);
	/* Set prop for use by travel options */
	prop[snake]=1;
l9021:  
	k=liq();
	if (k == obj) obj=bottle;
	if (obj == bottle && k != 0) place[k]=0;
	if (obj == cage && prop[bird] != 0) drop(bird,loc);
	if (obj == bird) prop[bird]=0;
	drop(obj,loc);
	goto l2012;
l9024:  
	if (obj != coins ||  ! here(vend)) goto l9025;
	dstroy(coins);
	drop(batter,loc);
	pspeak(batter,0);
	goto l2012;
l9025:  
	if (obj != bird ||  ! at(dragon) || prop[dragon] != 0
	    ) goto l9026;
	rspeak(154);
	dstroy(bird);
	prop[bird]=0;
	if (place[snake] == plac[snake])tally2++;
	goto l2012;
l9026:  
	if (obj != bear ||  !at(troll)) goto l9027;
	rspeak(163);
	move(troll,0);
	move(troll+100,0);
	move(troll2,plac[troll]);
	move(troll2+100,fixd[troll]);
	juggle(chasm);
	prop[troll]=2;
	goto l9021;
l9027:  
	if (obj == vase && loc != plac[pillow]) goto l9028;
	rspeak(54);
	goto l9021;
l9028:  
	prop[vase]=2;
	if (at(pillow)) prop[vase]=0;
	pspeak(vase,prop[vase]+1);
	if (prop[vase] != 0) fixed[vase]= -1;
	goto l9021;
	/* Say.  Echo wd2 (or wd1 if no wd2 (say what?, etc.).)
		   Magic words override.  */
l9030:  
	a5toa1(wd2,wd2x,tkword);
	if (blankp(wd2)) a5toa1(wd1,wd1x,tkword);
	else cpy(wd1,wd2);
	i=vocab(wd1,-1);
	if (i == 62 || i == 65 || i == 71 || i == 2025) goto l9035;
	printf ("Okay, \"%s\"\n", tkword);
	goto l2012;
l9035:  
	cpy(wd2, "     ");
	obj=0;
	goto l2630;
	/* Lock, unlock, no object given.  Assume various things if present.  */
l8040:  
	spk=28;
	if (here(clam)) obj=clam;
	if (here(oyster)) obj=oyster;
	if (at(door)) obj=door;
	if (at(grate)) obj=grate;
	if (obj != 0 && here(chain)) goto l8000;
	if (here(chain)) obj=chain;
	if (obj == 0) goto l2011;
	/* Lock, unlock object.  Special stuff for opening
		   clam/oyster and for chain.  */
l9040:  
	if (obj == clam || obj == oyster) goto l9046;
	if (obj == door) spk=111;
	if (obj == door && prop[door] == 1) spk=54;
	if (obj == cage) spk=32;
	if (obj == keys) spk=55;
	if (obj == grate || obj == chain) spk=31;
	if (spk != 31 ||  ! here(keys)) goto l2011;
	if (obj == chain) goto l9048;
	if (!closng) goto l9043;
	k=130;
	if (!panic) clock2=15;
	panic=1;
	goto l2010;
l9043:  
	k=34+prop[grate];
	prop[grate]=1;
	if (verb == lock) prop[grate]=0;
	k=k+2*prop[grate];
	goto l2010;
	/* Clam/oyster. */
l9046:  
	k=0;
	if (obj == oyster) k=1;
	spk=124+k;
	if (toting(obj)) spk=120+k;
	if (!toting(tridnt)) spk=122+k;
	if (verb == lock) spk=61;
	if (spk != 124) goto l2011;
	dstroy(clam);
	drop(oyster,loc);
	drop(pearl,105);
	goto l2011;
	/* Chain. */
l9048:  
	if (verb == lock) goto l9049;
	spk=171;
	if (prop[bear] == 0) spk=41;
	if (prop[chain] == 0) spk=37;
	if (spk != 171) goto l2011;
	prop[chain]=0;
	fixed[chain]=0;
	if (prop[bear] != 3) prop[bear]=2;
	fixed[bear]=2-prop[bear];
	goto l2011;
l9049:  
	spk=172;
	if (prop[chain] != 0) spk=34;
	if (loc != plac[chain]) spk=173;
	if (spk != 172) goto l2011;
	prop[chain]=2;
	if (toting(chain)) drop(chain,loc);
	fixed[chain]= -1;
	goto l2011;
	/* Light lamp */
l9070:  
	if(!here(lamp)) goto l2011;
	spk=184;
	if(limit < 0) goto l2011;
	prop[lamp]=1;
	rspeak(39);
	if(wzdark) goto l2000;
	goto l2012;
	/* Lamp off */
l9080:  
	if(!here(lamp)) goto l2011;
	prop[lamp]=0;
	rspeak(40);
	if(dark()) rspeak(16);
	goto l2012;
	/* Wave.  No effect unless waving rod at fissure. */
l9090:  
	if ((!toting(obj)) && (obj != rod ||  ! toting(rod2))
	    ) spk=29;
	if (obj != rod ||  ! at(fissur) ||  ! toting(obj)
	    || closng) goto l2011;
	prop[fissur]=1-prop[fissur];
	pspeak(fissur,2-prop[fissur]);
	goto l2012;
	/*
		   Attack.  Assume target if unambiguous.  "throw" also
		   links here.  Attackable objects fall into two categories:
		   enemies (snake, dwarf, etc.) and others (bird, clam).
		   Ambiguous if two enemies, or if no enemies but two
		   others.
		*/
l9120:  
	for (i = 1; i <= 5; i++) {
		if(dloc[i] == loc && dflag >= 2) goto l9122;
	}
	i=0;
l9122:  
	if(obj != 0) goto l9124;
	if(i != 0) obj=dwarf;
	if(here(snake)) obj=obj*100+snake;
	if(at(dragon) && prop[dragon] == 0) obj=obj*100+dragon;
	if(at(troll)) obj=obj*100+troll;
	if(here(bear) && prop[bear] == 0) obj=obj*100+bear;
	if(obj > 100) goto l8000;
	if(obj != 0) goto l9124;
	/* Can't attack bird by throwing axe. */
	if(here(bird) && verb != throw) obj=bird;
	/* Clam and oyster both treated as clam for intransitive
		   case; no harm done.  */
	if(here(clam) || here(oyster)) obj=100*obj+clam;
	if(obj > 100) goto l8000;
l9124:  
	if(obj != bird) goto l9125;
	spk=137;
	if(closed) goto l2011;
	dstroy(bird);
	prop[bird]=0;
	if(place[snake] == plac[snake])tally2++;
	spk=45;
l9125:  
	if(obj == 0) spk=44;
	if(obj == clam || obj == oyster) spk=150;
	if(obj == snake) spk=46;
	if(obj == dwarf) spk=49;
	if(obj == dwarf && closed) goto l19000;
	if(obj == dragon) spk=167;
	if(obj == troll) spk=157;
	if(obj == bear) spk=165+(prop[bear]+1)/2;
	if(obj != dragon || prop[dragon] != 0) goto l2011;
	/*
		   Fun stuff for dragon.  If he insists on attacking it,
		   win!  Set prop to dead, move dragon to central loc (still
		   fixed), move rug there (not fixed), and move him there,
		   too.  Then do a null motion to get new description.
		*/
	rspeak(49);
	verb=0;
	obj=0;
	getin(wd1,wd1x,wd2,wd2x);
	if (!eqp (wd1, "y") && !eqp (wd1, "yes")) goto l2608;
	pspeak(dragon,1);
	prop[dragon]=2;
	prop[rug]=0;
	k=(plac[dragon]+fixd[dragon])/2;
	move(dragon+100,-1);
	move(rug+100,0);
	move(dragon,k);
	move(rug,k);
	for (obj=1; obj<=100; obj++) {
		if (place[obj] == plac[dragon] || place[obj] == fixd[dragon]
		    ) move(obj,k);
	}
	loc=k;
	k=nullx;
	goto l8;
	/*
		   Pour.  If no object, or object is bottle, assume contents
		   of bottle.  Special tests for pouring water or oil on
		   plant or rusty door.
		*/
l9130:  
	if(obj == bottle || obj == 0) obj=liq();
	if(obj == 0) goto l8000;
	if(!toting(obj)) goto l2011;
	spk=78;
	if(obj != oil && obj != water) goto l2011;
	prop[bottle]=1;
	place[obj]=0;
	spk=77;
	if(!(at(plant) || at(door))) goto l2011;
	if(at(door)) goto l9132;
	spk=112;
	if(obj != water) goto l2011;
	pspeak(plant,prop[plant]+1);
	prop[plant]=(prop[plant]+2) % 6;
	prop[plant2]=prop[plant]/2;
	k=nullx;
	goto l8;
l9132:  
	prop[door]=0;
	if(obj == oil) prop[door]=1;
	spk=113+prop[door];
	goto l2011;
	/*
		   Eat.  Intransitive:  assume food if present, else ask
		   what.  Transitive:  food ok, some things lose appetite,
		   rest are ridiculous.
		*/
l8140:  
	if(!here(food)) goto l8000;
l8142:  
	dstroy(food);
	spk=72;
	goto l2011;
l9140:  
	if(obj == food) goto l8142;
	if (obj == bird || obj == snake || obj == clam || obj == oyster
	    || obj == dwarf || obj == dragon || obj == troll
	    || obj == bear) spk=71;
	goto l2011;
	/*
		   Drink.  If no object, assume water and look for it here.
		   if water is in the bottle, drink that, else must be at a
		   water loc, so drink stream.
		*/
l9150:  
	if (obj == 0 && liqloc(loc) != water && (liq() != water
	    ||  ! here(bottle))) goto l8000;
	if(obj != 0 && obj != water) spk=110;
	if (spk == 110 || liq() != water ||  ! here(bottle)
	    ) goto l2011;
	prop[bottle]=1;
	place[water]=0;
	spk=74;
	goto l2011;
	/* Rub.  Yields various snide remarks. */
l9160:  
	if(obj != lamp) spk=76;
	goto l2011;
	/*
		   Throw.  Same as discard unless axe.  Then same as attack
		   except ignore bird, and if dwarf is present then one
		   might be killed.  (only way to do so!) Axe also special
		   for dragon, bear, and troll.  Treasures special for
		   troll.
		*/
l9170:  
	if(toting(rod2) && obj == rod &&  ! toting(rod)) obj=rod2;
	if(!toting(obj)) goto l2011;
	if(obj >= 50 && obj <= maxtrs && at(troll)) goto l9178;
	if(obj == food && here(bear)) goto l9177;
	if(obj != axe) goto l9020;
	for (i = 1; i <= 5; i++) {
		/* Needn't check dflag if axe is here. */
		if(dloc[i] == loc) goto l9172;
	}
	spk=152;
	if(at(dragon) && prop[dragon] == 0) goto l9175;
	spk=158;
	if(at(troll)) goto l9175;
	if(here(bear) && prop[bear] == 0) goto l9176;
	obj=0;
	goto l9120;
l9172:  
	spk=48;
	if(ran(3) == 0) goto l9175;
	dseen[i]=0;
	dloc[i]=0;
	spk=47;
	dkill++;
	if(dkill == 1) spk=149;
l9175:  
	rspeak(spk);
	drop(axe,loc);
	k=nullx;
	goto l8;
	/* This'll teach him to throw the axe at the bear! */
l9176:  
	spk=164;
	drop(axe,loc);
	fixed[axe]= -1;
	prop[axe]=1;
	juggle(bear);
	goto l2011;
	/* But throwing food is another story. */
l9177:  
	obj=bear;
	goto l9210;
l9178:  
	spk=159;
	/* Snarf a treasure for the troll. */
	drop(obj,0);
	move(troll,0);
	move(troll+100,0);
	drop(troll2,plac[troll]);
	drop(troll2+100,fixd[troll]);
	juggle(chasm);
	goto l2011;
	/* Quit.  Intransitive only.  Verify intent and exit if
		   that's what he wants.  */
l8180:  
	gaveup=yes(22,54,54);
	if(gaveup) goto l20000;
	goto l2012;
	/* Find.  Might be carrying it, or it might be here.  Else give caveat. */
l9190:  
	if (at(obj) || (liq() == obj && at(bottle))
	    || k == liqloc(loc)) spk=94;
	for (i = 1; i <= 5; i++) {
		if(dloc[i] == loc && dflag >= 2 && obj == dwarf) spk=94;
	}
	if(closed) spk=138;
	if(toting(obj)) spk=24;
	goto l2011;
	/* Inventory.  If object, treat same as find.  Else report
		   on current burden.  */
l8200:  
	spk=98;
	for (i = 1; i <= 100; i++) {
		if(i == bear ||  ! toting(i)) goto l8201;
		if(spk == 98) rspeak(99);
		blklin=0;
		pspeak(i,-1);
		blklin=1;
		spk=0;
l8201:     
		;
	}
	if(toting(bear)) spk=141;
	goto l2011;
	/*
		Feed.  If bird, no seed.  snake, dragon, troll:  quip.  If
		   dwarf, make him mad.  Bear, special.
		*/
l9210:  
	if(obj != bird) goto l9212;
	spk=100;
	goto l2011;
l9212:  
	if(obj != snake && obj != dragon && obj != troll) goto l9213;
	spk=102;
	if(obj == dragon && prop[dragon] != 0) spk=110;
	if(obj == troll) spk=182;
	if(obj != snake || closed ||  ! here(bird)) goto l2011;
	spk=101;
	dstroy(bird);
	prop[bird]=0;
	tally2++;
	goto l2011;
l9213:  
	if(obj != dwarf) goto l9214;
	if(!here(food)) goto l2011;
	spk=103;
	dflag++;
	goto l2011;
l9214:  
	if(obj != bear) goto l9215;
	if(prop[bear] == 0) spk=102;
	if(prop[bear] == 3) spk=110;
	if(!here(food)) goto l2011;
	dstroy(food);
	prop[bear]=1;
	fixed[axe]=0;
	prop[axe]=0;
	spk=168;
	goto l2011;
l9215:  
	spk=14;
	goto l2011;
	/* Fill.  Bottle must be empty, and some liquid available.
		   (vase is nasty.) */
l9220:  
	if(obj == vase) goto l9222;
	if(obj != 0 && obj != bottle) goto l2011;
	if(obj == 0 &&  ! here(bottle)) goto l8000;
	spk=107;
	if(liqloc(loc) == 0) spk=106;
	if(liq() != 0) spk=105;
	if(spk != 107) goto l2011;
	prop[bottle]=(cond[loc] % 4)/2;
	prop[bottle]=prop[bottle]*2;
	k=liq();
	if(toting(bottle)) place[k]= -1;
	if(k == oil) spk=108;
	goto l2011;
l9222:  
	spk=29;
	if(liqloc(loc) == 0) spk=144;
	if(liqloc(loc) == 0 ||  !toting(vase)) goto l2011;
	rspeak(145);
	prop[vase]=2;
	fixed[vase]= -1;
	goto l9024;
	/* Blast.  No effect unless you've got dynamite, which is a
		   neat trick!  */
l9230:  
	if(prop[rod2] < 0 ||  ! closed) goto l2011;
	bonus=133;
	if(loc == 115) bonus=134;
	if(here(rod2)) bonus=135;
	rspeak(bonus);
	goto l20000;
	/* Score.  Go to scoring section, which will return to 8241
		   if scorng is true.  */
l8240:  
	scorng=1;
	goto l20000;
l8241:  
	scorng=0;
	printf ("If you were to quit now, you would score ");
	printf ("%d out of a possible %d in %d turns.\n",
	score, mxscor, turns+1);
	goto l2012;
	/*
		   Fee fie foe foo (and fum).  Advance to next state if given
		   in proper order.  Look up wd1 in section 3 of vocab to
		   determine which word we've got.  Last word zips the eggs
		   back to the giant room (unless already there).
		*/
l8250:  
	k=vocab(wd1,3);
	spk=42;
	if(foobar == 1-k) goto l8252;
	if(foobar != 0) spk=151;
	goto l2011;
l8252:  
	foobar=k;
	if(k != 4) goto l2009;
	foobar=0;
	if (place[eggs] == plac[eggs]
	    || (toting(eggs) && loc == plac[eggs])) goto l2011;
	/* Bring back troll if we steal the eggs back from him
		   before crossing.  */
	if (place[eggs] == 0 && place[troll] == 0 && prop[troll] == 0
	    ) prop[troll]=1;
	k=2;
	if(here(eggs)) k=1;
	if(loc == plac[eggs]) k=0;
	move(eggs,plac[eggs]);
	pspeak(eggs,k);
	goto l2012;
	/* Brief.  Intransitive only.  Suppress long descriptions
		   after first time.  */
l8260:  
	spk=156;
	abbnum=10000;
	detail=3;
	goto l2011;
	/* Read.  Magazines in dwarvish, message we've seen, and .
		   .  .  oyster?  */
l8270:  
	if(here(magzin)) obj=magzin;
	if(here(tablet)) obj=obj*100+tablet;
	if(here(messag)) obj=obj*100+messag;
	if(closed && toting(oyster)) obj=oyster;
	if(obj > 100 || obj == 0 || dark()) goto l8000;
l9270:  
	if(dark()) goto l5190;
	if(obj == magzin) spk=190;
	if(obj == tablet) spk=196;
	if(obj == messag) spk=191;
	if(obj == oyster && hinted[2] && toting(oyster)) spk=194;
	if (obj != oyster || hinted[2] ||  !toting(oyster)
	    ||  !closed) goto l2011;
	hinted[2]=yes(192,193,54);
	goto l2012;
	/* Break.  Only works for mirror in repository and, of
		   course, the vase.  */
l9280:  
	if(obj == mirror) spk=148;
	if(obj == vase && prop[vase] == 0) goto l9282;
	if(obj != mirror ||  !closed) goto l2011;
	rspeak(197);
	goto l19000;
l9282:  
	spk=198;
	if(toting(vase)) drop(vase,loc);
	prop[vase]=2;
	fixed[vase]= -1;
	goto l2011;
	/* Wake.  Only use is to disturb the dwarves. */
l9290:  
	if(obj != dwarf ||  !closed) goto l2011;
	rspeak(199);
	goto l19000;
	/*
			Suspend. Exit leaving things restartable.
		*/
l8300:
	if ((suspfd = fopen (suspfile, SUSPWRITE)) == NULL) {
		printf ("Something's wrong...I can't suspend.\n");
		if (hungup) {
			hungup = 0;
			goto l20000;
		}
		goto l2012;
	}
	hungup = 0;
	printf ("OK...I'm suspending this game in %s\n", suspfile);

	/* Block interrupts to ensure completion of suspension */
	signal (SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGHUP, SIG_IGN);

	/* Write the release and level into the suspend file */
	putw (1, suspfd);
	putw (20, suspfd);

	/* Write the time to prevent premature resumption */
	tvec = time(0);
	putl (tvec, suspfd);

	/* Write the suspend data into the file */
	fwrite (&suspbeg, sizeof suspbeg, &suspend - &suspbeg, suspfd);

	/* Make sure everything went ok */
	if (ferror (suspfd))
		fatal ("I/O error during suspension");

	fclose (suspfd);
#ifdef NOTIME
	printf ("Play will resume automatically next time.\n");
#else
	printf ("You may resume play half an hour from now.\n");
#endif
	exit(0);
	/* Hours.  Report current non-prime-time hours. */
l8310:
	printf (HOURS);
	goto l2012;
	/* Log.  Toggle loggin either on or off */
setlog: 
	logon = ! logon;
	if (logon)
		printf ("Log on.\n");
	else
	    printf ("Log off.\n");
	goto l2012;
	/*
		   hints
		   Come here if he's been long enough at required loc(s) for
		   some unused hint.  Hint number is in variable "hint".
		   branch to quick test for additional conditions, then come
		   back to do neat stuff.  goto 40010 if conditions are met
		   and we want to offer the hint.  goto 40020 to clear
		   hintlc back to zero, 40030 to take no action yet.
		*/
l40000: 
	switch (hint-4) {
	case 0: 
		goto l40400;        /* cave */
	case 1: 
		goto l40500;        /* bird */
	case 2: 
		goto l40600;        /* snake */
	case 3: 
		goto l40700;        /* maze */
	case 4: 
		goto l40800;        /* dark */
	case 5: 
		goto l40900;        /* witt */
	}
	bug(27);
l40010: 
	hintlc[hint]=0;
	if(!yes(hints[hint][3],0,54)) goto l2602;
	printf ("I am prepared to give you a hint,");
	printf (" but it will cost you %d points.\n", hints[hint][2]);
	hinted[hint]=yes(175,hints[hint][4],54);
	if (hinted[hint] && limit > 30
	    ) limit=limit+30*hints[hint][2];
l40020: 
	hintlc[hint]=0;
l40030: 
	goto l2602;
	/* Now for the quick tests.  See database description for
		   one-line notes.  */
l40400: 
	if(prop[grate] == 0 &&  ! here(keys)) goto l40010;
	goto l40020;
l40500: 
	if(here(bird) && toting(rod) && obj == bird) goto l40010;
	goto l40030;
l40600: 
	if(here(snake) &&  ! here(bird)) goto l40010;
	goto l40020;
l40700: 
	if (atloc[loc] == 0 && atloc[oldloc] == 0
	    && atloc[oldlc2] == 0 && holdng > 1) goto l40010;
	goto l40020;
l40800: 
	if(prop[emrald] != -1 && prop[pyram] == -1) goto l40010;
	goto l40020;
l40900: 
	goto l40010;
	/*
		   Cave closing and scoring
		   These sections handle the closing of the cave.  The cave
		   closes "clock1" turns after the last treasure has been
		   located (including the pirate's chest, which may of
		   course never show up).  Note that the treasures need not
		   have been taken yet, just located.  Hence clock1 must be
		   large enough to get out of the cave (it only ticks while
		   inside the cave).  When it hits zero, we branch to 10000
		   to start closing the cave, and then sit back and wait for
		   him to try to get out.  If he doesn't within clock2
		   turns, we close the cave; if he does try, we assume he
		   panics, and give him a few additional turns to get
		   frantic before we close.  When clock2 hits zero, we
		   branch to 11000 to transport him into the final puzzle.
		   Note that the puzzle depends upon all sorts of random
		   things.  For instance, there must be no water or oil,
		   since there are beanstalks which we don't want to be able
		   to water, since the code can't handle it.  Also, we can
		   have no keys, since there is a grate (having moved the
		   fixed object!) there separating him from all the
		   treasures.  Most of these problems arise from the use of
		   negative prop numbers to suppress the object descriptions
		   until he's actually moved the objects.
		   When the first warning comes, we lock the grate, destroy
		   the bridge, kill all the dwarves (and the pirate), remove
		   the troll and bear (unless dead), and set "closng" to
		   true.  Leave the dragon; too much trouble to move it.
		   From now until clock2 runs out, he cannot unlock the
		   grate, move to any location outside the cave (loc<9), or
		   create the bridge.  Nor can he be resurrected if he dies.
		   Note that the snake is already gone, since he got to the
		   treasure accessible only via the hall of the mt.  king.
		   also, he's been in giant room (to get eggs), so we can
		   refer to it.  Also also, he's gotten the pearl, so we
		   know the bivalve is an oyster.  AND, the dwarves must
		   have been activated, since we've found chest.
		*/
l10000: 
	prop[grate]=0;
	prop[fissur]=0;
	for (i = 1; i <= 6; i++) {
		dseen[i]=0;
	}
	move(troll,0);
	move(troll+100,0);
	move(troll2,plac[troll]);
	move(troll2+100,fixd[troll]);
	juggle(chasm);
	if(prop[bear] != 3) dstroy(bear);
	prop[chain]=0;
	fixed[chain]=0;
	prop[axe]=0;
	fixed[axe]=0;
	rspeak(129);
	clock1= -1;
	closng=1;
	goto l19999;
	/*
		   Once he's panicked, and clock2 has run out, we come here
		   to set up the storage room.  The room has two locs,
		   hardwired as 115 (ne) and 116 (sw).  At the ne end, we
		   place empty bottles, a nursery of plants, a bed of
		   oysters, a pile of lamps, rods with stars, sleeping
		   dwarves, and him.  At the sw end we place grate over
		   treasures, snake pit, covey of caged birds, more rods,
		   and pillows.  A mirror stretches across one wall.  Many
		   of the objects come from known locations and/or states
		   (e.g.  the snake is known to have been destroyed and
		   needn't be carried away from its old "place"), making the
		   various objects be handled differently.  We also drop all
		   other objects he might be carrying (lest he have some
		   which could cause trouble, such as the keys).  We
		   describe the flash of light and trundle back.
		*/
l11000: 
	prop[bottle]=put(bottle,115,1);
	prop[plant]=put(plant,115,0);
	prop[oyster]=put(oyster,115,0);
	prop[lamp]=put(lamp,115,0);
	prop[rod]=put(rod,115,0);
	prop[dwarf]=put(dwarf,115,0);
	loc=115;
	oldloc=115;
	newloc=115;
	/* Leave the grate with normal (non-negative property). */
	foo=put(grate,116,0);
	prop[snake]=put(snake,116,1);
	prop[bird]=put(bird,116,1);
	prop[cage]=put(cage,116,0);
	prop[rod2]=put(rod2,116,0);
	prop[pillow]=put(pillow,116,0);
	prop[mirror]=put(mirror,115,0);
	fixed[mirror]=116;
	for (i = 1; i <= 100; i++) {
		if(toting(i)) dstroy(i);
	}
	rspeak(132);
	closed=1;
	goto l2;
	/*
		   Another way we can force an end to things is by having
		   the lamp give out.  When it gets close, we come here to
		   warn him.  We go to 12000 if the lamp and fresh batteries
		   are here, in which case we replace the batteries and
		   continue.  12200 is for other cases of lamp dying.  12400
		   is when it goes out, and 12600 is if he's wandered
		   outside and the lamp is used up, in which case we force
		   him to give up.
		*/
l12000: 
	rspeak(188);
	prop[batter]=1;
	if(toting(batter)) drop(batter,loc);
	limit=limit+2500;
	lmwarn=0;
	goto l19999;
l12200: 
	if (lmwarn ||  !here(lamp)) goto l19999;
	lmwarn=1;
	spk=187;
	if (place[batter] == 0) spk=183;
	if (prop[batter] == 1) spk=189;
	rspeak(spk);
	goto l19999;
l12400: 
	limit= -1;
	prop[lamp]=0;
	if (here(lamp)) rspeak(184);
	goto l19999;
l12600: 
	rspeak(185);
	gaveup=1;
	goto l20000;
	/* Oh dear, he's disturbed the dwarves. */
l19000: 
	rspeak(136);
	/*
		   Exit code.
		   the present scoring algorithm is as follows:
		      objective:          points:        present total possible:
		   getting well into cave   25                    25
		   each treasure < chest    12                    60
		   treasure chest itself    14                    14
		   each treasure > chest    16                   144
		   surviving             (max-num)*10             30
		   not quitting              4                     4
		   reaching "closng"        25                    25
		   "closed": quit/killed    10
		             klutzed        25
		             wrong way      30
		             success        45                    45
		   came to witt's end        1                     1
		   round out the total       2                     2
		                                        total:   350
		   (points can also be deducted for using hints.)
		*/
l20000: 
	score=0;
	mxscor=0;
	/*
		   First tally up the treasures.  Must be in building and
		   not broken.  Give the poor guy 2 points just for finding
		   each treasure.
		*/
	for (i = 50; i <= maxtrs; i++) {
		if (ptext[i] != 0) {
			k=12;
			if (i == chest) k=14;
			if (i > chest) k=16;
			if (prop[i] >= 0) score=score+2;
			if (place[i] == 3 && prop[i] == 0) score=score+k-2;
			mxscor=mxscor+k;
		}
	}
	/*
		   Now look at how he finished and how far he got.  maxdie
		   and numdie tell us how well he survived.  gaveup says
		   whether he exited via quit.  dflag will tell us if he
		   ever got suitably deep into the cave.  closng still
		   indicates whether he reached the endgame.  And if he got
		   as far as "cave closed" (indicated by "closed"), then
		   bonus is zero for mundane exits or 133, 134, 135 if he
		   blew it (so to speak).
		*/
	score=score+(maxdie-numdie)*10;
	mxscor=mxscor+maxdie*10;
	if (!(scorng || gaveup)) score=score+4;
	mxscor=mxscor+4;
	if (dflag != 0) score=score+25;
	mxscor=mxscor+25;
	if (closng) score=score+25;
	mxscor=mxscor+25;
	if (!closed) goto l20020;
	if (bonus == 0) score=score+10;
	if (bonus == 135) score=score+25;
	if (bonus == 134) score=score+30;
	if (bonus == 133) score=score+45;
l20020: 
	mxscor=mxscor+45;
	/* Did he come to Witt's End as he should? */
	if (place[magzin] == 108)score++;
	mxscor++;
	/* Round it off. */
	score=score+2;
	mxscor=mxscor+2;
	/* Deduct points for hints.  hints < 4 are special; see
		   database description.  */
	for (i = 1; i <= hntmax; i++) {
		if (hinted[i]) score=score-hints[i][2];
	}
	/* Return to score command if that's where we came from. */
	if (scorng) goto l8241;
	/* That should be good enough.  Let's tell him all about it. */
	printf ("You scored %d out of a possible %d using %d turn%s.\n",
	score, mxscor, turns, turns==1? "": "s");
	for (i = 1; i <= clsses; i++) {
		if (cval[i] >= score) goto l20210;
	}
	printf("You just went off my scale!!!\n");
	goto l25000;
l20210: 
	speak(ctext[i]);
	if (i == clsses-1) goto l20220;
	k=cval[i]+1-score;
	printf ("To achieve the next higher rating, you need %d more point%s.\n",
	k, k==1? "": "s");
	goto l25000;
l20220:
	printf ("To achieve the next higher rating would be a neat trick!\n");
	printf ("Congratulations!!\n");
l25000:
	if (logon) {
		/* Log this termination for the interest of other users */
		FILE *logfile;
		char *ctime();
		if ((logfile = fopen ("/usr/games/advlog", "a")) != NULL) {
			tvec = time((long *) 0);
			cp1 = ctime (&tvec);
			/* Assumed format "Mon Jan 99 99:99:99 1999\n\0" */
			cp1[10] = '\0';
			fprintf (logfile, "%s; %s: %d in %d\n",
			cp1 + 4, pwbuf -> pw_name,
			score, turns);
		}
	}
}

/*
 *	subroutines/functions
 *	toting(obj)  = true if the obj is being carried
 *	here(obj)    = true if the obj is at "loc" (or is being carried)
 *	at(obj)      = true if on either side of two-placed object
 *	liq(dummy)   = object number of liquid in bottle
 *	liqloc(loc)  = object number of liquid (if any) at loc
 *	bitset(l,n)  = true if cond(l) has bit n set (bit 0 is units bit)
 *	forced(loc)  = true if loc moves without asking for input (cond=2)
 *	dark(dummy)  = true if location "loc" is dark
 *	pct(n)       = true n% of the time (n integer from 0 to 100)
 */

toting(ob)
{
	return place[ob] == -1;
}

here(ob)
{
	return place[ob] == loc || toting (ob);
}

at(ob)
{
	return place[ob] == loc || fixed[ob] == loc;
}

liq2(pbotl)
{
	int liq2temp;
	liq2temp = pbotl/2;
	return (1-pbotl)*water + liq2temp * (water+oil);
}

liq()
{
	int t;
	t = prop[bottle];
	return liq2(t>-1-t? t: -1-t);
}

liqloc(where)
{
	int t1, t2;
	t1 = cond[where] / 2;
	t1 = t1 * 2;
	t2 = cond[where] / 4;
	return liq2 (((t1 % 8)-5)*(t2%2)+1);
}

bitset (mm, n)
{
	return (cond[mm] >> n) & 1;
}

forced(where)
{
	return cond[where] == 2;
}

dark()
{
	return ((cond[loc] & 1) == 0) && (prop[lamp] == 0 || !here(lamp));
}

pct (n)
{
	return ran(100) < n;
}

/*
 *	Place any object anywhere by picking it up and dropping
 *	it. May already be toting, in which case the carry is
 *	a no-op. Mustn't pick up objects which are not at any
 *	loc, since carry wants to remove objects from atloc chains.
 */
move (object, where) {
	int source;
	if (object <= 100)
		source = place[object];
	else
		source = fixed[object-100];
	if (source > 0 && source <= 300)
		carry (object, source);
	drop (object, where);
}

dstroy (object)
{
	move (object, 0);
}

juggle (object)
{
	register int ii, jj;
	ii = place[object];
	jj = fixed [object];
	move (object, ii);
	move (object+100, jj);
}

put (object, where, pval)
{
	move (object, where);
	return -1-pval;
}

/*
 *	Start toting an object, removing it from the list of things at
 *	its former location. Increment holding unless it was already
 *	being toted. If object>100 (moving "fixed" second loc)
 *	don't change place or holdng.
 */
carry (object, where)
{
	int tmp;
	if (object <= 100) {
		if (place[object] == -1) return;
		place[object] = -1;
		holdng++;
	}
	if (atloc[where] == object) {
		atloc[where] = rlink[object];
		return;
	}
	tmp = atloc[where];
	while (rlink[tmp] != object)
		tmp = rlink[tmp];
	rlink[tmp] = rlink[object];
}

/*
 *	Place an object at a given loc, prefixing ot onto the atloc list.
 *	Decrement holdng if the object was being toted.
 */
drop (object, where)
{
	if (object > 100)
		fixed[object-100] = where;
	else
	    {
		if (place[object] == -1) holdng--;
		place[object] = where;
	}
	if (where <= 0) return;
	rlink[object] = atloc[where];
	atloc[where] = object;
}

fatal(s)
	char *s;
{
	printf ("\nFatal error: %s\n", s);
	exit(1);
}

bug(n)
{
	printf ("Bug number %d\n", n);
	printf ("Program quits\n");
	exit(1);
}

/* Returns a random number between 0 and num-1 inclusive */
ran(num)
{
	return (((long) num * rand()) / 32768L);
}

/*
 *	return 1 if the five-character argument
 *	is entirely blank, 0 otherwise
 */
blankp(a5)
char *a5;
{
	return eqp (a5, "     ");
}

/*
 *	return 1 if a5 and b5 are equal, 0 otherwise.
 *	The lengths of a5 and b5 are limited to 5,
 *	but if either contains a null character, it is assumed
 *	to be padded out to length 5 with blanks.
 */
eqp(a5, b5)
	char *a5, *b5;
{
	register int z;
	register char *aa, *bb;
	aa = a5;
	bb = b5;
	z = 5;
	do {
		if ((*aa == '\0'? ' ': *aa++) !=
		    (*bb == '\0'? ' ': *bb++))
			return 0;
	} while (--z);
	return 1;
}

/*
 *	copy the character string from "source" to "sink". Length is limited
 *	to 5 characters, and "sink" is blank padded if "source" is shorter.
 */
cpy (sink, source)
	register char *source, *sink;
{
	register n;
	n = 5;
	do {
		if (*source == '\0')
			*sink++ = ' ';
		else
		    *sink++ = *source++;
	} while (--n);
}

/*
 *	Look up id in the vocabulary (atab) and return its "definition"
 *	(ktab), or -1 if not found. If init is positive, this is an
 *	initialization call setting up a keyword variable, and not finding
 *	it constitutes a bug. It also means that only ktab values which taken
 *	over 1000 equal init may be considered. Thus "steps", which is a
 *	motion verb as well as an object, may be located as an object. It also
 *	means the ktab value is taken mod 1000.
 */
vocab (id, init)
char *id;
{
	for (i=1; i<=tabsiz; i++) {
		if (ktab[i] == -1) goto l2;
		if ((init < 0 || init == ktab[i]/1000) && eqp(atab[i], id))
			goto l3;
	}
	bug(21);
l2:
	if (init < 0) return -1;
	bug(5);
l3:
	return init<0? ktab[i]: ktab[i] % 1000;
}

/*
 *	This program catenates the characters of x and y, which are assumed to
 *	be 5-character fields, into z. The process stops at the first blank,
 *	and a null character is appended to the result.
 */
a5toa1 (x, y, z)
	char *x, *y, *z;
{
	register int n;
	n = 5;
	do {
		if (*x == ' ') {
			*z++ = '\0';
			return;
		}
		*z++ = *x++;
	} while (--n);
	n = 5;
	do {
		if (*y == ' ') {
			*z++ = '\0';
			return;
		}
		*z++ = *y++;
	} while (--n);
	*z++ = '\0';
}

/*
 *	Get a command from the terminal. The first word goes into pl and pr,
 *	and the second word goes into ql and qr. In each case the word is
 *	padded with blanks to 10 characters; the first 5 will be in the "l"
 *	variable and the second 5 will be in the "r" variable.
 *	If hungup is nonzero, indicating he hung up the phone, fudge
 *	pl, pr, ql, and qr to make it look as if he typed "suspend".
 */
getin (pl, pr, ql, qr)
	char *pl, *pr, *ql, *qr;
{
	register int p;
	cpy (pl, "");
	cpy (pr, "");
	cpy (ql, "");
	cpy (qr, "");
	fflush(stdout);
	/* Eat blank lines */
	if (!hungup) {
		while ((p = getchar()) == '\n');
		while (p == '!') {
			char pl[512];
			if (fgets (pl, sizeof(pl), stdin) == 0)
				p = EOF;
			if (strcmp (pwbuf->pw_name, "games") == 0)
				printf ("No Shell escape from \"games\"\n");
			else {
				system (pl);
				printf ("!\n");
				fflush (stdout);
			}
			if (p != EOF)
				p = getchar();
		}
		if (p == EOF) {
			cpy (pl, "suspe");
			cpy (pr, "nd");
			hungup = 1;
			return;
		}
		ungetc (p, stdin);
	}
	if (snarf (pl, pr) && snarf (ql, qr))
		while (getchar() != '\n');
}

/*
 *	This is a subroutine of getin
 */
snarf (left, right)
	char *left, *right;
{
	register int n;
	char s[10];
	register int p;
	/* Blank the array */
	for (n=0; n<10; n++)
		s[n] = ' ';
	/* If hung up phone, pretend he said 'suspend' followed by nl */
	if (hungup)
		goto susp;
	/* Skip leading blanks; if nl encountered, return immediately */
	while ((p=getchar()) == ' ');
	if (p == '\n') return 0;
	if (p == EOF) goto susp;
	/* Now eat characters until blank or newline */
	n = 0;
	do {
		if (n < 10)
			s[n++] = p;
		p = getchar();
		if (p == EOF) goto susp;
	} while (p != ' ' && p != '\n');
	/* Break up the string into two five-character components */
	cpy (left, s);
	cpy (right, s+5);
	/* Indicate to caller whether we hit a blank or newline */
	return p == ' ';
	/* Abnormal exit for EOF */
susp:
	cpy (left, "suspe");
	cpy (right, "nd");
	hungup = 1;
	return 0;
}

/*
 *	Print the n-th "random" message (section 6)
 */
rspeak(n)
{
	if (n != 0)
		speak (rtext[n]);
}

/*
 *	Print the message which starts at lines[n]
 */
speak(n)
{
	long rec;
	do {
		rec = lines[n];
		if (rec < 0)
			rec = -rec;
		fseek (caves, rec, 0);
		fgets (linebuf, sizeof linebuf, caves);
		if (linebuf[0] != '>')
			fputs (linebuf, stdout);
	} while (++n < linuse && lines[n] >= 0);
}

/*
 *	Find the skip+1st message from msg and print it. Msg should be
 *	the index of the inventory message for object
 */
pspeak (msg, skip)
{
	int n, q;
	q = ptext[msg];
	if (skip >= 0)
		for (n=0; n<=skip; n++) {
			while (lines[++q] >= 0);
		}
	speak(q);
}

/*
 *	Print message x, wait for yes/no. If yes, print message y and leave
 *	"yea" true; else print message z and leave "yea" false.
 *	If hungup is nonzero, he hung up the phone, so simulate an
 *	answer of "no".
 */
yes (x, y, z) {
	char reply[300];
	rspeak(x);
	fflush(stdout);
	if (scanf ("%s", reply) == EOF)
		hungup = 1;
	while (hungup == 0 && reply[0] != 'y' && reply[0] != 'n') {
		printf ("Please answer the question.\n");
		fflush(stdout);
		if (scanf ("%s", reply) == EOF)
			hungup = 1;
	}
	if (yea = (reply[0] == 'y' && hungup == 0)) {
		if (y != 0)
			rspeak(y);
	} else if (z != 0)
		rspeak (z);
	return yea;
}
