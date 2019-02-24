extern char *MAILROOT;	/* root of mail system */
extern char *UPASROOT;	/* root of upas system */
extern char *SMTPQROOT; /* root of smtpq directory */
extern char *SYSALIAS;	/* file system alias files are listed in */
extern char *USERALIAS;	/* file system alias files are listed in */
extern int MBOXMODE;	/* default mailbox protection mode */

/* format of REMOTE FROM lines */
extern char *REMFROMRE;
extern int REMSENDERMATCH;
extern int REMDATEMATCH;
extern int REMSYSMATCH;

/* format of mailbox FROM lines */
#define IS_HEADER(p) ((p)[0]=='F'&&(p)[1]=='r'&&(p)[2]=='o'&&(p)[3]=='m'&&(p)[4]==' ')
extern char *FROMRE;
extern int SENDERMATCH;
extern int DATEMATCH;

extern void print_header();
extern void print_remote_header();

#ifndef NULL
#define NULL 0
#endif
