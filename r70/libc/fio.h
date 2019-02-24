#define	FIOBSIZE	4096
#define	NOFILE		20

typedef struct
{
	char	*next;			/* next char to be used */
	char	*end;			/* first invalid char */
	char	*lnext;			/* previous value of next */
	short	flags;			/* see FIO_.* below */
	short	oflush;			/* if >= 0 fd to flush on read */
	long	offset;			/* seek of end */
	char	buf[FIOBSIZE];
} Fbuffer;
extern Fbuffer*	Ffb[NOFILE];

#define	FIORESET(f)	((f)->next=(f)->lnext=(f)->end=(f)->buf, (f)->flags&= ~FIO_MALLOCED, (f)->oflush= -1)
#define	FIOSET(f, fd)	if((f=Ffb[fd&=0x7f]) == 0){Finit(fd,(char *)0);f=Ffb[fd];}
/* FIOLINELEN is length of last input */
#define	FIOLINELEN(fd)	(((int)(Ffb[fd]->next - Ffb[fd]->lnext))-1)
/* FIOSEEK is lseek of next char to be processed */
#define	FIOSEEK(fd)	(Ffb[fd]->offset - (Ffb[fd]->end - Ffb[fd]->next))
#define	FIOFLUSH(fd)	if((fd >= 0) && Ffb[fd]) Fflush(fd)

#define	FIO_RDLAST	0x0001		/* true if last input was rdline */
#define	FIO_WRITING	0x0002		/* true if last action was write */
#define	FIO_MALLOCED	0x0004		/* set if Finit did the malloc */

extern	void	Finit(int, void*);
extern	void	Fclose(int);
extern	char*	Frdline(int);
extern	void	Fundo(int);
extern	int	Fgetc(int);
extern	int	Fputc(int, int);
extern	long	Fread(int, void*, long);
extern	long	Fwrite(int, void*, long);
extern	long	Fseek(int, long, int);
extern	int	Fflush(int);

extern	int	Ffillbuf(int);
extern	void	Fexit(int);
extern	int	F_flush(Fbuffer*, int);
extern	void	Ftie(int, int);

extern	int	Fprint(int, char *, ...);

