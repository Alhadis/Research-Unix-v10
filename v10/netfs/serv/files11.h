/*
 * FILES-11 ODS-1 data structures
 * rude comments believed due to Rob Pike
 */

struct filnam {
	unsigned short f_nam[3];	/* File name (Radix-50) */
	unsigned short f_typ;	/* File type (Radix-50) */
	unsigned short f_ver;	/* Version number */
};

struct uic {
	char	u_prog;		/* Programmer number */
	char	u_proj;		/* Project number */
};

struct fileid {
	unsigned short f_num;	/* File number */
	unsigned short f_seq;	/* File sequence number (worthless concept) */
	unsigned short f_rvn;	/* Relative volume number (ditto and MBZ) */
};

struct fcs {
	char	f_rtyp;		/* Record type */
	char	f_ratt;		/* Record attributes */
	unsigned short f_rsiz;	/* Record size */
	unsigned short f_hibk[2];	/* Highest VBN allocated */
	unsigned short f_efbk[2];	/* End of file block */
	unsigned short f_ffby;	/* First free byte */
};

/*
 * record types and attributes
 */
#define	RTVAR	2	/* variable length text records */
#define	RTCR	2	/* carriage return format */
/* need RTBLK	xx	/* records don't cross blocks */

struct ident {
	struct filnam i_fnam;	/* File name, type, version number */
	unsigned short i_rvno;	/* Revision number */
	char	i_rvdt[7];	/* Revision date (ASCII) */
	char	i_rvti[6];	/* Revision time (ASCII) */
	char	i_crdt[7];	/* Creation time (ASCII) */
	char	i_crti[6];	/* Creation time (ASCII) */
	char	i_exdt[7];	/* Expiration date (ASCII) */
	char	i_UU;		/* Unused */
};

struct map {
	char	m_esqn;		/* Extension segment number */
	char	m_ervn;		/* Extension relative volume number */
	unsigned short m_efnu;	/* Extension file number */
	unsigned short m_efsq;	/* Extension file sequence number */
	char	m_ctsz;		/* Block count field size */
	char	m_lbsz;		/* LBN Field size */
	unsigned char m_use;	/* Map words in use */
	unsigned char m_max;	/* Map words available */
	unsigned char m_rtrvp[410];	/* Retrieval pointers */
};

struct header {
	char	h_idof;		/* Ident area offset */
	char	h_mpof;		/* Map area offset */
	unsigned short h_fnum;	/* File number */
	unsigned short h_fseq;	/* File sequence number (why no RVN?) */
	unsigned short h_flev;	/* File structure level */
	struct uic h_fown;	/* File owner UIC */
	short	h_fpro;		/* File protection code */
	char	h_fcha[2];	/* File characteristics */
	struct fcs h_fcs;	/* FCS area */
	char	h_ufat[18];	/* User attribute area */
	struct ident h_ident;	/* Ident area */
	struct map h_map;	/* Map area */
};

struct homeblock {
	unsigned short H_ibsz;	/* Index file bitmap size */
	unsigned short H_iblb[2];	/* Index file bitmap LBN */
	unsigned short H_fmax;	/* Maximum number of files */
	unsigned short H_sbcl;	/* Storage bitmap cluster factor */
	unsigned short H_dvty;	/* Disk device type */
	unsigned short H_vlev;	/* Volume structure level */
	char	H_vnam[12];	/* Volume name */
	char	H_UU1[4];	/* Unused 1 */
	struct uic H_vown;	/* Volume owner UIC */
	short	H_vpro;		/* Volume protection code */
	char	H_vcha[2];	/* Volume characteristics */
	short	H_fpro;		/* Default file protection */
	char	H_UU2[6];	/* Unused 2 */
	char	H_wisz;		/* Default window size */
	char	H_fiex;		/* Default file extend */
	char	H_lruc;		/* Directory pre-access limit */
	char	H_UU3[11];	/* Unused 3 */
	unsigned short H_chk1;	/* First checksum */
	char	H_vdat[14];	/* Volume creation date */
	char	H_UU4[398];	/* Unused 4 */
	char	H_indn[12];	/* Volume name (don't you believe H_vnam?) */
	char	H_indo[12];	/* Volume owner (in a different format!) */
	char	H_indf[12];	/* Format type == "DECFILE11A " */
	char	H_UU5[2];	/* Unused 5 */
	unsigned short H_chk2;	/* Final checksum */
};

struct directory {
	struct fileid d_fid;	/* File id */
	struct filnam d_fname;	/* File name, type, version number */
};

#define	BLKSIZ	512		/* size of a block */

#define	HOMEBLK	1		/* home block is first 512-byte block */
#define	HOMEOFF	512		/* or HOMEBLK+(HOMEOFF*j) for some j */

/*
 * well-known files
 */
#define	FINDEX	1		/* index file */
#define	FBITMAP	2		/* storage bitmap */
#define	FROOT	4		/* root directory (MFD) */

/*
 * fileid to blkno
 * blkno wrt beginning of list in index file
 * for the well-known files, wrt known first file header
 */
#define	fidtob(f)	((f)-1)

/*
 * blocks in the index file
 * zero-origin, not 1-origin VBNs
 */

/* boot block	0 */
/* home block	1 */
#define	IBITMAP	2		/* first block of bitmap */
