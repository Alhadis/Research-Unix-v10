(* Copyright 1989 by AT&T Bell Laboratories *)
(* emitter.sig
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY 14853
 * jhr@cs.cornel.edu
 *
 * HISTORY:
 *   11/20/89  created
 *
 *    This is the signature of the assembler and machine code emitters.
 *)

signature EMITTER =
sig
    type instruction
    type label

    val emitLong : int -> unit              (* emit an integer constant *)
    val emitString : string -> unit         (* emit a (padded) string constant *)
    val emitReal : string -> unit           (* emit a real constant *)
(* NOTE: the following is useful for asm code, but could be replaced by
 * emitLong. *)
    val emitAddr : (label * int) -> unit    (* emit a label value (with offset) *)

    val define : label -> unit              (* define a label *)
    val mark : unit -> unit                 (* emit a back-pointer mark *)

    val emitInstr : instruction -> unit     (* emit an instruction *)

    val comment : string -> unit

    val init : int -> unit                  (* initialize to emit n bytes of code *)

end (* signature EMITTER *)
