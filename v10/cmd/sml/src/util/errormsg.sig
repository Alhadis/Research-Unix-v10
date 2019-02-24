(* Copyright 1989 by AT&T Bell Laboratories *)
signature ERRORMSG =
 sig
    exception Syntax
    val anyErrors : bool ref
    val lineNum : int ref
    val fileName : string ref
    val debugging : bool ref
    val say : string -> unit
    val warn : string -> unit
    val complain : string -> unit
    val condemn : string -> 'a
    val impossible : string -> 'a
    val debugmsg : string -> bool
    val flaggedmsg : bool ref -> string -> bool
 end
