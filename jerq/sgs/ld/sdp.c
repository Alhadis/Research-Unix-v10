static char ID[] = "@(#) sdp.c: 1.2 11/23/82";
#if !NOSDP
#include "system.h"

#include <stdio.h>
#include <fatal.h>
#include <signal.h>

#include "sdp1.h"
#include "sdpsrc/hd/define2.h"
#include "sdpsrc/hd/define3.h"
#include "sdpsrc/hd/define4.h"

#include "structs.h"
#include "extrns.h"
#include "paths.h"
#include "instr.h"

#include "sdpsrc/hd/struct.h"
#include "sdpsrc/hd/globals.h"

#include "sdpsrc/lo/delist.c"
#include "sdpsrc/lo/enlist.c"
#include "sdpsrc/lo/fatal.c"
#include "sdpsrc/lo/getnxtpage.c"
#include "sdpsrc/lo/readpage.c"
#include "sdpsrc/lo/sojourn.c"
#include "sdpsrc/lo/rpartition.c"
#include "sdpsrc/lo/writeframe.c"

#include "sdpsrc/hi/allocate.c"
#include "sdpsrc/hi/flush.c"
#include "sdpsrc/hi/init.c"
#include "sdpsrc/hi/lock.c"
#include "sdpsrc/hi/unlock.c"
#endif
