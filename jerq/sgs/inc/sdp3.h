/*
 * static char ID_sdp3h[] = "@(#) sdp3.h: 1.1 1/7/82";
 */

#include "sdpsrc/hd/define3.h"
#include "sdpsrc/hd/define4.h"
#include "sdpsrc/hd/struct.h"
#include "sdpsrc/hd/globals.h"

#include "sdpsrc/lo/delist.c"
#include "sdpsrc/lo/enlist.c"

#ifndef lpw
#include "sdpsrc/lo/fatal.c"
#endif

#include "sdpsrc/lo/getnxtpage.c"
#include "sdpsrc/lo/readpage.c"
#include "sdpsrc/lo/sojourn.c"
#include "sdpsrc/lo/sp2or.c"
#include "sdpsrc/lo/writeframe.c"

#include "sdpsrc/hi/allocate.c"
#include "sdpsrc/hi/change.c"
#include "sdpsrc/hi/flush.c"
#include "sdpsrc/hi/init.c"
#include "sdpsrc/hi/lock.c"
#include "sdpsrc/hi/release.c"
#include "sdpsrc/hi/unlock.c"

