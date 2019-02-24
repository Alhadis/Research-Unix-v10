/* Suite++: The Plum Hall Validation Suite for C++
 * VERSION: 1
 * AND      The Plum Hall Validation Suite for C
 * VERSION: 4
 * Unpublished copyright (c) 1991, Plum Hall Inc (Editor)
 * DATE: 1993-01-01
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 * UTIL - various utility routines
 */
#include "defs.h"
#ifndef FREESTANDING
#include <stdio.h>
#endif

#define BUFSIZE 256

void setzero();
void setremark();
void trace PARMS((const char *msg, int line));

long Nerrs = 0;
long Nskips = 0;
long Nremarks = 0;
long Nsuccess = 0;
int Debug = FALSE;
char *Filename = 0;
char last_char_printed = 0;
static char details[BUFSIZE] = {0};
static int trace_msg_len = 0;
static char trace_routine[20] = "";
static char trace_filename[BUFSIZE] = "";
static int remarks = FALSE;
static int first = 1;

#include "clib.h"

/*
 * STR_REV - reverse a string
 */
char *str_rev FN_DEF1(s,
	char *s)
	{
	char c;
	char *p, *q;

	for (p = s, q = s + str_len(s) - 1; p < q; ++p, --q)
		{
		c = *p;
		*p = *q;
		*q = c;
		}
	return s;
	}

/*
 * IABS - absolute value of an integer
 */
int iabs FN_DEF1(i,
	int i)
	{
	return (i >= 0) ? i : -i;
	}

/*
 * DABS - absolute value of a double
 */
double dabs FN_DEF1(d,
	double d)
	{
	return (d >= 0.0) ? d : -d;
	}

/*
 * DPOWU - returns a double d raised to the power of unsigned u
 */
double dpowu FN_DEF2(d, u,
	double d,
	unsigned u)
	{
	double p = 1.0;

	while (u-- > 0)
		p *= d;
	return p;
	}

/*
 * ULTOSTR - convert unsigned long u to a string in base b, return the
 * address of the null terminator
 */
char *ultostr FN_DEF3(s, u, b,
	char *s,
	ULONG u,
	unsigned b)
	{
	char *p = s;

	do
		{
		if ((*p = u % b) < 10)
			*p += '0';
		else
			*p += 'A' - 10;
		++p;
		}
	while ((u /= b) != 0);
	*p = '\0';
	str_rev(s);
	return p;
	}

/*
 * LTOSTR - convert long i to a string in base b
 */
char *ltostr FN_DEF3(s, i, b,
	char *s,
	long i,
	unsigned b)
	{
	if (i < 0)
		{
		*s++ = '-';
		i = -i;
		}
	return ultostr(s, i, b);
	}

#define utostr(s, u, b) ultostr((s), (ULONG)(u), (b))

#define itostr(s, i, b) ltostr((s), (long)(i), (b))

#define ULONG_DIG 64

/*
 * PR_UL - print unsigned long u as a string in base b
 */
void pr_ul FN_DEF2(lu, b,
	ULONG lu,
	unsigned b)
	{
	char buf[ULONG_DIG + 1];

	ultostr(buf, lu, b);
	pr_ok(buf);
	}

/*
 * PR_L - print long li as a string in base b
 */
void pr_l FN_DEF2(li, b,
	long li,
	unsigned b)
	{
	char buf[ULONG_DIG + 2];

	ltostr(buf, li, b);
	pr_ok(buf);
	}

#define pr_i(i, b) pr_l((long)(i), (b))

/*
 * ENTRY_FN - in Debug mode, record entry to a function
 */
void entry_fn FN_DEF1(s,
	const char *s)
	{
	if (Debug)
		{
		if (last_char_printed != '\n')
			pr_ok("\n");
		pr_ok("ENTERING: ");
		pr_ok(s);
		pr_ok("\n");
		}
	}

/*
 * ERRMSG - print and tabulate each message
 */
