/*   "@(#) update.c:  V 2.4  8/19/83" */

#include <stdio.h>

extern char	*getcwd();

main()
{
	char	filpath[BUFSIZ],
		BASIC[BUFSIZ],
		*path;
	FILE	*fp;
	
	gets(filpath);
	if (*filpath == '/')		/* full path */
		strcpy(BASIC, filpath);
	else
	{				/* create full path */
		path = getcwd((char *)NULL,BUFSIZ);
		strcpy(BASIC, path);
		free(path);
		strcat(BASIC, "/");
		strcat(BASIC, filpath);
	}
	if ((fp = fopen("path.c","w")) == NULL)
	{
		fprintf(stderr, "Cannot open path.c for output\n");
		exit(2);
	}
	fprintf(fp, "char BASIC[] = \"%s\";",BASIC);
	fclose(fp);
}
