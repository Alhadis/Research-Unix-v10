/* i8save.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

integer i8save_(isw, ivalue, set)
integer *isw, *ivalue;
logical *set;
{
    /* Initialized data */

    static struct {
	integer e_1[2];
	} equiv_1 = { 0, 2 };


    /* System generated locals */
    integer ret_val;

    /* Local variables */
#define iparam ((integer *)&equiv_1)
#define lrecov ((integer *)&equiv_1 + 1)
#define lerror ((integer *)&equiv_1)


/*  IF (ISW = 1) I8SAVE RETURNS THE CURRENT ERROR NUMBER AND */
/*               SETS IT TO IVALUE IF SET = .TRUE. . */

/*  IF (ISW = 2) I8SAVE RETURNS THE CURRENT RECOVERY SWITCH AND */
/*               SETS IT TO IVALUE IF SET = .TRUE. . */



/*  START EXECUTION ERROR FREE AND WITH RECOVERY TURNED OFF. */


    ret_val = iparam[*isw - 1];
    if (*set) {
	iparam[*isw - 1] = *ivalue;
    }

    return ret_val;

} /* i8save_ */

#undef lerror
#undef lrecov
#undef iparam


