
/*
 *
 * A few definitions needed by routines that handle the editing expressions.
 * Stuff's primarily used by the scanner.
 *
 */


#define START		0		/* just starting next statment */
#define ASSIGNOP	1
#define LPAREN		2
#define RPAREN		3
#define PLUS		4
#define MINUS		5
#define TIMES		6
#define DIVIDE		7
#define UMINUS		8
#define CONSTANT	9
#define XREF		10
#define YREF		11
#define HEIGHT		12
#define WIDTH		13
#define CHWIDTH		14
#define ENDEDIT		15		/* found "end" string - done editing */
#define BUILD		16		/* found a "build" command - ends edit */


typedef struct  {

	char	*str;			/* when we find this string */
	int	code;			/* scanner returns this value */

} Tokens;

