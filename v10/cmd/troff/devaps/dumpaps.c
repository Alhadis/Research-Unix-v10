#include	<stdio.h>
#include	"aps.h"
char	*trans[129];
char	*tr();

main()
{
	short ch;

	init();
	while ((ch = (getchar() & 0377)) != EOF & ch != 0377) {
		switch (ch) {
		case STRTJOB:
			printf(" startjob %d", getint());
			break;
		case STRTPG:
			printf(" startpage %d", getint());
			break;
		case APSNOOP:
			printf(" nop");
			break;
		case ENDJOB:
			printf("\nendjob\n");
			break;
		case RTN:
			printf(" rtn");
			break;
		case SETTAB:
			printf(" settab %d", getint());
			break;
		case XTAB:
			printf(" xtab");
			break;
		case HSPABS:
			printf(" hspabs %d", getint());
			break;
		case VSPABS:
			printf(" vspabs %d", getint());
			break;
		case HVSIZE:
			printf(" hvsize %d", getint());
			break;
		case FONT:
			printf(" font %d", getint());
			break;
		default:
			if (ch > 0200)
				printf("\nStrange ch %d %o\n", ch, ch);
			else {
				printf(" ch %d %s\n", ch, trans[ch]);
			}
			break;
		}
	}
}

getint()
{
	short n;

	n = getchar();
	return(n << 8 | getchar());
}

char *tr(c)
int c;
{
	if (c > 0 && c <= 128 && trans[c] != NULL)
		return trans[c];
	else
		return "";
}

init()
{
	trans[1] = "A";
	trans[2] = "B";
	trans[3] = "C";
	trans[4] = "D";
	trans[5] = "E";
	trans[6] = "F";
	trans[7] = "G";
	trans[8] = "H";
	trans[9] = "I";
	trans[10] = "J";
	trans[11] = "K";
	trans[12] = "L";
	trans[13] = "M";
	trans[14] = "N";
	trans[15] = "O";
	trans[16] = "P";
	trans[17] = "Q";
	trans[18] = "R";
	trans[19] = "S";
	trans[20] = "T";
	trans[21] = "U";
	trans[22] = "V";
	trans[23] = "W";
	trans[24] = "X";
	trans[25] = "Y";
	trans[26] = "Z";
	trans[27] = "a";
	trans[28] = "b";
	trans[29] = "c";
	trans[30] = "d";
	trans[31] = "e";
	trans[32] = "f";
	trans[33] = "g";
	trans[34] = "h";
	trans[35] = "i";
	trans[36] = "j";
	trans[37] = "k";
	trans[38] = "l";
	trans[39] = "m";
	trans[40] = "n";
	trans[41] = "o";
	trans[42] = "p";
	trans[43] = "q";
	trans[44] = "r";
	trans[45] = "s";
	trans[46] = "t";
	trans[47] = "u";
	trans[48] = "v";
	trans[49] = "w";
	trans[50] = "x";
	trans[51] = "y";
	trans[52] = "z";
	trans[53] = "1";
	trans[54] = "2";
	trans[55] = "3";
	trans[56] = "4";
	trans[57] = "5";
	trans[58] = "6";
	trans[59] = "7";
	trans[60] = "8";
	trans[61] = "9";
	trans[62] = "0";
	trans[63] = "$";
	trans[64] = "ct";
	trans[65] = "&";
	trans[66] = ":";
	trans[67] = ";";
	trans[68] = ".";
	trans[69] = ",";
	trans[70] = "`";
	trans[71] = "'";
	trans[72] = "?";
	trans[73] = "!";
	trans[74] = "%";
	trans[75] = "*";
	trans[76] = "(";
	trans[77] = ")";
	trans[78] = "/";
	trans[79] = "hy";
	trans[80] = "em";
	trans[81] = "en";
	trans[82] = "ff";
	trans[83] = "fi";
	trans[84] = "fl";
	trans[85] = "Fi";
	trans[86] = "Fl";
	trans[91] = "14";
	trans[92] = "12";
	trans[93] = "34";
	trans[111] = "co";
	trans[112] = "rg";
	trans[113] = "tm";
	trans[116] = "#";
	trans[120] = "@";
	trans[121] = "bu";
	trans[123] = "de";
	trans[124] = "dg";
	trans[125] = "dd";
	trans[126] = "sc";
	trans[127] = "[";
	trans[128] = "]";
}
