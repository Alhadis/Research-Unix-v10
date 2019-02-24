
#define MSIZE sizeof("delivered  From ") + DIRSIZ + 1
#define MAILLOG "/usr/spool/mail/mail.log"
#define MAILDIR "/usr/spool/mail"
#define MAILNAME "mail.log"

struct scanmail {
    char	sm_file[DIRSIZ+1];	/* name of file */
    long		sm_pos;			/* current position in file */
    char	sm_match[MSIZE];	/* preface to match */
    char	sm_log;			/* true if using mail log */
};

