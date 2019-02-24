(* Copyright 1989 by AT&T Bell Laboratories *)
(* sharing.sig *)

signature SHARING =
  sig
    structure Basics: BASICS
    val doSharing : Basics.symtable * Basics.strenv * Stampset.stampsets 
		    * Basics.sharespec -> Basics.sharespec
    val checkSharing : Basics.symtable * Basics.strenv * Basics.sharespec -> unit
  end
