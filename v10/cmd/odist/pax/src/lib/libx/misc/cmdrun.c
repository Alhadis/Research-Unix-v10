/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * cmdopen() + cmdclose()
 * no env mods
 * no redirection
 * effective=real
 */

extern int	cmdopen();
extern int	cmdclose();

int
cmdrun(cmd, argv)
char*	cmd;
char**	argv;
{
	return(cmdclose(cmdopen(cmd, argv, (char**)0, (int**)0, "gu")));
}