static void errmsg FN_DEF2(msg, line,
	char *msg,
	int line)
	{
	if (first)
		{
		setzero();
		setremark();
		first = 0;
		}
	if (trace_msg_len != 0 )
		{
		if (last_char_printed != '\n')
			pr_ok("\n");
		trace_msg_len = 0;
		str_cpye(trace_routine, "");
		}
	if (line > 0 || remarks)
		{
		pr_ok(line > 0 ? "ERROR" : "REMARK");
		pr_ok(" in ");
		pr_ok(Filename);
		pr_ok(" at line ");
		pr_i(iabs(line), 10);
		pr_ok(msg);
		pr_ok("\n");
		}
	if (line > 0)
		++Nerrs;
	else
		++Nremarks;
	}

/*
 * IEQUALS - 'int' equality check.  If val1 != val2, then report an error.
 */
int iequals FN_DEF3(line, val1, val2,
	int line,
	int val1,
	int val2)
	{
	char *p;

	if (val1 != val2)
		{
		p = str_cpye(details, ": (");
		p = itostr(p, val1, 10);
		p = str_cpye(p, ") != (");
		p = itostr(p, val2, 10);
		p = str_cpye(p, ")");
		errmsg(details, line);
		return 0;
		}
	else
		trace("iequals", line);
	return 1;
	}

/*
 * INOTEQUALS - 'int' non-equality check.  If val1 == val2, then
 * report an error.
 */
int inotequals FN_DEF3(line, val1, val2,
	int line,
	int val1,
	int val2)
	{
	char *p;

	if (val1 == val2)
		{
		p = str_cpye(details, ": (");
		p = itostr(p, val1, 10);
		p = str_cpye(p, ") == (");
		p = itostr(p, val2, 10);
		p = str_cpye(p, ")");
		errmsg(details, line);
		return 0;
		}
	else
		trace("inotequals", line);
	return 1;
	}

/*
 * LEQUALS - 'long' equality check.  If val1 != val2, then
 * report an error.
 */
int lequals FN_DEF3(line, val1, val2,
	int line,
	long val1,
	long val2)
	{
	char *p;

	if (val1 != val2)
		{
		p = str_cpye(details, ": (");
		p = ltostr(p, val1, 10);
		p = str_cpye(p, ") != (");
		p = ltostr(p, val2, 10);
		p = str_cpye(p, ")");
		errmsg(details, line);
		return 0;
		}
	else
		trace("lequals", line);
	return 1;
	}

/*
 * ULEQUALS - 'unsigned long' equality check.  If val1 != val2, then
 * report an error.
 */
int ulequals FN_DEF3(line, val1, val2,
	int line,
	ULONG val1,
	ULONG val2)
	{
	char *p;

	if (val1 != val2)
		{
		p = str_cpye(details, ": (");
		p = ultostr(p, val1, 10);
		p = str_cpye(p, ") != (");
		p = ultostr(p, val2, 10);
		p = str_cpye(p, ")");
		errmsg(details, line);
		return 0;
		}
	else
		trace("ulequals", line);
	return 1;
	}

/*
 * STEQUALS - string equality.
 */
int stequals FN_DEF3(line, val1, val2,
	int line,
	const char *val1,
	const char *val2)
	{
	char *p;

	if (str_cmp(val1, val2))
		{
		p = str_cpye(details, ": \"");
		p = str_cpye(p, val1);
		p = str_cpye(p, "\" != \"");
		p = str_cpye(p, val2);
		p = str_cpye(p, "\"");
		errmsg(details, line);
		return 0;
		}
	else
		trace("stequals", line);
	return 1;
	}

/*
 * COMPLAIN - unconditional failure.
 */
int complain FN_DEF1(line,
	int line)
	{
	errmsg("", line);
	return 0;
	}

/*
 * ARBHEX - convert an arbitrary byte-sequence into hex codes
 */
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#define NIBBLES_PER_BYTE ((CHAR_BIT + 3) / 4)

