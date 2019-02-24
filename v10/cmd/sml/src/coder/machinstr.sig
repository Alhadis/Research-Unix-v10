(* Copyright 1989 by AT&T Bell Laboratories *)
(* machinstr.sig
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY 14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   11/20/89  created
 *
 *   This is an abstract interface to the machine instruction set.
 *)

signature MACHINSTR =
sig
    eqtype register
    eqtype label sharing type label = BaseCoder.label

  (** Instructions **)

    type instruction

    datatype ikind = IK_NOP | IK_JUMP | IK_INSTR

    val instrKind : instruction -> ikind
    val nop : instruction

  (** Span dependent instructions **)

    type sdi

    val minSize : sdi -> int
    val sizeOf : sdi -> (bool * int)
	(* sizeOf(I) returns the size of I under the current address assignment
	 * for labels plus true if the size if the maximum possible for the sdi. *)
    val expand : (sdi * int) -> instruction list
	(* expand (I, n) returns the expansion of I into n bytes of machine
	 * instructions. *)

  (** Resource usage **)

    val numResources : int
    val rUseDef : instruction -> (int list * int list)
    val hazard : (instruction * instruction) -> bool
    val needsNop : (instruction * instruction) -> bool

end (* signature MACHINSTR *)

