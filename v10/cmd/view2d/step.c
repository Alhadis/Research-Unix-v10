/* steps for displaying color map or grey scale */
/* short tab[] = {0,1,3,7,15,31,63,127,255}; */
short tab[] = {0,32,64,96,128,159,191,223,255};
main()
{
  short w[512][512];
  int i, j;
  for(i=0;i<512;i++){
    for(j=0;j<512;j++){
      w[j][i] = 255-tab[(3*i/512) + 3*(3*j/512)];
    }
  }
  view2d(1,512,512,0.,0,0,1,0,255,w);
  exit(0);
}
