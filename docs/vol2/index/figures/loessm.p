%!PS-Adobe-1.0
%%Title: S QPE graphics
%%Creator: Trevor Hastie
%%CreationDate: Mon Mar  5 22:44:11 1990
%%Pages: (atend)
%%BoundingBox: 89.88 252.12 521.88 540.12
%%EndComments
% beginning of preamble
100 dict begin
/bd {bind def} def
% drawing commands
/I {Coord SetPage 1 setlinecap 1 setlinejoin
    LineTypes {RastersPerPoint ScaleArray} forall
    /Helvetica findfont
    PointSize RastersPerPoint mul Cex mul scalefont setfont} bd
/A {PageBegin} bd
/B {newpath} bd
/C {currentpoint stroke moveto} bd
/E {stroke} bd
/M {moveto} bd
/L {lineto} bd
/S {moveto lineto stroke} bd
/F {closepath fill} bd
/P {gsave moveto Pch-x Pch-y rmoveto Pch Show grestore} bd
/T {/Adjust exch def gsave translate StringRot rotate 0 0 moveto
    dup stringwidth pop neg Adjust mul 0 rmoveto
    currentpoint translate TextShow grestore} bd
/X {erasepage InPage {PageEnd} if} bd
/Z {gsave showpage grestore PageEnd} bd
/W {end} bd

% parameter setting commands
/St {1 sub LineTypes dup 3 1 roll length Rem floor get 0 setdash} bd
/Sw {abs 2 div RastersPerPoint mul setlinewidth SetClip} bd
/Sc {dup dup 1 lt exch 0 ge and
     {1 exch sub setgray}
     {1 sub Colors dup 3 1 roll length Rem floor get
	dup type /arraytype eq {aload pop sethsbcolor} {setgray} ifelse} ifelse} bd
/Sp {Pch exch 0 exch put SetPchSize} bd
/Sx {dup Cex div /Ratio exch def
     /Cex exch def
     currentfont Ratio scalefont setfont
     /Pch-x Pch-x Ratio mul def
     /Pch-y Pch-y Ratio mul def
     /Text-y Text-y Ratio mul def} bd
/So {4 1 roll exch 4 -1 roll Plot astore pop SetClip} bd
/Sg {4 1 roll exch 4 -1 roll Figure astore pop SetClip} bd
/Sr {/StringRot exch def} bd
/Sh {/CharRot exch def} bd
/Sd {0 eq /ClipToPlot exch def SetClip} bd
/Sf {dup 0 lt /Outline exch def abs
     1 sub Fonts dup 3 1 roll length Rem floor get
     findfont PointSize Cex mul RastersPerPoint mul scalefont dup setfont
     dup /FontMatrix get /Matrix exch def /FontBBox get aload pop
     Matrix transform 4 2 roll Matrix transform
     exch pop add /Text-y exch def pop SetPchSize} bd

% other variable definitions
/InPage false def
/Clip 4 array def
/Page 4 array def
/Figure [0 0 1 1] def
/Plot [0 0 1 1] def
/ClipToPlot true def
/Cex 1 def
/Outline false def
/Pch 1 string def
/Pch-x 0 def
/Pch-y 0 def
/Text-y 0 def
/LineTypes [ % in default units
	[]		[1 2]		[4 4]		[8 4]
	[13 3]		[16 2 2 2]	[8 2 2 2]	[1 13]
	[6 5]		[12 4]
] def

% other procedure definitions
/Rem {2 copy div floor mul sub floor cvi} bd
/RastersPerPoint {RastersPerInch 72 div} bd
/ScaleArray {/Factor exch def /Array exch def
	     0 1 Array length 1 sub
	     {dup Array exch get Factor mul Array 3 1 roll put} for} bd
