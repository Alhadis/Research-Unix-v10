/*
 *	gt - converts troff -g output to troff -t
 */

#define N 65
char obuf[512];
char ibuf[512];
char *ibufp ibuf;
char *eibufp ibuf;
char *obufp obuf;
int id;
int stop 100;
int first 1;
int pn 0;
int skip 0;
int xxx;

main(argc,argv)
int argc;
char **argv;
{
	register i, j;

	while((--argc > 0) && ((++argv)[0][0]=='-')){
		switch(argv[0][1]){
			case 's':
				stop = atoi(&argv[0][2]);
				continue;
			case 'k':
				skip = atoi(&argv[0][2]);
				skip++;
				continue;
			case 'o':
				getpn(&argv[0][2]);
				continue;
			default:
				continue;
		}
	}
	if(argc){
		if((id=open(argv[0],0)) < 0){
			printf("Cannot open: %s\n",argv[0]);
			exit(1);
		}
	}
	while(1){
		oput(i=igetc());
		if(i == 0100){ /*init*/
			if(pn++ > stop){
				pn = 1;
				trailer();
			}
			if(skip){
				skip--;
				if(!skip)oput(0100);
			}
			if((i=igetc()) != 0111){  /*stop*/
				oput(i);
				continue;
			}else{
				switch(i=igetc()){
					case 'f':
						for(j=8; j>0; j--)igetc();
						if(first && !skip){
							first = 0;
							oput(0140);
						}
						continue;
					case 0:
						trailer();
						ex();
					default:
						oput(i);
						continue;
				}
			}
		}
	}
}
trailer(){
	register j;

	for(j=N; j>0; j--)oput(0140);
	oput(0111);
	for(j=8; j>0; j--)oput(0101);
}
ex(){
	oflush();
	exit(0);
}
igetc(){
	register i;

	if(ibufp >= eibufp){
		if((i=read(id,ibuf,512)) <= 0)ex();
		eibufp = ibuf + i;
		ibufp = ibuf;
	}
	return((*ibufp++) & 0377);
}
oput(i)
char i;
{
	if(skip)return;
	*obufp++ = i;
	if(obufp >= &obuf[512])oflush();
}
oflush(){

	if(obufp > obuf)
		write(1,obuf,obufp-obuf);
	obufp = obuf;
}
getpn(){}
