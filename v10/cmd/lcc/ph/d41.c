/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#define LIB_TEST 1
#include "defs.h"
#if !ANSI || !HAS_PROTOTYPES
#define SKIP41 1
#endif

#include "flags.h"
#ifndef SKIP41

static void d4_1_1();
static void d4_1a();
static void d4_1_2();
#ifndef FREESTANDING
void d4_1_3();
#endif /* FREESTANDING */
void d41_4a();
void d41_4b();
void d4_1_5();
static void d4_1_6();
#ifndef FREESTANDING
extern int atoi(const char *);
#endif /* FREESTANDING */

/*
 * 4.1 - Introduction
 * This file is not relevant to pre ANSI C compilers.
 */
void d4_1()
	{
	Filename = "d41.c";
	d4_1_1();
	d4_1_2();
#ifndef FREESTANDING
	d4_1_3();
#endif /* FREESTANDING */
	d41_4a();
	d41_4b();
	d4_1_5();
	d4_1_6();
#ifndef FREESTANDING
	/* test various methods of library call here,
	 * before including the headers.
	 */
		/* implicit declaration */
		{
		int i;
		i = atoi("4");
		iequals(__LINE__, i, 4);
		}

		/* explicit declaration */
		{
		int i;
		extern int atoi(const char *);
		i = atoi("4");
		iequals(__LINE__, i, 4);
	
		/* using its header */
		d4_1a();
		}
#endif /* FREESTANDING */
	}


#ifndef FREESTANDING
#include <stdlib.h>
#endif /* FREESTANDING */

static void d4_1a()
	{
#ifndef FREESTANDING
		{
		/* by use of the associated header */
		int i;
	
		i = atoi("5");
		iequals(__LINE__, i, 5);

		i = (atoi)("7");
		iequals(__LINE__, i, 7);
	
		#undef atoi
		i = atoi("6");
		iequals(__LINE__, i, 6);
		}
#endif /* FREESTANDING */

	}


/*
 * 4.1.1 - Definitions of terms
 */
static void d4_1_1()
	{
	/* definitions of "string", "letter", and "decimal-point character" */
	}

/*
 * 4.1.2 - Headers
 */
#ifndef SKIP41_2
int near, far, cdecl, unix, vax;				/* non-reserved names */
int globaldef, asm, fortran, pascal, entry;		/* non-reserved names */
int environ, i386, i387, exception;	/* etc. */	/* non-reserved names */
#define value (Be sure that any dummy names used in headers are valid.)
#define filename (A conforming implementation cannot just copy the library synopsis.)
#define stream (One possible technique is to have NO dummy names in headers.)
#define nmemb (If headers have this problem, they can be altered manually to proceed with testing.)
/* make sure that all of the specified headers exist */
#ifndef FREESTANDING
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#endif /* FREESTANDING */
#include <float.h>	/* required even in FREESTANDING */
#include <limits.h>	/* required even in FREESTANDING */
#include <stdarg.h>	/* required even in FREESTANDING */
#include <stddef.h>	/* required even in FREESTANDING */
#ifndef FREESTANDING
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif /* FREESTANDING */

/* check that another order and multiple includes are OK */
#ifndef FREESTANDING
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif /* FREESTANDING */
#include <stddef.h>	/* required even in FREESTANDING */
#include <limits.h>	/* required even in FREESTANDING */
#include <float.h>	/* required even in FREESTANDING */
#include <stdarg.h>	/* required even in FREESTANDING */
#ifndef FREESTANDING
#include <signal.h>
#include <setjmp.h>
#include <math.h>
#include <locale.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#endif /* FREESTANDING */
#endif /* SKIP41_2 */
static void d4_1_2()
	{
#ifndef SKIP41_2
	int _ok = 8703;	/* ANSI8703: internal id's of underscore + lowercase letter now OK */
	int UNIX = 2;
	int VAX = 3;
	int _near = 4;
	int _far = 5;

	iequals(__LINE__, _ok, 8703);
	iequals(__LINE__, UNIX, 2);
	iequals(__LINE__, VAX, 3);
	iequals(__LINE__, _near, 4);
	iequals(__LINE__, _far, 5);
#endif /* SKIP41_2 */
	}
