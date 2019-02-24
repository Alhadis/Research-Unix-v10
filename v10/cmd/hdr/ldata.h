/*
 *	Local data type structure.
 */
typedef struct
{
	char	*ld_name;
	short	ld_init;
	short	ld_write;
	short	ld_read;
	short	ld_count;
	int	ld_flags;
	short	ld_cvt[LD_NUM];
}
	ldata;

extern ldata	linfo[];
