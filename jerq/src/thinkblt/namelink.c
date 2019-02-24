char *
namelink(envname)
char *envname;
{
	char *linkname;
	char *p, *malloc(), *strcat(), *strdup(), *getenv();
	if (p = getenv(envname))
		return strdup(p);
	else if (p = getenv("HOME")) {
		linkname = malloc(strlen(p)+2+strlen(envname)+1);
		strcpy(linkname, p);
		strcat(linkname, "/.");
		return strcat(linkname, envname);
	} else
		return 0;
}
