.if\*(]ZMM DEFINED \{\
.tmERROR: MM package read twice
.ab	\}
.ds]Z MM DEFINED
.if\nT=1 \{.if \nW=0 .nr W 80
.if\nO=0 .nr O 3 \}
.if\nT=2 \{.nr L 84
.dsBU o
.rmul
.rmcu \}
.if!\w'\gE' .nr E 0
.ie\nL .nr L \nLv
.el.nr L 11i
.pl\nLu
.ie\w'\gO' .nr O \nOn
.el.ie \nA .nr O 1i
.el.nr O .75i
.po\nOu
.if\nP .nr P -1
.nrP \nP 1
.if!\nS .nr S 10
.ie\nW .nr W \nWn
.el.ie \nA .nr W 5.8i
.el.nr W 6.0i
.if\n(mo-0 .ds DT January
.if\n(mo-1 .ds DT February
.if\n(mo-2 .ds DT March
.if\n(mo-3 .ds DT April
.if\n(mo-4 .ds DT May
.if\n(mo-5 .ds DT June
.if\n(mo-6 .ds DT July
.if\n(mo-7 .ds DT August
.if\n(mo-8 .ds DT September
.if\n(mo-9 .ds DT October
.if\n(mo-10 .ds DT November
.if\n(mo-11 .ds DT December
.asDT " \n(dy, 19\n(yr
.S D D
'ev0
.)R
.ll\nWu
.lt\nWu
'ev
'ev1
.)R
.ll\nWu
.lt\nWu
'ev
'ev2
.)R
.ll\nWu
.lt\nWu
'ev
.PH "''- \\\\nP -''
.if\nC=3 \{.ds ]C D\ R\ A\ F\ T
.PF "''\\\\*(DT'' \}
.if\nC=4 \{.ds ]C D\ R\ A\ F\ T
.PF "''\\\\*(DT''  
.ls2
.nrPt 1
.nrPi 10 \}
.if\nC=2 .ds ]C ''DATE FILE COPY''
.if\nC=1 .ds ]C ''OFFICIAL FILE COPY''
.if(\nN=3):(\nN=5) \{.nr :S 1
.PF "''\\\\n(H1-\\\\nP''"
.PH \}
.if\nN=4 \{.PH
.nrN 0 \}
.if\nD .PH "'\\*(RE line # \\\\n(.c'- \\\\nP -''
.if\nA=1 \{.nr ;n 1
.ds}2
'br\}
'em)q
