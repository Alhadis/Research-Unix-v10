#include <libc.h>
#include "3610op.h"
main(argc,argv)
  char **argv;
{
  msnare();
  mcmd(INI);				/* power-up reset, without self-test */
  mcmd(CMSEL); mbyte(0); mbyte(0);	/* write in cmap buffer 0 */
  mcmd(CMRAMP);mword(0);mword(256);     /* default linear color map */
    mbyte(0);mbyte(0);mbyte(0);
    mbyte(255);mbyte(255);mbyte(255);
  mcmd(CMACT);				/* make current buffer active */
  mcmd(CLEAR);				/* clear all pixels */
  mflush();
  exit(0);
}
