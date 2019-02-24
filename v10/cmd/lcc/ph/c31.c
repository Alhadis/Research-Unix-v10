/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP31
/*
 * 3.1 - Lexical Elements
 */
#include "defs.h"
#if ANSI
#include <stddef.h>
#endif
/*
 * 3.1.1 - Keywords. These are tested throughout.
 */
static void c3_1_1(){}

/*
 * 3.1.2 - Identifiers.  A few checks for valid identifiers.
 */
static void c3_1_2()
	 {
	 int abcdefghijklmnopqrstuvwxyz = 1;
	 int ABCDEFGHIJKLMNOPQRSTUVWXYZ = 2;
	 int a23456789012345678901234567890a = 3;
#if ANSI
	 int a23456789012345678901234567890b = 4;

	/* ANSI requires 31 characters of significance */
	inotequals(__LINE__,
		a23456789012345678901234567890b,
		a23456789012345678901234567890a);
#endif

	/* upper and lower cases are significant */
	inotequals(__LINE__,
		abcdefghijklmnopqrstuvwxyz,
		ABCDEFGHIJKLMNOPQRSTUVWXYZ);

#if !(LONG_IDENTIFIERS || ANSI)
	/* K&R/V7 declares that significance is ONLY to 8 characters */
	iequals(__LINE__, a23456789, a2345678);
#endif
	}


/*
 * 3.1.2.1 - Scopes of Identifiers
 */
#if ANSI && HAS_PROTOTYPES
/* function prototype scope */
int nofunc(int i);
#endif

/* file scope */
static int i = 17;
enum {AA, BB};
#if ANSI
static int f(enum {BB, AA} n) { iequals(__LINE__, BB, 0); return BB; }
#endif /* ANSI */
struct tag1 { int stuff; };	/* at file level */
static void c3_1_2_1(i)
	int i; /*  block scope (set to 19 from call above) */
	{
		{
		/* block scope */
		int i = ivalue(18);
		iequals(__LINE__, i, 18);
		}
	iequals(__LINE__, i, 19);
	iequals(__LINE__, BB, 1);
#if ANSI8703
	{
	enum { A, B = A+7 } e = A;
	int i;
	static struct tag1 { struct tag1 *next; } s1 = {&s1};	/* new scope */
	iequals(__LINE__, f(0), 0);
	checkthat(__LINE__, s1.next == &s1);
		{
#if ANSI8809
		void nonexist(union tag2 *junk);	/* tag2, in prototype scope, disappears */
#endif
		i = sizeof(struct tag2 { int stuff; });	/* declares tag2 */
			{
			struct tag2 s = {1};
			checkthat(__LINE__, s.stuff == 1);
			}
		} 
	} 
#endif 
#if ANSI 
	goto a; 
	{ iequals(__LINE__, 1, 0); /* should not be reached */ a: ; } 
#endif 
	} 

/*
 * 3.1.2.2 - Linkages of identifiers
 */
static	int s11 = 11;		/* "lex 1st decl w file scope" says "static" --> internal */
		int g13 = 13;		/* no "static" -->  external */
        int g13;			/* no sc --> external */
extern int s11;				/* same linkage as previous --> internal */
extern int g13;				/* same linkage as previous --> external */
static int s16();			/* internal linkage */
       int s16();			/* no sc (on fn) --> as though "extern" (same as previous) */

static void c3_1_2_2()
	{
	int j9 = ivalue(9);		/* no linkage */
	extern int s11;			/* internal linkage (already defined) */
	static int e12 = 12;	/* no linkage */
	extern int g13;			/* external linkage (already defined) */
	extern int g14;			/* external linkage (forward reference) */
	extern int e15;			/* external linkage (defined in main1.c) */

	/* this is not referenced and shouldn't require any linkage */
	extern int notdefinedanywhere;
	iequals(__LINE__, j9, 9);
	iequals(__LINE__, s11, 11);
	iequals(__LINE__, g13, 13);
	iequals(__LINE__, g14, 14);
	iequals(__LINE__, e15, 15);
	iequals(__LINE__, s16(), 16);

	/* this should have internal linkage -- it is erroneous to link
	 * to the static variable below.
	 */
	iequals(__LINE__, e12, 12);

	/* scope testing of 'static i' from c3_1_2_1 */
	iequals(__LINE__, i, 17);

	/* test scoping into and out of include files */
		{
		int i = 9;
#include "c31sco.h"
		iequals(__LINE__, i, 10);
		}
	iequals(__LINE__, i, 17);
	}

