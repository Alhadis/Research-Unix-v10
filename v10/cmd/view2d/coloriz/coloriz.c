/* program to convert the output of view2d -Th,
  namely a stream for grey-scale bytes,
  into pictures(5) format suitable for color display
  on the barco or making photos on the matrix.
  Eric Grosse  28 Jul 87
*/
#include <stdio.h>
#include <math.h>
#include <assert.h>

char *progname;
main(argc,argv)
  int argc;
  char **argv;
{
  int i;
  int nx=512, ny=512;
  int bytesin=0;
  char *map;
  int mapr[256], mapg[256], mapb[256];
  int mat=0;
  progname=argv[0];
  map = NULL;
  for(argc--, argv++; *argv && (**argv == '-' ); argv++){
    switch(argv[0][1]){
     case 'M':
       mat = 1;
       break;
     case 'm':
       map = &argv[0][2];
       break;
     case 'n':
       i = sscanf(&argv[0][2], "%hd, %hd", &nx, &ny);
       if(i == 1) { ny = nx; i = 2; }
       if((i!=2)||(nx<0)||(ny<0)) error("bad NX, NY");
       break;
     default:
     err:
       fprintf(stderr,"unrecognized option: %s\n",argv[0]);
       exit(1);
     }
  }
  cvmap(map,256,mapr,mapg,mapb);
  printf("TYPE=dump\n");
  printf("WINDOW=0 0 %d %d\n",mat?512:nx,mat?512:ny);
  printf("NCHAN=3\n");
  printf("CHAN=rgb\n");
  printf("\n");
  if(mat) for(i=0;i<512*(512-ny)/2;i++){ putchar(0);putchar(0);putchar(0); }
  while( (i=getchar()) != EOF ){
     bytesin++;
     assert(i>=0);
     assert(i<256);
     if(mat && (bytesin%nx==0))
      for(i=0;i<(512-nx)/2;i++){ putchar(0);putchar(0);putchar(0); }
     putchar((char)mapr[i]); putchar((char)mapg[i]); putchar((char)mapb[i]);
     if(mat && (bytesin%nx==0))
      for(i=0;i<(512-nx)/2;i++){ putchar(0);putchar(0);putchar(0); }
  }
  if(mat) for(i=0;i<512*(512-ny)/2;i++){ putchar(0);putchar(0);putchar(0); }
  if(bytesin!=(nx*ny)){
    fprintf(stderr,"%d bytes read, %d expected\n",bytesin,nx*ny);
    exit(1);
  }
  exit(0);
}
