#ifndef PARSE_H
#define PARSE_H
#include "gram.h"
#include "y.tab.h"
#include "expr.pub"

class Parse{
	int	goal;
	Expr	*expr;
public:
	char	*error;
		Parse(int g, Expr *e=0)	{ goal = g; expr = e; }
	void	*parse(char *);
};
#endif
