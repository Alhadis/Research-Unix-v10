#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"fns.h"

static Function fns[] = {
	{ "alternate", "alternate [lun]", "L?", sony_alt },
	{ "config", "config", "", sony_conf },
	{ "copy", "copy srclun start n destlun dest", "LIILI", sony_copy },
	{ "diskid", "diskid [lun]", "L?", sony_diskid },
	{ "eject", "eject lun", "L", sony_eject },
	{ "inq", "inq [lun]", "L?", sony_inq },
	{ "internal", "internal test [drive]  # internal -1 for list", "II?", sony_internal },
	{ "media", "media lun start count [file]", "LIIS?", sony_media },
	{ "readid", "readid lun [start]", "LI?", sony_readid },
	{ "rel", "rel lun [shelfside]", "LS?", sony_rel },
	{ "sense", "sense [lun=0]", "L?", sony_sense },
	{ "set", "set shelfside lun", "SL", sony_set },
	{ "status", "status", "", sony_status },
	{ 0 }
};

Device sonydev = {
	"sony", "Sony WDA-3000",
	sony_extsense,
	fns
};
