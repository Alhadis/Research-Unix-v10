(* Copyright 1989 by AT&T Bell Laboratories *)
signature MCOPT =
  sig
    structure Absyn : BAREABSYN
    structure Access : ACCESS
    type rhs
    val opt : (Absyn.pat list * rhs) list * Access.lvar list ->
              (Absyn.pat list * rhs) list * Access.lvar list
  end

structure MCopt : MCOPT = struct

structure Absyn : BAREABSYN = Absyn
structure Lambda : LAMBDA = Lambda
structure Access : ACCESS = Access
open Basics Absyn Lambda
open PrintUtil PrintBasics PrintAbsyn MCprint ErrorMsg

type rhs = (Access.lvar * Access.lvar) list * int (* bindings and tag *)

fun cons2 (hd::hds,tl::tls) = (hd::tl)::cons2(hds,tls)
  | cons2 (hd::hds,[]) = [hd]::cons2(hds,[])
  | cons2 ([],[]) = []
  | cons2 _ =  impossible "cons2 in mcopt"

infixr cons2

(* take a list of record patterns and return the list
   of the tail fields of each record pattern *)
fun tl2 ([_]::_) = []
  | tl2 ((_::tl)::pats) = tl::(tl2 pats)
  | tl2 [] = []
  | tl2 _ = impossible "tl2 in mcopt"

(* take a list of record patterns and return the list
   of the first field of each record pattern *)
fun hd2 ((hd::_)::pats) = hd::(hd2 pats)
  | hd2 [] = []
  | hd2 _ = impossible "hd2 in mcopt"

fun combine(relf::relfs,irrelf::irrelfs) = (relf@irrelf)::(combine(relfs,irrelfs))
  | combine([],[]) = []
  | combine([],irrel) = irrel
  | combine(rel,[]) = rel
fun addtail (fields::pats,rhs::tl) = (fields,rhs)::addtail(pats,tl)
  | addtail ([],[]) = []
  | addtail _ = impossible "addtail in mcopt"
fun strip ((fields,rhs)::pats) =
    let val (fl,tl) = strip pats in  (fields::fl,rhs::tl) end
  | strip [] = ([],[])

fun branch_factor fs =
 let fun existsPat f =
      let val rec ePat =
	   fn [] => false
            | VARpat _::more => ePat more
            | WILDpat::more => ePat more
            | LAYEREDpat (_,p)::more => ePat (p::more)
            | CONSTRAINTpat (p,_)::more => ePat (p::more)
            | p::more => (f p orelse ePat more
			 handle Match => impossible "ePat in mcopt")
      in  ePat
      end
     fun within(p,plist) =
       case p
         of APPpat(DATACON{name=r1,...},_) =>
		existsPat (fn APPpat(DATACON{name=r2,...},_) => Symbol.eq(r1,r2)
			    | CONpat _ => false) plist
	  | CONpat(DATACON{name=r1,...}) =>
		existsPat (fn CONpat(DATACON{name=r2,...}) => Symbol.eq(r1,r2)
			    | APPpat _ => false) plist
	  | INTpat i => existsPat (fn INTpat j => i=j) plist
	  | REALpat r => existsPat (fn REALpat s => r=s) plist
	  | STRINGpat s => existsPat (fn STRINGpat t => s=t) plist
	  | VARpat _ => true
	  | WILDpat => true
	  | LAYEREDpat (_,p) => within (p,plist)
	  | CONSTRAINTpat (p,_) => within (p,plist)
	  |  _ => impossible "within in mcopt"
 in  length (fold (fn(a::_,b) => if within(a,b) then b else a::b) fs [])
 end

fun arity ((hd::_)::_) = 
      let val rec ar =
	   fn INTpat _ => 1
	    | REALpat _ => 1
	    | STRINGpat _ => 1
	    | VARpat _ => 0
	    | WILDpat => 0
	    | RECORDpat{pats=ref pats,...} => length pats
	    | APPpat (_,p) => 1 + ar p
	    | CONpat _ => 1
	    | LAYEREDpat (_,p) => ar p
	    | CONSTRAINTpat (p,_) => ar p
      in  ar hd
      end
  | arity _ = impossible "arity in mcopt"

