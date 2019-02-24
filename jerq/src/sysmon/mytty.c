#include <stdio.h>
#include <string.h>

char *
mytty()
{
	static char buf[100];
	register char *p;
	FILE *fp;

	if ((fp = popen("logtty", "r")) == NULL)
		return (NULL);
	if (fgets(buf, sizeof(buf), fp) == NULL) {
		pclose(fp);
		return (NULL);
	}
	if (pclose(fp) != 0)
		return (NULL);
	if ((p = strchr(buf, '\n')) == NULL)
		return (NULL);
	*p = 0;
	return (buf);
}
