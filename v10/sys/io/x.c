/*
 * dummy block device driver
 * throw this away;
 * it's here to make a partial system build
 */

#include "sys/param.h"
#include "sys/conf.h"

struct bdevsw xbdev = bdinit(nodev, nodev, nodev, 0);
