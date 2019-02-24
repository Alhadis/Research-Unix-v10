(* Copyright 1989 by AT&T Bell Laboratories *)
signature TOKEN = sig
  datatype token =
    EOF | ID of Symbol.symbol | IDDOT of Symbol.symbol | TYVAR of Symbol.symbol
  | INT of int | REAL of string | STRING of string | ABSTRACTION | ABSTYPE | AND
  | ARROW | AS | BAR | CASE | DATATYPE | DOTDOTDOT | ELSE | END | EQUAL
  | EQTYPE | EXCEPTION | DO | DARROW | FN | FUN | FUNCTOR | HANDLE | HASH | IF | IN
  | INCLUDE | INFIX | INFIXR | LET | LOCAL | NONFIX | OF | OP | OPEN | OVERLOAD
  | RAISE | REC | SHARING | SIG | SIGNATURE | STRUCT | STRUCTURE | THEN
  | TYPE | VAL | WHILE | WILD | WITH | WITHTYPE | ASTERISK | COLON | COMMA | LBRACE
  | LBRACKET | LPAREN | RBRACE | RBRACKET | RPAREN | SEMICOLON | ORELSE | ANDALSO | IMPORT

  val tokenName : token -> string
end  (* TOKEN *)

structure Token : TOKEN = struct
  datatype token =
    EOF | ID of Symbol.symbol | IDDOT of Symbol.symbol | TYVAR of Symbol.symbol
  | INT of int | REAL of string | STRING of string | ABSTRACTION | ABSTYPE | AND
  | ARROW | AS | BAR | CASE | DATATYPE | DOTDOTDOT | ELSE | END | EQUAL
  | EQTYPE | EXCEPTION | DO | DARROW | FN | FUN | FUNCTOR | HANDLE | HASH | IF | IN
  | INCLUDE | INFIX | INFIXR | LET | LOCAL | NONFIX | OF | OP | OPEN | OVERLOAD
  | RAISE | REC | SHARING | SIG | SIGNATURE | STRUCT | STRUCTURE | THEN
  | TYPE | VAL | WHILE | WILD | WITH | WITHTYPE | ASTERISK | COLON | COMMA | LBRACE
  | LBRACKET | LPAREN | RBRACE | RBRACKET | RPAREN | SEMICOLON | ORELSE | ANDALSO | IMPORT

 fun tokenName(tok: token) : string =
     case tok of
       EOF => "EOF"
     | ID s => "ID " ^ Symbol.name(s)
     | IDDOT s => "IDDOT " ^ Symbol.name(s)
     | TYVAR s => "TYVAR " ^ Symbol.name(s)
     | INT i => "INT" ^ makestring(i)
     | REAL s => "REAL " ^ s
     | STRING s => "STRING " ^ s
     | AND => "AND"
     | ABSTRACTION => "ABSTRACTION"
     | ABSTYPE => "ABSTYPE"
     | ARROW => "ARROW"
     | AS => "AS"
     | BAR => "BAR"
     | CASE => "CASE"
     | DATATYPE => "DATATYPE"
     | DOTDOTDOT => "DOTDOTDOT"
     | ELSE => "ELSE"
     | END => "END"
     | EQUAL => "EQUAL"
     | EQTYPE => "EQTYPE"
     | EXCEPTION => "EXCEPTION"
     | DO => "DO"
     | DARROW => "DARROW"
     | FN => "FN"
     | FUN => "FUN"
     | FUNCTOR => "FUNCTOR"
     | HANDLE => "HANDLE"
     | HASH => "HASH"
     | IF => "IF"
     | IN => "IN"
     | INCLUDE => "INCLUDE"
     | INFIX => "INFIX"
     | INFIXR => "INFIXR"
     | LET => "LET"
     | LOCAL => "LOCAL"
     | NONFIX => "NONFIX"
     | OF => "OF"
     | OP => "OP"
     | OPEN => "OPEN"
     | OVERLOAD => "OVERLOAD"
     | RAISE => "RAISE"
     | REC => "REC"
     | SHARING => "SHARING"
     | SIG => "SIG"
     | SIGNATURE => "SIGNATURE"
     | STRUCT => "STRUCT"
     | STRUCTURE => "STRUCTURE"
     | THEN => "THEN"
     | TYPE => "TYPE"
     | VAL => "VAL"
     | WHILE => "WHILE"
     | WILD => "WILD"
     | WITH => "WITH"
     | WITHTYPE => "WITHTYPE"
     | ASTERISK => "ASTERISK"
     | COLON => "COLON"
     | COMMA => "COMMA"
     | LBRACE => "LBRACE"
     | LBRACKET => "LBRACKET"
     | LPAREN => "LPAREN"
     | RBRACE => "RBRACE"
     | RBRACKET => "RBRACKET"
     | RPAREN => "RPAREN"
     | SEMICOLON => "SEMICOLON"
     | ORELSE => "ORELSE"
     | ANDALSO => "ANDALSO"
     | IMPORT => "IMPORT"
end  (* Token *)
