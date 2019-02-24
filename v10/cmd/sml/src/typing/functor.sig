(* Copyright 1989 by AT&T Bell Laboratories *)
(* functor.sig *)

signature FUNCTOR =
sig

  val abstractBody : Basics.Structure * Basics.Structure
		     * Stampset.stampsets * int -> Basics.Structure

  val applyFunctor : Basics.Functor * Basics.Structure * Basics.Symbol.symbol list 
		     * Stampset.stampsets
		     -> Basics.Structure * Basics.thinning
end
