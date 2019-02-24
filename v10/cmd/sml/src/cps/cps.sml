(* Copyright 1989 by AT&T Bell Laboratories *)
signature CPS = sig

type lvar
exception Ctable

datatype const = INTconst of int | REALconst of string | STRINGconst of string

datatype accesspath = OFFp of int | SELp of int * accesspath

datatype cexp = RECORD of (lvar * accesspath) list * lvar * cexp
	      | SELECT of int * lvar * lvar * cexp
	      | OFFSET of int * lvar * lvar * cexp
	      | APP of lvar * lvar list
	      | FIX of (lvar * lvar list * cexp) list * cexp
	      | SWITCH of lvar * cexp list
	      | PRIMOP of Access.primop * lvar list * lvar list * cexp list
type function
val recordpath : lvar list -> (lvar * accesspath) list
val combinepaths : accesspath * accesspath -> accesspath
val lenp : accesspath -> int

end

structure CPS : CPS = struct

type lvar = int
exception Ctable

datatype const = INTconst of int | REALconst of string | STRINGconst of string

datatype accesspath = OFFp of int | SELp of int * accesspath

datatype cexp
  = RECORD of (lvar * accesspath) list * lvar * cexp
  | SELECT of int * lvar * lvar * cexp
  | OFFSET of int * lvar * lvar * cexp
  | APP of lvar * lvar list
  | FIX of function list * cexp
  | SWITCH of lvar * cexp list
  | PRIMOP of Access.primop * lvar list * lvar list * cexp list
withtype function = lvar * lvar list * cexp

val recordpath = map (fn v => (v,OFFp 0))

fun combinepaths(p,OFFp 0) = p
  | combinepaths(p,q) = 
    let val rec comb =
	fn (OFFp 0) => q
	 | (OFFp i) => (case q of
		          (OFFp j) => OFFp(i+j)
		        | (SELp(j,p)) => SELp(i+j,p))
	 | (SELp(i,p)) => SELp(i,comb p)
    in comb p
    end

fun lenp(OFFp _) = 0
  | lenp(SELp(_,p)) = 1 + lenp p

end