char *arbhex FN_DEF3(str, p, n,
	char *str,
	const char *p,
	int n)
	{
	int i, nib, hex_dig;
	static char codes[] = "0123456789ABCDEF";

	for (i = 0; i < n; ++i, ++p)
		for (nib = NIBBLES_PER_BYTE - 1; nib >= 0; --nib)
			{
			hex_dig = (*p & (unsigned)(0xF << (nib * 4))) >> (nib * 4);
			*str++ = codes[hex_dig];
			}
	*str = '\0';
	return str;
	}

/*
 * AEQUALS - 'address' equality check.  If val1 != val2, then
 * report an error.
 */
int aequals FN_DEF3(line, val1, val2,
	int line,
	const void *val1,
	const void *val2)
	{
#ifdef FREESTANDING
	char *p;
#endif

	if (val1 != val2)
		{
#ifdef FREESTANDING
		p = str_cpye(details, ": (");
		p = arbhex(p, (const char *)&val1, (int)sizeof(val1));
		p = str_cpye(p, ") != (");
		p = arbhex(p, (const char *)&val2, (int)sizeof(val2));
		p = str_cpye(p, ")");
#else
		sprintf(details, ": (%p) != (%p)",  val1, val2);
#endif
		errmsg(details, line);
		return 0;
		}
	else
		trace("aequals", line);
	return 1;
	}

/*
 * FAEQUALS - function address equality check.  If val1 != val2, then
 * report an error. The address of a function is not necessarily the same
 * size/type as the address of data.
 */
int faequals FN_DEF3(line, val1, val2,
	int line,
	int (*val1)(),
	int (*val2)())
	{
	char *p;

	if (val1 != val2)
		{
		p = str_cpye(details, ": (");
		p = arbhex(p, (char *)&val1, (int)sizeof(val1));
		p = str_cpye(p, ") != (");
		p = arbhex(p, (char *)&val2, (int)sizeof(val2));
		p = str_cpye(p, ")");
		errmsg(details, line);
		return 0;
		}
	else
		trace("faequals", line);
	return 1;
	}

/*
 * DEQUALS - 'double' equality check.  If val1 != val2, then
 * report an error. This is computed using an equality approximation
 * that verifies that the two numbers are equal to R digits whenever
 *
 *    |x - y|    1    1-R
 *    ------- <= - 10
 *      |x|      2
 *
 * DIGITS_MAX  is defined in defs.h
 */
double Delta = 0.0;

int dequals FN_DEF3(line, val1, val2,
	int line,
	double val1,
	double val2)
	{
#ifdef FREESTANDING
	char *p;
#endif
	double *pd;

 	if (line < 0)	/* no "remarks" for floating point */
 		return 0;
	if (Delta == 0.0)
		Delta = 0.5 / dpowu(10.0, DIGITS_MAX - 1); 
	if (val1 == val2)
		{
		trace("dequals", line);
		return 1;
		}
	pd = &val1;
	if (val1 == 0.0)
		pd = &val2;

	/* special cases to handle zero against very small numbers */
	if (dabs(val1) == 0.0 && dabs(val2) < Delta)
		;
	else if (dabs(val2) == 0.0 && dabs(val1) < Delta)
		;
	else if ((dabs(val1 - val2) / dabs(*pd)) > Delta)
		{
#ifdef FREESTANDING
		p = str_cpye(details, ": (");
		p = arbhex(p, (const char *)&val1, (int)sizeof(val1));
		p = str_cpye(p, ") != (");
		p = arbhex(p, (const char *)&val2, (int)sizeof(val2));
		p = str_cpye(p, ")");
#else
		sprintf(details, ": (%.*G) != (%.*G)",
			DIGITS_MAX + 2, val1, DIGITS_MAX + 2, val2);
#endif
		errmsg(details, line);
		return 0;
		}
	trace("dequals", line);
	return 1;
	}

#if ANSI
/*
 * LDEQUALS - Long double equality ... more of the same.
 */
