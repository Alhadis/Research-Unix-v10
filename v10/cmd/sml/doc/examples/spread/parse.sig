signature PARSE =
sig
    structure F : FORMULA
    val parse : string -> F.formula
end

(* Grammar (in order of precedence):

    exp ->  NUM  |  ( exp )  |  [ exp , exp ] 
    exp ->  exp * exp  |   exp / exp
    exp ->  exp + exp  |  exp - exp
    exp ->  "if" exp "then" exp "else" exp
*)

