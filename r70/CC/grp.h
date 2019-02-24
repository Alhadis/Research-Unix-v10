struct	group { /* see getgrent(3) */
	char	*gr_name;
	char	*gr_passwd;
	int	gr_gid;
	char	**gr_mem;
};

extern "C" {
	extern struct group *getgrent();
	extern struct group *getgrgid(int);
	extern struct group *getgrnam(char *);
	extern int setgrent();
	extern int endgrent();
}
