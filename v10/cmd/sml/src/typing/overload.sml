(* Copyright 1989 by AT&T Bell Laboratories *)
(* overload.sml *)

structure Overload : OVERLOAD = struct

structure Basics = Basics

local open Basics TypesUtil PrintUtil ErrorMsg BasicTypes
   PrintType
 in

type subst = (tyvar * tvkind) list

exception SoftUnify

fun copyScheme (tyfun as TYFUN{arity,...}) : ty * ty =
    let val tvs = typeArgs arity
     in (applyTyfun(tyfun,tvs),
	 if arity>1 then tupleTy tvs else hd tvs)
    end

fun rollBack subst =
    let fun loop (nil,trace) = trace
	  | loop (((tv as ref kind),oldkind)::subst,trace) =
	       (tv := oldkind;
		loop(subst,(tv,kind)::trace))
     in loop(subst,nil)
    end

fun redoSubst nil = ()
  | redoSubst ((tv,INSTANTIATED ty)::rest) =
      (instantiate(tv, ty); redoSubst rest)
  | redoSubst (_) = impossible "Overload--redoSubst"

fun softUnify(ty1: ty, ty2: ty): subst =
    let val subst: subst ref = ref nil
	fun softInst(tv as ref kind: tyvar, ty: ty) : unit =
	    let fun scan(ty: ty) : unit =  (* simple occurrence check *)
		   case ty
		     of VARty(tv') => 
		          if eqTyvar(tv, tv') then raise SoftUnify else ()
		      | CONty(_, args) => app scan args
		      | FLEXRECORDty(ref(OPEN fields)) =>
		          app (fn (_,ty') => scan ty') fields
		      | ty => ()  (* propagate error *)
	     in case kind
		  of META _ => ()
		   | _ => raise SoftUnify;
 	        scan ty;
		subst := (tv, kind)::(!subst);
		tv := INSTANTIATED ty
	    end
	
	fun unify(ty1: ty, ty2: ty): unit =
	    let val ty1 = prune ty1
		and ty2 = prune ty2
	     in case (ty1,ty2)
		  of (VARty(tv1),VARty(tv2)) =>
		       if eqTyvar(tv1,tv2) then () else softInst(tv1,ty2)
		   | (VARty(tv1),_) => softInst(tv1,ty2)
		   | (_,VARty(tv2)) => softInst(tv2,ty1)
		   | (CONty(ref tycon1, args1), CONty(ref tycon2, args2)) =>
		       if eqTycon(tycon1, tycon2)
		       then unifyLists(args1, args2)
		       else (unify(reduceType ty1, ty2)
			     handle ReduceType => 
			       unify(ty1, reduceType ty2)
			       handle ReduceType => raise SoftUnify)
		   | (ERRORty, _) => ()  (* propagate error *)
		   | (_, ERRORty) => ()  (* propagate error *)
		   | _ => raise SoftUnify
	    end
	
	and unifyLists([],[]) = ()
	  | unifyLists(ty1::rest1, ty2::rest2) = 
	      (unify(ty1,ty2); unifyLists(rest1,rest2))
	  | unifyLists(_) = raise SoftUnify

     in unify(ty1,ty2)
	  handle SoftUnify => (rollBack(!subst); raise SoftUnify);
	!subst
    end


exception Overld

datatype stack
  = EMPTY
  | PUSH of var ref * ty * stack
  | MARK of stack

val overloaded = ref EMPTY  (* stack of currently unresolved overloaded vars *)

fun resetOverloaded () = overloaded := EMPTY

fun markOverloaded () = overloaded := MARK(!overloaded)

fun pushOverloaded (refvar as ref(OVLDvar{options,scheme,...}) : var ref) = 
	   let val (scheme',ty) = copyScheme(scheme)
	    in overloaded := PUSH(refvar,ty,!overloaded);
	       scheme'
	   end
  | pushOverloaded _ = impossible "overload.1"

fun resolveOVLDvar(rv as ref(OVLDvar{name,options,...}),context) =
   (let fun findFirst({indicator, variant}::rest) =
	    ((softUnify(applyPoly(indicator,0,infinity), context), variant, rest)
	      handle SoftUnify => findFirst(rest))
	  | findFirst(nil) = 
	      (complain("overloaded variable \"" ^ Symbol.name(name) ^
	                "\" not defined at type:");
	       print "   ";
	       resetPrintType(); printType(context); newline();
	       raise Overld)
	fun findSecond({indicator, variant}::rest) =
	    ((rollBack(softUnify(applyPoly(indicator,0,infinity), context));
	      complain("overloaded variable \"" ^ Symbol.name(name) ^
			"\" cannot be resolved");
	      raise Overld)
	     handle SoftUnify => findSecond(rest))
	  | findSecond(nil) = ()
        val (subst,var,restOptions) = findFirst(!options)
	val subst = rollBack subst
     in findSecond(restOptions);
	redoSubst subst;
	rv := var
    end handle Overld => ())
  | resolveOVLDvar _ = impossible "overload.2"

fun resolveOverloaded () =
    let fun loop EMPTY = ()
	  | loop(MARK(s)) = (overloaded := s)
	  | loop(PUSH(refvar,context,s)) = 
	      (resolveOVLDvar(refvar,context); loop(s))
     in loop(!overloaded)
    end

end (* local *)

end (* structure Overload *)
