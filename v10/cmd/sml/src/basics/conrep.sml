(* Copyright 1989 by AT&T Bell Laboratories *)
(* conrep.sml *)

structure ConRep = struct

local open Basics BasicTypes in

fun count predicate l
  = let fun test (a::rest,acc) = test (rest,if predicate a then 1+acc else acc)
	  | test (nil,acc) = acc
     in test (l,0)
    end

fun boxed ([(_,false,_)]: (Symbol.symbol*bool*ty) list): conrep list = [TRANSPARENT]
  | boxed predcons =
      let val multiple = (count (fn (_,const,_) => not const) predcons) > 1
	  fun decide (i,j,(_,true,_)::rest) = CONSTANT i :: decide(i+1,j,rest)
	    | decide (i,j,(_,_,CONty(_,[argty,_]))::rest) =
		(case (multiple,argty)
		  of (false,CONty(ref(TYCON{kind=RECORDtyc(_::_::_), ...}),_)) =>
		       TRANSPARENT :: decide(i,j,rest)
		   | _ => TAGGED j :: decide(i,j+1,rest))
	    | decide (_,_,nil) = []
	    | decide _ = ErrorMsg.impossible "ConRep.boxed"
       in decide(0,0,predcons)
      end

end (* local *)

end (* structure ConRep *)
