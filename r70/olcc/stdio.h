#include <stddef.h>

#define	BUFSIZ	4096
#define	_NFILE	120
extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[_NFILE];

#define	_IOREAD	01
#define	_IOWRT	02
#define	_IONBF	04
#define	_IOMYBUF	010
#define	_IOEOF	020
#define	_IOERR	040
#define	_IOSTRG	0100
#define	_IOLBF	0200
#define	_IORW	0400
#define	NULL	0
#define	FILE	struct _iobuf
#define	EOF	(-1)

#define	stdin	(&_iob[0])
#define	stdout	(&_iob[1])
#define	stderr	(&_iob[2])
#define	getc(p)		(--(p)->_cnt>=0? (int)*(p)->_ptr++:_filbuf(p))
#define	getchar()	getc(stdin)
#define putc(x,p) (--(p)->_cnt>=0? ((int)(*(p)->_ptr++=(x))):_flsbuf((x),p))
#define	putchar(x)	putc(x,stdout)
#define	feof(p)		(((p)->_flag&_IOEOF)!=0)
#define	ferror(p)	(((p)->_flag&_IOERR)!=0)
#define	fileno(p)	((p)->_file)

#define	TMP_MAX		25
#define FOPEN_MAX	40
#define FILENAME_MAX	120
#define SEEK_CUR	1	/* 3rd arg of fseek */
#define SEEK_END	2
#define SEEK_SET	0

typedef	long	fpos_t;

extern void	clearerr(FILE*);
extern int	fclose(FILE*);
extern FILE*	fdopen(int, char*);
extern int	fflush(FILE*);
extern int	fgetc(FILE*);
extern int	fgetpos(FILE *, fpos_t *);	/* like ftell */
extern char*	fgets(char*, int, FILE*);
extern FILE*	fopen(const char*, const char*);
extern int	fprintf(FILE*, char* ,...);
extern int	fputc(int, FILE*);
extern int	fputs(const char*, FILE*);
extern size_t	fread(void*, size_t, size_t, FILE*);
extern FILE*	freopen(const char*, const char*, FILE*);
extern int	fscanf(FILE*, const char* ,...);
extern int	fseek(FILE*, long, int);
extern int	fsetpos(FILE*, const fpos_t *);
extern long	ftell(FILE*);
extern size_t	fwrite(const void*, size_t, size_t, FILE*);
extern char*	gets(char*);
extern int	getw(FILE*);
extern int	pclose(FILE*);
/* extern void	perror(const char*); */
extern FILE*	popen(char*, char*);
extern int	printf(const char* ,...);
extern int	puts(char*);
extern int	putw(int, FILE*);
extern int	remove(const char *filename);
extern int	rename(const char *, const char *);
extern void	rewind(FILE*);
extern int	scanf(const char* ,...);
extern void	setbuf(FILE*, char*);
extern int	setvbuf(FILE *, char *, int mode, size_t size);
/* extern int	sprintf(char*, const char* ,...); */
extern int	sscanf(const char*, const char* ,...);
extern FILE*	tmpfile(void);
extern char*	tmpnam(char *s);
extern int	ungetc(int, FILE*);
extern int	vfprintf(FILE*, char* ,...);
extern int	vprintf(const char* ,...);
extern int	vsprintf(char*, const char* ,...);
