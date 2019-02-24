|include(list.h)
|include(macro.h)
|include(style.h)

|attribute(page.offset; init 0;
	$ [
'\"	O current page offset
'\"	On page offset for column n
.		po $
.		nr O \n(.o
.		nr O0 \n(.o
])

|attribute(line.length; init 6i;
	$ [
'\"	W current page width
'\"	Wn page width for column n
.		ll $
.		lt $
.		nr W \n(.lu
])

|attribute(page.length; init 11i;
	$ [
.		pl $
])

|attribute(spacing; init off;
	on [
.		rs
]	off [
.		ns
])

|attribute(new.page; nostack;
	[
.		bp
]	odd [
'\"	;P page number register TEMP
.		bp
.		af ;P \g'page_counter'		\" save format of page_counter
.		af page_counter 1			\" normal format for next line
.		nr ;P \n'page_counter'			\" must use different name for P
.		af page_counter \g(;P			\" restore format to P
.		af ;P 1					\" normal format for control register
.		if !\n(;P%2 \{\
.			rs
.			bp \}
.		rr ;P					\" free storage
]	even [
'\"	;P page number register TEMP
.		bp
.		af ;P \g'page_counter'		\" save format of page_counter
.		af page_counter 1			\" normal format for next line
.		nr ;P \n'page_counter'			\" must use different name for P
.		af page_counter \g(;P			\" restore format to P
.		af ;P 1					\" normal format for control register
.		if \n(;P%2 \{\
.			rs
.			bp \}
.		rr ;P					\" free storage
])

|attribute(line.spacing; stack, init 1;
	$ [
.		ls $
])

|attribute(blank.lines; nostack;
	$ [
.		sp $
.		ti \n(.iu				\" movement resets indentation
]	[
.		sp 1
.		ti \n(.iu				\" movement resets indentation
])

|attribute(minimum.lines; nostack;
	$ [
'\"	minimum space mechanism
'\"	:B amount of accumulated blank space
'\"	:D amount of accumulated blank space in diversion
'\"	:E position of last minimum space in diversion
'\"	:N position of last minimum space
'\"	}D name of last minimum space diversion
'\"	;4 amount to space TEMP
.		br
'\"	when in a diversion, use .d (rather than nl) for position
'\"	careful--space might occur at the same
'\"	position BUT in a different named diversion
'\"	(hence, the use of the string }D)
'\"	bug still might be possible, if the same named diversion
'\"	is used, that a space might occur in the same vertical place.
'\"	very unlikely for
'\"		a) monk will remove :D and :E when not in a diversion
'\"		b) tbl generates unique names
.		ie !'\n(.z'' \{\
.			if !'\n(.z'\*(}D' .rr :D :E
.			nr ;4 $v
.			if !(\n(.d=\n(:D) .nr :E 0	\" different place for sure
.			nr ;4 -\n(:Eu			\" remove previous accumulation, if any
.			if \n(;4 \{\
.				sp \n(;4u
.				nr :E +\n(;4u \}
.			rm ;4
.			ds }D \n(.z
.			nr :D \n(.d \}
.		el \{\
.			rr :D :E			\" remove the in-diversion registers
.			nr ;4 $v
.			if !(\n(nl=\n(:N) .nr :B 0	\" different place for sure
.			nr ;4 -\n(:Bu			\" remove previous accumulation, if any
.			if \n(;4 \{\
.				sp \n(;4u		\" space and recompute accumulation
.				nr :B +\n(;4u \}
.			rm ;4
.			nr :N \n(nl \}
.			ti \n(.iu			\" movement resets indentation
]	[
'\"	minimum space mechanism
'\"	:B amount of accumulated blank space
'\"	:D amount of accumulated blank space in diversion
'\"	:E position of last minimum space in diversion
'\"	:N position of last minimum space
'\"	}D name of last minimum space diversion
'\"	;4 amount to space TEMP
.		br
'\"	when in a diversion, use .d (rather than nl) for position
'\"	careful--space might occur at the same
'\"	position BUT in a different named diversion
'\"	(hence, the use of the string }D)
'\"	bug still might be possible, if the same named diversion
'\"	is used, that a space might occur in the same vertical place.
'\"	very unlikely for
'\"		a) monk will remove :D and :E when not in a diversion
'\"		b) tbl generates unique names
.		ie !'\n(.z'' \{\
.			if !'\n(.z'\*(}D' .rr :D :E
.			nr ;4 1v
.			if !(\n(.d=\n(:D) .nr :E 0	\" different place for sure
.			nr ;4 -\n(:Eu			\" remove previous accumulation, if any
.			if \n(;4 \{\
.				sp \n(;4u
.				nr :E +\n(;4u \}
.			rm ;4
.			ds }D \n(.z
.			nr :D \n(.d \}
.		el \{\
.			rr :D :E			\" remove the in-diversion registers
.			nr ;4 1v
.			if !(\n(nl=\n(:N) .nr :B 0	\" different place for sure
.			nr ;4 -\n(:Bu			\" remove previous accumulation, if any
.			if \n(;4 \{\
.				sp \n(;4u		\" space and recompute accumulation
.				nr :B +\n(;4u \}
.			rm ;4
.			nr :N \n(nl \}
.			ti \n(.iu			\" movement resets indentation
])

|attribute(horizontal.motion; nostack;
	$ [\h'$']
	number $$. [\h'\n$$.']
	number $number.register [\h'\n($number.register'])

|attribute(vertical.motion; nostack;
	$ [\v'$']
	number $$. [\v'\n$$.']
	number $number.register [\v'\n($number.register'])

|attribute(concatenate; nostack;
	[\&\c
])

|attribute(new.line; nostack;
	[
.		br
])

|attribute(indent; stack, default 0;
	$ [
.		in $
])

|attribute(indent.line; nostack;
	$ [
.		ti $
]
	number $$. [
.		ti \n$$.u
]
	number $number.register [
.		ti \n($number.registeru
])

|attribute(fill; stack, init on;
	on [
'		fi
]	off [
'		nf
])

|attribute(adjust; stack, init both;
	right [
.		ad r
]	left [
.		ad l
]	both [
.		ad b
]	on [
.		ad
]	off [
.		na
])

|attribute(title; nostack;
	$left $center $right [
.		tl \(ts$left\(ts$center\(ts$right\(ts
]	$$. [
.		tl \*$$.
]	$$.. [
.		tl \*($$..
]	$ [
.		tl $
])

|attribute(center; stack, default off;
	on [
.		ce 9999
]	off [
.		ce 0
])

|attribute(center.block; stack, default off;
	on [
'\"	:1 store for centered block text
'\"	:Y width of a center block diversion
.		nr :Y 0					\" initialize width of block
.		br					\" output partial line before block
.		di :1					\" begin diversion
]	off [
'\"	:1 store for centered block text
'\"	:Y width of a center block diversion
'\"	;L remembered line spacing TEMP
'\"	;i remembered indent TEMP
'\"	;q remembered fill/no-fill mode TEMP
.		br					\" get last partial line
.		di					\" end diversion of centered block
.		nr ;q \n(.u				\" save prevailing fill/no-fill mode
.		nf					\" bring it back in no-fill
.		nr ;L \n(.L				\" save line spacing
.		ls 1					\" output display in ls 1
.		nr ;i \n(.i				\" save prevailing indent
.		if \n(dl>\n(:Y .nr :Y \n(dl
.		if \n(:Y<\n(.l .in (\n(.lu-\n(:Yu)/2u	\"indent by half of white-space
.		:1					\" text
.		if \n(;q .fi				\" restore fill if necessary
.		ls \n(;L				\" restore line spacing
.		in \n(;iu				\" restore prevailing indent
.		rr ;L ;i ;q				\" free storage
.		rm :1
])

|attribute(text; nostack;
	$ [$]
	roman $ [\f1\&$\fP\&]
	italics $ [\f2\&$\fP\&]
	bold $ [\f3\&$\fP\&]
	bolditalics $ [\f4\&$\fP\&]
	cw $ [\f(CW\&$\fP\&])

|attribute(error; nostack;
	number $* $$. [
.		tm "__FILE__":__LINE__: $* \n$$.
]	number $* $number.register [
.		tm "__FILE__":__LINE__: $* \n($number.register
]	string $* $$. [
.		tm "__FILE__":__LINE__: $* \n$$.
]	string $* $string.register [
.		tm "__FILE__":__LINE__: $* \n($string.register
]	$* [
.		tm "__FILE__":__LINE__: $*
])

|attribute(here; stack, default off;
	on [
'\"	group mechanism
'\"	:0 store for initally diverted text
'\"	:y flag indicating group/float in progress
.		if \n(:y .tm "__FILE__":__LINE__: Here's and/or Around's can not be nested
.		nr :y 1					\" set flag indicating group
.		br					\" output partial line before display
.		di :0					\" collect in :0
]	off [
'\"	Control page orientation of blocks.
'\"	Blocks are output as soon after their definition as
'\"	feasible.  The following rules are used:
'\"	     1. if the block will fit on the current page, output it there.
'\"	     2. if the block won't fit on any page, and we have used less than
'\"		half of the current page, then output the block on the
'\"		current page.
'\"	     3. skip to the next page.
.		br					\" get last partial line
.		di					\" end diversion
'\"	The following if condition tests: (1) if the block is too large to
'\"	fit on the current page and either (2) it will fit on an unused page
'\"	or (3) we have already used more than half of the current page. (the
'\"	three conditions are grouped in the form 1 & (2 : 3)). If this
'\"	total condition is true, then a page is ejected and the block output
'\"	on the next page.
'\"	:0 store for initally diverted text
'\"	:d prevent )h from outputting a floating group
'\"	:m initial footer place and partial footnote diverter trap
'\"	:o footer trap location
'\"	:r position at end of page header macro
'\"	:y flag indicating group/float in progress
'\"	;L remembered line spacing TEMP
'\"	;i remembered indent TEMP
'\"	;q remembered fill/no-fill mode TEMP
.		nr :d 1					\" prevent )h from outputting a floating group
.		if (\n(dn>=\n(.t)&((\n(dn<(\n(.p-\n(:r-\n(:m)):\
(\n(nl>(\n(.p-\n(:r-\n(:o/2u+\n(:r)))\
.			ne \n(.tu+1v			\" spring the trap
.		nr ;q \n(.u				\" save prevailing fill/no-fill mode
.		nf					\" bring it back in no-fill
.		nr ;L \n(.L				\" save line spacing
.		ls 1					\" output display in ls 1
.		nr ;i \n(.i				\" save prevailing indent
.		in 0					\" indent back
.		rs					\" restore spacing
.		:0					\" text
.		nr :d 0					\" allow )h to output a floating group
.		if \n(;q .fi				\" restore fill if necessary
.		ls \n(;L				\" restore line spacing
.		in \n(;iu				\" restore prevailing indent
.		nr :y 0					\" reset flag indicating group
.		rr ;L ;i ;q				\" free storage
.		rm :0
])

|attribute(around; stack, default off;
	on [
'\"	float mechanism
'\"	:0 store for initally diverted text
'\"	:y flag indicating group/float in progress
'\"	Wn page width for column n
'\"	;L prevailing line spacing
'\"	;f prevailing font
'\"	;i prevailing indent
'\"	;j prevailing adjust
'\"	;l prevailing line length
'\"	;s prevailing point size
'\"	;u prevailing fill/no-fill mode
'\"	;v prevailing vertical spacing
.		if \n(:y .tm "__FILE__":__LINE__: Here's and/or Around's can not be nested
.		nr :y 1					\" set flag indicating group
.		nr ;s \n(.s				\" save prevailing point size
.		nr ;f \n(.f				\" save prevailing font
.		nr ;u \n(.u				\" save prevailing fill/no-fill mode
.		nr ;j \n(.j				\" save prevailing adjust
.		nr ;v \n(.v				\" save prevailing vertical spacing
.		nr ;L \n(.L				\" save prevailing line spacing
.		nr ;l \n(.l				\" save prevailing line length
.		nr ;i \n(.i				\" save prevailing indent
.		ev 1					\" use footnote environment
.		ps \n(;s				\" restore point size
.		vs \n(;sp+2p				\" restore vertical spacing
.		ft \n(;f				\" restore font
.		nf					\" bring it back in no-fill
.		if \n(;u .fi				\" restore fill if necessary
.		ad \n(;j				\" restore page adjust
.		vs \n(;vu				\" restore vertical spacing
.		ls \n(;L				\" restore line spacing
.		ll \n(;lu				\" restore line length
.		in \n(;iu				\" restore indent
.		ti \n(;iu				\" restore indentation
.		hy 14					\" turn on hyphenation
.		lt \n(;lu				\" restore title length
|ifvalue around_width full [
.		if \n(column_counter \{\
.			ll \n(W0u			\" go wide
.			lt \n(W0u \}
]
.		di :0					\" collect in :0
]	off [
'\"	Control page orientation of floats.
'\"	Floats are output as soon after their definition as
'\"	feasible.  The following rules are used:
'\"	     1. if the queue contains only this float and it will fit on the
'\"		current page, output it there.
'\"	     2. if the queue contains only this float and it won't fit on any
'\"		page, and we have used less than half of the current page,
'\"		then output the float on the current page.
'\"	     3. let the top of page macro handle it.
'\"	!{a-z} width of stored floating 0=narrow, 1=wide
'\"	:0 store for initally diverted text
'\"	:m initial footer place and partial footnote diverter trap
'\"	:o footer trap location
'\"	:r position at end of page header macro
'\"	:w output counter for floating keeps
'\"	:y flag indicating group/float in progress
'\"	:z input counter for floating keeps
'\"	?{a-z} text of stored floating
'\"	W current page width
'\"	|{a-z} size of stored floating
'\"	;L prevailing line spacing
'\"	;f prevailing font
'\"	;i prevailing indent
'\"	;j prevailing adjust
'\"	;l prevailing line length
'\"	;s prevailing point size
'\"	;u prevailing fill/no-fill mode
'\"	;v prevailing vertical spacing
'\"	;w convert to alpha format
.		br					\" get last partial line
.		di					\" end diversion of around block
|ifvalue around_width full [
.		if \n(column_counter \{\
.			ll \nWu				\" go narrow
.			lt \nWu \}
]
.		ev					\" back to previous environment
.		nr :z \n(:z%26+1			\" get index of next output element
.		if \n(:z-\n(:w=0 .tm "__FILE__":__LINE__: too many floating displays
.		af ;w a					\" conversion register for display queue names
.		nr ;w \n(:z				\" form the next queue element name
.		rn :0 ?\n(;w				\" put the display into the queue
.		nr !\n(;w 0				\" narrow
|ifvalue around_width full [
.		nr !\n(;w 1				\" wide around
]
|ifnotvalue around_width full [
.		if !\n(column_counter .nr !\n(;w 1	\" wide around if only one column
]
.		nr |\n(;w \n(dn				\" put the size of text
'\"	The following if condition tests: (1) if there is only one float
'\"	on the queue to output and (2) if wide displays and (3) we are on the
'\"	first column and (4) if the float fits on the current page or
'\"	(5) the float is too large to fit on any one page, and (6) we have used
'\"	less than half of the current page. (the six conditions are grouped
'\"	in the form (1 & (2 & 3) & (4 : (5 & 6))). If this total condition is true,
'\"	then a float is output.
.		if (((\n(:z-\n(:w=1):(\n(:z+26-\n(:w=1))&((\n(!\n(;w=1)&(\n(column_counter<2))&\
((\n(dn<\n(.t):((\n(dn>(\n(.p-\n(:r-\n(:m))&\
(\n(nl<=(\n(.p-\n(:r-\n(:o/2u+\n(:r))))) .)z
.		nr :y 0					\" reset flag indicating group
.		rr ;L ;f ;i ;j ;l ;s ;u ;v ;w		\" free storage
])

|attribute(around_placement; nostack;
	[
.		)w
])

|attribute(footnote; stack, default off;
	on [
'\"	:F Footnote text
'\"	:q amount of footnote text
'\"	:x flag indicating footnote in progress
'\"	Wn page width for column n
'\"	;L prevailing line spacing
'\"	;f prevailing font
'\"	;j prevailing adjust
'\"	;l prevailing line length
'\"	;o prevailing page offset ??????? bug with .po
'\"	;s prevailing point size
'\"	;u prevailing fill/no-fill mode
'\"	;v prevailing vertical spacing
.		if \n(:x .tm "__FILE__":__LINE__: Footnotes can not be nested
.		nr :x 1					\" set flag indicating footnote
.		nr ;s \n(.s				\" save prevailing point size
.		nr ;f \n(.f				\" save prevailing font
.		nr ;u \n(.u				\" save prevailing fill/no-fill mode
.		nr ;j \n(.j				\" save prevailing adjust
.		nr ;v \n(.v				\" save prevailing vertical spacing
.		nr ;L \n(.L				\" save prevailing line spacing
.		nr ;l \n(.l				\" save prevailing line length
.		ev 1					\" use footnote environment
.		ps \n(;s				\" restore point size
.		vs \n(;sp+2p				\" restore vertical spacing
.		ft \n(;f				\" restore font
.		nf					\" bring it back in no-fill
.		if \n(;u .fi				\" restore fill if necessary
.		ad \n(;j				\" restore page adjust
.		vs \n(;vu				\" restore vertical spacing
.		ls \n(;L				\" restore line spacing
.		ll \n(;lu				\" restore line length
.		in 0					\" reset indent
.		ti 0					\" reset indentation
.		hy 14					\" turn on hyphenation
.		lt \n(;lu				\" restore title length
|ifvalue footnote_width full [
.		if \n(column_counter \{\
.			ll \n(W0u			\" go wide
.			lt \n(W0u \}
]
.		da :F					\" collect in :F
.		ie !\n(:q \{\
.			nr ;o \n(.o			\" save prevailing page offset
.			po 0				\" always from left margin
.			ps 10				\" 10 point roman font
.			vs 12p
.			ft 1
.			ie \n(.A=0 \l'72p'		\" layout partial rule if new footnote
.			el  __________
.			ft				\" restore values
.			vs
.			ps
.			po \n(;ou \}
.		el .sp 1				\" leave a blank between them
]	off [
'\"	:V vertical spacing
'\"	:m initial footer place and partial footnote diverter trap
'\"	:o footer trap location
'\"	:q amount of footnote text
'\"	:v width of stored footnote 0=narrow, 1=wide
'\"	:x flag indicating footnote in progress
'\"	W current page width
'\"	;L prevailing line spacing
'\"	;f prevailing font
'\"	;j prevailing adjust
'\"	;l prevailing line length
'\"	;o prevailing page offset ??????? bug with .po
'\"	;s prevailing point size
'\"	;u prevailing fill/no-fill mode
'\"	;v prevailing vertical spacing
.		br					\" get last partial line
.		da					\" end diversion
|ifvalue footnote_width full [
.		if \n(column_counter \{\
.			ll \nWu				\" go narrow
.			lt \nWu \}
.		nr :v 1					\" wide footnote
]
|ifnotvalue footnote_width full [
.		if !\n(column_counter .nr :v 1		\" wide footnote if only one column
]
.		ev					\" go back to previous environment
.		nr :x 0					\" clear flag indicating footnote
.		if !\n(:o-\n(:m \{\
.			nr :o +\n(:Vu-1u		\" traps spring once we go past
.			nr dn +\n(:V-\n(.v \}		\" uncertainty correction
.		nr :q +\n(dnu				\" add in amount of new footnotes
'\"	Move up footer trap, but not above current position on page
.		nr :o +\n(dnu
.		if !\n(.pu-\n(nlu-.5p-\n(:ou .nr :o \n(.pu-\n(nlu-.5p
'\"	or below :m!
.		if !\n(:ou-\n(:mu .nr :o \n(:mu
'\"	or at all if in multi-column mode and the footnote is wide
.		if \n(column_counter>1&\n(:v .nr :o \n(:mu
.		ch )f -\n(:ou				\" move footer trap
.		rr ;L ;f ;j ;l ;o ;s ;u ;v		\" free storage
])

|attribute(size; stack, default 10;
	$ [|ifvalue inline on [\s$\&]|ifvalue inline off [
.		ps $
]])

|attribute(space; stack, init 12;
	$ [
.		vs $
])

|attribute(SPACE; nostack;
	[
.		vs \n(.sp+2p				\" restore vertical spacing
])

|attribute(font; stack, default roman;
	roman [|ifvalue inline on [\&\f1\&]|ifvalue inline off [
.		ft 1
]]	italics [|ifvalue inline on [\&\f2\&]|ifvalue inline off [
.		ft 2
]]	bold [|ifvalue inline on [\&\f3\&]|ifvalue inline off [
.		ft 3
]]	bolditalics [|ifvalue inline on [\&\f4\&]|ifvalue inline off [
.		ft 4
]]	cw [|ifvalue inline on [\&\f(CW\&]|ifvalue inline off [
.		ft CW
]])

|attribute(font.family; stack, default times;
	times [
.		fp 1 R
.		fp 2 I
.		fp 3 B
.		fp 4 BI
]	palatino [
.		fp 1 PA
.		fp 2 PI
.		fp 3 PB
.		fp 4 PX
]	bembo [
.		fp 1 B1
.		fp 2 B2
.		fp 3 B3
.		fp 4 B4
]	optima [
.		fp 1 O1
.		fp 2 O2
.		fp 3 O3
.		fp 4 O4
]	souvenir [
.		fp 1 SV
.		fp 2 SI
.		fp 3 SB
.		fp 4 SX
]	helvetica [
.		fp 1 H
.		fp 2 HI
.		fp 3 HB
.		fp 4 HX
]	cw [
.		fp 1 CW
.		fp 2 I
.		fp 3 B
.		fp 4 BI
]	gothic [
.		fp 1 C
.		fp 2 CX
.		fp 3 TB
.		fp 4 TX
]	condensed [
.		fp 1 Hcond
.		fp 2 HcondI
.		fp 3 HcondB
.		fp 4 HlightI
]	narrow [
.		fp 1 Hr
.		fp 2 Hi
.		fp 3 Hb
.		fp 4 Hx
])

|attribute(if; nostack;
	$test [
.		if $test \
]	begin $test [
.		if $test \{\
]	end [\}\c
]	else $test [
.		ie $test \
]	else begin $test [
.		ie $test \{\
]	else [
.		el \
]	else begin [
.		el \{\
])

|attribute(tab; nostack;
	[\&	])

|attribute(leader; nostack;
	[\&])

|attribute(thick.line; nostack;
	$ [
.		ps 24
\l'$'
.		ps
]	[
.		ps 24
\l'\n(.lu'
.		ps
])

|attribute(line; nostack;
	$ [\l'$']
	[\l'\n(.lu'])

|attribute(box; nostack;
	x [\s12\o'\(mu\(sq'\s0]
	empty [\s12\(sq\s0]
	[\(sq])

|attribute(underline; stack, default off;
	on [\kx]
	off [\l'|\nxu\(ul'])

|attribute(hyphenate; stack, init on;
	on [
.		hy 14
]	off [
.		nh
])

|attribute(case; stack, init any;
	upper [
'\" need to write this yet
]	lower [
'\" need to write this yet
]	capitalize [
'\" need to write this yet
]	any [
'\" need to write this yet
])

|attribute(tab.stops; nostack;
	$* [
.		ta $*
]	[
.		ta .5i 1.0i 1.5i 2.0i 2.5i 3.0i 3.5i 4.0i 4.5i 5.0i 5.5i 6.0i 6.5i 7.0i 7.5i
])

|attribute(set; nostack;
	$* $number [
|for i in $* {
.		nr $i $number
}]	string $* $string [
|for i in $* {
.		ds $i $string
}])

|attribute(store; nostack;
	$* $$. [
|for i in $* {
.		nr $i \n$$.
}]	$* $number.register [
|for i in $* {
.		nr $i \n($number.register
}]	string $* $$. [
|for i in $* {
.		ds $i \*$$.
}]	string $* $string.register [
|for i in $* {
.		ds $i \*($string.register
}]	number.from.string $* $$. [
|for i in $* {
.		nr $i \*$$.
}]	number.from.string $* $string.register [
|for i in $* {
.		nr $i \*($string.register
}]	string.from.number $* $$. [
|for i in $* {
.		ds $i \n$$.
}]	string.from.number $* $number.register [
|for i in $* {
.		ds $i \n($number.register
}]	width $* $$. [
|for i in $* {
.		nr $i \w'\*$$.'
}]	width $* $number.register [
|for i in $* {
.		nr $i \w'\*($number.register'
}]	indent $* [
|for i in $* {
.		nr $i \n(.i
}]	linelength $* [
|for i in $* {
.		nr $i \n(.l
}]	pagewidth $* [
|for i in $* {
.		nr $i \nWu
}]	totrap $* [
|for i in $* {
.		nr $i \n(.t
}]	pageoffset $* [
|for i in $* {
.		nr $i \n(.o
}]	diversionheight $* [
|for i in $* {
.		nr $i \n(dn
}])

|attribute(clear; nostack;
	$* [
|for i in $* {
.		nr $i 0
}]	string $* [
.		rm $*
])

|attribute(rename; nostack;
	$from $to [
.		rn $from $to
])

|attribute(add; nostack;
	$result $$. $* [
.		nr $result \n$$.
|for i in $* {
.		nr $result +\n($i
}]	$result $operand $* [
.		nr $result \n($operand
|for i in $* {
.		nr $result +\n($i
}]	$result $$. [
.		nr $result +\n$$.
]	$result $operand [
.		nr $result +\n($operand
]	constant $result $constant $* [
.		nr $result $constant
|for i in $* {
.		nr $result +$i
}]	constant $result $constant [
.		nr $result +$constant
]	string $string $text [
.		as $string $text
]	string.from.number $string $$. [
.		as $string \n$$.
]	string.from.number $string $number.register [
.		as $string \n($number.register
]	string.from.string $string $$. [
.		as $string \*$$.
]	string.from.string $string $string.register [
.		as $string \*($string.register
])

|attribute(sub; nostack;
	$result $$. $* [
.		nr $result \n$$.
|for i in $* {
.		nr $result -\n($i
}]	$result $operand $* [
.		nr $result \n($operand
|for i in $* {
.		nr $result -\n($i
}]	$result $$. [
.		nr $result -\n$$.
]	$result $operand [
.		nr $result -\n($operand
]	constant $result $constant $* [
.		nr $result $constant
|for i in $* {
.		nr $result -$i
}]	constant $result $constant [
.		nr $result -$constant
])

|attribute(incr; nostack;
	$* [
|for i in $* {
.		nr $i +1
}])

|attribute(decr; nostack;
	$* [
|for i in $* {
.		nr $i -1
}])

|attribute(protect; nostack;
	$ [
.		ne $
]	[
.		ne 3
])

|attribute(warn.orphan; nostack;
	$ [
'\" need to write this yet
]	[
'\" need to write this yet
])

|attribute(column; nostack;
	$column $width $offset [
'\"	:c number of columns
'\"	On page offset for column n
'\"	Wn page width for column n
.		if \n(:c<$column .nr :c $column
.		nr W$column $width
.		nr O$column $offset
]	width $width [
'\"	:c number of columns
'\"	Wn page width for column n
.		nr W\n(:c $width
]	offset $offset [
'\"	:c number of columns
'\"	On page offset for column n
.		nr O\n(:c $offset
]	next [
'\"	:c number of columns
.		nr :c +1
]	clear	[
'\"	:c number of columns
.		rr :c
]	new [
.		ie !\n(column_counter .tm "__FILE__":__LINE__: not in multi-column mode
.		el \{\
.			br				\" force out last partial line
.			ne \n(.tu+1v \}			\" spring footer trap for this column
]	new $ [
'\"	O current page offset
'\"	On page offset for column n
'\"	W current page width
'\"	Wn page width for column n
'\"	;d delta width TEMP
.		ie !\n(column_counter .tm "__FILE__":__LINE__: not in multi-column mode
.		el \{\
.			br				\" force out last partial line
.			ne \n(.tu+1v			\" spring footer trap for this column
.			nr ;d \n(.lu-\n(W\n('column_counter'u \" quotations shrink line length
.			nr column_counter $		\" set column to $
.			nr O \n(O$u			\" restore new offset
.			po \nOu				\" set new offset
.			nr W \n(W$u+\n(;du		\" restore new width
.			ll \nWu				\" set new line length
.			lt \nWu				\" set new title length
.			rr ;d \}			\" free storage
]	old $ [
'\"	O current page offset
'\"	On page offset for column n
'\"	Un position to return to for column n
'\"	W current page width
'\"	Wn page width for column n
'\"	;d delta width TEMP
.		ie !\n(column_counter .tm "__FILE__":__LINE__: not in multi-column mode
.		el \{\
.			br				\" force out last partial line
.			if \n(nl>\n(U\n(column_counter \
.				mk U\n(column_counter	\" new vertical position
.			nr ;d \n(.lu-\n(W\n('column_counter'u \" quotations shrink line length
.			nr column_counter $		\" set column to $
.			nr O \n(O$u			\" restore new offset
.			po \nOu				\" set new offset
.			nr W \n(W$u+\n(;du		\" restore new width
.			ll \nWu				\" set new line length
.			lt \nWu				\" set new title length
.			rs				\" restore spacing
.			ie \n(U$ .sp |\n(U$u		\" restore vertical position
.			el .sp |\n(U0u			\" default top if not set
.			ns				\" turn off spacing
.			rr ;d \}			\" free storage
]	on [
'\"	O current page offset
'\"	On page offset for column n
'\"	Un position to return to for column n
'\"	W current page width
'\"	Wn page width for column n
.		ie \n(column_counter .tm "__FILE__":__LINE__: already in multi-column mode
.		el \{\
.			br				\" force out last partial line
.			nr column_counter 1		\" flag for multi-column mode, first column
.			nr W \n(W1u			\" restore first column width
.			ll \nWu				\" set first column line length
.			lt \nWu				\" set first column title length
.			nr O \n(O1u			\" restore first column offset
.			po \nOu				\" set first column offset
.			mk U0				\" set where all columns start
.			ns				\" avoid .sp in user text here
.			rr U1 U2 U3 U4 U5 U6 U7 U8 U9	\" zero column starts
.			rr ;d \}			\" free storage
]	off [
'\"	:c number of columns
'\"	Un position to return to for column n
'\"	W current page width
'\"	Wn page width for column n
'\"	;d delta width TEMP
.		ie !\n(column_counter .tm "__FILE__":__LINE__: already in full page mode
.		el \{\
.			br				\" force out last partial line
.			if \n(nl>\n(U\n(column_counter \
.				mk U\n(column_counter	\" new vertical position
.			nr ;d \n(.lu-\n(W\n('column_counter'u \" quotations shrink line length
.			nr column_counter 0		\" set flag for full page mode
.			nr O \n(O0u			\" restore new offset
.			po \nOu				\" set new offset
.			nr W \n(W0u+\n(;du		\" restore line length
.			ll \nWu				\" set new line length
.			lt \nWu				\" set new title length
.			rs				\" restore spacing
.			sp |\n(U0u			\" restore vertical position
.			if \n(nl<\n(U1 \
.				sp |\n(U1u
.			if \n(nl<\n(U2 \
.				sp |\n(U2u
.			if \n(nl<\n(U3 \
.				sp |\n(U3u
.			if \n(nl<\n(U4 \
.				sp |\n(U4u
.			if \n(nl<\n(U5 \
.				sp |\n(U5u
.			if \n(nl<\n(U6 \
.				sp |\n(U6u
.			if \n(nl<\n(U7 \
.				sp |\n(U7u
.			if \n(nl<\n(U8 \
.				sp |\n(U8u
.			if \n(nl<\n(U9 \
.				sp |\n(U9u
.			ti \n(.iu			\" movement resets indentation
.			rr U1 U2 U3 U4 U5 U6 U7 U8 U9	\" zero column starts
.			ns				\" turn off spacing
.			rr ;d \}			\" free storage
]	2 [
'\"	:c number of columns
'\"	O current page offset
'\"	On page offset for column n
'\"	Un position to return to for column n
'\"	W current page width
'\"	Wn page width for column n
.		ie \n(column_counter .tm "__FILE__":__LINE__: already in multi-column mode
.		el \{\
.			br				\" force out last partial line
.			nr column_counter 1		\" flag for multi-column mode, first column
.			nr W0 \nW			\" save width
.			nr W \n(.lu*8u/17u		\" new width
.			nr W1 \nW			\" new width
.			nr W2 \nW			\" new width
.			nr O0 \n(.ou			\" page offset for other column
.			nr O1 \n(.ou			\" page offset for other column
.			nr O2 \n(.lu*9u/17u+\n(.ou	\" page offset for other column
.			ll \nWu				\" set new line length
.			lt \nWu				\" set new title length
.			nr :c 2				\" two columns
.			mk U0				\" set where all columns start
.			ns				\" turn off spacing
.			rr U1 U2 \}			\" zero column starts
])

|attribute(divert.string; stack, default off;
	$string.name [
.		ds $string.name ]
	off [
])

|attribute(divert.number; stack, default off;
	$number.name [
.		nr $number.name ]
	off [
])

|attribute(divert.output; stack, default off;
	on $string.name [
'\"	;L prevailing line spacing
'\"	;f prevailing font
'\"	;j prevailing adjust
'\"	;l prevailing line length
'\"	;o prevailing page offset ??????? bug with .po
'\"	;s prevailing point size
'\"	;u prevailing fill/no-fill mode
'\"	;v prevailing vertical spacing
.		nr ;s \n(.s				\" save prevailing point size
.		nr ;f \n(.f				\" save prevailing font
.		nr ;u \n(.u				\" save prevailing fill/no-fill mode
.		nr ;j \n(.j				\" save prevailing adjust
.		nr ;v \n(.v				\" save prevailing vertical spacing
.		nr ;L \n(.L				\" save prevailing line spacing
.		nr ;l \n(.l				\" save prevailing line length
.		ev 1					\" use footnote environment
.		ps \n(;s				\" restore point size
.		vs \n(;sp+2p				\" restore vertical spacing
.		ft \n(;f				\" restore font
.		nf					\" bring it back in no-fill
.		if \n(;u .fi				\" restore fill if necessary
.		ad \n(;j				\" restore page adjust
.		vs \n(;vu				\" restore vertical spacing
.		ls \n(;L				\" restore line spacing
.		ll \n(;lu				\" restore line length
.		in 0					\" reset indent
.		ti 0					\" reset indentation
.		hy 14					\" turn on hyphenation
.		lt \n(;lu				\" restore title length
.		di $string.name				\" collect in $string.name
]	off [
'\"	;L prevailing line spacing
'\"	;f prevailing font
'\"	;j prevailing adjust
'\"	;l prevailing line length
'\"	;o prevailing page offset ??????? bug with .po
'\"	;s prevailing point size
'\"	;u prevailing fill/no-fill mode
'\"	;v prevailing vertical spacing
.		br					\" get last partial line
.		di					\" end diversion
.		ev					\" go back to previous environment
.		rr ;L ;f ;j ;l ;o ;s ;u ;v		\" free storage
])

|attribute(divert.input; stack, default off;
	on $string.name [
.		eo
.		de $string.name\(ts\(ts
]	off [
.\(ts\(ts
.		ec
])

|attribute(append.output; stack, default off;
	on $string.name [
'\"	;L prevailing line spacing
'\"	;f prevailing font
'\"	;j prevailing adjust
'\"	;l prevailing line length
'\"	;o prevailing page offset ??????? bug with .po
'\"	;s prevailing point size
'\"	;u prevailing fill/no-fill mode
'\"	;v prevailing vertical spacing
.		nr ;s \n(.s				\" save prevailing point size
.		nr ;f \n(.f				\" save prevailing font
.		nr ;u \n(.u				\" save prevailing fill/no-fill mode
.		nr ;j \n(.j				\" save prevailing adjust
.		nr ;v \n(.v				\" save prevailing vertical spacing
.		nr ;L \n(.L				\" save prevailing line spacing
.		nr ;l \n(.l				\" save prevailing line length
.		ev 1					\" use footnote environment
.		ps \n(;s				\" restore point size
.		vs \n(;sp+2p				\" restore vertical spacing
.		ft \n(;f				\" restore font
.		nf					\" bring it back in no-fill
.		if \n(;u .fi				\" restore fill if necessary
.		ad \n(;j				\" restore page adjust
.		vs \n(;vu				\" restore vertical spacing
.		ls \n(;L				\" restore line spacing
.		ll \n(;lu				\" restore line length
.		in 0					\" reset indent
.		ti 0					\" reset indentation
.		hy 14					\" turn on hyphenation
.		lt \n(;lu				\" restore title length
.		da $string.name				\" collect in $string.name
]	off [
'\"	;L prevailing line spacing
'\"	;f prevailing font
'\"	;j prevailing adjust
'\"	;l prevailing line length
'\"	;o prevailing page offset ??????? bug with .po
'\"	;s prevailing point size
'\"	;u prevailing fill/no-fill mode
'\"	;v prevailing vertical spacing
.		br					\" get last partial line
.		da					\" end diversion
.		ev					\" go back to previous environment
.		rr ;L ;f ;j ;l ;o ;s ;u ;v		\" free storage
])

|attribute(append.input; stack, default off;
	on $string.name [
.		eo
.		am $string.name\(ts\(ts
]	off [
.\(ts\(ts
.		ec
])

|attribute(invoke; nostack;
	on $offset $what  [
.		wh 15i $what				\" activate the trap
.		ch $what \n(nlu+$offset			\" move it in place
]	on number $$. $what  [
.		wh 15i $what				\" activate the trap
.		ch $what \n(nlu+\n$$.u			\" move it in place
]	on number $number.register $what  [
.		wh 15i $what				\" activate the trap
.		ch $what \(nlu+\n($number.registeru	\" move it in place
]	at constant $constant $what [
.		wh 15i $what				\" activate the trap
.		ch $what $constant			\" move it in place
]	at number $$. $what [
.		wh 15i $what 				\" activate the trap
.		ch $what \n$$.u				\" move it in place
]	at number $number.register $what [
.		wh 15i $what				\" activate the trap
.		ch $what \n($number.registeru		\" move it in place
]	end [
.		br					\" get last partial line
.		di					\" turn off diversions
]	off $what [
.		ch $what				\" remove the trap
])

|attribute(remember; nostack;
	$ [
.		mk $
])

|attribute(return; nostack;
	$$. [
.		sp |\n$$.u
.		ti \n(.iu				\" movement resets indentation
]	$ [
.		sp |\n($u
.		ti \n(.iu				\" movement resets indentation
])

|attribute(goto; nostack;
	$ [
.		sp |$
.		ti \n(.iu				\" movement resets indentation
])

|attribute(string; nostack;
	$$. [\*$$.]
	$string.register [\*($string.register]
	roman $$. [\f1\&\*$$.\fP\&]
	italics $$. [\f2\&\*$$.\fP\&]
	bold $$. [\f3\&\*$$.\fP\&]
	bolditalics $$. [\f4\&\*$$.\fP\&]
	cw $$. [\f(CW\&\*$$.\fP\&]
	roman $string.register [\f1\&\*($string.register\fP\&]
	italics $string.register [\f2\&\*($string.register\fP\&]
	bold $string.register [\f3\&\*($string.register\fP\&]
	bolditalics $string.register [\f4\&\*($string.register\fP\&]
	cw $string.register [\f(CW\&\*($string.register\fP\&])

|attribute(macro; nostack;
	$ [
.		$
])

|attribute(number; nostack;
	$$. [\n$$.]
	$number.register [\n($number.register]
	++ $$. [\n+$$.]
	++ $number.register [\n+($number.register]
	format $* $format [
|for i in $* {
.		af $i $format
}]
	incr $* $incr [
|for i in $* {
.		nr $i +0 $incr
}]
	format.from.string $* $$. [
|for i in $* {
.		af $i \*$$.
}]
	incr $* $incr [
|for i in $* {
.		nr $i +0 $incr
}]
	format.from.string $* $format [
|for i in $* {
.		af $i \*($format
}]
	incr $* $incr [
|for i in $* {
.		nr $i +0 $incr
}])

|attribute(list.pre; stack, default;
	[
.		rm premark_string
]
	$pre [
.		ds premark_string $pre
])

|attribute(list.format; stack, default;
	[
.		rm format_string
]
	$format [
.		ds format_string $format
])

|attribute(list.post; stack, default;
	[
.		rm postmark_string
]
	$post [
.		ds postmark_string $post
])

|attribute(list.indent; stack, default;
	[
.		rm indent_string
]
	$indent [
.		ds indent_string $indent
])

|attribute(list; nostack;
	off [
'\"	;i remembered indent TEMP
.		)C ;i indentsave_string \*(indentsave_string
.		in \n(;iu
.		)C list_counter listsave_string \*(listsave_string
.		rm ;i
]	on [
.		ds indentsave_string \n(.i \*(indentsave_string
.		in +\w'\*(indent_string'u
.		af list_counter 0
.		ds listsave_string \n(list_counter \*(listsave_string
.		nr list_counter 0 1
])

|attribute(file.information; nostack;
	[
.		lf __LINE__ __FILE__
])

|attribute(reference.style; nostack;
	acm [|reference_style(acm)
]	apa [|reference_style(apa)
]	att [|reference_style(att)
]	ieee [|reference_style(ieee)
]	lsa [|reference_style(lsa)
]	pami [|reference_style(pami)
]	spectrum [|reference_style(spectrum)
])

|attribute(reference.option; nostack;
	tm [|reference_style(same tm)
]	rp [|reference_style(same rp)
]	nroff [|reference_style(same nroff)
]	troff [|reference_style(same troff)
]	sort [|reference_style(same sort)
]	sequence [|reference_style(same sequence)
]	a [|reference_style(same a)
]	d [|reference_style(same d)
]	t [|reference_style(same t)
]	ad [|reference_style(same ad)
]	at [|reference_style(same at)
]	da [|reference_style(same da)
]	dt [|reference_style(same dt)
]	ta [|reference_style(same ta)
]	td [|reference_style(same td)
]	adt [|reference_style(same adt)
]	atd [|reference_style(same atd)
]	dat [|reference_style(same dat)
]	dta [|reference_style(same dta)
]	tad [|reference_style(same tad)
]	tda [|reference_style(same tda)
])

|attribute(reference; stack, default off;
	database [|reference_database<]
	include [|reference_include<]
	list [|reference_list<]
	on [|reference<]
	off [>])

|attribute(reference.placement; nostack;
	[|reference_placement
])

|attribute(table; stack, default off;
	on [
.		TS
]	off [
.		TE
])

|attribute(ipa; stack, default off;
	on [@ipa(]
	off [)])

|attribute(picture; stack, default off;
	on [
.		PS
]	off [
.		PE
])

|attribute(ped; stack, default off;
	on [
.		GS
]	off[
.		GE
])

|attribute(ideal; stack, default off;
	on [
.		IS
]	off[
.		IE
])

|attribute(equation; stack, default off;
	on [
'\"	:4 store for equations
.		br					\" output partial line before equation
.		di :4					\" begin diversion
.		EQ					\" begin equation
]	off [
'\"	:4 store for equations
'\"	:D amount of accumulate blank lines in diversion
'\"	:E position of last minimum space in diversion
'\"	:Y width of a center block diversion
'\"	10 the equation as a string
.		EN					\" end equation
.		br					\" get last partial line
.		di					\" end diversion of equation
.		rm :4					\" throw the equation away
|ifvalue center.block on [
.		if \w@\*(10\*(equation_string@ \{\
.			if \n(:Y<\w@\*(10@ .nr :Y \w@\*(10@ \" update width
\!\*(10\h'\\n(.lu-\w@\*(10\*(equation_string@u-\\n(.iu'\*(equation_string	\" output next expansion
.			rr :D :E \}]			\" pretend we've moved
|ifvalue center on [
.		if \w@\*(10\*(equation_string@ .tl \(ts\(ts\*(10\(ts\*(equation_string\(ts]
|ifnotvalue center.block on [ |ifnotvalue center on [
.		if \w@\*(10\*(equation_string@ \{\
.			pc				\" protect against use of % in eqn
.			tl \(ts\h'\n(.iu'\*(10\(ts\(ts\*(equation_string\(ts
.			pc % \}]]			\" give % back to tl
.		rm 10					\" free storage
])

|attribute(inline.equation; stack, default off;
	on []
	off [])

|attribute(graph; stack, default off;
	on [
.		G1
]	off [
.		G2
])

|attribute(bargraph; stack, default off;
	on [
.		B1
]	off [
.		B0
])

|attribute(computeroutput; stack, default off;
	on [
.		CW
]	off [
.		CN
])

|attribute(citation; nostack;
	save on [
.		CD "]
	save off string $$. [" "\*$$."
]	save off string $string.register [" "\*($string.register"
]	save off number $$. [" "\n$$."
]	save off number $number.register [" "\n($number.register"
]	save string $name $$. [\&\c
.		CD "$name" "\*$$."
]	save string $name $string.register [\&\c
.		CD "$name" "\*($string.register"
]	save number $name $$. [\&\c
.		CD "$name" "\n$$."
]	save number $name $number.register [\&\c
.		CD "$name" "\n($number.register"
]	remember on [\&\c
.		CU "]
	remember off ["]
	remember string $name [\&\c
.		CU "$name"])

|attribute(index; stack, default off;
	on [\&\c
.		tm ix: ]
	off [	\n'page_counter'
\&])

|attribute(contents; nostack;
	$category $level $sequence $text [
.		tm .TC	$category	$level	\*($sequence	\*($text	\n'page_counter'
])

|attribute(white.text; nostack;
	$text [|ifvalue verbatim off []|ifvalue verbatim on [$text]])

|attribute(nl.paragraph; nostack;
	[|ifvalue verbatim on [
]|ifvalue verbatim off []])

|attribute(format.type; stack, init none;
	model [
.		ds format_type model
]	acm [
.		ds format_type acm
]	none [
.		ds format_type none
])

|attribute(list.type; stack, default none;
	fixed []
	calculated []
	none [])

|attribute(list.spacing; stack, default off;
	off []
	on [])

|attribute(verbatim; stack, default off;
	off []
	on [])

|attribute(draft; stack, default off;
	off []
	on [])

|attribute(sectioning; stack, default bysection;
	bysection []
	bychapter []
	byappendix [])

|attribute(globalparagraph.style; stack, default flushed;
	indented []
	numbered []
	flushed [])

|attribute(localparagraph.style; stack, default flushed;
	indented []
	numbered []
	flushed [])

|attribute(inline; stack, default on;
	off []
	on [])

|attribute(footnote_width; stack, default normal;
	narrow []
	full []
	normal[])

|attribute(around_width; stack, default normal;
	narrow []
	full []
	normal[])

|attribute(sawreference; stack, default off;
	off []
	on [])

|attribute(sawabstract; stack, default off;
	off []
	on [])

|attribute(sawkeywords; stack, default off;
	off []
	on [])

|attribute(sawoverflow; stack, default off;
	off []
	on [])

|attribute(sawtitlebox; stack, default off;
	off []
	on [])

|attribute(sawendnote; stack, default off;
	off []
	on [])
