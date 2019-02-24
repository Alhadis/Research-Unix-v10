(* Copyright 1989 by AT&T Bell Laboratories *)
(* basecoder.sml
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY 14850
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   11/20/89  created
 *)

signature BASECODER =
sig
    datatype label = Label of {id : int, addr : int ref}

    val newLabel : unit -> label
    val addrOf : label -> int
    val nameOf : label -> string

end (* signature BASECODER *)


structure BaseCoder : BASECODER =
struct

    datatype label = Label of {id : int, addr : int ref}

    local
      val cnt = ref 0
    in
    fun nextId () = (!cnt before ((inc cnt) handle Overflow => cnt := 0))
    end

    fun newLabel () = Label{id= nextId(), addr= ref 0}

    fun addrOf (Label{addr, ...}) = !addr
    fun nameOf (Label{id, ...}) = "L" ^ (makestring id)

end (* structure BaseCoder *)
