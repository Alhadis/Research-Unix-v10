/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 * d49a - formatting tests
 */

#include "flags.h"
#ifndef SKIP49
#include <stdio.h>
#define LIB_TEST 1
#include "defs.h"
#if ANSI
#include <float.h>	/* to get DBL_MAX_10_EXP, which non-ANSI machine.h defines */
#else
#define remove unlink
#endif
char *save_name();

#define T_0 0		/* no data needed */
#define T_I1 1		/* int */
#define T_I2 2		/* int (negative) */
#define T_L1 3		/* long */
#define T_SI 4		/* short */
#define T_D1 5		/* double */
#define T_P 6		/* char * */
#define T_S 7		/* string */
#define T_STAR1 8	/* width as argument */
#define T_STAR2 9	/* precision as argument */
#define T_STAR3 10	/* width and precision as arguments */
#define T_C1 11		/* character */
#define T_C2 12		/* character */
#define T_F 13		/* float */
#define T_U 14		/* unsigned int */
#define T_D2 15		/* very large double */
#define T_D3 16		/* double with zero value */
#define T_D4 17		/* double with integral value */
#define T_D5 18		/* double with exponent < -4 */
#define T_D6 19		/* double with exponent == precision */
#define T_D7 20		/* double with exponent > precision */
#define T_D8 21		/* long double */
#define T_SU 22		/* unsigned short */
#define T_I0 23		/* integer arg with value of 0 */
#define T_STARD 24	/* width and precision as arguments */
#define T_STARN 25	/* width and precision with a neg number */

static int Ival1 = 77;
static int Ival2 = -77;
static long Lval1 = 65000;
static short Sval = 0x7fff;
static unsigned short Suval = 0x7fff;
static double Dval1 = 12.3456789;
static float Fval = 3.0;
static int *Pval = &Ival1;
static char *String = "this is a string\n";
static char Cval = 'a';
static unsigned int Ui = 65000;
static double Dval2 = 0.0;	/* assigned dynamically below */
static double Dval3 = 0.0;
static double Dval4 = 1.0;
static double Dval5 = 1.23e-6;
static double Dval6 = 9.876e+2;
static double Dval7 = 9.876e+3;
#if ANSI
static long double Dval8 = 98.7654L;
#endif
void fpsteq();


