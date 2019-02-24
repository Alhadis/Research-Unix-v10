/* i8tsel.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

integer i8tsel_(inow)
integer *inow;
{
    /* Initialized data */

    static integer ienter = 0;

    /* System generated locals */
    integer ret_val;


/*  TO RETURN I8TSEL = THE POINTER TO THE CURRENT ENTER-BLOCK AND */
/*  SET THE CURRENT POINTER TO INOW. */

/*  START WITH NO BACK-POINTER. */


    ret_val = ienter;
    if (*inow >= 0) {
	ienter = *inow;
    }

    return ret_val;

} /* i8tsel_ */

