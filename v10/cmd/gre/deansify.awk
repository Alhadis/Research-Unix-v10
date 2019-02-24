# De-ANSI-fy C programs

# change void* to char *
	{ gsub(/void *\*/, "char *") }

# remove args from function declarations and typedefs
# assume one per line (fitting on one line)

/^[a-zA-Z0-9_]+.*\([^(]*\);.*/ {
	if($0 !~ /^print\(/ && $0 !~ /^fprint\(/ && $0 !~ /^if\(/) {
		sub(/\([^(]*\);/, "();")
		print
		next
	}
    }

#    change function definition headers to old-style
#    function definition headers on one line, ending with ')',
#       with the return type (if not omitted) on previous line
#    assume no parentheses inside arg list

/^[a-zA-Z0-9_]+\(.*\)$/ {
	st = index($0, "(") + 1
	n = length($0) - st
	rawargs = substr($0, st, n)
	printf("%s(", substr($0, 1, st-2))
	if( rawargs == "void" ) {
		printf(")\n");
		next
	}
	nargs = split(rawargs, args, ", *")
	for(i = 1; i <= nargs; i++){
		if(! match(args[i], /[a-zA-Z0-9_]+ *$/)){
			if(! match(args[i], /[a-zA-Z0-9_]+\[.*\]$/))
				id = "OOPS"
			else {
				id = substr(args[i], RSTART)
				sub(/\[.*\]/, "", id)
			}
		} else
			id = substr(args[i], RSTART, RLENGTH)
		printf("%s", id)
		if(i < nargs) printf(", ")
	}
	printf(")\n")
	for(i = 1; i <= nargs; i++)
		printf("\t%s;\n", args[i])
	next
    }

# remove pragmas

/^#[    ]*pragma/ { next }

# just print remaining lines
	{ print }