static struct output
	{
	int type;
	char *fmt;
	char *out;
	int line;
	} Output[] = 
		{
		/* %[-+ #][width|*][precision|*][h|l|L][diouxXfeEgGcspn%] */

		T_0,	"",					"",						__LINE__,
		T_0,	"no format chars",	"no format chars",		__LINE__,
		T_I1,	"%d",				"77",					__LINE__,
		T_I1,	"%-3d",				"77 ",					__LINE__,
		T_I1,	"%+d",				"+77",					__LINE__,
		T_I2,	"%+d",				"-77",					__LINE__,
		T_I2,	"%+4d",				" -77",					__LINE__,
		T_I1,	"% 4d",				"  77",					__LINE__,
		T_I2,	"% 4d",				" -77",					__LINE__,
		T_I1,	"% +4d",			" +77",					__LINE__,
		T_I1,	"%+ 4d",			" +77",					__LINE__,
		T_I1,	"%+04d",			"+077",					__LINE__, 
		T_I2,	"%04d",				"-077",					__LINE__, 
		T_I1,	"%04.2d",			"  77",					__LINE__, 
		T_I1,	"%.d",				"77",					__LINE__, 
		T_I1,	"%04.d",			"  77",					__LINE__, 
		T_I0,	"%.d",				"",						__LINE__, 
		T_I1,	"%.4d",				"0077",					__LINE__, 
		T_I1,	"%6.4d",			"  0077",				__LINE__, 
		T_I1,	"% 6.4d",			"  0077",				__LINE__, 
		T_I1,	"%06.4d",			"  0077",				__LINE__, 
		T_I1,	"%3d",				" 77",					__LINE__,
		T_I1,	"%0d",				"77",					__LINE__,
		T_I1,	"%1d",				"77",					__LINE__,
		T_I1,	"%-1d",				"77",					__LINE__,
		T_I1,	"%x",				"4d",					__LINE__,
		T_I1,	"%#x",				"0x4d",					__LINE__,
		T_I1,	"%#X",				"0X4D",					__LINE__,
		T_I1,	"%o",				"115",					__LINE__, 
		T_I1,	"%#o",				"0115",					__LINE__,
		T_I1,	"%d",				"77",					__LINE__,
		T_I1,	"%u",				"77",					__LINE__,
		T_I1,	"%.6d",				"000077",				__LINE__,
		T_I1,	"%.d",				"77",					__LINE__,
		T_I2,	"%.6d",				"-000077",				__LINE__,
		T_I1,	"%.1d",				"77",					__LINE__,
		T_I1,	"%.6o",				"000115",				__LINE__,
		T_I1,	"%.d",				"77",					__LINE__,
		T_I1,	"%4.6d",			"000077",				__LINE__,
		T_I1,	"%6.4d",			"  0077",				__LINE__,
		T_I1,	"%-6.4d",			"0077  ",				__LINE__,
		T_I1,	"%06d",				"000077",				__LINE__,
		T_I1,	"%-06d",			"77    ",				__LINE__,
		T_L1,	"%ld",				"65000",				__LINE__,
		T_D1,	"%f",				"12.345679",			__LINE__,
		T_D1,	"%6.3f",			"12.346",				__LINE__,
		T_D1,	"%.0f",				"12",					__LINE__,
		T_D2,	"%e",				"1.234568e+100",		__LINE__,
		T_D2,	"%E",				"1.234568E+100",		__LINE__,
		T_D2,	"%.3e",				"1.235e+100",			__LINE__,
		T_D2,	"%.10e",			"1.2345678900e+100",	__LINE__,
		T_D1,	"%e",				"1.234568e+01",			__LINE__,
		T_D1,	"%+e",				"+1.234568e+01",		__LINE__, 
		T_D1,	"%15.4e",			"     1.2346e+01",		__LINE__, 
		T_D1,	"%E",				"1.234568E+01",			__LINE__,
		T_D1,	"%.3e",				"1.235e+01",			__LINE__,
		T_D1,	"%.10e",			"1.2345678900e+01",		__LINE__,
		T_D3, 	"%e",				"0.000000e+00",			__LINE__,
		T_D1,	"%.0e",				"1e+01",				__LINE__,
		T_D1,	"%.e",				"1e+01",				__LINE__,
		T_D1,	"%#.0e",			"1.e+01",				__LINE__, 
		T_D5,	"%.2g",				"1.2e-06",				__LINE__,
		T_D6,	"%.2g",				"9.9e+02",				__LINE__,
		T_D7,	"%.2g",				"9.9e+03",				__LINE__,
		T_D7,	"%.2G",				"9.9E+03",				__LINE__,
		T_D4,	"%.0g",				"1",					__LINE__,
		T_D1,	"%.1g",				"1e+01",				__LINE__,
		T_D4,	"%#.0g",			"1.",					__LINE__,
		T_D1,	"%.2g",				"12",					__LINE__,
		T_D4,	"%.2g",				"1",					__LINE__,
		T_D4,	"%#.4g",			"1.000",				__LINE__,
		T_S,	"free form input",	"free form input",		__LINE__,
		T_S,	"%%%s%%d",			"%this is a string\n%d",__LINE__,
		T_S,	"%-20s",			"this is a string\n   ",__LINE__, 
		T_S,	"%20s",				"   this is a string\n",__LINE__,
		T_S,	"%20.6s",			"              this i",	__LINE__,
		T_S,	"%.6s",				"this i",				__LINE__,
		T_C1,	"%c",				"a",					__LINE__,
		T_C1,	"%2c",				" a",					__LINE__,
#if ANSI
		T_STAR1,"%.*d",				"000077",				__LINE__,
		T_STAR1,"%*d",				"    77",				__LINE__,
		T_STAR1,"%-*d",				"77    ",				__LINE__,
		T_STAR2,"%.*d",				"77",					__LINE__,
		T_STAR2,"%*d",				"77    ",				__LINE__,
		T_STAR3,"%*.*d", 			"  0077",				__LINE__,
		T_STAR3,"%-*.*d", 			"0077  ",				__LINE__,
		T_STARN,"%.*e",				"1.234568e+01",			__LINE__, 
		T_STARD,"%*.*e",			"     1.2346e+01",		__LINE__, 
		T_I1,	"%06.4d",			"  0077",				__LINE__,
		T_I1,	"%i",				"77",					__LINE__,
		T_SI,	"%#hx",				"0x7fff",				__LINE__,
		T_U,	"%u",				"65000",				__LINE__,
		T_SU,	"%ho",				"77777",				__LINE__,
		T_SU,	"%hu",				"32767",				__LINE__,
		T_SU,	"%hx",				"7fff",					__LINE__,
		T_L1,	"%lo",				"176750",				__LINE__,
		T_L1,	"%lu",				"65000",				__LINE__,
		T_L1,	"%lx",				"fde8",					__LINE__,
		T_D8,	"%Le",				"9.876540e+01",			__LINE__,
		T_D8,	"%Lf",				"98.765400",			__LINE__,
		T_D8,	"%Lg",				"98.7654",				__LINE__,
#endif
		0,		0,					0,						__LINE__,
		};

