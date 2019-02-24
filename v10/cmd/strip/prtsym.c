#include <stdio.h>
#include <a.out.h>
#include <time.h>
#define	DESCNAMES
#define	DESCSYMBS
#include "stab.h"

int strdebug;

static char *typnam[128];
static int init;

prtsym(stream,sym)
FILE *stream; register struct nlist *sym;
{
	int jtype, jext; char tqual[8]; struct tm *ctm, *localtime();
	register i; register unsigned w;
	if (!init) {
		typnam[N_UNDF/2] = "UNDF";
		typnam[N_ABS/2] = "ABS";
		typnam[N_TEXT/2] = "TEXT";
		typnam[N_DATA/2] = "DATA";
		typnam[N_BSS/2] = "BSS";
	
		typnam[N_SO/2] = "SO";
		typnam[N_ESO/2] = "ESO";
		typnam[N_SOL/2] = "SOL";
		typnam[N_SLINE/2] = "SLINE";
		typnam[N_BFUN/2] = "BFUN";
		typnam[N_ENTRY/2] = "ENTRY";
		typnam[N_RFUN/2] = "RFUN";
		typnam[N_LBRAC/2] = "LBRAC";
		typnam[N_RBRAC/2] = "RBRAC";
		typnam[N_EFUN/2] = "EFUN";
		typnam[N_STSYM/2] = "STSYM";
		typnam[N_LCSYM/2] = "LCSYM";
		typnam[N_ECOML/2] = "ECOML";
		typnam[N_GSYM/2] = "GSYM";
		typnam[N_STFUN/2] = "STFUN";
		typnam[N_RSYM/2] = "RSYM";
		typnam[N_LSYM/2] = "LSYM";
		typnam[N_PSYM/2] = "PSYM";
		typnam[N_BSTR/2] = "BSTR";
		typnam[N_ESTR/2] = "ESTR";
		typnam[N_SSYM/2] = "SSYM";
		typnam[N_SFLD/2] = "SFLD";
		typnam[N_BCOMM/2] = "BCOMM";
		typnam[N_ECOMM/2] = "ECOMM";
		typnam[N_FNAME/2] = "FNAME";
		typnam[N_LENG/2] = "LENG";
		typnam[N_NARGS/2] = "NARGS";
		typnam[N_TYID/2] = "TYID";
		typnam[N_DIM/2] = "DIM";
		typnam[N_VER/2] = "VER";
		init++;
	}
	jtype = sym->n_type/2;
	jext  = sym->n_type&N_EXT;
	fputs(sym->n_un.n_name, stream); putc('\t', stream);
	if (strlen(sym->n_un.n_name) < 8) putc('\t', stream);
	fputs(typnam[jtype], stream);
	if (jext) putc('\'', stream); putc('\t', stream);

	switch (sym->n_type&(~N_EXT)) {
	case N_UNDF:	/* undefined */
	case N_ABS:	/* absolute */
	case N_TEXT:	/* text */
	case N_DATA:	/* data */
	case N_BSS:	/* bss */
	
	case N_SO:	/* source file name: name,,address */
	case N_SOL:	/* #line source filename: name,,address */
	case N_SLINE:	/* src line: ,linenumber,address */
	case N_RFUN:	/* return from function:  ,,0,linenumber,address */
	case N_ESO:	/* end source file: name,,0,0,address */
	case N_ENTRY:	/* alternate entry: name,linenumber,address */
	case N_BFUN:	/* procedure: name,lineno,address */
	case N_EFUN:	/* end of function: name,lineno,address */
	case N_LBRAC:	/* left bracket: ,nesting level,address */
	case N_RBRAC:	/* right bracket: ,nesting level,address */
	case N_ECOML:	/* end common (local name): ,,address */
	case N_BCOMM:	/* begin common: name,, */
	case N_ECOMM:	/* end common: name,, */
	case N_FNAME:	/* procedure name (f77 kludge): name,,0 */
	case N_NARGS:	/* number of args in the function call */
		fprintf(stream,"\t%5d\t%08x",
			sym->n_desc,sym->n_value);
		break;

	case N_LENG:	/* additional entry with length: ,,length */
	case N_TYID:	/* struct, union, or enum name */
	case N_DIM:	/* dimension for arrays, etc */
		fprintf(stream,"\t%5d\t%8d",
			sym->n_desc,sym->n_value);
		break;
	
	case N_BSTR:	/* begin structure: name,, */
	case N_ESTR:	/* end structure: name,, */
	case N_STSYM:	/* static symbol: name,type,address */
	case N_LCSYM:	/* .lcomm symbol: name,type,address */
	case N_GSYM:	/* global sym: name,type,0 */
	case N_STFUN:	/* static function: name,type,0 */
	case N_RSYM:	/* register sym: name,register,offset */
	case N_LSYM:	/* local sym: name,type,offset */
	case N_PSYM:	/* parameter: name,type,offset */
	case N_SSYM:	/* structure elt: name,type,struct_offset */
		i = 0; w = sym->n_desc >> (BTSHIFT-TSHIFT);
		while (tqual[i++] = qualchars[(w >>= TSHIFT)&MTMASK]);
		while (i++ < 7) putc(' ', stream);
		printf("%s%s\t%08x",
			tqual,basenames[BTYPE(sym->n_desc)],
			sym->n_value);
		break;
	
	case N_SFLD:	/* structure field: name,,0,type,struct_offset */
		printf(" %s<%d>\t%08x",
			basenames[BTYPE(sym->n_desc)], sym->n_desc >> BTSHIFT,
			sym->n_value);
		break;
	
	case N_VER:	/* symbol table version */
		ctm = localtime(&sym->n_value);
		fprintf(stream,"%2d.%02d.%02x  %2d.%02d.%02d.%02d:%02d",
			((sym->n_desc>>12)&0x0f)+80,
			(sym->n_desc>>8)&0x0f, sym->n_desc&0xff,
			ctm->tm_year, ctm->tm_mon+1, ctm->tm_mday,
			ctm->tm_hour,ctm->tm_min);
		break;
	
	default:
		fprintf(stream,"%02x %02x\t%04x\t%08x",
			sym->n_type,sym->n_other,sym->n_desc,sym->n_value);
		break;
	}
	if (strdebug && sym->n_un.n_name)
		fprintf(stream,"\t%8x",sym->n_un.n_name);
	putc('\n', stream);
}
