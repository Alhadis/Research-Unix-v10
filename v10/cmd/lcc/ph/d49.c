/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


#include "flags.h"
#ifndef SKIP49
#define LIB_TEST 1
/*
 * 4.9 - Input / Output <stdio.h>
 */
#include <stdio.h>
#include <ctype.h>
#include "defs.h"
#if ANSI
#include <stdarg.h>
#include <stddef.h>
#define W_BIN "wb"
#define R_BIN "rb"
#define A_BIN "ab"
#define WU_BIN "w+b"
#define RU_BIN "r+b"
#define AU_BIN "a+b"
#define W_BIN_U "wb+"
#define R_BIN_U "rb+"
#define A_BIN_U "ab+"
static void d4_9_6_9(char *, char *, ...);
#define L_FirstName L_tmpnam
#else
#define L_FirstName 256
#endif
static char FirstName[L_FirstName] = {0};
char names[25+1][L_FirstName] = {0};
int next_name = 0;
char *save_name(p)
	char *p;
	{
	if (next_name < 25)
		str_cpye(names[next_name++], p);
	return (p);
	}
#if !ANSI && !V7
extern int errno;
#endif
#if ANSI8703 || V7
#include <errno.h>
#endif
/* if ANSI, but prior to 87/03, errno will be found in <stddef.h> */
#if !ANSI
#define remove unlink
#define rename(old, new) ((link(old, new) == 0 && unlink(old) == 0) ? 0 : -1)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define W_BIN "w"
#define R_BIN "r"
#define A_BIN "a"
#define WU_BIN "w+"
#define RU_BIN "r+"
#define AU_BIN "a+"
#define W_BIN_U "w+"
#define R_BIN_U "r+"
#define A_BIN_U "a+"
#endif
static int c;	/* for use in various SKIPNULLS */
static void d4_9_1();
static void d4_9_2();
static void d4_9_3();
static void d4_9_4();
static void d4_9_4_4();
static void d4_9_5();
static void d4_9_5_3();
static void d4_9_6();
static void d4_9_7();
static void d4_9_7_11();
static void d4_9_8();
static void d4_9_9();
static void d4_9_10();
void print();	/* in d49a.c */
void scan();	/* in d49a.c */

void d4_9()
	{
	Filename = "d49.c";
	d4_9_1();
	d4_9_2();
	d4_9_3();
	d4_9_4();
	d4_9_5();
	d4_9_6();
	d4_9_7();
 	d4_9_8();
	d4_9_9();
	d4_9_10();
	d4_9_4_4();	/* do after all tmpnam calls are completed */
	}
/*
 * 4.9.1 - Introduction
 */
static void d4_9_1()
	{
#ifndef SKIP491
#if ANSI

	int i, j, k;
	FILE *files[FOPEN_MAX-3];
	int open_success = 1;

	/* check for existence of required macros */
	i = _IOFBF;
	j = _IOLBF;
	k = _IONBF;
	checkthat(__LINE__, EOF < 0);

	/* size_t is defined here (and elsewhere) */
	iequals(__LINE__, sizeof(size_t), sizeof(sizeof(size_t)));

#ifndef FILENAME_MAX
	complain(__LINE__);
#endif
	/* make sure that FOPEN_MAX-3 files can be opened simultaneously ...
	 * the -3 allows for stdin, stdout, and stderr.
	 *
	 * IT HAS BEEN DETERMINED THAT THE STANDARD DOES NOT MANDATE THAT
	 * CALLING  tmpfile  FOR  FOPEN_MAX-3  TIMES MUST SUCCEED.
	 * AN IMPLEMENTATION WHICH FAILS THIS FORM OF THE TEST MAY SUPPLY
	 * AN ALTERNATIVE PROGRAM TO DEMONSTRATE THAT  FOPEN_MAX-3  FILES
	 * CAN BE OPENED SUCCESSFULLY.
	 */
	open_success = 1;
	for (i = 0; i < FOPEN_MAX-3; ++i)
		{
		files[i] = tmpfile();
		if (files[i] == NULL)
			{
			open_success = 0;
			break;
			}
		}
	checkthat(__LINE__, open_success == 1);  /* success at fopen'ing FOPEN_MAX files */


	/* now clean it up */
	while (--i >= 0)
		fclose(files[i]);
#endif
#endif /* SKIP491 */
	}

/*
 * 4.9.2 - Streams
 */
