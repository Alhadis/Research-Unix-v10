#include <stdio.h>

/*
 * security - an authentication server
 *
 * For a given user, this locates that user's key in the
 * key database, challenges the user to encrypt some data,
 * and compares that user's result with the result of
 * encrypting the data with the user's real key.  If the
 * results are the same, the user is authenticated, otherwise,
 * no.
 */
main()
{
	int k[8], i, j;
	char s[30], c[30], b[64];
	unsigned l, r;

	puts("key: 8 groups of three octal digits");
	puts("data: a decimal integer, up to 8 digits");
	puts("");
	fputs("key: ", stdout);

	scanf("%o %o %o %o %o %o %o %o", &k[0], &k[1], &k[2], &k[3],
	   &k[4], &k[5], &k[6], &k[7]);
	gets(s);
	for(i=0; i<8; i++)
	   for(j=0; j<8; j++)
	      b[8*i+j] = (k[i]>>(7-j))&1;
	setkey(b);
	while(1){
	   fputs("data: ", stdout);
	   for(i=0; i<8; i++) s[i] = 0;
	   gets(s);
	   for(i=0; i<8; i++)
	      for(j=0; j<8; j++)
		 b[8*i+j] = (s[i]>>(7-j))&1;
	   encrypt(b, 0);
	   for(i=0; i<32; i++){
	      l = (l<<1) | b[i];
	      r = (r<<1) | b[i+32];
	   }
	   printf("%.8x  %.8x\n", l, r);
	}
}

