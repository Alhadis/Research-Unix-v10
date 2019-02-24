/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     T. Thompson - ATT-BL HO - first versions        */
/* Routines for generating PostScript from RLE. */
/* The routines at the bottom of this file, which */
/* do the actual conversion to postscript, were */
/* hacked out of 'sun2ps'.  The original header */
/* giving credit to its authors is there. */

/* The 'binary' version of the output is collected in a */
/* temporary file, which is then fed back to the routines */
/* that generate postscript in a run-length-encoded form. */
/* Ideally it should just convert the original run-length */
/* encoding directly, but the REAL bottleneck is the */
/* PostScript printer, of course.  Printing an 800 by 800 */
/* pixel image takes 5 minutes. */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "CPU.h"
#include "boole.h"
#include "limits.h"     /* numeric extreme values */
#include "Units.h"
#include "Coord.h"
#include "pic.h"

FILE *Fp, *tmpfile();
int Raswidth;
int Raslength;
int Rasbytes;	/* bytes per row */
int Inv = 0;
int Aspect = 1;		/* if non-zero, retain original aspect ratio */
int Land = 0;		/* if non-zero, print in landscape mode */
char Revbyte[256];
/*  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 */
char Revnib[16] = {
    0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15
    };
 
void
POST_start(h)
PIC_hdr *h;
{
	int n, v;

	/* build a table of reversed bytes */
	for ( n=0; n<256; n++ )
		Revbyte[n] = Revnib[(n>>4)&0xf] | Revnib[n&0xf]<<4;

	h->bpl = (h->bpl+7)/8;
	Rasbytes = h->bpl;
	Raslength = h->bpl * h->bx.b.y;
	Raswidth = h->bx.b.x;
	postmain(1,NULL,Raswidth,h->bx.b.y);
#if CPU==SUN
	Fp = tmpfile();
#else
	Fp = fopen("tmp.postlib.0","rw");
#endif
}

void
POST_end()
{
	int c;
	rewind(Fp);
	(VOID) Encode(Raslength, Inv);
	PrintPostScriptClosing();
	fclose(Fp);
}

/* write a full line of picture data, returning status:  1 OK, 0 EOF, -1 ERR */
int POST_wline(h,line)
    PIC_hdr *h;
    unsigned char *line;
{   int stat;
        if( (stat=fwrite(line,1,h->bpl,Fp)) == h->bpl) {
                h->seek += h->bpl;
                h->cy++;
                return(1);
                }
        else { /* ERR */
                err("write to fd%d stat%d",fileno(h->fp),stat);
                if((stat>=0)&&(stat<h->bpl)) return(0 /*EOF*/);
                else return(-1);
                };
        }

/******************************************************************************
*									      *
*	File:         sun2ps.c						      *
*	Author:       Glenn Boysko					      *
*	Organization: Case Western Reserve University			      *
*	EMail:	      {decvax, sun}!mandrill!boysko			      *
*		      boysko@mandrill.cwru.edu				      *
*	Created:      Wed Mar 23 9:25pm					      *
*	Contents:     Sun Rasterfile to PostScript image (using a run-length  *
*			encoding scheme.)				      *
*									      *
*	(Adapted from "postimage" filter by J. R. Bammi.)		      *
*									      *
*	@(#)sun2ps.c	1.8
******************************************************************************/

/*
 * Usage:
 *  sun2ps [-s sizex sizey] [-t transx transy] [-r rot] [-l] [-i] [-a] file ...
 *
 *	-s sizex sizey   = size of postscript image - default 7.5 x 10 inches.
 *	-t transx transy = translate image - default 0.5 0.5 inches
 *	-r rotate	 = rotate image     - default 0 degress
 *      -l		 = landscape (overrides *all* settings.) 
 *	-i		 = inverse image - default no inverse 
 *				(Inverse enabled implies white on black.)
 *	-a		 = maintain correct aspect ratio - default none.
 *
 */

