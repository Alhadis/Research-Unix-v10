#include <stdio.h>
#include <libc.h>
#include "3610op.h"

main()
{
  msnare();
  mcmd(ZOOM);mbyte(0);
  mcmd(MOVP1);mword(0);mword(0);
  mcmd(PPAN);
  mflush();
  exit(0);
}
