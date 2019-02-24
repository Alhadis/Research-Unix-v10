#define INCH 240
/*
HDS Concept AVT-APL
nroff driving tables
width and code tables
*/

struct termtable tavt = {
/*bset*/	0,
/*breset*/	0,
/*Hor*/		INCH/10,
/*Vert*/	INCH/6,
/*Newline*/	INCH/6,
/*Char*/	INCH/10,
/*Em*/		INCH/10,
/*Halfline*/	INCH/6,
/*Adj*/		INCH/10,
/*twinit*/	"",
/*twrest*/	"",
/*twnl*/	"\n",
/*hlr*/		0,
/*hlf*/		0,
/*flr*/		"\033M",
/*bdon*/	"\033[1m",
/*bdoff*/	"\033[1!{",
/*iton*/	"\033[4m",
/*itoff*/	"\033[4!{",
/*ploton*/	"",
/*plotoff*/	"",
/*up*/		"\033M",
/*down*/	"\033D",
/*right*/	"\033[C",
/*left*/	"\033[D",
/*codetab*/
"\001\033[C",	/*space*/
"\001!",	/*!*/
"\001\"",	/*"*/
"\001#",	/*#*/
"\001$",	/*$*/
"\001%",	/*%*/
"\001&",	/*&*/
"\001'",	/*' close*/
"\001(",	/*(*/
"\001)",	/*)*/
"\001*",	/***/
"\001+",	/*+*/
"\001,",	/*,*/
"\001-",	/*- hyphen*/
"\001.",	/*.*/
"\001/",	/*/*/
"\2010",	/*0*/
"\2011",	/*1*/
"\2012",	/*2*/
"\2013",	/*3*/
"\2014",	/*4*/
"\2015",	/*5*/
"\2016",	/*6*/
"\2017",	/*7*/
"\2018",	/*8*/
"\2019",	/*9*/
"\001:",	/*:*/
"\001;",	/*;*/
"\001<",	/*<*/
"\001=",	/*=*/
"\001>",	/*>*/
"\001?",	/*?*/
"\001@",	/*@*/
"\201A",	/*A*/
"\201B",	/*B*/
"\201C",	/*C*/
"\201D",	/*D*/
"\201E",	/*E*/
"\201F",	/*F*/
"\201G",	/*G*/
"\201H",	/*H*/
"\201I",	/*I*/
"\201J",	/*J*/
"\201K",	/*K*/
"\201L",	/*L*/
"\201M",	/*M*/
"\201N",	/*N*/
"\201O",	/*O*/
"\201P",	/*P*/
"\201Q",	/*Q*/
"\201R",	/*R*/
"\201S",	/*S*/
"\201T",	/*T*/
"\201U",	/*U*/
"\201V",	/*V*/
"\201W",	/*W*/
"\201X",	/*X*/
"\201Y",	/*Y*/
"\201Z",	/*Z*/
"\001[",	/*[*/
"\001\\",	/*\*/
"\001]",	/*]*/
"\001^",	/*^*/
"\001_",	/*_ dash*/
"\001`",	/*` open*/
"\201a",	/*a*/
"\201b",	/*b*/
"\201c",	/*c*/
"\201d",	/*d*/
"\201e",	/*e*/
"\201f",	/*f*/
"\201g",	/*g*/
"\201h",	/*h*/
"\201i",	/*i*/
"\201j",	/*j*/
"\201k",	/*k*/
"\201l",	/*l*/
"\201m",	/*m*/
"\201n",	/*n*/
"\201o",	/*o*/
"\201p",	/*p*/
"\201q",	/*q*/
"\201r",	/*r*/
"\201s",	/*s*/
"\201t",	/*t*/
"\201u",	/*u*/
"\201v",	/*v*/
"\201w",	/*w*/
"\201x",	/*x*/
"\201y",	/*y*/
"\201z",	/*z*/
"\001{",	/*{*/
"\001|",	/*|*/
"\001}",	/*}*/
"\001~",	/*~*/
"\000\0",	/*narrow sp*/
"\001-",	 /*hyphen*/
"\001\016O\bP\017",	 /*bullet*/
"\001\016L\017",	 /*square*/
"\001-",	 /*3/4 em*/
"\001_",	 /*rule*/
"\0031/4",	/*1/4*/
"\0031/2",	/*1/2*/
"\0033/4",	/*3/4*/
"\001-",	 /*minus*/
"\202fi",	 /*fi*/
"\202fl",	 /*fl*/
"\202ff",	 /*ff*/
"\203ffi",	 /*ffi*/
"\203ffl",	 /*ffl*/
"\001\016J\017",	 /*degree*/
"\001\016M\b@\017",	 /*dagger*/
"\000\0",	 /*section*/
"\001\016K\017",	 /*foot mark*/
"\001'",	 /*acute accent*/
"\001`",	 /*grave accent*/
"\001_",	 /*underrule*/
"\001/",	 /*slash (longer)*/
"\000\0",	/*half narrow space*/
"\001\033[C",	/*unpaddable space*/
"\201\016A\017", /*alpha*/
"\201b", /*beta*/
"\201g", /*gamma*/
"\201d", /*delta*/
"\201\016E\017", /*epsilon*/
"\201z", /*zeta*/
"\201e", /*eta*/
"\201\016J\b/\017", /*theta*/
"\201\016I\017",	 /*iota*/
"\201k",	 /*kappa*/
"\201l", /*lambda*/
"\201m", /*mu*/
"\201n", /*nu*/
"\201x", /*xi*/
"\201o",	 /*omicron*/
"\201p", /*pi*/
"\201\016R\017", /*rho*/
"\201\016O\b@\017", /*sigma*/
"\201\016I\bT\017", /*tau*/
"\201v",	 /*upsilon*/
"\201f", /*phi*/
"\201c",	 /*chi*/
"\201s", /*psi*/
"\201\016W\017", /*omega*/
"\201G", /*Gamma*/
"\201\016H\017", /*Delta*/
"\201\016O\bM\017", /*Theta*/
"\201L", /*Lambda*/
"\001X",	 /*Xi*/
"\201P", /*Pi*/
"\201S", /*Sigma*/
"\000\0",	 /**/
"\201Y",	 /*Upsilon*/
"\201F", /*Phi*/
"\203Psi", /*Psi*/
"\201O", /*Omega*/
"\000\0",	 /*square root*/
"\000\0",	 /*terminal sigma*/
"\001\016@\017",	 /*root en*/
"\001\016^\017",	 /*>=*/
"\001\016$\017",	 /*<=*/
"\001_\b=",	 /*identically equal*/
"\001-",	 /*equation minus*/
"\001\016%\bT\017",	 /*approx =*/
"\001\016T\017",	 /*approximates*/
"\001\016*\017",	 /*not equal*/
"\001\016]\017",	 /*right arrow*/
"\001\016[\017",	 /*left arrow*/
"\001\016Y\017",	 /*up arrow*/
"\001\016U\017",	 /*down arrow*/
"\001=",	 /*equation equal*/
"\001\016=\017",	 /*multiply*/
"\001\016+\017",	 /*divide*/
"\001_\b+",	 /*plus-minus*/
"\001\016V\017",	 /*cup (union)*/
"\001\016C\017",	 /*cap (intersection)*/
"\001\016Z\017",	 /*subset of*/
"\001\016X\017",	 /*superset of*/
"\001\016Z\bF\017",	 /*improper subset*/
"\001\016X\bF\017",	 /* improper superset*/
"\002oo",	 /*infinity*/
"\000\0", /*partial derivative*/
"\000\0", /*gradient*/
"\000\0", /*not*/
"\000\0", /*integral sign*/
"\000\0",	 /*proportional to*/
"\000\0",	 /*empty set*/
"\000\0",	 /*member of*/
"\001+",	 /*equation plus*/
"\001R",	 /*registered*/
"\001C",	 /*copyright*/
"\001\016M\017",	 /*box rule */
"\001\016Z\bM\017",	 /*cent sign*/
"\001\016M\b@\b_\017",	 /*dbl dagger*/
"\001\016\\\017",	 /*right hand*/
"\001\016|\017",	 /*left hand*/
"\001*",	 /*math * */
"\000\0",	 /*bell system sign*/
"\001\016(\017",	 /*or (was star)*/
"\001\016O\017",	 /*circle*/
"\001|",	 /*left top (of big curly)*/
"\001|",	 /*left bottom*/
"\001|",	 /*right top*/
"\001|",	 /*right bot*/
"\001{",	 /*left center of big curly bracket*/
"\001}",	 /*right center of big curly bracket*/
"\001|",	/*bold vertical*/
"\001\016D\017",	/*left floor (left bot of big sq bract)*/
"\001|",	/*right floor (rb of ")*/
"\001|",	/*left ceiling (lt of ")*/
"\001\016S\017"};	/*right ceiling (rt of ")*/
