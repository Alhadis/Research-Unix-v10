/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of the Suite corresponds to the official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#define ANSI 1  /* This file follows ANSI arithmetic rules, which may not work in non-ANSI modes */
#include "types.h"
int main()
	{
	extern char *Filename;
	auto CHAR Jc[12][12];
	int x_0 = 0;
	int y_0 = 0;
#if ANSI
	auto SCHAR Jsc[12][12];
	int x_1 = 1;
	int y_1 = 1;
#endif
	auto SHORT Js[12][12];
	int x_2 = 2;
	int y_2 = 2;
	auto INT Ji[12][12];
	int x_3 = 3;
	int y_3 = 3;
	auto UCHAR Juc[12][12];
	int x_4 = 4;
	int y_4 = 4;
	auto USHORT Jus[12][12];
	int x_5 = 5;
	int y_5 = 5;
	auto UINT Jui[12][12];
	int x_6 = 6;
	int y_6 = 6;
	auto LONG Jl[12][12];
	int x_7 = 7;
	int y_7 = 7;
	auto ULONG Jul[12][12];
	int x_8 = 8;
	int y_8 = 8;
	auto FLOAT Jf[12][12];
	int x_9 = 9;
	int y_9 = 9;
	auto DOUBLE Jd[12][12];
	int x_10 = 10;
	int y_10 = 10;
#if ANSI
	auto LDOUBLE Jld[12][12];
	int x_11 = 11;
	int y_11 = 11;
#endif
	Jc[x_0][y_0] = 7;
#if ANSI
	Jsc[x_1][y_1] = 8;
#endif
	Js[x_2][y_2] = 9;
	Ji[x_3][y_3] = 10;
	Juc[x_4][y_4] = 11;
	Jus[x_5][y_5] = 12;
	Jui[x_6][y_6] = 13;
	Jl[x_7][y_7] = 14;
	Jul[x_8][y_8] = 15;
	Jf[x_9][y_9] = 16;
	Jd[x_10][y_10] = 17;
#if ANSI
	Jld[x_11][y_11] = 18;
#endif
	{
	auto struct {
		UINT b1: 1;
		UINT b2: 2;
		UINT b3: 3;
		UINT b4: 4;
		UINT b5: 5;
		UINT b6: 6;
		UINT b7: 7;
		UINT b8: 5;
		UINT b9: 4;
		UINT b10: 3;
		UINT b11: 2;
		UINT b12: 1;
		} K;
	K.b1 = 1;
	K.b2 = 2;
	K.b3 = 3;
	K.b4 = 4;
	K.b5 = 5;
	K.b6 = 6;
	K.b7 = 7;
	K.b8 = 5;
	K.b9 = 4;
	K.b10 = 3;
	K.b11 = 2;
	K.b12 = 1;
	{
	int true = 1, false = 0;
	Filename =  " auto array2 auto bits quest ";
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b1, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b1, 1); 
	iequals(__LINE__, true?K.b1:Jc[x_0][y_0], 1); 
	iequals(__LINE__, false?K.b1:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b2, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b2, 2); 
	iequals(__LINE__, true?K.b2:Jc[x_0][y_0], 2); 
	iequals(__LINE__, false?K.b2:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b3, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b3, 3); 
	iequals(__LINE__, true?K.b3:Jc[x_0][y_0], 3); 
	iequals(__LINE__, false?K.b3:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b4, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b4, 4); 
	iequals(__LINE__, true?K.b4:Jc[x_0][y_0], 4); 
	iequals(__LINE__, false?K.b4:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b5, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b5, 5); 
	iequals(__LINE__, true?K.b5:Jc[x_0][y_0], 5); 
	iequals(__LINE__, false?K.b5:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b6, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b6, 6); 
	iequals(__LINE__, true?K.b6:Jc[x_0][y_0], 6); 
	iequals(__LINE__, false?K.b6:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b7, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b7, 7); 
	iequals(__LINE__, true?K.b7:Jc[x_0][y_0], 7); 
	iequals(__LINE__, false?K.b7:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b8, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b8, 5); 
	iequals(__LINE__, true?K.b8:Jc[x_0][y_0], 5); 
	iequals(__LINE__, false?K.b8:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b9, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b9, 4); 
	iequals(__LINE__, true?K.b9:Jc[x_0][y_0], 4); 
	iequals(__LINE__, false?K.b9:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b10, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b10, 3); 
	iequals(__LINE__, true?K.b10:Jc[x_0][y_0], 3); 
	iequals(__LINE__, false?K.b10:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b11, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b11, 2); 
	iequals(__LINE__, true?K.b11:Jc[x_0][y_0], 2); 
	iequals(__LINE__, false?K.b11:Jc[x_0][y_0], 7); 
	iequals(__LINE__, true?Jc[x_0][y_0]:K.b12, 7); 
	iequals(__LINE__, false?Jc[x_0][y_0]:K.b12, 1); 
	iequals(__LINE__, true?K.b12:Jc[x_0][y_0], 1); 
	iequals(__LINE__, false?K.b12:Jc[x_0][y_0], 7); 
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b1, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b1, 1); 
	iequals(__LINE__, true?K.b1:Jsc[x_1][y_1], 1); 
	iequals(__LINE__, false?K.b1:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b2, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b2, 2); 
	iequals(__LINE__, true?K.b2:Jsc[x_1][y_1], 2); 
	iequals(__LINE__, false?K.b2:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b3, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b3, 3); 
	iequals(__LINE__, true?K.b3:Jsc[x_1][y_1], 3); 
	iequals(__LINE__, false?K.b3:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b4, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b4, 4); 
	iequals(__LINE__, true?K.b4:Jsc[x_1][y_1], 4); 
	iequals(__LINE__, false?K.b4:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b5, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b5, 5); 
	iequals(__LINE__, true?K.b5:Jsc[x_1][y_1], 5); 
	iequals(__LINE__, false?K.b5:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b6, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b6, 6); 
	iequals(__LINE__, true?K.b6:Jsc[x_1][y_1], 6); 
	iequals(__LINE__, false?K.b6:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b7, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b7, 7); 
	iequals(__LINE__, true?K.b7:Jsc[x_1][y_1], 7); 
	iequals(__LINE__, false?K.b7:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b8, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b8, 5); 
	iequals(__LINE__, true?K.b8:Jsc[x_1][y_1], 5); 
	iequals(__LINE__, false?K.b8:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b9, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b9, 4); 
	iequals(__LINE__, true?K.b9:Jsc[x_1][y_1], 4); 
	iequals(__LINE__, false?K.b9:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b10, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b10, 3); 
	iequals(__LINE__, true?K.b10:Jsc[x_1][y_1], 3); 
	iequals(__LINE__, false?K.b10:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b11, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b11, 2); 
	iequals(__LINE__, true?K.b11:Jsc[x_1][y_1], 2); 
	iequals(__LINE__, false?K.b11:Jsc[x_1][y_1], 8); 
#endif
#if ANSI
	iequals(__LINE__, true?Jsc[x_1][y_1]:K.b12, 8); 
	iequals(__LINE__, false?Jsc[x_1][y_1]:K.b12, 1); 
	iequals(__LINE__, true?K.b12:Jsc[x_1][y_1], 1); 
	iequals(__LINE__, false?K.b12:Jsc[x_1][y_1], 8); 
#endif
	iequals(__LINE__, true?Js[x_2][y_2]:K.b1, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b1, 1); 
	iequals(__LINE__, true?K.b1:Js[x_2][y_2], 1); 
	iequals(__LINE__, false?K.b1:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b2, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b2, 2); 
	iequals(__LINE__, true?K.b2:Js[x_2][y_2], 2); 
	iequals(__LINE__, false?K.b2:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b3, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b3, 3); 
	iequals(__LINE__, true?K.b3:Js[x_2][y_2], 3); 
	iequals(__LINE__, false?K.b3:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b4, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b4, 4); 
	iequals(__LINE__, true?K.b4:Js[x_2][y_2], 4); 
	iequals(__LINE__, false?K.b4:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b5, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b5, 5); 
	iequals(__LINE__, true?K.b5:Js[x_2][y_2], 5); 
	iequals(__LINE__, false?K.b5:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b6, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b6, 6); 
	iequals(__LINE__, true?K.b6:Js[x_2][y_2], 6); 
	iequals(__LINE__, false?K.b6:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b7, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b7, 7); 
	iequals(__LINE__, true?K.b7:Js[x_2][y_2], 7); 
	iequals(__LINE__, false?K.b7:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b8, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b8, 5); 
	iequals(__LINE__, true?K.b8:Js[x_2][y_2], 5); 
	iequals(__LINE__, false?K.b8:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b9, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b9, 4); 
	iequals(__LINE__, true?K.b9:Js[x_2][y_2], 4); 
	iequals(__LINE__, false?K.b9:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b10, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b10, 3); 
	iequals(__LINE__, true?K.b10:Js[x_2][y_2], 3); 
	iequals(__LINE__, false?K.b10:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b11, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b11, 2); 
	iequals(__LINE__, true?K.b11:Js[x_2][y_2], 2); 
	iequals(__LINE__, false?K.b11:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Js[x_2][y_2]:K.b12, 9); 
	iequals(__LINE__, false?Js[x_2][y_2]:K.b12, 1); 
	iequals(__LINE__, true?K.b12:Js[x_2][y_2], 1); 
	iequals(__LINE__, false?K.b12:Js[x_2][y_2], 9); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b1, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b1, 1); 
	iequals(__LINE__, true?K.b1:Ji[x_3][y_3], 1); 
	iequals(__LINE__, false?K.b1:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b2, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b2, 2); 
	iequals(__LINE__, true?K.b2:Ji[x_3][y_3], 2); 
	iequals(__LINE__, false?K.b2:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b3, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b3, 3); 
	iequals(__LINE__, true?K.b3:Ji[x_3][y_3], 3); 
	iequals(__LINE__, false?K.b3:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b4, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b4, 4); 
	iequals(__LINE__, true?K.b4:Ji[x_3][y_3], 4); 
	iequals(__LINE__, false?K.b4:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b5, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b5, 5); 
	iequals(__LINE__, true?K.b5:Ji[x_3][y_3], 5); 
	iequals(__LINE__, false?K.b5:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b6, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b6, 6); 
	iequals(__LINE__, true?K.b6:Ji[x_3][y_3], 6); 
	iequals(__LINE__, false?K.b6:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b7, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b7, 7); 
	iequals(__LINE__, true?K.b7:Ji[x_3][y_3], 7); 
	iequals(__LINE__, false?K.b7:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b8, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b8, 5); 
	iequals(__LINE__, true?K.b8:Ji[x_3][y_3], 5); 
	iequals(__LINE__, false?K.b8:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b9, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b9, 4); 
	iequals(__LINE__, true?K.b9:Ji[x_3][y_3], 4); 
	iequals(__LINE__, false?K.b9:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b10, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b10, 3); 
	iequals(__LINE__, true?K.b10:Ji[x_3][y_3], 3); 
	iequals(__LINE__, false?K.b10:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b11, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b11, 2); 
	iequals(__LINE__, true?K.b11:Ji[x_3][y_3], 2); 
	iequals(__LINE__, false?K.b11:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Ji[x_3][y_3]:K.b12, 10); 
	iequals(__LINE__, false?Ji[x_3][y_3]:K.b12, 1); 
	iequals(__LINE__, true?K.b12:Ji[x_3][y_3], 1); 
	iequals(__LINE__, false?K.b12:Ji[x_3][y_3], 10); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b1, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b1, 1); 
	iequals(__LINE__, true?K.b1:Juc[x_4][y_4], 1); 
	iequals(__LINE__, false?K.b1:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b2, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b2, 2); 
	iequals(__LINE__, true?K.b2:Juc[x_4][y_4], 2); 
	iequals(__LINE__, false?K.b2:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b3, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b3, 3); 
	iequals(__LINE__, true?K.b3:Juc[x_4][y_4], 3); 
	iequals(__LINE__, false?K.b3:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b4, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b4, 4); 
	iequals(__LINE__, true?K.b4:Juc[x_4][y_4], 4); 
	iequals(__LINE__, false?K.b4:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b5, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b5, 5); 
	iequals(__LINE__, true?K.b5:Juc[x_4][y_4], 5); 
	iequals(__LINE__, false?K.b5:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b6, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b6, 6); 
	iequals(__LINE__, true?K.b6:Juc[x_4][y_4], 6); 
	iequals(__LINE__, false?K.b6:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b7, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b7, 7); 
	iequals(__LINE__, true?K.b7:Juc[x_4][y_4], 7); 
	iequals(__LINE__, false?K.b7:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b8, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b8, 5); 
	iequals(__LINE__, true?K.b8:Juc[x_4][y_4], 5); 
	iequals(__LINE__, false?K.b8:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b9, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b9, 4); 
	iequals(__LINE__, true?K.b9:Juc[x_4][y_4], 4); 
	iequals(__LINE__, false?K.b9:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b10, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b10, 3); 
	iequals(__LINE__, true?K.b10:Juc[x_4][y_4], 3); 
	iequals(__LINE__, false?K.b10:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b11, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b11, 2); 
	iequals(__LINE__, true?K.b11:Juc[x_4][y_4], 2); 
	iequals(__LINE__, false?K.b11:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Juc[x_4][y_4]:K.b12, 11); 
	iequals(__LINE__, false?Juc[x_4][y_4]:K.b12, 1); 
	iequals(__LINE__, true?K.b12:Juc[x_4][y_4], 1); 
	iequals(__LINE__, false?K.b12:Juc[x_4][y_4], 11); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b1, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b1, 1); 
	iequals(__LINE__, true?K.b1:Jus[x_5][y_5], 1); 
	iequals(__LINE__, false?K.b1:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b2, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b2, 2); 
	iequals(__LINE__, true?K.b2:Jus[x_5][y_5], 2); 
	iequals(__LINE__, false?K.b2:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b3, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b3, 3); 
	iequals(__LINE__, true?K.b3:Jus[x_5][y_5], 3); 
	iequals(__LINE__, false?K.b3:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b4, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b4, 4); 
	iequals(__LINE__, true?K.b4:Jus[x_5][y_5], 4); 
	iequals(__LINE__, false?K.b4:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b5, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b5, 5); 
	iequals(__LINE__, true?K.b5:Jus[x_5][y_5], 5); 
	iequals(__LINE__, false?K.b5:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b6, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b6, 6); 
	iequals(__LINE__, true?K.b6:Jus[x_5][y_5], 6); 
	iequals(__LINE__, false?K.b6:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b7, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b7, 7); 
	iequals(__LINE__, true?K.b7:Jus[x_5][y_5], 7); 
	iequals(__LINE__, false?K.b7:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b8, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b8, 5); 
	iequals(__LINE__, true?K.b8:Jus[x_5][y_5], 5); 
	iequals(__LINE__, false?K.b8:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b9, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b9, 4); 
	iequals(__LINE__, true?K.b9:Jus[x_5][y_5], 4); 
	iequals(__LINE__, false?K.b9:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b10, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b10, 3); 
	iequals(__LINE__, true?K.b10:Jus[x_5][y_5], 3); 
	iequals(__LINE__, false?K.b10:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b11, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b11, 2); 
	iequals(__LINE__, true?K.b11:Jus[x_5][y_5], 2); 
	iequals(__LINE__, false?K.b11:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jus[x_5][y_5]:K.b12, 12); 
	iequals(__LINE__, false?Jus[x_5][y_5]:K.b12, 1); 
	iequals(__LINE__, true?K.b12:Jus[x_5][y_5], 1); 
	iequals(__LINE__, false?K.b12:Jus[x_5][y_5], 12); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b1, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b1, 1); 
	iequals(__LINE__, true?K.b1:Jui[x_6][y_6], 1); 
	iequals(__LINE__, false?K.b1:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b2, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b2, 2); 
	iequals(__LINE__, true?K.b2:Jui[x_6][y_6], 2); 
	iequals(__LINE__, false?K.b2:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b3, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b3, 3); 
	iequals(__LINE__, true?K.b3:Jui[x_6][y_6], 3); 
	iequals(__LINE__, false?K.b3:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b4, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b4, 4); 
	iequals(__LINE__, true?K.b4:Jui[x_6][y_6], 4); 
	iequals(__LINE__, false?K.b4:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b5, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b5, 5); 
	iequals(__LINE__, true?K.b5:Jui[x_6][y_6], 5); 
	iequals(__LINE__, false?K.b5:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b6, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b6, 6); 
	iequals(__LINE__, true?K.b6:Jui[x_6][y_6], 6); 
	iequals(__LINE__, false?K.b6:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b7, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b7, 7); 
	iequals(__LINE__, true?K.b7:Jui[x_6][y_6], 7); 
	iequals(__LINE__, false?K.b7:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b8, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b8, 5); 
	iequals(__LINE__, true?K.b8:Jui[x_6][y_6], 5); 
	iequals(__LINE__, false?K.b8:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b9, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b9, 4); 
	iequals(__LINE__, true?K.b9:Jui[x_6][y_6], 4); 
	iequals(__LINE__, false?K.b9:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b10, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b10, 3); 
	iequals(__LINE__, true?K.b10:Jui[x_6][y_6], 3); 
	iequals(__LINE__, false?K.b10:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b11, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b11, 2); 
	iequals(__LINE__, true?K.b11:Jui[x_6][y_6], 2); 
	iequals(__LINE__, false?K.b11:Jui[x_6][y_6], 13); 
	iequals(__LINE__, true?Jui[x_6][y_6]:K.b12, 13); 
	iequals(__LINE__, false?Jui[x_6][y_6]:K.b12, 1); 
	iequals(__LINE__, true?K.b12:Jui[x_6][y_6], 1); 
	iequals(__LINE__, false?K.b12:Jui[x_6][y_6], 13); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b1, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b1, 1L); 
	lequals(__LINE__, true?K.b1:Jl[x_7][y_7], 1L); 
	lequals(__LINE__, false?K.b1:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b2, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b2, 2L); 
	lequals(__LINE__, true?K.b2:Jl[x_7][y_7], 2L); 
	lequals(__LINE__, false?K.b2:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b3, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b3, 3L); 
	lequals(__LINE__, true?K.b3:Jl[x_7][y_7], 3L); 
	lequals(__LINE__, false?K.b3:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b4, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b4, 4L); 
	lequals(__LINE__, true?K.b4:Jl[x_7][y_7], 4L); 
	lequals(__LINE__, false?K.b4:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b5, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b5, 5L); 
	lequals(__LINE__, true?K.b5:Jl[x_7][y_7], 5L); 
	lequals(__LINE__, false?K.b5:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b6, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b6, 6L); 
	lequals(__LINE__, true?K.b6:Jl[x_7][y_7], 6L); 
	lequals(__LINE__, false?K.b6:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b7, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b7, 7L); 
	lequals(__LINE__, true?K.b7:Jl[x_7][y_7], 7L); 
	lequals(__LINE__, false?K.b7:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b8, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b8, 5L); 
	lequals(__LINE__, true?K.b8:Jl[x_7][y_7], 5L); 
	lequals(__LINE__, false?K.b8:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b9, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b9, 4L); 
	lequals(__LINE__, true?K.b9:Jl[x_7][y_7], 4L); 
	lequals(__LINE__, false?K.b9:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b10, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b10, 3L); 
	lequals(__LINE__, true?K.b10:Jl[x_7][y_7], 3L); 
	lequals(__LINE__, false?K.b10:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b11, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b11, 2L); 
	lequals(__LINE__, true?K.b11:Jl[x_7][y_7], 2L); 
	lequals(__LINE__, false?K.b11:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jl[x_7][y_7]:K.b12, 14L); 
	lequals(__LINE__, false?Jl[x_7][y_7]:K.b12, 1L); 
	lequals(__LINE__, true?K.b12:Jl[x_7][y_7], 1L); 
	lequals(__LINE__, false?K.b12:Jl[x_7][y_7], 14L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b1, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b1, 1L); 
	lequals(__LINE__, true?K.b1:Jul[x_8][y_8], 1L); 
	lequals(__LINE__, false?K.b1:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b2, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b2, 2L); 
	lequals(__LINE__, true?K.b2:Jul[x_8][y_8], 2L); 
	lequals(__LINE__, false?K.b2:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b3, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b3, 3L); 
	lequals(__LINE__, true?K.b3:Jul[x_8][y_8], 3L); 
	lequals(__LINE__, false?K.b3:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b4, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b4, 4L); 
	lequals(__LINE__, true?K.b4:Jul[x_8][y_8], 4L); 
	lequals(__LINE__, false?K.b4:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b5, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b5, 5L); 
	lequals(__LINE__, true?K.b5:Jul[x_8][y_8], 5L); 
	lequals(__LINE__, false?K.b5:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b6, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b6, 6L); 
	lequals(__LINE__, true?K.b6:Jul[x_8][y_8], 6L); 
	lequals(__LINE__, false?K.b6:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b7, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b7, 7L); 
	lequals(__LINE__, true?K.b7:Jul[x_8][y_8], 7L); 
	lequals(__LINE__, false?K.b7:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b8, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b8, 5L); 
	lequals(__LINE__, true?K.b8:Jul[x_8][y_8], 5L); 
	lequals(__LINE__, false?K.b8:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b9, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b9, 4L); 
	lequals(__LINE__, true?K.b9:Jul[x_8][y_8], 4L); 
	lequals(__LINE__, false?K.b9:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b10, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b10, 3L); 
	lequals(__LINE__, true?K.b10:Jul[x_8][y_8], 3L); 
	lequals(__LINE__, false?K.b10:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b11, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b11, 2L); 
	lequals(__LINE__, true?K.b11:Jul[x_8][y_8], 2L); 
	lequals(__LINE__, false?K.b11:Jul[x_8][y_8], 15L); 
	lequals(__LINE__, true?Jul[x_8][y_8]:K.b12, 15L); 
	lequals(__LINE__, false?Jul[x_8][y_8]:K.b12, 1L); 
	lequals(__LINE__, true?K.b12:Jul[x_8][y_8], 1L); 
	lequals(__LINE__, false?K.b12:Jul[x_8][y_8], 15L); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b1, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b1, 1.); 
	dequals(__LINE__, true?K.b1:Jf[x_9][y_9], 1.); 
	dequals(__LINE__, false?K.b1:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b2, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b2, 2.); 
	dequals(__LINE__, true?K.b2:Jf[x_9][y_9], 2.); 
	dequals(__LINE__, false?K.b2:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b3, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b3, 3.); 
	dequals(__LINE__, true?K.b3:Jf[x_9][y_9], 3.); 
	dequals(__LINE__, false?K.b3:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b4, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b4, 4.); 
	dequals(__LINE__, true?K.b4:Jf[x_9][y_9], 4.); 
	dequals(__LINE__, false?K.b4:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b5, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b5, 5.); 
	dequals(__LINE__, true?K.b5:Jf[x_9][y_9], 5.); 
	dequals(__LINE__, false?K.b5:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b6, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b6, 6.); 
	dequals(__LINE__, true?K.b6:Jf[x_9][y_9], 6.); 
	dequals(__LINE__, false?K.b6:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b7, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b7, 7.); 
	dequals(__LINE__, true?K.b7:Jf[x_9][y_9], 7.); 
	dequals(__LINE__, false?K.b7:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b8, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b8, 5.); 
	dequals(__LINE__, true?K.b8:Jf[x_9][y_9], 5.); 
	dequals(__LINE__, false?K.b8:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b9, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b9, 4.); 
	dequals(__LINE__, true?K.b9:Jf[x_9][y_9], 4.); 
	dequals(__LINE__, false?K.b9:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b10, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b10, 3.); 
	dequals(__LINE__, true?K.b10:Jf[x_9][y_9], 3.); 
	dequals(__LINE__, false?K.b10:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b11, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b11, 2.); 
	dequals(__LINE__, true?K.b11:Jf[x_9][y_9], 2.); 
	dequals(__LINE__, false?K.b11:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jf[x_9][y_9]:K.b12, 16.); 
	dequals(__LINE__, false?Jf[x_9][y_9]:K.b12, 1.); 
	dequals(__LINE__, true?K.b12:Jf[x_9][y_9], 1.); 
	dequals(__LINE__, false?K.b12:Jf[x_9][y_9], 16.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b1, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b1, 1.); 
	dequals(__LINE__, true?K.b1:Jd[x_10][y_10], 1.); 
	dequals(__LINE__, false?K.b1:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b2, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b2, 2.); 
	dequals(__LINE__, true?K.b2:Jd[x_10][y_10], 2.); 
	dequals(__LINE__, false?K.b2:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b3, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b3, 3.); 
	dequals(__LINE__, true?K.b3:Jd[x_10][y_10], 3.); 
	dequals(__LINE__, false?K.b3:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b4, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b4, 4.); 
	dequals(__LINE__, true?K.b4:Jd[x_10][y_10], 4.); 
	dequals(__LINE__, false?K.b4:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b5, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b5, 5.); 
	dequals(__LINE__, true?K.b5:Jd[x_10][y_10], 5.); 
	dequals(__LINE__, false?K.b5:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b6, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b6, 6.); 
	dequals(__LINE__, true?K.b6:Jd[x_10][y_10], 6.); 
	dequals(__LINE__, false?K.b6:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b7, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b7, 7.); 
	dequals(__LINE__, true?K.b7:Jd[x_10][y_10], 7.); 
	dequals(__LINE__, false?K.b7:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b8, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b8, 5.); 
	dequals(__LINE__, true?K.b8:Jd[x_10][y_10], 5.); 
	dequals(__LINE__, false?K.b8:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b9, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b9, 4.); 
	dequals(__LINE__, true?K.b9:Jd[x_10][y_10], 4.); 
	dequals(__LINE__, false?K.b9:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b10, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b10, 3.); 
	dequals(__LINE__, true?K.b10:Jd[x_10][y_10], 3.); 
	dequals(__LINE__, false?K.b10:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b11, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b11, 2.); 
	dequals(__LINE__, true?K.b11:Jd[x_10][y_10], 2.); 
	dequals(__LINE__, false?K.b11:Jd[x_10][y_10], 17.); 
	dequals(__LINE__, true?Jd[x_10][y_10]:K.b12, 17.); 
	dequals(__LINE__, false?Jd[x_10][y_10]:K.b12, 1.); 
	dequals(__LINE__, true?K.b12:Jd[x_10][y_10], 1.); 
	dequals(__LINE__, false?K.b12:Jd[x_10][y_10], 17.); 
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b1, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b1, 1.L); 
	ldequals(__LINE__, true?K.b1:Jld[x_11][y_11], 1.L); 
	ldequals(__LINE__, false?K.b1:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b2, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b2, 2.L); 
	ldequals(__LINE__, true?K.b2:Jld[x_11][y_11], 2.L); 
	ldequals(__LINE__, false?K.b2:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b3, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b3, 3.L); 
	ldequals(__LINE__, true?K.b3:Jld[x_11][y_11], 3.L); 
	ldequals(__LINE__, false?K.b3:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b4, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b4, 4.L); 
	ldequals(__LINE__, true?K.b4:Jld[x_11][y_11], 4.L); 
	ldequals(__LINE__, false?K.b4:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b5, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b5, 5.L); 
	ldequals(__LINE__, true?K.b5:Jld[x_11][y_11], 5.L); 
	ldequals(__LINE__, false?K.b5:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b6, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b6, 6.L); 
	ldequals(__LINE__, true?K.b6:Jld[x_11][y_11], 6.L); 
	ldequals(__LINE__, false?K.b6:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b7, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b7, 7.L); 
	ldequals(__LINE__, true?K.b7:Jld[x_11][y_11], 7.L); 
	ldequals(__LINE__, false?K.b7:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b8, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b8, 5.L); 
	ldequals(__LINE__, true?K.b8:Jld[x_11][y_11], 5.L); 
	ldequals(__LINE__, false?K.b8:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b9, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b9, 4.L); 
	ldequals(__LINE__, true?K.b9:Jld[x_11][y_11], 4.L); 
	ldequals(__LINE__, false?K.b9:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b10, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b10, 3.L); 
	ldequals(__LINE__, true?K.b10:Jld[x_11][y_11], 3.L); 
	ldequals(__LINE__, false?K.b10:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b11, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b11, 2.L); 
	ldequals(__LINE__, true?K.b11:Jld[x_11][y_11], 2.L); 
	ldequals(__LINE__, false?K.b11:Jld[x_11][y_11], 18.L); 
#endif
#if ANSI
	ldequals(__LINE__, true?Jld[x_11][y_11]:K.b12, 18.L); 
	ldequals(__LINE__, false?Jld[x_11][y_11]:K.b12, 1.L); 
	ldequals(__LINE__, true?K.b12:Jld[x_11][y_11], 1.L); 
	ldequals(__LINE__, false?K.b12:Jld[x_11][y_11], 18.L); 
#endif
	}}
	return 	report(Filename);
	}
