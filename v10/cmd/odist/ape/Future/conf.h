/* structure corresponding to line of the config file */
struct conf {
	char *host;	/* regexp */
	char *user;	/* regexp */
	char *file;	/* regexp */
	char *cmds;	/* cmd+cmd+cmd... */
};

extern struct conf *confread(FILE *fp, char *host, char *user, char *file);

/* configuration command name and associated function to call */
struct confcmds {
	char *name;
	int (*func)(char *);
};

extern int confexec(char *cmdstr, struct confcmds *cmdtab);
