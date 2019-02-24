
#define _IOFBF 0
#define _IOLBF 0200
#define _IONBF 04
#define BUFSIZ 4096
#define EOF (-1)

extern struct _iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[];

#define FILE struct _iobuf
#define FILENAME_MAX 256
#define FOPEN_MAX 100

typedef long fpos_t;

#define L_tmpnam 25
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0
#define	stdin (&_iob[0])
#define	stdout (&_iob[1])
#define	stderr (&_iob[2])
#define TMP_MAX 17576

extern int remove(char *filename);
extern int rename(char *old, char *new);
extern FILE *tmpfile(void);
extern char *tmpnam(char *s);
extern int fclose(FILE *stream);
extern int fflush(FILE *stream);
extern FILE *fopen(char *filename, char *mode);
extern FILE *freopen(char *filename, char *mode, FILE *stream);
extern void setbuf(FILE *stream, char *buf);
extern int setvbuf(FILE *stream, char *buf, int mode, int size);
extern int fprintf(FILE *stream, char *format, ...);
extern int fscanf(FILE *stream, char *format, ...);
extern int printf(char *format, ...);
extern int scanf(char *format, ...);
extern int sprintf(char *s, char *format, ...);
extern int sscanf(char *s, char *format, ...);
extern int fgetc(FILE *stream);
extern char *fgets(char *s, int n, FILE *stream);
extern int fputc(int c, FILE *stream);
extern int fputs(char *s, FILE *stream);
extern int getc(FILE *stream);
extern int getchar(void);
extern char *gets(char *s);
extern int putc(int c, FILE *stream);
extern int putchar(int c);
extern int puts(char *s);
extern int ungetc(int c, FILE *stream);
extern unsigned fread(void *ptr, unsigned size, unsigned nmemb, FILE *stream);
extern unsigned fwrite(void *ptr, unsigned size, unsigned nmemb, FILE *stream);
extern int fgetpos(FILE *stream, fpos_t *pos);
extern int fseek(FILE *stream, long int offset, int whence);
extern int fsetpos(FILE *stream, fpos_t *pos);
extern long int ftell(FILE *stream);
extern void rewind(FILE *stream);
extern void clearerr(FILE *stream);
extern int feof(FILE *stream);
extern int ferror(FILE *stream);
extern void perror(char *s);

#define	_IOEOF 020
#define	_IOERR 040

#define	getc(p) (--(p)->_cnt>=0? (int)*(p)->_ptr++:_filbuf(p))
#define putc(x,p) (--(p)->_cnt>=0? ((int)(*(p)->_ptr++=(x))):_flsbuf((x),p))
extern int _filbuf(FILE *), _flsbuf(unsigned, FILE *);
#define feof(p) ((p)->_flag&_IOEOF)
#define ferror(p) ((p)->_flag&_IOERR)
#define	clearerr(p) ((p)->_flag &= ~(_IOERR|_IOEOF))
#define	getchar() getc(stdin)
#define	putchar(x) putc(x,stdout)
#define	fileno(p)	((p)->_file)
