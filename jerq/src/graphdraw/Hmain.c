#include <stdio.h>
#include <sgtty.h>
#include "/usr/jerq/include/jioctl.h"
#include <sys/types.h>
int killflag;  /* global var to kill anim. process */

main(argc, argv) 
int argc; 
char *argv[]; {

	int fileflag=0, badfile, chdir(), select(), haltflag=0;
	fd_set r, w;  
	int nfds;
/*for select(); for 'interrupt' from terminal to close process being animated*/

	struct sgttyb otty, ntty;  /* oldtty, newtty */
	int fd = 1;   /* file descriptor */

	FILE *fp, *fopen(), *pp, *popen();
	char cmd, Acmd, Acmdch2, fname[80], c, msg[80];
				/* note: 'A' ==> Animation */

	int done=0, i, j, k, i1, i2, x1, y1, t;
			/* var's for anim op's */

	int nverts, nedges; 
	char gtype[5];  /* graph type */
	int vertdeg, x, y;      /* ~ file format */
	char vertwt[30], vertcolor[5], edgewt[30], edgecolor[5];
	int indx;



	ioctl( fd, TIOCGETP,  &otty );   /* read old tty mode */
	ntty = otty;    
	ntty.sg_flags = (ntty.sg_flags | CBREAK | RAW) & ~ECHO;   /* set new flags */
	ioctl( fd, TIOCSETP,  &ntty );
	ioctl( fd, TIOCEXCL,  0 );
	
	if ( argc > 1 ) { 	 /* 'graph' was called with filename */
		cmd = 'r';
		strcpy( fname, argv[1] );
		fileflag = 1;
	}
	
	while ( !done ) {

		if ( !fileflag ) {
			scanf( "%c", &cmd );  /* read command */

			while ((cmd != 'c') && (cmd != 'q') && (cmd != 'r') && (cmd != 'w') && (cmd != '!'))
				scanf( "%c", &cmd );  /* read valid command */

			if ( (cmd == 'c') || (cmd == 'r') || (cmd == 'w') || (cmd == '!')) {
				/* read 'filename' */
				k = 0;
				while( (c = getchar()) == ' ' );
				fname[k++] = c;	
				while( (c = getchar()) != '\0' && (c != '\r') && (c != '\n'))
					fname[k++] = c;	
				fname[k] = '\0';
			}
		}



		switch (cmd) {
		case 'c':
			if (chdir(fname) == -1) {
				printf("0\n");
				fflush( stdout );
				break;
			}
			else {
				printf("1\n");
				fflush( stdout );
			}
			break;

		case '!':
			if ((pp = popen(fname, "r")) == NULL) {
				printf("0\n");
				fflush( stdout );
				break;
			}
			else {

			printf("1\n");
			fflush( stdout );

			killflag = 0;
			nfds = max(0, fileno(pp)) + 1;
			while (!killflag) {
			/* if EOF, breaks out of this loop where fscanning for Acmd*/
				FD_ZERO(r);
				FD_ZERO(w);
				FD_SET(0,r);
				FD_SET(fileno(pp),r);
				select(nfds, &r, &w, 0);
			
				if (FD_ISSET(0, r)) {
				/*if ready to read from terminal*/
					scanf("%c", &c);
					switch(c) {
						case 'h':  /*halt*/
							while ((c != 'r') && (c != 'k')) /*resume*/
							/*wait for resume signal from terminal*/
								scanf("%c", &c);
							if (c == 'k') killflag = 1;
							break;    

						case 'k':  /*kill*/
							killflag = 1;	
							/*breaks out of while loop*/
							break;    

						case 'c':  /*command*/
							printf("2\n");
							/*tell blit ready for command*/
							scanf("%d", &i);
							if (i==1)
								readcmd();
							break;    
					}
/*don't forget about extra flushes for buffer --A. Hume's advice*/
				}
 


				if (FD_ISSET(fileno(pp), r)) {  
				/*if ready to read from process*/
					if (fscanf(pp, "%c", &Acmd) == EOF)
						break;  /*break out of while loop*/


				/* look for anim. cmds */
	                  if ((Acmd != 'v') && (Acmd != 'e') && (Acmd != 'p'))
				continue;
				 	/* read valid command */

			  switch( Acmd ) {
				case 'v':  /* vertex op */
					fscanf(pp, "%c", &Acmdch2);  /* from process */
					switch (Acmdch2) {
						case 'c':  /* create */
							if (fscanf(pp, "%d%d", &x1, &y1) != 2)
								break;
							printf("a %d %d\n", x1, y1);
								/* to terminal */
							fflush(stdout);
							break; /* case 'vc' */

						case 'd':  /* delete */
							if (fscanf(pp, "%d", &i1) != 1)
								break;
							printf("b %d\n", i1);
							fflush(stdout);
							break; /* case 'vd' */

						case 'm':  /* move */
							if (fscanf(pp, "%d%d%d", &i1, &x1, &y1) != 3)
								break;
							printf("c %d %d %d\n", i1, x1, y1);
							fflush(stdout);
							break;  /* case 'vm' */

						case 'p':  /* copy */
							if (fscanf(pp, "%d %d %d", &i1, &x1, &y1) != 3)
								break;
							printf("d %d %d %d\n", i1, x1, y1);
							fflush(stdout);
							break; /* case 'vp' */

						case 'g':  /* graphics */
							if (fscanf(pp, "%d%s", &i1, vertcolor) != 2)
								break;
							printf("e %d %s\n", i1, vertcolor);
							fflush(stdout);
							break; /* case 'vg' */

						case 'l':  /* label */
							if (fscanf(pp, "%d%s", &i1, vertwt) != 2)
								break;
							printf("f %d %s\n", i1, vertwt);
							fflush(stdout);
							break; /* case 'vl' */

						case 's':  /* show labels */
							printf("g\n");
							fflush(stdout);
							break; /* case 'vs' */

						case 'h':  /* hide labels */
							printf("h\n");
							fflush(stdout);
							break; /* case 'vh' */

						case 'n':  /* number labels */
							printf("i\n");
							fflush(stdout);
							break; /* case 'vn' */

					} /*switch Acmdch2 for case 'v'*/
					break;  /* case 'v' */

				case 'e':  /* edge op */
					fscanf(pp, "%c", &Acmdch2);  /* from process */
					switch (Acmdch2) {
						case 'c':
							if (fscanf(pp, "%d %d", &i1, &i2) != 2)
								break;
							printf("j %d %d\n", i1, i2);
							fflush(stdout);
							break; /* case 'ec' */

						case 'd':  /* delete */
							if (fscanf(pp, "%d%d", &i1, &i2) != 2)
								break;
							printf("k %d %d\n", i1, i2);
							fflush(stdout);
							break; /* case 'ed' */

						case 'g':  /* graphics */
							if (fscanf(pp, "%d%d%s", &i1,&i2, edgecolor) != 3)
								break;
							printf("l %d %d %s\n", i1, i2, edgecolor);
							fflush(stdout);
							break; /* case 'eg' */

						case 'l':  /* label */
							if (fscanf(pp, "%d%d%s", &i1,&i2, edgewt) != 3)
								break;
							printf("m %d %d %s\n", i1, i2, edgewt);
							fflush(stdout);
							break; /* case 'el' */

						case 's':  /* show labels */
							printf("n\n");
							fflush(stdout);
							break; /* case 'es' */

						case 'h':  /* hide labels */
							printf("o\n");
							fflush(stdout);
							break; /* case 'eh' */

						case 'e':  /* euclidean labels */
							printf("p\n");
							fflush(stdout);
							break; /* case 'ee' */

					}  /* switch Acmdch2 for case 'e'*/
					break;  /* case 'e' */


				case 'p':  /* program op */
					fscanf(pp, "%c", &Acmdch2);  /* from process */
					switch (Acmdch2) {
						case 'd':   /* program delay */
							if (fscanf(pp, "%d", &t) != 1)
								break;
							printf("q %d\n", t);
							fflush(stdout);
							break; /* case 'pd' */

						case 's':   /* program stop */
							printf("r\n");
							fflush(stdout);
							scanf("%c", &c);
							while ((c != 'r') && (c != 'k') && (c != 'c'))
								scanf("%c", &c);
							switch(c) {
							case 'r':  /*resume*/
								break;    

							case 'k':  /*kill*/
								killflag = 1;	
								/*breaks out of while loop*/
								break;    

							case 'c':  /*command*/
								scanf("%d", &i);
								if (i==1)
									readcmd();
								break;    
							} /*switch*/
 
							break; /* case 'ps' */

						case 'm':   /* program message */
							k = 0;
							while( (c = getc(pp)) == ' ' );
							msg[k++] = c;	
							while( (c = getc(pp)) != '\0' && (c != '\r') && (c != '\n'))
								msg[k++] = c;	
							msg[k] = '\0';
							printf("s %s\n", msg);
							fflush(stdout);
							break; /* case 'pm' */

						case 'r':   /* program restart */
							printf("t\n");
							fflush(stdout);
							break; /* case 'pr' */

						case 'c':   /* program command */
							printf("u\n");
							fflush(stdout);
							scanf("%d", &i);
							if (i==1)
								readcmd();
							break; /* case 'pc' */

						case 'w':   /* program window scale */
							if (fscanf(pp, "%d", &i1) != 1)
								break;
							printf("v %d\n",i1);
							fflush(stdout);
							break; /* case 'pw' */

				 	}  /* switch Acmdch2  for Acmd = 'p' */	
					break;  /* case 'p' */
					

				} /*switch Acmd*/
			   } /*if FD_ISSET(pp, r)...*/

			  }   /*while !EOF*/
		          pclose(pp);	
			  printf("z\n");  /* tell terminal done running program */
			}  /* else */

			break;   /* case '!'*/

		case 'q':
			done = 1;
			break;


		case 'w':
			if ( (fp = fopen( fname, "w" )) == NULL) {  /* open file */
				printf("0\n");  /* msg to blit: no file coming */
				fflush( stdout );
			}
                        else {
				printf("1\n"); /* msg to blit: here comes file */	
				fflush( stdout );
					/* rcv list from blit */
				scanf("%d%d%s", &nverts, &nedges, gtype);
				fprintf(fp, "%d %d %s\n", nverts, nedges, gtype ); 
			
				for ( i = 0; i < nverts; i++ ) {
					scanf("%d%s%d%d%s",&vertdeg, vertwt, &x, &y, vertcolor);
					fprintf(fp, "%d %s %d %d %s\n", vertdeg, vertwt, x, y, vertcolor);
					for ( j = 0; j < vertdeg; j++ ) {
						scanf("%d%s%s", &indx, edgewt, edgecolor );
						fprintf(fp, "%d %s %s\n", indx, edgewt, edgecolor);
					}
				}
				fclose( fp );
                        }
			break;



		case 'r':
			fileflag = 0;   /* flag is only for argc > 1 */

			if ( ( fp = fopen( fname, "r" ) ) == NULL )  {
				printf("0\n");
				fflush( stdout );
			}

			else {
				printf("1\n");	 /* good fname */
				fflush( stdout );

				/* check file */
				badfile = 0;
				if (fscanf(fp, "%d%d", &nverts, &nedges) != 2)
					badfile = 1;
				if (!badfile)
					while ( (c = getc(fp)) != '\n' ) ;
					/* get optional gtype */
				if (!badfile) {
					for (i=0; i<nverts; i++) {
						if (fscanf(fp, "%d%s%d%d", &vertdeg, vertwt, &x, &y) != 4) {
							badfile = 1;
							break;
						}
						while ( (c = getc(fp)) != '\n' ) ;
							/* get opt. vert color */
						for (j=0; j<vertdeg; j++) {
							if (fscanf(fp,"%d%s", &indx, edgewt) != 2) {
								badfile = 1;
								break;
							}
							while ( (c = getc(fp)) != '\n' && c != EOF) ;
							/* get opt edge color */
						}
					}	
				}
				if (!badfile)
					printf("%d\n", nverts);
				/*so blit can see if enough room for new file*/
				else 
					printf("0\n");
				fflush(stdout);
				fclose (fp);


				if (badfile)
					break;

				scanf("%d", &k);
				if (k==0) break;
			/*too many vertices, don't send file*/

				fp = fopen(fname, "r");
					/* send file to blit */	
				fscanf( fp, "%d%d", &nverts, &nedges );
				printf("%d %d ", nverts, nedges ); 

				fflush( stdout );
				
				/* gtype */
				k = 0;
				while( (c = getc(fp)) != '\n' )
					if (c != ' ' && c != '\t')
						gtype[k++] = c;	
				if ( k != 0 )
					printf("%s ", gtype);
				else
					printf("u ");

				fflush( stdout );
	
				for ( i = 0; i < nverts; i++ ) {
					fscanf( fp,"%d%s%d%d",&vertdeg, vertwt, &x, &y);
					printf( "%d %s %d %d ", vertdeg, vertwt, x, y);

					/* vertcolor */
					k = 0;
					while( (c = getc(fp)) != '\n' )
						if (c != ' ' && c != '\t')
							vertcolor[k++] = c;	
					if ( k != 0 )
						printf("%s ", vertcolor);
					else
						printf("L ");

					fflush( stdout );
	
					for ( j = 0; j < vertdeg; j++ ) {
						fscanf( fp, "%d%s", &indx, edgewt);
						printf( "%d %s ", indx, edgewt);

						/* edgecolor */
						k = 0;
						while( (c = getc(fp)) != '\n' && c != EOF)
							if (c != ' ' && c != '\t')
								edgecolor[k++] = c;	
						if ( k != 0 )
							printf("%s ", edgecolor);
						else
							printf("L ");

						fflush( stdout );
	
					}
				}

				fclose( fp );
			}
			break;

		} /*switch*/
	} /*while*/

	ioctl( fd, TIOCSETP, &otty );   /* resets terminal...*/
	ioctl( fd, JTERM, 0 );

} /*Hmain*/