/* Sun standard raster file format (as obtained by screendump(1)).
 *
 * Header	(8 16-bit quantities)
 * Color Map
 * Image
 *
 */

/* Header Format:
 * 
 * ras_magic		(int)	Raster Magic number 0x59a66a95
 * ras_width		(int)	Width of image in pixels.
 * ras_height		(int)	Height of image in pixels.
 * ras_depth		(int)	Bits per pixel.	Either 1 or 8 bits.
 * ras_length		(int)	Length of image in bytes.
 * ras_type		(int)	Type of file. Assumed to be RT_STANDARD (1) if
 *				produced by a screendump command.
 * ras_maptype		(int)	Type of color map. 
 * ras_maplength	(int)	Length of color map in bytes.
 *
 */

/* Ras_maplength bytes of Color map data. */

/* Ras_length bytes of Image data. */

/* Buffer and Input Modes... */
#define LITERAL	0
#define COPY	1
#define IGNORE	2

/* Transmission Variables. */
int BufCount;

unsigned char Buffer[128],
     	      CurrByte,
     	      NextByte,
     	      *BufferP = Buffer;

/* Diagnostic Variables. */
int	DiagNLongRuns = 0,
     	DiagMaxRunLength = 0,
     	DiagNumRuns = 0;
double	DiagSumRunLength = 0;

postmain(argc,argv,width,height)
int argc;
char **argv;
int width, height;
{
     char      *filename;
     double 	sizex, sizey, transx, transy, rotate;

     extern double atof();
     
     filename = "STDIN";
     sizex = 7.5;
     sizey = 10.0;
     transx = transy = 0.5;
     rotate = 0.0;
     
     while(--argc > 0)
     {
	  ++argv;
	  if((*argv)[0] == '-')
	  {
	       switch((*argv)[1])
	       {
		 case 'l':
		 case 'L':
		    Land = 1;
		    break;
		    
		 case 's':
		 case 'S':
		    sizex = atof(*++argv);
		    sizey = atof(*++argv);
		    argc -= 2;
		    break;
		    
		 case 't':
		 case 'T':
		    transx = atof(*++argv);
		    transy = atof(*++argv);
		    argc -= 2;
		    break;
		    
		 case 'r':
		 case 'R':
		    rotate = atof(*++argv);
		    argc--;
		    break;
		    
		 case 'I':
		 case 'i':
		    Inv = 1;
		    break;
		    
		 case 'A':
		 case 'a':
		    Aspect = 1;
		    break;
		    
		 default:
		    fprintf(stderr,"Illegal switch %c - ignored\n",
			    (*argv)[1]);
	       }
	  }
     }
     if (Land)
     {
	  transx = 8.0;
	  transy = 0.5;
	  sizex  = 10.0;
	  sizey  = 7.5;
	  rotate = 90.0;
     }
	if (Aspect) {
		if ((sizex / width) < (sizey / height)) {
			sizey = sizex * (height * 1.0 / width);
		}
		else {
			sizex = sizey * (width * 1.0 / height);
		}
	}
	PrintPostScriptRoutines(height, width, 1 /*depth*/ ,
			     transx, transy, sizex, sizey, rotate);
}

/******************************************************************************
*	I/O Routines.							      *
******************************************************************************/
int
gb()		/* Get a byte from Fp. */
{
     int byte;
     
     if (!feof(Fp))
	  byte = getc(Fp);
     else
	  Error("Premature EOF.\n");
     if (ferror(Fp))
	  Error("I/O Error.\n");
     return(Revbyte[byte]);
}

SendHex(Byte)		/* Send a Hex char to Stdout. */
unsigned char Byte;
{
     static int LineCount = 0;

     printf("%02x",  0xff & Byte);
     if (++LineCount == Rasbytes)
     {
	  putchar('\n');
	  LineCount = 0;
     }
}
     
