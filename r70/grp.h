struct	group { /* see getgrent(3) */
	char	*gr_name;
	char	*gr_passwd;
	int	gr_gid;
	char	**gr_mem;
};
extern struct group *getgrent();
extern struct group *getgrgid();
extern struct group *getgrnam();
