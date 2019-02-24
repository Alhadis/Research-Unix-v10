class Wd : public PadRcv {
	long	key;
	Pad	*pad;
	char	*getwd;
	char	*prevwd;
	Index	ix;
	Index	carte();
	void	pwd(Attrib=0);
PUBLIC(Wd,U_WD)
		Wd();
	char	*kbd(char *);
	char	*help();
};

