(* propositional calculus *)

datatype truth = TRUE | FALSE

datatype term
  = VAR of string
  | CON of truth
  | NOT of term
  | AND of term * term
  | OR  of term * term

(* (~ p) and q  ==> AND(NOT(VAR "p"), VAR "q") *)

(* transform term to conjuctive normal form *)

fun negate (NOT(NOT p)) = negate p 
  | negate (NOT p) = p
  | negate (AND(p,q)) = OR(NOT p, NOT q)
  | negate (OR(p,q)) = AND(NOT p, NOT q)
  | negate p = NOT p

fun cnf (AND(p,q)) = AND(cnf p, cnf q)
  | cnf (NOT(NOT p)) = cnf p
  | cnf (NOT(AND(p,q))) = cnf(OR(negate p, negate q))
  | cnf (NOT(OR(p,q))) = AND(cnf(NOT p), cnf(NOT q))
  | cnf (OR(AND(p,q),r)) = AND(cnf(OR(p,r)),cnf(OR(q,r)))
  | cnf (OR(p,AND(q,r))) = AND(cnf(OR(p,q)),cnf(OR(p,r)))
  | cnf (OR(NOT p,q)) = cnf(OR(negate p, q))
  | cnf (OR(p, NOT q)) = cnf(OR(p, negate q))
  | cnf p = p

(* exercise: write a tautology checker.
  (hint: use resolution) *)
