/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

	int i;
	static char *digits = "0123456789";
	static char *lower = "abcdefghijklmnopqrstuvwxyz";
	static char *upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static char *graph = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
	static char *space = " \f\t\n\r\v";
	static char *xdigit = "abcdefABCDEF";
	char *p;

	Filename = "d43h.h";


	for (i = 0; i < (ANSI ? 256 : 128); ++i)
		{
		/* 4.3.1.1 isalnum
		 */
		if (isalpha(i) || isdigit(i))
			checkthat(__LINE__, isalnum(i));
		else
			checkthat(__LINE__, !isalnum(i));

		/* 4.3.1.2 isalpha
		 */
		if (islower(i) || isupper(i))
			checkthat(__LINE__, isalpha(i));
		else
			checkthat(__LINE__, !isalpha(i));

		/* 4.3.1.3 iscntrl
		 * implementation defined, but not printable
		 */
		if (isprint(i))
			checkthat(__LINE__, !iscntrl(i));

		/* 4.3.1.4 isdigit
		 */
		if (in_set(i, digits))
			checkthat(__LINE__, isdigit(i));
		else
			checkthat(__LINE__, !isdigit(i));

		/* 4.3.1.5 isgraph
		 */
		if (isalnum(i) || in_set(i, graph))
			checkthat( - __LINE__, isgraph(i));
		else
			checkthat( - __LINE__, !isgraph(i));

		/* 4.3.1.6 islower
		 */
		if (in_set(i, lower))
			checkthat(__LINE__, islower(i));
		else
			checkthat(__LINE__, !islower(i));

		/* 4.3.1.7 isprint
		 */
		if (isgraph(i) || i == ' ')
			checkthat(__LINE__, isprint(i));
		else
			checkthat(__LINE__, !isprint(i));

		/* 4.3.1.8 ispunct
		 */
		if (isgraph(i) && !isalnum(i))
			checkthat(__LINE__, ispunct(i));
		else
			checkthat(__LINE__, !ispunct(i));

		/* 4.3.1.9 isspace
		 */
		if (in_set(i, space))
			checkthat(__LINE__, isspace(i));
		else
			checkthat(__LINE__, !isspace(i));

		/* 4.3.1.10 isupper
		 */
		if (in_set(i, upper))
			checkthat(__LINE__, isupper(i));
		else
			checkthat(__LINE__, !isupper(i));

		/* 4.3.1.11 isxdigit
		 */
		if (isdigit(i) || in_set(i, xdigit))
			checkthat(__LINE__, isxdigit(i));
		else
			checkthat(__LINE__, !isxdigit(i));


		/* 4.3.2.1 tolower
		 * does nothing for !isalpha
		 */
		/* 4.3.2.2 toupper
		 * does nothing for !isalpha
		 */
		if (islower(i))
			checkthat(__LINE__, toupper(i) != i);
		else if (isupper(i))
			checkthat(__LINE__, tolower(i) != i);
		else
			{
			iequals(__LINE__, toupper(i), i);
			iequals(__LINE__, tolower(i), i);
			}
		}

	/* check case mapping */
	for (i = 0; i < strlen(upper); ++i)
		{
		iequals(__LINE__, tolower(upper[i]), lower[i]);
		iequals(__LINE__, toupper(lower[i]), upper[i]);
		}

	/* EOF is a legal argument to these functions */
	if (isalnum(EOF))
		complain(__LINE__);
	if (isalpha(EOF))
		complain(__LINE__);
	if (iscntrl(EOF))
		complain(__LINE__);
	if (isdigit(EOF))
		complain(__LINE__);
	if (isgraph(EOF))
		complain(__LINE__);
	if (islower(EOF))
		complain(__LINE__);
	if (isprint(EOF))
		complain(__LINE__);
	if (ispunct(EOF))
		complain(__LINE__);
	if (isspace(EOF))
		complain(__LINE__);
	if (isupper(EOF))
		complain(__LINE__);
	if (isxdigit(EOF))
		complain(__LINE__);

	checkthat(__LINE__, toupper(EOF) == EOF);
	checkthat(__LINE__, tolower(EOF) == EOF);
