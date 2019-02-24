	Filename = "c318.h";
	#define CAT(a, b) NXCAT(a, b)
	#define NXCAT(a, b) a ## b
	stequals(__LINE__, STR(CAT(1., E9)), "1.E9");
	stequals(__LINE__, STR(CAT(1.u, 1E+)), "1.u1E+");
#define E 2
	checkthat(__LINE__, 1E1 == 10.);
	Filename = "c31.c";