static	int e12 = 0;
		int g14 = 14;

static int s16() { return ivalue(16); }
/*
 * 3.1.2.3 - Name spaces of identifiers.
 * K&R and V7 have 2 (variables/labels and tags/members);
 * later UNIX (set TAG_AND_MEMB_SPACES to 1) has 3 (var/labels, tags, members);
 * ANSI has 4 (variables, labels, tags, members).
 */
static void c3_1_2_3()
	{
	int ident;

#if (TAG_AND_MEMB_SPACES || ANSI)
	struct ident
		{
		int xxx;
		int ident; /* members and variables in different spaces */
		}p;
#endif
	struct a
		{
		int ident2;
		} pa;
#if (ANSI || UNIQ_MEMB_NAMES)
	struct b
		{
		long ident2;
		} pb;
#endif

	ident = ivalue(7);
	pa.ident2 = ivalue(8);
#if ANSI
	{
	struct x { int x; } x = { 6 };
	iequals(__LINE__, x.x, ivalue(6));
	}
	if (ident)
		goto ident;
#else
	if (ident)
		goto ident1;
#endif

	/* this statement should never be reached */
	complain(__LINE__);






																						/* 3.1.2.3 (cont.) */
#if ANSI
	ident:
#else
	ident1:
#endif
	iequals(__LINE__, pa.ident2, 8);

#if (TAG_AND_MEMB_SPACES || ANSI)
	p.ident = ivalue(9);
	iequals(__LINE__, p.ident, 9);
#endif
	}





































/*
 * 3.1.2.4 - Storage classes of objects.
 */
static void c3_1_2_4(arg)
	int arg;
	{
	static int i = 0;
	int j = 17;

	/* make sure static items are still alive at each invocation */
	if (arg == 0)
		{
		i = 13;
		/* recursion ensures that stack frames (if any) don't coincide */
		c3_1_2_4(1);
		}
	else
		{
		iequals(__LINE__, i, 13);
		goto label;
		}

	for (i = 1; i <= 2; ++i)
		{
		int j = i + 1;

		iequals(__LINE__, j, i + 1);	/* auto var of inner block */
		}
		{
		int j = 14;
label:
		j = 15;
		}
	
	iequals(__LINE__, j, 17);

	/* local items might hold their values, but it is only coincidence */
	}












/*
 * 3.1.2.5 - Types
 */