/*
 * PRINT - this test the printf, etc. scanf, etc. formatting.
 * We make the reasonable assumption that the formatting routines 
 * for sprintf are common with those for sprintf, fprintf, etc. and only
 * test one set.
 */
void print()
	{
#ifndef SKIP_PRINT
	int i;
	struct output *po;
	char buffer[64];

	Filename = "d49a.c";
	if (DBL_MAX_10_EXP >= 100)
		Dval2 = 1.23456789 * dpowu(10., 100);

	for (po = Output; po->fmt; ++po)
		{
		if (po->type == T_D2 && DBL_MAX_10_EXP < 100)
			{
			/* See update item #533, Review Board meeting 8/92 */
			checkthat(__LINE__, 1); /* count one successful test */
			continue;
			}
		switch (po->type)
			{
		case T_0:		sprintf(buffer, po->fmt); 				break;
		case T_U:		sprintf(buffer, po->fmt, Ui); 			break;
		case T_I0:		sprintf(buffer, po->fmt, 0);	 		break;
		case T_I1:		sprintf(buffer, po->fmt, Ival1); 		break;
		case T_I2:		sprintf(buffer, po->fmt, Ival2); 		break;
		case T_L1:		sprintf(buffer, po->fmt, Lval1); 		break;
		case T_SI:		sprintf(buffer, po->fmt, Sval); 		break;
		case T_SU:		sprintf(buffer, po->fmt, Suval); 		break;
		case T_D1:		sprintf(buffer, po->fmt, Dval1); 		break;
		case T_D2:		sprintf(buffer, po->fmt, Dval2); 		break;
		case T_D3:		sprintf(buffer, po->fmt, Dval3); 		break;
		case T_D4:		sprintf(buffer, po->fmt, Dval4); 		break;
		case T_D5:		sprintf(buffer, po->fmt, Dval5); 		break;
		case T_D6:		sprintf(buffer, po->fmt, Dval6); 		break;
		case T_D7:		sprintf(buffer, po->fmt, Dval7); 		break;
#if ANSI
		case T_D8:		sprintf(buffer, po->fmt, Dval8); 		break;
#endif
		case T_P:		sprintf(buffer, po->fmt, Pval); 		break;
		case T_S:		sprintf(buffer, po->fmt, String); 		break;
		case T_STAR1:	sprintf(buffer, po->fmt, 6, Ival1); 	break;
		case T_STAR2:	sprintf(buffer, po->fmt, -6, Ival1); 	break;
		case T_STAR3:	sprintf(buffer, po->fmt, 6, 4, Ival1); 	break;
		case T_STARD:	sprintf(buffer, po->fmt, 15, 4, Dval1);	break;
		case T_STARN:	sprintf(buffer, po->fmt, -4, Dval1);	break;
		case T_C1:		sprintf(buffer, po->fmt, Cval); 		break;
			}
		
		switch (po->type)
			{
			case T_D1:
			case T_D2:
			case T_D3:
			case T_D4:
			case T_D5:
			case T_D6:
			case T_D7:
			case T_D8:
			case T_STARD:
			case T_STARN:
				fpsteq(po->line, buffer, po->out);
				break;
			default:
				stequals(po->line, buffer, po->out);
			}
		}

#if ANSI
	{
	char *buffer_p;
	int t1; short t2; long t3;
	/* some special cases not covered above */

	buffer[0] = 0;
	sprintf(buffer, "%p", buffer);
	/* implementation defined format, but it should have written something */
	inotequals(__LINE__, buffer[0], 0);
	sscanf(buffer, "%p", &buffer_p);
	aequals(__LINE__, buffer, buffer_p);

#if ANSI8712	/* added h and l modifiers to n */
	/* %.0d of 0 produces no characters */
	sprintf(buffer, "%.0d", 0);
	stequals(__LINE__, buffer, "");
#endif

#if ANSI8703	/* added h and l modifiers to n */
	sprintf(buffer, "%nabc%hn%d%ln", &t1, &t2, 123, &t3);
	stequals(__LINE__, buffer, "abc123");
	iequals(__LINE__, t1, 0);
	iequals(__LINE__, t2, 3);
	lequals(__LINE__, t3, 6L);
#endif /* ANSI8703 */
	}
#endif
#endif /* SKIP_PRINT */
	}




