#include <jerq.h>
#include <font.h>

static int tbits[] = {
	0x0,	0x0,	0x0,	0x0,	0x6,	0x80000000,	0x50020,	0x180000,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x1C01C000,	0x30000000,	0x0,	0x0,	0x0,	0x0,	0x2200800,
	0x800000,	0x0,	0x4200018,	0x8B,	0xE008000B,	0x110000,	0x850073,	0x24180A02,
	0x0,	0x70871C13,	0xE33E71C0,	0x1C,	0x71CF1CF3,	0xEF9C89C0,	0xA282289C,	0xF1CF1CFA,
	0x28A28A2F,	0x90004200,	0x30080008,	0x3008000,	0x20400000,	0x20,	0x0,	0x4208400,
	0x800000,	0x8000,	0xA200004,	0x7BC72288,	0x27040080,	0x2208F80,	0x8514AB,	0x2A20110A,
	0x88000002,	0x8988A232,	0x4048A20,	0x100422,	0x8A28A24A,	0x8228880,	0xA4836CA2,	0x8A28A222,
	0x28A25141,	0x10804708,	0x8080008,	0x4808080,	0xA0400000,	0x20,	0x0,	0x4208400,
	0x868E20,	0x33E8000,	0x8F80502,	0x8028A270,	0x2DBE0040,	0x4404022,	0x803EA0,	0x4A002087,
	0x8000004,	0x98808253,	0xC8048A2C,	0x3023E204,	0xBA28A04A,	0x8208880,	0xA882ACA2,	0x8A28A022,
	0x28A25141,	0x10404A90,	0x1CF1C79,	0xC41CF080,	0xA2434E1C,	0xF1CB1EFA,	0x28A28A2F,	0x84214400,
	0x891153,	0xE4144000,	0x8200A9E,	0x8028A251,	0xEA8403E0,	0x3E208F94,	0x801470,	0x8400208F,
	0xBE03E008,	0xA8810C90,	0x2F0871E0,	0x400108,	0xABEF204B,	0xCF20F880,	0xB0822AA2,	0xF2AF1C22,
	0x28A22082,	0x1020423E,	0x28A28A,	0x2E228880,	0xA442A922,	0x8A2CA022,	0x28A25221,	0x8222200,
	0x2A91E88,	0x27142000,	0x8200AA2,	0x80289C50,	0x2D880040,	0x8110008,	0x801429,	0xA802087,
	0x8000010,	0xC88202F8,	0x28888820,	0x23E208,	0xBA28A04A,	0x8268880,	0xA88229A2,	0x826A0222,
	0x252A5084,	0x10104210,	0x1E8A08B,	0xC4228880,	0xB842A922,	0x8A281C22,	0x28A22222,	0x4214400,
	0x1C91100,	0x4145000,	0x8F80522,	0x7BC00020,	0x27100080,	0x3E000F80,	0x3E2A,	0x3900110A,
	0x88400020,	0x88842212,	0x2888884C,	0x10100400,	0x8228A24A,	0x8228888,	0xA48229A2,	0x82692222,
	0x25365084,	0x10084208,	0x228A08A,	0x4228880,	0xA442A922,	0x8A280222,	0x252A5144,	0x4208400,
	0x869E00,	0x3148800,	0x2800001C,	0x23,	0xE03E0000,	0x1071C000,	0x8014AA,	0x36800A02,
	0x400600,	0x71CF9C11,	0xC7087180,	0x10000018,	0x722F1CF3,	0xE81C89C7,	0x22FA289C,	0x81889C21,
	0xC222888F,	0x90004200,	0x1EF1E79,	0xC41E8880,	0xA242A91C,	0xF1E83C19,	0xC214888F,	0x84208400,
	0x1000,	0x0,	0x10000000,	0x0,	0x100000,	0x20000000,	0x800070,	0x0,
	0x800000,	0x0,	0x0,	0x20000000,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x1C01C000,	0x0,	0x20008,	0x80000000,	0x80200000,	0x100,	0x2200800,
	0x2000,	0x0,	0x0,	0x0,	0x8F800,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x1C0007,	0x0,	0x80200000,	0x200,	0x200000,
};

static Bitmap strike = {
	tbits,
	24,
	0, 0, 768, 10,
	0,
};

