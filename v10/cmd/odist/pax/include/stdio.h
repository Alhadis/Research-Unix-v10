/*
 * AT&T Bell Laboratories
 *
 * sfio stdio source emulation
 */

#ifndef _STDIO_H_

#if __cplusplus || c_plusplus

#include <../CC/stdio.h>

#ifndef _STDIO_H_
#define _STDIO_H_
#endif

#else

#define _STDIO_H_

#include <sfio.h>

#define _IOFBF		0
#define _IONBF		1
#define _IOLBF		2
#define L_ctermid	9
#define L_cuserid	9
#define P_tmpdir	"/usr/tmp/"
#define L_tmpnam	(sizeof(P_tmpdir)+15)

#if __cplusplus || c_plusplus
extern "C"
{
#endif

extern char	*ctermid _SFA_((char*));
extern char	*cuserid _SFA_((char*));
extern char	*tmpnam _SFA_((char*));
extern char	*tempnam _SFA_((char*));
extern char	*_stdgets _SFA_((char*, int n));
extern int	_stdprintf _SFA_((const char*, ...));
extern int	_stdsprintf _SFA_((char*, const char*, ...));
extern int	_stdscanf _SFA_((const char*, ...));
extern int	_stdsetvbuf _SFA_((Sfile_t*, char*, int, int));

#if __cplusplus || c_plusplus
}
#endif

#define stdin		sfstdin
#define stdout		sfstdout
#define stderr		sfstderr
#define FILE		Sfile_t
#define BUFSIZ		SF_BUFSIZE

#define fopen(f,m)	sfopen((Sfile_t*)0,f,m)
#define fdopen(fd,m)	sfdopen(fd,m)
#define freopen(f,m,p)	sfopen(p,f,m)
#define tmpfile()	sftmpfile()
#define popen(cmd,m)	sfpopen(cmd,m,(Sfile_t**)0)
#define fclose(f)	sfclose(f)
#define pclose(f)	sfclose(f)

#define fwrite(p,s,n,f)	((_Sfi = sfwrite(f,p,(s)*(n))) <= 0 ? _Sfi : _Sfi/(s))
#define fputc(c,f)	sfputc(f,c)
#define putc(c,f)	sfputc(f,c)
#define putw(w,f)	(_Sfi = (int)w, sfwrite(f,&_Sfi,sizeof(int)) <= 0 ? 1 : 0)
#define putchar(c)	sfputc(sfstdout,c)
#define fputs(s,f)	sfputs(f,s,0)
#define puts(s)		sfputs(sfstdout,s,'\n')
#define fprintf		sfprintf
#define vfprintf	sfvprintf
#define vprintf(f,a)	sfvprintf(sfstdout,f,a)
#define vsprintf	_stdvsprintf
#define	printf		_stdprintf
#define sprintf		_stdsprintf

#define fread(p,s,n,f)	((_Sfi = sfread(f,p,(s)*(n))) <= 0 ? _Sfi : _Sfi/(s))
#define fgetc(f)	sfgetc(f)
#define getc(f)		sfgetc(f)
#define getw(f)		(sfread(f,&_Sfi,sizeof(int)) == sizeof(int) ? _Sfi : -1)
#define getchar()	sfgetc(sfstdin)
#define ungetc(c,f)	sfungetc(f,c)
#define fgets(s,n,f)	sfgets(f,s,n)
#define _SIZEOF(s)	(sizeof(s) != sizeof(char*) ? sizeof(s) : BUFSIZ)
#define gets(s)		_stdgets(s,_SIZEOF(s))
#define fscanf		sfscanf
#define vfscanf		sfvscanf
#define sscanf		sfsscanf
#define vscanf(f,a)	sfvscanf(sfstdin,f,a)
#define scanf		_stdscanf
#define vsscanf		_stdvssanf

#define fflush(f)	sfsync(f)
#define fseek(f,o,t)	(sfseek(f,o,t) < 0L ? -1 : 0)
#define rewind(f)	sfseek((f),0L,0)
#define ftell(f)	sftell(f)
#define setbuf(f,b)	sfsetbuf(f,b,(b) ? BUFSIZ : 0)
#define setbuffer(f,b,n) sfsetbuf(f,b,n)
#define setlinebuf(f)	sfsetflag(f,SF_LINE,1)
#define setvbuf		_stdsetvbuf

#define fileno(f)	sffileno(f)
#define feof(f)		sfeof(f)
#define ferror(f)	sferror(f)
#define clearerr(f)	(sfclearerr(f),sfclearlock(f))

#endif

#endif
