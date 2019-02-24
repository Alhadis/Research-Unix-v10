/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP36
/*
 * 3.6 - Statements
 */
#include "defs.h"

static void c3_6_1();
static void c3_6_2();
static void c3_6_3();
static void c3_6_4_1();
static void c3_6_4_2();
static void c3_6_5_1();
static void c3_6_5_2();
static void c3_6_5_3();
static void c3_6_6_1();
static void c3_6_6_2();
static void c3_6_6_3();
static void c3_6_6_4();

void c3_6()
	{
	Filename = "c36.c";
	c3_6_1();
	c3_6_2();
	c3_6_3();
	c3_6_4_1();
	c3_6_4_2();
	c3_6_5_1();
	c3_6_5_2();
	c3_6_5_3();
	c3_6_6_1();
	c3_6_6_2();
	c3_6_6_3();
	c3_6_6_4();
	}






/*
 * 3.6.1  - Labeled statements
 */
static void c3_6_1()
	{
	int i;
	/* labels do not interfere with the flow of control */
	i = 1;
a:
	iequals(__LINE__, i, 1);
	i = 2;
b:
	iequals(__LINE__, i, 2);
	i = 3;
c: d: e:
	iequals(__LINE__, i, 3);
	}

































/*
 * 3.6.2  - Compound statement or block
 */
static void c3_6_2()
	{
	int i = 1;
		{
		int i = 2;
			{
			int i = 3, j = i + 1;
			static float c3_6var = 5.;		/* see also 3.1.2.1 Scope */
				{
				/* initialize the block each time it is entered */
				int j;
				for (j = 7; j < 10; ++j)
				lab:{
					int i = j;
					static int k = 9;
#if ANSI
					extern int c3_6var;
					iequals(__LINE__, i, j);
					iequals(__LINE__, k, 9);
					iequals(__LINE__, c3_6var, 12);
#endif /* ANSI */
					}
				}
			iequals(__LINE__, i, 3);
			iequals(__LINE__, j, 4);
			dequals(__LINE__, c3_6var, 5.);
			}
		iequals(__LINE__, i, 2);
		}
	iequals(__LINE__, i, 1);
	}
int c3_6var = 12;















/*
 * 3.6.3  Expression and null statement
 */
static void c3_6_3()
	{
	int i, j, k;
	char p[4];
	/* expressions don't have to do anything */
	i = ivalue(11);
	;;;;;
	i;
	1;2;(void)3;
	iequals(__LINE__, i, 11);

#if (V7 || ANSI)
	/* evaluate an expression for its side effects */
	(void)str_cpye(p, "abc");
	checkthat(__LINE__, str_cmp(p, "abc") == 0);
	str_cpye(p, "xyz");
	checkthat(__LINE__, str_cmp(p, "xyz") == 0);
#endif

	/* null statement in use */
	for (i = 0; i < 10; ++i)
		;
	iequals(__LINE__, i, 10);
label: ;
	}






















/*
 * 3.6.4 - Selection statements
 */

/*
 * 3.6.4.1  - The 'if' statement
 */
static void c3_6_4_1()
	{
	int i = ivalue(1), j = ivalue(2), k = ivalue(3);
	double d = dvalue(1.);
	char *p = 0;

	/* the controlling expr is of scalar type */
	if (d) dequals(__LINE__, d, 1.);
	else   complain(__LINE__);

	if (p) complain(__LINE__);
	else   aequals(__LINE__, p, (char *)0);

	/* check else binding (nearest if) */
	if (ivalue(i+j) == 0)
		complain(__LINE__);
	else if (ivalue(j+k) == 0)
		complain(__LINE__);
	else
		lab: i = 4;
	iequals(__LINE__, i, 4);

	/* and in the presence of labels */
	goto lab1;
	if (i != 0)
lab1:	i = 5;
	else
		i = 6;
	iequals(__LINE__, i, 5);

	goto lab2;
	if (i != 0)
		i = 7;
	else
lab2:	i = 8;
	iequals(__LINE__, i, 8);

	/* The tested expr is a sequence point. */
	i = ivalue(8);
	if (i == 0) { complain(__LINE__); if ( i > 0 ) complain(__LINE__); }
	else	iequals(__LINE__, i, 8);
	}

/*
 * 3.6.4.2 - The 'switch' statement
 */
