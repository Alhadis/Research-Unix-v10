/*
 * Metheus 2610 per device driver data, defined in conf
 */
extern struct om{
	int flag;
	ubm_t ubm;
	struct omreg *reg;
	struct buf buf;
}om[];
