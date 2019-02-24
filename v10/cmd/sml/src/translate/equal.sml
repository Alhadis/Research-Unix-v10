(* Copyright 1989 by AT&T Bell Laboratories *)
(* equal.sml *)
signature EQUAL =
sig
  structure Basics : BASICS
  structure Lambda : LAMBDA
  val equal : Basics.ty -> Lambda.lexp
end

structure Equal : EQUAL =
struct

  structure Basics : BASICS = Basics
  structure Lambda : LAMBDA = Lambda

open ErrorMsg Basics Lambda Access BasicTypes TypesUtil

fun translatepath [v] = VAR v
  | translatepath (x::p) = SELECT(x,translatepath p)
  | translatepath nil = ErrorMsg.impossible "equal.translatepath nil"

fun argType(POLYty{tyfun=TYFUN{arity,body=CONty(_,[domain,_])},...}, args) =
      applyTyfun(TYFUN{arity=arity,body=domain},args)
  | argType(CONty(_,[argty,_]), []) = argty
  | argType _ = impossible "Equal.argType"


exception Poly

fun atomeq tyc =
	  if eqTycon(tyc,!intTycon) 
	     orelse eqTycon(tyc,!boolTycon)
	     orelse eqTycon(tyc,!refTycon)
	     orelse eqTycon(tyc,!arrayTycon) then PRIM P.ieql
	  else if eqTycon(tyc,!realTycon) then PRIM P.feql
	  else if eqTycon(tyc,!stringTycon) then 
	    translatepath(!CoreInfo.stringequalPath)
	  else raise Poly

exception Notfound

val trueLexp = CON(trueDcon,RECORD[])
val falseLexp = CON(falseDcon,RECORD[])

fun equal (concreteType : ty) : lexp =
    let val cache : (ty * lexp * lexp ref) list ref = ref nil
	fun enter ty =
	    let val v = VAR(mkLvar())
		val r = ref v
	     in if !System.Control.debugging 
		  then (print "enter: "; PrintType.printType ty; print "\n")
		  else ();
		cache := (ty, v, r) :: !cache; (v,r)
	    end
	fun find ty =
	    let fun f ((t,v,e)::r) = if equalType(ty,t) then v else f r
		  | f nil = (if !System.Control.debugging
			      then print "find-notfound\n"
			      else ();
			     raise Notfound)
	     in if !System.Control.debugging 
		 then (print "find: "; PrintType.printType ty; print "\n")
		 else ();
		f (!cache)
	    end

	fun test(ty) =
	(if !System.Control.debugging
	 then (print "test: "; PrintType.printType ty; print "\n")
	 else ();
	 case ty
	  of VARty(ref(INSTANTIATED t)) => test t
	   | FLEXRECORDty(ref(CLOSED t)) => test t
	   | CONty(ref(tyc as TYCON{kind=ABStyc,eq=ref YES,...}),tyl) =>  atomeq tyc
	   | CONty(ref(tyc as TYCON{kind=ABStyc,eq=ref NO,...}),tyl) =>
	       impossible("Attempt to test opaque type for equality: "
				     ^ Symbol.name(tycName tyc))
	   | CONty(ref(TYCON{kind=DEFtyc _,...}),tyl) => test(reduceType ty)
	   | CONty(ref(tyc as
		       TYCON{kind=DATAtyc([DATACON{const=false,rep,typ,...}]),
		             ...}),
		   tyl) =>
	       (case rep
		  of TRANSPARENT => 
		       (find ty handle Notfound =>
			 let val (eqv,patch) = enter ty
			     val v = mkLvar()
	                     val ty' = argType(!typ,tyl)
			  in patch := FN(v,APP(test ty', VAR v));
			     eqv
		         end)
		   | REF => atomeq tyc
		   | _ => impossible "Equal #498")
	   | CONty(ref(TYCON{kind,...}), tyl) =>
	      (find ty
	       handle Notfound =>
	       let val v = mkLvar() and x=mkLvar() and y=mkLvar()
		   val (eqv, patch) = enter ty
		   fun inside (DATACON{const=true,...}) = trueLexp
		     | inside (c as DATACON{typ, const=false,...}) =
			 APP(test(argType(!typ,tyl)),
			     RECORD[DECON(c, VAR x),
				    DECON(c, VAR y)])
		   val body = 
		       case kind
			 of DATAtyc([dcon]) =>
			      inside dcon	
			  | DATAtyc(dcons) =>
			      let fun concase dcon =
				      (DATAcon(dcon),
				       SWITCH(VAR y,[(DATAcon(dcon), inside dcon)],
					    SOME(falseLexp)))
			       in SWITCH(VAR x,map concase dcons,NONE)
			      end
			  | RECORDtyc _ =>
			    let fun loop(n,[ty]) =
					 APP(test(ty), RECORD[SELECT(n, VAR x),
						SELECT(n, VAR y)])
				  | loop(n,ty::r) =
				      SWITCH(loop(n,[ty]),
				       [(DATAcon(trueDcon), loop(n+1,r)),
					(DATAcon(falseDcon), falseLexp)],
				       NONE)
				  | loop(_,nil) = trueLexp
			     in loop(0,tyl)
			    end
			  | _ => raise Poly
		in patch := FN(v,APP(FN(x,APP(FN(y,body),
					      SELECT(1,VAR v))),
				     SELECT(0,VAR v)));
		   eqv
	       end)
	   | _ => raise Poly)

	val body = test(concreteType)

     in FIX(map (fn (_,VAR v,_) => v | _ => impossible "Equal #324") (!cache),
	    map (fn (_,_,e) => !e) (!cache),
	    body)
    end
    handle Poly => translatepath(!CoreInfo.polyequalPath)
         | Syntax =>
	   (print "equal: type = ";
	    PrintType.resetPrintType();
	    PrintType.printType concreteType; print"\n";
	    RECORD[])
		
end (* struct *)