char a_incomplete[]; char a_incomplete[3] = "ab";
static void c3_1_2_5()
	{
	static char Carray[] = 
		{
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
		'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
		'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
		',', '.', '/', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
		'-', '=', '|', '<', '>', '?', '!',      '#',      '%', '^', '&',
		     '"', '~', '[', ']', '{', '}', '*', '(', ')', '_', '+', '\\',
		'\'', 
		};	/* dollar-sign, accent-grave, and at-sign are not in C source set */

#if ANSI
	signed char sc;
	signed short ss;
	signed int si;
	signed long sl;
	long double ld;
	struct s_incomplete *psinc;
	struct s_incomplete { int i; } sinc = {3};
#endif
	char c;
	short int s;
	int i;
	long int li;
	float f;
	double d;
	unsigned int ui;


	UCHAR uc;
	USHORT us;
	ULONG int ul;

	checkthat(__LINE__, sizeof(a_incomplete) == 3);
	checkthat(__LINE__, sizeof(c) <= sizeof(s));
	checkthat(__LINE__, sizeof(s) <= sizeof(i));
	checkthat(__LINE__, sizeof(i) <= sizeof(li));
	checkthat(__LINE__, sizeof(f) <= sizeof(d));
	checkthat(__LINE__, sizeof(uc) == sizeof(c));
	checkthat(__LINE__, sizeof(us) == sizeof(s));
	checkthat(__LINE__, sizeof(ui) == sizeof(i));
	checkthat(__LINE__, sizeof(ul) == sizeof(li));

#if ANSI																				/* 3.1.2.5 (cont.) */
	psinc = &sinc;
	iequals(__LINE__, psinc->i, 3);
	checkthat(__LINE__, sizeof(d) <= sizeof(ld));
	checkthat(__LINE__, sizeof(uc) == sizeof(sc));
	checkthat(__LINE__, sizeof(us) == sizeof(ss));
	checkthat(__LINE__, sizeof(ui) == sizeof(si));
	checkthat(__LINE__, sizeof(ul) == sizeof(sl));
#endif
	
	/* all source char constants are positive */
	for (i = 0; i < sizeof(Carray); ++i)
		checkthat(__LINE__, Carray[i] > 0);
#if ANSI
	/* <limits.h> is included in the "defs.h" for ANSI */
	checkthat(__LINE__, (int)INT_MAX == (unsigned int)INT_MAX);
	checkthat(__LINE__, (long)LONG_MAX == (unsigned long)LONG_MAX);

	checkthat(__LINE__, ULONG_MAX >= LONG_MAX);
	checkthat(__LINE__, UINT_MAX >= INT_MAX);
	checkthat(__LINE__, USHRT_MAX >= SHRT_MAX);
	ul = ULONG_MAX;
		{
		struct ints { char c; short s; int i; long l; };
		struct uints { UCHAR c; USHORT s; unsigned int i; ULONG l; };
		union { short s; unsigned short us; } u1;
		static char str2[] = "abc", str3[] = "abc";
		union { void *v2; char *s2; } u2;
		union { void *v3; char *s3; } u3;

		checkthat(__LINE__, sizeof(struct ints) == sizeof(struct uints));	/* ANSI8703 same alignments */
	
		/* ANSI8809 - "same representation and alignment" means interchangeable in args, returned values, and unions */
		u1.s = 10;
		iequals(__LINE__, abs(u1.us), ivalue(10));	/* no prototype for abs at this point, defaults */
		u2.s2 = str2, u3.s3 = str3;
		iequals(__LINE__, str_cmp(u2.v2, u3.v3), 0);	/* no prototype for str_cmp at this point, defaults */

		}
	checkthat(__LINE__, ul + ULONG_MAX > 0);	/* unsigned "can't oflo" */
	ui = UINT_MAX;
	checkthat(__LINE__, ui + UINT_MAX > 0);		/* unsigned "can't oflo" */
	us = USHRT_MAX;
	checkthat(__LINE__, us + USHRT_MAX > 0);	/* unsigned "can't oflo" */
	ldequals(__LINE__, (ul += 1), 0);	/* unsigned "can't oflo" */
	ldequals(__LINE__, (ui += 1), 0);	/* unsigned "can't oflo" */
	ldequals(__LINE__, (us += 1u), 0);	/* unsigned "can't oflo" */
#endif /* ANSI */
	}

#if ANSI8709
/*
 * 3.1.2.6 - Compatible type and composite type
 */
typedef char CHAR_COMPATIBLE;
extern struct s3126 { CHAR_COMPATIBLE i; signed int j; } s3126;	/* link with lang.c */
extern union u3126 { signed int j; CHAR_COMPATIBLE i; } u3126;	/* link with lang.c */
extern enum e3126 { E3126B=5, E3126A=4 } e3126;					/* link with lang.c */
typedef void VF();			/* no parm list */
typedef void VF_VF(VF *);	/* one parm of type VF * */
static void c3_1_2_6();
static void c3_1_2_6(VF *);
static void c3_1_2_6(VF_VF *);
static void c3_1_2_6(
	VF_VF *pf)
	{
	iequals(__LINE__, s3126.j, 2);
	iequals(__LINE__, u3126.i, 3);
	iequals(__LINE__, e3126, 5);
	checkthat(__LINE__, pf == c3_1_2_6);
	}
#endif




























/*
 * 3.1.3.1 - Floating constants
 */
