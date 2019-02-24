/* some functions from the UNIX manual section 3 */
#ifndef __LIBC
#define __LIBC
#include <stddef.h>

extern "C" {
	extern int abort();
	extern char* crypt(char*, char*);
	extern void setkey(char*);
	extern void encrypt(char*, int);
	extern char* ecvt(double, int, int*, int*);
	extern char* fcvt(double, int ,int*, int*);
	extern char* gcvt(double, int, char*);
	extern double frexp(double, int*);
	extern double ldexp(double, int);
	extern double modf(double, double*);
	extern char* getenv(char*);
	extern char* getlogin();
	extern char* getpass(char*);
	extern int getpw(int, char*);
	extern void l3tol(long*, char*, int);
	extern void ltol3(char*, long*, int);
	extern char *malloc(unsigned);
	extern void free(char *);
	extern char *realloc(char *, unsigned);
	extern char *calloc(unsigned, unsigned);
	extern void cfree(char*);
	extern char* mktemp(char*);
	extern void perror(char*);
	extern void qsort(void*, size_t, size_t, int(*)(const void*, const void*));
	extern void swab(char*, char*, int);
	extern int system(const char*);
	extern char* ttyname(int);
	extern int isatty(int);
	extern int ttyslot();
	extern int onexit(auto int(*)());
	extern int atexit(auto void(*)());
	extern void exit(int);
	extern void _exit(int);
	extern int rand();
	extern long lrand();
	extern double frand();
	extern int nrand(int);
	extern void srand(int);
	extern print(const char* ...);
	extern fprint(int, const char* ...);
	extern sprint(char*, const char* ...);
	extern int tolower(int);
	extern int toupper(int);

	extern int getfields(char *, char**, int);
	extern int getmfields(char *, char**, int);
	extern char *setfields(char *);
	
	extern int atoi(const char*);
	extern double atof(const char*);
	extern long atol(const char*);
	extern double strtod(const char*, char**);
	extern long strtol(const char*, char**, int);
	extern unsigned long strtoul(const char*, char**, int);

	extern int abs(int);
	extern long labs(long);
	extern int gcd(int, int);
	extern long lcm(int, int);
	extern int min(int, int);
	extern int max(int, int);
	extern int sgn(int);
}
#endif
