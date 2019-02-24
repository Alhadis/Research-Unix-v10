BEGIN {
  print "structure Opcodes = struct"
  print "val andb = Bits.andb"
  print "fun lshift(op1,amt) = "
  print "    if amt<0 then Bits.rshift(op1,0-amt)"
  print "    else Bits.lshift(op1,amt)"
  print "nonfix sub"	# bug fixes; want [[sub]] to be a MIPS opcode
  print "nonfix div"	# bug fixes; want [[div]] to be a MIPS opcode
  
  decode = "mipsdecode.sml";
  print "structure MipsDecode = struct" > decode
  print "val andb = Bits.andb" > decode
  print "fun rshift(op1,amt) = " > decode
  print "    if amt<0 then Bits.lshift(op1,0-amt)" > decode
  print "    else Bits.rshift(op1,amt)" > decode
  OPCODE = 1 ; SPECIAL = 2 ; BCOND = 3 ; COP1 = 4
  stderr="/dev/tty"
}
function mlnumber(n, s) {
	if (n<0) s = sprintf("~%d", -n)
	else s = sprintf("%d", n)
	return s
}
function min(x,y){
	if (x<y) return x
	else return y
}
function function_definition(name, argc,  i, temp) {
	if (argc==0) {
		temp = sprintf("val %s = ", name)
	} else {
		temp = sprintf( "fun %s(", name)
		for (i=1; i< argc; i++) temp = sprintf("%sA%d,", temp,i)
		temp = sprintf( "%sA%d) = ", temp, argc)
	}
	return temp
}
function insist_fields(n) {
	if (NF != n) {
		print "Must have", n, "fields on line",NR ":", $0 > stderr
		return 0
	} else {
		return 1
	}
}
NF == 1 && $1 == "opcode" {
	startline = NR
	opcodes = 1
	next
}
NF == 1 && $1 == "special" {
	startline = NR
	specials = 1
	next
}
NF == 1 && $1 == "bcond" {
	startline = NR
	bconds = 1
	next
}
NF == 1 && $1 == "cop1" {
	startline = NR
	cop1s = 1
	next
}
NF == 0 {opcodes = 0; specials = 0; bconds = 0; cop1s = 0
	fields = 0
        instructions= 0
}
opcodes || specials || bconds || cop1s {
	if (!insist_fields(8)) next
	type = OPCODE * opcodes + SPECIAL * specials + BCOND * bconds + COP1 * cop1s
	major = NR - startline - 1		# major octal digit from row
	for (i=1; i<= NF; i++) {
		minor = i-1			# minor octal digit from column
		code = minor + 8 * major
		if ($i != "*") {
	        	numberof[$i] = code
	        	typeof[$i] = type
	        	opcode[type,code] = $i
	        } else {
	        	opcode[type,code] = "reserved"
	        }
	}
}
NF == 1 && $1 == "fields" {
	startline = NR
	fields = 1
	print "val S_fmt = 16+0"
        print "val D_fmt = 16+1"
        print "val W_fmt = 16+4"

	next
}
fields {
	if (!insist_fields(3)) next
	fieldname = $1;  low = $2; high = $3
	if (substr(fieldname,1,1)=="+") {
        	signed = 1
        	fieldname = substr(fieldname,2)
        } else {
        	signed = 0
        }
	fieldnames[fieldname]= 1	# rememeber all the field names

	if (low >= 16) {
        	printf "%s", function_definition(fieldname "LO",1); print "0"
        } else {
        	printf "%s", function_definition(fieldname "LO",1)
                printf "andb(lshift(A1,%d),65535)\n", low
        }
        if (high < 16) {
        	printf "%s", function_definition(fieldname "HI",1); print "0"
        } else {
        	printf "%s", function_definition(fieldname "HI",1)
                printf "lshift(A1,%s)\n", mlnumber(low - 16)
        }
	printf "%s", function_definition("THE" fieldname,2) > decode
        if (signed) printf "let val n = " > decode
        if (low >= 16) {
        	printf "0" > decode
        } else {
                printf "andb(rshift(A2,%d),%d)", low,
        			(2**(min(15,high)-low+1)-1) > decode
        }
        printf " + " > decode
        if (high < 16) {
        	printf "0\n" > decode
        } else {
                printf "rshift(andb(A1,%d),%s)\n", (2**(high-16+1)-1),
        			mlnumber(low - 16) > decode
        }
        if (signed) {
        	printf "in if n < %d then n else n - %d\nend\n",
        		2**(high-low), 2**(high-low+1) > decode
        }


}
NF == 1 && $1 == "instructions" {
	startline = NR
	instructions = 1
	next
}
instructions && $0 !~ /^#/ {
	opname = $1

	temp = "\"" opname " \""
        for (i=2; i<=NF; i++) {
        	temp = sprintf( "%s ^ \"%s = \" ^ S%s", temp, $i, $i)
        	if (i<NF) temp = sprintf("%s ^ \",\" ", temp)
        }
        displayof[opname]=temp " ^ \"\\n\""

