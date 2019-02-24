/*%cc -O %
 */
char copr[] = "Copyright (c) 1976 Thomas S. Duff";
#include <stdio.h>
#define NCOLS (132+1)
#define CENTRE	1
#define UL	2
#define UR	4
#define LL	8
#define LR	16
char line[NCOLS+1][3];
char outline[NCOLS];
int eof;
int xs, ys;
int nsp;
char buf[BUFSIZ];
main(argc, argv)
char *argv[];
{
	extern fout;
	if(argc!=3){
		fprintf(stderr, "Usage: rnd verscale horscale\n");
		exit(1);
	}
	setbuf(stdout, buf);
	ys=atoi(argv[1]);
	xs=atoi(argv[2]);
	readline();
	while(!eof){
		readline();
		fill();
		writeline();
	}
}
readline(){
	register c;
	register i;
	for(i=1;i!=NCOLS;i++){
		line[i][0]=line[i][1];
		line[i][1]=line[i][2];
	}
	if(eof)
		for(i=1;i!=NCOLS;i++)
			line[i][3]=0;

	else{
		for(i=1;i!=NCOLS;i++){
			switch(getchar()){
			case EOF:
				eof++;
			case '\n':
				for(;i!=NCOLS;i++)
					line[i][2]=0;
				return;
			case ' ':
				line[i][2]=0;
				break;
			default:
				line[i][2]=1;
			}
		}
		while(getchar()!='\n');
	}
}
#define wchars(bit,ctrl) if(outline[i]&bit)for(ctrl)putx();else for(ctrl)nsp++
writeline(){
	register i, j, k;
	int t;
	for(j=0;j<ys/2;j++){
		t=xs/2-j*xs/ys;
		if(t<0)
			t=0;
		for(i=1;i!=NCOLS;i++){
			wchars(UL, k=0;k<t;k++);
			wchars(CENTRE, ;k<xs-t;k++);
			wchars(UR, ;k<xs;k++);
		}
		nsp=0;
		putchar('\n');
	}
	for(;j<ys;j++){
		t=j*xs/ys-xs/2;
		if(t<0)
			t=0;
		for(i=1;i!=NCOLS;i++){
			wchars(LL, k=0;k<t;k++);
			wchars(CENTRE, ;k<xs-t;k++);
			wchars(LR, ;k<xs;k++);
		}
		nsp=0;
		putchar('\n');
	}
}
putx(){
	register i;
	i=nsp;
	if(i)
		do
			putchar(' ');
		while(--i);
	putchar('#');
	nsp=0;
}
#define NDIRS 4
char corner[NDIRS] = {
	UL,
	UR,
	LL,
	LR};
struct{
	int x0, y0, x1, y1, x2, y2;
}full[NDIRS] = {
	-1,0,0,0,-1,1,
	0,0,1,0,1,1,
	-1,1,-1,2,0,2,
	1,1,1,2,0,2
};
struct{
	int x0, y0, x1, y1;
}empty[NDIRS] = {
	-1,1,0,0,
	0,0,1,1,
	-1,1,0,2,
	0,2,1,1
};
fill(){
	register i, j;
	for(i=1;i!=NCOLS;i++)
		if(line[i][1]){
			outline[i]=CENTRE;
			for(j=0;j!=NDIRS;j++)
				if(line[full[j].x0+i][full[j].y0])
					outline[i]|=corner[j];
				else if(line[full[j].x1+i][full[j].y1])
					outline[i]|=corner[j];
				else if(line[full[j].x2+i][full[j].y2])
					outline[i]|=corner[j];
		}
		else{
			outline[i]=0;
			for(j=0;j!=NDIRS;j++)
				if(line[empty[j].x0+i][empty[j].y0]
				&& line[empty[j].x1+i][empty[j].y1])
					outline[i]|=corner[j];
		}
}
