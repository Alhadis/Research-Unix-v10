(* Copyright 1989 by AT&T Bell Laboratories *)
(* printdec.sig *)

signature PRINTDEC =
  sig
    structure BareAbsyn: BAREABSYN
    type object
    val printDec : (int -> object) -> BareAbsyn.dec -> unit
    val printBindingTbl: Basics.symtable -> unit
  end
