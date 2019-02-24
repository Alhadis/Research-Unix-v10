extern float reg_ra;
extern long reg_cp, reg_ia, reg_wa, reg_wb, reg_wc, reg_xr, reg_xl;

/* macros to fetch parameter registers */
#define RA(type) ((type) reg_ra)
#define CP(type) ((type) reg_cp)
#define IA(type) ((type) reg_ia)
#define WA(type) ((type) reg_wa)
#define WB(type) ((type) reg_wb)
#define WC(type) ((type) reg_wc)
#define XR(type) ((type) reg_xr)
#define XL(type) ((type) reg_xl)

/* macros to set parameter registers */
#define SET_RA(val) reg_ra = (float) (val)
#define SET_CP(val) reg_cp = (long) (val)
#define SET_IA(val) reg_ia = (long) (val)
#define SET_WA(val) reg_wa = (long) (val)
#define SET_WB(val) reg_wb = (long) (val)
#define SET_WC(val) reg_wc = (long) (val)
#define SET_XR(val) reg_xr = (long) (val)
#define SET_XL(val) reg_xl = (long) (val)

/* a temporary scblk for various routines */
struct {
	long type;
	long len;
	char s[100];
} tempstr;

/* pointer to output filename */
char *outptr;
