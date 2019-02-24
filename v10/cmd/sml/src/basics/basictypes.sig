(* Copyright 1989 by AT&T Bell Laboratories *)
(* basictypes.sig *)

signature BASICTYPES = sig

structure Basics: BASICS

infix -->
val arrowTycon : Basics.tycon ref
val --> : Basics.ty * Basics.ty -> Basics.ty
val isArrowType : Basics.ty -> bool
val domain : Basics.ty -> Basics.ty
val range : Basics.ty -> Basics.ty

val intTycon : Basics.tycon ref
val intTy : Basics.ty

val realTycon  : Basics.tycon ref
val realTy : Basics.ty

val stringTycon  : Basics.tycon ref
val stringTy : Basics.ty

val exnTycon : Basics.tycon ref
val exnTy : Basics.ty

val contTycon : Basics.tycon ref

val arrayTycon : Basics.tycon ref

val unitTycon : Basics.tycon ref
val unitTy : Basics.ty
val isUnitTy : Basics.ty -> bool

val recordTy : (Basics.label * Basics.ty) list -> Basics.ty
val tupleTy : Basics.ty list -> Basics.ty

val boolTycon : Basics.tycon ref
val boolTy : Basics.ty
val falseDcon : Basics.datacon
val trueDcon : Basics.datacon

val refTycon : Basics.tycon ref
val refPatType : Basics.ty
val refDcon : Basics.datacon

val listTycon : Basics.tycon ref
val nilDcon : Basics.datacon
val consDcon : Basics.datacon

end (* signature BASICTYPES *)
