 
#define CAMREAD		1
#define	CAMWRITE	2
#define	CAMSET		3
#define CSRREAD		4
#define LAMHIREAD	5
#define LAMLOREAD	6
#define VCTREAD		7
#define CSRWRITE	8

struct module_info {
	char		F;
	unsigned 	N;
	unsigned 	A;
	long		data;
	unsigned short	Q;
};

struct reg_info {
	unsigned	reg;
};
