/* Copyright 1989 by AT&T Bell Laboratories */
/* slinkdata.c -- group files into text segment of a .s file */

#ifndef MIPS
#define DATALIST	    "_datalist"
#define END_SYMBOL  "_endmo"
#define WORD        ".long"
#else
#define DATALIST	    "datalist"
#define END_SYMBOL  "endmo"
#define WORD        ".word"
#endif

#include "tags.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

void die(s)
    char *s;
{
    perror(s);
    exit(1);
}

  
#define WORDS_PER_LINE 6
main(argc, argv)
    int argc;
    char *argv[];
{
    int i, sz, n;
    struct stat st;
    FILE *in;
    int buf[WORDS_PER_LINE];

    if (argc <= 1) die("Not enough arguments");

    printf(".text\n.globl %s\n", DATALIST);


    for (i=1; i<argc; i++) {
	printf(".align 2\n");
	printf("1: %s 0x31\n",WORD); /* descriptor */
	if (i==1) printf("%s: ",DATALIST);
	printf("%s 2f+4\n",WORD); /* filename */
	printf("%s 3f+4\n",WORD); /* contents */
	printf("%s %s\n",WORD,i==argc-1 ? "1" : "1f+4"); /* next entry */
	printf("2: %s %d\n",WORD,mak_desc(strlen(argv[i]),tag_string));
	printf(".ascii \"%s\"\n",argv[i]);
	printf(".align 2\n");
	if (stat(argv[i], &st) == -1)
	  die("stat");
	sz = st.st_size;
	if (sz % 4 != 0) die ("file size not a multiple of 4");
	printf("3:%s %d\n",WORD,mak_desc(sz,tag_string));
        if ((in=fopen(argv[i],"r"))==NULL) die ("open");
	while(n=fread(buf,sizeof(int),WORDS_PER_LINE,in)) {
	    int j=0;
	    printf("%s 0x%x",WORD,buf[0]);
	    for(j=1;j<n;j++) printf(", 0x%x",buf[j]);
	    printf("\n");
	}
	fclose(in);
    }
}