static void c3_1_3_1()
	{

	/* check multiple representations for equality */
	dequals(__LINE__, 123., 123.0); 
	dequals(__LINE__, 123.0, 12.3e1); 
	dequals(__LINE__, 12.3e1, 12.3E1); 
	dequals(__LINE__, 12.3E1, 1.23E+2); 
	dequals(__LINE__, 123.e-2, 12.3e-1);
	dequals(__LINE__, .9, 9e-1);
	checkthat(__LINE__, sizeof(double) == sizeof(1.0));

#if ANSI
	/* check for existance of long double constants */
	ldequals(__LINE__, 101.L, 101.0l); 
	fequals(__LINE__, 202.f, 202.0F);
	checkthat(__LINE__, sizeof(1.F) == sizeof(float) && sizeof(1.f) == sizeof(float));
	checkthat(__LINE__, sizeof(1.L) == sizeof(long double) && sizeof(1.l) == sizeof(long double));
#endif
	}



























/*
 * 3.1.3.2 - Integer constants.
 */
static void c3_1_3_2()
	{
	/* make sure all hex digits are accepted */
	iequals(__LINE__, 0xabc, 0xABC);
	iequals(__LINE__, 0xdef, 0xDEF);

	/* equivalence checking */
	iequals(__LINE__, 456, 0x1c8);
	iequals(__LINE__, 456, 0710);

	/* size checking */
	iequals(__LINE__, sizeof(1), sizeof(int));
	iequals(__LINE__, sizeof(0xabc), sizeof(int));
	iequals(__LINE__, sizeof(0123), sizeof(int));
	iequals(__LINE__, sizeof(1L), sizeof(long));
	iequals(__LINE__, sizeof(0xabcL), sizeof(long));
	iequals(__LINE__, sizeof(0123L), sizeof(long));
	iequals(__LINE__, sizeof(1l), sizeof(long));
	iequals(__LINE__, sizeof(0xabcl), sizeof(long));
	iequals(__LINE__, sizeof(0123l), sizeof(long));
	iequals( - __LINE__, sizeof(999999999), sizeof(long));
	iequals( - __LINE__, sizeof(0x9999999), sizeof(long));
	iequals( - __LINE__, sizeof(0777777777), sizeof(long));
	/* the 3 lines above are not strict - could fit a very large int */
#if ANSI
	/* check for existence of both U and L suffixes */
	iequals(__LINE__, sizeof(1u), sizeof(unsigned int));
	iequals(__LINE__, sizeof(1U), sizeof(unsigned int));
	iequals(__LINE__, sizeof(1LU), sizeof(unsigned long));
	iequals(__LINE__, sizeof(1lU), sizeof(unsigned long));
	iequals(__LINE__, sizeof(01uL), sizeof(unsigned long));
	iequals(__LINE__, sizeof(0x1ul), sizeof(unsigned long));
#endif

/* ANSI requires that 0xffff be unsigned int (if too big for int),
 * but that 65535 be long (if too big for int).
 */
#if ANSI
	iequals(__LINE__, sizeof(0xffff), sizeof(unsigned int));
	if (MAX_UINT == 65535)
		{
		iequals(__LINE__, sizeof(65535), sizeof(long));
		iequals(__LINE__, sizeof(65535u), sizeof(unsigned int));
		iequals(__LINE__, sizeof(0xffff), sizeof(int));
		}
	else
		{
		checkthat(__LINE__, sizeof(65535) <= sizeof(long));
		iequals(__LINE__, sizeof(65535u), sizeof(unsigned int));
		iequals(__LINE__, sizeof(0xffff), sizeof(int));
		}
#endif
	}
/*
 * 3.1.3.3 - Enumeration constants.
 */
static void c3_1_3_3()
	{
#if (V7 || ANSI)
	enum color { red = -1, blue = -2, green = (MAX_INT-1-MAX_INT) /* i.e., -1 again */ };
	checkthat(__LINE__, sizeof(red) <= sizeof(int));
	iequals(__LINE__, red, ivalue(-1));
	iequals(__LINE__, blue, ivalue(-2));
	iequals(__LINE__, green, ivalue(-1));
#endif
	}





































