#define MAGIC 0135246
#define BIG 32765

typedef struct Rd2d {
  int siz;       /* nx*ny */
  int fd;        /* file descriptor */
  double ts;     /* time of starting frame */
  double te;     /* time of ending frame */
  short u;       /* global u value */
  short v;       /* global v value */
  short fixuv;
  short pmin;    /* global min pixel value */
  short pmax;    /* global max pixel value */
  float fmin;    /* global min */
  float fmax;    /* global max */
  int nfr;       /* number of frames */
} Rd2d;

typedef struct Header {
  unsigned short magic1, magic2;
  short ver;
  short nx, ny;
  short u, v;
  short fixuv;
  short pmin, pmax;
  char time[16];  /* -9.9999999e+999 */
} Header;

typedef struct Header2 {     /* old style headers */
  unsigned short magic1, magic2;
  short ver;
  short nx, ny;
  short time;
  short u, v;
  short fixuv;
  short pmin, pmax;
  short pad;  /* just to fill out the structure to a word boundary */
} Header2;

