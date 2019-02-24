/*  "@(#) expunge.c: V 2.12 10/17/83"  */

#include <stdio.h>
#include <a.out.h>
#include <ldfcn.h>
#include <fcntl.h>

extern LDFILE	*ldopen();

extern long	lseek();

#ifdef	vax
#define MAIN	"_main"
#define MATHERR	"_matherr"
#else
#define MAIN	"main"
#define MATHERR	"matherr"
#endif

main(argc,argv)
int	argc;
char	**argv;
{
	LDFILE	*ldptr;
	SYMENT	symbol;
	AUXENT	aux;
	FILHDR	filehead;
	long	numsyms,
		symaddr,
		stringstart,
		symsleft = 0,
		i;
	int	pass,
		fd,
		fd_tmp,
		throwaway = 0,
		fixaux = 0,
		j;
	char	buf[BUFSIZ],
		ch,
		filenam[10];

	pass = atoi(*++argv);
	if (pass == 1)				/* before final link edit */
		strcpy(filenam,"basic.tmp");
	else
		strcpy(filenam,"basic");
	if ((ldptr = ldopen(filenam, NULL)) == NULL)
	{
		fprintf(stderr,"Cannot find %s:  FATAL ERROR\n",filenam);
		exit(2);
	}
	if (ldfhread(ldptr,&filehead) == FAILURE)
	{
		fprintf(stderr,"Cannot find file header: FATAL ERROR\n");
		exit(2);
	}
	numsyms = filehead.f_nsyms;
	symaddr = filehead.f_symptr;
	stringstart = symaddr + (numsyms * SYMESZ);
	if (pass == 1)
	{
		ldclose(ldptr);
		fd = open("basic.tmp", O_RDWR);
		if (lseek(fd, symaddr, 0) < 0)
		{
			fprintf(stderr,"Cannot seek to symbol table: FATAL ERROR\n");
			exit(2);
		}
		i = 0;
		while (i < numsyms)
		{
			if (read(fd,(char *)(&symbol),SYMESZ) != SYMESZ)
			{
				fprintf(stderr,"EOF on symbol table: FATAL ERROR\n");
				exit(2);
			}
			if (strcmp(MAIN, symbol.n_name) == 0 ||
					strncmp(MATHERR, symbol.n_name, 7) == 0)
				goto checkaux;
			if (symbol.n_sclass == C_EXT && (symbol.n_value != 0 ||
							symbol.n_scnum != 0))
			{
#ifdef	REL2
				if (symbol.n_zeroes == 0L)
				{				/* flexnames */
					long	temploc;

					temploc = lseek(fd, 0, 1);
					lseek(fd, stringstart +
							symbol.n_offset, 0);
					if (read(fd, (char *) &ch, 1) != 1)
					{
						fprintf(stderr,"Unexpected EOF\n");
						exit(2);
					}
					lseek(fd, -1, 1);
					ch = (unsigned char)(ch - 0100);
					if (write(fd, (char *) &ch, 1) < 0)
					{
						fprintf(stderr, "Write failed\n");
						exit(2);
					}
					lseek(fd, temploc, 0);
				}
				else
#endif
					symbol.n_name[0] -= 0100;

				if (lseek(fd, -SYMESZ, 1) < 0)
				{
					fprintf(stderr, "Symbol updating failed\n");
					exit(2);
				}
				if (write(fd, (char *) (&symbol), SYMESZ) < 0)
				{
					fprintf(stderr, "Write failed\n");
					exit(2);
				}
			}
	checkaux:
			if ((j = symbol.n_numaux) > 0)
			{
				while (j-- > 0)
				{
					if (lseek(fd, AUXESZ, 1) < 0)
					{
						fprintf(stderr,"EOF on auxiliary entry: FATAL ERROR\n");
						exit(-1);
					}
					++i;
				}
			}
			++i;
		}
		close(fd);
	}
	else
	{
		fd_tmp = open("basic.tmp", O_WRONLY | O_CREAT | O_TRUNC);
		fd = open("basic", O_RDONLY);
		while (symaddr - BUFSIZ >= BUFSIZ)
		{
			if (read(fd, (char *) buf, BUFSIZ) != BUFSIZ)
			{
				fprintf(stderr,"Unexpected EOF\n");
				exit(2);
			}
			if (write(fd_tmp, (char *) buf, BUFSIZ) < 0)
			{
				fprintf(stderr, "Write failed\n");
				exit(2);
			}
			symaddr -= BUFSIZ;
		}
		while (symaddr--)
		{
			if (read(fd, (char *) &ch, 1) != 1)
			{
				fprintf(stderr,"Unexpected EOF\n");
				exit(2);
			}
			if (write(fd_tmp, (char *) &ch, 1) < 0)
			{
				fprintf(stderr, "Write failed\n");
				exit(2);
			}
		}
		i = 0;
		while (i < numsyms)
		{
			if (read(fd, (char *) (&symbol), SYMESZ) != SYMESZ)
			{
				fprintf(stderr,"EOF on symbol table: FATAL ERROR\n");
				exit(2);
			}
#ifdef	REL2
			if (symbol.n_zeroes == 0L)
			{
				long	temploc;

				temploc = lseek(fd, 0, 1);
				lseek(fd, stringstart + symbol.n_offset, 0);
				if (read(fd, (char *) &ch, 1) != 1)
				{
					fprintf(stderr,"EOF on string table: FATAL ERROR\n");
					exit(2);
				}
				if ((unsigned char) ch < 0101)
					throwaway = 1;
				lseek(fd, temploc, 0);
			}
			else
#endif
			if ((unsigned char) (symbol.n_name[0]) < 0101)
				throwaway = 1;
			else
			{
				throwaway = 0;
				fixaux = symbol.n_numaux;
				symbol.n_numaux = 0;
			}
			if (!throwaway)
			{
				if (write(fd_tmp,(char *)(&symbol),SYMESZ) < 0)
				{
					fprintf(stderr, "Write failed\n");
					exit(2);
				}
				++symsleft;
			}
			if ((j = symbol.n_numaux) > 0 || fixaux)
			{
				if (fixaux)
					j = fixaux;
				while (j-- > 0)
				{
					if (read(fd, (char *) (&aux),
							AUXESZ) != AUXESZ)
					{
						fprintf(stderr,"EOF on symbol table: FATAL ERROR\n");
						exit(2);
					}
					++i;
				}
			}
			++i;
			fixaux = 0;
		}
		while (read(fd, (char *) &ch, 1) == 1)
			write(fd_tmp, (char *) &ch, 1);
		close(fd);
		close(fd_tmp);
		fd = open("basic.tmp", O_RDWR);
		if (read(fd, (char *) &filehead, sizeof(filehead)) !=
					sizeof(filehead))
		{
			fprintf(stderr, "Could not read file header\n");
			exit(2);
		}
		filehead.f_nsyms = symsleft;
		lseek(fd,0,0);
		if (write(fd,(char *)&filehead,sizeof(filehead)) < 0)
		{
			fprintf(stderr, "Write of file header failed\n");
			exit(2);
		}
		close(fd);
		if (system("mv basic.tmp basic") < 0)
		{
			fprintf(stderr,"Can't move basic.tmp to basic\n");
			exit(2);
		}
	}
}
