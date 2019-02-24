/*
 * libx system(3)
 */

extern int	cmdrun();

int
system(cmd)
char*	cmd;
{
	char*	sh[4];

	sh[0] = "sh";
	sh[1] = "-c";
	sh[2] = cmd;
	sh[3] = 0;
	return(cmdrun((char*)0, sh));
}
