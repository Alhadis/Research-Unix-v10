	data("b(op,l,r)")
	data("u(op,r)")
	data("i_fcall(name,head,tail)")
	data("fcall(name,args,l,r)")
	data("argexp(exp,next)")

	stack = table()
	bconv = table(30)
	deflist = table(50)
	inctab = table()
	stno_tab = table(100)

	# The "binfo" structure contains the information needed to
	# map Snocone binary operators into SNOBOL4 binary operators.
	# The significance of the fields is as follows:
	#
	#	out	The corresponding SNOBOL4 operator
	#	lp	The operator priority when it's on the left
	#		side of a precedence comparison
	#	rp	The operator priority when it's on the right
	#		side of a precedence comparison.  lp is always
	#		equal to rp or rp-1; if equal, the operator is
	#		left-associative, otherwise right-associative.
	#	slp	Like lp, but for the SNOBOL4 operator
	#	srp	Like rp, but for the SNOBOL4 operator
	#	fn	Non-null if this operator translates into a
	#		call to a built-in function instead of an operator.
	data("binfo(out,lp,rp,slp,srp,fn)")

	# Paren isn't really an operator, but precedence comparisons
	# work out more easily if bconv has an entry for them.
	bconv['('] = par_binfo = binfo('',0)

	bconv['='] = binfo('=',1,2,0,1)
	bconv['?'] = binfo('?',2,2,1,1)
	bconv['|'] = binfo('|',3,3,2,2)
	bconv['||'] = or_binfo = binfo('',4,4,0,0,1)
	bconv['&&'] = cat_binfo = binfo(' ',5,5,4,4)
	bconv['>'] = binfo('GT',6,6,0,0,1)
	bconv['<'] = binfo('LT',6,6,0,0,1)
	bconv['>='] = binfo('GE',6,6,0,0,1)
	bconv['<='] = binfo('LE',6,6,0,0,1)
	bconv['=='] = binfo('EQ',6,6,0,0,1)
	bconv['!='] = binfo('NE',6,6,0,0,1)
	bconv['::'] = binfo('IDENT',6,6,0,0,1)
	bconv[':!:'] = binfo('DIFFER',6,6,0,0,1)
	bconv[':>:'] = binfo('LGT',6,6,0,0,1)
	bconv[':<:'] = binfo('LLT',6,6,0,0,1)
	bconv[':>=:'] = binfo('LGE',6,6,0,0,1)
	bconv[':<=:'] = binfo('LLE',6,6,0,0,1)
	bconv[':==:'] = binfo('LEQ',6,6,0,0,1)
	bconv[':!=:'] = binfo('LNE',6,6,0,0,1)
	bconv['+'] = binfo('+',7,7,5,5)
	bconv['-'] = binfo('-',7,7,5,5)
	bconv['/'] = binfo('/',8,8,7,7)
	bconv['*'] = binfo('*',8,8,8,8)
	bconv['%'] = binfo('REMDR',8,8,0,0,1)
	bconv['^'] = binfo('**',9,10,10,11)
	bconv['.'] = binfo('.',10,10,11,11)
	bconv['$'] = binfo('$',10,10,11,11)

	ht = char(9)
	optblank = nspan(" " && ht)
	blank = span(" " && ht)
	digits = "0123456789"
	letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"

	integer = span(digits)
	exponent = any("eEdD") && (any("+-") | "") && integer
	real = integer && "." && (integer | "") && (exponent | "") |
		integer && exponent | "." && integer && (exponent | "")
	number = real | integer

	string = any("'" && '"') $ squote && break(*squote) && len(1)

	constant = number | string
	identifier = any(letters) && nspan(letters && digits)
	unaryop = any("+-*&@~?.$")
	binaryop = "==" | "!=" | "<=" | ">=" | "&&" | "||" | ":==:" |
		":!=:" | ":>:" | ":<:" | ":>=" | ":<=:" | "::" | ":!:" | 
		any("+-*/<>=^.$?|%")

	fcall = identifier . *mkfcall() && optblank &&
		(("(" && list(*exp . *invoke(.mkarg), optblank && ",") &&
		optblank && ")" . *invoke(.endfc)) |
		("[" && list(*exp . *invoke(.mkarg), optblank && ",") &&
		optblank && "]" . *invoke(.endfc) . *invoke(.mkarray)))

	term = optblank && (constant . *push() . *dotck() |
		identifier . *push() | "(" && *exp && optblank && ")" | fcall)
	operand = term | optblank && unaryop . *push() &&
		*operand . *invoke(.unop)
	exp = "" . *begexp() && *operand &&
		arbno(optblank && binaryop . *push() . *invoke(.mkbinfo) &&
		*operand . *invoke(.binop)) && "" . *endexp()

	label = optblank && identifier . lab . *emitlab(lab) && optblank && ":"

	clausend = any("{}") . del | rpos(0)

	clause = fence && arbno(label) && optblank &&
		("if" . cl_type && optblank && "(" && *exp && optblank && ")" |
		"while" . cl_type && optblank && "(" && *exp && optblank && ")" |
		(kw("return") | kw("freturn") | kw("nreturn")) . cl_type && optblank &&
			("" . *push() | *exp) &&
			optblank && clausend |
		"go" && optblank && "to" && blank &&
			identifier . dest . *invoke(.gocl) |
		("{" | "}") . cl_type |
		("do" | kw("else")) . cl_type && (span(" " && ht) | rpos(0)) |
		"procedure" . cl_type && blank && identifier . fname |
		"struct" . cl_type && blank && identifier . stname |
		"for" . cl_type && optblank &&
			"(" && *exp && optblank && "," && *exp && optblank &&
			"," && *exp && optblank && ")" |
		rpos(0) . cl_type && *invoke(.emiteos) |
		*exp && optblank && clausend . *invoke(.expcl))

	gl_files = table()
	gl_lines = table()
	gl_index = -1

	emit_stno = 0

	&stlimit = 1000000000
