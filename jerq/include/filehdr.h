/*	@(#)filehdr.h	2.1	*/
struct filehdr {
	unsigned short	f_magic;	/* magic number */
	unsigned short	f_nscns;	/* number of sections */
	long		f_timdat;	/* time & date stamp */
	long		f_symptr;	/* file pointer to symtab */
	long		f_nsyms;	/* number of symtab entries */
	unsigned short	f_opthdr;	/* sizeof(optional hdr) */
	unsigned short	f_flags;	/* flags */
	};


/*
 *   Bits for f_flags:
 *
 *	F_RELFLG	relocation info stripped from file
 *	F_EXEC		file is executable  (i.e. no unresolved
 *				external references)
 *	F_LNNO		line nunbers stripped from file
 *	F_LSYMS		local symbols stripped from file
 *	F_MINMAL	this is a minimal object file (".m") output of fextract
 *	F_UPDATE	this is a fully bound update file, output of ogen
 *	F_SWABD		this file has had its bytes swabbed (in names)
 *	F_AR16WR	this file created on AR16WR machine(e.g. 11/70)
 *	F_AR32WR	this file created on AR32WR machine(e.g. vax)
 *	F_AR32W		this file created on AR32W machine (e.g. 3b,maxi)
 *	F_PATCH		file contains "patch" list in optional header
 */

#define  F_RELFLG	0000001
#define  F_EXEC		0000002
#define  F_LNNO		0000004
#define  F_LSYMS	0000010
#define  F_MINMAL	0000020
#define  F_UPDATE	0000040
#define  F_SWABD	0000100
#define  F_AR16WR	0000200
#define  F_AR32WR	0000400
#define  F_AR32W	0001000
#define  F_PATCH	0002000


/*
 *   Magic Numbers
 */

	/* Basic-16 */

#define  B16MAGIC	0502
#define  BTVMAGIC	0503

	/* x86 */

#define  X86MAGIC	0510
#define  XTVMAGIC	0511

	/* n3b */

#define  N3BMAGIC	0550
#define  NTVMAGIC	0551

	/*  MAC-80   3b-5  */

#define  M80MAGIC	0560
#define  MTVMAGIC	0561


	/* VAX 11/780 and VAX 11/750 */

			/* writeable text segments */
#define VAXWRMAGIC	0570
			/* readonly sharable text segments */
#define VAXROMAGIC	0575

#define	FILHDR	struct filehdr
#define	FILHSZ	sizeof(FILHDR)
