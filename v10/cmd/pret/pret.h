#define NORM	0	/* normal  message */
#define INITM	1	/* initial message */

#define SND	2
#define RCV	4
#define SAR	6
#define RFR	1
#define DCL	2
#define RAD	3
#define ADR	4
#define DAR	6

#define MAXNAME	32	/* max available composite namelength */
#define MAXPROC	16	/* max number of processes */
#define MANY	512	/* max nr of distinct rows or names */
#define MAXDEPTH 32	/* max nesting depth of if's and do's */
#define EXPRMAX	64
#define NOSTATE -1	/* default state */

#define SOME	 1	/* not none */
#define NONE	-1	/* default signal */
#define NEW	-1
#define OLD	0
#define LAB	1

#define ISM	0	/* message variable   */
#define ISQ	1	/* queue-set variable */
#define ISV	2	/* protocol variable  */
#define ISQN	3	/* queuename variable */

			/* types of transitions: */
#define INP	10		/* recv specific message */
#define DFL	11		/* default input from q  */
#define TMO	12		/* transition on timeout */
#define OUTP	13		/* append message to a q */
#define SPN	14		/* builtin call or transit state */
#define CND	15		/* conditional   */
#define FCT	16		/* function call */

				/* `builtins:' */
#define INCR	16		/* increment   */
#define DECR	17		/* decrement   */
#define SUBT	18		/* subtract    */
#define ADDT	19		/* add         */
#define SETV	32		/* set:     =  */
#define ISEQ	256		/* compare: == */
#define NTEQ	257		/* !=          */
#define GREQ	258		/* >=          */
#define SMEQ	259		/* <=          */
#define GRNQ	260		/* >           */
#define SMNQ	261		/* <           */

#define SPNT(u)	(u == TMO || u == OUTP)

struct QTABLE {
	char name[MAXNAME];
	short status;
	short limit;
	short owner;
	int multiple;
	unsigned char magic;
};

#define BASEVAL	MAXPROC		/* base value for coding messages */
#define NQUEUES	2*MAXPROC	/* maximum number of queues */

struct PROCTABLE {
	char name[MAXNAME];
	int nrstates;
	int unreach;
	int replic;
};
