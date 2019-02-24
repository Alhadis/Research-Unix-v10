/*
 * static char ID_slotvech[] = "@(#) slotvec.h: 1.3 3/13/83";
 */


struct slotvec{
	long svsymndx;		/* original (input) symbol table index	*/
	long svovaddr;		/* original (input) symbol virtual addr	*/
	long svnvaddr;		/* new (output) symbol virtual addr	*/
	long svnewndx;		/* new (output) symbol table index	*/
	char svsecnum;		/* new (output) section number		*/
	char svflags;
	};

#define SLOTVEC struct slotvec
#define SLOTSIZ sizeof(SLOTVEC)


#define  SV_TV		01
#define  SV_ERR		02

extern SLOTVEC	*svread( );
#if SDP
extern SLOTVEC	*getslot( );
#define GETSLOT(slt,action)	getslot(slt,action)
#else
#define GETSLOT(slt,action)	&svpageptr[(slt)]
#endif
