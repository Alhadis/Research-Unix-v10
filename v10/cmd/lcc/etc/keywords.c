#include <stdio.h>
#include <ctype.h>

char *keyword[] = {
	"auto", "break", "case", "*char", "const", "continue",
	"default", "*double", "do", "else", "enum", "extern", "*float", "for",
	"goto", "if", "*int", "*long", "register", "return", "*short",
	"signed", "sizeof", "static", "struct", "switch", "typedef", "union",
	"unsigned", "*void", "volatile", "while"
};

main() {
	char *k, *lastk = 0;
	int i;

	for (i = 0; i < sizeof keyword/sizeof keyword[0]; i++) {
		k = keyword[i];
		if (*k == '*')
			k++;
		if (lastk && *lastk != *k)
			printf("\tgoto id;\n");
		if (lastk == 0 || *lastk != *k)
			printf("case '%c':\n", *k);
		lastk = k;
		printf("\tif (rcp[0] == '%c'\n", *++k);
		while (*++k && *k != '*')
			printf("\t&&  rcp[%d] == '%c'\n", k - lastk - 1, *k);
		printf("\t&& !(map[rcp[%d]]&(DIGIT|LETTER))) {\n", k - lastk - 1);
		printf("\t\tcp = rcp + %d;\n", k - lastk - 1);
		if (*keyword[i] == '*')
			printf("\t\ttsym = %stype->u.sym;\n", lastk);
		printf("\t\treturn ");
		for (k = lastk; *k; k++)
			putchar(toupper(*k));
		printf(";\n");
		printf("\t}\n");
	}
	for (k = "hjkmnopqxyzABCDEFGHIJKMNOPQRSTUVWXYZ_"; *k; k++)
		printf("case '%c':\n", *k);
	return 0;
}