static void d4_9_2()
	{
#ifndef SKIP492
	int i;
	char to[254], from[254];
	FILE *fd;

	save_name(tmpnam(FirstName));

	/* in text mode, we are guaranteed to get back chars in the
	 * same order as written if they are printable, HT, NL, VT, or FF
	 */
	fd = fopen(FirstName, "w");
	for (i = 0; i < 256; ++i)
		if (isprint(i) || i == '\t' || i == '\v' || i == '\n' || i == '\f')
			fputc(i, fd);
	fputc('\n', fd); 
	fclose(fd);
	fd = fopen(FirstName, "r");
	for (i = 0; i < 256; ++i)
		if (isprint(i) || i == '\t' || i == '\v' || i == '\n' || i == '\f')
			if (!iequals(__LINE__, fgetc(fd), i))
				break;
	iequals(__LINE__, fgetc(fd), '\n');
	iequals(__LINE__, fgetc(fd), EOF);
	fclose(fd);
	remove(FirstName);

	/* in binary mode, the characters must all match */
	fd = fopen(FirstName, W_BIN);
	for (i = 0; i < 256; ++i)
		fputc(i, fd);
	fclose(fd);
	fd = fopen(FirstName, R_BIN);
	for (i = 0; i < 256; ++i)
		if (!iequals(__LINE__, fgetc(fd), i))
			break;
#define SKIPNULLS(fd, i) \
	if ((i = fgetc(fd)) == 0) \
		{ complain(- __LINE__); while ((i = fgetc(fd)) == 0) ; }
	SKIPNULLS(fd, i);
			
	iequals(__LINE__, i, EOF);
		
	fclose(fd);
	remove(FirstName);

	/* make sure that a text mode file can support a line == 254 chars */
	fd = fopen(FirstName, "w");
	for (i = 0; i < 253; ++i)
		to[i] = "0123456789"[i % 10];
	to[253] = '\n';
	fwrite(to, 254, 1, fd);
	fwrite(" \n", 2, 1, fd);	/* ANSI8612 write whitespace before newline */
	fclose(fd);
	fd = fopen(FirstName, "r");
	fread(from, 254, 1, fd);
	for (i = 0; i < 253; ++i)
		if (!iequals(__LINE__, from[i], "0123456789"[i % 10]))
			break;
	iequals(__LINE__, from[253], '\n');
	fread(from, 2, 1, fd);		/* ANSI8612 read the whitespace */
	from[2] = '\0';
	if (str_cmp(from, " \n") != 0)	/* 4.9.2 P2s1 */
		{
		complain(- __LINE__);		/* whether the blank reads back is implem-def */
		iequals(__LINE__, from[0], '\n');	/* but if it does not, the line must be just newline */
		}
	fclose(fd);
	remove(FirstName);

	checkthat(__LINE__, BUFSIZ >= 256);
#endif /* SKIP492 */
	}

/*
 * 4.9.3 - Files
 */
static void d4_9_3()
	{
#ifndef SKIP493
	char *name;
	FILE *fd;

	/* just check existence */
	FILE *f1 = stdin;
	FILE *f2 = stdout;
	FILE *f3 = stderr;

#if ANSI
#if ANSI8712
	checkthat(__LINE__, FOPEN_MAX >= 8);
#else
	checkthat(__LINE__, OPEN_MAX >= 8);
#endif

	/* whether a file of 0 length actually exists is implementation defined */
	name = save_name(tmpnam(NULL));
	fd = fopen(name, "w");
	fclose(fd);
	fd = fopen(name, "r");
	checkthat(- __LINE__, fd != NULL);
	fclose(fd);
	remove(name);
#endif
#endif /* SKIP493 */
	}

/*
 * 4.9.4 Operations on files
  */
static void d4_9_4()
	{
#ifndef SKIP494
	FILE *f;
	char *old, *new, *tmp;
#if ANSI
	char name[L_tmpnam], buf1[L_tmpnam], buf2[L_tmpnam], buf3[L_tmpnam];
#else
	static char name[] = "dXXXXXX";
	extern char *mktemp();
#endif

	/* 4.9.4.1 - remove */
	/* remove of non-existent file returns non-zero */
	inotequals(__LINE__, remove(save_name(tmpnam(NULL))), 0);

	/* successful remove returns 0 */
	f = fopen(new=save_name(tmpnam(NULL)), "w");
	fputc('a', f); fputc('\n', f);
	fclose(f);
	iequals(__LINE__, remove(new), 0);

	/* 4.9.4.2 The rename function */
#if ANSI
	old = save_name(tmpnam(buf1));
	new = save_name(tmpnam(buf2));
	tmp = save_name(tmpnam(buf3));
#else
	old = mktemp("aXXXXXX");
	new = mktemp("bXXXXXX");
	tmp = mktemp("cXXXXXX");
#endif
	/* make a file, write to it, rename it, check it, clean up */
	checkthat(__LINE__, (f = fopen(old, "w")) != NULL);
	fputc('x', f); fputc('\n', f);
	fclose(f);
	iequals(__LINE__, rename(old, new), 0);
	checkthat(__LINE__, (f = fopen(new, "r")) != NULL);
	iequals(__LINE__, fgetc(f), 'x');
	iequals(__LINE__, fgetc(f), '\n');
	fclose(f);

	/* make sure that the old file is effectively removed */
	checkthat(__LINE__, (f = fopen(old, "r")) == NULL);

	/* attempt to rename non-existent file should fail and return non 0 */
	inotequals(__LINE__, rename(tmp, old), 0);
	
	/* if the new file exists, the behavior is implementation defined */
	checkthat(__LINE__, (f = fopen(tmp, "w")) != NULL);
	fputc('x', f); fputc('\n', f);
	fclose(f);
	inotequals(- __LINE__, rename(new, tmp), 0);

	/* clean up */
	remove(new);
	remove(tmp);
#if ANSI
	/* 4.9.4.3 The tmpfile function */
	/* temporary file creation */
	f = tmpfile();
	fputc('y', f);
	fputc('\n', f);
	iequals(__LINE__, ferror(f), 0);
	fseek(f, 0L, SEEK_SET);
	iequals(__LINE__, fgetc(f), 'y');
	fclose(f);
#endif
	/* 4.9.4.4 The tmpnam function */
#if ANSI
	checkthat(__LINE__, TMP_MAX >= 25);
	old = save_name(tmpnam(buf1));
	aequals(__LINE__, old, buf1);
#else
	old = mktemp(name);
	aequals(__LINE__, old, name);
#endif
	/* old and new above should have created unique names */
	checkthat(__LINE__, str_cmp(old, new) != 0);
#endif /* SKIP494 */
	}

/*
 * 4.9.5 - File access functions
 */
