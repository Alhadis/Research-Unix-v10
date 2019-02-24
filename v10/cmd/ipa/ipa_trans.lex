%Start P1 P2 P3
%{
void ipa_out();
%}
%%
"@ipa("	{ BEGIN P1; }
"@ipa{"	{ BEGIN P2; }
"@ipa/" { BEGIN P3; }
<P1>")"	{ BEGIN 0; }
<P2>"}" { BEGIN 0; }
<P3>"/" { BEGIN 0; }
<P1,P2,P3>"p"	ipa_out(1,1);
<P1,P2,P3>"t"	ipa_out(1,3);
<P1,P2,P3>"C"	{ ipa_out(1,3); ipa_out(1,66); }
<P1,P2,P3>"k"	ipa_out(1,5);
<P1,P2,P3>"b"	ipa_out(1,2);
<P1,P2,P3>"d"	ipa_out(1,4);
<P1,P2,P3>"J"	{ ipa_out(1,4); ipa_out(1,67); }
<P1,P2,P3>"g"	ipa_out(1,11);
<P1,P2,P3>"m"	ipa_out(1,6);
<P1,P2,P3>"n"	ipa_out(1,7);
<P1,P2,P3>"G"	ipa_out(1,39);
<P1,P2,P3>"l"	ipa_out(1,8);
<P1,P2,P3>"r"	ipa_out(1,59);
<P1,P2,P3>"f"	ipa_out(1,9);
<P1,P2,P3>"T"	ipa_out(1,57);
<P1,P2,P3>"D"	ipa_out(1,58);
<P1,P2,P3>"s"	ipa_out(1,18);
<P1,P2,P3>"S"	ipa_out(1,66);
<P1,P2,P3>"Z"	ipa_out(1,67);
<P1,P2,P3>"v"	ipa_out(1,19);
<P1,P2,P3>"z"	ipa_out(1,22);
<P1,P2,P3>"w"	ipa_out(1,20);
<P1,P2,P3>"y"	ipa_out(1,99);
<P1,P2,P3>"h"	ipa_out(1,10);
<P1,P2,P3>"?"	ipa_out(1,35);
<P1,P2,P3>"Q"	ipa_out(1,52);
<P1,P2,P3>"E"	ipa_out(1,91);
<P1,P2,P3>"i"	ipa_out(2,8);
<P1,P2,P3>"e"	ipa_out(1,93);
<P1,P2,P3>"A"	{ ipa_out(1,92); ipa_out(2,71); }
<P1,P2,P3>"a"	ipa_out(2,12);
<P1,P2,P3>"o"	ipa_out(1,95);
<P1,P2,P3>"@"	ipa_out(2,2);
<P1,P2,P3>"^"	ipa_out(2,3);
<P1,P2,P3>"O"	ipa_out(1,97);
<P1,P2,P3>"U"	ipa_out(1,98);
<P1,P2,P3>"u"	ipa_out(2,9);
<P1,P2,P3>"W"	{ ipa_out(1,94); ipa_out(2,73); }
<P1,P2,P3>"I"	{ ipa_out(1,94); ipa_out(2,71); }
<P1,P2,P3>"Y"	{ ipa_out(1,96); ipa_out(2,71); }
<P1,P2,P3>"R"	ipa_out(2,77);
<P1,P2,P3>"%"	ipa_out(2,99);
<P1,P2,P3>"c"	ipa_out(1,96);
<P1,P2,P3>">"	ipa_out(1,96);
<P1,P2,P3>"&"	ipa_out(2,14);
<P1,P2,P3>"*"	ipa_out(2,6);
<P1,P2,P3>"#"	ECHO;
<P1,P2,P3>"+"	ECHO;
<P1,P2,P3>"'"	ipa_out(2,78);
<P1,P2,P3>"."	ipa_out(2,79);
<P1,P2,P3>"N"	ipa_out(2,76);
<P1,P2,P3>"L"	ipa_out(2,74);
<P1,P2,P3>":"   ipa_out(2,66);
<P1,P2,P3>"["[1-2]"."[0-9]+"]"		{
		/* any member of one of the two fonts */
		ipa_out(atoi(&yytext[1]),atoi(&yytext[3]));
	}
<P1,P2,P3>[ 	\n]	ECHO;
<P1,P2,P3>.	{ 
		ipa_out(2,68);	 /* a slightly raised 'x'
					    for all characters not otherwise
					    defined */
		}
.	ECHO;
%%
void
ipa_out(fontnum,charnum)
int fontnum, charnum;
{
	printf("\\f(P%d\\N'%d'\\fP",fontnum,charnum);
/* should check that charnum is in range */
}
