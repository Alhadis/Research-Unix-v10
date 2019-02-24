(* Copyright 1989 by AT&T Bell Laboratories *)
structure FirstSets = struct

local open Token Basics in

fun firstApat lookFIX =
    fn ID s =>  lookFIX(s) = NONfix
     | OP => true
     | IDDOT _ => true
     | INT _ => true 
     | REAL _ => true
     | STRING _ => true
     | WILD => true
     | LPAREN => true
     | LBRACE => true
     | LBRACKET => true
     | _    => false

fun firstAexp lookFIX =
    fn ID s =>   lookFIX(s) = NONfix
     | OP => true
     | IDDOT _ => true
     | INT _ => true
     | REAL _ => true
     | STRING _ => true
     | HASH => true
     | LPAREN => true
     | LBRACE => true
     | LBRACKET => true
     | LET => true
     | _    => false

fun firstExp lookFIX =
    fn IF => true
     | WHILE => true
     | CASE => true
     | RAISE => true
     | FN => true
     | tok => firstAexp lookFIX tok
     
val firstLdec = 
    fn VAL => true
     | FUN => true
     | TYPE => true
     | DATATYPE => true
     | ABSTYPE => true
     | EXCEPTION => true
     | Token.OPEN => true
     | LOCAL => true
     | INFIX => true
     | INFIXR => true
     | NONFIX => true
     | OVERLOAD => true
     | _    => false

val firstSdec = 
    fn VAL => true
     | FUN => true
     | TYPE => true
     | DATATYPE => true
     | ABSTYPE => true
     | EXCEPTION => true
     | Token.OPEN => true
     | LOCAL => true
     | INFIX => true
     | INFIXR => true
     | NONFIX => true
     | OVERLOAD => true
     | STRUCTURE => true
     | ABSTRACTION => true
     | Token.FUNCTOR => true  (* monster structure hack *)
     | SIGNATURE => true      (* ditto *)
     | _    => false

val firstTdec = 
    fn SIGNATURE => true
     | STRUCTURE => true
     | ABSTRACTION => true
     | Token.FUNCTOR => true
     | _    => false

val firstSpec =
    fn STRUCTURE => true
     | TYPE => true
     | EQTYPE => true
     | DATATYPE => true
     | SHARING => true
     | VAL => true
     | EXCEPTION => true
     | INFIX => true
     | INFIXR => true
     | NONFIX => true
     | OVERLOAD => true
     | INCLUDE => true
     | LOCAL => true
     | Token.OPEN => true
     | _    => false

end (* local *)

end (* FirstSets *)