/*
 * 3.1.3.4 - Character constants.
 */
static void c3_1_3_4()
	{
	int c;

	/* the basic set of character constants was tested above */

	/* escape sequence character constants */
	inotequals(__LINE__, '\b', 'b');
	inotequals(__LINE__, '\f', 'f');
	inotequals(__LINE__, '\n', 'n');
	inotequals(__LINE__, '\r', 'r');
	inotequals(__LINE__, '\t', 't');
	iequals(__LINE__, '\"', '"');
	iequals(__LINE__, '\\', "\\"[0]);

#if ANSI
	inotequals(__LINE__, '\a', 'a');
	inotequals(__LINE__, '\v', 'v');

	/* hex digit form */
	iequals(__LINE__, '\xa', 0xa);
	iequals(__LINE__, '\x7b', 0x7b);
	iequals(__LINE__, '\x001', 1);
#if WIDE_CHARS
#include "c3134.h"  /* to hide the following lines from non-ANSI lexical scan */
/* 	checkthat(__LINE__, sizeof(L'x') == sizeof(wchar_t)); */
/* 	iequals(__LINE__, L'a', 'a'); */
/* 	iequals(__LINE__, L'\0', 0); */
#endif

#endif

	/* octal digit form */
	iequals(__LINE__, '\0', 0);
	iequals(__LINE__, '\1', 1);
	iequals(__LINE__, '\12', 012);
	iequals(__LINE__, '\123', 0123);
	
	iequals(__LINE__, sizeof('0'), sizeof(int));








#if (SIGNED_CHAR == 1 && MAX_UCHAR == 0xff)												/* 3.1.3.4 (cont.) */
	/* character constants sign extend */
	iequals(__LINE__, '\377', -1);
#else
#if UNSIGNED_CHAR
	/* character constants do not sign extend */
	iequals(__LINE__, '\377', 255);
#endif
#endif
	}

/*
 * 3.1.4 - String literals.
 */
int distinct(s)
	char *s;
	{
	int i, j;

	for (i = 0; s[i] != '\0'; ++i)
		{
		for (j = 0; s[j] != s[i]; ++j)
			;
		if (i != j)
			return (0);		/* s[i] is dup'ed by an earlier char */
		}
	return (1);
	}
#define OLD_ESC_CHARS "\"\\\b\f\n\r\t"
#define SOURCE_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\
0123456789!#%&'()*+,-./:;<=>?[]^_{|}~"
static void c3_1_4()
	{
	static char *p = "abc";
	static char *q = OLD_ESC_CHARS;
	static char *r = "abc";
	/* check out all legal characters */
	static char *s = SOURCE_CHARS;
	static char *z = "";

	/* empty string */
	iequals(__LINE__, z[0], 0);
	
	/* check for "array of char" behaviour and trailing null */
	iequals(__LINE__, p[0], 'a');
	iequals(__LINE__, p[1], 'b');
	iequals(__LINE__, p[2], 'c');
	iequals(__LINE__, p[3], '\0');


																						/* 3.1.3.4 (cont.) */
	/* check escape sequences */
	checkthat(__LINE__, distinct(q));
	checkthat(__LINE__, distinct(s));
	iequals(__LINE__, q[0], '\"');
	iequals(__LINE__, q[1], '\\');
	iequals(__LINE__, q[2], '\b');
	iequals(__LINE__, q[3], '\f');
	iequals(__LINE__, q[4], '\n');
	iequals(__LINE__, q[5], '\r');
	iequals(__LINE__, q[6], '\t');

#if ANSI
#define NEW_ESC_CHARS "\a\v"
		{
		/* extra escape characters and string catenation */
		static char *s = NEW_ESC_CHARS;
		static char *t = "abc""def";
		static char *u = SOURCE_CHARS OLD_ESC_CHARS NEW_ESC_CHARS ;
		iequals(__LINE__, s[0], '\a');
		iequals(__LINE__, s[1], '\v');
		iequals(__LINE__, t[3], 'd');
		iequals(__LINE__, t[6], '\0');
		checkthat(__LINE__, distinct(u));
		}
#endif

#if V7	/* this changed in ANSI */
	/* check for distinctness and writability of strings */
		{
		static char *u = "xyz";
		static char *t = "xyz";
	
		u[0] = ivalue(0);
		iequals(__LINE__, u[0], 0);
		inotequals(__LINE__, t[0], 0);
		}
#endif
	/* pure numbers through escapes */
	s = "\0123";
	iequals(__LINE__, 012, s[0]);
	iequals(__LINE__, '3', s[1]);
#if ANSI
	s = "\x012""3";						/* The catenation "" is needed for ANSI8706 */
	iequals(__LINE__, 0x12, s[0]);							/* See 3.1.4 Example */
	iequals(__LINE__, '3', s[1]);
#if ANSI8706
	checkthat(__LINE__, str_cmp("\x023", "\x02""3") != 0);	/* ANSI8706 */
#endif
#endif
																						/* 3.1.4 (cont.) */
	/* strings are arrays of chars */
	iequals(__LINE__, "abcd"[0], 'a');
	iequals(__LINE__, "abcd"[1], 'b');
	iequals(__LINE__, "abcd"[2], 'c');
	iequals(__LINE__, "abcd"[3], 'd');
	iequals(__LINE__, "abcd"[4], 0);
	}