char details[BUFSIZ] = {0};
static void d4_9_5()
	{
#ifndef SKIP495
	FILE *f, *g;
#if ANSI
	char buf1[L_tmpnam];
	char buf2[L_tmpnam];
	char *afile = save_name(tmpnam(buf1));
	char *bfile = save_name(tmpnam(buf2));
#else
	char *afile = mktemp("aXXXXXX");
	char *bfile = mktemp("bXXXXXX");
#endif

	/* 4.9.5.1 The fclose function */
	f = fopen(afile, "w");
	fputc('z', f);
	fputc('\n', f);
	iequals(__LINE__, fclose(f), 0);

	/* 4.9.5.2 The fflush function */
	/* fflush undoes the effects of an ungetc ... tested in 4.9.7.11 */
#if ANSI8804
	iequals(__LINE__, fflush(NULL), 0);	/* flush all output streams */
#endif /* ANSI8804 */
	/* make sure the previous write worked OK */
	f = fopen(afile, "r");
	iequals(__LINE__, fgetc(f), 'z');
	iequals(__LINE__, fgetc(f), '\n');
	fgetc(f);
	inotequals(__LINE__, feof(f), 0); 
	fclose(f);
	iequals(__LINE__, remove(afile), 0);

	/* 4.9.5.3 The fopen function */
	d4_9_5_3();

	/* 4.9.5.4 The reopen function */
	f = fopen(afile, "wb");
	fputc('a', f);
	fclose(f);
	f = fopen(bfile, "w+b");
	fputc('b', f);
	rewind(f);
	checkthat(__LINE__, (f = freopen(afile, "rb", f)) != NULL);

	/* should now be reading from afile, not bfile */
	iequals(__LINE__, fgetc(f), 'a');
	fclose(f);

	/* do reopens to change modes */
	checkthat(__LINE__, (f = fopen(afile, "rb")) != NULL);
	iequals(__LINE__, fgetc(f), 'a');
	checkthat(__LINE__, (f = freopen(afile, "wb", f)) != NULL);
	iequals(__LINE__, fputc('c', f), 'c');
	checkthat(__LINE__, (f = freopen(afile, "rb", f)) != NULL);
	iequals(__LINE__, fgetc(f), 'c');
	checkthat(__LINE__, (f = freopen(afile, "ab", f)) != NULL);
	iequals(__LINE__, fputc('d', f), 'd');
	if (checkthat(__LINE__, (f = freopen(afile, "r+b", f)) != NULL))
		{
		iequals(__LINE__, fgetc(f), 'c');
		SKIPNULLS(f, c); iequals(__LINE__, c, 'd');
		rewind(f);
		iequals(__LINE__, fputc('e', f), 'e');
		iequals(__LINE__, fputc('f', f), 'f');
		checkthat(__LINE__, (f = freopen(afile, "rb", f)) != NULL);
		iequals(__LINE__, fgetc(f), 'e');
		iequals(__LINE__, fgetc(f), 'f');
		SKIPNULLS(f, c);
		if (c == 'd')
			SKIPNULLS(f, c);
		iequals(__LINE__, c, EOF);
		checkthat(__LINE__, (f = freopen(afile, "w+b", f)) != NULL);
		iequals(__LINE__, fputc('g', f), 'g');
		rewind(f);
		iequals(__LINE__, fgetc(f), 'g');
		SKIPNULLS(f, c);
		if (c == 'f')
			SKIPNULLS(f, c);
		iequals(__LINE__, c, EOF);
		}
	fclose(f);
	/* get NULL pointer on failure ? */
	remove(bfile);
	f = fopen(afile, "rb");
 	if (freopen(bfile, "rb", f) != NULL)
		{
		complain(__LINE__);	/* freopen should have failed: no bfile */
		fclose(f);
		}
	/* 4.9.5.5 The setbuf function */
	/* we have already checked for existence ... about all we can do is 
	 * call it and make sure that it doesn't crash the program.
	 */
	f = fopen(afile, "rb");
	setbuf(f, NULL);
	setbuf(f, details);
	/* 4.9.5.6 The setvbuf function */
#if ANSI
	/* not much that can be done except check for 0/nonzero return */
	/* ANSI8809 - re-affirms that setvbuf is allowed to be "unable to perform" */
	iequals( - __LINE__, setvbuf(f, details, _IOFBF, BUFSIZ/2), 0);
	iequals( - __LINE__, setvbuf(f, details, _IOLBF, BUFSIZ/2), 0);
	iequals( - __LINE__, setvbuf(f, details, _IONBF, BUFSIZ/2), 0);
	iequals( - __LINE__, setvbuf(f, NULL, _IOFBF, BUFSIZ/2), 0);
	iequals( - __LINE__, setvbuf(f, NULL, _IOLBF, BUFSIZ/2), 0);
	iequals( - __LINE__, setvbuf(f, NULL, _IONBF, BUFSIZ/2), 0);

	/* force an illegal size and mode */
	checkthat( - __LINE__, setvbuf(f, NULL, _IOFBF, -1) != 0);
	checkthat(__LINE__, setvbuf(f, NULL, _IOFBF+_IOLBF+_IONBF, 0) != 0);
#endif

	fclose(f);
	remove(afile);
#endif /* SKIP495 */
	}

/* 
 * 4.9.5.3 - test all of the file open modes.
 */
