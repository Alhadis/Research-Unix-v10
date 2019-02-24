(* Copyright 1989 by AT&T Bell Laboratories *)
(* parse.sig *)

signature PARSE = sig
  structure BareAbsyn : BAREABSYN
  exception Eof
  val interdec : Lex.lexer -> BareAbsyn.dec
end
