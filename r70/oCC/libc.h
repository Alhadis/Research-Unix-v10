
	/*	some functions from the UNIX manual section 3

	  	INCOMPLETE: contains only declarations of functions that
		(1) is not declared with their data structures
		(2) are identically declared in UNIX5.2 and bsd4.2
	*/

extern int abort(...);
/*	extern int abs(int); */
/*	extern long clock(); */
extern char* crypt(char*, char*);
extern void setkey(char*);
extern void encrypt(char*, int);
extern char* ecvt(double, int, int*, int*);
extern char* fcvt(double, int ,int*, int*);
extern char* gcvt(double, int, char*);
/* extern end, etext, edata; */
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
extern char* mktemp(char*);
extern void perror(char*);
/* extern sleep(unsigned); */
extern void swab(char*, char*, int);
extern int system(char*);
extern char* ttyname(int);
extern int isatty(int);
extern int ttyslot();
extern int sprint(char *, char * ...);
extern int print(char * ...);
extern int fprint(int, char * ...);
extern int system(char *);
extern long atol(const char *);
extern void exit(int);
extern int creat(char *, int);
extern int atoi(const char *);