static void d4_9_5_3()
	{
#ifndef SKIP4953  /* note: if SKIP495 is defined, 4953 isn't called */
	FILE *f;
#if ANSI
	fpos_t pos;
	char *afile = save_name(tmpnam(NULL));
#else
	char *afile = mktemp("aXXXXXX");
#endif
	int c;

	/* open with "r" and non-existent file fails */
	aequals(__LINE__, fopen(afile, "r"), NULL);
	
	/* TEXT MODE
	 * open(write), write a char, close.
	 * open(read), read a char, check value, close.
	 * open(append), seek, 0, write a char, 
	 * open(read), check that write was to end of file
	 */
	checkthat(__LINE__, (f = fopen(afile, "w")) != NULL);

	/* make sure that ferror and feof are 0 at this point */
	iequals(__LINE__, ferror(f), 0);
	iequals(__LINE__, feof(f), 0);

	fputc('d', f); fputc('\n', f);
	fclose(f);

	checkthat(__LINE__, (f = fopen(afile, "r")) != NULL);
	iequals(__LINE__, fgetc(f), 'd');
	fclose(f);

	checkthat(__LINE__, (f = fopen(afile, "a")) != NULL);
	fseek(f, 0L, SEEK_SET);
	fputc('e', f); fputc('\n', f);
	fclose(f);
	checkthat(__LINE__, (f = fopen(afile, "r")) != NULL);
	iequals(__LINE__, fgetc(f), 'd');
	iequals(__LINE__, fgetc(f), '\n');
	iequals(__LINE__, fgetc(f), 'e');
	iequals(__LINE__, fgetc(f), '\n');
	fclose(f);
	remove(afile);

	/* BINARY MODE
	 * same sequence, but with binary mode opens
	 */
	checkthat(__LINE__, (f = fopen(afile, W_BIN)) != NULL);
	fputc(1, f);
	fclose(f);

	checkthat(__LINE__, (f = fopen(afile, R_BIN)) != NULL);
	iequals(__LINE__, fgetc(f), 1);
	fclose(f);

	checkthat(__LINE__, (f = fopen(afile, A_BIN)) != NULL);
	fseek(f, 0L, SEEK_SET);
	fputc(2, f);
	fclose(f);
	{
	char c;

	checkthat(__LINE__, (f = fopen(afile, R_BIN)) != NULL);
	iequals(__LINE__, fgetc(f), 1);
	SKIPNULLS(f, c);
	iequals(__LINE__, c, 2);
	fclose(f);
	remove(afile);
	}

	/* UPDATE TEXT
	 * open(write update), write, seek 0, read, check, close.
	 * open(read update), read, check, seek 0, write, seek 0, check, close.
	 * open(append update), seek 0, write, seek 0, read, check, close
	 * open(read), read, check.
	 */
	checkthat(__LINE__, (f = fopen(afile, "w+")) != NULL);
	fputc('f', f); fputc('\n', f);
	fseek(f, 0L, SEEK_SET);
	iequals(__LINE__, fgetc(f), 'f');
	fclose(f);
	checkthat(__LINE__, (f = fopen(afile, "r+")) != NULL);
	iequals(__LINE__, fgetc(f), 'f');
	fseek(f, 0L, SEEK_SET);
	fputc('g', f);
	fseek(f, 0L, SEEK_SET);
	iequals(__LINE__, fgetc(f), 'g');
	fclose(f);
	checkthat(__LINE__, (f = fopen(afile, "a+")) != NULL);
	fseek(f, 0L, SEEK_SET);
	fputc('h', f);
	fputc('\n', f);
	fseek(f, 0L, SEEK_SET);
	iequals(__LINE__, fgetc(f), 'g');
	iequals(__LINE__, fgetc(f), '\n');
	SKIPNULLS(f, c); iequals(__LINE__, c, 'h'); /* open might be binary */
	iequals(__LINE__, fgetc(f), '\n');
	fclose(f);
	remove(afile);

	/* UPDATE BINARY (X+B form)
	 * open(write update), write, seek 0, read, check, close.
	 * open(read update), read, check, seek 0, write, seek 0, check, close.
	 * open(append update), seek 0, write, seek -2, read, check, close
	 * open(read), read, check.
	 */
	checkthat(__LINE__, (f = fopen(afile, WU_BIN)) != NULL);
	fputc('f', f);
	fseek(f, 0L, SEEK_SET);
	/* we wrote 'f' ... go back to zero and check that it is there */
	iequals(__LINE__, fgetc(f), 'f');
	fclose(f);
	checkthat(__LINE__, (f = fopen(afile, RU_BIN)) != NULL);
	/* close and reopen (for read-write) ... check that it is 'f' */
	iequals(__LINE__, fgetc(f), 'f');
	fseek(f, 0L, SEEK_SET);
	fputc('g', f);
	fseek(f, 0L, SEEK_SET);

	/* we wrote over it with a 'g' ... check that out */
	iequals(__LINE__, fgetc(f), 'g');
	fclose(f);
	checkthat(__LINE__, (f = fopen(afile, AU_BIN)) != NULL);
	fseek(f, 0L, SEEK_SET);
	fputc('h', f);
	fseek(f, -2L, SEEK_CUR);
	iequals( - __LINE__, fgetc(f), 'g');
	iequals(__LINE__, fgetc(f), 'h');
	fclose(f);
	checkthat(__LINE__, (f = fopen(afile, R_BIN)) != NULL);
	/* by now the file should have a 'g' and an 'h' in it */
	iequals(__LINE__, fgetc(f), 'g');
	SKIPNULLS(f, c); iequals(__LINE__, c, 'h');
	fclose(f);

	/* In update mode, there must be a fseek,rewind,fpos, or EOF between
	 * input and output, or an fflush between output and input.
	 * Diagrams use _ for file position, N for possible nul-padding, 
	 * and  n  for new-line 
	 */
	checkthat(__LINE__, (f = fopen(afile, W_BIN)) != NULL);
	fputc('g', f);								/*  g_  */
	fputc('\n', f);								/*  g n_ */
	fclose(f);									/*  g n N */

	checkthat(__LINE__, (f = fopen(afile, R_BIN_U)) != NULL);
	iequals(__LINE__, fgetc(f), 'g');			/*  g_n N */
	fseek(f, 0L, SEEK_SET);						/* _g n N */
	iequals(__LINE__, fgetc(f), 'g');			/*  g_n N */
	iequals(__LINE__, fgetc(f), '\n');			/*  g n_N */
	SKIPNULLS(f, c); iequals(__LINE__, c, EOF);	/*  g n N_ */
	fseek(f, 0L, SEEK_SET);						/* _g n N */
	fputc('i', f);								/*  i_n N */

	/* now make sure that these work */
	fseek(f, 0L, SEEK_SET);						/* _i n N */
	iequals(__LINE__, fgetc(f), 'i');			/*  i_n N */
	fseek(f, 0L, SEEK_SET);						/* _i n N */
	fputc('j', f);								/*  j_n N */
	fseek(f, 0L, SEEK_SET);						/* _j n N */
	iequals(__LINE__, fgetc(f), 'j');			/*  j_n N */
	iequals(__LINE__, fgetc(f), '\n');			/*  j n_N */
	SKIPNULLS(f, c); iequals(__LINE__, c, EOF);	/*  j n N_ */
	fseek(f, 0L, SEEK_CUR);						/*  j n N_*/
	fputc('z', f);								/*  j n N z_ */
	fseek(f, 0L, SEEK_SET);						/* _j n N z N */
	iequals(__LINE__, fgetc(f), 'j');			/*  j_n N z N */
	rewind(f);									/* _j n N z N */
	fputc('k', f);								/*  k_n N z N */
	rewind(f);									/* _k n N z N */
	iequals(__LINE__, fgetc(f), 'k');			/*  k_n N z N */
	iequals(__LINE__, fgetc(f), '\n');			/*  k n_N z N */
	SKIPNULLS(f, c); iequals(__LINE__, c, 'z');	/*  k n N z_N */
	SKIPNULLS(f, c); iequals(__LINE__, c, EOF);	/*  k n N z N_ */
#if ANSI
	fseek(f, 0L, SEEK_SET);						/* _k n N z N */
	iequals(__LINE__, fgetc(f), 'k');			/*  k_n N z N */
	fseek(f, 0L, SEEK_CUR);	/* allow writing */	/*  k_n N z N */
	fputc('l', f);								/*  k l_N z N */
	fputc('\n', f);								/*  k l n_N z N */
	rewind(f);									/* _k l n N z N */
	iequals(__LINE__, fgetc(f), 'k');			/*  k_l n N z N */
	iequals(__LINE__, fgetc(f), 'l');			/*  k l_n N z N */
	iequals(__LINE__, fgetc(f), '\n');			/*  k l n_N z N */

	fseek(f, 0L, SEEK_SET);						/* _k l n N z N */
	fgetpos(f, &pos);							/* _k l n N z N */
	iequals(__LINE__, fgetc(f), 'k');			/*  k_l n N z N */
	fsetpos(f, &pos);							/* _k l n N z N */
	fputc('m', f);								/*  m_l n N z N */
	fsetpos(f, &pos);							/* _m l n N z N */
	iequals(__LINE__, fgetc(f), 'm');			/*  m_l n N z N */
	iequals(__LINE__, fgetc(f), 'l');			/*  m l_n N z N */
	iequals(__LINE__, fgetc(f), '\n');			/*  m l n_N z N */

#endif
	fclose(f);									/*  m l n N z N */
	remove(afile);								/*  removed */

	/* UPDATE BINARY (using  XB+ form)
	 * open(write update), write, seek 0, read, check, close.
	 * open(read update), read, check, seek 0, write, seek -1, check, close.
	 * open(append update), seek 0, write, seek -1, read, check, close
	 * open(read), read, check.
	 */
	checkthat(__LINE__, (f = fopen(afile, W_BIN_U)) != NULL);
	fputc('f', f);								/*  f_ */
	fseek(f, 0L, SEEK_SET);						/* _f */
	/* we wrote 'f' ... go back to zero and check that it is there */
	iequals(__LINE__, fgetc(f), 'f');			/*  f_ */
	fclose(f);									/*  f N */
	checkthat(__LINE__, (f = fopen(afile, R_BIN_U)) != NULL);
	/* close and reopen (for read-write) */		/* _f N */
	iequals(__LINE__, fgetc(f), 'f');			/*  f_N */
	fseek(f, 0L, SEEK_SET);						/* _f N */
	fputc('g', f);								/*  g_N */
	fseek(f, -1L, SEEK_CUR);					/* _g N */
	iequals(__LINE__, fgetc(f), 'g');			/*  g_N */
	/* we wrote over it with a 'g' */
	fclose(f);									/*  g N */
	checkthat(__LINE__, (f = fopen(afile, A_BIN_U)) != NULL);
	fseek(f, 0L, SEEK_SET);						/* _g N */
	fputc('h', f);	/* appended */				/* _g N h */
	fseek(f, 1L, SEEK_SET);						/*  g_N h */
	SKIPNULLS(f, c);							/*  g N h_ */
	iequals(__LINE__, c, 'h');					/*  g N h_ */
	fclose(f);									/*  g N h N */
	checkthat(__LINE__, (f = fopen(afile, R_BIN)) != NULL);
	/* should have a 'g' and an 'h' */			/* _g N h N */
	iequals(__LINE__, fgetc(f), 'g');			/*  g_N h N */
	SKIPNULLS(f, c);							/*  g N h_N */
	iequals(__LINE__, c, 'h');					/*  g N h_N */
	fclose(f);									/*  g N h N */

	remove(afile);								/*  removed */
#endif /* SKIP4953 */
	}

