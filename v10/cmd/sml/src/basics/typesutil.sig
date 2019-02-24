(* Copyright 1989 by AT&T Bell Laboratories *)
(* typesutil.sig *)

(* types.sig *)

signature TYPESUTIL = sig

  structure Basics: BASICS
  
  (* primitive operations on tycons *)
  val tycStamp : Basics.tycon -> int
  val setTycStamp : int * Basics.tycon -> Basics.tycon
  val tycName : Basics.tycon -> Basics.Symbol.symbol
  val setTycPath : Basics.Symbol.symbol list * Basics.tycon -> Basics.tycon
  val tyconArity : Basics.tycon -> int
  val eqTycon : Basics.tycon * Basics.tycon -> bool
  val getEpath : int list * Basics.Structure -> Basics.Structure
  val getEpathTyc : int list * Basics.strenv -> Basics.tycon 
  val tyconInContext : Basics.strenv -> Basics.tycon -> Basics.tycon

  val typeInContext : Basics.ty * Basics.strenv -> Basics.ty
  val printableType : Basics.Structure -> Basics.ty -> Basics.ty
  val prune : Basics.ty -> Basics.ty

  val defaultMETA : Basics.tvkind
  val eqTyvar : Basics.tyvar * Basics.tyvar -> bool
  val mkUBOUND : Basics.Symbol.symbol -> Basics.tvkind
  val bindTyvars : Basics.tyvar list -> unit
  val bindTyvars1 : Basics.tyvar list -> {weakness:int, eq:bool} list
    
  exception ReduceType
  val applyTyfun : Basics.tyfun * Basics.ty list -> Basics.ty
  val reduceType : Basics.ty -> Basics.ty
  val headReduceType : Basics.ty -> Basics.ty
  val equalTycon : Basics.tycon * Basics.tycon -> bool
  val equalType  : Basics.ty * Basics.ty -> bool

  (* making a "generic" copy of a type *)
  val typeArgs : int -> Basics.ty list
  val mkPolySign : int -> {weakness:int, eq:bool} list
  val applyPoly : Basics.ty * int * int -> Basics.ty
  
  (* matching a scheme against a target type -- used declaring overloadings *)
  val matchScheme : Basics.tyfun * Basics.ty -> Basics.ty
  
  (* type unification *)
  exception Unify of string
  exception CHECKEQ
  val defineEqTycon : (Basics.tycon -> Basics.tycon) -> Basics.tycon -> unit
  val checkEqTySig : Basics.ty * Basics.polysign -> unit
  val isEqTycon : Basics.tycon -> bool
  val isEqType : Basics.ty -> bool
  val instantiate : Basics.tyvar * Basics.ty ->  unit
  val unifyTy : Basics.ty * Basics.ty -> unit
  val unifyTypes : Basics.ty list -> Basics.ty

end  (* signature TYPESUTIL *)
