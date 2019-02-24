/*	@(#)stdio.h	84/11/10	*/
#ifndef STDIO_H
#define STDIO_H 1
#define	BUFSIZ	4096
#define	_NFILE	120

# ifndef FILE
extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[_NFILE];
# endif

#define	_IOREAD 01
#define	_IOWRT 02
#define	_IONBF 04
#define	_IOMYBUF 010
#define	_IOEOF 020
#define	_IOERR 040
#define	_IOSTRG 0100
#define	_IOLBF 0200
#define	_IORW 0400
#define	NULL 0
#define	FILE struct _iobuf
#define	EOF (-1)

extern int _flsbuf(unsigned,FILE*);
extern int _filbuf(FILE*);

#define	stdin (&_iob[0])
#define	stdout (&_iob[1])
#define	stderr (&_iob[2])
#define	getc(p) (--(p)->_cnt>=0? *(p)->_ptr++&0377:_filbuf(p))
#define	getchar() getc(stdin)
#define putc(x,p) (--(p)->_cnt>=0? ((int)(*(p)->_ptr++=(unsigned)(x))):_flsbuf((unsigned)(x),p))
#define	putchar(x) putc(x,stdout)
#define	feof(p) (((p)->_flag&_IOEOF)!=0)
#define	ferror(p) (((p)->_flag&_IOERR)!=0)
#define	fileno(p) ((p)->_file)

extern FILE* fopen(const char*, const char*);
extern FILE* fdopen(int, const char*);
extern FILE* freopen(const char*, const char*, FILE*);
extern long ftell(const FILE*);
extern char* fgets(char*, int, FILE*);

#define	P_tmpdir "/tmp/"
#define	L_tmpnam (sizeof(P_tmpdir) + 15)

extern char* gets(char*);
extern puts(const char*);
extern fputs(const char*, FILE*);
extern int printf(const char* ...);
extern int fprintf(FILE*, const char* ...);
extern int sprintf(char*, const char* ...);
extern int scanf(const char* ...);
extern int fscanf(FILE*, const char* ...);
extern int sscanf(char*, const char* ...);
extern int fread(char*, int, int, FILE*);
extern int fwrite(const char*, int, int, FILE*);
extern int fclose(FILE*);
extern fflush(FILE*);
extern clearerr(FILE*);
extern int fseek(FILE*, long, int);
extern int rewind(FILE*);
extern int getw(FILE*);
extern int fgetc(FILE*);
extern FILE* popen(const char*, const char*);
extern int pclose(FILE*);
extern int putw(int, FILE*);
extern int fputc(int, FILE*);
extern setbuf(FILE*, char*);
extern int ungetc(int, FILE*);

extern void exit(int);
extern int abort(...);

extern int atoi(const char*);
extern double atof(const char*);
extern long atol(const char*);
#endif
