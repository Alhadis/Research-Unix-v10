/*
 * uugrep <system>
 *  -- produce lines for named system out of Sysfiles;
 *     just cat all sys files if system not found in btree,
 *     or if btree is out of date
 *
 * uugrep
 * -- update btree if necessary
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cbt.h>	

#ifndef	INSDIR
#define	INSDIR	"/usr/lib/uucp"
#endif

char	Bsysf[] = "Bsystems";
char	BsysfT[] = "Bsystems.T";
char	Sysf[] = "Sysfiles";

main(argc, argv)
char **argv;
{
	bfile *bp;
	char keyval[32], valval[512];
	register len, n, anyfound;
	mbuf key, val, outkey;

	chdir(INSDIR);
	if (outofdate()) {
		if (argc < 2)
			execl("./uusysmake","uusysmake",(char *)NULL);
		catfiles();
		exit(0);
	}	
	bp = bopen(Bsysf, 0);
	if (bp == NULL) {
		catfiles();
		exit(0);
	}
	if (argc < 1)
		exit(0);
	len = strlen(argv[1]);
	if (len > 30)
		exit(1);
	strcpy(keyval, argv[1]);
	keyval[len] = '.';
	key.mlen = len+2;
	key.mdata = keyval;
	val.mdata = valval;
	anyfound = 0;
	for (keyval[len+1] = 'A'; keyval[len+1]<='Z'; keyval[len+1]++) {
		n = bseek(bp, key);
		if (n != 1)
			break;
		if (bread(bp, (mbuf *)NULL, &val) != 0)
			break;
		printf("%.*s\n", val.mlen, val.mdata);
		anyfound++;
	}
	if (anyfound==0)
		catfiles();
	exit(0);
}

outofdate()
{
	char sfile[128];
	struct stat sbuf;
	register FILE *mfp;
	time_t t;
	register char *cp;
	extern char *strchr();

	if (stat(BsysfT, &sbuf) < 0)
		return(1);
	t = sbuf.st_mtime;
	mfp = fopen(Sysf, "r");
	if (mfp == NULL)
		return(1);
	if (fstat(fileno(mfp), &sbuf) < 0)
		goto ood;
	if (sbuf.st_mtime >= t)
		goto ood;
	while (fgets(sfile, 128, mfp)) {
		cp = strchr(sfile, '\n');
		if (cp)
			*cp = '\0';
		if (stat(sfile, &sbuf) != 0)
			continue;		/* nonexistent OK */
		if (sbuf.st_mtime >= t)
			goto ood;
	}
	fclose(mfp);
	return(0);
ood:
	fclose(mfp);
	return(1);
}

catfiles()
{
	char sfile[128];
	register FILE *mfp, *fp;
	register c;
	register char *cp;
	extern char *strchr();

	mfp = fopen(Sysf, "r");
	if (mfp == NULL)
		return;
	while (fgets(sfile, 128, mfp)) {
		cp = strchr(sfile, '\n');
		if (cp)
			*cp = '\0';
		fp = fopen(sfile, "r");
		if (fp == NULL)
			continue;
		while ((c = getc(fp)) != EOF)
			if (putchar(c) == EOF)
				exit(1);
		fclose(fp);
	}
}