int
SendBuffer(Inv)		/* Send a buffer to Stdout. Return BytesSent. */
int Inv;
{
     int i, BytesSent;
     
     if (BufferMode() == LITERAL)
     {
	  SendHex( (unsigned char) 0xff & BufCount );
	  for (i = 0; i < BufCount+1; i++)
	  {
	       SendHex( (Inv) ? Buffer[i] : ~Buffer[i]);
	  }
	  BytesSent = BufCount+2;
     }
     else if (BufferMode() == COPY)
     {
	  SendHex( (unsigned char) 0xff & (0x100 + BufCount) );
	  SendHex( (Inv) ? Buffer[0] : ~Buffer[0]);
	  BytesSent = 2;
	  DiagRecLRun(mag(BufCount)+1);
     }
     return(BytesSent);
}

/******************************************************************************
*	Utility Routines.						      *
******************************************************************************/
int
mag(Byte)	/* Magitude of a signed char. */
int Byte;
{
     if (Byte & 0x80)
     {
	  /* Signed */
	  Byte = ~(--Byte);
     }
     return( 0xff & Byte );
}
	  
/******************************************************************************
*	Buffer Management Routines.					      *
******************************************************************************/
int
InputMode()
{
     if (CurrByte == NextByte)
	  return(COPY);
     return(LITERAL);
}

int
BufferMode()
{
     if (BufCount >= 0 && BufCount <= 127)
	  return(LITERAL);
     else if (BufCount >= -127 && BufCount <= -1)
	  return(COPY);
     return(IGNORE);
}

InitLitMode(NBytes, Inv)
int *NBytes, Inv;
{
     BufferP    = Buffer;
     BufCount   = -1;
     ContLitMode(NBytes, Inv);
}

ContLitMode(NBytes, Inv)
int *NBytes, Inv;
{
     if (BufCount == 127)
     {
	  SendBuffer(Inv);
	  BufferP  = Buffer;
	  BufCount = -1;
     }
     *BufferP++ = CurrByte;
     BufCount++;
     CurrByte   = NextByte;
     NextByte   = (unsigned char) gb();
     (*NBytes)--;
}
     
InitCopyMode(NBytes, Inv)
int *NBytes, Inv;
{
     BufferP    = Buffer;
     *BufferP++ = CurrByte;
     BufCount   = -1;
     CurrByte   = (unsigned char) gb();
     NextByte   = (unsigned char) gb();
     *NBytes   -= 2;
}

ContCopyMode(NBytes, Inv)
int *NBytes, Inv;
{
     if (BufCount == -127)
     {
	  SendBuffer(Inv);
	  InitCopyMode(NBytes, Inv);
	  DiagNLongRuns++;
     }
     BufCount--;
     CurrByte   = NextByte;
     NextByte   = gb();
     (*NBytes)--;
}

/******************************************************************************
*	Encoding Algorithm.						      *
******************************************************************************/
int
Encode(NBytes, Inv)
int NBytes, Inv;
{
     int BytesSent = 0;
     
     /* Initialize Buffer, BufCount, NextByte, CurrByte */
     CurrByte = (unsigned char) gb();
     NextByte = (unsigned char) gb();
     if (InputMode() == LITERAL)
     {
	  InitLitMode(&NBytes, Inv);
     }
     else
     {
	  InitCopyMode(&NBytes, Inv);
     }
     while (NBytes > 3)
     {
	  switch(BufferMode())
	  {
	    case LITERAL:
	       if (InputMode() == COPY)
	       {
		    BytesSent += SendBuffer(Inv);
		    InitCopyMode(&NBytes, Inv);
	       }
	       else
	       {
		    ContLitMode(&NBytes, Inv);
	       }
	       break;
	    case COPY:
	       if (CurrByte == Buffer[0])
	       {
		    ContCopyMode(&NBytes, Inv);
	       }
	       else
	       {
		    BytesSent += SendBuffer(Inv);
		    if (InputMode() == COPY)
		    {
			 InitCopyMode(&NBytes, Inv);
		    }
		    else
		    {
			 InitLitMode(&NBytes, Inv);
		    }
	       }
	       break;
	    default:
	       Error("Bad Buffer Mode... Sorry\n");
	       break;
	  }
     }
     BytesSent += SendBuffer(Inv);
     /* Send out rem'g 2-3 bytes in LITERAL mode. */
     Buffer[0] = CurrByte;
     Buffer[1] = NextByte;
     if (NBytes == 3)
	  Buffer[2] = gb();
     BufCount = NBytes-1;
     BytesSent += SendBuffer(Inv);
     return(BytesSent);
}

