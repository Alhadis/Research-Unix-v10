#include <stdio.h>
#include <string.h>

extern FILE *popen();

/* use qns to canonicalize names */
void
canonsys(char *sys, char cansys[])
{
	char cmd[1000], *p;
	FILE *fp;

	if (strncmp(sys, "dk!", 3) == 0)
		sprintf(cmd, "qns value name %s,dk", sys + 3);
	else if (strncmp(sys, "tcp!", 4) == 0)
		sprintf(cmd, "qns value name %s,dom", sys + 4);
	else if (strchr(sys, '/'))
		sprintf(cmd, "qns value name %s,dk", sys);
	else {
		strcpy(cansys, sys);
		return;
	}
	fp = popen(cmd, "r");
	if (fgets(cmd, sizeof cmd, fp)) {
		strcpy(cansys, cmd);
		if (p = strchr(cansys, '\n'))
			*p = '\0';
	} else
		strcpy(cansys, sys);
	pclose(fp);
	return;
}
