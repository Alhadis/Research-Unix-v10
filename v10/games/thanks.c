#include <stdio.h>

char *line[500] = {
	"Thank you",
	"I'm greatly in your debt",
	"My gratitude overflows",
	"Words cannot express my thanks",
	"I am touched by your thoughtfulness",
	"Gracias",
	"Profoundest Gratitude",
	"Vielen Dank",
	"Merci",
	"wanna buy a plant?",
	"We lie under your obligation",
	"I will never forget your kind action",
	"Gramercy",
	"Much Obliged",
	"Wherever did you find it?",
	"I used to play with one of those in my highchair",
	"Didn't you save the receipt?",
	"Why, this is the one I gave you last year",
	"Why, this is just like the one you gave me last year",
	"Oh my, I could never sleep in something like that",
	"You must have spent hours sewing all those eyes on the ends of its tentacles",
	0 };

main()
{
	register char * l;
	long t;
	int n, i;

	time(&t);
	srand(getpid() + (int)((t>>16) + t));
	for( n=0; line[n]; ++n ) { ; }
	for(;;) {
		i = rand();
		i = (i/32768.)*n;
		if( i==n ) i = n-1;
		else if( i>n || i<0 ) fprintf( stderr, "bad rand: %d", i );
		printf( "%s!\n", line[i] );
		}
	}
