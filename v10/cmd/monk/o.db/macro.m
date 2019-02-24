|include(macro.h)

|include(style.h)

|macro(.top.of.page;
	[
'\"	#T used by tbl to draw boxes ?????
'\"	:2 store for multi-page table heading
'\"	:A multi-page table heading being used
'\"	:T top of table printed
'\"	:V vertical spacing
'\"	:d prevent )h from outputting a floating group
'\"	:m initial footer place and partial footnote diverter trap
'\"	:o footer trap location
'\"	:q amount of footnote text
'\"	:r position at end of page header macro
'\"	:v width of stored footnote 0=narrow, 1=wide
'\"	:w output counter for floating keeps
'\"	:z input counter for floating keeps
'\"	Un position to return to for column n
'\"	W current page width
'\"	Wn page width for column n
'\"	;i remembered indent TEMP
'\"	;o prevailing page offset ??????? bug with .po
'\"	;t copy of :w TEMP
'\"	;w convert to alpha format TEMP
.		de )h
.		ev 2					\" use header environment
.		ll \\nWu				\" use full page
.		lt \\nWu
.		ie "\*(.T"aps" \{\
.			nr ;o \\n(.o			\" save prevailing page offset
.			po 0				\" always from left margin
.			lt 7.5i				\" width of photo composer
.			ps 10				\" 10 point roman font
.			vs 12p
.			ft 1
.			tl '--''--'			\" output the cut marks
.			ft				\" restore values
.			vs
.			ps
.			lt
.			po \\n(;ou \}
.		el \{\
.			ps 10				\" 10 point roman font
.			vs 12p
.			ft 1
.			tl ''''				\" output nothing
.			ft				\" restore values
.			vs
.			ps \}
.		nr page_counter +1			\" incr page number
.		if \\n(column_counter \{\
.			ll \\n(W0u			\" go wide
.			lt \\n(W0u \}
.		TP					\" user-redefinable macro
.		if \\n(column_counter \{\
.			ll \\nWu			\" go narrow
.			lt \\nWu \}
.		br					\" force a break in the header environment
.		ev					\" back to main environment
.		ch )n -\\n(:mu				\" reset partial footnote diverter
.		ch )f -\\n(:ou				\" reset footer trap
.		if !\\n(:q .nr :v 0			\" no saved footnotes, thus narrow
.		nr :V \\n(.v				\" for footnote uncertainty correction
.		mk :r					\" position at end of page header macro
.		mk U0					\" save where all columns start
.		rr U1 U2 U3 U4 U5 U6 U7 U8 U9		\" zero column starts
'\"	output floating displays
.		if \\n(:d=0&((\\n(:z-\\n(:w):(\\n(:w-\\n(:z)) \{\
.			nr ;t \\n(:w%26+1		\" get index of next output element
.			af ;w a				\" conversion register for display queue names
.			nr ;w \\n(;t			\" form the next queue element name
.			ie \\n(!\\n(;w \{\
.				)z			\" output at least one floating display
.				)s			\" but as many as will fit
.				mk U0			\" save where all columns start
.				rr U1 U2 U3 U4 U5 U6 U7 U8 U9 \} \" zero column starts
.			el \{\
.				)z			\" output at least one floating display
.				)s \}			\" but as many as will fit
.			rr ;t ;w \}			\" free storage
'\"	print top part of multi-page table
.		nr :T 0					\" mark multi-page table header not printed
.		if \\n(:A>0 \{\
.			nr ;i \\n(.i			\" save prevailing indent
.			in 0				\" indent back
.			:2				\" heading
.			nr :T 1				\" mark multi-page table header printed
.			in \\n(;iu \}			\" restore prevailing indent
.		mk #T					\" mark spot for table drawing ???????
.		rr ;i ;o ;t ;w				\" free storage
.		ns					\" avoid .sp in user text here
..
])

|macro(top.of.page;
	[
'\"	;P page number register TEMP
.		de TP
'		sp
.		af ;P \\g+Q+-Q-page_counter		\" save format of page_counter
.		af page_counter 1			\" normal format for next line
.		nr ;P \\n page_counter			\" must use different name for P
.		af page_counter \\g(;P			\" restore format to P
.		af ;P 1					\" normal format for control register
'		sp 2
.		if \\n(;P-1 .tl \\*(page_header		\" output headers
.		if !\\n(;P%2 .tl \\*(even_header
.		if \\n(;P%2 .tl \\*(odd_header
'		sp 2
.		if \\n(;P=4 .if "\\*(format_type"model" \
.			pl 13.25i			\" model format kludge
.		rr ;P					\" free storage
..
])

|macro(.bottom.of.page;
	[
'\"	!{a-z} width of stored floating 0=narrow, 1=wide
'\"	#T used by tbl to draw boxes ?????
'\"	:2 store for multi-page table heading
'\"	:A multi-page table heading being used
'\"	:K position for )f trap
'\"	:M initial footer and partial footnote diverter for full output page
'\"	:T top of table printed
'\"	:V vertical spacing
'\"	:c number of columns
'\"	:d prevent )h from outputting a floating group
'\"	:f bottom of table printed
'\"	:m initial footer place and partial footnote diverter trap
'\"	:o footer trap location
'\"	:q amount of footnote text
'\"	:v width of stored footnote 0=narrow, 1=wide
'\"	:w output counter for floating keeps
'\"	:z input counter for floating keeps
'\"	O current page offset
'\"	On page offset for column n
'\"	Un position to return to for column n
'\"	W current page width
'\"	Wn page width for column n
'\"	T# used by tbl to draw boxes ?????
'\"	T. ?????
'\"	;d delta width TEMP
'\"	;i remembered indent TEMP
'\"	;t copy of :w TEMP
'\"	;u hidden def of )f TEMP
'\"	;w convert to alpha format TEMP
'\"	;y room left on the page TEMP
.		de )f
'\"	print bottom part of boxed multi-page table
.		if \\n(:A \{\
.			rn )f ;u			\" hide footer
.			nr T. 1
.			if \\n(:f=0 .T# 1		\" print multi-page table footer
'			br				\" output new page
.			nr :f 1				\" remember footer has been printed
.			rn ;u )f \}			\" restore footer
.		nr ;y \\n(.pu-\\n(nlu-\\n(:mu		\" compute amount of room left on the page
.		if \\n(:qu>1v .nr ;y -1v		\" output a reasonable amount
'\"	if footnotes and there is room left and full page or left column output,
'\"	or narrow footnotes, call footnote expander
.		ie \\n(:q&\\n(;y&((\\n(column_counter<2):(\\n(:v=0)) .)o
.		el \{\
.			nr :o \\n(:m+\\n(:q		\" reset footer trap location
.			if \\n(:q \
.				nr :o +\\n(:Vu-1u	\" traps spring once we go past
.			ch )n 15i			\" hide partial footnote diverter
.			ch )f 15i \}			\" move footer over it
.		ie \\n(column_counter=0 'bp		\" new page if not multi-column
'\"	end last column
.		el .ie \\n(column_counter=\\n(:c \{\
.			nr ;d \\n(.lu-\\n(W\\n(column_counter+Q+-Q-u \" quotations shrink line length
.			nr column_counter 1		\" set column indicator
.			nr O \\n(O1u			\" restore first column offset
.			po \\nOu			\" set first column offset
.			nr W \\n(W1u+\\n(;du		\" restore first column width
.			ll \\nWu			\" set first column line length
.			lt \\nWu			\" set first column title length
.			nr :m \\n(:M			\" set initial footer place and partial footnote diverter trap
'			bp				\" force next page
.			rr ;d \}			\" free storage
'\"	end other columns
.		    el \{\
.			if \\n(column_counter=1 .if "\\*(format_type"acm" \
.				pl 10.875i			\" acm format kludge
.			if \\n(nl>\\n(U\\n(column_counter \
.				mk U\\n(column_counter	\" new vertical position
.			nr ;d \\n(.lu-\\n(W\\n(column_counter+Q+-Q-u \" quotations shrink line length
.			nr column_counter +1		\" set column indicator
.			ie \\n(U\\n(column_counter+Q+-Q-u \
'				sp |\\n(U\\n(column_counter+Q+-Q-u \" restore vertical position
.			el 'sp |\\n(U0u			\" default top if not set
.			ch )n -\\n(:mu			\" reset partial footnote diverter
.			ch )f -\\n(:ou			\" reset footer trap
.			if !\\n(:q .nr :v 0		\" no saved footnotes, thus narrow
.			nr O \\n(O\\n(column_counter+Q+-Q-u \" restore new offset
.			po \\nOu			\" set new offset
.			nr W \\n(W\\n(column_counter+Q+-Q-u+\\n(;du \" restore new width
.			ll \\nWu			\" set new line length
.			lt \\nWu			\" set new title length
'\"	output floating displays that are narrow
.			if \\n(:d=0&((\\n(:z-\\n(:w):(\\n(:w-\\n(:z)) \{\
.				nr ;t \\n(:w%26+1	\" get index of next output element
.				af ;w a			\" conversion register for display queue names
.				nr ;w \\n(;t		\" form the next queue element name
'\"	inhibit if wide display
.				if !\\n(!\\n(;w \{\
.					)z		\" output at least one floating display
.					)s \}		\" but as many as will fit
.				rr ;t ;w \}		\" free storage
'\"	print top part of multi-page table
.			nr :T 0				\" mark multi-page table header not printed
.			if \\n(:A>0 \{\
.				nr ;i \\n(.i		\" save prevailing indent
.				in 0			\" indent back
.				:2			\" heading
.				nr :T 1			\" mark multi-page table header printed
.				in \\n(;iu \}		\" restore prevailing indent
.			mk #T				\" mark spot for table drawing ???????
.			rr ;d ;i ;t ;w			\" free storage
.			ns \}				\" avoid .sp in user text here
.		if \\n(:A \{\
.			nr :K \\n(:ou+2v		\" position for )f trap
.			ch >f -(\\n(:Ku+1v)		\" when moved by >f macro
'			br \}
.		rr ;y					\" free storage
..
])

|macro(bottom.of.page;
	[
'\"	;P page number register TEMP
.		de BP
.		ev 2					\" use header environment
.		if \\n(column_counter \{\
.			ll \\n(W0u			\" go wide
.			lt \\n(W0u \}
'		sp
.		af ;P \\g+Q+-Q-page_counter		\" save format of page_counter
.		af page_counter 1			\" normal format for next line
.		nr ;P \\n page_counter			\" must use different name for P
.		af page_counter \\g(;P			\" restore format to P
.		af ;P 1					\" normal format for control register
.		if !\\n(;P%2 .tl \\*(even_footer
.		if \\n(;P%2 .tl \\*(odd_footer
.		tl \\*(page_footer			\" output footers
.		if \\n(column_counter \{\
.			ll \\nWu			\" go narrow
.			lt \\nWu \}
.		ev					\" back to main environment
.		rr ;P					\" free storage
..
])

|macro(.float.output;
	[
'\"	!{a-z} width of stored floating 0=narrow, 1=wide
'\"	:d prevent )h from outputting a floating group
'\"	:w output counter for floating keeps
'\"	:z input counter for floating keeps
'\"	?{a-z} text of stored floating
'\"	|{a-z} size of stored floating
'\"	;L remembered line spacing TEMP
'\"	;i remembered indent TEMP
'\"	;q remembered fill/no-fill mode TEMP
'\"	;w convert to alpha format TEMP
.		de )z
.		if \\n(:z=\\n(:w .tm no floats to output
.		nr :w \\n(:w%26+1			\" get index of next output element
.		af ;w a					\" conversion register for display queue names
.		nr ;w \\n(:w				\" form the next queue element name
.		nr ;q \\n(.u				\" save prevailing fill/no-fill mode
.		nr ;L \\n(.L				\" save line spacing
.		nr ;i \\n(.i				\" save prevailing indent
.		ev 1					\" use footnote environment
.		nf					\" bring it back in no-fill
.		ls 1					\" output display in ls 1
.		in 0					\" indent back
.		nr :d 1					\" inhibit header from calling )y
.		rs					\" restore spacing
.		br					\" output partial line before display
.		?\\n(;w					\" lay out one keep
.		if \\n(;q .fi				\" restore fill if necessary
.		ls \\n(;L				\" restore line spacing
.		in \\n(;iu				\" restore prevailing indent
.		nr :d 0					\" allow )h to process fl keeps again
.		ev					\" back to previous environment
.		ne 2					\" require two lines under float
.		rm ?\\n(;w				\" free storage
.		rr |\\n(;w !\\n(;w
.		rr ;L ;i ;q ;w
..
])

|macro(.multiple.float.output;
	[
'\"	!{a-z} width of stored floating 0=narrow, 1=wide
'\"	:m initial footer place and partial footnote diverter trap
'\"	:o footer trap location
'\"	:r position at end of page header macro
'\"	:w output counter for floating keeps
'\"	:z input counter for floating keeps
'\"	|{a-z} size of stored floating
'\"	;h height of floating display on top of queue TEMP
'\"	;t copy of :w TEMP
'\"	;w convert to alpha format TEMP
.		de )s
'\"	inhibit if no floats in queue
.		if !(\\n(:z=\\n(:w) \{\
.			nr ;t \\n(:w%26+1		\" get index of next output element
.			af ;w a				\" conversion register for display queue names
.			nr ;w \\n(;t			\" form the next queue element name
.			nr ;h \\n(|\\n(;w		\" height of current display
'\"	The following if condition tests: (1) if wide displays and (2) we are on the
'\"	first column and (3) if the float fits on the current page
'\"	or (4) the float is too large to fit on any one page, and (5) we have used
'\"	less than half of the current page. (the five conditions are grouped
'\"	in the form ((1 & 2) & (1 : (2 & 3))). If this total condition is true,
'\"	then a float is output.
.			if (((\\n(!\\n(;w=1)&(\\n(column_counter<2))&\
((\\n(;h<\\n(.t):((\\n(;h>(\\n(.p-\\n(:r-\\n(:m))&\
(\\n(nl<=(\\n(.p-\\n(:r-\\n(:o/2u+\\n(:r))))) \{\
.				)z			\" output it
.				)s \}			\" recurse
.			rr ;h ;t ;w \}			\" free storage
..
])

|macro(.flush.float.output;
	[
'\"	:d prevent )h from outputting a floating group
'\"	:w output counter for floating keeps
'\"	:z input counter for floating keeps
'\"	;o copy of :w TEMP
.		de )w
'\"	inhibit if no floats in queue
.		if !(\\n(:z=\\n(:w) \{\
.			nr ;o \\n(:w			\" save output count
.			)s
.			if \\n(:w=\\n(;o \{\
.				nr :d 1			\" inhibit header from calling )y
.				rs			\" turn on spacing
.				bp			\" top of next page
.				nr :d 0 \}		\" allow )h to process fl keeps again
.			)w \}				\" recurse
.		rr ;o					\" free storage
..
])

|macro(.footnote.output;
	[
'\"	dump accumulated footnote text
'\"	long text may spring )n trap so the remaining partial
'\"	text may go into :3
'\"	:3 store for partial footnotes
'\"	:F Footnote text
'\"	:H height of footnote separater
'\"	:V vertical spacing
'\"	:m initial footer place and partial footnote diverter trap
'\"	:o footer trap location
'\"	:q amount of footnote text
'\"	:v width of stored footnote 0=narrow, 1=wide
.		de )o
.		if \\n(:v&\\n(column_counter=1 .nr :m \\n(:o \" if printing wide
.		ev 1					\" use footnote environment
.		nf					\" bring it back in no-fill
.		ls 1					\" output display in ls 1
.		in 0					\" indent back
.		:F					\" lay out footnotes
'\"	did we spring the partial footnote trap
.		ie "\\n(.z":3" \{\
.			br				\" get last partial line
.			di				\" end diversion
.			ie \\n(dnu<=\\n(:Hu \{\
.				nr :q 0			\" ignore just divider line
.				nr :o \\n(:mu		\" reset footnote location register
.				rm :3 :F \}
.			el \{\
.				nr dn +\\n(:V-\\n(.v	\" uncertainty correction
.				nr :q \\n(dnu		\" fix amount of footnote text
.				nr :o \\n(:mu+\\n(:qu	\" reset footnote location register
.				nr :o +\\n(:Vu-1u	\" traps spring once we go past
.				rn :3 :F \} \}		\" put text back in :F
.		el \{\
.			nr :q 0				\" clear amount of footnote text
.			rm :F				\" clear text of footnote
.			nr :o \\n(:mu			\" reset footnote location register
.			ch )n 15i \}			\" hide partial footnote diverter
.		ev					\" back to main environment
..
])

|macro(.footnote.diverter;
	[
'\"	partial footnote text diverter
'\"	called via trap planted at -\n(:m
'\"	since the default footer trap is also at -n(:m,
'\"	this macro actually invoked when the footer trap
'\"	has been moved up due to footnote processing
'\"	:3 store for partial footnotes
'\"	:H height of footnote separater
'\"	O current page offset
'\"	;o prevailing page offset ??????? bug with .po
.		de )n
.		di :3
.		nr ;o \\n(.o				\" save prevailing page offset
.		po \\nOu				\" always from left margin
.		ps 10					\" 10 point roman font
.		vs 12p
.		ft 1
.		ie \n(.A=0 \l@\\n(.lu@
.		el _____________________________
.		nr :H \\n(.v				\" height of separater
.		ft					\" restore values
.		vs
.		ps
.		po \\n(;ou
..
])

|macro(.catch.diverter;
	[
'\"	catch output diverter
.		de catch_diverter
.		di catch_output
..
])

|macro(.end.of.file;
	[
.		de )q
..
])

|macro(.pop.counter;
	[
'\"	clever way to restore a number from a stack
'\"	push is via .ds fo \n(xx \*(fo
'\"	.)C counter-name stack-name stack-args
.		de )C
.			nr \\$1 \\$3
.			ds \\$2 \\$4 \\$5 \\$6 \\$7 \\$8 \\$9
..
])

|macro(.initalize;
	[
'\"	REQUIRED INITIALIZATIONS
'\"	initialize various regs
'\"	!{a-z} width of stored floating 0=narrow, 1=wide
'\"	#T used by tbl to draw boxes ?????
'\"	:0 store for initally diverted text
'\"	:1 store for centered block text
'\"	:2 store for multi-page table heading
'\"	:3 store for partial footnotes
'\"	:4 store for equations
'\"	:A multi-page table heading being used
.		nr :A 0
'\"	:B amount of accumulate blank lines
.		nr :B 0
'\"	column_counter 0=one column, N=column number
.		nr column_counter 0
`\"	:D amount of accumulate blank lines in diversion
'\"	:E position of last minimum space in diversion
'\"	:F Footnote text
'\"	:H height of footnote separater
'\"	:K position for )f trap
.		nr :K 0
'\"	:M initial footer and partial footnote diverter for full output page
.		nr :M .8i
'\"	:N position of last minimum space
.		nr :N 0
'\"	:T top of table printed
.		nr :T 0
'\"	:V vertical spacing
.		nr :V \n(.v
'\"	:Y width of a center block diversion
'\"	:c number of columns
.		nr :c 0
'\"	:d prevent )h from outputting a floating group
.		nr :d 0
'\"	:f bottom of table printed
.		nr :f 0
'\"	:m initial footer place and partial footnote diverter trap
.		nr :m .8i
'\"	:o footer trap location
.		nr :o \n(:mu
'\"	:q amount of footnote text
.		nr :q 0
'\"	:r position at end of page header macro
.		nr :r .6i
'\"	:v width of stored footnote 0=narrow, 1=wide
.		nr :v 0
'\"	:w output counter for floating keeps
.		nr :w 0
'\"	:x flag indicating footnote in progress
.		nr :x 0
'\"	:y flag indicating group/float in progress
.		nr :y 0
'\"	:z input counter for floating keeps
.		nr :z 0
'\"	?{a-z} text of stored floating
'\"	|{a-z} size of stored floating
'\"	}D name of last minimum space diversion
'\"	10 the equation as a string
'\"	AT string containing AT&T in helvetica
.		ds AT \f(HBA\h'-.2m'T\h'-.15m'\s0\s13&\s0\s16\h'-.15m'T\fP
'\"	date string
.		if \n(mo-0 .ds date_string January
.		if \n(mo-1 .ds date_string February
.		if \n(mo-2 .ds date_string March
.		if \n(mo-3 .ds date_string April
.		if \n(mo-4 .ds date_string May
.		if \n(mo-5 .ds date_string June
.		if \n(mo-6 .ds date_string July
.		if \n(mo-7 .ds date_string August
.		if \n(mo-8 .ds date_string September
.		if \n(mo-9 .ds date_string October
.		if \n(mo-10 .ds date_string November
.		if \n(mo-11 .ds date_string December
.		as date_string " \n(dy, 19\n(yr
'\"	at&t logo
.		ie "\*(.T"aps" .ds att_logo \s36\(Lb\s0
.		el .ie "\*(.T"eps" .ds att_logo \s36\(Lb\s0
.		el .ie "\*(.T"i300" .ds att_logo \s36\(Lb\s0
.		el .ie "\*(.T"im300" .ds att_logo \s36\(Lb\s0
.		el .ie "\*(.T"post" .ds att_logo \s36\(LH\s0
.		el .ds att_logo \s36\(L1\s0\s16\v'-.4'\*(AT\v'.4'\h'-\w'\*(AT'u'\s0
'\"	MK set to a non zero by eqn if a mark or a lineup is used
'\"	O current page offset
'\"	On page offset for column n
'\"	page number
.		if \n page_counter .nr page_counter -1
.		nr page_counter \n page_counter 1
'\"	T# used by tbl to draw boxes ?????
'\"	T. ?????
'\"	Un position to return to for column n
.		nr U0 .6i
.		rr U1 U2 U3 U4 U5 U6 U7 U8 U9
'\"	W current page width
'\"	Wn page width for column n
'\"	INITIAL SET UP
.		ds page_header ''- \\n page_counter -''	\" top of page string
.		wh 0 )h					\" trap for header
.		wh -\n(:mu )f				\" trap for footer
.		ch )f 15i				\" move it over partial footnote diverter
.		wh -\n(:mu )n				\" trap for partial footnote diverter
.		ch )f -\n(:mu
.		wh -\n(:ru BP				\" trap for bottom
.		em )q					\" end macro
'\"	TROFF ACCENTS (` ' ^ ~ cedilla and 2 umlauts)
'\"	The accent string must follow immediately the character to be accented.
'\"	These strings are "tuned" to the Times Roman type faces ONLY.
'\"	The R, I, and B fonts are assumed to be mounted in
'\"	positions 1, 2, and 3, respectively.
'\"	All of these strings alter the number register `:'.
'\"	Grave accent -- {aeou}\*`
.		ds ` \\k:\h@-\\n(.wu*8u/10u@\h@\\n(.fu/2u*2u+1u-\\n(.fu*.2m@\(ga\h@|\\n:u@
'\"	Lower-case acute accent -- {aeou}\*'
.		ds ' \\k:\h@-\\n(.wu*8u/10u@\h@\\n(.fu/2u*2u+1u-\\n(.fu*.2m+.07m@\(aa\h@|\\n:u@
'\"	Upper-case acute accent -- {aeou}\*+
.		ds + \\k:\h@-\\n(.wu*8u/10u@\h@\\n(.fu/2u*2u+1u-\\n(.fu*.2m+.07m@\v@-.2m@\(aa\v@.2m@\h@|\\n:u@
'\"	Circumflex -- {aeou}\*^
.		ds ^ \\k:\h@-\\n(.wu*8u/10u@\h@\\n(.fu/2u*2u+1u-\\n(.fu*.15m-.07m@\
\h@\\n(.fu-1u/2u*.02m@^\h@|\\n:u@
'\"	Tilde -- n\*~ (But watch out for ".tr ~")
.		ds ~ \\k:\h@-\\n(.wu*8u/10u@\h@\\n(.fu/2u*2u+1u-\\n(.fu*.2m-.07m@\
\h@\\n(.fu-1u/2u*.05m@~\h@|\\n:u@
'\"	Cedilla -- c\*,
.		ds , \\k:\h@-\\n(.wu*85u/100u@\v@.07m@,\v@-.07m@\h@|\\n:u@
'\"	Lower-case umlaut -- {aeou}\*:
.		ds : \\k:\h@-\\n(.wu*85u/100u@\h@\\n(.fu/2u*2u+1u-\\n(.fu*3u*.06m@\
\h@3u-\\n(.fu/2u*.05m-.1m@\
\v@-.6m@\z.\h@\\n(.fu-1u/2u*.05m+.2m@.\v@.6m@\h@|\\n:u@
'\"	Upper-case umlaut -- {AEOU}\*;
.		ds ; \\k:\h@-\\n(.wu*75u/100u@\h@\\n(.fu/2u*2u+1u-\\n(.fu*3u*.09m@\
\h@3u-\\n(.fu/2u*.06m-.15m@\h@\\n(.fu-1u/2u*.04m@\
\v@-.85m@\z.\h@.3m@.\v@.85m@\h@|\\n:u@
'\"	Nasal -- c\*-
.		ds - \\k:\h@-\\n(.wu*75u/100u@\v@.7m@`\v@-.7m@\h@|\\n:u@
'\"	Slash -- c\*/
.		ds / \\k:\h@-\\n(.wu*85u/100u@\v@.2m@\(aa\v@-.2m@\h@|\\n:u@
])
