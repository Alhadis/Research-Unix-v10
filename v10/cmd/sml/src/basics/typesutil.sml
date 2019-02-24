(* Copyright 1989 by AT&T Bell Laboratories *)
(* typesutil.sml *)

structure TypesUtil : TYPESUTIL = struct

structure Basics = Basics

open PrintUtil Basics List2 ErrorMsg BasicTypes

(* primitive operations on tycons *)

fun tycStamp(tycon) =
    case tycon
      of TYCON{stamp,...} => stamp
       | _ => impossible "tycStamp"

fun setTycStamp(stamp,tycon) =
    case tycon
      of TYCON{path,arity,eq,kind,...} =>
	   TYCON{stamp=stamp,path=path,arity=arity,eq=eq,kind=kind}
       | _ => impossible "setTycStamp"

fun tycName(tycon) =
    case tycon
      of TYCON{path=name::_,...} => name
       | _ => impossible "tycName"

fun setTycPath(path,tycon) =
    case tycon
      of TYCON{stamp,arity,eq,kind,...} =>
	   TYCON{stamp=stamp,path=path,arity=arity,eq=eq,kind=kind}
       | _ => impossible "setTycName"

fun tyconArity(tycon) =
    case tycon
      of TYCON{arity,...} => arity
       | _ => impossible "tyconArity"

fun eqTycon(tyc1,tyc2) =
    tycStamp(tyc1) = tycStamp(tyc2)

fun prIntPath(p: int list) = printSequence "." print p
fun prSymPath(p: Symbol.symbol list) = printSequence "." printSym p

fun getEpath([],str) = str
  | getEpath(i::r,STRstr{env=REL{s,...},...}) = getEpath(r,s sub i)
  | getEpath _ = impossible "getEpath"

fun getEpathTyc([i],REL{t,...}) = t sub i
  | getEpathTyc(i::r,REL{s,...}) =
    let val STRstr{env,...} = s sub i
     in getEpathTyc(r,env)
    end
  | getEpathTyc _ = impossible "getEpathTyc"

(* lookTycPath: int list * strenv -> tycon
     interpret index path through structure environments *)
fun lookTycPath([i],REL{s,t}) = ((t sub i)
handle Subscript => (print "$$ lookTycPath 1: "; print i; print "\n";raise Subscript))
  | lookTycPath(i::rest,REL{s,t}) =
      (case (s sub i
handle Subscript => (print "$$ lookTycPath 2: "; prIntPath(i::rest); print "\n";
		     raise Subscript))
        of STRstr{env,...} => lookTycPath(rest,env)
	 | NULLstr => (prIntPath (i::rest); newline();
		       ErrorMsg.impossible "TypesUtil.lookTycPath: NULLstr")
	 | INDstr _ => (prIntPath (i::rest); newline();
		        ErrorMsg.impossible "TypesUtil.lookTycPath: INDstr")
	 | SHRstr _ => (prIntPath (i::rest); newline();
		        ErrorMsg.impossible "TypesUtil.lookTycPath: SHRstr"))
  | lookTycPath _ = ErrorMsg.impossible "TypesUtil.lookTycPath.2"

fun tyconInContext env =
    fn (RELtyc path) => lookTycPath(path,env)
     | (INDtyc i) => 
         (case env of REL{t,...} => t sub i | _ => impossible "tyconInContext")
     | (SHRtyc p) => getEpathTyc(p,env)
     | tyc => tyc

