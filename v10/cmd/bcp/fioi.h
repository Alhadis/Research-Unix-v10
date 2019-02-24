/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */

/* fioi.h - buffered I/O of machine-, OS-, and compiler-independent binary values.

   Machine-, OS-, and compiler-independence of 'fioi' functions require that:
      1) chars are exactly 8 bits and their contents are invariant under I/O;
      2) the order of chars, as written by putc() and read by getc(), is
	 also invariant, across all machines, OS, and compilers.
   Both of these requirements are customarily met by UNIX OS & tools.

   All values are written/read as a stream of char's, ultimately
   using putc() & getc() macroes.
   To maximize speed, macroes are used wherever possible:
   -  the first argument F of each macro is a stream (FILE *).
   -  write macroes have the form fwri_TYPE(F,value); `value' is assigned to
      a variable of type TYPE before it is written (so compiler-dependent type
      coercions may apply).
   - read macroes have the form frdi_TYPE(F), returning a value of type TYPE.
   Arguments to these macroes are evaluated exactly once.
   */

/** MACHINE-DEPENDENT CUSTOMIZATION OF TYPES **/
/* For each machine, all these types must be supported in the sense that
   some roughly-equivalent machine-supported type exists.  If an exact
   equivalent doesn't exist, pick the shortest that is at least as long
   (in no. bytes), if possible.  If there is no machine representation
   at least as long as specified, the only penalty will be the inevitable one:
   when a value is read, it may be truncated to the maximum value that can be
   represented.
   */
#define int1 char		/* 8-bit signed 2's complement integer */
#define int2 short		/* 16-bit signed 2's complement integer */
#define int3 int		/* 24-bit signed 2's complement integer */
#define int4 int		/* 32-bit signed 2's complement integer */
#define int8 long		/* 64-bit signed 2's complement integer */
#define uint1 unsigned char	/* 8-bit unsigned integer */
#define uint2 unsigned short	/* 16-bit unsigned integer */
#define uint3 unsigned int	/* 24-bit unsigned integer */
#define uint4 unsigned int	/* 32-bit unsigned integer */
#define uint8 unsigned long	/* 64-bit unsigned integer */

/* IEEE 64-bit floating point format */
typedef struct Ieeed {
	int8 l;
	int8 h;
	} Ieeed;

/* Cope with unsigned chars on 3B2 & compensate for bug on MIPS */
#if CPU!=ATT3B && CPU!=MIPS
#define toint1(c) (int1)(c)
#else
#if MAIN
int toint1(c)
    register int c;
{	if ( ((1<<7)&c) !=0 ) return(c-256);
	else return c;
	}
#else
extern int toint1();
#endif
#endif

/** THE REST OF THIS FILE IS MACHINE-, OS-, and COMPILER-INDEPENDENT **/

typedef struct Fioi {
	int1 i1;
	int2 i2;
	int3 i3;
	int4 i4;
	int8 i8;
	uint1 ui1;
	uint2 ui2;
	uint3 ui3;
	uint4 ui4;
	uint8 ui8;
	char str[100];
	} Fioi;
#define Init_Fioi {0,0,0,0,0,0,0,0,0,0,""}
#if MAIN
Fioi _Fioi = Init_Fioi;
#else
extern Fioi _Fioi;
#endif

/* Characters */
#define fwri_ch(F,V) putc((V),(F))
#define frdi_ch(F) getc((F))

#if T	/* TRANSITIONAL:  used to read:  "#if FWRI" */
/* Strings are '\0'-terminated both in main memory & in peripheral files. */
#define fwri_str(F,S) { fputs((S),(F)); putc('\0',(F)); }
#else
/* Strings are '\0'-terminated in the variable and '\n'-terminated in the file.
   BAD, OBSOLESCENT POLICY:  both should use '\0'-termination */
#define fwri_str(F,S) { fputs((S),(F)); putc('\n',(F)); }
#endif

#define MAX_FIO_STRLEN (128)

#if T	/* TRANSITIONAL:  used to read:  "#if FRDI" */

/* Strings are '\0'-terminated both in main memory & in peripheral files. */
/*** TRANSITIONAL PATCH:  accept '\n' as a terminator also;  this will be disabled
     as soon as all classifier tables have been recreated with FWRI==T. ***/
/* Read string of maximum length MAX_FIO_STRLEN; return pointer to malloc space */
#if MAIN
char *frdi_str(f)
    FILE *f;
{   static char s[MAX_FIO_STRLEN+1];
    register int ch;
    register char *c,*ce;
    char *res;
	ce=(c=s)+MAX_FIO_STRLEN;
	while((c<ce)&&((ch=getc(f))!=EOF)&&(ch!='\0')&&(ch!='\n')) *(c++) = ch;
	*c='\0';
	if(c==ce) /* label is truncated; find end of it */ {
		while(((ch=getc(f))!=EOF)&&(ch!='\0')&&(ch!='\n')) ;
		};
	if((res=(char *)strdup(s))==NULL)
		abort("frdi_str: can't dup char *s[%d]",strlen(s));
	return(res);
	}
#else
char *frdi_str();
#endif
/* 's' is an already-allocated buffer of known length >=N (including '\0') */
#if MAIN
int frdi_strn(f,s,n)
    FILE *f;
    char s[];
    int n;
{   register int ch;
    register char *c,*ce;
	ce=(c=s)+n-1;
	while((c<ce)&&((ch=getc(f))!=EOF)&&(ch!='\0')&&(ch!='\n')) *(c++) = ch;
	*c='\0';
	if(c==ce) /* label is truncated; find end of it */ {
		while(((ch=getc(f))!=EOF)&&(ch!='\0')&&(ch!='\n')) ;
		};
	if(ch==EOF) return(0); else if(ferror(f)) return(-errno); else return(1);
	}
