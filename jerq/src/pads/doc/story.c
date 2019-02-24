class Story : public PadRcv {
	char	file[16];
	FILE	*fp;
	long	lines;
	Pad	*pad;
	void	middle()        { linereq( lines/2, SELECTLINE ); }
	void	linereq(long,Attrib);
public:
	void	open();
		Story(char *f)  { strncpy( file, f, strlen(f)-1 ); }
};
