#include <stdio.h>
#include <libc.h>
#include "3610op.h"

main()
{
  msnare();
  mcmd(INI);
  mcmd(WRMSKL); mlong(0xffffffff);
  mcmd(WRBANK); mbyte(15);
  mcmd(ZOOM);mbyte(0);
  mcmd(MOVP1);mword(0);mword(0);
  mcmd(PPAN);
  mcmd(SELRES); mbyte(0xc2); mbyte(0);  /* 24-bit mode */
  mcmd(CMSEL); mbyte(0); mbyte(0);	/* write in cmap buffer 0 */
  mcmd(CMRAMP);mword(0);mword(256);     /* default linear color map */
    mbyte(0);mbyte(0);mbyte(0);
    mbyte(255);mbyte(255);mbyte(255);
  mcmd(CMACT);				/* make current buffer active */
  mcmd(SETCLL); mbyte(0); mbyte(0); mbyte(0); mbyte(0);
  mcmd(CLEAR);				/* clear all pixels */
  mflush();
  exit(0);
}
