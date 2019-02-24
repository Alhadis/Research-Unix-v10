/* generate test image for calibrating Matrix and 691 film */
#include <fio.h>

#define t(i) (char)(i)
#define RED 0
#define GREEN 1
#define BLUE 2
char pict[512][512][3];

main()
{
  char *picstop, *q;
  int i, j, k;

  /* clear pict */
  q = &pict[0][0][0];
  picstop = q + sizeof(pict);
  while(q<picstop) *q++ = 0;

  i=0;j=0;k=0; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=1;j=0;k=63; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=2;j=0;k=80; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=3;j=0;k=120; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=0;j=1;k=150; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=1;j=1;k=180; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=2;j=1;k=215; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=3;j=1;k=235; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=0;j=2;k=255; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=1;j=2; fill(&pict[56+100*j][56+100*i][0],t(255),t(0),t(0));
  i=2;j=2; fill(&pict[56+100*j][56+100*i][0],t(0),t(255),t(0));
  i=3;j=2; fill(&pict[56+100*j][56+100*i][0],t(0),t(0),t(255));
  i=0;j=3;k=255; fill(&pict[56+100*j][56+100*i][0],t(k),t(k),t(k));
  i=1;j=3; fill(&pict[56+100*j][56+100*i][0],t(255),t(0),t(255));
  i=2;j=3; fill(&pict[56+100*j][56+100*i][0],t(255),t(255),t(0));
  i=3;j=3; fill(&pict[56+100*j][56+100*i][0],t(0),t(255),t(255));

  Fprint(1,"TYPE=dump\n");
  Fprint(1,"WINDOW=0 0 512 512\n");
  Fprint(1,"NCHAN=3\n");
  Fprint(1,"CHAN=rgb\n");
  Fprint(1,"\n");
  Fflush(1);
  write(1,pict,sizeof(pict));
  exit(0);
}

fill(q,r,g,b)
  char *q, r, g, b;
{
  int i, j;
  for(i=0;i<100;i++){
    for(j=0;j<100;j++){
	*q++ = r;
	*q++ = g;
	*q++ = b;
    }
    q += 3*(512-100);
  }
}
