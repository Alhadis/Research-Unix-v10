#ifdef SPARC
#  define GLOBAL(x)	.global x
#  define TEXT		.seg "text"
#  define DATA		.seg "data"
#  define ALIGN		.align 4
#  define longword(x)	.word x
#  define DATALIST	_datalist
#else
#  define GLOBAL(x)	.globl x
#  define TEXT		.text
#  define DATA		.data
#  define ALIGN		.align 2
#ifdef MIPS
#    define	longword(x)	.word x
#    define	DATALIST	datalist
#else
#    define     DATALIST        _datalist
#ifdef NS32
#    define	longword(x)	.double x
#else
#    define	longword(x)	.long x
#endif
#endif
#endif

/* we have a DATALIST with one garbage entry, because some linkers
   are too stupid to define an absolute symbol for the end of list. */

	GLOBAL(DATALIST)
	TEXT
	longword(0x0000010f)		/* String tag */
LC0:
	.ascii	"%never%^match\0\0\0"
	longword(0x0000010f)		/* String tag */
LC1:
	.ascii	"%bad%^%^data%\0\0\0"
	DATA
	ALIGN
DATALIST:
	longword(LC0)
	longword(LC1)
	longword(1)	/* Next link: null */

	GLOBAL(_endmo)
_endmo:

