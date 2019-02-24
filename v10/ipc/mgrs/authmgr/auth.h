#define BUFSIZE	1024

#define	TRUE	1
#define FALSE	0

extern char	*getchallenge();
extern int	responseok();
extern int	doconnect();

enum keytypes {NONE, PASS, ATALLA};

/* if system V:
#define srand srand48
#define lrand lrand48
*/