#
#	save the current state -- we really begin execution here.
	exit(3)
#
#	Establish a starting point for input files
	gl_arg = host(3) - 1
	if (~host (2, host(3))) {
		output = "snocone: nothing to compile"
		go to end
	}

	if (~gl_nextfile()) {
		go to end
	}
#
#	establish the "object" file
	outfile = "a.out"
	&errlimit = &errlimit + 1
	savexit = setexit()
	if (~output(.outf,-1,outfile)) {
		output = "cannot write " && outfile
		go to end
	}
	&errlimit = &errlimit - 1
	setexit (savexit)

	outf = "#!/usr/bin/spitbol -s16k"
#
#	Permanent prologue
	emitlab("MAIN.")
#
#	The main loop.  We expect to read a series of statements.
	while (nclause (1)) {
		if (ident (cl_type, "procedure"))
			funct()
		else if (ident (cl_type, "struct"))
			dostruct()
		else
			dostmt()
	}


#	Epilogue
exit:
	emitg("END")
	emitlab("START.")
	emit("CODE('START.')")
	for (i = 1, i <= deflist[0], i = i + 1) {
		emiteos()
		emit(deflist[i])
	}

	# put out code to trap run-time errors
	emiteos()
	emit("&ERRLIMIT = 1")
	emiteos()
	emit("SETEXIT(.err.exit)")

	# put out code to assign the statement map
	emiteos()
	emit ("err.map = '")
	while (bst_stab ? len(50) . bst_chunk = "") {
		emit (bst_chunk && "'")
		emiteos()
		emitlab("+")
		emit("'")
	}
	emit (bst_stab && "'")

	emitg("MAIN.")
	emiteos()

#	Epilogue
	input(.inf,-2,'/usr/lib/snocone/epilogue')
	while (line = inf)
		outf = line
	endfile(-2)

	endfile(-1)
	host (1, "chmod +x " && outfile)



#			subroutines




#	like span, but the pattern returned
#	can also match the null string
procedure nspan (str) {
	return span (str) | ""
}