/Coord {Region aload pop /uy exch def /ux exch def /ly exch def /lx exch def
	uy ly sub ux lx sub Landscape {exch} if /Width exch def /Height exch def
	lx ly translate Landscape {90 rotate 0 Height neg translate} if
	1 RastersPerPoint div dup scale} bd
/SetPchSize {gsave
	     newpath 0 0 moveto Pch false charpath flattenpath pathbbox
	     exch 3 1 roll
	     add 2 div neg /Pch-y exch def
	     add 2 div neg /Pch-x exch def
	     grestore} bd
/TextShow {CharRot StringRot sub dup 0 eq {pop SimpleShow} {FancyShow} ifelse} bd
/SimpleShow {0 Text-y 2 div neg rmoveto Show} bd
/FancyShow {
	/RotDiff exch def
	/Cos RotDiff cos abs def
	/Sin RotDiff sin abs def
	{
		( ) dup 0 4 -1 roll put
		dup stringwidth pop /CharWidth exch def
		Cos 0 eq {
			Text-y Sin div
		} {
			Sin 0 eq {
				CharWidth Cos div
			} {
				/H Text-y Sin div def
				/W CharWidth Cos div def
				H W lt {H} {W} ifelse
			} ifelse
		} ifelse 2 div /CharDist exch def
		CharDist 0 translate 0 0 moveto
		gsave
			RotDiff rotate
			CharWidth 2 div neg Text-y 2 div neg rmoveto
			Outline {false charpath stroke} {show} ifelse
		grestore
		CharDist 0 translate 0 0 moveto
	} forall
} bd
/Show {Outline {false charpath stroke} {show} ifelse} bd
/BoxClip {/CLW currentlinewidth def
	  2 {CLW add 4 1 roll} repeat
	  2 {CLW sub 4 1 roll} repeat
	  initclip newpath 2 index exch 2 index exch dup 6 index exch
	  moveto 3 {lineto} repeat closepath clip newpath} bd
/Subregion {/A exch def /Uy exch def /Ux exch def /Ly exch def /Lx exch def
	    Ux Lx sub A 0 get mul Lx add
	    Uy Ly sub A 1 get mul Ly add
	    Ux Lx sub A 2 get mul Lx add
	    Uy Ly sub A 3 get mul Ly add} bd
/SetFigure {Page aload pop Figure Subregion} bd
/SetPlot {SetFigure Plot Subregion} bd
/SetClip {ClipToPlot {SetPlot} {SetFigure} ifelse BoxClip} bd
/SetPage {0 0 Width Height Page astore RastersPerPoint ScaleArray} bd
/PageBegin {save /PageContext exch def /InPage true def} bd
/PageEnd {PageContext restore /InPage false def} bd
% end of preamble

% fixed controlling parameters
/Landscape false def
/Region [89.88 252.12 521.88 540.12] def
/RastersPerInch 300 def
/PointSize 14 def
/Fonts [
	/Helvetica
	/Courier
	/Times-Roman
	/Helvetica-Oblique
	/Helvetica-Bold
	/Helvetica-BoldOblique
	/Courier-Oblique
	/Courier-Bold
	/Courier-BoldOblique
	/Times-Italic
	/Times-Bold
	/Times-BoldItalic
	/Symbol
	/AvantGarde-Book
	/AvantGarde-BookOblique
	/AvantGarde-Demi
	/AvantGarde-DemiOblique
	/Bookman-Demi
	/Bookman-DemiItalic
	/Bookman-Light
	/Bookman-LightItalic
	/Helvetica-Narrow
	/Helvetica-Narrow-Bold
	/Helvetica-Narrow-BoldOblique
	/Helvetica-Narrow-Oblique
	/NewCenturySchlbk-Roman
	/NewCenturySchlbk-Bold
	/NewCenturySchlbk-Italic
	/NewCenturySchlbk-BoldItalic
	/Palatino-Roman
	/Palatino-Bold
	/Palatino-Italic
	/Palatino-BoldItalic
	/ZapfChancery-MediumItalic
	/ZapfDingbats
] def
/Colors [
	0
	0.6
	0.3
	0.9
	0.4
	0.7
	0.1
	0.5
	0.8
	0.2
] def

