/*
From pegasus.ATT.COM!hansen Tue Oct  9 00:34 EDT 1990
Received: by pyxis; Tue Oct  9 00:34 EDT 1990
FROM:       hansen@pegasus.ATT.COM (t.l.hansen)
TO:         research!andrew
DATE:        9 Oct 1990   0:27 EDT
SUBJECT:    bug in re_reexec()!

Compile and link the following program. The expected output is:

parens = 2
matched
0: 0x80881430 - 0x80881445
"!nosuchsystem!testing"
1: 0x80886334 - 0xc0020c1c
"nosuchsystem"
2: 0xc0020a78 - 0x80883924
"testing"

Instead, I get this. (Note the strings after 1: and 2:.)

parens = 2
matched
0: 0x80881430 - 0x80881445
"!nosuchsystem!testing"
1: 0x80886334 - 0xc0020c1c
""
2: 0xc0020a78 - 0x80883924
""

Can you please look into this soon? If not, let me know so that I can hunt
for it. I probably won't be able to find it as quickly as you, though. This
showed up within mail and I have an MR haunting me. Thanks.

					Tony

----------------------------------------------------------------
*/
#include <stdio.h>
#include <ctype.h>
#include "re.h"
#include "lre.h"

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

    for (i = 0; i < 256; i++)
	re_map[i] = i;
    regex = re_recomp(pat, pat+strlen(pat), re_map);
    parens = re_paren(regex);
    (void) printf("parens = %d\n", parens);
    if (parens != 2)
	return 0;
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