#	a pattern that matches a list of zero or more
#	"item"s separated by "delim"s
procedure list (item, delim) {
	return item && arbno (delim && item) | ""
}

#	a pattern that matches the keyword given by the
#	argument, insisting that it be followed by a non-letter.
procedure kw (s) {
	return span(letters) $ dummy &&
		convert("ident(dummy,'" && s && "')", "EXPRESSION")
}

#	return the name of the (new) top stack element
procedure push() {
	stackptr = stackptr + 1
	nreturn  .stack[stackptr]
}

#	return the value of the (old) top stack element
procedure pop() {
	pop = stack[stackptr]
	stack[stackptr] = ""
	stackptr = stackptr - 1
}

#	return the name of the stack element n away from the top
procedure peek (n) {
	if (n >= stackptr)
		go to err
	nreturn .stack[stackptr - n]
}

#	top()
#	return the name of the top stack element
procedure top() {
	nreturn .stack[stackptr]
}

#	isbin(x)
#	is x a structure describing a binary operator?
#	things like == and ||, which syntactically look
#	more like functions than operators in their snobol form,
#	are considered not to be operators.
procedure isbin (x) {
	if (differ (datatype (x), 'B') || differ (fn (op (x))))
		freturn
}

#	isneg(x)
#	is x a structure describing a unary negation operator?
procedure isneg (x) {
	if (differ (datatype (x), 'U') || differ (op (x), '~'))
		freturn
}

#
#	print an expression in snobol form
procedure dprint (x) op, l, r, d, i, del {
	d = datatype(x)
	if (ident (d, 'STRING')) {
		emit (x)
		return
	}

	if (ident (d, 'U')) {
		# unary operator
		emit (op (x))
		if (isbin(r(x)))
			emit('(')
		dprint(r(x))
		if (isbin(r(x)))
			emit(')')
		return
	}


	if (ident (d, 'FCALL')) {
		# function call or array reference
		emit (name (x))
		emit (l (x))
		r = args (x)
		while (differ (r)) {
			emit (del)
			dprint (exp (r))
			del = ','
			r = next (r)
		}
		emit (r (x))
		return
	}

	if (ident (d, 'B')) {
		# binary operator
		op = op (x)
		if (ident (op, or_binfo)) {
			emit ('(')
			bprint (x)
			emit (')')
			return
		}
		l = isbin(l(x)) && slp(op(l(x))) < srp(op) && 1 || ""
		r = isbin(r(x)) && slp(op) > srp(op(r(x))) && 1 || ""

		# check for [f](a,b)
		if (differ (fn (op))) {
			emit(out(op))
			emit('(')
			dprint(l(x))
			emit(',')
			dprint(r(x))
			emit(')')
			return
		}

		# ordinary binary operator
		if (differ (l))
			emit ('(')
		dprint(l(x))
		if (differ (l))
			emit (')')
		emitb(out(op))
		if (differ(r))
			emit('(')
		dprint(r(x))
		if (differ(r))
			emit(')')
		return
	}

	# unknown datatype -- this "shouldn't happen"
	i = 1
	emit(d)
	emit ('(')
	while (dprint (apply (field (d, i), x))) {
		i = i + 1
		emit (',')
	}
	emit (')')
}

#	bprint(x)
#	subroutine of dprint -- used to handle printing of
#	things of the form (a,b), which are inherently
#	associative and can therefore be grouped as follows
#	((a,b),c) <=>  (a,(b,c)) <=>  (a,b,c)
procedure bprint (x) {
	if (differ (datatype(x), 'B') || differ (op(x), or_binfo)) {
		dprint (x)
		return
	}
	bprint(l(x))
	emit(',')
	bprint(r(x))
}

#	sprint(x)
#	like dprint, but print in a form appropriate for
#	an entire statement.  This procedure exists
#	because if the top level operator is a concatenation,
#	it is necessary to enclose the whole thing in parentheses.
#	Otherwise it would be mistaken for a pattern match.
procedure sprint (x) {
	if (ident(datatype(x),'B') && ident(op(x),cat_binfo))
		emit('(')
	dprint(x)
	if (ident(datatype(x),'B') && ident(op(x),cat_binfo))
		emit(')')
	emiteob()
}

