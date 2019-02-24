/*
* ldclose - close current object file.
*		if current object file is an archive member,
*		set up for next object file from archive.
*
* #ifdef PORTAR		printable ascii headers archive version
* #else #ifdef PORT5AR	UNIX 5.0 semi-portable archive version
* #else			pre UNIX 5.0 (old) archive version
* #endif
*/
#include <stdio.h>
#include <ar.h>
#include "filehdr.h"
#include "ldfcn.h"

int
ldclose(ldptr)
	LDFILE *ldptr;
{
	extern int fseek();
	extern int fread();
	extern int fclose();
	extern int vldldptr();
	extern int freeldptr();
	extern long sgetl();

#ifdef PORTAR
	struct ar_hdr arhdr;
	long ar_size;

	if (vldldptr(ldptr) == FAILURE)
		return (SUCCESS);
	if (TYPE(ldptr) == ARTYPE &&
		FSEEK(ldptr, -((long)sizeof(arhdr)), BEGINNING) == OKFSEEK &&
		FREAD((char *)&arhdr, sizeof(arhdr), 1, ldptr) == 1 &&
		!strncmp(arhdr.ar_fmag, ARFMAG, sizeof(arhdr.ar_fmag)) &&
		sscanf(arhdr.ar_size, "%ld", &ar_size) == 1)
	{
		/*
		* Be sure OFFSET is even
		*/
		OFFSET(ldptr) += ar_size + sizeof(arhdr) + (ar_size & 01);
		if (FSEEK(ldptr, 0L, BEGINNING) == OKFSEEK &&
			FREAD((char *)&(HEADER(ldptr)), FILHSZ, 1, ldptr) == 1)
		{
			return (FAILURE);
		}
	}
#else
#ifdef PORT5AR
	struct arf_hdr arhdr;
	long ar_size, nsyms;

	if (vldldptr(ldptr) == FAILURE)
		return (SUCCESS);
	if (TYPE(ldptr) == ARTYPE &&
		FSEEK(ldptr, -((long)sizeof(arhdr)), BEGINNING) == OKFSEEK &&
		FREAD((char *)&arhdr, sizeof(arhdr), 1, ldptr) == 1)
	{
		ar_size = sgetl(arhdr.arf_size);
		/*
		* Be sure offset is even
		*/
		OFFSET(ldptr) += ar_size + sizeof(arhdr) + (ar_size & 01);
		if (FSEEK(ldptr, 0L, BEGINNING) == OKFSEEK &&
			FREAD((char *)&(HEADER(ldptr)), FILHSZ, 1, ldptr) == 1)
		{
			return (FAILURE);
		}
	}
#else
	ARCHDR arhdr;

	if (vldldptr(ldptr) == FAILURE)
		return (SUCCESS);
	if (TYPE(ldptr) == ARTYPE &&
		FSEEK(ldptr, -((long)ARCHSZ), BEGINNING) == OKFSEEK &&
		FREAD((char *)&arhdr, ARCHSZ, 1, ldptr) == 1)
	{
		/*
		* Be sure OFFSET is even
		*/
		OFFSET(ldptr) += arhdr.ar_size + ARCHSZ + (arhdr.ar_size & 01);
		if (FSEEK(ldptr, 0L, BEGINNING) == OKFSEEK &&
			FREAD((char *)&(HEADER(ldptr)), FILHSZ, 1, ldptr) == 1)
		{
			return (FAILURE);
		}
	}
#endif
#endif
	fclose(IOPTR(ldptr));
	freeldptr(ldptr);
	return (SUCCESS);
}

static char ID[] = "@(#) ldclose.c: 1.3 2/16/83";
