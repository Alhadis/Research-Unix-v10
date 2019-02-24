#include <ctype.h>
#include "common.h"
#include "code.h"
#include "sym.h"
#include "y.tab.h"

int yyline = 1;
char token_buffer[MAXIDSIZE+1];
extern YYSTYPE yylval;
static void ScanCodeBlock();
static void ScanComment();
static Code *curCdBlock;
static char get();

yylex()
{
	register c;
	register char *cp;
	int in_ident = 0;
	yylval.y_nodep = (struct node *) NULL;
	cp = token_buffer;
	while((c=getchar())!=EOF) {
		switch(c) {
		case ' ': case '\t': case '\f':
			continue;
		case '@': case '[': case ']': case ';': case ':':
		case '(': case ')': case ',': case '=':
		case '*':
			if(debug_flag&DB_LEX) {
				putc(c,stderr);
				putc('\n', stderr);
			}
			*cp++ = c;
			*cp = '\0';
			return(c);

		case '{':
			ScanCodeBlock();
			yylval.y_code = curCdBlock;
			curCdBlock = NULL;
			*cp++ = '{'; *cp++ = '}';
			return(CBLOCK);

		case '\n':
			yyline++;
			continue;
		case '/':
			if ((c=getchar())=='*') {
				ScanComment(get);
				continue;
			} else {
				ungetc(c, stdin);
				c = '/';
			}
			/* FALL THRU */

		default:
			if (isdigit(c)) {
				int errs = 0;
				do {
					if(cp > &token_buffer[MAXIDSIZE]) {
						token_buffer[MAXIDSIZE] = '\0';
						yyerror("number too long");
						errs++;
					} else *cp++ = c;
					c = getchar();
				} while (isdigit(c));
				if(isalpha(c))
					yyerror2("illegal digit '%c'", c);
				ungetc(c, stdin);
				if(errs)
					return(ERROR);
				yylval.y_int = atoi(token_buffer);
				return(NUMBER);
			}
			if (isalpha(c)) {
				SymbolEntry *sp;
				int errs = 0;
				do {
					if(cp > &token_buffer[MAXIDSIZE]) {
						token_buffer[MAXIDSIZE] = '\0';
						yyerror("ID too long");
						errs++;
					} else *cp++ = c;
					c = getchar();
				} while (isalpha(c)||isdigit(c)||c=='_');
				ungetc(c, stdin);
				if(errs)
					return(ERROR);
				*cp = '\0';

				sp = SymbolLookup (token_buffer);
				if (sp==NULL) {
					/* undefined */
				    yylval.y_symp = SymbolAllocate(token_buffer);
				} else {
				    /* already defined */
				    if (sp->attr == A_KEYWORD)
						return (sp->sd.keyword);
				    yylval.y_symp = sp;
				}
				if(debug_flag&DB_LEX)
					fprintf(stderr, "ID\n");
				return(ID);
			}
			yyerror2("illegal character (\\%03o)", c);
		}
	}
	strcpy(token_buffer, "EOF");
	return(0);
}

void
LexInit()
{
}

lexCleanup()
{
}

/*
 * Beware: ungets of the characters from these routines may screw up the
 * line count
 */
static char
getput()
{
	/* keutzer
	char c;
	*/
	int c;
	c = getchar();
	if(c=='\n') yyline++;
	if(c!=EOF)
		curCdBlock = CodeStoreChar(curCdBlock, c);
	return(c);
}

static char
get()
{
	/* keutzer
	char c;
	*/
	int c;
	c = getchar();
	if(c=='\n') yyline++;
	return(c);
}

extern int nerrors;

static void
ScanComment(rdfunc)
	char (*rdfunc)();
{
	int startline = yyline;
	/* keutzer
	char c;
	*/
	int c;
	int saw_star = 0;
	while ((c=rdfunc())!=EOF) {
		if (c=='*')
			saw_star++;
		else if(c=='/' && saw_star) {
			return;
		} else saw_star = 0;
	}
	yyerror2("unexpected EOF in comment beginning on line %d", startline);
	nerrors++;
	cleanup(1);
}

static
ScanString(rdfunc)
	char (*rdfunc)();
{
	int startline = yyline;
	/* keutzer
	char c;
	*/
	int c;
	int saw_backsl = 0;

	while((c=rdfunc())!=EOF) {
		if (c=='"' && !saw_backsl)
			return;
		if (c=='\\' && !saw_backsl) {
			saw_backsl = 1;
			continue;
		}
		saw_backsl = 0;
	}
	/* fall thru due to EOF */
	yyerror2("unexpected EOF in string beginning on line %d", startline);
	nerrors++;
	cleanup(1);
}

static
ScanChar()
{
	int startline = yyline;
	/* keutzer
	char c;
	*/
	int c;
	int saw_backsl = 0;

	while((c=getput(stdin))!=EOF) {
		if (c=='\'' && !saw_backsl)
			return;
		if (c=='\\' && !saw_backsl) {
			saw_backsl = 1;
			continue;
		}
		saw_backsl = 0;
	}
	/* fall thru due to EOF */
	yyerror2("unexpected EOF in character constant beginning on line %d",
		 startline);
	nerrors++;
	cleanup(1);
}

static void
ScanTreeReference()
{

	/* keutzer
	char c;
	*/
	int c;
	c = getchar();
	if(c=='%') {
		curCdBlock = CodeStoreString (curCdBlock, "_ll[(");
		while ((c=getchar())!=EOF && c!='$') {
			if(!isdigit(c)) {
				yyerror("unclosed $ reference");
				ungetc(c,stdin);
				break;
			}
			curCdBlock = CodeStoreChar(curCdBlock, c);
		}
		curCdBlock = CodeStoreString (curCdBlock, ")-1]");
		return;
	}
	else if(c=='$') {
		curCdBlock = CodeStoreString(curCdBlock, "root");
		return;
	} else curCdBlock = CodeStoreString(curCdBlock, "_mtG(root,");
	do {
		if(!isdigit(c) && c!='.') {
			yyerror("unclosed $ reference");
			ungetc(c,stdin);
			break;
		}
		curCdBlock = CodeStoreChar(curCdBlock, c=='.' ? ',' : c);
	} while((c=getchar())!=EOF && c!='$');
	curCdBlock = CodeStoreString(curCdBlock, ", -1)");
}

static void
ScanCodeBlock()
{
	int startline = yyline;
	/* keutzer
	char c;
	*/
	int c;
	if(curCdBlock==NULL) {
		curCdBlock = CodeGetBlock();
		curCdBlock = CodeMarkLine(curCdBlock,yyline);
	}
	while((c=getc(stdin))!=EOF) {
		if (c=='}')
			return;
		else if (c=='$') ScanTreeReference();
		else curCdBlock = CodeStoreChar(curCdBlock, c);
		if (c=='\n') yyline++;
		if (c=='"') ScanString(getput);
		else if (c=='\'') ScanChar();
		else if (c=='/') {
			if ((c=getc(stdin))=='*') {
				curCdBlock = CodeStoreChar(curCdBlock, '*');
				ScanComment(getput);
			}
			else ungetc(c, stdin);
		}
		else if (c=='{') {
			ScanCodeBlock();
			curCdBlock = CodeStoreChar (curCdBlock, '}');
		}
	}
	yyerror2("{ on line %d has no closing }", startline);
	nerrors++;
}
