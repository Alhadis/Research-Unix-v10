(* Copyright 1989 by AT&T Bell Laboratories *)
(* tableS.sml *)

signature TABLE =
  sig
    type 'a table
    val namednew : (string * int * exn) -> '1a table
    val new : int * exn -> '1a table
    val add : '2a table -> Symbol.symbol*'2a -> unit
    val rem : 'a table -> Symbol.symbol -> unit
    val map : 'a table -> Symbol.symbol -> 'a
    val app : (Symbol.symbol*'a -> unit) -> 'a table -> unit
    val tableToList: 'a table -> (Symbol.symbol*'a)list
  end
