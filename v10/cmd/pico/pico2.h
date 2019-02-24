#define	XREG		11
#define	YREG		10
#define IREG		 9

#define DII		new(REG, Z, Z, IREG)	/* default i-index */

#define DOLRED(A,B)	new(DEREFB, new(VAR,Z,Z,A->pixred), ncopy(B), Z)
#define DOLGRN(A,B)	new(DEREFB, new(VAR,Z,Z,A->pixgrn), ncopy(B), Z)
#define DOLBLU(A,B)	new(DEREFB, new(VAR,Z,Z,A->pixblu), ncopy(B), Z)
#define ARRRED(A)	DOLRED(A, DII)
#define ARRGRN(A)	DOLGRN(A, DII)
#define ARRBLU(A)	DOLBLU(A, DII)

#define AREG(x)		new(REG, Z, Z, x)