static char carray[] = { 0, 1, 2, 3};
static short sarray[] = { 0, 1, 2, 3};
static long larray[] = { 0xff00000, 0xff10000, 0xff20000, 3};
static void c3_6_4_2()
	{
#if (ANSI || V7)
	enum e {zero, one, two, three} e;
#endif
	char c;
	int i, sum, flag;
	unsigned u;
	long l;
	short s;

#if ANSI
	{
	int a = 1;

	/* syntax does not require a block, only a stmt */
	switch (a) default: switch (a+1) default: a = 9;
	iequals(__LINE__, a, 9);
	switch(a) ;	/* do-nothing */
	}
#endif

	flag = sum = 0;
	for (i = 0; i < 10; ++i) 
		{
		switch(i)
			{
inswitch:
			case 0:
			case 9:
				sum += 1;
				/* falls through */
			case 1:
			case 8:
				sum += 2;
				break;
			default:
				sum += 4;
			}
		if (flag)
			break;
		}


																						/* 3.6.4.2 (cont.) */
	if (flag)
		/* gets here via goto below */
		iequals(__LINE__, sum, 37);
	else
		{
		iequals(__LINE__, sum, 34);
		/* check that switch control works on obscure entry */
		flag = 1;
		goto inswitch;
		}

	/* nested switch */
	for (i = 0; i < 2; ++i)
		switch(i)
			{
			default :
				switch(i)
					{
					case 0:
						sum = 0;
						break;
					case 1 :
						sum += 1;
						break;
					}
				break;
			case 11:
				sum = 123;
			}
	iequals(__LINE__, sum, 1);

	/* the tested expr is a sequence point */
	i = ivalue(9);
	switch (i++)
		{
	case 9:
		iequals(__LINE__, i, 10);
		break;
	default:
		complain(__LINE__);
		}








																						/* 3.6.4.2 (cont.) */
	/* switch with no match */
	switch(sum)
		{
		case 0:
		case 2:
		case 3:
			sum = 123;
			break;
		}
	iequals(__LINE__, sum, 1);

	/* unsigned should work correctly at the zero crossing */
	for (u = (unsigned)-1, sum = 0; u != 2; ++u)
		switch (u)
			{
			case -1:	/* promoted to unsigned */
				sum += 1;
				break;
			case 0:
				sum += 2;
				break;
			case 1:
				sum += 4;
				break;
			default:
				complain(__LINE__);
			}
	iequals(__LINE__, sum, 7);





















																						/* 3.6.4.2 (cont.) */
#if ANSI
	/* ANSI allows all integral types to be used in a switch */
	for (i = 0, sum = 0; i <= 3; ++i)
		{
		switch (carray[i])
			{
			case 0:
				sum += 1;
				break;
			case 1:
				sum += 2;
				break;
			case 2:
				sum += 4;
				break;
			case 3:
				sum += 8;
				break;
			default:
				complain(__LINE__);
			}
		}
	iequals(__LINE__, sum, 15);

	for (i = 0, sum = 0; i <= 3; ++i)
		{
		switch (sarray[i])
			{
			case 0:
				sum += 1;
				break;
			case 1:
				sum += 2;
				break;
			case 2:
				sum += 4;
				break;
			case 3:
				sum += 8;
				break;
			default:
				complain(__LINE__);
			}
		}
	iequals(__LINE__, sum, 15);




																						/* 3.6.4.2 (cont.) */
	for (i = 0, sum = 0; i <= 3; ++i)
		{
		switch (larray[i])
			{
			case 0xff00000:
				sum += 1;
				break;
			case 0xff10000:
				sum += 2;
				break;
			case 0xff20000:
				sum += 4;
				break;
			case 3:	/* must convert to long */
				sum += 8;
				break;
			default:
				complain(__LINE__);
			}
		}
	iequals(__LINE__, sum, 15);
	for (e = (enum e)(i = 0), sum = 0; i <= 3; ++i, e = (enum e)i)
		{
		switch (e)
			{
			case zero:
				sum += 1;
				break;
			case one:
				sum += 2;
				break;
			case two:
				sum += 4;
				break;
			case three:
				sum += 8;
				break;
			default:
				complain(__LINE__);
			}
		}
	iequals(__LINE__, sum, 15);
#endif /* ANSI */
	} /* end c3_6_4_2 */





/*
 * 3.6.5  - Iteration statements
 */

/*
 * 3.6.5.1 - The 'while' statement
 */
static void c3_6_5_1()
	{
	int sum, i;

	/* the controlling expr is of scalar type */
	{ float v=0;		while (v) { complain(__LINE__); break; } }
	{ int (*v)()=0;		while (v) { complain(__LINE__); break; } }
	{ char *v=0;		while (v) { complain(__LINE__); break; } }
	{ struct unk *v=0;	while (v) { complain(__LINE__); break; } }
	{ enum {z} v=0;		while (v) { complain(__LINE__); break; } }
	{ char v=0;			while (v) { complain(__LINE__); break; } }
	{ unsigned v=0;		while (v) { complain(__LINE__); break; } }

	i = 10;
	sum = 0;
	/* make sure the bounds are computed each time through */
	while (i)
		{
		sum += i;
		if (--i == 5)
			i = 0;
		}
	iequals(__LINE__, sum, 40); 

	/* zero execution loop */
	sum = 0;
	while (i)
		sum = 99;
	iequals(__LINE__, sum, 0);

	/* the controlling expr is a sequence point */
	i = ivalue(1);
	while ( i -- )
		iequals(__LINE__, i, 0);
	}








/*
 * 3.6.5.2 - The 'do' statement
 */
