(* Copyright 1989 by AT&T Bell Laboratories *)
(* lambda.sml *)

signature LAMBDA = sig

structure Access: ACCESS
structure Basics: BASICS

type primop sharing type primop = Access.primop

datatype con
  = DATAcon of Basics.datacon
  | INTcon of int
  | REALcon of string
  | STRINGcon of string

datatype lexp
  = VAR of Access.lvar
  | FN of Access.lvar * lexp
  | FIX of Access.lvar list * lexp list * lexp
  | APP of lexp * lexp
  | INT of int
  | REAL of string
  | STRING of string
  | SWITCH of lexp * (con*lexp) list * lexp option
  | RECORD of lexp list
  | SELECT of int * lexp
  | RAISE of lexp
  | HANDLE of lexp * lexp
  | PRIM of primop

 val CON : Basics.datacon * lexp -> lexp
 val DECON : Basics.datacon * lexp -> lexp

end

structure Lambda : LAMBDA = struct 

  structure Access = Access
  structure Basics = Basics
  open Access Basics

 type primop = Access.primop

 datatype con
  = DATAcon of datacon
  | INTcon of int
  | REALcon of string
  | STRINGcon of string

  datatype lexp
    = VAR of lvar
    | FN of lvar * lexp
    | FIX of lvar list * lexp list * lexp
    | APP of lexp * lexp
    | INT of int
    | REAL of string
    | STRING of string
    | SWITCH of lexp * (con*lexp) list * lexp option
    | RECORD of lexp list
    | SELECT of int * lexp
    | RAISE of lexp
    | HANDLE of lexp * lexp
    | PRIM of primop

fun trpath [i] = VAR i
  | trpath (a::r) = SELECT(a, trpath r)

fun CON (DATACON{rep=(TAGGED i),...},e) = RECORD[e,INT i]
  | CON (DATACON{rep=TRANSPARENT,...},e) = e
  | CON (DATACON{rep=TRANSU,...},e) = e
  | CON (DATACON{rep=TRANSB,...},e) = e
  | CON (DATACON{rep=REF,...},e) = APP(PRIM Access.P.makeref,e)
  | CON (DATACON{rep=(VARIABLE(PATH p)),const=true,...},e) = trpath p
  | CON (DATACON{rep=(VARIABLE(PATH p)),...},e) = RECORD[e, trpath p]
  | CON (DATACON{rep=(CONSTANT i),...},e) = INT i

fun DECON (DATACON{rep=(TAGGED _),...},e) = SELECT(0,e)
  | DECON (DATACON{rep=TRANSPARENT,...},e) = e
  | DECON (DATACON{rep=TRANSU,...},e) = e
  | DECON (DATACON{rep=TRANSB,...},e) = e
  | DECON (DATACON{rep=REF,...},e) = APP(PRIM Access.P.!,e)
  | DECON (DATACON{rep=(VARIABLE _),...},e) = SELECT(0,e) 
  | DECON (DATACON{name,rep=r,...},e) =
     (print (Symbol.name name); print " "; PrintBasics.printRep r; print "\n";
     ErrorMsg.impossible "DECON with bad constructor")

end
