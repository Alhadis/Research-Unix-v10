/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "defs.h"

void c3_1();
void c3_2();
void c3_3();
void c3_4();
void c3_5();
void c3_6();
void c3_7();
void c3_8();

int main(argc, argv)
	int argc;
	char **argv;
	{
	extern int Debug;

	if (argc > 1)
		Debug = TRUE;

	Filename = "lang.c";
	c3_1();
	c3_2();
	c3_3();
	c3_4();
	c3_5();
	c3_6();
	c3_7();
	c3_8();
	return report("LANG");
	}







/*
 * Global variables needed elsewhere in the tests.
 */
int e12 = 12;	/* should not link with static e12 in c3_1_2_2 */
int e15 = 15;	/* should link with extern e15 in c3_1_2_2 */

struct { char i; int j; }   s3126 = { 1, 2};	/* compatible with s3126 in c3_1_2_6 */
union { char i; int j; }    u3126 
#if ANSI
								  = { 3 }
#endif
										;		/* compatible with u3126 in c3_1_2_6 */
enum { E3126A = 4, E3126B } e3126 = { E3126B };	/* compatible with e3126 in c3_1_2_6 */
