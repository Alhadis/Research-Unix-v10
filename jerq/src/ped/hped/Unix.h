#undef UNIX5
#ifdef UNIX5

#define LOAD_32X "/usr/DMD/bin/32ld"
#define LOAD_68X "/usr/jerq/bin/68ld"

#define PED_5620M "/usr/DMD/lib/ped.m"
#define PED_blitM "/usr/jerq/mbin/ped.m"

#define ZED_5620M "/usr/DMD/lib/zped.m"
#define ZED_blitM "/usr/jerq/mbin/zped.m"

#define DOC_LIB "/usr/DMD/lib/ped/DOC"

#define FNT_5620 "/usr/DMD/font/R"
#define TFNT_5620 "/usr/DMD/font/"
#define FNT_blit "/usr/jerq/font/B"
#define TFNT_blit "/usr/jerq/font/"

#define PRIV_PED "/ustg/hutch/jped/ped.m"
#define PRIV_LIB "/ustg/hutch/dped/DOC"

#else
#define LOAD_32X "/usr/jerq/bin/32ld"
#define LOAD_68X "/usr/blit/bin/68ld"

#define PED_5620M "/usr/jerq/mbin/ped.m"
#define PED_blitM "/usr/blit/mbin/ped.m"

#define ZED_5620M "/usr/jerq/mbin/zped.m"
#define ZED_blitM "/usr/blit/mbin/zped.m"

#define DOC_LIB "/usr/jerq/lib/ped/DOC"

#define FNT_5620 "/usr/jerq/font/B"
#define TFNT_5620 "/usr/jerq/font/"
#define FNT_blit "/usr/blit/font/B"
#define TFNT_blit "/usr/blit/font/"

#define PRIV_PED "/usr/theo/dped/ped.m"
#define PRIV_LIB "/usr/theo/yped/DOC"
#endif
