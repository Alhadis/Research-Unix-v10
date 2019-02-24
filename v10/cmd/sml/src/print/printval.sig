(* Copyright 1989 by AT&T Bell Laboratories *)
(* printval.sig *)

signature PRINTVAL = 
  sig
    structure Basics: BASICS
    type object
    val printVal: object * Basics.ty * int -> unit
  end
