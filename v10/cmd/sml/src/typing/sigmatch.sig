(* Copyright 1989 by AT&T Bell Laboratories *)
(* sigmatch.sig *)

signature SIGMATCH =
sig
  structure Basics: BASICS

  val match0  : {mapstr1: int*Basics.Structure->unit,
	         maptyc: int*Basics.tycon->unit,
		 mapstr: Basics.Structure*Basics.Structure->unit}
	     -> bool * Basics.Symbol.symbol list * Stampset.stampsets
	         * Basics.Structure * Basics.Structure * Basics.Structure
	     -> Basics.Structure * Basics.thinning

  val match   : bool * Basics.Symbol.symbol list * Stampset.stampsets
	         * Basics.Structure * Basics.Structure * Basics.Structure
	     -> Basics.Structure * Basics.thinning

  val realize : bool * Basics.Symbol.symbol list * Stampset.stampsets * Basics.stamp
                 * Basics.Structure * Basics.Structure
	     -> Basics.Structure * Basics.trans list
end

