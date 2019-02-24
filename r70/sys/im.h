struct im {
	char	open;		/* single open */
	enum states state;	/* driver state */
	struct imdevice *addr;
	ubm_t	ubm;
	struct buf buf;
};
