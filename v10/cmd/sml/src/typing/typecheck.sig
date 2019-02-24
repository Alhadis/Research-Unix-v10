(* Copyright 1989 by AT&T Bell Laboratories *)
signature TYPECHECK = sig

  structure BareAbsyn : BAREABSYN

  val reset : unit -> unit
  val decType : BareAbsyn.dec -> unit

end  (* signature TYPECHECK *)
