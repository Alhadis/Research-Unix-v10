#define NONE	-1
#define MAXPROC	16
#define PLENTY	16		/* max namelength */

#define MANY	512		/* range globals: 0 - MANY          */
				/* range locals : MANY - 2*MANY     */
				/* range params : 2*MANY and 3*MANY */
				/* range constants: 3*MANY++ or 3*MANY-- */

				/* `column types:'       */
#define INP	10		/* recv specific message */
#define DFL	11		/* default input from q  */
#define TMO	12		/* transition on timeout */
#define OUTP	13		/* append message to a q */
#define SPN	14		/* builtin call or transit state */
#define CND	15		/* conditional   */
#define FCT	16		/* function call */

#define INCR	16		/* increment a sequence number       */
#define DECR	17		/* decrement a sequence number       */
#define SUBT	18		/* subtract    */
#define ADDT	19		/* add         */
#define SETV	32		/* set seq. number to initial value  */
#define ISEQ	256		/* compare two variables (s.numbers) */
#define NTEQ	257		/* != */
#define GREQ	258		/* >= */
#define SMEQ	259		/* <= */
#define GRNQ	260		/* >  */
#define SMNQ	261		/* <  */

#define POP	1
#define PUSH	-1

#define SO	2	/* send only    */
#define RO	4	/* recv only    */
#define SR	6	/* send & recv  */
#define TC	8	/* reftask call */
#define TO	16	/* timeout      */
#define LV	32	/* local  var updated */
#define GV	64	/* global var updated */

#define FAIL	-1	/* backward receive: cargo mismatch */

#define NOREF(i)	(i < 0 || i >= MAXPROC)
#define NOTOOBIG	16383	/* was  32767 */
