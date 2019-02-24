/*
 *      init.c 1.3
 *
 *	Initialization Module for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/1/82 11:11:58
 *      Retrieved 11/15/82 13:22:27
 */
#include	<stdio.h>
#include	"vis.h"

struct qheader	Fixup, Depend;

struct rowhdr	Row;
struct colhdr	Col;

void pinit()
{
	Fixup.first = 0;
	Fixup.last = 0;
	Depend.first = 0;
	Depend.last = 0;
	Row.size = 0;
	Row.table = (struct rowlabel **)malloc((unsigned)4);
	Col.size = 0;
	Col.table = (struct collabel **)malloc((unsigned)4);
}
