/*
 * For Versatec printer/plotter driver
 */

struct vp_softc {
	short	sc_state;
	short	sc_ubno;
	int	sc_count;
	int	sc_bufp;
	struct	buf *sc_bp;
	ubm_t	sc_ubinfo;
	struct vpdevice *sc_addr;
};