#	invoke(f)
#	call an argument-free function in a context where
#	a name is required, such as    arb . *invoke(.foo)
procedure invoke (f) {
	apply (f)
	nreturn .dummy
}

#	a unary operator has been detected during parsing
procedure unop() r, op {
	r = pop()
	op = pop()
	push() = u(op,r)
}

#	mkfcall()
#	Parsing has detected the beginning of a function call
procedure mkfcall() {
	push() = i_fcall()
	nreturn .name(top())
}

#	parsing has detected an argument to a function
procedure mkarg() x, f {
	x = argexp(pop(),"")
	f = top()
	if (differ(tail(f)))
		next(tail(f)) = x
	tail(f) = x
	head(f) = ident(head(f)) && x
}

#	parsing has detected the end of a function call
procedure endfc() f {
	f = pop()
	push() = fcall(name(f),head(f),'(',')')
}

#	the fcall at the head of the stack is really an array
procedure mkarray() t {
	t = top()
	l(t) = '<'
	r(t) = '>'
}

#	the beginning of an expression has been detected
procedure begexp() {
	push() = bconv['(']
	nreturn .dummy
}

#	a binary operator has been detected.  We handle
#	precedence here rather than in the grammar
#	because it is less work.
procedure binop() l, r, op, newr, newop {
	while (lp(peek(3)) >= rp(peek(1))) {
		newr = pop()
		newop = pop()
		r = pop()
		op = pop()
		l = pop()
		push() = b(op,l,r)
		push() = newop
		push() = newr
	}
}

#	the end of an expression has been detected
procedure endexp() l, r, op {
	while (differ (peek (1), par_binfo)) {
		r = pop()
		op = pop()
		l = pop()
		push() = b(op,l,r)
	}
	r = pop()
	pop()
	push() = r
	nreturn .dummy
}

#	locate the binfo structure that describes the
#	binary operator whose input character representation
#	has been placed on the top of the stack.
procedure mkbinfo() op {
	op = bconv[pop()]
	if (ident(op))
		go to err
	push() = op
}

#	dotck()
#	if necessary, append a leading zero to a floating-point
#	constant that begins with a decimal point.  The idea
#	that .5 is syntactically correct but semantically illegal
#	is just too scary to leave in.
procedure dotck() {
	top() ? fence && '.' = '0.'
	nreturn .dummy
}

#	write label l to the output
procedure emitlab (l) {
	if (differ(l)) {
		emiteos()
		st_lab = l
	}
	nreturn .dummy
}

#	put string s in the output
procedure emit (s) {
	if (differ(emit_eob))
		emiteos()
	st_body = st_body && s
}

#	we are done with the body of the generated statement
procedure emiteob() {
	if (ident (emit_eob)) {
		buildstab (emit_stno, gi_file, gi_line)
		emit_eob = 1
	}
}

#	write success branch l
procedure emits (l) {
	emiteob()
	st_s = l
}

#	emitf(l)
#	write failure branch l
procedure emitf (l) {
	emiteob()
	st_f = l
}

#	write unconditional branch l
procedure emitg (l) {
	emiteob()
	st_s = ident(st_s) && l
	st_f = ident(st_f) && l
}

#	write s surrounded by blanks
procedure emitb (s) {
	emit(' ')
	if (differ (s, ' ')) {
		emit (s)
		emit(' ')
	}
}

#	emiteos()out,goto
#	we are done with the entire statement
procedure emiteos() out, goto, s, del {
	emit_eob = ""
	if (differ(st_lab) || differ(st_body) || differ(st_s) || differ(st_f)) {
		emit_stno = emit_stno + 1
		out = st_lab && " " && st_body
		if (differ (st_s) || differ (st_f)) {
			goto = " :"
			if (ident (st_s, st_f))
				goto = goto && "(" && st_s && ")"
			else {
				if (differ (st_s))
					goto = goto && "S(" && st_s && ")"
				if (differ (st_f))
					goto = goto && "F(" && st_f && ")"
			}
		}
		out = out && goto
		while (size(out) >= 70) {
			out ? fence &&
				(arbno(break(" '" && '"') &&
				(" " | any("'" && '"') $ del &&
				break(*del) && len(1))) $ s &&
				*(size(s) > 50)) . outf = "+"
		}
		outf = out
		st_lab = st_body = st_s = st_f = ""
	}
}

