#include <stdio.h>

#define DBL 0200
/*
C version of pti
*/

char *ap;
int esc;
int escd;
int verd;
int esct;
int osize = 02;
int size = 02;
int leadtot;
int railmag;
int lead;
int mcase;
int stab[] = {010,0,01,07,02,03,04,05,0211,06,0212,0213,0214,0215,0216,0217};
int rtab[] = {6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 28, 36, 18};

char *asctab[128] = {
"(blank)",	/*blank*/
"h",	/*h*/
"t",	/*t*/
"n",	/*n*/
"m",	/*m*/
"l",	/*l*/
"i",	/*i*/
"z",	/*z*/
"s",	/*s*/
"d",	/*d*/
"b",	/*b*/
"x",	/*x*/
"f",	/*f*/
"j",	/*j*/
"u",	/*u*/
"k",	/*k*/
"(blank)",	/*blank*/
"p",	/*p*/
"em dash",	/*3/4 em dash*/
";",	/*;*/
"(blank)",	/*blank*/
"a",	/*a*/
"_ (baseline)",	/*rule*/
"c",	/*c*/
"` (open)",	/*` open*/
"e",	/*e*/
"' (close)",	/*' close*/
"o",	/*o*/
"1/4",	/*1/4*/
"r",	/*r*/
"1/2",	/*1/2*/
"v",	/*v*/
"hyphen",	/*- hyphen*/
"w",	/*w*/
"q",	/*q*/
"/",	/*/*/
".",	/*.*/
"g",	/*g*/
"3/4",	/*3/4*/
",",	/*,*/
"&",	/*&*/
"y",	/*y*/
"(blank)",	/*blank*/
"%",	/*%*/
"(blank)",	/*blank*/
"Q",	/*Q*/
"T",	/*T*/
"O",	/*O*/
"H",	/*H*/
"N",	/*N*/
"M",	/*M*/
"L",	/*L*/
"R",	/*R*/
"G",	/*G*/
"I",	/*I*/
"P",	/*P*/
"C",	/*C*/
"V",	/*V*/
"E",	/*E*/
"Z",	/*Z*/
"D",	/*D*/
"B",	/*B*/
"S",	/*S*/
"Y",	/*Y*/
"(blank)",	/*blank*/
"F",	/*F*/
"X",	/*X*/
"A",	/*A*/
"W",	/*W*/
"J",	/*J*/
"U",	/*U*/
"K",	/*K*/
"0",	/*0*/
"1",	/*1*/
"2",	/*2*/
"3",	/*3*/
"4",	/*4*/
"5",	/*5*/
"6",	/*6*/
"7",	/*7*/
"8",	/*8*/
"9",	/*9*/
"*",	/***/
"-",	/*minus*/
"fi",	/*fi*/
"fl",	/*fl*/
"ff",	/*ff*/
"cent",	/*cent sign*/
"ffl",	/*ffl*/
"ffi",	/*ffi*/
"(",	/*(*/
")",	/*)*/
"[",	/*[*/
"]",	/*]*/
"degree",	/*degree*/
"dagger",	/*dagger*/
"=",	/*=*/
"registered",	/*registered*/
":",	/*:*/
"+",	/*+*/
"(blank)",	/*blank*/
"!",	/*!*/
"bullet",	/*bullet*/
"?",	/*?*/
"' (foot)",	/*foot mark*/
"|",	/*|*/
"(blank)",	/*blank*/
"copyright",	/*copyright*/
"[]",	/*square*/
"$" };	/*$*/

char *spectab[128] = {
"(blank)",	/*blank*/
"psi",	/*psi*/
"theta",	/*theta*/
"nu",	/*nu*/
"mu",	/*mu*/
"lambda",	/*lambda*/
"iota",	/*iota*/
"zeta",	/*zeta*/
"sigma",	/*sigma*/
"delta",	/*delta*/
"beta",	/*beta*/
"xi",	/*xi*/
"eta",	/*eta*/
"phi",	/*phi*/
"upsilon",	/*upsilon*/
"kappa",	/*kappa*/
"(blank)",	/*blank*/
"pi",	/*pi*/
"@",	/*at-sign*/
"down arrow",	/*down arrow*/
"(blank)",	/*blank*/
"alpha",	/*alpha*/
"| or",	/*or*/
"chi",	/*chi*/
"\"",	/*"*/
"epsilon",	/*epsilon*/
"= math",	/*=*/
"omicron",	/*omicron*/
"left arrow",	/*left arrow*/
"rho",	/*rho*/
"up arrow",	/*up arrow*/
"tau",	/*tau*/
"_ (under)",	/*underrule*/
"\\",	/*\*/
"Psi",	/*Psi*/
"bell system sign",	/*bell system sign*/
"infinity",	/*infinity*/
"gamma",	/*gamma*/
"improper superset",	/*improper superset*/
"proportional to",	/*proportional to*/
"right hand",	/*right hand*/
"omega",	/*omega*/
"(blank)",	/*blank*/
"gradient",	/*gradient*/
"(blank)",	/*blank*/
"Phi",	/*Phi*/
"Theta",	/*Theta*/
"Omega",	/*Omega*/
"cup (union)",	/*cup (union)*/
"root en",	/*root en*/
"terminal sigma",	/*terminal sigma*/
"Lambda",	/*Lambda*/
"- math",	/*minus*/
"Gamma",	/*Gamma*/
"integral sign",	/*integral sign*/
"Pi",	/*Pi*/
"subset of",	/*subset of*/
"superset of",	/*superset of*/
"approximates",	/*approximates*/
"partial",	/*partial derivative*/
"Delta",	/*Delta*/
"square root",	/*square root*/
"Sigma",	/*Sigma*/
"approx =",	/*approx =*/
"(blank)",	/*blank*/
">",	/*>*/
"Xi",	/*Xi*/
"<",	/*<*/
"/ (match \\)",	/*slash (longer)*/
"cap (intersection)",	/*cap (intersection)*/
"Upsilon",	/*Upsilon*/
"not",	/*not*/
"right ceiling",	/*right ceiling (rt of ")*/
"left top curly",	/*left top (of big curly)*/
"bold vertical",	/*bold vertical*/
"left center curley",	/*left center of big curly bracket*/
"left bottom curley",	/*left bottom*/
"right top curley",	/*right top*/
"right center curley",	/*right center of big curly bracket*/
"right bottom curley",	/*right bot*/
"right floor",	/*right floor (rb of ")*/
"left floor",	/*left floor (left bot of big sq bract)*/
"left ceiling",	/*left ceiling (lt of ")*/
"multiply",	/*multiply*/
"divide",	/*divide*/
"+-",	/*plus-minus*/
"<=",	/*<=*/
">=",	/*>=*/
"==",	/*identically equal*/
"not equal",	/*not equal*/
"{",	/*{*/
"}",	/*}*/
"'",	/*' acute accent*/
"`",	/*` grave accent*/
"^",	/*^*/
"#",	/*sharp*/
"left hand",	/*left hand*/
"member of",	/*member of*/
"~",	/*~*/
"empty set",	/*empty set*/
"(blank)",	/*blank*/
"dbl dagger",	/*dbl dagger*/
"| (box rule)",	/*box rule*/
"*",	/*asterisk*/
"improper subset",	/*improper subset*/
"circle",	/*circle*/
"(blank)",	/*blank*/
"+ math",	/*eqn plus*/
"right arrow",	/*right arrow*/
"section mark" };	/*section mark*/

