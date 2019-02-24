/*
 * one dkmod per datakit interface
 * also any other data common to datakit
 */

#include <sys/param.h>
#include <sys/dkmod.h>
#include <sys/dkstat.h>

#define	NDKMOD	4	/* big enough for everyone */

int dkmodcnt = NDKMOD;

struct dkmodule dkmod[NDKMOD];
struct dkstat dkstat;

/*
 * find the appropriate dkmod;
 * alloc a new one if need be
 * dev is the entire device
 */

struct dkmodule *
dkmodall(dev, lo, hi)
dev_t dev;
{
	register int i;
	register struct dkmodule *dk;
	register struct dkmodule *ek;

	dev = major(dev);
	ek = NULL;
	for (dk = dkmod, i = 0; i < NDKMOD; dk++, i++) {
		if (dk->dev == 0 && ek == NULL) {
			ek = dk;
			continue;
		}
		if (dk->dev == dev && dk->lo == lo && dk->hi == hi)
			return (dk);
	}
	if (ek) {
		ek->dev = dev;
		ek->lo = lo;
		ek->hi = hi;
	}
	return (ek);
}

struct dkmodule *
getdkmod(dev)
dev_t dev;
{
	register int i;
	register struct dkmodule *dk;
	register maj, min;

	maj = major(dev);
	min = minor(dev);
	for (dk = dkmod, i = 0; i < NDKMOD; dk++, i++)
		if (dk->dev == maj && min >= dk->lo && min < dk->hi)
			return (dk);
	return (NULL);
}