# attempt to read an input line, taking #include into account
procedure getline() x, file, del, dir {
	do {
		# Try to read a line from the current file
		while (x = gl_in) {

			# We have a line: count it
			gl_line = gl_line + 1

			# If it's not an include statement, we're done.
			if (~(x ? fence && "#" && *optblank && "include" &&
			    *optblank && any('"<{' && "'") $ del &&
			    break(*replace(del,'<{','>}')) . file &&
			    len (1) && *optblank && rpos(0)))
				return x

			# If the name is enclosed in quotes and
			# relative, then it is relative to the
			# directory containing the currently included
			# file, if any.  If it is enclosed in brackets,
			# it is relative to a canonical directory.
			if ("'" && '"' ? del) {
				if (substr (file, 1, 1) :!=: '/') {
					if ((gl_file && '/') ? fence &&
					    (breakx('/') && len(1)) . dir &&
					    break('/') && len(1) && rpos(0))
						file = dir && file
				}
			} else
				file = "/usr/lib/snocone/" && file

			# If the name was enclosed by single quotes
			# or set brackets, ensure the particular file
			# was included only once.  Right now, we're pretty
			# literal-minded about when two files are really
			# the same:  'x' and './x' are different, for instance.
			if (('"<' ? del) || ident (inctab[file])) {
				inctab[file] = 1
				gl_open (file)
			}
		}

		# We've reached the end of this file.
		gl_close()
	} while (gl_index >= 0 || gl_nextfile());

	freturn
}

procedure gl_nextfile() {
	gl_arg = gl_arg + 1
	if (~gl_open (host (2, gl_arg)))
		freturn
}

procedure gl_close() {
	endfile (gl_index)
	gl_index = gl_index - 1
	gl_file = gl_files[gl_index]
	gl_line = gl_lines[gl_index]
	if (gl_index >= 0)
		input(.gl_in, gl_index)
	gl_files[gl_index] = gl_lines[gl_index] = ""
}

procedure gl_open (file) t {
	gl_files[gl_index] = gl_file
	gl_lines[gl_index] = gl_line
	gl_index = gl_index + 1
	gl_line = 0
	gl_file = file
	&errlimit = &errlimit + 1
	t = setexit()
	if (input (.gl_in, gl_index, file)) {
		setexit(t)
		&errlimit = &errlimit - 1
		return
	}
	setexit(t)
	gl_close()
	error ("cannot read " && file)
	freturn
}

# Attempt to read an input line, return on ultimate failure.
# This procedure strips comments and handles continuation lines.
procedure getinput (recur) del, line {

	# have we already encountered the last EOF?
	if (differ (gi_eof))
		freturn

	if (line = line && getline()) {

		# if this is the first line, remember its identity
		if (ident (recur)) {
			gi_file = gl_file
			gi_line = gl_line
		}

		# strip comments
		line ?
		    fence &&
			(arbno (break ("'" && '"') &&
			    len(1) $ del && break (*del) && len(1)) &&
			break ("'" && '"#')) . line && "#"
		
		# check for continuation
		if (line? any("@$%^&*(-+=[<>|~,?:") && optblank && rpos(0))
			line = line && getinput(1)

		return line
	}

	# we're out of input - signal final eof
	gi_eof = 1
	freturn
}

#	phrase()
#	return the next phrase from the input
procedure phrase() del {
	if (ph_buf ? fence && optblank && rpos(0)) {
		if (ph_buf = phbuf && getinput())
			return phrase()
		else
			freturn
	}

	if (ph_buf ? fence && arbno(break('"' && "';") && fence &&
	    (any('"' && "'")$del && break(*del) && len(1) |
	    "")).phrase && ';'  = '')
		return
	phrase = ph_buf
	ph_buf = ''
}

