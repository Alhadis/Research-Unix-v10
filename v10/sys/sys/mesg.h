struct imesg {
	char type;
	char msflag;
	short size;
	struct block *saveb;
};