static struct input
	{
	int type;
	char *fmt;
	char *in;
	int line;
	} Input[] = 
		{
		T_I1,	"%d",				"77",					__LINE__,
		T_I1,	"%d",				"  		  77",			__LINE__,
#if ANSI
		T_I1,	"%d",				"+77",					__LINE__,
		T_SI, 	"%hd", 				"32767",				__LINE__,
#endif
		T_I2,	"%d",				"-77",					__LINE__,
		T_I1,	"abc%d",			"abc77",				__LINE__,
		T_L1,	"%ld",				"65000",				__LINE__,
#if ANSI
		T_I1,	"%i",				"77",					__LINE__,
		T_I1,	"%i",				"+77",					__LINE__,
		T_I2,	"%i",				"-77",					__LINE__,
		T_I1,	"%i",				"0115",					__LINE__,
		T_I1,	"%i",				"0x4d",					__LINE__,
		T_I1,	"%i",				"0X4d",					__LINE__,
		T_SI, 	"%hi", 				"0x7fff",				__LINE__,
		T_SU,	"%ho",				"077777",				__LINE__,
		T_SU,	"%hu",				"32767",				__LINE__,
		T_SU,	"%hx",				"7fff",					__LINE__,
		T_L1,	"%lo",				"0176750",				__LINE__,
		T_L1,	"%lu",				"65000",				__LINE__,
		T_L1,	"%lx",				"fde8",					__LINE__,
#endif
		T_I1,	"%o",				"0115",					__LINE__,
		T_I1,	"%u",				"77",					__LINE__,
		T_I1,	"%x",				"4d",					__LINE__,
		T_I1,	"%x",				"4D",					__LINE__,
		T_I1,	"%X",				"4d",					__LINE__,
		T_I1,	"%X",				"4D",					__LINE__,
		T_S,	"%s",				" this is",				__LINE__,
		T_C1,	"%c",				"athis is",				__LINE__,
		T_C2,	"%c",				" this is",				__LINE__,
		T_F,	"%f",				"3.",					__LINE__,
		T_F,	"%e",				"3.",					__LINE__,
		T_F,	"%f",				".3e+01",				__LINE__,
		T_F,	"%e",				".3e+01",				__LINE__,
		T_F,	"%f",				"3.0",					__LINE__,
		T_F,	"%g",				"3.0",					__LINE__,
		T_D1,	"%le",				"12.3456789",			__LINE__,
		T_D1,	"%lf",				"12.3456789",			__LINE__,
		T_D1,	"%lg",				"1.23456789e+01",		__LINE__,
#if ANSI
		T_F,	"%E",				".3e+01",				__LINE__,
		T_F,	"%G",				"3.0",					__LINE__,
		T_D8,	"%Le",				"98.7654",				__LINE__,
		T_D8,	"%Lf",				"98.7654",				__LINE__,
		T_D8,	"%Lg",				"9.87654e+01",			__LINE__,
#endif
		0,		(char *)0,			(char *)0,				__LINE__,
		};



/*
 * SCAN - this test the scanf, etc. scanf, etc. number cracking.  
 * We make the reasonable assumption that the scanning routines
 * for sscanf are common with those for scanf, fscanf, etc. and only
 * test one set.
 */