static void c3_6_5_2()
	{
	int sum, i;

	/* the controlling expr is of scalar type */
	{ float v=0;		do { checkthat(__LINE__, v == 0); } while (v); }
	{ int (*v)()=0;		do { checkthat(__LINE__, v == 0); } while (v); }
	{ char *v=0;		do { checkthat(__LINE__, v == 0); } while (v); }
	{ struct unk *v=0;	do { checkthat(__LINE__, v == 0); } while (v); }
	{ enum {z} v=0;		do { checkthat(__LINE__, v == 0); } while (v); }
	{ char v=0;			do { checkthat(__LINE__, v == 0); } while (v); }
	{ unsigned v=0;		do { checkthat(__LINE__, v == 0); } while (v); }
	i = 10;
	sum = 0;
	/* make sure the bounds are computed each time through */
	do 
		{
		sum += i;
		if (--i == 5)
			i = 0;
		} while(i);
	iequals(__LINE__, sum, 40); 

	/* guarantee one execution of the loop */
	sum = 0;
	do
		{
		i = 0;
		sum ++;
		} while(i);
	iequals(__LINE__, sum, 1);

	/* the controlling expr is a sequence point */
	i = ivalue(1);
	do { } while (i-- == 1); iequals(__LINE__, i, -1);
	}











/*
 * 3.6.5.3 - The 'for' loop.
 */
static void c3_6_5_3()
	{
	int i, j;

	/* check that the for loop behaves the same as its decomposition */
	for (i = 0, j = 0; i < 10; ++i)
		j += i;
	iequals(__LINE__, j, 45);

	i = 0; j = 0;
	while(i < 10)
		{
		j += i;
		++i;
		}
	iequals(__LINE__, j, 45);

	/* any of the three fields can be left out */
	i = 0; j = 0;
	for (; i < 10; ++i)
		j += i;
	iequals(__LINE__, j, 45);

	for (i = 0, j = 0; ; (void)++i)
		{
		j += i;
		if (i >= 10)
			break;
		}
	iequals(__LINE__, j, 55);

	for (i = 0, (void)(j = 0); i < 10;)
		{
		j += i;
		++i;
		}
	iequals(__LINE__, j, 45);

	/* a for loop can be jumped into */
	i = 5, j = 0;
	goto forloop;
	for (i = 0, j = 0; i < 10; ++i)
		forloop: j += i;
	iequals(__LINE__, j, 35);



	/* each 'for' line expr is a sequence point */
	for (i = ivalue(1); i-- ; )
		iequals(__LINE__, i, 0);
	} /* end 3.6.5.3 */


/*
 * 3.6.6 - Jump statements
 */

/*
 * 3.6.6.1  - The goto statement
 */
static void c3_6_6_1()
	{
	goto label;
	complain(__LINE__);
		{
		complain(__LINE__);
		label:;
		goto label2;
		complain(__LINE__);
		}
	complain(__LINE__);
	label2: ;
	}
























/*
 * 3.6.6.2 - The 'continue' statement
 */
static void c3_6_6_2()
	{
	int i, j, k;
	/* check them out in for/while/do loops */
	/* a nested example ... */
	i = ivalue(0);
	for (k = 0; k < 3; ++k)
		{
		iequals(__LINE__, i, 0);
		for (i = 0, j = 0; i < 10; ++i)
			if (i%2)
				continue;
			else
				j += i;
		iequals(__LINE__, j, 20);
		i = ivalue(0);
		}

	i = ivalue(10);
	j = ivalue(0);
	while (--i)
		if (i%2)
			continue;
		else
			j += i;
	iequals(__LINE__, j, 20);

	j = ivalue(0);
	do
		if (i%2)
			continue;
		else
			j += i;
		while (++i < 10);
	iequals(__LINE__, j, 20);
	}











/*
 * 3.6.6.3  - The 'break' statement
 */
static void c3_6_6_3()
	{
	int i, j, k;

	/* test in each of for/while/do/switch */
	/* a nested example ... */
	for (k = 0; k < 3; ++k)
		{
		j = 0;
		for (i = 0; i < 10; ++i)
			if (i == 4)
				break;
			else
				j += i;
		iequals(__LINE__, j, 6);
		}
	iequals(__LINE__, k, 3);

	while (i--)
		if (i == 1)
			break;
		else
			--j;
	iequals(__LINE__, j, 4);

	do
		if (--j == 3)
			break;
		else
			++i;
		while (j);
	iequals(__LINE__, i, 1);

	switch(i)
		{
		case 1:
			break;
		default:
			i = 99;
		}
	iequals(__LINE__, i, 1);
	}





/*
 * 3.6.6.4  - The 'return' statement
 */
double fc3_664() { return 99; }	/* must convert */

static void c3_6_6_4()
	{
	int i;

	dequals(__LINE__, fc3_664(), 99.);
	i = ivalue(0);
	if (!i)
		return;
	complain(__LINE__);

	/* the expression on the return stmt is a sequence point, */
	/* but so too is any means of examining its side-effects */
	}

#else /* if SKIP36 */

void c3_6() { pr_skip("c3_6: SKIPPED ENTIRELY\n"); }
#endif /* SKIP36 */

