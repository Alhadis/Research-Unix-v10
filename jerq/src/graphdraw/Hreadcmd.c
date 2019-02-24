#include <stdio.h>



readcmd() {
	int fileflag=0, badfile, chdir(), killflag;

	FILE *fp, *fopen();
	char cmd, fname[80], c, msg[80];

	int done=0, i, j, k, i1, i2, x1, y1, t;
			/* var's for anim op's */

	int nverts, nedges, edgewtsum; 
	char gtype[5];  /* graph type */
	int vertdeg, x, y;      /* ~ file format */
	char vertwt[30], vertcolor[5], edgewt[30], edgecolor[5];
	int indx;


	scanf( "%c", &cmd );  /* read command */

	while ((cmd != 'c') && (cmd != 'q') && (cmd != 'r') && (cmd != 'w'))
		scanf( "%c", &cmd );  /* read valid command */

		if ( (cmd == 'c') || (cmd == 'r') || (cmd == 'w') ) {
			/* read 'filename' */
			k = 0;
			while( (c = getchar()) == ' ' );
			fname[k++] = c;	
			while( (c = getchar()) != '\0' && (c != '\r') && (c != '\n'))
				fname[k++] = c;	
			fname[k] = '\0';
		}


	switch(cmd) {

		case 'q':
			killflag = 1;  /*kill process being animated*/
			break;

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
	}  /*switch(cmd)*/
}  /*readcmd()*/
