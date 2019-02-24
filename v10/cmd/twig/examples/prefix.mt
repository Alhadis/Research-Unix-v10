prologue {
#include <stdio.h>
#include <ctype.h>
typedef	struct node *	NODEPTR;
#define	COST		int
#define	INFINITY	1000000
#define DEFAULT_COST	0
#define COSTLESS(x,y)	((x)<(y))

NODEPTR Root;
struct node {
	char op;		/* '\0' if node is a leaf */
	NODEPTR left, right;
	char id;
	int mark;
};

}; /* end of prologue */

node	nOp nIdent;
label	lExpr;

lExpr:	nIdent
	= { putchar($$->id); };

lExpr:	nOp(lExpr,lExpr)
	{ TOPDOWN; }
	= {
		putchar($$->op);
		tDO($%1$); tDO($%2$);
	};

insert {

mtValue(root)
	NODEPTR root;
{
	if(root==NULL)
		return(0);
	if(root->op==0)
		return(nIdent);
	else
		return(nOp);
}

NODEPTR
mtGetNodes(r,n)
	NODEPTR r;
	int n;
{
	if(r==NULL)
		if(n==1)
			return(Root);
		else
			return(NULL);
	if(n==1)
		return(r->left);
	else if(n==2)
		return(r->right);
	else return(NULL);
}

mtSetNodes(r,n,c)
	NODEPTR r,c;
	int n;
{
	if(r==NULL && n==1) {
		Root = c;
		return;
	}
	if(n==1)
		r->left = c;
	else if(n==2)
		r->right = c;
}

syntax_error()
{
	printf("syntax error\n");
	exit(1);
}

/* allocated a node */
NODEPTR
getnode(op,id)
	char op,id;
{
	NODEPTR n;
	n = (NODEPTR) malloc(sizeof(struct node));
	n->op = op;
	n->left = n->right = NULL;
	n->id = id;
	return(n);
}

NODEPTR
gettree()
{
	char c;
	NODEPTR n = NULL;

	c = getchar();
	if(c==EOF) 
		syntax_error();

	n = getnode(0,0);

	/* build node for left operand */
	if(c=='(')
		n->left = gettree();
	else
		n->left = getnode(0,c);

	c = getchar();
	if(strchr("+-*/",c)==NULL)
		syntax_error();
	n->op = c;

	c = getchar();
	/* build node for right operand */
	if(c=='(')
		n->right = gettree();
	else
		n->right = getnode(0,c);

	c = getchar();
	if(c!=')') 
		syntax_error();

	return(n);
}

main()
{
	char c;
	_matchinit();		/* initialize the matcher */

	/* get the initial ( */
	c = getchar();
	if(c!='(')
		syntax_error();

	Root = gettree();
	_match();
	putchar('\n');
}
printTree(x)
{}


};	/* of insert */
