#include <string.h>

/* reduce tcp and dk names to their final component */
void
canonsys(char *sys, char cansys[])
{
	if (strncmp(sys, "dk!", 3) == 0)
		sys += 3;
	if (strncmp(sys, "tcp!", 4) == 0)
		sys += 4;
	if (strrchr(sys, '/'))
		sys = strrchr(sys, '/') + 1;
	strcpy(cansys, sys);
	if (strrchr(cansys, '.'))
		*strchr(cansys, '.') = '\0';
}