/*
 * 4.9.6 Formatted input/output functions
 */
static void d4_9_6()
	{
#ifndef SKIP496
	int i, j;
	char buff[20];
	char *p;

	/* 4.9.6.1 The fprintf function */
	/* 4.9.6.2 The fscanf function */
	/* 4.9.6.3 The printf function */
	/* 4.9.6.4 The scanf function */
	/* 4.9.6.5 The sprintf function */
	/* make sure that excess args are evaluated */
	sprintf(buff, "%d", 3, i = 17);
	stequals(__LINE__, buff, "3");
	iequals(__LINE__, i, 17);

	/* returns number of characters transmitted */
	iequals(__LINE__, sprintf(buff, "%d", 17), 2);
	iequals(__LINE__, sprintf(buff, "%d %d", 17, 123), 6);
	iequals(__LINE__, sprintf(buff, "%s", ""), 0);
	iequals(__LINE__, sprintf(buff, "%%"), 1);

	/* now check all of the formatting primitives */
	print();
	Filename = "d49.c";

	/* 4.9.6.6 The sscanf function */
	scan();
	Filename = "d49.c";
	/* return number of items converted */
	iequals(__LINE__, sscanf("def", "abc", &p), 0); 
	iequals(__LINE__, sscanf("7", "%d", &i, &j), 1); 
	iequals(__LINE__, sscanf("7 8", "%d", &i, &j), 1); 
	iequals(__LINE__, sscanf("7 8", "%d %d", &i, &j), 2); 

	/* 4.9.6.7 The vfprintf function */
	/* 4.9.6.8 The vprintf function */
	/* 4.9.6.9 The vsprintf function */
#if ANSI
	d4_9_6_9(buff, "%s %d %s", "string1", 23, "string2");
	stequals(__LINE__, buff, "string1 23 string2");
#endif
#endif /* SKIP496 */
	}

