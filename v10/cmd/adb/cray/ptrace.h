/*
 * opcodes for ptrace
 * should be in a system header
 * sigh
 */

#define	P_INIT	0	/* let my parent trace me */
#define	P_RDI	1	/* read a word of i-space */
#define	P_RDD	2	/* read a word of d-space */
#define	P_RDU	3	/* read a word of user block */
#define	P_WRI	4	/* write a word of i-space */
#define	P_WRD	5	/* write a word of d-space */
#define	P_WRU	6	/* write a word to user block */
#define	P_CONT	7	/* continue running after signal */
#define	P_KILL	8	/* kill tracee */
#define	P_STEP	9	/* single step */

#define	CONTNEXT (int *)1	/* continue where we stopped */

typedef long PTWORD;
PTWORD ptrace();
