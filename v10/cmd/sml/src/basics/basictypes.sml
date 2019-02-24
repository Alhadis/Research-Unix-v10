(* Copyright 1989 by AT&T Bell Laboratories *)
(* basictypes.sml *)

structure BasicTypes : BASICTYPES = struct

structure Basics=Basics

open Basics Basics.Symbol

val arrowSym 	 = symbol "->"
val boolSym	 = symbol "bool"
val trueSym	 = symbol "true"
val falseSym	 = symbol "false"
val unitSym	 = symbol "unit"
val listSym	 = symbol "list"
val consSym	 = symbol "::"
val nilSym	 = symbol "nil"
val refSym	 = symbol "ref"
val intSym	 = symbol "int"
val realSym	 = symbol "real"
val stringSym	 = symbol "string"
val exceptionSym = symbol "exn"
val contSym	 = symbol "cont"
val arraySym	 = symbol "array"

(* primitive type constructors and types *)

infix -->
val arrowTycon = ref(mkABStyc([arrowSym],2,NO,Stampset.globalStamps))
val ref(TYCON{stamp=arrowStamp,...}) = arrowTycon
fun t1 --> t2 = CONty(arrowTycon,[t1,t2])
fun isArrowType(CONty(ref(TYCON{stamp,...}),_)) = (stamp = arrowStamp)
  | isArrowType(VARty(ref(INSTANTIATED ty))) = isArrowType ty
  | isArrowType _ = false
fun domain(CONty(_,[ty,_])) = ty
  | domain _ = ErrorMsg.impossible "domain"
fun range(CONty(_,[_,ty])) = ty
  | range _ = ErrorMsg.impossible "range"

val intTycon = ref(mkABStyc([intSym],0,YES,Stampset.globalStamps))
val intTy = CONty(intTycon,nil)

val realTycon  = ref(mkABStyc([realSym],0,YES,Stampset.globalStamps))
val realTy = CONty(realTycon,nil)

val stringTycon  = ref(mkABStyc([stringSym],0,YES,Stampset.globalStamps))
val stringTy = CONty(stringTycon,nil)

val exnTycon = ref(mkABStyc([exceptionSym],0,NO,Stampset.globalStamps))
val exnTy = CONty(exnTycon,nil)

val contTycon = ref(mkABStyc([contSym],1,NO,Stampset.globalStamps))

val arrayTycon = ref(mkABStyc([arraySym],1,YES,Stampset.globalStamps))

(* building record and product types *)

fun recordTy(fields: (label * ty) list) : ty = 
    CONty(ref(Tuples.mkRECORDtyc(map (fn (a,b) => a) fields)),
	  (map (fn(a,b)=>b) fields))

fun tupleTy(tys: ty list) : ty =
    CONty(ref(Tuples.mkTUPLEtyc(length tys)), tys)

val unitTycon = ref(Tuples.mkTUPLEtyc(0))
val unitTy = CONty(unitTycon, nil)
val TYCON{stamp = unitTyconStamp, ...}  = !unitTycon
fun isUnitTy(CONty(ref(TYCON{stamp,...}),_)) = stamp = unitTyconStamp
  | isUnitTy _ = false


(* predefined datatypes *)

val alpha = VARty(mkTyvar(IBOUND 0))

(* bool *)

val boolsign = [CONSTANT 0, CONSTANT 1]
val boolTycon = ref(!unitTycon)
val boolTy = CONty(boolTycon,nil)
val falseDcon = 
    DATACON
      {name = falseSym,
       const = true,
       rep = CONSTANT 0,
       typ = ref boolTy,
       sign = boolsign}
val trueDcon =
    DATACON
      {name = trueSym,
       const = true,
       rep = CONSTANT 1,
       typ = ref boolTy,
       sign = boolsign}
val _ = boolTycon := mkDATAtyc([boolSym],0,[falseDcon,trueDcon],YES,Stampset.globalStamps)

(* references *)

val refTycon = ref(!unitTycon)
val refTyfun = TYFUN
		{arity = 1,
		 body = alpha --> CONty(refTycon, [alpha])}
val refPatType = POLYty {sign = [{weakness=infinity,eq=false}], tyfun = refTyfun}
val refDcon = 
    DATACON
      {name = refSym,
       const = false,
       rep = REF,
       typ = ref(POLYty {sign = [{weakness=1,eq=false}], tyfun = refTyfun}),
       sign = [REF]}
val _ = refTycon := mkDATAtyc([refSym],1,[refDcon],YES,Stampset.globalStamps)

(* lists *)

val listsign = [TRANSB,CONSTANT 0]
val listTycon = ref(!unitTycon)
val consDcon =
    DATACON 
      {name = consSym,
       const = false,
       rep = TRANSB,
       typ = ref(POLYty
		  {sign = [{weakness=infinity,eq=false}],
		   tyfun = TYFUN
			    {arity = 1,
			     body = CONty(arrowTycon,
				      [tupleTy[alpha, CONty(listTycon,[alpha])],
				       CONty(listTycon,[alpha])])}}),
       sign = listsign}
val nilDcon = 
    DATACON
      {name = nilSym,
       const = true,
       rep = CONSTANT 0,
       typ = ref(POLYty
		  {sign = [{weakness=infinity,eq=false}],
		   tyfun = TYFUN{arity=1,body=CONty(listTycon,[alpha])}}),
       sign = listsign}
val _ = listTycon := mkDATAtyc([listSym],1,[consDcon,nilDcon],YES,Stampset.globalStamps)

end (* structure BasicTypes *)

