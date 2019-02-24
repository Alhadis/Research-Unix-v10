%!PS-Adobe-1.0
%%Title: S QPE graphics
%%Creator: Trevor Hastie
%%CreationDate: Mon Mar  5 17:07:32 1990
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
538 127 M
538 1156 L
1262 1156 L
1262 127 L
F
1 Sc
67 842 P
70 608 P
71 705 P
74 717 P
119 551 P
126 510 P
130 533 P
143 555 P
161 688 P
164 797 P
164 476 P
169 687 P
187 563 P
190 417 P
190 571 P
191 522 P
193 519 P
194 376 P
198 352 P
199 383 P
202 333 P
203 550 P
211 530 P
216 629 P
228 526 P
240 441 P
246 504 P
248 288 P
259 430 P
282 613 P
284 643 P
297 414 P
307 490 P
320 517 P
340 258 P
342 416 P
357 440 P
360 279 P
361 495 P
362 329 P
366 497 P
367 395 P
372 450 P
378 443 P
382 421 P
386 466 P
392 170 P
404 413 P
415 582 P
426 256 P
428 417 P
429 244 P
432 462 P
433 457 P
444 401 P
470 488 P
471 561 P
472 378 P
494 407 P
498 335 P
512 408 P
515 436 P
527 361 P
532 328 P
548 284 P
548 501 P
550 349 P
571 403 P
576 457 P
584 361 P
590 413 P
618 389 P
637 535 P
644 517 P
663 344 P
671 426 P
706 660 P
708 500 P
718 523 P
722 385 P
729 651 P
739 631 P
743 491 P
755 445 P
757 581 P
761 653 P
770 718 P
772 592 P
787 513 P
792 630 P
807 686 P
807 607 P
809 687 P
822 630 P
835 541 P
835 548 P
837 833 P
863 634 P
868 560 P
888 698 P
891 702 P
900 710 P
901 835 P
912 774 P
922 804 P
933 585 P
938 739 P
940 681 P
943 589 P
959 726 P
962 834 P
980 910 P
992 686 P
999 521 P
999 807 P
1008 630 P
1009 857 P
1023 767 P
1042 878 P
1066 789 P
1078 637 P
1079 756 P
1099 1015 P
1118 756 P
1125 813 P
1128 803 P
1128 824 P
1131 719 P
1142 1007 P
1147 971 P
1163 985 P
1184 901 P
1201 952 P
1215 957 P
1235 838 P
1236 858 P
1251 799 P
1256 848 P
1261 1006 P
1294 850 P
1304 1048 P
1305 856 P
1306 1097 P
1313 944 P
1337 982 P
1338 835 P
1339 971 P
1343 959 P
1347 1006 P
1349 1030 P
1372 1156 P
1377 1021 P
1378 940 P
1382 813 P
1385 1013 P
1390 869 P
1397 1093 P
1412 944 P
1413 980 P
1429 922 P
1432 935 P
1438 925 P
1439 1045 P
1441 1034 P
1450 835 P
1479 909 P
1489 924 P
1492 1028 P
1492 1009 P
1510 676 P
1518 879 P
1522 1073 P
1523 693 P
1527 917 P
1543 809 P
1549 1061 P
1555 860 P
1558 988 P
1572 880 P
1577 680 P
1590 823 P
1593 884 P
1603 849 P
1606 741 P
1622 1109 P
1623 757 P
1631 764 P
1635 872 P
1679 790 P
1684 830 P
1684 828 P
1689 767 P
1690 712 P
1693 831 P
1717 856 P
1726 911 P
1726 617 P
1728 660 P
1730 881 P
1733 590 P
1.5 Sw
B
67 665 M
70 660 L
71 659 L
74 655 L
119 601 L
126 594 L
130 590 L
143 576 L
161 560 L
164 557 L
164 557 L
169 553 L
187 537 L
190 535 L
190 535 L
191 534 L
193 532 L
194 531 L
198 528 L
199 527 L
202 525 L
203 525 L
211 519 L
216 515 L
228 507 L
240 499 L
246 495 L
248 493 L
259 487 L
282 474 L
284 473 L
297 466 L
307 461 L
320 455 L
340 447 L
342 446 L
357 441 L
360 440 L
361 439 L
362 439 L
366 438 L
367 437 L
372 436 L
378 434 L
382 433 L
386 432 L
392 431 L
404 428 L
415 427 L
426 426 L
428 425 L
429 425 L
432 425 L
433 425 L
444 425 L
470 426 L
471 426 L
472 426 L
494 429 L
498 430 L
512 433 L
515 434 L
527 438 L
532 440 L
548 445 L
548 445 L
550 446 L
571 455 L
576 458 L
584 461 L
590 464 L
618 479 L
637 490 L
644 495 L
663 507 L
671 512 L
706 536 L
708 538 L
718 545 L
722 548 L
729 554 L
739 561 L
743 564 L
755 573 L
757 575 L
761 579 L
770 585 L
772 587 L
787 599 L
792 602 L
807 615 L
807 615 L
809 616 L
822 627 L
835 636 L
835 637 L
837 638 L
863 658 L
868 662 L
888 677 L
891 679 L
900 686 L
901 686 L
912 694 L
922 701 L
933 709 L
938 713 L
940 714 L
943 716 L
959 727 L
962 729 L
980 742 L
992 750 L
999 754 L
999 754 L
1008 760 L
1009 761 L
1023 771 L
1042 783 L
1066 798 L
1078 806 L
1079 806 L
1099 819 L
1118 831 L
1125 835 L
1128 837 L
1128 837 L
1131 839 L
1142 845 L
1147 849 L
1163 858 L
1184 869 L
1201 878 L
1215 885 L
1235 895 L
1236 896 L
1251 902 L
1256 905 L
1261 907 L
1294 919 L
1304 922 L
1305 922 L
1306 922 L
1313 924 L
1337 929 L
1338 929 L
1339 930 L
1343 930 L
1347 931 L
1349 931 L
1372 932 L
1377 932 L
1378 932 L
1382 932 L
1385 932 L
1390 932 L
1397 931 L
1412 929 L
1413 929 L
1429 926 L
1432 925 L
1438 923 L
1439 923 L
1441 922 L
1450 920 L
1479 910 L
1489 905 L
1492 904 L
1492 904 L
1510 896 L
1518 892 L
1522 890 L
1523 889 L
1527 888 L
1543 879 L
1549 876 L
1555 873 L
1558 871 L
1572 863 L
1577 860 L
1590 853 L
1593 851 L
1603 845 L
1606 843 L
1622 833 L
1623 832 L
1631 827 L
1635 824 L
1679 796 L
1684 792 L
1684 792 L
1689 789 L
1690 788 L
1693 786 L
1717 769 L
1726 763 L
1726 762 L
1728 761 L
1730 759 L
1733 757 L
E
1 Sw
B
67 127 M
70 127 L
71 127 L
74 127 L
119 127 L
126 127 L
130 127 L
143 127 L
161 127 L
164 127 L
164 127 L
169 127 L
187 127 L
190 127 L
190 127 L
191 127 L
193 127 L
194 127 L
198 127 L
199 127 L
202 127 L
203 127 L
211 127 L
216 127 L
228 127 L
240 127 L
246 127 L
248 127 L
259 127 L
282 127 L
284 127 L
297 127 L
307 127 L
320 127 L
340 127 L
342 127 L
357 127 L
360 127 L
361 127 L
362 127 L
366 127 L
367 127 L
372 127 L
378 127 L
382 127 L
386 127 L
392 127 L
404 127 L
415 128 L
426 128 L
428 128 L
429 128 L
432 128 L
433 128 L
444 128 L
470 130 L
471 130 L
472 130 L
494 132 L
498 132 L
512 134 L
515 134 L
527 136 L
532 137 L
548 140 L
548 140 L
550 141 L
571 147 L
576 149 L
584 152 L
590 155 L
618 169 L
637 182 L
644 188 L
663 204 L
671 212 L
706 251 L
708 254 L
718 267 L
722 272 L
729 282 L
739 296 L
743 302 L
755 318 L
757 321 L
761 328 L
770 340 L
772 343 L
787 364 L
792 370 L
807 390 L
807 390 L
809 392 L
822 408 L
835 422 L
835 422 L
837 424 L
863 445 L
868 447 L
888 454 L
891 455 L
900 456 L
901 456 L
912 454 L
922 451 L
933 446 L
938 443 L
940 442 L
943 440 L
959 427 L
962 425 L
980 405 L
992 391 L
999 382 L
999 382 L
1008 371 L
1009 368 L
1023 349 L
1042 322 L
1066 288 L
1078 271 L
1079 271 L
1099 245 L
1118 223 L
1125 216 L
1128 213 L
1128 213 L
1131 210 L
1142 200 L
1147 195 L
1163 182 L
1184 168 L
1201 159 L
1215 152 L
1235 145 L
1236 145 L
1251 141 L
1256 139 L
1261 138 L
1294 133 L
1304 132 L
1305 132 L
1306 132 L
1313 131 L
1337 129 L
1338 129 L
1339 129 L
1343 129 L
1347 129 L
1349 129 L
1372 128 L
1377 128 L
1378 128 L
1382 128 L
1385 128 L
1390 128 L
1397 127 L
1412 127 L
1413 127 L
1429 127 L
1432 127 L
1438 127 L
1439 127 L
1441 127 L
1450 127 L
1479 127 L
1489 127 L
1492 127 L
1492 127 L
1510 127 L
1518 127 L
1522 127 L
1523 127 L
1527 127 L
1543 127 L
1549 127 L
1555 127 L
1558 127 L
1572 127 L
1577 127 L
1590 127 L
1593 127 L
1603 127 L
1606 127 L
1622 127 L
1623 127 L
1631 127 L
1635 127 L
1679 127 L
1684 127 L
1684 127 L
1689 127 L
1690 127 L
1693 127 L
1717 127 L
1726 127 L
1726 127 L
1728 127 L
1730 127 L
1733 127 L
E
B
900 127 M
900 183 L
914 169 L
E
B
886 169 M
900 183 L
E
Z

%%Trailer
W
%%Pages: 1
