(* Copyright 1989 by AT&T Bell Laboratories *)
(* env.sig *)

signature ENV =
sig
   (*  structure Intmap: TABLE
       structure Basics: BASICS
       sharing Intmap = Basics.Intmap *)
  type binding (* = Basics.binding *)
  type info (* = {path: int list,
                  strenv: {s: Basics.Structure array, t: Basics.tycon array}} *)
  type symtable (* = Basics.binding IntStrMap.intstrmap *)
  type env
  datatype 'a locality = LOCAL of 'a | GLOBAL of 'a
  exception Unbound
  exception Unboundrec
  exception UnboundTable
  val newTable : unit -> symtable
  val appenv : (int * string * binding -> unit) -> env * env -> unit
  val current : unit -> env
  val openOld : info * symtable -> unit
  val openNew : info * symtable -> unit
  val openRec : unit -> unit
  val openStr : unit -> unit
  val closeStr : unit -> unit
  val openScope : unit -> env
  val resetEnv : env -> unit
  val collectTable : ((int * string * binding) * info -> unit) -> unit
  val splice : env * env -> unit
  val add : int * string * binding -> unit
  val lookEnv : env * (int * string) -> binding * info
  val look : int * string -> binding * info
  val lookStrLocal : int * string -> binding * info
  val lookRec : int * string -> (binding * info) locality
  val lookRecLocal : int * string -> binding * info
  val commit : unit -> unit
  val restore : unit -> unit
  val previous : unit -> env
  val consolidate : unit -> unit
  val reset : unit -> unit

  val popModule: env -> Basics.symtable

  val closeCurrentNewEnv: unit -> env

end
