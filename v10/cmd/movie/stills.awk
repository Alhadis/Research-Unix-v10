BEGIN {
  # develop is set from first argument of form develop=filename

  fillstring =	"; line from GL.n  to GL.s; line from GL.e  to GL.w "\
		"; line from GL.ne to GL.sw; line from GL.nw to GL.se"
  set(textname, "small s medium m big b bigbig B")
  set(linename, "solid s fat f fatfat F")
  set(justtr, "l ljust r rjust a above b below")
  set(linetr, "o dotted a dashed"); linetr["s"] = linetr["f"] = linetr["F"] = ""
  set(arrowtr, "< <- > -> x <->"); arrowtr["-"] = ""
  set(convert, "dot \\v'-.2m'\\s+3.\\s-3\\v'.2m' "\
	"bullet \\v'.2m'\\s+2\\(bu\\s-2\\v'-.2m' "\
	"times \\s-3\\(mu\\s+3 circle \\s-3\\(ob\\s+3")
  instills = 0
}

instills == 0	      { if ($1 == ".begin" && /^\.begin[ \t]+stills/) {
				instills = 1; startstills()
			} else print
			next
		      }
/^\.end/	      {	instills = 0; endstills(); next }
END		      { if (instills) fatal("eof inside begin/end") }

		      {	inputline = $0; sub(/\#.*$/, "") }
/^[ \t]*$/	      { next }
$1 == "view"          { viewnum[$2] = viewct++
			if (NF <= 2) text = $2
			else {	text = rest(3)
				if (text ~ /^"/) {
				    if (text !~ /"$/) text = substr(text,2)
				    else text=substr(text,2,length(text)-2)
				}
			}
			inviewtitle[$2] = text
			next
		      }
$1 == "print"         {	sawprint = 1
			if ($2 == "all") printglobal = 1
			else if ($2 == "final") printfinal = 1
			else { # print <clickname> ...
				if ($3 == "all") { # print <clickname> all
					allnamearr[$2] = 1
				} else { # print <clickname> v1 v2 ...
					if (!($2 in snamenum))
						snamenum[$2] = snamenumct++
					for (i = 3; i <= NF; i++)
					    printsome[snamenum[$2] "." $i]=1
				}
			}
			next
		      }
/./		      {	# parameter assignments
			n = $2; sub(/^\+/, "", n); n = 0 + n
			if      ($1 == "file")		infile = $2
			else if ($1 == "across")	across = n
			else if ($1 == "down")		down = n
			else if ($1 == "margin")	margin = n
			else if ($1 == "frameht")	frameht = n
			else if ($1 == "framewid")	framewid = n
			else if ($1 in textname)	textinc[textname[$1]] = n
			else if ($1 in linename)	lineinc[linename[$1]] = n
			else if ($1 ~ /^times?$/) {
				wanttime = 1;  if ($2 == "invis") wanttime = 0
			} else if ($1 ~ /^frames?$/) {
				wantframe = 1; if ($2 == "invis") wantframe = 0
			} else warn("unrecognized command")
			next 
		      }

function rest(f,  s, i) { # rest of $0, starting at $f
	if (NF <= f) return $f
	s = $0
	sub(/^[ \t]+/, "", s)
	for (i = 1; i < f; i++)
		sub(/^[^ \t]+[ \t]+/, "", s)
	return s
}

function dump(time,  i, gtype, vnum, x1, x2, y1, y2,
		opts, text, delta, btype) {
	btype = ""; if (wantframe == 0) btype = " invis"
	if (goacross == 0) { # time goes down the page, views go across
		if (colcnt >= colmax) {
			delta = .1; if (wanttime) delta += .8
			print "A0: box invis wid 0 ht 0 with .sw at A0.sw + ("\
				viewct*framewid + delta " , 0)"
			print "B0: box invis wid 0 ht 0 with .sw at A0.sw"
			colcnt = 0
		}
		print "B0: box" btype " wid framewid ht frameht with .n at B0.s"
		for (i = 1; i < viewct; i++)
			print "B" i ":	box" btype\
			    " wid framewid ht frameht with .w at B" i-1 ".e"
		if (colcnt == 0)
			for (i = 0; i < viewct; i++)
				if (viewtitle[i] != "")
					print "\"" viewtitle[i] "\" at B"\
						viewbase[i] ".n + (0, .1)"
		if (wanttime)
			print "\"" time "\" ljust at B" viewct-1 ".e + (.1,0)"
	} else { # time goes across the page, views go down
		if (colcnt >= colmax) {
			delta = .1; if (wanttime) delta += .3
			print "A0: box invis wid 0 ht 0 with .c at A0.c - (0,"\
				viewct*frameht + delta ")"
			print "B0: box invis wid 0 ht 0 with .c at A0.c"
			colcnt = 0
		}
		print "B0: box" btype " wid framewid ht frameht with .w at B0.e"
		for (i = 1; i < viewct; i++)
			print "B" i ":	box" btype\
			    " wid framewid ht frameht with .n at B" i-1 ".s"
		if (colcnt == 0)
			for (i = 0; i < viewct; i++)
				if (viewtitle[i] != "")
					print "\"" viewtitle[i] "\" rjust at B"\
						viewbase[i] ".w - (.1, 0)"
		if (wanttime)
			print "\"" time "\" at B" viewct-1 ".s + (0,-.15)"
	}
	colcnt++
	for (i = 1; i <= slotcnt; i++)
		if (slot[i] != "") {
			if (slot[i] ~ /^g\t/) slot[i] = makepic(slot[i])
			print slot[i]
		}
	for (i = 1; i <= staticcnt; i++)
		print static[i]
}

function pos(x, y) {
	return sprintf("B%d.sw+(%.3g,%.3g)", basenum,\
		framexfac*(frameborder+x), frameyfac*(frameborder+y))
}

function makepic(istring,    fields, gtype, vnum, pstring, just,
			x1, x2, y1, y2, opts, text, i, t, linemode, arrow,
			inc, ss, es) { # convert int file string to pic
	fields = split(istring, f, "\t")
	gtype=f[3]; vnum=f[4]; opts=f[5]; x1=f[6]; y1=f[7]
	basenum = viewbase[vnum]
	if (gtype == "l") { # line
		x2 = f[8]; y2 = f[9]
		linemode = substr(opts,1,1) # solid,fat,fatfat,dotted,dashed
		arrow = substr(opts,2,1) # -,->,<-, <->
		pstring = "line " linetr[linemode] " " arrowtr[arrow]\
			  " from " pos(x1, y1) " to " pos(x2, y2)
		if (linemode != "f" && linemode != "F") linemode = "s"
		inc = lineinc[linemode]
		if (inc != 0) {
			if (inc < 0) { ss = inc; es = "+" 0-inc }
			else	     { ss = "+" inc; es = "-" inc }
			pstring = ".ps " ss "\n" pstring "\n.ps " es
		}
	} else if (gtype == "b") { # box
		x2 = f[8]; y2 = f[9]
		if (x1 > x2) { t = x1; x1 = x2; x2 = t }
		if (y1 > y2) { t = y1; y1 = y2; y2 = t }
		pstring = "GL: box with .sw at " pos(x1,y1)\
				" ht "  (y2-y1)*frameyfac\
				" wid " (x2-x1)*framexfac
		if (substr(opts,1,1) == "f") pstring = pstring fillstring
	} else if (gtype == "c") { # circle
		pstring = "GL: circle radius " framexfac*f[8]\
			" at " pos(x1, y1)
		if (substr(opts,1,1) == "f") pstring = pstring fillstring
	} else if (gtype == "t") { # text
		if (fields == 8) text = f[8]
		for (i = 9; i <= fields; i++) text = text "\t" f[i]
		if (text in convert) text = convert[text]
		just = justtr[substr(opts,1,1)]
		pstring = "\"" text "\" " just " at " pos(x1, y1)
		inc = textinc[substr(opts,2,1)]
		if (inc != 0) {
			if (inc < 0) { ss = inc; es = "+" 0-inc }
			else	     { ss = "+" inc; es = "-" inc }
			pstring = ".ps " ss "\n" pstring "\n.ps " es
		}
	} else warn("int file bug: illegal modifier after g")
	return pstring
}

function warn(s) {
	print "stills warning: " s >"/dev/tty"
	print "  near input line " NR ": " inputline >"/dev/tty"
}
function fatal(s) {
	print "stills fatal error: " s >"/dev/tty"
	print "  near input line " NR ": " inputline >"/dev/tty"
	exit
}

function startstills() {
	set(textinc, "s -2 m 0 b 3 B 8")
	set(lineinc, "s 0 f 3 F 8")
	infile = ""; sawprint = 0
	frameht = 1.5; framewid = 1.5; margin = 0.05
	wanttime = wantframe = 1 
	printglobal = printfinal = 0
	across = 0; down = -1
	clear(allnamearr)
	clear(allnumarr)
	clear(clickname)
	clear(inviewtitle)
	clear(printsome)
	clear(slot)	; slotcnt = 0
	clear(somenum)
	clear(snamenum)	; snamenumct = 0	
	clear(static)	; staticcnt = 0
	clear(timectr)
	clear(viewactive)
	clear(viewtitle)
	clear(viewnum)	; viewct = 0	
	print ".PS"
	print "A0: B0: box invis wid 0 ht 0"
}

function clear(a, i) {
	for (i in a) delete a[i]
}
function set(a, s,  i, n, q) {
	n = split(s, q)
	for (i = 1; i < n; i += 2)
		a[q[i]] = q[i+1]
}

function endstills() {
if (sawprint = 0) warn("no print statements")
if (viewct == 0) viewglobal = 1
if (down < 0) {
	goacross = 1; colmax = across; ratio = 6/framewid
} else {
	goacross = 0; colmax = down; ratio = 8/frameht
}
if (colmax <= 0) colmax = int(ratio)
inputrange = 10000			# 0..9999
frameborder = inputrange * margin	# border of whitespace
framexfac = framewid/(inputrange + 2*frameborder)
frameyfac =  frameht/(inputrange + 2*frameborder)
print "frameht  = " frameht
print "framewid = " framewid
if (infile == "") fatal("missing script file name")
if (infile !~ /\.s$/) fatal("script file name must end in .s")
system(develop " " infile)
sub(".s$", ".i", infile)
OFMT = "%.3g"
FS = "\t"
while ((getline <infile) > 0) {
	if ($1 == "g") {
		if (viewactive[$4]) {
			snum = $2
			if (snum > slotcnt) slotcnt = snum
			if (snum != 0) slot[snum] = $0
			else static[++staticcnt] = makepic($0)
		}
	} else if ($1 == "e") { # erase
		if (viewactive[$4]) slot[$2] = ""
	} else if ($1 == "c") { # click
		++timectr[$2]
		if (printglobal || allnumarr[$2] ||
			  (somenum[$2] "." timectr[$2] in printsome))
			dump(clickname[$2] ": " timectr[$2])
	} else if ($1 == "#") { # comment -- ignore it
	} else if ($1 == "b") { # blank the current view by enclosed erases
	} else if ($1 == "d") { # define
		if ($2 == "c") { # click
			clickname[$3] = $4
			allnumarr[$3] = 0
			if ($4 in allnamearr) allnumarr[$3] = 1
			somenum[$3] = -1
			if ($4 in snamenum) somenum[$3] = snamenum[$4]
		} else if ($2 == "v") { # view
			viewactive[$3] = 0
			if ($4 in viewnum) {
				viewactive[$3] = 1
				viewbase[$3] = viewnum[$4]
				viewtitle[$3] = inviewtitle[$4]
			} else if (viewglobal) {
				viewactive[$3] = 1
				viewbase[$3] = viewct++
				viewtitle[$3] = $4
			}
		} else if ($2 == "p") { # ``pragmas'' -- check here?
		} else warn("int file bug: illegal modifier following d")
	} else warn("int file bug: unrecognized command")
}
close(infile)
if (printfinal) dump("final")
print ".PE"
FS = " "
}
