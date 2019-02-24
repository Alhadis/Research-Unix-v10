struct bnode : public common {
	bnode* l;
	bnode* r;
	common* p;
	bnode(common* pp)			{ l=r=0; p=pp; }
	bnode(common* pp, bnode* ll, bnode* rr)	{ l=ll; r=rr; p=pp; }
};

