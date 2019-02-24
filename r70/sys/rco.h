struct rco {
	u_short flags;
	struct device *addr;
	struct buf buf;
	ubm_t	ubm;
	ubm_t	chunkubm;
	struct pte *pte;
	int	mreg;

	u_short resol;		/* resolution */
	u_short dither;		/* 1=dither on */
};