#	return a new label
procedure newlab() {
	nl_count = nl_count + 1
	return "L." && nl_count
}

#	return a new label and place it on the current statement.
#	If the current statement already has a label, use that.
procedure marklab() {
	if (differ (st_lab) && ident (emit_eob))
		return st_lab
	marklab = newlab()
	emitlab (marklab)
}

#	little routines to indicate what type of clause was read

#	expression clause
procedure expcl() {
	cl_type = "exp"
}

#	goto clause
procedure gocl() {
	cl_type = "goto"
}

#	read a new clause and classify it
#	if end of input, error unless "okeof" argument is non-null,
#	in which case we merely fail
#	if rep_clause is set, give us the last clause again
procedure nclause (okeof) del {
nclause_start:
	if (differ (rep_clause)) {
		rep_clause = ""
		if (ident (eof))
			return
		else
			freturn
	}
	if (linebuf ? fence && *optblank && rpos(0)) {
		if (linebuf = phrase())
			go to nclause_start

		#	end of input
		if (ident(okeof)) {
			error ('premature EOF')
			go to exit
		}
		eof = 1
			freturn
	}

	#	we really have some input
	if (linebuf ? clause = del)
		return
	error("syntax error")
	linebuf = ""
	go to nclause_start
}

procedure error (msg) prefix {
	if (ident (gl_file))
		prefix = "snocone"
	else
		prefix = gl_file && "(" && gl_line && ")"
	terminal = prefix && ": " && msg
	&code = 1
}

