 struct ELM {
	short transf;		/* next state function */
	short valtrans;		/* value transferred   */
 };

 struct IND {
	short nrpils;
	struct ELM *one;
 };

 struct VARPARS {
	short nrms,    *ms;		/* message parameters */
	short nrvs,    *vs;		/* pvar parameters    */
	short nrlvars, *lvarvals;	/* local variables    */
 };

 struct TBLPARS {
	short nrms;
	short nrvs;			/* max available space for params */	
	short nrlvars;			/* max available space for locvars */
 };

 struct LOCVARS {
	short nrlvars, *lvarvals;
 };

 struct CPARS {
	short callwhat;
	short nrms,    *ms;		/* message parameters */
	short nrvs,    *vs;		/* pvar parameters    */
 };

 struct LBT {
	int *mapcol, *orgcol;		/* mapped versions of colmap and colorg */
 };

/*
 *	endrow:		proper endstates of the process table
 *	deadrow:	set if deadend (eg return state)
 *	badrow:		row with at least one output option
 *	labrow:		labeled row (potential start of a loop)
 */

 struct TBL {
	int nrrows, nrcols;
	int *endrow, *deadrow, *badrow, *labrow;
	int *colmap, *colorg, *coltyp;
	char **rowname;

	struct IND **ptr;
	struct CPARS *calls;
 };

 struct MBOX {
	char limit;		/* length of queue */
	char owner;		/* the process reading from this queue */
	char qname[PLENTY];	/* user-level name for the queue */
 };

 struct MNAME {
	char mname[PLENTY];
 };

 struct BLTIN {
	short target;
	short type;
	short value;
 };

 struct REVPOL {
	char toktyp;
	short tokval;
 };

 struct PROCSTACK {
	short uptable;			/* table where we come from         */
	short uptransf;			/* pending transition in that table */

	struct VARPARS  *varparsaved;	/* saves parameters and locals */
	struct PROCSTACK *follow;	/* next stackframe */
 };

#define USED	32768
#define PASSED	16384
#define TRAIL	1

 struct QUEUE {
	short mesg;			/* on rcv PASSED is OR'ed in */
	unsigned short cargo;		/* when set USED is OR'ed in */
#if TRAIL
	short tstate[2];		/* linecode reference */
#endif TRAIL
	struct QUEUE *next;
	struct QUEUE *last;
	struct QUEUE *s_forw;
	struct QUEUE *s_back;
 };

 struct CONTS {
	short mesg;
	unsigned short cargo;
 };

 struct TEMPLATE {
	short *g_vars;			/* current globals   */
	struct LOCVARS   **l_vars;	/* current local vars */
	struct PROCSTACK **traceback;	/* traceback of complete stack */
 };

 struct STATE {
	unsigned short *pstate;		/* old states & maps    */
	struct TEMPLATE *pcon;		/* variables and stacks */
	struct VISIT *next;
 };

#define ANALYZED	16384

 struct VISIT {
	unsigned int howmany;	/* which queues are nonempty */
	struct CONTS **c;	/* queue contents  */
	union {
		struct QUEUE *s;	/* when searching, last message sent  */
		short countme;		/* when analyzed, counts nr of returns */
	} prop;
	unsigned short depth;	/* level of first visit, ANALYZED is OR'ed in */
	struct VISIT *next;
 };

#define ISANA(x)	(x->depth &  ANALYZED)
#define DEPTH(x)	(x->depth & ~ANALYZED)

/*
 *	normal ppushes (via forward()) save the parameters in the procstack
 *	so that the corresponding ppop (via backup()) can reset them properly
 *
 *	series of ppushes and ppops, performed in a single freeze() call
 *	(via the retable() and dotable() subroutines) are different:
 *
 *	additional ppushes are no problem, since they save the
 *	proper state information via the normal route
 *
 *	additional ppops however would lose the procstacked info
 *	that info is stored in the CUBE structure so that when
 *	a ppop from freeze() is undone in unfreeze()
 *	the proper state information can be restored onto the procstack
 */

 struct CUBE {
	short poporpush;
	short which;
	short procsaved;
	short transfsaved;

	struct VARPARS  *varparsaved;	/* parameter map and local var map */

	struct CUBE *pntr;
	struct CUBE *rtnp;
 };

 struct FREEZE {
	short lastsav;
	short *statsaved;		/* save states of tables */
	short *varsaved;		/* save global variables */
	short whichvar;			/* set if a var changed */
	short oldvalue;			/* of that loc var      */

	struct CUBE *cube;		/* place to save stackframe */
 };

 struct Swiffle {
	struct STATE *st;
	struct VISIT *vi;
	struct Swiffle *next;
	struct Swiffle *last;
 };
