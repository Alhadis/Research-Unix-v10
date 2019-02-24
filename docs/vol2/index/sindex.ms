.so tmac.s
.so ../ADM/mac
.XX index 611 "Index"
.....8.5->7 .911->.75 .303->.25 leaving 7.286
.nr PO .911i
.po .911i
.nr LL 7.286i
.nr LT 6.5i
.nr zz \n(%#u-1u
.de PT
.lt \\n(LTu
.pc %
.nr PN \\n%u+\\n(zzu
.if \\n%-1 .if o \{\
.nr PO .961i
.po .961i\}
.if \\n%-1 .if e \{\
.nr PO .353i
.po .353i\}
.ps +1
.if \\n%-1 .if o \\*(%o
.if \\n%-1 .if e \\*(%e
.ps -1
.lt \\n(.lu
.ps 4
.ft I
.nr PO \\n(PO-.05i
.po \\n(POu
......was -.05 2.412 4.874
.nf
\v'|.9i'\h'|0i'\L'9i'
.sp -1
\v'|.9i'\h'|2.462i'\L'9i'
.sp -1
\v'|.9i'\h'|4.924i'\L'9i'
.sp -1
.ft
.ps \\n(PS
.nr PO \\n(PO+.05i
.po \\n(POu
..
.de BT
.nr LT 6.5i
.nr PN \\n%u+\\n(zzu
.nr PX \\n(.s
.nr PF \\n(.f
.ft 1
.ps \\n(PS
'lt \\n(LTu
.po \\n(POu
.ps +1
.if o \\*(%O
.if e \\*(%E
.ps -1
.ft \\n(PF
.ps \\n(PX
..
.TL
Subject Index
.LP
.ps 9
.vs 9
.nr PS 9
.nr VS 9
......was 2.362 & .1i
.MC 2.312i .15i
.na
.de XX
.br
.ti -.1i
.ne 2
..
.de XY
.br
.ne 2
..
.de YY
.sp
..
.hy 0
.rm hy
.nf
.ps 4
\v'|2.3i'\h'|0i'\L'7.6i'
.sp -1
\v'|2.3i'\h'|2.462i'\L'7.6i'
.sp -1
\v'|2.3i'\h'|4.924i'\L'7.6i'
.sp -1
.nr PO \n(PO+.05i
.po \n(POu
.ps 9
.ft R
.so sindex
.ft R
.ne 4i
.so ../contents/pico_list