void scan()
	{
#ifndef SKIP_SCAN
	int i, n;
	struct input *pi;
	char buffer[64];
	char ctmp;
	short stmp;
	unsigned short sutmp;
	int itmp;
	long ltmp;
	double dtmp;
#if ANSI
	long double ldtmp;
#endif
	float ftmp;
	int *ptmp;
	char *p = buffer;
	char *q = 0;

	Filename = "d49a.c";

	for (pi = Input; pi->fmt; ++pi)
		{
		switch (pi->type)
			{
			case T_I1 :
				sscanf(pi->in, pi->fmt, &itmp);
				iequals(pi->line, itmp, Ival1);
				itmp = 0;
				break;
			case T_SI :
				sscanf(pi->in, pi->fmt, &stmp);
				iequals(pi->line, stmp, Sval);
				stmp = 0;
				break;
			case T_SU :
				sscanf(pi->in, pi->fmt, &sutmp);
				iequals(pi->line, sutmp, Suval);
				sutmp = 0;
				break;
			case T_L1 :
				sscanf(pi->in, pi->fmt, &ltmp);
				lequals(pi->line, ltmp, Lval1);
				ltmp = 0;
				break;
			case T_D1 :
				sscanf(pi->in, pi->fmt, &dtmp);
				dequals(pi->line, dtmp, Dval1);
				dtmp = 0;
				break;
#if ANSI
			case T_D8 :
				sscanf(pi->in, pi->fmt, &ldtmp);
				ldequals(pi->line, ldtmp, Dval8);
				ldtmp = 0;
				break;
#endif
			case T_F :
				sscanf(pi->in, pi->fmt, &ftmp);
				dequals(pi->line, ftmp, Fval);
				ftmp = 0;
				break;
			case T_S :
				sscanf(pi->in, pi->fmt, buffer);
				stequals(pi->line, buffer, "this");
				buffer[0] = 0;
				break;
			case T_C1 :
				sscanf(pi->in, pi->fmt, &ctmp);
				iequals(pi->line, ctmp, 'a');
				ctmp = 0;
				break;
			case T_C2 :
				sscanf(pi->in, pi->fmt, &ctmp);
				iequals(pi->line, ctmp, ' ');
				ctmp = 0;
				break;
			}
		}

	/* special cases */

	/* if a directive fails, no more input is consumed */
		{
		char filename[256];
		FILE *fd;
		int tmp1 = 0;
		int tmp2 = 0;
		char c = 0;

		save_name(tmpnam(filename));
		fd = fopen(filename, "w+");
		fprintf(fd, "xxx 17 yyy 18");
		rewind(fd);
		fscanf(fd, "xxx %d zzz %d", &tmp1, &tmp2);
		iequals(__LINE__, tmp1, 17);
		iequals(__LINE__, tmp2, 0);
		fscanf(fd, "yyy %d", &tmp2);
		iequals(__LINE__, tmp2, 18);
		fscanf(fd, "%c", &c);
		iequals(__LINE__, c, 0);

		fclose(fd);
		remove(filename); 
		}

#if ANSI
	/* skip white space unless %c, %n, or %[ ] */
	sscanf(" \f\n\r\t\v10", "%d", &Ival1);
	iequals(__LINE__, Ival1, 10);
	sscanf(" \f\n\r\t\vA", "%c%c%c%c%c%c%c", p+0, p+1, p+2, p+3, p+4, p+5, p+6);
	iequals(__LINE__, p[0], ' ');
	iequals(__LINE__, p[1], '\f');
	iequals(__LINE__, p[2], '\n');
	iequals(__LINE__, p[3], '\r');
	iequals(__LINE__, p[4], '\t');
	iequals(__LINE__, p[5], '\v');
	iequals(__LINE__, p[6], 'A');

	sscanf("xxx yyy", "xxx%n%c", &Ival1, &Cval);
	iequals(__LINE__, Ival1, 3);
	iequals(__LINE__, Cval, ' ');

	p = " \f\n\r\t\vXXX";
	sscanf(p, "%s", buffer);
	stequals(__LINE__, buffer, "XXX");
	sscanf(p, "%[ \f\n\r\t\v]", buffer);
	stequals(__LINE__, buffer, " \f\n\r\t\v");

	/* assignment supression */
	Ival1 = 1;
	Ival2 = 2;
	sscanf("77 78", "%*d%d", &Ival1, &Ival2);
	iequals(__LINE__, Ival1, 78);
	iequals(__LINE__, Ival2, 2);

	/* field widths */
	sscanf("123456", "%3d%d", &Ival1, &Ival2);
	iequals(__LINE__, Ival1, 123);
	iequals(__LINE__, Ival2, 456);
	str_cpye(buffer+30, "XXXXXXX");
	sscanf("123456789", "%3d%3s%2c", &Ival2, buffer, buffer+30);
	iequals(__LINE__, Ival2, 123);
	stequals(__LINE__, buffer, "456");
	iequals(__LINE__, buffer[30], '7');
	iequals(__LINE__, buffer[31], '8');
	iequals(__LINE__, buffer[32], 'X');
	sscanf("abcabcabc", "%4[abc]", buffer);
	stequals(__LINE__, buffer, "abca");
	sscanf("xyzxyzxyz", "%4[^abc]", buffer);
	stequals(__LINE__, buffer, "xyzx");

	/* conversion count */
	sscanf("123 5", "%d%n", &i, &n);
	iequals(__LINE__, i, 123);
	iequals(__LINE__, n, 3);

	sscanf("123 5", "%d%hn", &i, &stmp);
	iequals(__LINE__, i, 123);
	iequals(__LINE__, stmp, 3);

	sscanf("123 5", "%d%ln", &i, &ltmp);
	iequals(__LINE__, i, 123);
	lequals(__LINE__, ltmp, 3L);

	/* '%%' is an  escape for '%' */
	str_cpye(buffer, "xxxxxx");
	sscanf("%%aaa", "%%%%%s", buffer);
	stequals(__LINE__, buffer, "aaa");

	/* scan sets */
	sscanf("ddccbbaaxxxxx", "%[abcd]", buffer);
	stequals(__LINE__, buffer, "ddccbbaa");

	sscanf("xxyyzzaabbccdd", "%[^abcd]", buffer);
	stequals(__LINE__, buffer, "xxyyzz");

	sscanf("dd]cc]bb]aaxxxxx", "%[]abcd]", buffer);
	stequals(__LINE__, buffer, "dd]cc]bb]aa");

	sscanf("xxyyzz]aabbccdd", "%[^]abcd]", buffer);
	stequals(__LINE__, buffer, "xxyyzz");

	/* exceptional behaviour of '-' is implementation defined */
	sscanf("-", "%[a-z]", buffer);
	stequals(- __LINE__, buffer, "-");

	/* %p should be idempotent */
	sprintf(buffer, "%p", p);
	sscanf(buffer, "%p", &q);
	aequals(__LINE__, p, q);
#endif
#endif /* SKIP_SCAN */
	}

