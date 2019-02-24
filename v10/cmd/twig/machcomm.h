/*
 * The machine is laid out as a sequence of integers in the walker.
 * The form described above is only used inside the preprocessor.
 * Each machine state is one of the following sequence:
 *
 * TABLE <value_1><index_1>...<value_n><index_n> -1 [FAIL <fail_index>] -1
 * or
 * TABLE2 <value_1><index_1>...<value_n><index_n> -1 [FAIL <fail_index>] -1
 * or
 * ACCEPT <accept table index> -1
 *
 * The accept table is in the form:
 *
 * <tree index_1> ...<tree_index_m> -1
 *
 */

#define FASTLIM	0
#define TABLE	1
#define FAIL	2
#define ACCEPT	3
#define TABLE2	4
#define EOT	-1

/* special machine state */
#define HANG	-1

/* used by the evaluator to interpret path table */
#define	eSTOP	0
#define	ePOP	-1
#define eEVAL	-2
#define eNEXT	-3
#define ePUSH	-4

/* Tags that indicate the type of a value */
#define M_BRANCH 010000
#define M_NODE	0
#define M_LABEL 01000
#define MAX_NODE_VALUE 00777
#define MTAG_SHIFT 9
#define M_DETAG(x)	((x)&~(M_BRANCH|M_LABEL|M_NODE))

/* predicates to tell whether a value x is of type NODE, BRANCH, or LABEL */
#define MI_NODE(x)	(((x)&(M_BRANCH|M_LABEL))==0)
#define MI_DEFINED(x)	((x)!=-1)
#define MI_LABEL(x)	(((x)&M_LABEL)!=0)
#define MI_BRANCH(x)	(((x)&M_BRANCH)!=0)

/* build a tagged value */
#define MV_NODE(x)	(x)
#define MV_BRANCH(x)	((x)|M_BRANCH)
#define MV_LABEL(x)	((x)|M_LABEL)