#define ldabs(ld) ((ld) < 0.0 ? -(ld) : (ld))

long double LDelta = 0.0;

int ldequals FN_DEF3(line, val1, val2,
	int line,
	long double val1,
	long double val2)
	{
#ifdef FREESTANDING
	char *p;
#endif
	long double *pd;

 	if (line < 0)	/* no "remarks" for floating point */
 		return 0;
	if (LDelta == 0.0)
		{
		LDelta = 0.5L / dpowu(10.0, LDIGITS_MAX - 1); 
		}
	if (val1 == val2)
		{
		trace("ldequals", line);
		return 1;
		}
	pd = &val1;
	if (val1 == 0.0)
		pd = &val2;

	/* special cases to handle zero against very small numbers */
	if (ldabs(val1) == 0.0 && ldabs(val2) < LDelta)
		;
	else if (ldabs(val2) == 0.0 && ldabs(val1) < LDelta)
		;
	else if ((ldabs(val1 - val2) / ldabs(*pd)) > LDelta)
		{
#ifdef FREESTANDING
		p = str_cpye(details, ": (");
		p = arbhex(p, (char *)&val1, (int)sizeof(val1));
		p = str_cpye(p, ") != (");
		p = arbhex(p, (char *)&val2, (int)sizeof(val2));
		p = str_cpye(p, ")");
#else
		sprintf(details, ": (%.*LE) != (%.*LE)",
			LDIGITS_MAX + 2, val1, LDIGITS_MAX + 2, val2);
#endif
		errmsg(details, line);
		return 0;
		}
	trace("ldequals", line);
	return 1;
	}
#endif

/*
 * FEQUALS - same as DEQUALS, but to FDIGITS_MAX instead of DIGITS_MAX.
 */
double FDelta = 0.0;

int fequals FN_DEF3(line, in1, in2,
	int line,
	double in1,
	double in2)
	{
#ifdef FREESTANDING
	char *p;
#endif
	float *pf;
	float val1 = in1;
	float val2 = in2;

 	if (line < 0)	/* no "remarks" for floating point */
 		return 0;
	if (FDelta == 0.0)
		FDelta = 0.5 / dpowu(10.0, FDIGITS_MAX - 1); 
	if (val1 == val2)
		{
		trace("fequals", line);
		return 1;
		}
	pf = &val1;
	if (val1 == 0.0)
		pf = &val2;

	/* special cases to handle zero against very small numbers */
	if (dabs(val1) == 0.0 && dabs(val2) < FDelta)
		;
	else if (dabs(val2) == 0.0 && dabs(val1) < FDelta)
		;
	else if ((dabs(val1 - val2) / dabs(*pf)) > FDelta)
		{
#ifdef FREESTANDING
		p = str_cpye(details, ": (");
		p = arbhex(p, (char *)&val1, (int)sizeof(val1));
		p = str_cpye(p, ") != (");
		p = arbhex(p, (char *)&val2, (int)sizeof(val2));
		p = str_cpye(p, ")");
#else
		sprintf(details, ": (%.*G) != (%.*G)",
			FDIGITS_MAX+2, val1, FDIGITS_MAX+2, val2);
#endif
		errmsg(details, line);
		return 0;
		}
	trace("fequals", line);
	return 1;
	}

/*
 * CHECKTHAT - simple condition check.  If val1 == 0, then
 * report an error.
 */
int checkthat FN_DEF2(line, cond,
	int line,
	int cond)
	{
	if (!cond)
		{
		errmsg("", line);
		return 0;
		}
	else
		trace("checkthat", line);
	return 1;
	}

/*
 * VALUE - the value routines are used to defeat value propagation in
 * optimizing compilers.  We want to make sure that we are testing what we
 * think we are testing, not what the compiler transformed it to.
 * 1988: Some compilers "open-code" all small functions.  Now we have to hide
 * the constants still further.
 */
static int Zero = 0;	/* See  setzero()  below */