/*
 * Operators.
 * No capability requirements.
 * Each operator will be tested later (c3_3 etc.).
 */
static void c3_1_5(){}

/*
 * Punctuators.
 * No capability requirements.
 * Each use of punctuator in syntax will be tested later (c3_5, c3_6, c3_7, c3_8).
 */
static void c3_1_6(){}

/* 3.1.7 - Header names (tested throughout) */

/*
 * 3.1.8 - Preprocessing numbers
 */
#if ANSI
	#define STR(a) NXSTR(a)
	#define NXSTR(a) #a
#endif
static void c3_1_8()
	{
#if PP_CAT_AND_STR
#include "c318.h" /* to prevent non-ANSI lexical scan of ... */
/* 	#define CAT(a, b) NXCAT(a, b) */
/* 	#define NXCAT(a, b) a ## b */
/* 	iequals(__LINE__, str_cmp(STR(CAT(1., E9)), "1.E9"), 0); */
/* 	iequals(__LINE__, str_cmp(STR(CAT(1.u, 1E+)), "1.u1E+"), 0); */
/* #define E 2 */
/* 	checkthat(__LINE__, 1E1 == 10.); */
#endif
	}






/*
 * Comments - 3.1.9
 */
/* /* no nesting */
/* comment prevents preprocessing of its contents...
#include "unknown"
*/

static void c3_1_9()
	{
	int i;
	char c;
	char *s;





	i = 2; s = "/*"; i = 3; /* no comment in string literal */
	iequals(__LINE__, i, 3);
	/* comment prevents compilation of its contents...
	complain(__LINE__);
	*/
	}


























/*
 * 3.1 - Lexical elements
 */
void c3_1()
	{
	int a = 1;
	int b = 2;
	int c = 3;

	Filename = "c31.c";

	/* check for insignificance of white space */
#if ANSI
#include "c31h.h"
	Filename = "c31.c";
#else
	iequals(__LINE__, (a+b-c),  (a +	b
							/* */-
							c));
#endif

	/* a token uses the longest sequence of characters */
	iequals(__LINE__, c+++b, 5);
	iequals(__LINE__, b, 2);
	iequals(__LINE__, c, 4);

	c3_1_1();
	c3_1_2();
	c3_1_2_1(19);
	c3_1_2_2();
	c3_1_2_3();
	c3_1_2_4(0);
	c3_1_2_5();
#if ANSI8709
	c3_1_2_6(&c3_1_2_6);
#endif
	c3_1_3_1();
	c3_1_3_2();
	c3_1_3_3();
	c3_1_3_4();
	c3_1_4();
	c3_1_5();
	c3_1_6();
	c3_1_8();
	c3_1_9();
	}
#else /* if SKIP31 */

void c3_1() { pr_skip("c3_1: SKIPPED ENTIRELY\n"); }
#endif /* SKIP31 */

