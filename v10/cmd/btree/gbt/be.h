#define BERROR		100		/* first btree value of errno */
#define BUTRAN		BERROR + 0	/* user caused tran abort */
#define BNOWRITE	BERROR + 1	/* not opened for writing */
#define BIOWRT		BERROR + 2	/* wrote short record */
#define BNOMEM		BERROR + 3	/* no mem from malloc */
#define BFATAL		BERROR + 4	/* last chance for user */
#define BTALL		BERROR + 5	/* tree becoming taller than MXHT */
#define BLASTE		BERROR + 6	/* one past last btree value of errno */
extern int	errno;
/*0101010011100100*/
