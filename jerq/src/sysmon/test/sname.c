#include "defs.h"
#include <stdio.h>

/* imported */
char *malloc();
char *strcpy();
char *strcat();

/* get the system name */
char *
getsysname()
{
    int i, fd;
    char *cp;
    static char name[DKNAMELEN+1];

    /* read the system name */
    fd = open("/etc/whoami", 0);
    if (fd < 0)
	return (NULL);
    i = read(fd, name, DKNAMELEN);
    if (i <= 0) {
	return (NULL);
    }
    (void)close (fd);
    name[i] = NULL;
    for (cp = name; *cp != 0 && *cp != '\n'; cp++);
    *cp = NULL;
    return (name);
}


/* make a name from the given system name and suffix */
char *
sname(sysname, generic)
char *sysname;
char *generic;
{
    static char name[DKNAMELEN+1];
    int i,j;

    i = strlen(sysname);
    j = strlen (generic);
    if (i+j > DKNAMELEN) {
	if (j >= DKNAMELEN)
	    return (NULL);
	for (i = 0, j = DKNAMELEN - j; i <= j; i++) {
	    name[i] = sysname[i];
	}
	name[i] = 0;
    } else {
	(void)strcpy (name, sysname);
    }
    (void)strcat (name, generic);
    return (name);
}

/* ealloc with error reporting */
char *
ealloc(size)
int size;
{
    char *ptr;

    ptr = malloc ((unsigned)size);
    if (ptr == NULL) {
	perror ("ealloc");
	exit(-1);
    }
    return (ptr);
}