fun printableTycon path (STRstr{env as REL{s,t},kind=SIGkind{bindings,...},...}) =
  (fn (RELtyc (p as [_])) => 
      let val TYCON{stamp,arity,eq,kind,path=path'} = lookTycPath(p,env)
       in TYCON{stamp=stamp,arity=arity,eq=eq,kind=kind,path=path'@path}
      end
    | (RELtyc(i::rest)) => 
      let fun scan(STRbind(STRvar{name,binding=INDstr j,...}):: others) =
		 if i=j then printableTycon (name@path) (s sub i) (RELtyc rest)
			else scan others
	    | scan(_::others) = scan others
	    | scan [] = impossible "TypesUtil.printableTycon 1"
       in scan bindings
      end
    | (INDtyc _) => impossible "TypesUtil.printableTycon: INDtyc"
    | (SHRtyc _) => impossible "TypesUtil.printableTycon: SHRtyc"
    | tyc => tyc)
  | printableTycon _ _ = impossible "printableTycon - bad arg"

(* relativize type to structure context, represented by the strenv *)
(* could optimize to share; should merge with applyPoly ??? *)
fun rewriteType getTycon =
    let fun typeInContext ty =
	let fun mapTycons ty =
		case ty
		 of VARty(ref(INSTANTIATED ty')) => mapTycons ty'
		  | FLEXRECORDty(ref(CLOSED ty')) => mapTycons ty'
		  | VARty _ => ty
		  | CONty(ref tyc, argtys) =>
		      CONty(ref(getTycon tyc), map mapTycons argtys)
		  | POLYty{sign,tyfun=TYFUN{arity,body}} =>
		      POLYty{sign=sign,
			     tyfun=TYFUN{arity=arity, body=mapTycons body}}
		  | _ => impossible "typeInContext"
	 in mapTycons ty
	end
     in typeInContext
    end

fun typeInContext(ty,DIR) = ty
  | typeInContext(ty,env) = rewriteType (tyconInContext env) ty

fun printableType str = rewriteType (printableTycon nil str)

fun prune(VARty(tv as ref(INSTANTIATED ty))) : ty =
      let val pruned = prune ty
       in tv := INSTANTIATED pruned; pruned
      end
  | prune(FLEXRECORDty(r as ref(CLOSED(ty)))) =
      let val pruned = prune ty
       in r := CLOSED pruned; pruned
      end
  | prune ty = ty
    
fun eqTyvar(tv1: tyvar, tv2: tyvar) = (tv1 = tv2)

fun mkUBOUND(id : Symbol.symbol) : tvkind =
    let val s = Symbol.name id
	val (start, eq) = if substring(s,1,1) = "'" then (2,true) else (1,false)
        fun digits(i,n) = 
	    let val c = ordof(s,i)
	     in if Ascii.isDigit(c)
		then digits(i+1,10*n+c-Ascii.zero)
		else n
	    end
	    handle Ord => n
        val weakness = if Ascii.isDigit(ordof(s,start)) handle Ord => false
		         then digits(start,0)
		       else if ordof(s,start) = Ascii.underscore then 1
		       else infinity
     in UBOUND{name=id,weakness=weakness,eq=eq}
    end

fun bindTyvars(tyvars: tyvar list) : unit =
    let fun loop([],_) = ()
	  | loop(tv::rest,n) =
	      (tv := IBOUND n;
	       loop(rest,n+1))
     in loop(tyvars,0)
    end

fun bindTyvars1(tyvars: tyvar list) : {weakness:int,eq:bool} list =
    let fun loop([],_) = []
	  | loop((tv as ref(UBOUND{weakness,eq,...}))::rest,n) =
	       (tv := IBOUND n;
	        {weakness=weakness,eq=eq} :: loop(rest,n+1))
     in loop(tyvars,0)
    end

exception SHARE

(* assume that f fails on identity, i.e. f x raises SHARE instead of 
   returning x *)
fun shareMap f nil = raise SHARE
  | shareMap f (x::l) =
      (f x) :: ((shareMap f l) handle SHARE => l)
      handle SHARE => x :: (shareMap f l)

fun applyTyfun(TYFUN{arity,body},args) =
    let fun subst(VARty(ref(IBOUND n))) = nth(args,n)
	  | subst(CONty(tycref,args)) = CONty(tycref, shareMap subst args)
	  | subst(VARty(ref(INSTANTIATED ty))) = subst ty
	  | subst(FLEXRECORDty(ref(CLOSED ty))) = subst ty
	  | subst _ = raise SHARE
     in if arity > 0
	then subst body
	     handle SHARE => body
	else body
    end

exception ReduceType

fun reduceType(CONty(ref(TYCON{kind=DEFtyc tyfun,...}), args)) =
      applyTyfun(tyfun,args)
  | reduceType _ = raise ReduceType

fun headReduceType ty =
    headReduceType(reduceType(ty))
    handle ReduceType => ty

fun equalTycon(tycs) =
    (* needed to deal with abbreviations *)
    eqTycon(tycs) orelse
    case tycs
      of (TYCON{kind=DEFtyc(TYFUN{arity,body}),...},
	  TYCON{kind=DEFtyc(TYFUN{arity=arity',body=body'}),...}) =>
	    arity = arity'
	    andalso equalType(body,body')
       | (TYCON{kind=DEFtyc(TYFUN{arity,body}),...}, tyc) =>
	  let val CONty(ref tyc',_) = body
	   in equalTycon(tyc,tyc')
	  end
       | (tyc, TYCON{kind=DEFtyc(TYFUN{arity,body}),...}) =>
	  let val CONty(ref tyc',_) = body
	   in equalTycon(tyc,tyc')
	  end
       | _ => false

and equalType(ty,ty') =
    let fun eq(VARty(tv),VARty(tv')) =
	      eqTyvar(tv,tv') orelse
	      (case (!tv,!tv')
	         of (IBOUND i1, IBOUND i2) => i1 = i2
		  | _ => false)
	  | eq(ty as CONty(ref tycon, args), ty' as CONty(ref tycon', args')) =
	      if eqTycon(tycon, tycon') then List2.all2 equalType(args,args') 
	      else (equalType(reduceType ty, ty')
		    handle ReduceType =>
		      (equalType(ty,reduceType ty')
		       handle ReduceType => false))
	  | eq _ = false
     in eq(prune ty, prune ty')
    end


(* instantiating polytypes *)

val defaultMETA = META{depth=infinity,weakness=infinity,eq=false}

fun typeArgs n = 
    if n>0
    then VARty(mkTyvar defaultMETA) :: typeArgs(n-1)
    else []

val default_tvprop = {weakness=infinity,eq=false}

fun mkPolySign 0 = []
  | mkPolySign n = default_tvprop :: mkPolySign(n-1)

(* this should be merged with typeInContext and used in varApplied, etc. *)
fun applyPoly(POLYty{sign,tyfun}, abs: int, wmax: int) : ty =
      applyTyfun(tyfun,
      		 map (fn {weakness,eq} => 
		       VARty(ref(META{weakness = if weakness >= infinity
					         then infinity
						 else min(weakness+abs,wmax),
				      depth = infinity,
				      eq = eq})))
		     sign)
  | applyPoly(ty,_,_) = ty


(* matching a scheme against a target type -- used declaring overloadings *)

fun matchScheme(TYFUN{arity,body}: tyfun, target: ty) : ty =
    let val tyenv = array(arity,UNDEFty)
	fun listofarray a =
	    let fun loop i = (a sub i)::loop(i+1) handle Subscript => []
	     in loop 0
	    end
	fun matchTyvar(i:int, ty: ty) : unit = 
	    case tyenv sub i
	      of UNDEFty => update(tyenv,i,ty)
	       | ty' => if equalType(ty,ty')
			then () 
			else impossible("matchScheme: bad tyvar "^makestring i)
        fun match(scheme:ty, target:ty) =
	    case (scheme,prune(target))
	      of (VARty(ref(IBOUND i)),ty) => matchTyvar(i,ty)
	       | (CONty(ref tycon1,args1), pt as CONty(ref tycon2,args2)) =>
		   if eqTycon(tycon1,tycon2)
		   then app2 match (args1, args2)
		   else (match(reduceType scheme, target)
			 handle ReduceType =>
			   (match(scheme, reduceType pt)
			    handle ReduceType =>
			      impossible "matchScheme: match -- tycons "))
	       | _ => impossible "matchScheme: match"
     in case prune target
	  of POLYty{sign,tyfun=TYFUN{arity=arity',body=body'}} =>
	       (match(body,body');
	        POLYty{sign = sign,
		       tyfun = TYFUN{arity = arity',
			             body = if arity>1
					    then tupleTy(listofarray tyenv)
					    else tyenv sub 0}})
	   | ty => 
	       (match(body,ty);
	        if arity>1
		then tupleTy(listofarray tyenv)
		else tyenv sub 0)
    end

(* type unification *)

type labeledTy = label * ty

val eqLabel = Symbol.eq
fun ltLabel(l1,l2) = Symbol.name l1 < Symbol.name l2

exception Unify of string
      and Instantiate

(* functions to determine and check equality types *)
val refstamp = tycStamp(!refTycon)
and arraystamp = tycStamp(!arrayTycon)
exception REFTYC
exception CHECKEQ

fun defineEqTycon findtyc (tyc as TYCON{eq,path,...}) =
    let val checked = ref([]: tycon list)
	fun member(tyc,[]) = false
	  | member(tyc,tyc'::rest) = eqTycon(tyc,tyc') orelse member(tyc,rest)
        fun eqtyc(tyc as TYCON{stamp,kind,path,eq,...}) =
	      if stamp = refstamp orelse stamp = arraystamp then raise REFTYC
	      else (case !eq
		     of YES => ()
		      | NO => raise CHECKEQ
		      | MAYBE => 
			  if member(tyc,!checked) then ()
			  else (checked := tyc :: !checked;
			        case kind
				  of DEFtyc(TYFUN{arity,body}) => eqty body
				   | DATAtyc(dcons) => app eqdcon dcons
				   | UNDEFtyc _ =>
				       impossible "defineEqTycon/eqtyc 1"
				   | _ => ()))
	  | eqtyc _ = impossible "defineEqTycon/eqtyc 2"
        and eqdcon(DATACON{typ=ref(CONty(_,[dom,_])),const=false,...}) = eqty dom
	  | eqdcon(DATACON{typ=ref(POLYty{tyfun=TYFUN{body=CONty(_,[dom,_]),...},...}),
			   const=false,...}) = eqty dom
	  | eqdcon _ = ()
        and eqty(VARty(ref(INSTANTIATED ty))) = eqty ty
	  | eqty(FLEXRECORDty(ref(CLOSED ty))) = eqty ty
	  | eqty(CONty(ref tyc, args)) =
	      ((eqtyc(findtyc tyc); app eqty args) handle REFTYC => ())
	  | eqty _ = ()
     in (eqtyc(tyc); app (fn TYCON{eq,path,...} => (eq := YES)) (!checked))
	handle CHECKEQ => (eq := NO)
	     | REFTYC => (eq := YES)
    end
  | defineEqTycon _ _ = impossible "defineEqTycon - bad arg"

fun eqtyc(tyc as TYCON{stamp,kind,path,eq,...}) =
      if stamp = refstamp orelse stamp = arraystamp then raise REFTYC
      else (case !eq
	      of YES => ()
	       | NO => raise CHECKEQ
	       | _ => impossible "eqtyc 1")
  | eqtyc _ = impossible "eqtyc 2"

fun checkeq ty =
    case ty
      of VARty(ref(INSTANTIATED ty)) => checkeq ty
       | FLEXRECORDty(ref(CLOSED ty)) => checkeq ty
       | FLEXRECORDty(ref(OPEN fields)) =>  
	 (* bug - misses fields filled in later *)
	  app (fn (_,ty) => checkeq ty) fields
       | CONty(ref(tyc), args) =>
	  ((eqtyc tyc; app checkeq args) handle REFTYC => ())
       | VARty(ref(UBOUND{eq,...})) => 
	  if eq then () else (raise CHECKEQ)
       | _ => ()

fun checkEqTySig(ty, sign: polysign) =
    let fun eqty(VARty(ref(INSTANTIATED ty))) = eqty ty
	  | eqty(FLEXRECORDty(ref(CLOSED ty))) = eqty ty
	  | eqty(CONty(ref tyc, args)) =
	      ((eqtyc tyc; app eqty args) handle REFTYC => ())
	  | eqty(VARty(ref(IBOUND n))) = 
	      let val {eq,...} = nth(sign,n)
	       in if eq then () else raise CHECKEQ
	      end
	  | eqty _ = ()
     in eqty ty
    end

fun isEqTycon(TYCON{stamp,kind,path,eq,...}) =
    (case !eq
       of YES => true
	| NO => false
	| _ => impossible "isEqTycon 1")
  | isEqTycon _ = impossible "isEqTycon 2"

fun isEqType(ty) = ((checkeq ty; true) handle CHECKEQ => false)

fun instantiate(tv as ref kind: tyvar, ty: ty) : unit =
    case kind
      of META{depth=d,weakness=w,eq=e} =>
	   let fun scan(ty: ty) : unit =
	           case ty  (* "pruning" done in-line *)
		     of VARty(ref(INSTANTIATED ty')) => scan ty'
		      | VARty(tv') => 
			if eqTyvar(tv, tv')
			then raise Unify("circularity")
			else (case !tv'
			       of META{depth=d',weakness=w',eq=e'} =>
				    tv' := META{depth = min(d,d'),
					   	weakness = min(w,w'),	
						eq = e orelse e'}
				| UBOUND{weakness=w',eq=e',...} =>
				    if w < w'
				    then raise Unify("weakness violation")
				    else ()
				| _ => ())
		      | CONty(_, args) => app scan args
		      | FLEXRECORDty(ref(OPEN fields)) =>
			  app (fn (_,ty') => scan ty') fields
		      | FLEXRECORDty(ref(CLOSED ty)) => scan ty
		      | _ => ()   (* propagate error *)
            in if e
	       then (checkeq ty
		     handle CHECKEQ => raise Unify "equality type required")
	       else ();
	       scan ty;
	       tv := INSTANTIATED ty
	   end
       | UBOUND _ => (case ty of ERRORty => () | _ => raise Instantiate)
       | IBOUND n => impossible("instantiate -- IBOUND: "^makestring n)
       | INSTANTIATED _ => impossible "instantiate -- INSTANTIATED"

fun unifyFields(labtys1: labeledTy list, labtys2: labeledTy list): labeledTy list =
    case (labtys1, labtys2)
      of ([],_) => labtys2
       | (_,[]) => labtys1
       | ((labty1 as (lab1,ty1))::labtys1',
	  (labty2 as (lab2,ty2))::labtys2') =>
	    if eqLabel(lab1,lab2)
	    then (unifyTy(ty1,ty2);  (* type error possible *)
		  labty1 :: unifyFields(labtys1',labtys2'))
	    else if ltLabel(lab1, lab2)
	    then labty1 :: unifyFields(labtys1', labtys2)
	    else labty2 :: unifyFields(labtys1, labtys2')

and matchFields(fields: labeledTy list, labels: label list,
		argTys: ty list) =
    case (fields, labels)
      of ([],_) => ()
       | (_,[]) => raise Unify("record length")
       | ((lab1,ty1)::fields', lab2::labels') =>
	   if eqLabel(lab1,lab2)
	   then let val ty2::argTys' = argTys
		 in unifyTy(ty1,ty2);
		    matchFields(fields',labels',argTys')
		end
           else if ltLabel(lab2,lab1)
	   then matchFields(fields, labels', tl argTys)
	   else raise Unify("record labels")

and unifyTy(ty1: ty, ty2: ty): unit =
    let val ty1 = prune ty1
	and ty2 = prune ty2
     in case (ty1,ty2)
	  of (VARty(tv1),VARty(tv2)) =>
	       if eqTyvar(tv1,tv2)
	       then ()
	       else (instantiate(tv1,ty2)
		     handle Instantiate =>  (* tv1 is UBOUND *)
		       instantiate(tv2,ty1)
		       handle Instantiate => (* tv2 also UBOUND *)
		         raise Unify("bound type var"))
	   | (VARty(tv1),_) => (instantiate(tv1,ty2)
			        handle Instantiate =>
			          raise Unify("bound type var"))
	   | (_,VARty(tv2)) => (instantiate(tv2,ty1)
			        handle Instantiate =>
			          raise Unify("bound type var"))
	   | (CONty(ref(TYCON{kind=RECORDtyc labels,...}), argTys),
	      FLEXRECORDty(r as ref(OPEN fields))) =>
	        (matchFields(fields, labels, argTys);
		 r := CLOSED(ty1))

	   | (FLEXRECORDty(r as ref(OPEN fields)),
	      CONty(ref(TYCON{kind=RECORDtyc labels,...}), argTys)) =>
	        (matchFields(fields, labels, argTys);
		 r := CLOSED(ty2))

	   | (FLEXRECORDty(r1 as ref(OPEN fields1)),
	      FLEXRECORDty(r2 as ref(OPEN fields2))) =>
		(r1 := CLOSED(FLEXRECORDty(ref(OPEN(unifyFields(fields1,fields2)))));
                 r2 := !r1)

	   | (CONty(ref tycon1, args1), CONty(ref tycon2, args2)) =>
	       if eqTycon(tycon1, tycon2)
	       then unifyArgs(args1, args2)
	       else (unifyTy(reduceType ty1, ty2)
		     handle ReduceType =>
		       unifyTy(ty1, reduceType ty2)
		       handle ReduceType => raise Unify("tycon mismatch"))
	   | (CONty(ref(TYCON{kind=DEFtyc _,...}),_),_) =>
	       unifyTy(reduceType ty1, ty2)
	   | (_,CONty(ref(TYCON{kind=DEFtyc _,...}),_)) =>
	       unifyTy(ty1, reduceType ty2)
	   | (ERRORty, _) => ()   (* propagate error *)
	   | (_, ERRORty) => ()   (* propagate error *)
           | _ => raise Unify("type mismatch")
    end

and unifyArgs([],[]) = ()
  | unifyArgs(ty1::rest1, ty2::rest2) = (unifyTy(ty1,ty2); unifyArgs(rest1,rest2))
  | unifyArgs(_) = raise Unify("argument numbers");

fun unifyTypes([]: ty list) : ty = raise Unify("empty set")
  | unifyTypes(ty :: rest) = (app (fn ty' => unifyTy(ty, ty')) rest; ty);

end (* structure TypesUtil *)
