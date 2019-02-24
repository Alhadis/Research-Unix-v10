/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 * 2.1.1.2 Translation Phases
 *	This module tests the translation phases by making sure that
 *  they are processed in the right order.
 */


/* #define NOTDEFINED 1 */

??=include<stddef.h>
#include/* SPACE */"defs.h"
#\
i\
f\
!\
d\
e\
f\
i\
n\
e\
d\
(\
N\
O\
T\
D\
E\
F\
I\
N\
E\
D\
)
#\
d\
e\
f\
i\
n\
e\
 \
U\
I\
 \
u\
n\
s\
i\
g\
n\
e\
d\
 \
i\
n??/
t

#define/* SPACE */PCHAR(/*SPACE*/x/*SPACE*/)/* SPACE */char x/*SPACE*/??(??)

#endif
UI/* SPACE */Ui/* SPACE */=/* SPACE */'\
\n' ;
PCHAR(Pc) = "string1??/
#\
"\
"string2" ;

#ifndef FREESTANDING
/* hosted environment assumes an invocation as "env 1" */
int main(
	int argc,
	char **argv)
#else
int main()
#endif
	??<
	extern int Debug;

	Filename = "b2.h";
#ifndef FREESTANDING
	if (argc > 2)
		Debug = TRUE;
#endif
	/* 2.1.1.2 - Phases of translation are tested throughout this module.  */
	{
		/* separate tokens remain separate unless catenated with ## */
		#define M -
    	int a=10, b=5, c;
	    c = a M-- b;
		iequals(__LINE__, a, 10);
		iequals(__LINE__, b, 4);
		iequals(__LINE__, c, 6);
	}
	checkthat(__LINE__, Ui??'Ui == 0);
	checkthat(__LINE__, Ui??!Ui == Ui);
	checkthat(__LINE__, ??-Ui == ~'\n');

	/* 2.1.2.2 - Hosted Environment */
#ifndef FREESTANDING
	checkthat(__LINE__, argc > 0);
	if (argc < 2)
		complain(__LINE__);
	else
		{
		stequals(- __LINE__, argv[1], "1");
		argv[1] = "string1??=string2";
		aequals(__LINE__, argv[argc], NULL);
		stequals(__LINE__, Pc, argv[1]);
		}
#endif

	/* 2.2.1 - Character sets */
	iequals(__LINE__, '0'+1, '1');
	iequals(__LINE__, '1'+1, '2');
	iequals(__LINE__, '2'+1, '3');
	iequals(__LINE__, '3'+1, '4');
	iequals(__LINE__, '4'+1, '5');
	iequals(__LINE__, '5'+1, '6');
	iequals(__LINE__, '6'+1, '7');
	iequals(__LINE__, '7'+1, '8');
	iequals(__LINE__, '8'+1, '9');
	iequals(__LINE__, sizeof('\n'), sizeof(int));
#ifndef FREESTANDING  /* 92/05/04 see upda3a.01 #515 */
	iequals(__LINE__, sizeof(**argv), 1);
#endif /* FREESTANDING */
	return report("ENVIRON");
	??>
