#include <stdio.h>
#include <math.h>
main()
{
  /* generates table for -mf option in view2d */
  /* all spectral colors:  blue to green to red */
  int nc=64;  /* number of colors (-c64 option) */
  int maxi=nc-1;  /* max color index */
  int i, max1;
  double r, g, b;
  max1=maxi/2;
  /* sqrt's here are for gamma correction of monitor */
  for(i=0; i<=max1; i++){
    r=sqrt(0.);
    g=sqrt(((double)i)/max1);
    b=sqrt(1-((double)i)/max1);
    printf("%d %d %d\n",(int)(255.499*r), (int)(255.499*g), (int)(255.499*b));
  }
  for(i=max1+1; i<=maxi; i++){
    r=sqrt(((double)i-max1)/(maxi-max1));
    g=sqrt(1-((double)i-max1)/(maxi-max1));
    b=sqrt(0.);
    printf("%d %d %d\n",(int)(255.499*r), (int)(255.499*g), (int)(255.499*b));
  }
}
