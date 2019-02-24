
/* affix codes */
#define	ED		01	/* +ed, +ing */
#define	ADJ		02	/* (nce)-t_ce, +ize,+al, +ness, -t+cy,
					+ity, +ly */	
#define	NOUN		04	/* +s (+es), +make, +hood, +ship
				   +less  */
#define PROP_COLLECT	010	/* +'s,  +an, +ship(for -manship)
				   +less */
#define ACTOR		020	/* +er  */
#define	EST		040
#define COMP		(EST | ACTOR)	/* +er,+est */
#define	DONT_TOUCH	0100
#define	ION		0200	/* +ion, +or */
#define	N_AFFIX		0400 	/* +ic, +ive, +ize, +like, +al,
				 +ful, +ism, +ist, -t+cy */
#define	V_AFFIX		01000	/* +able, +ive, +ity((bility),
				  +ment */
#define	V_IRREG		02000	/* +ing +es +s*/
#define	VERB		(V_IRREG | ED) 
#define MAN		04000	/* +man, +men, +women, +woman */
#define	ADV		010000	/* +hood, +ness */
#define STOP		040000	/* stop list */
#define	NOPREF		020000	/* no prefix */

#define MONO		0100000	/* double final consonant as in fib->fibbing */
#define IN		0200000 /* in- im- ir, not un- */
#define _Y		0400000	/* +y */

#define ALL		(~(NOPREF|STOP|DONT_TOUCH|MONO|IN))    /*anything goes (no stop or nopref)*/