########	gsub("[^a-z']+"," ")   ### ill-advised

		printf "%s", function_definition(opname, NF-1)
        	printf "("	# open parenthesis for pair
        	for (i=2; i<= NF; i++) {
        		if (!($i in fieldnames)) {
						 	print "unknown field", $i, "on line", NR > stderr
						 	next
						 }
        		printf "%sHI(A%d)+", $i, i-1
        	}
        	if (typeof[opname]==OPCODE) {
        		printf "op'HI(%d)", numberof[opname]
        	} else if (typeof[opname]==SPECIAL) {
        		printf "op'HI(%d)+", numberof["special"]
        		printf "functHI(%d)", numberof[opname]
        	} else if (typeof[opname]==BCOND) {
        		printf "op'HI(%d)+", numberof["bcond"]
        		printf "condHI(%d)", numberof[opname]
        	} else if (typeof[opname]==COP1) {
        		printf "op'HI(%d)+", numberof["cop1"]
        		printf "functHI(%d)", numberof[opname]
        	} else {
		       	print "unknown opcode", opname, "on line", NR > stderr
		       	next
		       }
        	printf ", "
        	for (i=2; i<= NF; i++) {
        		if (!($i in fieldnames)) {
						 	print "unknown field", $i, "on line", NR > stderr
						 	next
						 }
        		printf "%sLO(A%d)+", $i, i-1
        	}
        	if (typeof[opname]==OPCODE) {
        		printf "op'LO(%d)", numberof[opname]
        	} else if (typeof[opname]==SPECIAL) {
        		printf "op'LO(%d)+", numberof["special"]
        		printf "functLO(%d)", numberof[opname]
        	} else if (typeof[opname]==BCOND) {
        		printf "op'LO(%d)+", numberof["bcond"]
        		printf "condLO(%d)", numberof[opname]
        	} else if (typeof[opname]==COP1) {
        		printf "op'LO(%d)+", numberof["cop1"]
        		printf "functLO(%d)", numberof[opname]
        	} else {
		       	print "unknown opcode", opname, "on line", NR > stderr
		       	next
		       }
        	printf ")\n"
}

END {
  printf "%s", function_definition("decode",2) > decode
    print "let" > decode
      for (f in fieldnames) {
      	printf "val %s = THE%s(A1,A2)\n", f, f  > decode
      	printf "val S%s = Integer.makestring %s\n", f, f  > decode
      }
      print "val do_special ="  > decode
      print "(case funct of" > decode
      for (code=0; code<256; code++) {
      	name = opcode[SPECIAL,code]
      	if (name != ""  && name != "reserved") {
	      	if (code!=0) printf " | "  > decode # hack but it works
		      else printf "   " > decode
	      	disp = displayof[name]
	      	if (disp=="") disp="\"" name "(??? unknown format???)\\n\""
	      	printf "%d => %s\n", code, disp > decode
	      }
      }
      printf " | _ => \"unknown special\\n\"\n" > decode
      print "   ) " > decode
      print "val do_bcond =" > decode
      print "(case cond of" > decode
      for (code=0; code<256; code++) {
      	name = opcode[BCOND,code]
      	if (name != ""  && name != "reserved") {
	      	if (code!=0) printf " | "  > decode # hack but it works
		      else printf "   " > decode
	      	disp = displayof[name]
	      	if (disp=="") disp="\"" name "(??? unknown format???)\\n\""
	      	printf "%d => %s\n", code, disp > decode
	      }
      }
      printf " | _ => \"unknown bcond\\n\"\n" > decode
      print "   ) " > decode
      print "val do_cop1 =" > decode
      print "(case funct of" > decode
      for (code=0; code<256; code++) {
      	name = opcode[COP1,code]
      	if (name != ""  && name != "reserved") {
	      	if (code!=0) printf " | "  > decode # hack but it works
		      else printf "   " > decode
	      	disp = displayof[name]
	      	if (disp=="") disp="\"" name "(??? unknown format???)\\n\""
	      	printf "%d => %s\n", code, disp > decode
	      }
      }
      printf " | _ => \"unknown cop1\\n\"\n" > decode
      print "   ) " > decode
    print "in" > decode
      print "(case op' of" > decode
      for (code=0; code<256; code++) {
      	name = opcode[OPCODE,code]
      	if (name=="special") {
      		if (code!=0) printf " | "  > decode # hack but it works
		      else printf "   " > decode
      		printf "%d => %s\n", code, "do_special" > decode
      	} else if (name=="bcond") {
      		if (code!=0) printf " | "  > decode # hack but it works
		      else printf "   " > decode
      		printf "%d => %s\n", code, "do_bcond" > decode
      	} else if (name=="cop1") {
      		if (code!=0) printf " | "  > decode # hack but it works
		      else printf "   " > decode
      		printf "%d => %s\n", code, "do_cop1" > decode
      	} else if (name != ""  && name != "reserved") {
		     	if (code!=0) printf " | "  > decode # hack but it works
			     else printf "   " > decode
		     	disp = displayof[name]
		     	if (disp=="") disp="\"" name "(??? unknown format???)\\n\""
		     	printf "%d => %s\n", code, disp > decode
		     }
      }
      printf " | _ => \"unknown opcode\\n\"\n" > decode
      print "   ) " > decode
    print "end" > decode
  print "end (* Opcodes *)"
  print "end (* Decode *)" > decode
}
