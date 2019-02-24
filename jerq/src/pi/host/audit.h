class Audit : public PadRcv {
	Pad	*pad;
		Audit();
	long	lastclock;
	long	period;
	void	clone();
	void	lookup();
	void	lazy();
	void	setperiod(long);
	void	mon(long);
PUBLIC(Audit,U_AUDIT)
	void	cycle();
};