/*
 * 4.9.6.9 - check out the varargs form of formatting. */
#if ANSI
#if NEW_STYLE_FN_DEF
static void d4_9_6_9(char *buff, char *fmt, ...)
#else
static void d4_9_6_9(buff, fmt)
	char *buff;
	char *fmt;
#endif
	{
	va_list args;

	va_start(args, fmt);
	vsprintf(buff, fmt, args);
	va_end(args);
	}
#endif

/*
 * 4.9.7 Character input/output
 */
static void d4_9_7()
	{
#ifndef SKIP497
	FILE *f;
	char buff[65];
	int c, i;
#if ANSI
	char *afile = save_name(tmpnam(NULL));
#else
	char *afile = mktemp("aXXXXXX");
#endif

	/* 4.9.7.1 The fgetc function */
	/* 4.9.7.2 The fgets function */
	/* 4.9.7.3 The fputc function */
	/* 4.9.7.4 The fputs function */

	/* fgetc and fputc */
	f = fopen(afile, W_BIN);
	iequals(__LINE__, fputc(255, f), 255);
	iequals(__LINE__, fputc(0, f), 0);
	fclose(f);
	f = fopen(afile, R_BIN);
	/* make sure that there is no sign extension */
	iequals(__LINE__, fgetc(f), 255);
	iequals(__LINE__, fgetc(f), 0);
	SKIPNULLS(f, c);
	iequals(__LINE__, c, EOF);
	fclose(f);
	remove(afile);

	/* fputs and fgets */
	f = fopen(afile, "w");
	checkthat(__LINE__, fputs("string1\nstring2\nstring3\n", f) >= 0);	/* ANSI8709: weakened requirement */
	fclose(f);
	f = fopen(afile, "r");
	aequals(__LINE__, fgets(buff, 40, f), buff);
	iequals(__LINE__, str_cmp(buff, "string1\n"), 0);
	fgets(buff, 3, f);
	iequals(__LINE__, str_cmp(buff, "st"), 0);
	fgets(buff, 40, f);
	iequals(__LINE__, str_cmp(buff, "ring2\n"), 0);
	fgets(buff, 40, f);
	iequals(__LINE__, str_cmp(buff, "string3\n"), 0);
	aequals(__LINE__, fgets(buff, 40, f), NULL);
	/* buffer remains unchanged upon end of file */
	iequals(__LINE__, str_cmp(buff, "string3\n"), 0);
	fclose(f);

	/* error returns NULL */
	f = fopen(afile, "w");
	aequals(__LINE__, fgets(buff, 40, f), NULL);
	/* error returns non zero */
	f = freopen(afile, "r", f);
/* 	inotequals( - __LINE__, fputs(buff, f), 0); */
	fclose(f);
	remove(afile);

	/* 4.9.7.5 The getc function */
	/* 4.9.7.6 The getchar function */
	/* 4.9.7.7 The gets function */
	/* 4.9.7.8 The putc function */
	/* 4.9.7.9 The putchar function */
	/* 4.9.7.10 The puts function */
	/* getc and putc */
	f = fopen(afile, W_BIN);
	iequals(__LINE__, putc(255, f), 255);
	iequals(__LINE__, putc(0, f), 0);
	fclose(f);
	f = fopen(afile, R_BIN);
	/* make sure that there is no sign extension */
	iequals(__LINE__, getc(f), 255);
	iequals(__LINE__, getc(f), 0);
	SKIPNULLS(f, c);
	iequals(__LINE__, c, EOF);
	fclose(f); remove(afile);
	/* getchar, putchar, puts, gets not tested; we don't want to lose stdin and stdout. */
	/* 4.9.7.11 The ungetc function */
	f = fopen(afile, "w");
	iequals(__LINE__, fputc('a', f), 'a');
	iequals(__LINE__, fputc('z', f), 'z');
	fclose(f);
	d4_9_7_11(afile, "r");
	f = fopen(afile, "w+");
	iequals(__LINE__, fputc('a', f), 'a');
	iequals(__LINE__, fputc('z', f), 'z');
	fclose(f);
	d4_9_7_11(afile, "r+");remove(afile);

	f = fopen(afile, W_BIN);
	iequals(__LINE__, fputc('a', f), 'a');
	iequals(__LINE__, fputc('z', f), 'z');
	fclose(f);
	d4_9_7_11(afile, R_BIN);
	d4_9_7_11(afile, R_BIN_U);
#if ANSI
	/* it should be possible to do an ungetc on an empty file */
	f = fopen(afile, W_BIN_U);
	iequals(__LINE__, ungetc('b', f), 'b');
	iequals(__LINE__, getc(f), 'b');
 	/* fscanf and ungetc should not get in each others way */
 	rewind(f);
 	fprintf(f, "123abc\n");
 	rewind(f);
 	fscanf(f, "%d", &i);
 	ungetc('x', f);
 	iequals(__LINE__, getc(f), 'x');
 	iequals(__LINE__, getc(f), 'a');
	fclose(f);
#endif
	remove(afile);
#endif /* SKIP497 */
	}