#	handle a statement
procedure dostmt() lab, lab2, e1, e2, e3, flip {

	if (ident(cl_type,"exp")) {
		#	The clause is an expression,
		#	so that's the whole statement
		sprint(pop())
		return
	}

	#	It might be a sequence of statements in braces
	if (ident(cl_type,"{")) {
		nclause()
		while (differ (cl_type, "}")) {
			dostmt()
			nclause()
		}
		return
	}

	#	It might be a goto statement
	if (ident (cl_type, "goto")) {
		emitg (dest)
		return
	}

	#	It might be an if statement
	if (ident(cl_type,"if")) {
		e1 = pop()

		# optimize "if (~expr)"
		if (isneg (e1)) {
			flip = 1
			e1 = r(e1)
		}

		sprint(e1)

		#	Check for if(...)goto
		nclause()
		if (ident(cl_type,"goto")) {
			if (ident (flip))
				emits(dest)
			else
				emitf(dest)

			#	In the case of if (e) goto l; else ...
			#	we can pretend the else wasn't there
			if (~nclause(1) || differ (cl_type, "else")) {
				rep_clause = 1
				emitlab (lab)
				return
			}
			nclause()
			dostmt()
			return
		}

		#	Not if...goto, emit conditional jump over
		#	the statement which follows.
		lab = newlab()
		if (ident (flip))
			emitf(lab)
		else
			emits(lab)
		dostmt()

		#	Check for else clause
		if (nclause (1) && ident (cl_type, "else")) {

			#	There is indeed an else clause
			lab2 = newlab()
			emitg(lab2)
			emitlab(lab)
			nclause()
			dostmt()
			emitlab(lab2)
			return
		}

		#	No else clause; we must look at this clause again later
		rep_clause = 1
		emitlab(lab)
		return
	}

	#	Check for a while clause
	if (ident(cl_type,"while")) {
		lab = marklab()

		# optimize "while(~exp)"
		e1 = pop()
		if (isneg (e1)) {
			flip = 1
			e1 = r(e1)
		}

		sprint(e1)
		lab2 = newlab()
		if (ident (flip))
			emitf(lab2)
		else
			emits(lab2)
		nclause()
		dostmt()
		emitg(lab)
		emitlab(lab2)
		return
	}

	#	Check for a do clause
	if (ident(cl_type,"do")) {
		lab = marklab()
		nclause()
		dostmt()
		nclause()
		if (differ(cl_type,"while")) {
			error ("expected 'while', found " && cl_type)
			rep_clause = 1
			return
		}
		e1 = pop()
		if (isneg (e1)) {
			flip = 1
			e1 = r (e1)
		}
		sprint(e1)
		if (ident (flip))
			emits (lab)
		else
			emitf (lab)
		return
	}

	#	Check for a "for" clause
	if (ident(cl_type,"for")) {
		e3 = pop()
		e2 = pop()
		e1 = pop()
		sprint(e1)
		emiteob()
		lab = marklab()
		lab2 = newlab()
		if (isneg (e2)) {
			flip = 1
			e2 = r (e2)
		}
		sprint(e2)
		if (ident (flip))
			emitf (lab2)
		else
			emits (lab2)
		nclause()
		dostmt()
		sprint(e3)
		emitg(lab)
		emitlab(lab2)
		return
	}

	#	could be some kind of return statement
	if (cl_type ? "return") {
		e1 = pop()
		if (differ(e1)) {
			if (differ(fname))
				e1 = b(bconv["="],fname,e1)
			sprint(e1)
		}
		emitg (replace(cl_type,
			"abcdefghijklmnopqrstuvwxyz",
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
		return
	}

	#	could even be a null statement
	if (ident(cl_type))
		return

	error("bad " && cl_type && " clause, ignored")
}

#	We have seen "struct" -- parse the 'declaration'
procedure dostruct() args {
	if (expect ('{')) {
		args = getlist ('}')
		deflist[deflist[0] = deflist[0] + 1] =
			"DATA('" && stname && "(" && args && ")')"
	} else
		error ("bad structure definition")
	expect ('}')
}

#	We have seen "procedure" -- we must now parse the header
procedure funct() args, locals, flabel {
	if (expect('(')) {
		if (~(args = getlist(')')))
			go to fu_error
		expect(')')
		if (~(locals = getlist('{')))
			go to fu_error
	}

	deflist[deflist[0] = deflist[0] + 1] =
		"DEFINE('" && fname && '(' && args && ')' && locals && "')"

	#	if we just emitted the end of a previous procedure,
	#	we can jump around this one in one go
	if (ident (emit_eob) && st_lab ? ".END") {
		flabel = st_lab
		st_lab = ""
		emitlab(fname)
		nclause()
		dostmt()
		emitg("RETURN")
		emitlab(flabel)
		return
	}

	emitg(fname && '.END')
	emitlab(fname)
	nclause()
	dostmt()
	emitg("RETURN")
	emitlab(fname && '.END')
	return

fu_error:
	error("bad function definition")
}

#	the input should now contain something matching "p"
#	possibly surrounded by white space.  If not, fail
procedure expect (p) {

	# throw away blank lines
	while (linebuf ? fence && optblank && rpos (0)) {
		if (~(linebuf = phrase()))
			freturn
	}

	# try to match the given pattern, possibly preceded by white space
	if (linebuf ? fence && optblank && *p = "")
		return

	# didn't match: fail
	freturn
}

#	expect an identifier in the input; return it.
procedure getid() {
	if (expect (*identifier . getid))
		return
	freturn
}

#	expect a list of identifiers followed by tail
procedure getlist (tail) del {
getlist_start:
	if (expect(tail)) {
		linebuf = tail && linebuf
		return
	}
	if (getlist = getlist && del && getid()) {
		expect(',')
		del = ','
		go to getlist_start
	}
	if (expect(tail))
		return
	freturn
}

# This procedure is called once for each output statement.
# It maintains the correspondence between Snocone and SNOBOL4
# statements, which is inserted into the output for debugging.
procedure buildstab (stmt, file, line) desc, pad {
	pad = dupl ("?", stmt - bst_stmt - 1)
	bst_stab = bst_stab && pad
	if (ident (file, bst_file)) {
		if (differ(pad) || line != bst_line + 1)
			desc = line
	} else
		desc = file && ":" && line
	bst_stab = bst_stab && desc && ","
	bst_stmt = stmt
	bst_file = file
	bst_line = line
}
