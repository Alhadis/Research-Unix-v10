(* Copyright 1989 by AT&T Bell Laboratories *)
(* buildmod.sig *)

signature BUILDMOD =
sig

    val binderGt : Basics.binder * Basics.binder -> bool
         (*(int * string * Basics.binding) * 
		     (int * string * Basics.binding) -> bool*)

    val buildStrTable : unit -> Basics.trans list * Basics.symtable

end  (* signature BUILDMOD *)