% all initialization action here
I

%%EndProlog


%%Page: 1 1
A
1 St
1 Sw
1 Sc
0 Sr
42 Sp
1 Sx
0 1 0 1 So
0 1 0 1 Sg
0 Sh
0 Sd
1 Sf
B
0 1200 M
0 0 L
1800 0 L
1800 1200 L
0 1200 L
E
0.15 Sc
B
860 14 M
1371 14 L
1371 1186 L
860 1186 L
F
1 Sc
67 44 P
78 322 P
169 168 P
180 260 P
214 322 P
441 693 P
509 322 P
509 507 P
543 538 P
554 600 P
554 631 P
622 847 P
747 693 P
860 600 P
860 600 P
883 723 P
928 754 P
962 384 P
974 631 P
1076 600 P
1087 631 P
1144 662 P
1155 723 P
1166 507 P
1166 816 P
1189 693 P
1212 477 P
1223 569 P
1223 693 P
1246 693 P
1268 816 P
1302 538 P
1314 693 P
1371 723 P
1382 384 P
1405 168 P
1416 1156 P
1461 970 P
1461 538 P
1529 260 P
1609 878 P
1722 631 P
1733 631 P
B
67 184 M
78 193 L
169 267 L
180 276 L
214 302 L
441 474 L
509 525 L
543 553 L
554 562 L
622 593 L
747 620 L
860 649 L
883 642 L
928 636 L
962 630 L
974 630 L
1076 631 L
1087 630 L
1144 637 L
1155 640 L
1166 645 L
1189 647 L
1212 647 L
1223 645 L
1246 644 L
1268 643 L
1302 641 L
1314 644 L
1371 633 L
1382 632 L
1405 633 L
1416 632 L
1461 637 L
1529 645 L
1609 647 L
1722 654 L
1733 655 L
E
B
1087 -16 M
1087 75 L
1110 52 L
E
B
1064 52 M
1087 75 L
E
B
860 98 M
865 110 L
871 123 L
876 137 L
881 152 L
886 168 L
891 184 L
896 200 L
902 217 L
907 234 L
912 251 L
917 268 L
922 284 L
927 301 L
932 317 L
938 332 L
943 347 L
948 362 L
953 376 L
958 389 L
963 401 L
969 413 L
974 424 L
979 434 L
984 444 L
989 452 L
994 460 L
999 468 L
1005 474 L
1010 480 L
1015 485 L
1020 489 L
1025 493 L
1030 496 L
1036 499 L
1041 501 L
1046 503 L
1051 505 L
1056 506 L
1061 506 L
1066 507 L
1072 507 L
1077 507 L
1082 507 L
1087 507 L
1092 507 L
1097 507 L
1103 507 L
1108 507 L
1113 506 L
1118 506 L
1123 505 L
1128 503 L
1133 501 L
1139 499 L
1144 496 L
1149 493 L
1154 489 L
1159 485 L
1164 480 L
1170 474 L
1175 468 L
1180 460 L
1185 452 L
1190 444 L
1195 434 L
1200 424 L
1206 413 L
1211 401 L
1216 389 L
1221 376 L
1226 362 L
1231 347 L
1237 332 L
1242 317 L
1247 301 L
1252 284 L
1257 268 L
1262 251 L
1267 234 L
1273 217 L
1278 200 L
1283 184 L
1288 168 L
1293 152 L
1298 137 L
1304 123 L
1309 110 L
1314 98 L
1319 87 L
1324 78 L
1329 69 L
1334 62 L
1340 56 L
1345 52 L
1350 48 L
1355 46 L
1360 45 L
1365 45 L
1371 44 L
E
Z

%%Trailer
W
%%Pages: 1