#else
int frdi_strn();
#endif

#else

/* Strings are '\0'-terminated in the variable and '\n'-terminated in the file.
   BAD, OBSOLESCENT POLICY:  both should use '\0'-termination */
/* Read string of maximum length MAX_FIO_STRLEN; return pointer to malloc space */
#if MAIN
char *frdi_str(f)
    FILE *f;
{   static char s[MAX_FIO_STRLEN+1];
    register int ch;
    register char *c,*ce;
    char *res;
	ce=(c=s)+MAX_FIO_STRLEN;
	while((c<ce)&&((ch=getc(f))!=EOF)&&(ch!='\n')) *(c++) = ch;
	*c='\0';
	if(c==ce) /* label is truncated; find end of it */ {
		while(((ch=getc(f))!=EOF)&&(ch!='\n')) ;
		};
	if((res=(char *)strdup(s))==NULL)
		abort("frdi_str: can't dup char *s[%d]",strlen(s));
	return(res);
	}
#else
char *frdi_str();
#endif
/* S is an already-allocated buffer of known length >=N (including '\0') */
#define frdi_strn(F,S,N) ( feof(F)? 0 : ( \
	fgets((S),(N),(F)), \
	( (((S)[strlen((S))-1])=='\n')? (S)[strlen((S))-1]='\0': 1 ), \
	(ferror(F)? -errno: 1) ) )

#endif

/* Integers (unsigned and signed) are written low-order byte first. */
#define fwri_uint1(F,V) fwri_ch((F),(uint1)(V))
#define frdi_uint1(F) ((uint1)frdi_ch((F)))

#define fwri_uint2(F,V) { fwri_uint1((F),_Fioi.ui2=(V)); \
			fwri_uint1((F),_Fioi.ui2>>8); }
#define frdi_uint2(F) ((uint2)( _Fioi.ui2=frdi_uint1((F)), \
			(frdi_uint1((F))<<8)|_Fioi.ui2 ))

#define fwri_uint3(F,V) { fwri_uint2((F),_Fioi.ui3=(V)); \
			fwri_uint1((F),_Fioi.ui3>>16); }
#define frdi_uint3(F) ((uint3)( _Fioi.ui3=frdi_uint2((F)), \
			(frdi_uint1((F))<<16)|_Fioi.ui3 ))

#define fwri_uint4(F,V) { fwri_uint2((F),_Fioi.ui4=(V)); \
			fwri_uint2((F),_Fioi.ui4>>16); }
#define frdi_uint4(F) ((uint4)( _Fioi.ui4=frdi_uint2((F)), \
			(frdi_uint2((F))<<16)|_Fioi.ui4 ))

#define fwri_uint8(F,V) { fwri_uint4((F),_Fioi.ui8=(V)); \
			fwri_uint4((F),_Fioi.ui8>>32); }
#define frdi_uint8(F) ((uint8)( _Fioi.ui8=frdi_uint4((F)), \
			(frdi_uint4((F))<<32)|_Fioi.ui8 ))

/* Signed integers are assumed to be 2's complement.
   Note the suppression of inappropriate sign-extensions using judicious
   unsigned reads. */

#define fwri_int1(F,V) fwri_ch((F),_Fioi.i1=(V))
#define frdi_int1(F) (toint1(frdi_ch((F))))

#define fwri_int2(F,V) { fwri_int1((F),_Fioi.i2=(V)); \
			fwri_int1((F),_Fioi.i2>>8); }
#define frdi_int2(F) ((int2)( _Fioi.i2=frdi_uint1((F)), \
			(frdi_int1((F))<<8)|_Fioi.i2 ))

#define fwri_int3(F,V) { fwri_int2((F),_Fioi.i3=(V)); \
			fwri_int1((F),_Fioi.i3>>16); }
#define frdi_int3(F) ((int3)( _Fioi.i3=frdi_uint2((F)), \
			(frdi_int1((F))<<16)|_Fioi.i3 ))

#define fwri_int4(F,V) { fwri_int2((F),_Fioi.i4=(V)); \
			fwri_int2((F),_Fioi.i4>>16); }
#define frdi_int4(F) ((int4)( _Fioi.i4=frdi_uint2((F)), \
			(frdi_int2((F))<<16)|_Fioi.i4 ))

#define fwri_int8(F,V) { fwri_int4((F),_Fioi.i8=(V)); \
			fwri_int4((F),_Fioi.i8>>32); }
#define frdi_int8(F) ((int8)( _Fioi.i8=frdi_uint4((F)), \
			(frdi_int4((F))<<32)|_Fioi.i8 ))


/* Floating point */
#if MAIN
/* See Marsha Grabow & George Gilmer & KT */
void
dtoieeed(ieee,native)
    Ieeed *ieee;
    double native;
{
	double fr, ho, f;
	int iexp;

	if(native < 0) {
		dtoieeed(ieee, -native);
		ieee->h |= 0x80000000L;
		return;
	}
	if(native == 0) {
		ieee->l = 0;
		ieee->h = 0;
		return;
	}
	fr = frexp(native, &iexp);
	f = 2097152L;		/* shouldnt use fp constants here */
	fr = modf(fr*f, &ho);
	ieee->h = ho;
	ieee->h &= 0xfffffL;
	ieee->h |= (iexp+1022L) << 20;
	f = 65536L;
	fr = modf(fr*f, &ho);
	ieee->l = ho;
	ieee->l <<= 16;
	ieee->l |= (long)(fr*f);
}
#else
void dtoieeed();
#endif
