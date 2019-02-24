typedef struct Disk
{
	char *name;
	char busy;
	struct Client *c;
	long n;		/* number of file pairs */
	int *files;
	struct Disk *next, *snext;
} Disk;

typedef struct Client
{
	int fd;
	char busy;
	char *userinfo;
	Disk *disks;
	struct Client *next;
} Client;
extern Client *clients;

typedef struct Server
{
	int fd;
	char *name;
	char busy;
	Disk *disks;
	struct Server *next;
} Server;
extern Server *servers;
extern Server *newserver();

char me[];
