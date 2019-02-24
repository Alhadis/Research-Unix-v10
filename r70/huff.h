/*
	Written by James Johnston, AT&T Bell Labs, 6/2/87.  
*/

typedef struct node {
	char * datump;
	struct node * to, * from, * ldad, * rdad, * kid;
	float prob;
} 
NODE;

extern NODE *huff();
