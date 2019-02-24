/* step wedge for displaying color map or grey scale */
#include <fio.h>
#include <stdio.h>

main(argc,argv)
  char **argv;
{
  short *w;
  int i, j, nx=255, ny=20, nc=255;
  
  for(argc--, argv++; *argv && (**argv == '-' ); argv++){
    switch(argv[0][1]){
     case 'c':
        nc = atoi(&argv[0][2]);
        break;
     case 'n':
        i = sscanf(&argv[0][2], "%d, %d", &nx, &ny);
        if(i == 1) { ny = nx/10; i = 2; }
	break;
    }
  }
  w = (short *)malloc(nx*ny*sizeof(short));
  Fprint(2,"nx	%d\n",nx);
  Fprint(2,"ny	%d\n",ny);
  Fprint(2,"nc	%d\n",nc);

  for(i=0;i<nx;i++){
    for(j=0;j<ny;j++){
      w[i+j*nx] = i;
    }
  }
  view2d(1,nx,ny,0.,0,0,1,1,nc,w);
  exit(0);
}
