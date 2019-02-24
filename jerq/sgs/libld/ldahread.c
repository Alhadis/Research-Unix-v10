/*
* ldahread - fill archive file member header info.
*
* #ifdef PORTAR		printable ascii header archive version
* #else #ifdef PORT5AR	UNIX 5.0 semi-portable archive version
* #else			pre-UNIX 5.0 (old) archive version
* #endif
*/

#include <stdio.h>
#include <ar.h>
#include "filehdr.h"
#include "ldfcn.h"

int
ldahread(ldptr, arhead)
	LDFILE *ldptr;
	ARCHDR *arhead; 
{
	extern int fseek();
	extern int fread();
	extern long sgetl();
	extern int vldldptr();

#ifdef PORTAR
	struct ar_hdr arbuf;

	if (vldldptr(ldptr) == SUCCESS && TYPE(ldptr) == ARTYPE &&
		FSEEK(ldptr, -((long)sizeof(arbuf)), BEGINNING) == OKFSEEK &&
		FREAD((char *)&arbuf, sizeof(arbuf), 1, ldptr) == 1 &&
		!strncmp(arbuf.ar_fmag, ARFMAG, sizeof(arbuf.ar_fmag)))
	{
		register char *cp = arbuf.ar_name + sizeof(arbuf.ar_name);

		while (*--cp == ' ')
			;
		if (*cp == '/')
			*cp = '\0';
		else
			*++cp = '\0';
		(void)strncpy(arhead->ar_name, arbuf.ar_name,
			sizeof(arbuf.ar_name));
		if (sscanf(arbuf.ar_date, "%ld", &arhead->ar_date) == 1 &&
			sscanf(arbuf.ar_uid, "%d", &arhead->ar_uid) == 1 &&
			sscanf(arbuf.ar_gid, "%d", &arhead->ar_gid) == 1 &&
			sscanf(arbuf.ar_mode, "%o", &arhead->ar_mode) == 1 &&
			sscanf(arbuf.ar_size, "%ld", &arhead->ar_size) == 1)
		{
			return (SUCCESS);
		}
	}
#else
#ifdef PORT5AR
	struct arf_hdr arbuf;

	if (vldldptr(ldptr) == SUCCESS && TYPE(ldptr) == ARTYPE &&
		FSEEK(ldptr, -((long)sizeof(arbuf)), BEGINNING) == OKFSEEK &&
		FREAD((char *)&arbuf, sizeof(arbuf), 1, ldptr) == 1)
	{
		(void)strncpy(arhead->ar_name, arbuf.arf_name,
			sizeof(arbuf.arf_name));
		arhead->ar_date = sgetl(arbuf.arf_date);
		arhead->ar_uid = sgetl(arbuf.arf_uid);
		arhead->ar_gid = sgetl(arbuf.arf_gid);
		arhead->ar_mode = sgetl(arbuf.arf_mode);
		arhead->ar_size = sgetl(arbuf.arf_size);
		return (SUCCESS);
	}
#else
	if (vldldptr(ldptr) == SUCCESS && TYPE(ldptr) == ARTYPE &&
		FSEEK(ldptr, -((long) ARCHSZ), BEGINNING) == OKFSEEK &&
		FREAD((char *)arhead, ARCHSZ, 1, ldptr) == 1)
	{
		return (SUCCESS);
	}
#endif
#endif
	return (FAILURE);
}

static char ID[] = "@(#) ldahread.c: 1.4 3/3/83";
