(* Copyright 1989 by AT&T Bell Laboratories *)
(* coreinfo.sml *)

(* info extracted from Core structure *)

structure CoreInfo : COREINFO =
struct

  open Access Basics EnvAccess
  val bogusCON = DATACON{name=Symbol.symbol "bogus",const=true,typ=ref ERRORty,
			 rep=UNDECIDED,sign=[]}

  val exnBind = ref(bogusCON)
  val exnMatch = ref(bogusCON)
  val stringequalPath = ref[0]
  val polyequalPath = ref[0]
  val currentPath = ref[0]
  val toplevelPath = ref[0]
  val getDebugVar = ref(mkVALvar(Symbol.symbol "getDebug",
				 ref(VARty(mkTyvar TypesUtil.defaultMETA))))

  fun setCore(STRvar{access=PATH p,binding,...}) =
      let fun extractPath name = 
	      let val sym = Symbol.symbol name
		  val VARbind(VALvar{access=PATH p,...}) =
			lookVARCONinStr(binding, sym, p, [sym])
	       in p
	      end
	  fun coreCon name = lookCONinStr(binding,name,p,[name])
       in exnBind := coreCon(Symbol.symbol "Bind");
	  exnMatch := coreCon(Symbol.symbol "Match");
	  stringequalPath := extractPath "stringequal";
	  polyequalPath := extractPath "polyequal";
	  currentPath := extractPath "current";
	  toplevelPath := extractPath "toplevel";
	  getDebugVar := let val name = Symbol.symbol "getDebug"
			     val VARbind x = lookVARCONinStr(binding,name,p,[name])
			  in x
			 end
      end
    | setCore _ = ErrorMsg.impossible "EnvAccess.setCore"

end (* CoreInfo *)