int ivalue FN_DEF1(i,
	int i)
	{
	return i + Zero;
	}

long lvalue FN_DEF1(i,
	long i)
	{
	return i + Zero;
	}

double dvalue FN_DEF1(i,
	double i)
	{
	return i + Zero;
	}

float fvalue FN_DEF1(i,
	double i)
	{
	return (float)i + Zero;
	}

generic_ptr avalue FN_DEF1(i,
	generic_ptr i)
	{
 	if (i == 0)
 		return 0;
 	else
 		return (char *)i + Zero;
	}

/*
 * SCHECK - check both values and side effects.
 */
int Side = 0;

int scheck FN_DEF4(line, val1, se, val2,
	int line,
	int val1,
	int se,
	int val2)
	{
	int status = 1;

	if (Side != se)
		{
		errmsg(": incorrect side effect", line);
		status = 0;
		}
	else
		trace("scheck", line);
	Side = 0;
	return status == 1 && iequals(line, val1, val2);
	}

/*
 * DO_NOTHING - this is also intended to defeat optimizers by passing
 * the addresses of variables for which we want to stop any value propagation.
 */
int do_nothing FN_DEF1V(p,
	void *p)
	{
	*(char *)p = *((char *)p + Zero);
	return 0;
	}

/*
 * REPORT - summary report at end of testing.
 */
int report FN_DEF1(program,
	const char *program)
	{
	Nerrs += Nskips;
	return Nerrs > MAX_INT ? MAX_INT : (int)Nerrs;
	}

/*
 * DBPRINT - print the message if the Debug flag is on.
 */
void dbprint FN_DEF1(s,
	const char *s)
	{
	if (Debug)
		{
		pr_ok("***DEBUG***");
		pr_ok(s);
		}
	}

/*
 * TRACE - print a line-number trace for debugging
 * Also count successful tests.
 */
void trace FN_DEF2(routine, line,
	const char *routine,
	int line)
	{
	char buf[10];

	if (first)
		{
		setzero();
		setremark();
		first = 0;
		}
	if (Debug && line > 0)
		{
		if (str_cmp(trace_routine, routine) != 0 ||
			str_cmp(trace_filename, Filename) != 0 ||
			last_char_printed == '\n' ||
			trace_msg_len > 60)
			{
			if (last_char_printed != '\n')
				pr_ok("\n");
			pr_ok("TRACE: ");
			pr_ok(routine);
			pr_ok(" at ");
			pr_ok(Filename);
			pr_ok(" ");
			str_cpye(trace_routine, routine);
			str_cpye(trace_filename, Filename);
			trace_msg_len = 11 + str_len(routine) + str_len(Filename);
			}
		str_cpye(itostr(buf, iabs(line), 10), " ");
		pr_ok(buf);
		trace_msg_len += str_len(buf);
		}
	if (line > 0)
		++Nsuccess; /* don't count remarks in score */
	}

/*
 * PR_SKIP - printf a text line in SKIP situation 
 */
void pr_skip FN_DEF1(s,
	const char *s)
	{
	++Nskips;
	if (last_char_printed != '\n')
		pr_ok("\n"); 
	pr_ok("SKIPPED: "); 
	pr_ok(s);
	pr_ok("\n");
	}

/*
 * PR_ERR - printf a text line in error situation 
 */
void pr_err FN_DEF1(s,
	const char *s)
	{
	++Nerrs;
	pr_ok(s);
	}
/*
 * SETREMARK - determine whether "remark" messages should be printed
 */
void setremark()
	{
#ifdef SUITE_REMARK
	remarks = TRUE;
#else
	remarks = FALSE;
#endif
	}

/*
 * ignore - appear to use a value, but don't
 * (Purpose: shut up noisy "value not used" messages)
 */
void ignore FN_DEF1(addr,
	const void *addr)
	{
	if (Zero)
		pr_ok((char *)addr);
	}

#include "sdutil.h"