/******************************************************************************
*	Diagnostic Routines.						      *
******************************************************************************/
DiagRecLRun(Rlength)
int Rlength;
{
#ifdef DIAGS
     if (Rlength > DiagMaxRunLength)
	  DiagMaxRunLength = Rlength;
     DiagSumRunLength += Rlength;
     DiagNumRuns++;
#endif
}

Diags()
{
#ifdef DIAGS
     fprintf(stderr, "Longest Run (<= 128) = %d\n", DiagMaxRunLength);
     fprintf(stderr, "Number of Runs over 128 = %d\n", DiagNLongRuns);
     fprintf(stderr, "Average Run Length of %d. (%d Runs)\n",
	     (int) DiagSumRunLength / DiagNumRuns, DiagNumRuns);
#endif
}

/******************************************************************************
*	PostScript Output Routines. 					      *
******************************************************************************/
PrintPostScriptRoutines(ras_h, ras_w, ras_d, tx, ty, sx, sy, rot)
int ras_h, ras_w, ras_d;
double tx, ty, sx, sy, rot;
{
     printf("%%!\n/inch {72 mul} def\n");
     printf("/bpp %d def\n", ras_d);
     printf("/scanlines %d def\n", ras_h);
     printf("/scansize %d def\n", ras_w);
     printf("/bitmapx\n{");
     printf(" %d %d %d [%d 0 0 %d 0 %d] ", ras_w, ras_h, ras_d, ras_w, 
	    -ras_h, ras_h);
     printf("{currentfile readrlehexstring pop } image\n} def\n");
     printf("gsave\n");
     printf("%f inch %f inch translate\n",tx, ty);
     printf("%f rotate\n", rot );
     printf("%f inch %f inch scale\n", sx, sy);
     printf("/readrlehexstring\t%% rle_file => decoded_string boolean\n");
     printf("{\n\t/fileptr exch def\n\tfileptr 1 string readhexstring {");
     printf("\n\t\t0 get dup 128 and 0 eq\n");
     printf("\t\t{ 1 add /Buffer exch string def\n");
     printf("\t\t\tfileptr Buffer readhexstring\n\t\t}\n\t\t{");
     printf(" 256 exch sub /BufCount exch def\n");
     printf("\t\t\t/Buffer BufCount 1 add string def\n");
     printf("\t\t\t/RunInt fileptr 1 string readhexstring");
     printf(" pop 0 get def\n");
     printf("\t\t\t0 1 BufCount { RunInt Buffer 3 1 roll put } for\n");
     printf("\t\t\tBuffer true\n\t\t} ifelse\n\t}\n\t{ false } ifelse\n");
     printf("} def\n");
     printf("/clipathx\n{\tnewpath\n\t0 0 moveto\n\t%f inch 0", sx);
     printf(" lineto\n\t%f inch %f inch lineto\n\t0 %f inch lineto\n",
	    sx, sy, sy);
     printf("\tclosepath\n} def\nclipathx clip\n");
     printf("bitmapx\n");
}

PrintPostScriptClosing()
{     
     printf("\ngrestore\n");
     printf("showpage\n");
}

/******************************************************************************
*	Error Routine.							      *
******************************************************************************/
Error(S1, S2, S3)
char *S1, *S2, *S3;
{
     fprintf(stderr, S1, S2, S3);
     exit(-1);
}
