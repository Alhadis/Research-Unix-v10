%{
#include <ctype.h>
int i;
%}
L	[a-z0-9]
%%
^be[\t].+[\n]	{
	for(i=0;i<yyleng;i++)
		if(yytext[i] == '\t')break;
	while(++i<yyleng-1){
		if(islower(yytext[i]))
			putchar(toupper(yytext[i]));
		else putchar(yytext[i]);
	}
	putchar(' ');
	}
^prep[\t].+[\n]	{
	for(i=0;i<yyleng;i++)
		if(yytext[i] == '\t')break;
	while(++i<yyleng-1){
		if(islower(yytext[i]))
			putchar(toupper(yytext[i]));
		else putchar(yytext[i]);
	}
	putchar(' ');
	}
^"verb"[\t]"be"[\n]	{
	printf("BE");
	putchar(' ');
	}
^{L}+[\t].+[\n]	{
	for(i=0;i<yyleng;i++)
		if(yytext[i] == '\t')break;
	while(++i<yyleng-1){
		if(isupper(yytext[i]))
			putchar(tolower(yytext[i]));
		else
			putchar(yytext[i]);
	}
	putchar(' ');
	}
^[.,;:][\t][.,;:!][\n]	{
	putchar(yytext[2]);
	putchar('\n');
	}
^"..."[\t]"..."[\n]	{
	printf(" ...");
	}
^[\[\]()\{\}\"-][\t][\[\]()\{\}\"-][\n]	{
	putchar(yytext[0]);
	}
^"``"[\t]"``"[\n]	{
	printf("``");
	}
^"--"[\t]"--"[\n]	{
	printf(" -- ");
	}
^"."[\t]".\""	{
	printf(" .\"");
	}
^"'	'"[\n]	{
	putchar(yytext[0]);
	}
^"''"[\t]"''"[\n]	{
	printf(" ''");
	}
^"`	`"[\n]	{
	putchar(yytext[0]);
	}
