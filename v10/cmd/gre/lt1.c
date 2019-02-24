/*
From pegasus.ATT.COM!hansen Sun Oct 14 23:38 EDT 1990
Received: by pyxis; Sun Oct 14 23:38 EDT 1990
FROM:       hansen@pegasus.ATT.COM (t.l.hansen)
TO:         research!andrew
DATE:       14 Oct 1990  23:35 EDT
SUBJECT:    regex bug take 2

Something's getting lost when writing or reading the compiled expression
to/from a file. The following program displays the problem. It is similar to
the previous one except that it writes the regex to a temp file in the
middle and then reads it back in.

					Tony
*/
#include <stdio.h>
#include <ctype.h>
#define USE_STDIO
#include "re.h"

void prc(c)
unsigned char c;
{
    if (c >= 0200)
	{
	(void) printf("M-");
	c -= 0200;
	}
    if (isprint(c)) putchar(c);
    else
	{
	putchar('^');
	putchar(c ^ 0100);
	}
}

void pr(i, mb, me)
int i;
char *mb;
char *me;
{
    (void) printf("%d: %#x - %#x\n", i, mb, me);
    putchar('"');
    for (; mb < me; mb++)
	{
	if (!*mb)
	    break;
	prc(*mb);
	}
    putchar('"');
    putchar('\n');
}

main()
{
    re_re *regex;
    int i;
    unsigned char re_map[256];
    static char lname[] = "!nosuchsystem!testing";
    static char pat[] = "!([^!]+)!(.+)";
    char *match[10][2];
    int parens;
    FILE *fp;

    for (i = 0; i < 256; i++)
	re_map[i] = i;
    regex = re_recomp(pat, pat+strlen(pat), re_map);
    parens = re_paren(regex);
    (void) printf("parens = %d\n", parens);
    if (parens != 2)
        {
	(void) printf("!= 2!!\n");
	return 0;
	}
    fp = fopen("/tmp/junk.re", "w");
    if (!fp)
        {
	(void) printf("error opening output file\n");
	return 0;
	}

    re_refile(regex, fp);
    fclose(fp);
    if (ferror(fp))
        {
	(void) printf("error writing\n");
	return 0;
	}

    fp = fopen("/tmp/junk.re", "r");
    if (!fp)
        {
	(void) printf("error opening input file\n");
	return 0;
	}

    regex = re_filere(fp);
    if (!regex)
        {
	(void) printf("error reading regex\n");
	return 0;
	}

    fclose(fp);

    parens = re_paren(regex);
    (void) printf("parens = %d\n", parens);
    if (parens != 2)
        {
	(void) printf("!= 2!!\n");
	return 0;
	}
    
    if (!re_reexec(regex, lname, lname+strlen(lname), match))
	{
	(void) printf("no match\n");
	return 0;
	}

    (void) printf("matched\n");
    for (i = 0; i <= parens; i++)
	pr(i, match[i][0], match[i][1]);
    return 0;
}
