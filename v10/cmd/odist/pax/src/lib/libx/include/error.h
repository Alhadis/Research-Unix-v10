/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * option, error and message formatter external definitions
 */

#ifndef ERROR_INFO

#include <option.h>
#include <errno.h>

#ifndef VOID
#define VOID	char
#endif

#define ERROR_INFO	0		/* info message -- no err_id	*/
#define ERROR_WARNING	1		/* warning message		*/
#define ERROR_ERROR	2		/* error message -- no err_exit	*/
#define ERROR_FATAL	3		/* error message with err_exit	*/
#define ERROR_PANIC	ERROR_LEVEL	/* panic message with err_exit	*/

#define ERROR_LEVEL	000077		/* level portion of status	*/
#define ERROR_SYSTEM	000100		/* report system errno message	*/
#define ERROR_OUTPUT	000200		/* next arg is error fd		*/
#define ERROR_SOURCE	000400		/* next 2 args are FILE,LINE	*/
#define ERROR_USAGE	001000		/* usage message		*/
#define ERROR_PROMPT	002000		/* omit trailing newline	*/
#define ERROR_NOID	004000		/* omit err_id			*/
#define ERROR_LIBRARY	010000		/* library routine error	*/

#if __cplusplus
extern "C" {
#endif

extern int	errno;			/* system call error status	*/

#if __cplusplus
}
#endif

#if __STDC__ || __cplusplus || c_plusplus
#include <stdarg.h>
#if __cplusplus
extern "C" {
#endif
extern char*	seterror(int, ...);
extern void	error(int, ...);
extern void	liberror(const char*, int, ...);
extern void	verror(const char*, int, va_list);
#if __cplusplus
}
#endif
#else
extern char*	seterror();
extern void	error();
extern void	liberror();
extern void	verror();
#endif

#endif
