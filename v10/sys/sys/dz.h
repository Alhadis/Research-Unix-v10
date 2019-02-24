struct	dz {
	short	state;
	short	flags;
	struct	device	*addr;
	struct	block	*oblock;
	struct	queue	*rdq;
	char	board;
	char	line;
	char	speed;
	char	brking;
};