#define IS_SPACE(c) ((c) == ' ')  /* Standard only allows space padding */
/*
 * fpsteq - When checking floating point output, the standard requires
 * a minimum of 2 digits for the exponent.  More is OK, so this routine
 * checks for conformance without being sensitive to the exact number
 * of digits.  When we know that the string is wrong, we call
 * stequals to create a meaningful error message. Also, compensate 
 * for differing number of white space in filling up the field width.
 */
void fpsteq(line, pval, pres)
	int line;
	char *pval;
	char *pres;
	{
	char result[32], value[32];
	int len;
	char *p, *q;

	/* check for field width (only on leading space) */
	if (IS_SPACE(*pval) && str_len(pres) != str_len(pval))
		{
		/* this will fail */
		stequals(line, pval, pres);
		return;
		}

	/* get rid of leading whitespace */
	while (IS_SPACE(*pval))
		++pval;
	while (IS_SPACE(*pres))
		++pres;
	if (str_str(pres, "E+") || str_str(pres, "E-") || 
			str_str(pres, "e+") || str_str(pres, "e-"))
		{
		len = str_cspn(pres, "Ee");
		if (len != str_cspn(pval, "Ee"))
			{
			/* this will fail */
			stequals(line, pval, pres);
			return;
			}
		st_ncpy(result, pres, len);
		result[len] = 0;
		st_ncpy(value, pval, len);
		value[len] = 0;
		if (str_cmp(result, value))
			{
			/* this will fail */
			stequals(line, pval, pres);
			}
		else /* OK so far, result matches value */
			{
			/* does pval have at least 2 digits of exponent? */
			p = pval + len + 2;
			if (str_len(p) < 2)
				{
				/* this will fail */
				stequals(line, pval, pres);
				}
			else /* OK so far, exponent >= 2 digits */
				{
				while (*p == '0')
					++p;
				q = pres + len + 2;
				while (*q == '0')
					++q;
				if (str_cmp(p, q))
					{
					/* this will fail */
					stequals(line, pval, pres);
					}
				else /* complete success, exponents match numerically */
					stequals(line, pval, pval);	/* just to get trace */
				}
			}
		}
	else /* no exponent in output format */
		stequals(line, pval, pres);
	}

#endif /* !SKIP49 */
