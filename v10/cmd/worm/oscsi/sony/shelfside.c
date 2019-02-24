#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

int
shelfside(char *arg, char *err)
{
	char *oarg = arg;
	int shelf;

	if((*arg < '0') || (*arg > '9')){
usage:
		sprintf(err, "shelfside '%s' must be numa or numb", oarg);
		return(-1);
	}
	shelf = 0;
	while((*arg >= '0') && (*arg <= '9'))
		shelf = 10*shelf + *arg++ - '0';
	shelf <<= 1;
	if(*arg == 'a')
		;
	else if(*arg == 'b')
		shelf |= 1;
	else
		goto usage;
	if(*++arg)
		goto usage;
	return(shelf);
}
