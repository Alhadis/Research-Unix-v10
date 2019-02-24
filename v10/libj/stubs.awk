BEGIN {

	FS=":";
	type["c"] = "char"
	type["c*"] = "char *"
	type["c**"] = "char **"
	type["i"] = "int"
	type["l"] = "long"
	type["s"] = "short"
	type["v"] = "void"

	type["B"] = "Bitmap"
	type["B*"] = "Bitmap *"
	type["C"] = "Code"
	type["F"] = "Font"
	type["F*"] = "Font *"
	type["M"] = "Menu"
	type["M*"] = "Menu *"
	type["P"] = "Point"
	type["P*"] = "Point *"
	type["R"] = "Rectangle"
	type["R*"] = "Rectangle *"
	type["T"] = "Texture"
	type["T*"] = "Texture *"

	s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz "
	# set upper["A"] = upper["a"] = "A"
	for (i = 1; i <= 27; i++) {
		upper[substr(s,i+27,1)] = substr(s,i,1)
		upper[substr(s,i,1)] = substr(s,i,1)
	}

	STUB = "stubs.c"
	printf "#include \"jerq.h\"\n" >STUB
	printf "#include \"commands.h\"\n" >STUB
	printf "#include \"io.h\"\n" >STUB
	printf "\n" >STUB

	COM = "commands.h"
	printf "#define CCBALLOC 1\n#define CCBFREE 2\n#define CCEXIT 3\n" >COM
	printf "#define CCFFREE 4\n#define CCGETFONT 5\n" >COM
	printf "#define CCMENUHIT 6\n#define CCMENU 7\n#define CCBSPLINE 8\n" >COM
	printf "#define CCGETMOUSE 9\n#define CCKBDCHAR 20\n" >COM
	printf "#define CCTRACKARC 10\n#define CCTRACKRECT 11\n" >COM
	printf "#define CCTRACKCIRCLE 12\n#define CCTRACKDISC 13\n" >COM
	printf "#define CCTRACKELARC 14\n#define CCTRACKELDISC 15\n" >COM
	printf "#define CCTRACKELLIPSE 16\n#define CCTRACKLINE 17\n" >COM
	printf "#define CCTRACKRECTF 18\n#define CCTRACKSTRING 19\n" >COM
	d = 21
}

func doSendRec(s,cmd) {
	printf "\t%s", cmd >STUB
	u = index(s,"u")
	tt = index(s,"**")
	t = index(s,"*")
	f = index(s,"*()")
	if (f>0) {
		printf "Function" >STUB
		s = type[substr(s,u+1,f-u-1)]
	}
	else if (tt>0) {
		printf "ToTo" >STUB
		s = type[substr(s,u+1,tt-u-1)]
	}
	else if (t>0) {
		printf "To" >STUB
		s = type[substr(s,u+1,t-u-1)]
	}
	else if (u>0) {
		printf "Unsigned" >STUB
		s = type[substr(s,u+1,length(s)-u)]
	}
	else s = type[s]
	s2 = substr(s,2,length(s)-1)
	s1 = upper[substr(s,1,1)]
	printf "%s%s", s1, s2 >STUB
	if (cmd == "send") printf "(arg%d);\n", i-2 >STUB
	else printf "();\n" >STUB
}

{	
	printf "%s %s(", type[$1], $2 >STUB
	for (i=3; i<=NF; i++) {
		if (i>3) 
			printf ", " >STUB
		printf "arg%d", i-2  >STUB
	}
	printf ")\n" >STUB
	for (i=3; i<=NF; i++) {
		u = index($i, "u")
		f = index($i,"*()")
		t = index($i,"*")
		if (u>0) printf "unsigned " >STUB
		if (f>0) 
		 printf "%s (*arg%d)();\n", type[substr($i,u+1,f-u-1)], i-2 >STUB
		else {
			printf "%s", type[substr($i,u+1,length($i)-u)] >STUB
			if (t==0) printf " " >STUB
			printf "arg%d;\n", i-2 >STUB
		}
	}
	printf "{\n" >STUB
	printf "\tsendCommand(CC" >STUB
#	printf "\t" >COM
	printf "#define CC" >COM 
	for (i=1; i<=length($2); i++) {
		printf "%s", upper[substr($2,i,1)] >STUB
		printf "%s", upper[substr($2,i,1)] >COM	
	}
	printf ");\n" >STUB
#	printf ",\n" >COM
	printf " %d\n", d >COM
	d = d+1
	for (i=3; i<=NF; i++) doSendRec($i,"send")
	if ($1 != "v") doSendRec($1,"flush();\n\treturn rec")
	printf "}\n\n" >STUB
}

#END {
#	printf "};\n" >COM
#}