/*
 * 4.9.7.11 - the ungetc code will get executed for several
 * different file open modes.
 */
static void d4_9_7_11(afile, mode)
	char *afile;
	char *mode;
	{
#ifndef SKIP497_11 /* note: if SKIP497 is defined, 497_11 isn't called */
	long oldpos;
	int c;
	FILE *f;
#if ANSI
	fpos_t pos;
#endif

	f = fopen(afile, mode);
#if ANSI
	fgetpos(f, &pos);
#endif
	iequals(__LINE__, fgetc(f), 'a');
	iequals(__LINE__, ungetc('b', f), 'b');
	iequals(__LINE__, getc(f), 'b');
	iequals(__LINE__, ungetc('c', f), 'c');

	/* fseek, rewind, and fsetpos lose ungot character */
	fseek(f, 0L, SEEK_SET);
	iequals( - __LINE__, getc(f), 'a');
	iequals(__LINE__, getc(f), 'z');
	iequals(__LINE__, ungetc('b', f), 'b');
	rewind(f);
	iequals(__LINE__, getc(f), 'a');
	iequals(__LINE__, getc(f), 'z');
#if ANSI
	ungetc('b', f);
	fsetpos(f, &pos);
	iequals(__LINE__, getc(f), 'a');
	iequals(__LINE__, getc(f), 'z');
#endif

	/* the external file remains unchanged */
	iequals(__LINE__, ungetc('b', f), 'b');
	fclose(f);
	f = fopen(afile, mode);
	iequals(__LINE__, fgetc(f), 'a');
	iequals(__LINE__, fgetc(f), 'z');
	SKIPNULLS(f, c);
	/* record oriented systems might add this newline */
	if (c == '\n')
		c = fgetc(f);
	iequals(__LINE__, c, EOF);
	fclose(f);

	/* ungetc of EOF has no effect */
	f = fopen(afile, mode);
	iequals(__LINE__, fgetc(f), 'a');
	iequals(__LINE__, ungetc(EOF, f), EOF);
	iequals(__LINE__, fgetc(f), 'z');
	SKIPNULLS(f, c);
	/* record oriented systems might add this newline */
	if (c == '\n')
		c = fgetc(f);
	iequals(__LINE__, c, EOF);
#if ANSI
	/* a successful ungetc clears the EOF indicator */
	iequals(__LINE__, ungetc('b', f), 'b');
	iequals(__LINE__, feof(f), 0);
	iequals(__LINE__, fgetc(f), 'b');
#endif
	/* after ungetc then read, the file position is restored */
	oldpos = ftell(f);
	iequals(__LINE__, ungetc('b', f), 'b');
	iequals(__LINE__, fgetc(f), 'b');
	lequals(__LINE__, ftell(f), oldpos);

	/* if binary, the file position decrements by 1 */
	if (str_str(mode, "b") != NULL)
		{
		oldpos = ftell(f);
		iequals(__LINE__, ungetc('a', f), 'a');
		lequals(__LINE__, ftell(f), oldpos - 1L);
		}
#if ANSI
	/* ANSI requires ability to do an ungetc at the start of a file */
	rewind(f);
	iequals(__LINE__, ungetc('b', f), 'b');
	iequals(__LINE__, fgetc(f), 'b');
	iequals(__LINE__, fgetc(f), 'a');
#endif
	fclose(f);
#endif /* SKIP497_11 */
	}

/*
 * 4.9.8 - Direct input/output functions
 */
static long array1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
static long array2[11] = {0};
static void d4_9_8()
	{
#ifndef SKIP498
	FILE *f;
	int i;
#if ANSI
	char *afile = save_name(tmpnam(NULL));
#else
	char *afile = mktemp("aXXXXXX");
#endif
	int c;

	/* 4.9.8.1 The fread function */
	/* 4.9.8.2 The fwrite function */

	f = fopen(afile, W_BIN);
	iequals(__LINE__, fwrite(array1, sizeof(long), 10, f), 10);
	fclose(f);
	f = fopen(afile, R_BIN);
	iequals(__LINE__, fread(array2, sizeof(long), 6, f), 6);

	/* zero size or nelem leaves array unchanged */
 	iequals(__LINE__, fread(array2, 0, sizeof(long), f), 0);
 	iequals(__LINE__, fread(array2, 10, 0, f), 0);

	/* incomplete read */
	iequals(__LINE__, fread(array2+6, sizeof(long), 4, f), 4);
	for (i = 0; i < 10; ++i)
		lequals(__LINE__, array2[i], array1[i]);
	SKIPNULLS(f, c);

	/* at end of file */
	iequals(__LINE__, fread(array2, 10, sizeof(long), f), 0);
	fclose(f);
	remove(afile);
#endif /* SKIP498 */
	}

/*
 * 4.9.9 - Random-access functions
 */
