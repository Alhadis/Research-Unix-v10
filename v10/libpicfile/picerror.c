#include <picfile.h>
#include <stdio.h>
picerror(s)
char *s;
{
	if(PIC_error){
		if(s) fprintf(stderr, "%s: ", s);
		fprintf(stderr, "%s\n", PIC_error);
		PIC_error=0;
	}
	else
		perror(s);
}
