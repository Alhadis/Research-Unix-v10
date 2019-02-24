#define NSUBEXP  10
struct regexp { /* see regexp(3) */
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char program[1];
};
extern regexp*	regcomp(const char *);
extern int	regexec(regexp*, const char*);
extern void	regsub(const regexp*, const char*, char*);
extern void	regerr(char*);
