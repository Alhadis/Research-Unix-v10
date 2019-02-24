#include <picfile.h>
int PRD_dmp(), PWR_dmp(), PCL_dmp();
int PRD_run(), PWR_run(), PCL_run();
int PRD_bit(), PWR_bit(), PCL_bit();
int PRD_g31(), PWR_g31(), PCL_g31();
int PRD_g32(), PWR_g32(), PCL_g32();
int PRD_g4(),  PWR_g4(),  PCL_g4();
int PRD_601(), PWR_601(), PCL_601();
int PRD_pco(), PWR_pco(), PCL_pco();
struct PIC_conf PIC_conf[]={
	"dump",		PRD_dmp,	PWR_dmp,	PCL_dmp,	0,
	"runcode",	PRD_run,	PWR_run,	PCL_run,	0,
	"bitmap",	PRD_bit,	PWR_bit,	PCL_bit,	1,
	"pico",		PRD_pco,	PWR_pco,	PCL_pco,	0,
#ifdef NOTYET
	"ccitt-g31",	PRD_g31,	PWR_g31,	PCL_g31,	1,
	"ccitt-g32",	PRD_g32,	PWR_g32,	PCL_g32,	1,
	"ccitt-g4",	PRD_g4,		PWR_g4,		PCL_g4,		1,
	"ccir601",	PRD_601,	PWR_601,	PCL_601,	1,
#endif
	0
};
