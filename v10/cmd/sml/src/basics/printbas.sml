(* Copyright 1989 by AT&T Bell Laboratories *)
(* printbasics.sml *)

structure PrintBasics = struct

local open PrintUtil Access Basics PrintType System.Control in

val printTuple = printClosedSequence ("(", ",", ")")

val printList = printClosedSequence ("[", ",", "]") (* printList not used here *)
		
fun printPath [n:int] = (print n)
  | printPath (n::p) = (printPath p; print "."; print n)
  | printPath [] = ()

fun printAccess a =
    if !internals then case a
	    of (LVAR v) => (print "@"; print v)
	     | (SLOT n) => (print "#"; print n)
	     | (PATH p) => (print "$"; printPath p)
	     | (INLINE p) => (print "%"; print(Prim.inLineName p))
	else ()

fun printRep UNDECIDED = print "UNDECIDED"
  | printRep (TAGGED i) = (print "TAGGED["; print i; print "]")
  | printRep (CONSTANT i) = (print "CONSTANT["; print i; print "]")
  | printRep TRANSPARENT = print "TRANSPARENT"
  | printRep TRANSU = print "TRANSU"
  | printRep TRANSB = print "TRANSB"
  | printRep REF = print "REF"
  | printRep (VARIABLE a) = (print "VARIABLE["; printAccess a; print "]")

fun printDcon (DATACON{name,rep=VARIABLE(access),...}) =
		(printSym(name); printAccess access)
  | printDcon (DATACON{name,...}) = printSym(name)

fun printDatacon(DATACON{name,typ,...}) =
    (printSym name; print " : "; printType(!typ))

fun printExn(DATACON{name,typ,...}) =
    (printSym name; print " : "; printType(!typ))

fun printVar (UNKNOWNvar s) = (printSym s; if !internals then print "?" else())
  | printVar (VALvar {access,name,...}) = (print(formatQid name); printAccess access)
  | printVar (OVLDvar {name,...}) = printSym(name)

fun printVariable(VALvar{name,access,typ}) = 
    (print(formatQid name); printAccess access;
     print " : "; printType(!typ))
  | printVariable(OVLDvar{name,...}) = (printSym name; print " : overloaded")
  | printVariable(UNKNOWNvar name) = (printSym name; print " : ?")

fun printStr(STRstr _) = print "STRstr"
  | printStr(INDstr _) = print "INDstr"
  | printStr(SHRstr _) = print "SHRstr"
  | printStr(NULLstr) = print "NULLstr"

fun printStrVar(STRvar{name,access,binding}) =
    (print(formatQid name); printAccess access;
     if !internals then (print "["; printStr binding; print "]") else ())

fun printBinding(VARbind(var)) = (print "val "; printVariable var)
  | printBinding(CONbind(con)) = (print "con "; printDatacon con)
  | printBinding(TYCbind(ref tycon)) = (print "type "; printTycon tycon)
  | printBinding(TYVbind v) = (print "type "; printTyvar v)
  | printBinding(SIGbind(SIGvar{name,...})) = (print "signature "; printSym name)
  | printBinding(STRbind(strVar)) = (print "structure "; printStrVar strVar)
  | printBinding(FCTbind(FCTvar{name,...})) = (print "functor "; printSym name)
  | printBinding(FIXbind(FIXvar{name,binding=NONfix})) = (print "nonfix "; printSym name)
  | printBinding(FIXbind(FIXvar{name,binding=INfix _})) = (print "infix "; printSym name)

fun printTable(table) =
    IntStrMap.app (fn (_,_,binding) => (printBinding(binding); newline())) table

fun printStructure(STRstr{stamp,table,env,...}) =
    let fun printTenv (t: tycon array) =
	 let fun foreach i =
		 (print i; print " "; PrintType.printTycon(t sub i); newline();
		  foreach(i+1))
	  in print "types\n";
	     foreach 0
	     handle Subscript => print "end types\n"
	 end
     in
      (print "STRstr["; print stamp; print "]\n";
       case env
         of REL{t,...} => printTenv(t)
          | DIR => ();
       printTable table)
    end
  | printStructure(INDstr _) = ErrorMsg.impossible "printStructure: INDstr"
  | printStructure(SHRstr _) = ErrorMsg.impossible "printStructure: SHRstr"
  | printStructure(NULLstr) = ErrorMsg.impossible "printStructure: NULLstr"

  fun pr_path'[] = "]"
  |   pr_path'[x:int] = makestring x ^ "]"
  |   pr_path'((x:int)::rest)= makestring x ^ "," ^ pr_path' rest
  fun pr_path path = "[" ^ pr_path' path

end (* local *)

end (* PrintBasics *)
