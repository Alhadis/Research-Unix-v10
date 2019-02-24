# define TNULL 0
# define FARG 1
# define CHAR 2
# define SHORT 3
# define INT 4
# define LONG 5
# define FLOAT 6
# define DOUBLE 7
# define STRTY 8
# define UNIONTY 9
# define ENUMTY 10
# define MOETY 11
# define UCHAR 12
# define USHORT 13
# define UNSIGNED 14
# define ULONG 15
# define VOID 16
# define UNDEF 17
# define BITS 18
# define UBITS 19

# define PTR  0040
# define FTN  0100
# define ARY  0140

# define MTMASK 03
# define BTMASK 037
# define BTSHIFT 5 
# define TSHIFT 2
# define TMASK (MTMASK<<BTSHIFT)

# define BTYPE(x)  (x&BTMASK)   /* basic type of x */
# define ISPTR(x) ((x&TMASK)==PTR)
# define ISFTN(x)  ((x&TMASK)==FTN)  /* is x a function type */
# define ISARY(x)   ((x&TMASK)==ARY)   /* is x an array type */
# define INCREF(x) (((x&~BTMASK)<<TSHIFT)|PTR|(x&BTMASK))
# define DECREF(x) (((x>>TSHIFT)&~BTMASK)|(x&BTMASK))