long offset;
int lflg;

main(argc,argv)
int argc;
char **argv;
{
	register i, j;
	register char *k;
	extern ex();

	while((--argc > 0) && ((++argv)[0][0]=='-')){
		switch(argv[0][1]){
			case 'l':
				lflg++;
				continue;
			default:
				ap = &argv[0][1];
				while(((j = *ap++ - '0') >= 0)
					&& (j <= 9))offset = 8*offset +j;
				continue;
		}
	}
	if(argc){
		if (freopen(argv[0], "r", stdin) == NULL) {
			printf("Cannot open: %s\n",argv[0]);
			exit(1);
		}
	}
	if (offset)
		fseek(stdin, offset, 0);
	while((i = getchar()) != EOF){
		if(i & 0200){
			if(!lflg)printf("%o ",i);
			esc += (~i) & 0177;
			continue;
		}
		if(esc){
			if(escd){
				if(!lflg)printf("< %d\n",esc);
				esc = -esc;
			}else{
				if(!lflg)printf("> %d\n",esc);
			}
			esct += esc;
			esc = 0;
		}
		if(!lflg)printf("%o ",i);
		if(!i){if(!lflg)printf("\n"); continue;}
		switch(i){
			case 0100:	/*init*/
				escd = verd = mcase = railmag = 0;
				if(!lflg)printf("Initialize\n");
				continue;
			case 0101:	/*lower rail*/
				railmag &= ~01;
				if(!lflg)printf("Lower rail\n");
				continue;
			case 0102:	/*upper rail*/
				railmag |= 01;
				if(!lflg)printf("Upper rail\n");
				continue;
			case 0103:	/*upper mag*/
				railmag |= 02;
				if(!lflg)printf("Upper mag\n");
				continue;
			case 0104:	/*lower mag*/
				railmag &= ~02;
				if(!lflg)printf("Lower mag\n");
				continue;
			case 0105:	/*lower case*/
				mcase = 0;
				if(!lflg)printf("Lower case\n");
				continue;
			case 0106:	/*upper case*/
				mcase = 0100;
				if(!lflg)printf("Upper case\n");
				continue;
			case 0107:	/*escape forward*/
				escd = 0;
				if(!lflg)printf("> mode, %d\n",esct);
				continue;
			case 0110:	/*escape backward*/
				escd = 1;
				if(!lflg)printf("< mode, %d\n",esct);
				continue;
			case 0111:	/*stop*/
				if(!lflg)printf("STOP\n");
				continue;
			case 0112:	/*lead forward*/
				verd = 0;
				if(!lflg)printf("Lead forward, %d\n",leadtot);
				continue;
			case 0114:	/*lead backward*/
				verd = 1;
				if(!lflg)printf("Lead backward, %d\n",leadtot);
				continue;
			case 0115:	/*undefined*/
			case 0116:
			case 0117:
			case 0113:
				if(!lflg)printf("Undefined code\n");
				continue;
		}
		if((i & 0340) == 0140){	/*leading*/
			lead = (~i) & 037;
			if(!lflg)printf("Lead %d\n",lead);
			if(verd)lead = -lead;
			leadtot += lead;
			continue;
		}
		if((i & 0360) == 0120){	/*size change*/
			i &= 017;
			for(j = 0; i != (stab[j] & 017); j++);
			osize = size;
			size = stab[j];
			if(!lflg){
				printf("Size %d",rtab[j]);
				if(!(osize & DBL) && (size & DBL))printf(", double\n");
				else if((osize & DBL) && !(size & DBL))printf(", single\n");
				else printf("\n");
			}
			continue;
		}
		if(i & 0300)continue;
		i = (i & 077) | mcase;
		if(railmag != 03)k = asctab[i];
		else k = spectab[i];
		if(!lflg)printf("%s\n",k);
		continue;
	}
	ex();
}
ex(){
	printf("Total lead %d\n",leadtot);
	exit(0);
}
