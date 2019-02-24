# define SEQ 1
# define SENT 2
# define STORY 3
# define DEST 10
char *svc();
struct wordstr {
	char *wch;
	int seq;
	int ifneg;
	int mask;
	int which;
	int single;
	};
struct matchstr {
	int ntype;
	int next;
	int ct;
	int lim;
	int smask;
	};
struct deststr {
	char *dstr;
	int sndflg;
	char * homedir;
	};
