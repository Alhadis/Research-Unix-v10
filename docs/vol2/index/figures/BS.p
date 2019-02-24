%!PS-Adobe-1.0
%%Title: S QPE graphics
%%Creator: Trevor Hastie
%%CreationDate: Wed Aug 30 12:08:17 1989
%%Pages: (atend)
%%BoundingBox: 20.8788 108.12 590.881 684.12
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
/Region [20.8788 108.12 590.881 684.12] def
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
46 Sp
1 Sx
0.0589471 0.941053 0.175 0.941667 So
0 0.5 0.5 1 Sg
0 Sh
0 Sd
1 Sf
109 1780 P
185 1709 P
222 1726 P
235 1596 P
356 1591 P
368 1559 P
373 1774 P
397 1615 P
406 1711 P
413 1627 P
416 1553 P
421 1444 P
442 1617 P
444 1505 P
447 1499 P
468 1686 P
490 1718 P
517 1655 P
523 1594 P
525 1684 P
535 1768 P
537 1779 P
547 1596 P
563 1908 P
566 1729 P
568 1736 P
580 1696 P
588 1717 P
593 1803 P
593 1779 P
594 1690 P
595 1734 P
598 1745 P
598 1690 P
604 1896 P
613 1744 P
615 1732 P
615 1827 P
619 1751 P
630 1837 P
635 1889 P
637 1719 P
638 1761 P
639 1853 P
639 1863 P
647 1934 P
663 1878 P
665 1889 P
671 1889 P
671 1893 P
677 1871 P
681 1939 P
684 1898 P
686 1898 P
699 1928 P
703 2095 P
710 1921 P
711 2009 P
723 1953 P
724 2045 P
725 2080 P
731 1995 P
736 1995 P
745 1922 P
748 2089 P
759 2037 P
765 2151 P
766 1968 P
770 2041 P
770 2061 P
781 2002 P
800 2188 P
801 2138 P
804 2190 P
813 2071 P
822 2296 P
826 2074 P
828 1966 P
831 2116 P
857 2263 P
862 2028 P
864 2212 P
869 2126 P
872 2107 P
887 2215 P
911 2081 P
916 2183 P
918 2100 P
921 2147 P
925 2246 P
952 2056 P
961 2272 P
977 2110 P
989 2131 P
1002 2161 P
1008 2201 P
1020 2170 P
1025 2253 P
1045 2086 P
1079 2004 P
B
70 2330 M
70 1410 L
1118 1410 L
1118 2330 L
70 2330 L
E
4 St
487 1410 487 2330 S
856 1410 856 2330 S
1 St
B
109 1778 M
185 1704 L
222 1679 L
235 1671 L
356 1619 L
368 1614 L
373 1612 L
397 1603 L
406 1600 L
413 1597 L
416 1596 L
421 1594 L
442 1585 L
444 1584 L
447 1583 L
468 1572 L
E
B
490 1699 M
517 1692 L
523 1692 L
525 1693 L
535 1696 L
537 1698 L
547 1704 L
563 1718 L
566 1721 L
568 1723 L
580 1738 L
588 1747 L
593 1756 L
593 1756 L
594 1757 L
595 1758 L
598 1762 L
598 1763 L
604 1772 L
613 1786 L
615 1789 L
615 1790 L
619 1797 L
630 1817 L
635 1825 L
637 1829 L
638 1832 L
639 1834 L
639 1834 L
647 1848 L
663 1880 L
665 1884 L
671 1895 L
671 1895 L
677 1907 L
681 1915 L
684 1922 L
686 1925 L
699 1950 L
703 1958 L
710 1972 L
711 1973 L
723 1995 L
724 1998 L
725 2000 L
731 2010 L
736 2019 L
745 2034 L
748 2039 L
759 2056 L
765 2065 L
766 2065 L
770 2070 L
770 2072 L
781 2085 L
800 2104 L
801 2105 L
804 2107 L
813 2113 L
822 2117 L
826 2118 L
828 2119 L
831 2120 L
E
B
857 2171 M
862 2164 L
864 2160 L
869 2153 L
872 2150 L
887 2140 L
911 2139 L
916 2141 L
918 2142 L
921 2143 L
925 2145 L
952 2163 L
961 2168 L
977 2176 L
989 2179 L
1002 2178 L
1008 2176 L
1020 2167 L
1025 2161 L
1045 2125 L
1079 2008 L
E
1 Sd
(\(a\) discontinuous) 594 1305 0.5 T
0.5 1 0.5 1 Sg
0 Sd
1296 1780 P
1373 1709 P
1410 1726 P
1423 1596 P
1544 1591 P
1556 1559 P
1561 1774 P
1585 1615 P
1593 1711 P
1601 1627 P
1604 1553 P
1608 1444 P
1629 1617 P
1632 1505 P
1634 1499 P
1655 1686 P
1677 1718 P
1705 1655 P
1710 1594 P
1713 1684 P
1722 1768 P
1725 1779 P
1734 1596 P
1751 1908 P
1754 1729 P
1756 1736 P
1768 1696 P
1775 1717 P
1781 1803 P
1781 1779 P
1782 1690 P
1783 1734 P
1785 1745 P
1786 1690 P
1791 1896 P
1800 1744 P
1802 1732 P
1803 1827 P
1806 1751 P
1818 1837 P
1822 1889 P
1824 1719 P
1826 1761 P
1827 1853 P
1827 1863 P
1835 1934 P
1851 1878 P
1853 1889 P
1859 1889 P
1859 1893 P
1864 1871 P
1868 1939 P
1872 1898 P
1873 1898 P
1886 1928 P
1891 2095 P
1898 1921 P
1898 2009 P
1910 1953 P
1912 2045 P
1913 2080 P
1918 1995 P
1924 1995 P
1932 1922 P
1936 2089 P
1947 2037 P
1953 2151 P
1953 1968 P
1957 2041 P
1958 2061 P
1969 2002 P
1988 2188 P
1989 2138 P
1992 2190 P
2001 2071 P
2009 2296 P
2013 2074 P
2015 1966 P
2018 2116 P
2045 2263 P
2049 2028 P
2051 2212 P
2057 2126 P
2060 2107 P
2075 2215 P
2099 2081 P
2103 2183 P
2105 2100 P
2109 2147 P
2113 2246 P
2140 2056 P
2149 2272 P
2164 2110 P
2177 2131 P
2189 2161 P
2196 2201 P
2208 2170 P
2213 2253 P
2233 2086 P
2266 2004 P
B
1258 2330 M
1258 1410 L
2305 1410 L
2305 2330 L
1258 2330 L
E
4 St
1674 1410 1674 2330 S
2043 1410 2043 2330 S
1 St
B
1296 1768 M
1373 1719 L
1410 1688 L
1423 1676 L
1544 1597 L
1556 1594 L
1561 1593 L
1585 1592 L
1593 1594 L
1601 1595 L
1604 1596 L
1608 1598 L
1629 1609 L
1632 1610 L
1634 1612 L
1655 1630 L
E
B
1677 1654 M
1705 1669 L
1710 1674 L
1713 1676 L
1722 1685 L
1725 1688 L
1734 1698 L
1751 1718 L
1754 1722 L
1756 1724 L
1768 1741 L
1775 1752 L
1781 1760 L
1781 1761 L
1782 1762 L
1783 1763 L
1785 1767 L
1786 1768 L
1791 1777 L
1800 1791 L
1802 1795 L
1803 1796 L
1806 1802 L
1818 1822 L
1822 1829 L
1824 1833 L
1826 1836 L
1827 1838 L
1827 1838 L
1835 1851 L
1851 1881 L
1853 1884 L
1859 1895 L
1859 1895 L
1864 1906 L
1868 1913 L
1872 1920 L
1873 1923 L
1886 1947 L
1891 1954 L
1898 1967 L
1898 1968 L
1910 1989 L
1912 1992 L
1913 1994 L
1918 2003 L
1924 2012 L
1932 2027 L
1936 2033 L
1947 2050 L
1953 2060 L
1953 2061 L
1957 2066 L
1958 2067 L
1969 2083 L
1988 2108 L
1989 2109 L
1992 2113 L
2001 2123 L
2009 2132 L
2013 2136 L
2015 2138 L
2018 2140 L
E
B
2045 2156 M
2049 2151 L
2051 2149 L
2057 2144 L
2060 2142 L
2075 2137 L
2099 2141 L
2103 2143 L
2105 2144 L
2109 2146 L
2113 2148 L
2140 2165 L
2149 2171 L
2164 2177 L
2177 2179 L
2189 2177 L
2196 2174 L
2208 2164 L
2213 2159 L
2233 2123 L
2266 2011 L
E
1 Sd
(\(b\) continuous) 1781 1305 0.5 T
0 0.5 0 0.5 Sg
0 Sd
109 580 P
185 509 P
222 526 P
235 396 P
356 391 P
368 359 P
373 574 P
397 415 P
406 511 P
413 427 P
416 353 P
421 244 P
442 417 P
444 305 P
447 299 P
468 486 P
490 518 P
517 455 P
523 394 P
525 484 P
535 568 P
537 579 P
547 396 P
563 708 P
566 529 P
568 536 P
580 496 P
588 517 P
593 603 P
593 579 P
594 490 P
595 534 P
598 545 P
598 490 P
604 696 P
613 544 P
615 532 P
615 627 P
619 551 P
630 637 P
635 689 P
637 519 P
638 561 P
639 653 P
639 663 P
647 734 P
663 678 P
665 689 P
671 689 P
671 693 P
677 671 P
681 739 P
684 698 P
686 698 P
699 728 P
703 895 P
710 721 P
711 809 P
723 753 P
724 845 P
725 880 P
731 795 P
736 795 P
745 722 P
748 889 P
759 837 P
765 951 P
766 768 P
770 841 P
770 861 P
781 802 P
800 988 P
801 938 P
804 990 P
813 871 P
822 1096 P
826 874 P
828 766 P
831 916 P
857 1063 P
862 828 P
864 1012 P
869 926 P
872 907 P
887 1015 P
911 881 P
916 983 P
918 900 P
921 947 P
925 1046 P
952 856 P
961 1072 P
977 910 P
989 931 P
1002 961 P
1008 1001 P
1020 970 P
1025 1053 P
1045 886 P
1079 804 P
B
70 1130 M
70 210 L
1118 210 L
1118 1130 L
70 1130 L
E
4 St
487 210 487 1130 S
856 210 856 1130 S
1 St
B
109 576 M
185 509 L
222 479 L
235 470 L
356 407 L
368 405 L
373 404 L
397 402 L
406 402 L
413 403 L
416 403 L
421 404 L
442 408 L
444 409 L
447 410 L
468 418 L
E
B
490 431 M
517 454 L
523 460 L
525 463 L
535 474 L
537 477 L
547 489 L
563 513 L
566 518 L
568 520 L
580 539 L
588 551 L
593 561 L
593 561 L
594 562 L
595 564 L
598 568 L
598 569 L
604 579 L
613 594 L
615 598 L
615 599 L
619 605 L
630 626 L
635 634 L
637 638 L
638 641 L
639 643 L
639 643 L
647 657 L
663 687 L
665 690 L
671 701 L
671 701 L
677 712 L
681 720 L
684 726 L
686 729 L
699 752 L
703 760 L
710 772 L
711 773 L
723 793 L
724 796 L
725 798 L
731 807 L
736 815 L
745 829 L
748 835 L
759 850 L
765 859 L
766 860 L
770 865 L
770 866 L
781 880 L
800 900 L
801 902 L
804 904 L
813 912 L
822 919 L
826 922 L
828 923 L
831 925 L
E
B
857 935 M
862 936 L
864 937 L
869 939 L
872 940 L
887 947 L
911 959 L
916 961 L
918 962 L
921 964 L
925 966 L
952 976 L
961 978 L
977 978 L
989 975 L
1002 969 L
1008 965 L
1020 953 L
1025 947 L
1045 915 L
1079 829 L
E
1 Sd
(\(c\) continuous f') 594 105 0.5 T
0.5 1 0 0.5 Sg
0 Sd
1296 580 P
1373 509 P
1410 526 P
1423 396 P
1544 391 P
1556 359 P
1561 574 P
1585 415 P
1593 511 P
1601 427 P
1604 353 P
1608 244 P
1629 417 P
1632 305 P
1634 299 P
1655 486 P
1677 518 P
1705 455 P
1710 394 P
1713 484 P
1722 568 P
1725 579 P
1734 396 P
1751 708 P
1754 529 P
1756 536 P
1768 496 P
1775 517 P
1781 603 P
1781 579 P
1782 490 P
1783 534 P
1785 545 P
1786 490 P
1791 696 P
1800 544 P
1802 532 P
1803 627 P
1806 551 P
1818 637 P
1822 689 P
1824 519 P
1826 561 P
1827 653 P
1827 663 P
1835 734 P
1851 678 P
1853 689 P
1859 689 P
1859 693 P
1864 671 P
1868 739 P
1872 698 P
1873 698 P
1886 728 P
1891 895 P
1898 721 P
1898 809 P
1910 753 P
1912 845 P
1913 880 P
1918 795 P
1924 795 P
1932 722 P
1936 889 P
1947 837 P
1953 951 P
1953 768 P
1957 841 P
1958 861 P
1969 802 P
1988 988 P
1989 938 P
1992 990 P
2001 871 P
2009 1096 P
2013 874 P
2015 766 P
2018 916 P
2045 1063 P
2049 828 P
2051 1012 P
2057 926 P
2060 907 P
2075 1015 P
2099 881 P
2103 983 P
2105 900 P
2109 947 P
2113 1046 P
2140 856 P
2149 1072 P
2164 910 P
2177 931 P
2189 961 P
2196 1001 P
2208 970 P
2213 1053 P
2233 886 P
2266 804 P
B
1258 1130 M
1258 210 L
2305 210 L
2305 1130 L
1258 1130 L
E
4 St
1674 210 1674 1130 S
2043 210 2043 1130 S
1 St
B
1296 575 M
1373 511 L
1410 481 L
1423 471 L
1544 403 L
1556 401 L
1561 400 L
1585 398 L
1593 398 L
1601 399 L
1604 400 L
1608 400 L
1629 406 L
1632 407 L
1634 408 L
1655 419 L
E
B
1677 435 M
1705 462 L
1710 467 L
1713 470 L
1722 482 L
1725 485 L
1734 497 L
1751 520 L
1754 525 L
1756 527 L
1768 545 L
1775 556 L
1781 565 L
1781 565 L
1782 567 L
1783 568 L
1785 572 L
1786 573 L
1791 582 L
1800 596 L
1802 600 L
1803 601 L
1806 607 L
1818 626 L
1822 634 L
1824 637 L
1826 640 L
1827 641 L
1827 641 L
1835 655 L
1851 683 L
1853 686 L
1859 696 L
1859 696 L
1864 706 L
1868 713 L
1872 719 L
1873 722 L
1886 744 L
1891 751 L
1898 763 L
1898 765 L
1910 784 L
1912 787 L
1913 789 L
1918 797 L
1924 806 L
1932 820 L
1936 825 L
1947 841 L
1953 851 L
1953 851 L
1957 857 L
1958 858 L
1969 873 L
1988 897 L
1989 899 L
1992 902 L
2001 913 L
2009 922 L
2013 926 L
2015 928 L
2018 931 L
E
B
2045 953 M
2049 956 L
2051 957 L
2057 961 L
2060 962 L
2075 969 L
2099 975 L
2103 976 L
2105 976 L
2109 976 L
2113 976 L
2140 973 L
2149 970 L
2164 963 L
2177 956 L
2189 948 L
2196 943 L
2208 933 L
2213 929 L
2233 908 L
2266 867 L
E
1 Sd
(\(d\) continuous f'') 1781 105 0.5 T
Z

%%Trailer
W
%%Pages: 1
