(* Copyright 1989 by AT&T Bell Laboratories *)
(* coreinfo.sig *)

signature COREINFO =
sig
    val exnBind : Basics.datacon ref
    val exnMatch : Basics.datacon ref
    val stringequalPath : int list ref
    val polyequalPath : int list ref
    val currentPath : int list ref
    val toplevelPath : int list ref
    val getDebugVar : Basics.var ref
    val setCore : Basics.structureVar -> unit
end