exception Record
val rec relevant =
 fn VARpat _ => false
  | WILDpat => false (* any var always matches so never relevant *)
  | RECORDpat{pats=ref [],...} => false (* unit isDCB never relevant *)
  | RECORDpat _ => raise Record (* otherwise, immediately expand records *)
  | LAYEREDpat (_,p) => relevant p
  | CONSTRAINTpat (p,_) => relevant p
    (* if only one data constructor, no need to test *)
  | CONpat(DATACON{sign = [_],...}) => false
  | APPpat(DATACON{sign = [_],...},p) => relevant p
  | _ => true (* everything else is relevant *)

fun rel fs = fold (fn (a::_,b) => if relevant a then b else b+1) fs 0
	
(* a record should be immediately expanded by mcand so that
   the nested fields can be considered as well;
   don't bother to look at the rest of the fields,
   and leave the record at the end of relf.
   otherwise, just check the relevant.
*)
local
fun r_o([],[],relf,relx,irrelf,irrelx) = (relf,relx,irrelf,irrelx)
  | r_o(arg as (hd::_)::_,x::xs,relf,relx,irrelf,irrelx) =
    ((if relevant hd
      then r_o(tl2 arg,xs,(hd2 arg) cons2 relf,x::relx,irrelf,irrelx)
      else r_o(tl2 arg,xs,relf,relx,(hd2 arg) cons2 irrelf,x::irrelx))
     handle Record =>
      ((hd2 arg) cons2 [],[x],combine(relf,combine(tl2 arg,irrelf)),relx@xs@irrelx))
  | r_o _ = impossible "r_o in mcopt"
in fun rel_order(a,x) = r_o(a,x,[],[],[],[])
end

local
fun gen_order f =
    let fun order([],[],_,bestf,bestx,otherfs,otherxs) =
	    (bestf,bestx,otherfs,otherxs)
          | order(argp,x::xs,old,bestf,bestx,otherfs,otherxs) =
	    let val head = hd2 argp
		val tail = tl2 argp
		val new = f argp
	    in if Integer.<(new,old)
	       then order(tail,xs,new,head cons2 [],[x],
		          combine(bestf,otherfs),bestx@otherxs)
	       else if new > old
	       then order(tail,xs,old,bestf,bestx,head cons2 otherfs,x::otherxs)
	       else order(tail,xs,old,head cons2 bestf,x::bestx,otherfs,otherxs)
	    end
	  | order _ = impossible "order in mcopt"
    in fn(a,x::xs) =>
       let val hd = hd2 a
	   val tl = tl2 a
	   val bestf = hd cons2 []
       in order(tl,xs,f bestf,bestf,[x],[],[])
       end
    end
in
val relevance_order = gen_order rel
val branch_order = gen_order branch_factor
val arity_order = gen_order arity
end

(* OPT:  rearrange the fields of a tuple into a better order to evaluate.
	 use the relevant test.  if there are no relevant fields,
	 then the first pattern will match - don't bother returning
	 the rest.  if one of the fields is a record, return it first
	 so it is expanded.  if the relevant test does not isolate
	 one field, use the branch factor test, then the arity test. *)

fun opt (arg as ([],_)) = arg
  | opt (pl as hd::_,xl) =
    let val (pats,tl) = strip pl
	val (relf,relx,irrelf,irrelx) = rel_order(pats,xl)
    in case relf of
         [] => ([hd],xl)
       | [_]::_ => (addtail(combine(relf,irrelf),tl),relx@irrelx)
       | _ =>
	 let val (rf,rx,irf,irx) = relevance_order(relf,relx)
	     val rrest = combine(irf,irrelf)
	     val rrestx = irx@irrelx
	 in case rf of
	      [_]::_ => (addtail(combine(rf,rrest),tl),rx@rrestx)
	    | _ =>
	      let val (branchf,branchx,branchfs,branchxs) = branch_order(rf,rx)
		  val brest = combine(branchfs,rrest)
		  val brestx = branchxs@rrestx
	      in case branchf of
		   [_]::_ => (addtail(combine(branchf,brest),tl),branchx@brestx)
		 | _ => let val (arityf,arityx,arityfs,arityxs) =
				arity_order(branchf,branchx)
		        in (addtail(combine(arityf,combine(arityfs,brest)),tl),
			    arityx@arityxs@brestx)
			end
	      end
	 end
    end

end (* structure MCopt *)
