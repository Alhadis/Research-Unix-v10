%!PS-Adobe-1.0
%%Title: S QPE graphics
%%Creator: Trevor Hastie
%%CreationDate: Tue Mar  6 10:54:34 1990
%%Pages: (atend)
%%BoundingBox: 20.8788 44.76 590.881 747.48
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
/Region [20.8788 44.76 590.881 747.48] def
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
0.0442103 0.95579 0.0717213 0.856557 So
0 0.333333 0.666667 1 Sg
0 Sh
0 Sd
1 Sf
257 2405 P
421 2267 P
498 2484 P
503 2543 P
494 2444 P
103 2129 P
598 2129 P
730 2425 P
284 2563 P
107 2188 P
121 2228 P
239 2346 P
380 2405 P
257 2425 P
62 2050 P
557 2366 P
380 2405 P
544 2543 P
503 2346 P
407 2504 P
525 2385 P
603 2760 P
534 2464 P
66 2228 P
680 2582 P
562 2464 P
389 2484 P
648 2188 P
725 2425 P
466 2405 P
521 2326 P
584 2484 P
525 2464 P
253 2366 P
239 2228 P
212 2464 P
334 2464 P
621 2641 P
471 2425 P
589 2267 P
621 2366 P
425 2425 P
512 2464 P
B
35 2788 M
35 2022 L
757 2022 L
757 2788 L
35 2788 L
E
B
35 2252 M
757 2512 L
E
B
62 2102 M
66 2114 L
103 2198 L
107 2207 L
121 2234 L
212 2359 L
239 2383 L
239 2383 L
253 2393 L
257 2396 L
257 2396 L
284 2411 L
334 2428 L
380 2435 L
380 2435 L
389 2436 L
407 2436 L
421 2436 L
425 2436 L
466 2435 L
471 2434 L
494 2433 L
498 2432 L
503 2432 L
503 2432 L
512 2431 L
521 2431 L
525 2430 L
525 2430 L
534 2430 L
544 2429 L
557 2428 L
562 2428 L
584 2427 L
589 2427 L
598 2427 L
603 2427 L
621 2427 L
621 2427 L
648 2429 L
680 2433 L
725 2442 L
730 2443 L
E
1.125 Sx
1 Sd
(polynomial) 396 2889 0.5 T
1 Sx
0.333333 0.666667 0.666667 1 Sg
0 Sd
1049 2405 P
1213 2267 P
1290 2484 P
1294 2543 P
1285 2444 P
894 2129 P
1390 2129 P
1522 2425 P
1076 2563 P
899 2188 P
912 2228 P
1031 2346 P
1172 2405 P
1049 2425 P
853 2050 P
1349 2366 P
1172 2405 P
1335 2543 P
1294 2346 P
1199 2504 P
1317 2385 P
1394 2760 P
1326 2464 P
858 2228 P
1472 2582 P
1353 2464 P
1181 2484 P
1440 2188 P
1517 2425 P
1258 2405 P
1313 2326 P
1376 2484 P
1317 2464 P
1044 2366 P
1031 2228 P
1003 2464 P
1126 2464 P
1413 2641 P
1263 2425 P
1381 2267 P
1413 2366 P
1217 2425 P
1303 2464 P
B
827 2788 M
827 2022 L
1548 2022 L
1548 2788 L
827 2788 L
E
1.125 Sx
1 Sd
(bin) 1188 2889 0.5 T
1 Sx
0 Sd
B
853 2165 M
958 2165 L
958 2399 L
1101 2399 L
1101 2422 L
1238 2422 L
1238 2412 L
1392 2412 L
1392 2484 L
1522 2484 L
E
0.666667 1 0.666667 1 Sg
1841 2405 P
2004 2267 P
2081 2484 P
2086 2543 P
2077 2444 P
1686 2129 P
2181 2129 P
2313 2425 P
1868 2563 P
1691 2188 P
1704 2228 P
1822 2346 P
1963 2405 P
1841 2425 P
1645 2050 P
2141 2366 P
1963 2405 P
2127 2543 P
2086 2346 P
1991 2504 P
2109 2385 P
2186 2760 P
2118 2464 P
1650 2228 P
2263 2582 P
2145 2464 P
1972 2484 P
2231 2188 P
2309 2425 P
2050 2405 P
2104 2326 P
2168 2484 P
2109 2464 P
1836 2366 P
1822 2228 P
1795 2464 P
1918 2464 P
2204 2641 P
2054 2425 P
2172 2267 P
2204 2366 P
2009 2425 P
2095 2464 P
B
1618 2788 M
1618 2022 L
2340 2022 L
2340 2788 L
1618 2788 L
E
B
1645 2247 M
1650 2247 L
1686 2247 L
1691 2247 L
1704 2247 L
1795 2357 L
1822 2387 L
1822 2387 L
1836 2407 L
1841 2407 L
1841 2407 L
1868 2407 L
1918 2447 L
1963 2420 L
1963 2420 L
1972 2420 L
1991 2420 L
2004 2418 L
2009 2418 L
2050 2423 L
2054 2423 L
2077 2425 L
2081 2431 L
2086 2436 L
2086 2436 L
2095 2436 L
2104 2447 L
2109 2447 L
2109 2447 L
2118 2447 L
2127 2447 L
2141 2418 L
2145 2396 L
2168 2447 L
2172 2447 L
2181 2407 L
2186 2407 L
2204 2431 L
2204 2431 L
2231 2427 L
2263 2420 L
2309 2420 L
2313 2420 L
E
1.125 Sx
1 Sd
(running mean) 1979 2889 0.5 T
1 Sx
0 0.333333 0.333333 0.666667 Sg
0 Sd
257 1429 P
421 1291 P
498 1508 P
503 1567 P
494 1468 P
103 1153 P
598 1153 P
730 1449 P
284 1587 P
107 1212 P
121 1252 P
239 1370 P
380 1429 P
257 1449 P
62 1074 P
557 1390 P
380 1429 P
544 1567 P
503 1370 P
407 1528 P
525 1409 P
603 1784 P
534 1488 P
66 1252 P
680 1606 P
562 1488 P
389 1508 P
648 1212 P
725 1449 P
466 1429 P
521 1350 P
584 1508 P
525 1488 P
253 1390 P
239 1252 P
212 1488 P
334 1488 P
621 1665 P
471 1449 P
589 1291 P
621 1390 P
425 1449 P
512 1488 P
B
35 1812 M
35 1046 L
757 1046 L
757 1812 L
35 1812 L
E
B
64 1162 M
105 1215 L
121 1236 L
212 1371 L
239 1394 L
256 1400 L
284 1436 L
334 1437 L
380 1449 L
389 1458 L
407 1463 L
423 1464 L
469 1461 L
496 1454 L
503 1458 L
512 1467 L
524 1466 L
534 1459 L
544 1457 L
559 1460 L
587 1454 L
600 1464 L
621 1452 L
648 1456 L
680 1463 L
728 1461 L
E
1.125 Sx
1 Sd
(running line) 396 1913 0.5 T
1 Sx
0.333333 0.666667 0.333333 0.666667 Sg
0 Sd
1049 1429 P
1213 1291 P
1290 1508 P
1294 1567 P
1285 1468 P
894 1153 P
1390 1153 P
1522 1449 P
1076 1587 P
899 1212 P
912 1252 P
1031 1370 P
1172 1429 P
1049 1449 P
853 1074 P
1349 1390 P
1172 1429 P
1335 1567 P
1294 1370 P
1199 1528 P
1317 1409 P
1394 1784 P
1326 1488 P
858 1252 P
1472 1606 P
1353 1488 P
1181 1508 P
1440 1212 P
1517 1449 P
1258 1429 P
1313 1350 P
1376 1508 P
1317 1488 P
1044 1390 P
1031 1252 P
1003 1488 P
1126 1488 P
1413 1665 P
1263 1449 P
1381 1291 P
1413 1390 P
1217 1449 P
1303 1488 P
B
827 1812 M
827 1046 L
1548 1046 L
1548 1812 L
827 1812 L
E
B
853 1171 M
858 1176 L
894 1221 L
899 1226 L
912 1242 L
1003 1348 L
1031 1375 L
1031 1375 L
1044 1391 L
1049 1396 L
1049 1396 L
1076 1416 L
1126 1441 L
1172 1458 L
1172 1458 L
1181 1457 L
1199 1451 L
1213 1449 L
1217 1449 L
1258 1448 L
1263 1448 L
1285 1452 L
1290 1454 L
1294 1455 L
1294 1455 L
1303 1457 L
1313 1458 L
1317 1457 L
1317 1457 L
1326 1457 L
1335 1458 L
1349 1457 L
1353 1456 L
1376 1452 L
1381 1451 L
1390 1450 L
1394 1450 L
1413 1453 L
1413 1453 L
1440 1456 L
1472 1459 L
1517 1463 L
1522 1464 L
E
1.125 Sx
1 Sd
(loess) 1188 1913 0.5 T
1 Sx
0.666667 1 0.333333 0.666667 Sg
0 Sd
1841 1429 P
2004 1291 P
2081 1508 P
2086 1567 P
2077 1468 P
1686 1153 P
2181 1153 P
2313 1449 P
1868 1587 P
1691 1212 P
1704 1252 P
1822 1370 P
1963 1429 P
1841 1449 P
1645 1074 P
2141 1390 P
1963 1429 P
2127 1567 P
2086 1370 P
1991 1528 P
2109 1409 P
2186 1784 P
2118 1488 P
1650 1252 P
2263 1606 P
2145 1488 P
1972 1508 P
2231 1212 P
2309 1449 P
2050 1429 P
2104 1350 P
2168 1508 P
2109 1488 P
1836 1390 P
1822 1252 P
1795 1488 P
1918 1488 P
2204 1665 P
2054 1449 P
2172 1291 P
2204 1390 P
2009 1449 P
2095 1488 P
B
1618 1812 M
1618 1046 L
2340 1046 L
2340 1812 L
1618 1812 L
E
B
1645 1147 M
1650 1153 L
1686 1206 L
1691 1213 L
1704 1233 L
1795 1361 L
1822 1394 L
1822 1394 L
1836 1408 L
1841 1412 L
1841 1412 L
1868 1429 L
1918 1445 L
1963 1450 L
1963 1450 L
1972 1450 L
1991 1450 L
2004 1450 L
2009 1450 L
2050 1451 L
2054 1452 L
2077 1453 L
2081 1454 L
2086 1454 L
2086 1454 L
2095 1454 L
2104 1455 L
2109 1455 L
2109 1455 L
2118 1455 L
2127 1455 L
2141 1455 L
2145 1455 L
2168 1454 L
2172 1454 L
2181 1454 L
2186 1454 L
2204 1455 L
2204 1455 L
2231 1457 L
2263 1460 L
2309 1465 L
2313 1465 L
E
1.125 Sx
1 Sd
(gaussian kernel) 1979 1913 0.5 T
1 Sx
0 0.333333 0 0.333333 Sg
0 Sd
257 453 P
421 315 P
498 532 P
503 591 P
494 492 P
103 177 P
598 177 P
730 473 P
284 611 P
107 236 P
121 276 P
239 394 P
380 453 P
257 473 P
62 98 P
557 414 P
380 453 P
544 591 P
503 394 P
407 552 P
525 433 P
603 808 P
534 512 P
66 276 P
680 630 P
562 512 P
389 532 P
648 236 P
725 473 P
466 453 P
521 374 P
584 532 P
525 512 P
253 414 P
239 276 P
212 512 P
334 512 P
621 689 P
471 473 P
589 315 P
621 414 P
425 473 P
512 512 P
B
35 836 M
35 70 L
757 70 L
757 836 L
35 836 L
E
B
62 176 M
66 184 L
103 240 L
107 247 L
121 268 L
212 395 L
239 424 L
253 437 L
257 441 L
284 462 L
334 481 L
380 483 L
389 483 L
407 481 L
421 480 L
425 480 L
466 479 L
471 479 L
494 480 L
498 480 L
503 480 L
512 480 L
521 480 L
525 480 L
534 480 L
544 479 L
557 479 L
562 479 L
584 478 L
589 478 L
598 478 L
603 478 L
621 479 L
648 479 L
680 482 L
725 485 L
730 485 L
E
1.125 Sx
1 Sd
(smoothing spline) 396 937 0.5 T
1 Sx
0.333333 0.666667 0 0.333333 Sg
0 Sd
1049 453 P
1213 315 P
1290 532 P
1294 591 P
1285 492 P
894 177 P
1390 177 P
1522 473 P
1076 611 P
899 236 P
912 276 P
1031 394 P
1172 453 P
1049 473 P
853 98 P
1349 414 P
1172 453 P
1335 591 P
1294 394 P
1199 552 P
1317 433 P
1394 808 P
1326 512 P
858 276 P
1472 630 P
1353 512 P
1181 532 P
1440 236 P
1517 473 P
1258 453 P
1313 374 P
1376 532 P
1317 512 P
1044 414 P
1031 276 P
1003 512 P
1126 512 P
1413 689 P
1263 473 P
1381 315 P
1413 414 P
1217 473 P
1303 512 P
B
827 836 M
827 70 L
1548 70 L
1548 836 L
827 836 L
E
B
853 150 M
858 161 L
894 245 L
899 255 L
912 281 L
1003 409 L
1031 433 L
1031 433 L
1044 442 L
1049 445 L
1049 445 L
1076 460 L
1126 477 L
1172 482 L
1172 482 L
1181 483 L
1199 483 L
1213 483 L
1217 482 L
1258 480 L
1263 480 L
1285 478 L
1290 478 L
1294 478 L
1294 478 L
1303 478 L
1313 477 L
1317 477 L
1317 477 L
1326 477 L
1335 477 L
1349 477 L
1353 477 L
1376 478 L
1381 478 L
1390 478 L
1394 478 L
1413 479 L
1413 479 L
1440 480 L
1472 483 L
1517 486 L
1522 486 L
E
1.125 Sx
1 Sd
(regression spline) 1188 937 0.5 T
1 Sx
0.666667 1 0 0.333333 Sg
0 Sd
1841 453 P
2004 315 P
2081 532 P
2086 591 P
2077 492 P
1686 177 P
2181 177 P
2313 473 P
1868 611 P
1691 236 P
1704 276 P
1822 394 P
1963 453 P
1841 473 P
1645 98 P
2141 414 P
1963 453 P
2127 591 P
2086 394 P
1991 552 P
2109 433 P
2186 808 P
2118 512 P
1650 276 P
2263 630 P
2145 512 P
1972 532 P
2231 236 P
2309 473 P
2050 453 P
2104 374 P
2168 532 P
2109 512 P
1836 414 P
1822 276 P
1795 512 P
1918 512 P
2204 689 P
2054 473 P
2172 315 P
2204 414 P
2009 473 P
2095 512 P
B
1618 836 M
1618 70 L
2340 70 L
2340 836 L
1618 836 L
E
B
1645 161 M
1650 170 L
1686 235 L
1691 243 L
1704 267 L
1795 404 L
1822 434 L
1822 434 L
1836 447 L
1841 451 L
1841 451 L
1868 469 L
1918 485 L
1963 486 L
1963 486 L
1972 485 L
1991 483 L
2004 481 L
2009 480 L
2050 475 L
2054 474 L
2077 473 L
2081 473 L
2086 473 L
2086 473 L
2095 474 L
2104 475 L
2109 475 L
2109 475 L
2118 476 L
2127 477 L
2141 478 L
2145 479 L
2168 481 L
2172 481 L
2181 482 L
2186 482 L
2204 483 L
2204 483 L
2231 483 L
2263 484 L
2309 483 L
2313 483 L
E
1.125 Sx
1 Sd
(natural spline ) 1979 937 0.5 T
Z

%%Trailer
W
%%Pages: 1
