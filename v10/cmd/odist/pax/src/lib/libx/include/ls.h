/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * ls formatter interface definitions
 */

#ifndef LS_USER

#include <fsinfo.h>

#ifndef S_IRWXU
#ifndef S_IREAD
#define S_IREAD 	00400
#define S_IWRITE	00200
#define S_IEXEC		00100
#endif
#ifndef S_ISUID
#define S_ISUID		04000
#endif
#ifndef S_ISGID
#define S_ISGID		02000
#endif
#ifndef S_ISVTX
#define S_ISVTX		01000
#endif
#ifndef S_IRUSR
#define S_IRUSR		S_IREAD
#define S_IWUSR		S_IWRITE
#define S_IXUSR		S_IEXEC
#define S_IRGRP		(S_IREAD>>3)
#define S_IWGRP		(S_IWRITE>>3)
#define S_IXGRP		(S_IEXEC>>3)
#define S_IROTH		(S_IREAD>>6)
#define S_IWOTH		(S_IWRITE>>6)
#define S_IXOTH		(S_IEXEC>>6)
#endif

#define S_IRWXU		(S_IRUSR|S_IWUSR|S_IXUSR)
#define S_IRWXG		(S_IRGRP|S_IWGRP|S_IXGRP)
#define S_IRWXO		(S_IROTH|S_IWOTH|S_IXOTH)
#endif

#ifdef stat_blocks
#define tblocks(p)	(((p)->st_blocks+1)/2)
#else
#define tblocks(p)	iblocks(p)
#if __STDC__ || __cplusplus || c_plusplus
#if __cplusplus
extern "C" {
#endif
extern long		iblocks(struct stat*);
#if __cplusplus
}
#endif
#else
extern long		iblocks();
#endif
#endif

#define LS_ATIME	(1<<0)		/* list st_atime		*/
#define LS_BLOCKS	(1<<1)		/* list blocks used by file	*/
#define LS_CTIME	(1<<2)		/* list st_ctime		*/
#define LS_INUMBER	(1<<3)		/* list st_ino			*/
#define LS_LONG		(1<<4)		/* long listing			*/
#define LS_MARK		(1<<5)		/* append file name marks	*/
#define LS_NOGROUP	(1<<6)		/* omit group name for LS_LONG	*/
#define LS_NOUSER	(1<<7)		/* omit user name for LS_LONG	*/

#define LS_USER		(1<<8)		/* first user flag bit		*/

#define LS_W_BLOCKS	5		/* LS_BLOCKS field width	*/
#define LS_W_INUMBER	7		/* LS_INUMBER field width	*/
#define LS_W_LONG	55		/* LS_LONG width (w/o names)	*/
#define LS_W_LINK	4		/* link text width (w/o names)	*/
#define LS_W_MARK	1		/* LS_MARK field width		*/
#define LS_W_NAME	9		/* group|user name field width	*/

#if __STDC__ || __cplusplus || c_plusplus
#if __cplusplus
extern "C" {
#endif
extern char*	fmtls(char*, const char*, struct stat*, const char*, const char*, int);
#if __cplusplus
}
#endif
#else
extern char*	fmtls();
#endif

#endif
