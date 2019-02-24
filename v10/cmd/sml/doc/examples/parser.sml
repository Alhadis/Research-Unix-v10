(* a simple parser.  (R. Burstall)
The grammar is:

    var	  = x | y | z
    const = 0 | 1 | 2
    aexpr = var | const | (expr)
    expr  = aexpr + expr | aexpr * expr | aexpr
    stmt  = var := expr
	  | if expr then stmt
	  | while expr do stmt
	  | begin stmts end
    stmts = stmt ; stmts | stmt
    decl  = var : integer | var : boolean
    decls = decl ; decls | decl
    prog  = var decls begin stmts end .

If S is a set of string lists we say that a function f: string list -> string list
recognizes S if

  (1) if l does not begin with a list in S then f l fails
  (2) if s is the longest list in S such that l = s@m for some list m, then 
      f l = m

Each phrase name in the grammar above denotes a set of string lists.  We write ML
functions called var, const, aexpr, ..., to recognized each of these phrases
(i.e. to recognize the associated set of string lists).

If f1 recognizes P1 and f2 recognizes P2 then we can define a function g to
recognize P1 P2 by 

  g = f1 & f2	   where f & g = (fn s => g(f s))

We can define a function h to recognize P1 | P2 by

  h = f = f1 ||| f2    where f ||| g = (fn s => f1 s handle Fail => f2 s)
*)

exception Fail

infix 3 &
infix 2 |||

fun f1 & f2 = (fn s => f2(f1 s))
fun f1 ||| f2 = (fn s => f1 s handle Fail => f2 s)

fun quote (t: string) ([]: string list) = raise Fail
  | quote t (t' :: rest) = if t = t' then rest else raise Fail

val var = quote "x" ||| quote "y" ||| quote "z"

val const = quote "0" ||| quote "1" ||| quote "2"

fun aexpr s = (var ||| const ||| quote "(" & expr & quote ")") s

and expr s = (aexpr & quote "+" & expr ||| 
	      aexpr & quote "*" & expr ||| 
	      aexpr) s

and stmt s = (var & quote ":=" & expr ||| 
	      quote "if" & expr & quote "then" & stmt |||
	      quote "while" & expr & quote "do" & stmt |||
	      quote "begin" & stmts & quote "end") s

and stmts s = (stmt & quote ";" & stmts ||| stmt) s

and decl s = (var & quote ":" & quote "integer" |||
	      var & quote ":" & quote "boolean") s

and decls s = (decl & quote ";" & decls ||| decl) s

and prog s = (quote "var" & decls & quote "begin" & stmts & quote "end" &
	      quote ".") s

fun parse s = (if prog s = [] then "YES" else "NO") handle Fail => "NO"

(* examples *)

expr ["x","+","1", "end", "."];

parse ["var","x",":","integer",";","y",":","boolean",
       "begin","x",":=","0","end","."];
