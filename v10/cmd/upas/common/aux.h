extern string *abspath();
extern char *sysname_read();
extern char *basename();
extern int delivery_status();
extern void append_match();

/* mailbox types */
#define MF_NORMAL 0
#define MF_PIPE 1
#define MF_FORWARD 2
#define MF_NOMBOX 3
#define MF_NOTMBOX 4
