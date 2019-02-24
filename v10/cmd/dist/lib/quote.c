#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include "lib.h"

void
quote(char *qs, char *us)
{
	unsigned char c;

	while (c = *us++)
		if (!isgraph(c) || c == '\\') {
			*qs++ = '\\';
			switch (c) {
			case '\\':
				*qs++ = '\\';
				break;
			case '\a':
				*qs++ = 'a';
				break;
			case '\b':
				*qs++ = 'b';
				break;
			case '\f':
				*qs++ = 'f';
				break;
			case '\n':
				*qs++ = 'n';
				break;
			case '\r':
				*qs++ = 'r';
				break;
			case '\t':
				*qs++ = 't';
				break;
			case '\v':
				*qs++ = 'v';
				break;
			default:
				*qs++ = c / 64 % 8 + '0';
				*qs++ = c / 8 % 8 + '0';
				*qs++ = c % 8 + '0';
				break;
			}
		} else
			*qs++ = c;
	*qs = '\0';
}

void
unquote(char *us, char *qs)
{
	unsigned char c;
	int i;

	while (c = *qs++)
		if (c == '\\') {
			c = *qs++;
			switch (c) {
			case '\\':
				*us++ = '\\';
				break;
			case 'a':
				*us++ = '\a';
				break;
			case 'b':
				*us++ = '\b';
				break;
			case 'f':
				*us++ = '\f';
				break;
			case 'n':
				*us++ = '\n';
				break;
			case 'r':
				*us++ = '\r';
				break;
			case 't':
				*us++ = '\t';
				break;
			case 'v':
				*us++ = '\v';
				break;
			default:
				*us = 0;
				for (i = 0; i < 3; ++i) {
					if (c < '0' || c > '7')
						break;
					*us = *us * 8 + c - '0';
					c = *qs++;
				}
				--qs;
				++us;
				break;
			}
		} else
			*us++ = c;
	*us = '\0';
}