static char buffer[] = "     this is a test string of no particular significance\n";
static char xchar[] = "abcdefghijklmnopqrstuvwxyz";
static void d4_9_9()
	{
#ifndef SKIP499
	FILE *f;
	int i;
	long array[100];
#if ANSI
	fpos_t pos[100];
	char *afile = save_name(tmpnam(NULL));
#else
	char *afile = mktemp("aXXXXXX");
#endif

	/* 4.9.9.1 The fgetpos function */
	/* 4.9.9.2 The fseek function */
	/* 4.9.9.3 The fsetpos function */
	/* 4.9.9.4 The ftell function */

	/* 
	 * create a file with 100 strings, mark the "ftell" position
	 * of each one, and read them in backwards.
	 */
	f = fopen(afile, "w+");
	for (i = 0; i < 100; ++i)
		{
		array[i] = ftell(f);
		buffer[0] = xchar[i%26];
		fwrite(buffer, sizeof(buffer), 1, f);
		}
	for (i = 99; i >= 0; --i)
		{
		fseek(f, array[i], SEEK_SET);
		fread(buffer, sizeof(buffer), 1, f);
		iequals(__LINE__, xchar[i%26], buffer[0]);
		}
	fclose(f); remove(afile);

	/* for binary files, ftell can compute offsets */
	f = fopen(afile, WU_BIN);
	fwrite(array, sizeof(long), 100, f);
	lequals(__LINE__, ftell(f), 100L*sizeof(long));

	fseek(f, 50L, SEEK_SET);
	lequals(__LINE__, ftell(f), 50L);
	fseek(f, 40L, SEEK_CUR);
	lequals(__LINE__, ftell(f), 90L);
	fseek(f, -40L, SEEK_END);
	lequals( - __LINE__, ftell(f), 100*sizeof(long)-40L);	/* "remark" only - could be nul-padded */

	/* fseek clears the end of file indicator */
	fseek(f, 0L, SEEK_END);
	iequals(__LINE__, fgetc(f), EOF);
	fseek(f, 0L, SEEK_END);
	iequals(__LINE__, feof(f), 0);

	/* fseek: bad requests return non-zero 
	 * ftell: bad requests return -1L and set errno
	 */
	inotequals(__LINE__, fseek(f, -1L, SEEK_SET), 0);
	fclose(f); remove(afile);
	errno = 0;

#if ANSI
	lequals(__LINE__, ftell(f), -1L);
	inotequals(__LINE__, errno, 0);
#endif

	f = fopen(afile, W_BIN_U);
#if ANSI
	/* 
	 * create a file with 100 strings, mark the "getpos" position
	 * of each one, and read them in backwards.
	 */
	for (i = 0; i < 100; ++i)
		{
		fgetpos(f, &pos[i]);
		buffer[0] = xchar[i%26];
		fwrite(buffer, sizeof(buffer), 1, f);
		}
	for (i = 99; i >= 0; --i)
		{
		fsetpos(f, &pos[i]);
		fread(buffer, sizeof(buffer), 1, f);
		iequals(__LINE__, xchar[i%26], buffer[0]);
		}

	/* successful return is 0, unsuccessful is non-zero + errno */
	iequals(__LINE__, fgetpos(f, &pos[0]), 0);
	iequals(__LINE__, fsetpos(f, &pos[0]), 0);
#endif
	fputs("a\n", f);	/* be sure that following tests work, even for non-ANSI */
	fclose(f);

	/* 4.9.9.5 The rewind function */
	/* rewind is the same as fseek(f, 0L, SEEK_SET) */
	/* in order for  ftell(f)  to be well-defined, */
	/* file  f  must be binary (and must be created as binary) */
	f = fopen(afile, R_BIN);
	if (f == NULL)
		complain(__LINE__);
	else
		{
		fseek(f, 0L, SEEK_END);
		iequals(__LINE__, fgetc(f), EOF);
		rewind(f);
		lequals(__LINE__, ftell(f), 0L);
		fclose(f);
		}
	remove(afile);
#endif /* SKIP499 */
	}

/*
 * 4.9.10 - Error-handling functions
 */
static void d4_9_10()
	{
#ifndef SKIP49_10
	FILE *f;
	char * (*fp)();
#if ANSI
	char *afile = save_name(tmpnam(NULL));
#else
	char *afile = mktemp("aXXXXXX");
	extern void perror();
#endif
	char c;

	/* 4.9.9.1 The clearerr function */
	/* 4.9.9.2 The feof function */

	f = fopen(afile, W_BIN);
	fputc('x', f);
	fclose(f);
	f = fopen(afile, R_BIN);
	iequals(__LINE__, feof(f), 0);
	fgetc(f);
	SKIPNULLS(f, c);
	inotequals(__LINE__, feof(f), 0);
	clearerr(f);
	iequals(__LINE__, feof(f), 0);

	/* 4.9.9.3 The ferror function */
	iequals(__LINE__, ferror(f), 0);

	/* There is unfortunately no portable way to force an "I/O error" for testing ferror */

#if ANSI
	/* 4.9.9.4 The perror function */
	/* just check for existence */
	errno = 0;
	perror(NULL);
#endif

	fclose(f);
	remove(afile);
#endif /* SKIP49_10 */
	}

/*
 * 4.9.4.4 - Now that tmpnam() is no longer needed, verify that
* we can get at least 25 unique names (the min-max for TMP_MAX).
 * next_name is how many have been used so far.
 */
static void d4_9_4_4()
	{
#ifndef SKIP4944
	int i, j;

#if ANSI
	for (i = 0; i < next_name; ++i)
		for (j = 0; j < i; ++j)
			if (str_cmp(names[i], names[j]) == 0)
				{
				complain(__LINE__);
				return;
				}
	for (i = next_name; i < 25; ++i)
		{
		if (checkthat(__LINE__, tmpnam(names[i]) != NULL) != 0)
			return;
		for (j = 0; j < i; ++j)
			if (str_cmp(names[i], names[j]) == 0)
				{
				complain(__LINE__);
				return;
				}
		}
#endif 
#endif /* SKIP4944 */
	}

#else /* if SKIP49 */
void d4_9() { pr_skip("d4_9: SKIPPED ENTIRELY\n"); }
#endif

