/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * command line option parse assist definitions
 */

#ifndef __OPTION_H__
#define __OPTION_H__

#if __cplusplus
extern "C" {
#endif

extern char*	opt_arg;		/* {:,#} string argument	*/
extern int	opt_again;		/* see cmdargs()		*/
extern char**	opt_argv;		/* most recent argv		*/
extern int	opt_char;		/* char pos in argv[opt_index]	*/
extern int	opt_index;		/* argv index			*/
extern char*	opt_msg;		/* error/usage message buffer	*/
extern long	opt_num;		/* # numeric argument		*/
extern char	opt_option[3];		/* current flag {-,+} + option  */
extern int	opt_pchar;		/* prev opt_char for backup	*/
extern int	opt_pindex;		/* prev opt_index for backup	*/

#if __cplusplus
}
#endif

#if __STDC__ || __cplusplus || c_plusplus
#if __cplusplus
extern "C" {
#endif
extern int	optget(char**, const char*);
extern char*	optusage(const char*);
#if __cplusplus
}
#endif
#else
extern int	optget();
extern char*	optusage();
#endif

#endif
