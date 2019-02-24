#include "curses.ext"
/*	@(#) traceonoff.c: 1.1 10/15/83	(1.13	8/20/82)	*/

traceon()
{
#ifdef DEBUG
	if (outf == NULL) {
		outf = fopen("trace", "a");
		if (outf == NULL) {
			perror("trace");
			exit(-1);
		}
		fprintf(outf, "trace turned on\n");
	}
#endif
}

traceoff()
{
#ifdef DEBUG
	if (outf != NULL) {
		fprintf(outf, "trace turned off\n");
		fclose(outf);
		outf = NULL;
	}
#endif
}
