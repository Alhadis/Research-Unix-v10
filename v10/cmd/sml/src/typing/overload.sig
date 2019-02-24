(* Copyright 1989 by AT&T Bell Laboratories *)
(* overload.sig *)

signature OVERLOAD =
sig
  structure Basics : BASICS
  exception Overld
  val resetOverloaded : unit -> unit
  val markOverloaded : unit -> unit
  val pushOverloaded : Basics.var ref -> Basics.ty
  val resolveOverloaded : unit -> unit
end  (* signature OVERLOAD *)
