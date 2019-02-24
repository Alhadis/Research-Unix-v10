/*
 *		@(#)tdefs.h	2.1 DKHOST 85/01/15
 */

	typedef char		Tchar, *TcharP;

#	define Mchar	/* mos, mou */ Tchar
#	define McharP	/* mos, mou */ TcharP
#	define Achar	auto Tchar
#	define AcharP	auto TcharP
#	define Echar	extern Tchar
#	define EcharP	extern TcharP
#	define Rchar	register Tchar
#	define RcharP	register TcharP
#	define Schar	static Tchar
#	define ScharP	static TcharP
#	define Pchar	/* parameter, value */ Tchar
#	define PcharP	/* parameter, value */ TcharP
#	define Xchar	/* extdef */ Tchar
#	define XcharP	/* extdef */ TcharP

	typedef short		Tshort, *TshortP;

#	define Mshort	/* mos, mou */ Tshort
#	define MshortP	/* mos, mou */ TshortP
#	define Ashort	auto Tshort
#	define AshortP	auto TshortP
#	define Eshort	extern Tshort
#	define EshortP	extern TshortP
#	define Rshort	register Tshort
#	define RshortP	register TshortP
#	define Sshort	static Tshort
#	define SshortP	static TshortP
#	define Pshort	/* parameter, value */ Tshort
#	define PshortP	/* parameter, value */ TshortP
#	define Xshort	/* extdef */ Tshort
#	define XshortP	/* extdef */ TshortP

	typedef int		Tint, *TintP;

#	define Mint	/* mos, mou */ Tint
#	define MintP	/* mos, mou */ TintP
#	define Aint	auto Tint
#	define AintP	auto TintP
#	define Eint	extern Tint
#	define EintP	extern TintP
#	define Rint	register Tint
#	define RintP	register TintP
#	define Sint	static Tint
#	define SintP	static TintP
#	define Pint	/* parameter, value */ Tint
#	define PintP	/* parameter, value */ TintP
#	define Xint	/* extdef */ Tint
#	define XintP	/* extdef */ TintP

	typedef unsigned int	Tuint, *TuintP;

#	define Muint	/* mos, mou */ Tuint
#	define MuintP	/* mos, mou */ TuintP
#	define Auint	auto Tuint
#	define AuintP	auto TuintP
#	define Euint	extern Tuint
#	define EuintP	extern TuintP
#	define Ruint	register Tuint
#	define RuintP	register TuintP
#	define Suint	static Tuint
#	define SuintP	static TuintP
#	define Puint	/* parameter, value */ Tuint
#	define PuintP	/* parameter, value */ TuintP
#	define Xuint	/* extdef */ Tuint
#	define XuintP	/* extdef */ TuintP

	typedef long		Tlong, *TlongP;

#	define Mlong	/* mos, mou */ Tlong
#	define MlongP	/* mos, mou */ TlongP
#	define Along	auto Tlong
#	define AlongP	auto TlongP
#	define Elong	extern Tlong
#	define ElongP	extern TlongP
#	define Rlong	register Tlong
#	define RlongP	register TlongP
#	define Slong	static Tlong
#	define SlongP	static TlongP
#	define Plong	/* parameter, value */ Tlong
#	define PlongP	/* parameter, value */ TlongP
#	define Xlong	/* extdef */ Tlong
#	define XlongP	/* extdef */ TlongP

	typedef int		Tfn(), (*TfnP)();

#	define Mfn	/* mos, mou */ Tfn
#	define MfnP	/* mos, mou */ TfnP
#	define Afn	auto Tfn
#	define AfnP	auto TfnP
#	define Efn	extern Tfn
#	define EfnP	extern TfnP
#	define Rfn	register Tfn
#	define RfnP	register TfnP
#	define Sfn	static Tfn
#	define SfnP	static TfnP
#	define Pfn	/* parameter, value */ Tfn
#	define PfnP	/* parameter, value */ TfnP
#	define Xfn	/* extdef */ Tfn
#	define XfnP	/* extdef */ TfnP
