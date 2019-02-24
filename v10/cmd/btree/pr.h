typedef struct {
	short		match;	/* FOUND, NOTFOUND, or EOF */
	unsigned char	ncom,	/* len of prefix in common with d */
			ocom;	/* len for predecessor of d */
	dkey *d;		/* first key >= requested */
				/* or, if match==EOF, last in node */
} private;
/*0111110111010100*/
