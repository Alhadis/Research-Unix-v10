struct filehdr {
	unsigned short	f_magic;	/* magic number */
	unsigned short	f_nscns;	/* number of sections */
	long		f_timdat;	/* time & date stamp */
	long		f_symptr;	/* file pointer to symtab */
	long		f_nsyms;	/* number of symtab entries */
	unsigned short	f_opthdr;	/* sizeof(optional hdr) */
	unsigned short	f_flags;	/* flags */
};

#define  FBOMAGIC	0560

struct aouthdr {
	short	magic;		/* see magic.h				*/
	short	vstamp;		/* version stamp			*/
	long	tsize;		/* text size in bytes, padded to FW
				   bdry					*/
	long	dsize;		/* initialized data "  "		*/
	long	bsize;		/* uninitialized data "   "		*/
	long	entry;		/* entry pt.				*/
	long	text_start;	/* base of text used for this file	*/
	long	data_start;	/* base of data used for this file	*/
};

struct scnhdr {
	char	s_name[8];	/* section name */
	long	s_paddr;	/* physical address */
	long	s_vaddr;	/* virtual address */
	long	s_size;		/* section size */
	long	s_scnptr;	/* file ptr to raw data for section */
	long	s_relptr;	/* file ptr to relocation */
	long	s_lnnoptr;	/* file ptr to line numbers */
unsigned short	s_nreloc;	/* number of relocation entries */
unsigned short	s_nlnno;	/* number of line number entries */
	long	s_flags;	/* flags */
};

#define COFFENTRYSIZE 18	/* SymEnt is 2 entries */
class SymEnt {
	friend Block *CoffSymTab.gatherfunc(Func *);	/* for REGPARM */
	char	situ[4];	/* 1-3: here; 4-8: sf()'d; 9-?: string table */
	char	*ptr;
	long	_value;
	short	_scnum;
unsigned short	pcc1;
	char	_sclass;
	char	_numaux;
	char 	aux[18];
public:
	char	*id()	{ return this ? ptr : "?";		}
	long	value()	{ return _value;			}
	long	scnum()	{ return _scnum;			}
	int	desc()	{ return ((pcc1&017)|((pcc1<<1)&~037));	}
	long	sclass()	{ return _sclass;			}
	long	numaux()	{ return _numaux;			}
	void	mkid(char *stringtable)
		{
			if( *situ == 0){
				ptr += (long)stringtable;
				*situ = 1;
			} else if( *situ != 1){
				if( situ[3] ){
					ptr = sf("%0.8s",situ);
					*situ = 1;
				} else
					ptr = situ,*ptr;	// <<< silly fix;
			}
		}
	
#define AUX(n,t,f) t f() { return _numaux ? *(t*)(aux+n) : 0; }	/* ??? */

	AUX(0,	long,	scnlen	)		/* section */
	AUX(4,	short,	nreloc	)
	AUX(6,	short,	nlinno	)
	
	AUX(6,	short,	size	)		/* tag name */
	AUX(12,	long,	endndx	)

	AUX(0,	long,	tagndx	)		/* eos */

	AUX(4,	short,	lnno	)		/* .bf .ef */

	AUX(4,	long,	fsize	)		/* function */
	AUX(8,	long,	lnnoptr	)

	AUX(6,	short,	fldsize	)		/* bit field size in C_FIELD */

	char	*filename() 	{ return (char*)aux; }
	short	dimen(int i)	{ return (i<0||i>4) ? 0 : ((short[])aux)[4+i]; }
};

#define MAXLINENO 500
#define LINENOSIZE 6
struct LineNo{
	long	addr;
	short	line;
};

#define  C_EFCN          -1    /* physical end of function */
#define  C_NULL          0
#define  C_AUTO          1     /* automatic variable */
#define  C_EXT           2     /* external symbol */
#define  C_STAT          3     /* static */
#define  C_REG           4     /* register variable */
#define  C_EXTDEF        5     /* external definition */
#define  C_LABEL         6     /* label */
#define  C_ULABEL        7     /* undefined label */
#define  C_MOS           8     /* member of structure */
#define  C_ARG           9     /* function argument */
#define  C_STRTAG        10    /* structure tag */
#define  C_MOU           11    /* member of union */
#define  C_UNTAG         12    /* union tag */
#define  C_TPDEF         13    /* type definition */
#define  C_USTATIC	 14    /* undefined static */
#define  C_ENTAG         15    /* enumeration tag */
#define  C_MOE           16    /* member of enumeration */
#define  C_REGPARM	 17    /* register parameter */
#define  C_FIELD         18    /* bit field */
#define  C_BLOCK         100   /* ".bb" or ".eb" */
#define  C_FCN           101   /* ".bf" or ".ef" */
#define  C_EOS           102   /* end of structure */
#define  C_FILE          103   /* file name */