struct
{
	short n;		/* number of chars in font */
	char height;		/* height of bitmap */
	char ascent;		/* top of bitmap to baseline */
	long unused;		/* in case we think of more stuff */
	Bitmap *bits;		/* where the characters are */
	Fontchar info[130];	/* n+1 character descriptors */
}
tiny = {
	128,
	10,
	8,
	0,
	&strike,
	{
		{ 0,	0,	10,	0,	6 },
		{ 6,	0,	10,	0,	6 },
		{ 12,	0,	10,	0,	6 },
		{ 18,	0,	10,	0,	6 },
		{ 24,	0,	10,	0,	6 },
		{ 30,	0,	10,	0,	6 },
		{ 36,	0,	10,	0,	6 },
		{ 42,	0,	10,	0,	6 },
		{ 48,	0,	10,	0,	6 },
		{ 54,	0,	10,	0,	6 },
		{ 60,	0,	10,	0,	6 },
		{ 66,	0,	10,	0,	6 },
		{ 72,	0,	10,	0,	6 },
		{ 78,	0,	10,	0,	6 },
		{ 84,	0,	10,	0,	6 },
		{ 90,	0,	10,	0,	6 },
		{ 96,	0,	10,	0,	6 },
		{ 102,	0,	10,	0,	6 },
		{ 108,	0,	10,	0,	6 },
		{ 114,	0,	10,	0,	6 },
		{ 120,	0,	10,	0,	6 },
		{ 126,	0,	10,	0,	6 },
		{ 132,	0,	10,	0,	6 },
		{ 138,	0,	10,	0,	6 },
		{ 144,	0,	10,	0,	6 },
		{ 150,	0,	10,	0,	6 },
		{ 156,	0,	10,	0,	6 },
		{ 162,	0,	10,	0,	6 },
		{ 168,	0,	10,	0,	6 },
		{ 174,	0,	10,	0,	6 },
		{ 180,	0,	10,	0,	6 },
		{ 186,	0,	10,	0,	6 },
		{ 192,	0,	10,	0,	6 },
		{ 198,	0,	10,	0,	6 },
		{ 204,	0,	10,	0,	6 },
		{ 210,	0,	10,	0,	6 },
		{ 216,	0,	10,	0,	6 },
		{ 222,	0,	10,	0,	6 },
		{ 228,	0,	10,	0,	6 },
		{ 234,	0,	10,	0,	6 },
		{ 240,	0,	10,	0,	6 },
		{ 246,	0,	10,	0,	6 },
		{ 252,	0,	10,	0,	6 },
		{ 258,	0,	10,	0,	6 },
		{ 264,	0,	10,	0,	6 },
		{ 270,	0,	10,	0,	6 },
		{ 276,	0,	10,	0,	6 },
		{ 282,	0,	10,	0,	6 },
		{ 288,	0,	10,	0,	6 },
		{ 294,	0,	10,	0,	6 },
		{ 300,	0,	10,	0,	6 },
		{ 306,	0,	10,	0,	6 },
		{ 312,	0,	10,	0,	6 },
		{ 318,	0,	10,	0,	6 },
		{ 324,	0,	10,	0,	6 },
		{ 330,	0,	10,	0,	6 },
		{ 336,	0,	10,	0,	6 },
		{ 342,	0,	10,	0,	6 },
		{ 348,	0,	10,	0,	6 },
		{ 354,	0,	10,	0,	6 },
		{ 360,	0,	10,	0,	6 },
		{ 366,	0,	10,	0,	6 },
		{ 372,	0,	10,	0,	6 },
		{ 378,	0,	10,	0,	6 },
		{ 384,	0,	10,	0,	6 },
		{ 390,	0,	10,	0,	6 },
		{ 396,	0,	10,	0,	6 },
		{ 402,	0,	10,	0,	6 },
		{ 408,	0,	10,	0,	6 },
		{ 414,	0,	10,	0,	6 },
		{ 420,	0,	10,	0,	6 },
		{ 426,	0,	10,	0,	6 },
		{ 432,	0,	10,	0,	6 },
		{ 438,	0,	10,	0,	6 },
		{ 444,	0,	10,	0,	6 },
		{ 450,	0,	10,	0,	6 },
		{ 456,	0,	10,	0,	6 },
		{ 462,	0,	10,	0,	6 },
		{ 468,	0,	10,	0,	6 },
		{ 474,	0,	10,	0,	6 },
		{ 480,	0,	10,	0,	6 },
		{ 486,	0,	10,	0,	6 },
		{ 492,	0,	10,	0,	6 },
		{ 498,	0,	10,	0,	6 },
		{ 504,	0,	10,	0,	6 },
		{ 510,	0,	10,	0,	6 },
		{ 516,	0,	10,	0,	6 },
		{ 522,	0,	10,	0,	6 },
		{ 528,	0,	10,	0,	6 },
		{ 534,	0,	10,	0,	6 },
		{ 540,	0,	10,	0,	6 },
		{ 546,	0,	10,	0,	6 },
		{ 552,	0,	10,	0,	6 },
		{ 558,	0,	10,	0,	6 },
		{ 564,	0,	10,	0,	6 },
		{ 570,	0,	10,	0,	6 },
		{ 576,	0,	10,	0,	6 },
		{ 582,	0,	10,	0,	6 },
		{ 588,	0,	10,	0,	6 },
		{ 594,	0,	10,	0,	6 },
		{ 600,	0,	10,	0,	6 },
		{ 606,	0,	10,	0,	6 },
		{ 612,	0,	10,	0,	6 },
		{ 618,	0,	10,	0,	6 },
		{ 624,	0,	10,	0,	6 },
		{ 630,	0,	10,	0,	6 },
		{ 636,	0,	10,	0,	6 },
		{ 642,	0,	10,	0,	6 },
		{ 648,	0,	10,	0,	6 },
		{ 654,	0,	10,	0,	6 },
		{ 660,	0,	10,	0,	6 },
		{ 666,	0,	10,	0,	6 },
		{ 672,	0,	10,	0,	6 },
		{ 678,	0,	10,	0,	6 },
		{ 684,	0,	10,	0,	6 },
		{ 690,	0,	10,	0,	6 },
		{ 696,	0,	10,	0,	6 },
		{ 702,	0,	10,	0,	6 },
		{ 708,	0,	10,	0,	6 },
		{ 714,	0,	10,	0,	6 },
		{ 720,	0,	10,	0,	6 },
		{ 726,	0,	10,	0,	6 },
		{ 732,	0,	10,	0,	6 },
		{ 738,	0,	10,	0,	6 },
		{ 744,	0,	10,	0,	6 },
		{ 750,	0,	10,	0,	6 },
		{ 756,	0,	10,	0,	6 },
		{ 762,	0,	10,	0,	6 },
		{ 768,	0,	10,	0,	6 },
		{ 0,	0,	0,	0,	116 },
	}
};
