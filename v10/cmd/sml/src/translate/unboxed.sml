(* Copyright 1989 by AT&T Bell Laboratories *)
structure Unboxed : sig structure Basics : BASICS
			val unboxedAssign : Basics.ty -> Access.primop
			val unboxedUpdate : Basics.ty -> Access.primop
		    end =
struct

  structure Basics = Basics
  open Access Prim Basics BasicTypes
  val intStamp = TypesUtil.tycStamp(!intTycon)
  fun alwaysunboxed ty =
    case ty
     of VARty(ref(INSTANTIATED t)) => alwaysunboxed t
      | VARty _  => false
      | CONty(ref(TYCON{kind=DATAtyc dcons,...}), _) =>
	    not(exists (fn (DATACON{rep=CONSTANT _,...})=>false 
			 | _ => true)
		       dcons) 
      | CONty(ref tyc,_) => let val ans = TypesUtil.tycStamp tyc = intStamp
			    in if !System.Control.debugging
			       then  (print "tr: "; PrintType.printTycon tyc;
				      print "\n";
				      if ans then print "ans: true\n"
					     else print "ans: false\n")
			       else ();
			       ans
			    end
      | _ => false (* impossible ? *)

  fun unboxedAssign(CONty(_,[CONty(_,[_,VARty(ref(INSTANTIATED ty))]),_])) =
        if alwaysunboxed ty then P.unboxedassign else P.:=
    | unboxedAssign _ = P.:=

  fun unboxedUpdate(CONty(_,[CONty(_,[_,_,VARty(ref(INSTANTIATED ty))]),_]))=
        if alwaysunboxed ty then P.unboxedupdate else P.update
    | unboxedUpdate _ = P.update
  
end