/*
 * 4.1.6 - Use of library functions.
 */
static void d4_1_6()
	{
#ifndef SKIP416
#ifndef FREESTANDING
	/* All library functions must be accessible as real
	 * functions, whether implemented as macros or not.
	 * Each of these routines is supposed to have an underlying
	 * real function. Make sure that it exists. The definitions
	 * are supposed to occur in the standard header files.
	 */
	/* <ctype.h> */
	checkthat(__LINE__, isalnum != 0);
	checkthat(__LINE__, isalpha != 0);
	checkthat(__LINE__, iscntrl != 0);
	checkthat(__LINE__, isdigit != 0);
	checkthat(__LINE__, isgraph != 0);
	checkthat(__LINE__, islower != 0);
	checkthat(__LINE__, isprint != 0);
	checkthat(__LINE__, ispunct != 0);
	checkthat(__LINE__, isspace != 0);
	checkthat(__LINE__, isupper != 0);
	checkthat(__LINE__, isxdigit != 0);
	checkthat(__LINE__, tolower != 0);
	checkthat(__LINE__, toupper != 0);
	#undef isalnum
	#undef isalpha
	#undef iscntrl
	#undef isdigit
	#undef isgraph
	#undef islower
	#undef isprint
	#undef ispunct
	#undef isspace
	#undef isupper
	#undef isxdigit
	#undef tolower
	#undef toupper

	{
	int isalnum(int);
	int isalpha(int);
	int iscntrl(int);
	int isdigit(int);
	int isgraph(int);
	int islower(int);
	int isprint(int);
	int ispunct(int);
	int isspace(int);
	int isupper(int);
	int isxdigit(int);
	int tolower(int);
	int toupper(int);
	}

	/* <locale.h> */
	checkthat(__LINE__, setlocale != 0);
#if ANSI8709
	checkthat(__LINE__, localeconv != 0);
#endif
	#undef setlocale
	#undef localeconv
	{
	char *setlocale(int, const char *);
#if ANSI8709
	struct lconv *localeconv(void);
#endif
	}

	/* <math.h> */
	checkthat(__LINE__, acos != 0);
	checkthat(__LINE__, asin != 0);
	checkthat(__LINE__, atan != 0);
	checkthat(__LINE__, atan2 != 0);
	checkthat(__LINE__, cos != 0);
	checkthat(__LINE__, sin != 0);
	checkthat(__LINE__, tan != 0);
	checkthat(__LINE__, cosh != 0);
	checkthat(__LINE__, sinh != 0);
	checkthat(__LINE__, tanh != 0);
	checkthat(__LINE__, exp != 0);
	checkthat(__LINE__, frexp != 0);
	checkthat(__LINE__, ldexp != 0);
	checkthat(__LINE__, log != 0);
	checkthat(__LINE__, log10 != 0);
	checkthat(__LINE__, modf != 0);
	checkthat(__LINE__, pow != 0);
	checkthat(__LINE__, sqrt != 0);
	checkthat(__LINE__, ceil != 0);
	checkthat(__LINE__, fabs != 0);
	checkthat(__LINE__, floor != 0);
	checkthat(__LINE__, fmod != 0);

	#undef acos
	#undef asin
	#undef atan
	#undef atan2
	#undef cos
	#undef sin
	#undef tan
	#undef cosh
	#undef sinh
	#undef tanh
	#undef exp
	#undef frexp
	#undef ldexp
	#undef log
	#undef log10
	#undef modf
	#undef pow
	#undef sqrt
	#undef ceil
	#undef fabs
	#undef floor
	#undef fmod

	{
	double acos(double);
	double asin(double);
	double atan(double);
	double atan2(double, double);
	double cos(double);
	double sin(double);
	double tan(double);
	double cosh(double);
	double sinh(double);
	double tanh(double);
	double exp(double);
	double frexp(double, int *);
	double ldexp(double, int);
	double log(double);
	double log10(double);
	double modf(double, double *);
	double pow(double, double);
	double sqrt(double);
	double ceil(double);
	double fabs(double);
	double floor(double);
	double fmod(double, double);
	}

	/* <setjmp.h> */
	/* ANSI8706 now setjmp must be a macro - see 4.6 P3 */
	checkthat(__LINE__, longjmp != 0);
	#undef longjmp
	{
	void longjmp(jmp_buf, int);
	}

	/* <signal.h> */
	checkthat(__LINE__, signal != 0);
	checkthat(__LINE__, raise != 0);
	#undef signal
	#undef raise
	{
	void (*signal(int, void (*)(int)))(int);
	int raise(int);
	}


	/* <stdio.h> */
	checkthat(__LINE__, remove != 0);
	checkthat(__LINE__, rename != 0);
	checkthat(__LINE__, tmpfile != 0);
	checkthat(__LINE__, tmpnam != 0);
	checkthat(__LINE__, fclose != 0);
	checkthat(__LINE__, fflush != 0);
	checkthat(__LINE__, fopen != 0);
	checkthat(__LINE__, freopen != 0);
	checkthat(__LINE__, setbuf != 0);
	checkthat(__LINE__, setvbuf != 0);
	checkthat(__LINE__, fprintf != 0);
	checkthat(__LINE__, fscanf != 0);
	checkthat(__LINE__, printf != 0);
	checkthat(__LINE__, scanf != 0);
	checkthat(__LINE__, sprintf != 0);
	checkthat(__LINE__, sscanf != 0);
	checkthat(__LINE__, vfprintf != 0);
	checkthat(__LINE__, vprintf != 0);
	checkthat(__LINE__, vsprintf != 0);
	checkthat(__LINE__, fgetc != 0);
	checkthat(__LINE__, fgets != 0);
	checkthat(__LINE__, fputc != 0);
	checkthat(__LINE__, fputs != 0);
	checkthat(__LINE__, gets != 0);
	checkthat(__LINE__, getc != 0);
	checkthat(__LINE__, getchar != 0);
	checkthat(__LINE__, puts != 0);
	checkthat(__LINE__, putc != 0);
	checkthat(__LINE__, putchar != 0);
	checkthat(__LINE__, ungetc != 0);
	checkthat(__LINE__, fread != 0);
	checkthat(__LINE__, fwrite != 0);
	checkthat(__LINE__, fgetpos != 0);
	checkthat(__LINE__, fseek != 0);
	checkthat(__LINE__, fsetpos != 0);
	checkthat(__LINE__, ftell != 0);
	checkthat(__LINE__, rewind != 0);
	checkthat(__LINE__, clearerr != 0);
	checkthat(__LINE__, feof != 0);
	checkthat(__LINE__, ferror != 0);
	checkthat(__LINE__, perror != 0);

	#undef remove
	#undef rename
	#undef tmpfile
	#undef tmpnam
	#undef fclose
	#undef fflush
	#undef fopen
	#undef freopen
	#undef setbuf
	#undef setvbuf
	#undef fprintf
	#undef fscanf
	#undef printf
	#undef scanf
	#undef sprintf
	#undef sscanf
	#undef vfprintf
	#undef vprintf
	#undef vsprintf
	#undef fgetc
	#undef fgets
	#undef fputc
	#undef fputs
	#undef getc
	#undef getchar
	#undef gets
	#undef putc
	#undef putchar
	#undef puts
	#undef ungetc
	#undef fread
	#undef fwrite
	#undef fgetpos
	#undef fseek
	#undef fsetpos
	#undef ftell
	#undef rewind
	#undef clearerr
	#undef feof
	#undef ferror
	#undef perror

	{
	int remove(const char *);
	int rename(const char *, const char *);
	FILE *tmpfile(void);
	char *tmpnam(char *);
	int fclose(FILE *);
	int fflush(FILE *);
	FILE *fopen(const char *, const char *);
	FILE *freopen(const char *, const char *, FILE *);
	void setbuf(FILE *, char *);
	int setvbuf(FILE *, char *, int, size_t);
	int fprintf(FILE *, const char *, ...);
	int fscanf(FILE *, const char *, ...);
	int printf(const char *, ...);
	int scanf(const char *, ...);
	int sprintf(char *, const char *, ...);
	int sscanf(const char *, const char *, ...);
	int vfprintf(FILE *, const char *, va_list);
	int vprintf(const char *, va_list);
	int vsprintf(char *, const char *, va_list);
	int fgetc(FILE *);
	char *fgets(char *, int, FILE *);
	int fputc(int, FILE *);
	int fputs(const char *, FILE *);
	int getc(FILE *);
	int getchar(void);
	char *gets(char *);
	int putc(int, FILE *);
	int putchar(int);
	int puts(const char *);
	int ungetc(int, FILE *);
	size_t fread(void *, size_t, size_t, FILE *);
	size_t fwrite(const void *, size_t, size_t, FILE *);
	int fgetpos(FILE *, fpos_t *);
	int fseek(FILE *, long, int);
	int fsetpos(FILE *, const fpos_t *);
	long ftell(FILE *);
	void rewind(FILE *);
	void clearerr(FILE *);
	int feof(FILE *);
	int ferror(FILE *);
	void perror(const char *);
	}

	/* <stdlib.h> */
	checkthat(__LINE__, atof != 0);
	checkthat(__LINE__, atoi != 0);
	checkthat(__LINE__, atol != 0);
	checkthat(__LINE__, strtod != 0);
	checkthat(__LINE__, strtol != 0);
	checkthat(__LINE__, strtoul != 0);
	checkthat(__LINE__, rand != 0);
	checkthat(__LINE__, srand != 0);
	checkthat(__LINE__, calloc != 0);
	checkthat(__LINE__, free != 0);
	checkthat(__LINE__, malloc != 0);
	checkthat(__LINE__, realloc != 0);
	checkthat(__LINE__, abort != 0);
	checkthat(__LINE__, atexit != 0);
	checkthat(__LINE__, exit != 0);
	checkthat(__LINE__, getenv != 0);
	checkthat(__LINE__, system != 0);
	checkthat(__LINE__, bsearch != 0);
	checkthat(__LINE__, qsort != 0);
	checkthat(__LINE__, abs != 0);
	checkthat(__LINE__, div != 0);	
	checkthat(__LINE__, labs != 0);
	checkthat(__LINE__, ldiv != 0);	
#if WIDE_CHARS
	checkthat(__LINE__, mblen != 0);
	checkthat(__LINE__, mbtowc != 0);
	checkthat(__LINE__, wctomb != 0);
	checkthat(__LINE__, mbstowcs != 0);
	checkthat(__LINE__, wcstombs != 0);
#endif

	#undef atof
	#undef atoi
	#undef atol
	#undef strtod
	#undef strtol
	#undef strtoul
	#undef rand
	#undef srand
	#undef calloc
	#undef free
	#undef malloc
	#undef realloc
	#undef abort
	#undef atexit
	#undef exit
	#undef getenv
	#undef system
	#undef bsearch
	#undef qsort
	#undef abs
	#undef div
	#undef labs
	#undef ldiv
	#undef mblen
	#undef mbtowc
	#undef wctomb
	#undef mbstowcs
	#undef wcstombs

	{
	double atof(const char *);	
	int atoi(const char *);	
	long atol(const char *);
	double strtod(const char *, char **);	
	long strtol(const char *, char **, int );	
	unsigned long strtoul(const char *, char **, int);	
	int rand(void);
	void srand(unsigned int);
	void *calloc(size_t, size_t);
	void free(void *);	
	void *malloc(size_t);
	void *realloc(void *, size_t);	
	void abort(void);
	int atexit(void (*)(void));	
	void exit(int);
	char *getenv(const char *);	
	int system(const char *);
	void *bsearch(const void *, const void *, size_t, size_t, int (*)(const void *, const void *));	
	void qsort(void *, size_t, size_t, int (*)(const void *, const void *));	
	int abs(int);
	div_t div(int, int);	
	long labs(long);
	ldiv_t ldiv(long, long);	
#if WIDE_CHARS
	int mblen(const char *, size_t);
	int mbtowc(wchar_t *, const char *, size_t);
	int wctomb(char *, wchar_t);
	size_t mbstowcs(wchar_t *, const char *, size_t);
	size_t wcstombs(char *, const wchar_t *, size_t);
#endif
	}

	/* <string.h> */
	checkthat(__LINE__, memcpy != 0);
	checkthat(__LINE__, memmove != 0);
	checkthat(__LINE__, strcpy != 0);
	checkthat(__LINE__, strncpy != 0);
	checkthat(__LINE__, strcat != 0);
	checkthat(__LINE__, strncat != 0);
	checkthat(__LINE__, memcmp != 0);
	checkthat(__LINE__, strcmp != 0);
	checkthat(__LINE__, strncmp != 0);
	checkthat(__LINE__, strcoll != 0);
#if ANSI8612
	checkthat(__LINE__, strxfrm != 0);
#endif
	checkthat(__LINE__, memchr != 0);
	checkthat(__LINE__, strchr != 0);
	checkthat(__LINE__, strcspn != 0);
	checkthat(__LINE__, strpbrk != 0);
	checkthat(__LINE__, strrchr != 0);
	checkthat(__LINE__, strspn != 0);
	checkthat(__LINE__, strstr != 0);
	checkthat(__LINE__, strtok != 0);
	checkthat(__LINE__, memset != 0);
	checkthat(__LINE__, strerror != 0);
	checkthat(__LINE__, strlen != 0);
	#undef memcpy
	#undef memmove
	#undef strcpy
	#undef strncpy
	#undef strcat
	#undef strncat
	#undef memcmp
	#undef strcmp
	#undef strncmp
	#undef strcoll
	#undef memchr
	#undef strchr
	#undef strcspn
	#undef strpbrk
	#undef strrchr
	#undef strspn
	#undef strstr
	#undef strtok
	#undef memset
	#undef strerror
	#undef strlen
	{
	void *memcpy(void *, const void *, size_t);
	void *memmove(void *, const void *, size_t);
	char *strcpy(char *, const char *);
	char *strncpy(char *, const char *, size_t);
	char *strcat(char *, const char *);
	char *strncat(char *, const char *, size_t);
	int memcmp(const void *, const void *, size_t);
	int strcmp(const char *, const char *);
	int strncmp(const char *, const char *, size_t);
#if ANSI8612
	int strcoll(const char *, const char *);
	size_t strxfrm(char *, const char *, size_t);
#else
	size_t strcoll(char *, size_t, const char *);
#endif
	void *memchr(const void *, int, size_t);
	char *strchr(const char *, int);
	size_t strcspn(const char *, const char *);
	char *strpbrk(const char *, const char *);
	char *strrchr(const char *, int);
	size_t strspn(const char *, const char *);
	char *strstr(const char *, const char *);
	char *strtok(char *, const char *);
	void *memset(void *, int, size_t);
	char *strerror(int);
	size_t strlen(const char *);
	}

	/* <time.h> */
	checkthat(__LINE__, clock != 0);
	checkthat(__LINE__, difftime != 0);
	checkthat(__LINE__, mktime != 0);
	checkthat(__LINE__, time != 0);
	checkthat(__LINE__, asctime != 0);
	checkthat(__LINE__, ctime != 0);
	checkthat(__LINE__, gmtime != 0);
	checkthat(__LINE__, localtime != 0);
	checkthat(__LINE__, strftime != 0);

	#undef clock
	#undef difftime
	#undef mktime
	#undef time
	#undef asctime
	#undef ctime
	#undef gmtime
	#undef localtime
	#undef strftime
	{
	clock_t clock(void);
	double difftime(time_t, time_t);
	time_t mktime(struct tm *);
	time_t time(time_t *);
	char *asctime(const struct tm *);
	char *ctime(const time_t *);
	struct tm *gmtime(const time_t *);
	struct tm *localtime(const time_t *);
	size_t strftime(char *, size_t, const char *, const struct tm *);
	}
#endif /* FREESTANDING */
#endif /* SKIP416 */
	}

#else /* if SKIP41 */
void d4_1() { pr_skip("d4_1: SKIPPED ENTIRELY\n"); }
#endif	/* SKIP41 */
