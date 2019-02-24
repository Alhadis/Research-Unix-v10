/*
 * crack /etc/fstab; for mount, fsck, et al
 */

#include <fstab.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

static	struct	fstab fs;
static	FILE	*fs_file = 0;

static	int
fstabscan(fsp)
register struct	fstab *fsp;
{
	char *args[FSTABNARGS];
	char buf[256];

again:
	if (fgets(buf, sizeof(buf), fs_file) == NULL)
		return(0);
	buf[strlen(buf)-1] = 0;	/* discard \n */
	setfields(":");
	if (getfields(buf, args, FSTABNARGS) != FSTABNARGS)
		goto again;
	strcpy(fsp->fs_spec, args[0]);
	strcpy(fsp->fs_file, args[1]);
	if (isdigit(args[2][0]) || args[2][0] == '-') {
		fsp->fs_ftype = atoi(args[2]);
		fsp->fs_flags = atoi(args[3]);
	} else if (strcmp(args[2], "rw") == 0) {
		fsp->fs_ftype = 0;
		fsp->fs_flags = 0;
	} else if (strcmp(args[2], "ro") == 0) {
		fsp->fs_ftype = 0;
		fsp->fs_flags = 1;
	} else if (strcmp(args[2], "sw") == 0) {
		fsp->fs_ftype = FSSWAP;
		fsp->fs_flags = 0;
	} else {
		fsp->fs_ftype = FSNONE;
		fsp->fs_flags = 0;
	}
	fsp->fs_passno = atoi(args[4]);
	return (1);
}
	
int
setfsent()
{
	if (fs_file)
		endfsent();
	if ((fs_file = fopen(FSTAB, "r")) == NULL)
		return(0);
	return(1);
}

int endfsent()
{
	if (fs_file)
		fclose(fs_file);
	return(1);
}

struct fstab *
getfsent()
{

	if ((fs_file == 0) && (setfsent() == 0))
		return(0);
	if (fstabscan(&fs) == 0)
		return (0);
	return (&fs);
}

struct fstab *
getfsspec(name)
char	*name;
{
	register struct fstab *fsp;

	if (setfsent() == 0)	/* start from the beginning */
		return(0);
	while((fsp = getfsent()) != 0){
		if (strcmp(fsp->fs_spec, name) == 0)
			return(fsp);
	}
	return(0);
}

struct fstab *
getfsfile(name)
	char	*name;
{
	register struct fstab *fsp;

	if (setfsent() == 0)	/* start from the beginning */
		return(0);
	while ((fsp = getfsent()) != 0){
		if (strcmp(fsp->fs_file, name) == 0)
			return(fsp);
	}
	return(0);
}
