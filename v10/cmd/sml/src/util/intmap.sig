(* Copyright 1989 by AT&T Bell Laboratories *)
signature INTMAP =
  sig
    type 'a intmap
    val namednew : string * int * exn -> '1a intmap
    val new : int * exn -> '1a intmap
    val add : '2a intmap -> int * '2a -> unit
    val rem : 'a intmap -> int -> unit
    val map : 'a intmap -> int -> 'a
    val app : (int * 'a -> unit) -> 'a intmap -> unit
    val intMapToList: 'a intmap -> (int * 'a) list
  end
