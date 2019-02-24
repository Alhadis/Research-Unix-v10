(* Copyright 1989 by AT&T Bell Laboratories *)
(* access.sml *)

(* use "symbol.sig"; use "access.sig"; use "intmap.sig";

functor AccessF(structure Intmap: INTMAP
                      structure Symbol: SYMBOL) : ACCESS =
*)
structure Access : ACCESS =
struct

  structure Symbol = Symbol
  structure P = 
    struct 
      datatype primop = 
	! | * | + | - | := | < | <= | > | >= | alength | boxed | div | cast |
	eql | fadd |fdiv |feql |fge |fgt |fle |flt |fmul |fneg |fneq |fsub |
	gethdlr | ieql | ineq | neq | makeref | ordof | profile |
	sethdlr | slength | callcc | throw | delay | force |
	store | subscript | unboxedassign | unboxedupdate | update | ~ |
	rshift | lshift | orb | andb | xorb | notb

      fun pr_primop(!) = "!"
      |   pr_primop(op *) = "*"
      |   pr_primop(op +)  = "+"
      |   pr_primop(op -) = "-"
      |   pr_primop(op :=) = ":="
      |   pr_primop(op <)  = "<"
      |   pr_primop(op <=) = "<="
      |   pr_primop(op >)  = ">"
      |   pr_primop(op >=) = ">="
      |   pr_primop (alength) = "alength"
      |   pr_primop(boxed) = "boxed"
      |   pr_primop (op div) = "div"
      |   pr_primop cast = "cast"
      |   pr_primop eql = "eql"
      |   pr_primop fadd = "fadd"
      |   pr_primop fdiv = "fdiv"
      |   pr_primop feql = "feql"
      |   pr_primop fge  = "fge"
      |   pr_primop fgt  = "fgt"
      |   pr_primop fle = "fle"
      |   pr_primop flt = "flt"
      |   pr_primop fmul = "fmul"
      |   pr_primop fneg = "fneg"
      |   pr_primop fneq = "fneq"
      |   pr_primop fsub = "fsub"
      |   pr_primop gethdlr = "gethdlr"
      |   pr_primop ieql = "ieql"
      |   pr_primop ineq = "ineq"
      |   pr_primop neq = "neq"
      |   pr_primop makeref = "makeref"
      |   pr_primop ordof = "ordof"
      |   pr_primop profile = "profile"
      |   pr_primop sethdlr = "sethdlr"
      |   pr_primop slength = "slength"
      |   pr_primop callcc = "callcc"
      |   pr_primop throw = "throw"
      |   pr_primop store = "store"
      |   pr_primop subscript = "subscript"
      |   pr_primop unboxedassign = "unboxedassign"
      |   pr_primop unboxedupdate = "unboxedupdate"
      |   pr_primop (op update) = "update"
      |   pr_primop(~) = "~"
      |   pr_primop(rshift) = "rshift"
      |   pr_primop(lshift) = "lshift"
      |   pr_primop(orb) = "orb"
      |   pr_primop(andb) = "andb"
      |   pr_primop(xorb) = "xorb"
      |   pr_primop(notb) = "notb"
    end

  type lvar = int      (* lambda variable id number *)
  type slot = int      (* position in structure record *)
  type path = int list (* slot chain terminated by lambda variable id number *)
  type primop = P.primop

  datatype access 
    = LVAR of lvar
    | SLOT of slot
    | PATH of path  
    | INLINE of primop

  (* local *)
    val varcount = ref 0
    exception NoLvarName
    val lvarNames : string Intmap.intmap = Intmap.new(32, NoLvarName)
    val name = Intmap.map lvarNames
    val giveLvarName = Intmap.add lvarNames

  val saveLvarNames = System.Control.saveLvarNames
  fun mkLvar () : lvar = (inc varcount; !varcount)
  fun sameName(v,w) =
      if !saveLvarNames
      then giveLvarName(v,name w)
	     handle NoLvarName => (giveLvarName(w, name v)
				      handle NoLvarName => ())
      else ()
  fun dupLvar v =
      (inc varcount;
       if !saveLvarNames
       then giveLvarName(!varcount,name v) handle NoLvarName => ()
       else ();
       !varcount)
  fun namedLvar(id: Symbol.symbol) =
      (inc varcount;
       if !saveLvarNames then giveLvarName(!varcount,Symbol.name id) else ();
       !varcount)
  fun lvarName(lv : lvar) : string =
      (name lv ^ makestring lv) handle NoLvarName => makestring lv

  fun pr_lvar(lvar:lvar) = makestring(lvar)
  fun pr_slot(slot:slot) = makestring(slot)
  fun pr_path'[] = "]"
  |   pr_path'[x:int] = makestring x ^ "]"
  |   pr_path'((x:int)::rest)= makestring x ^ "," ^ pr_path' rest
  fun pr_path path = "[" ^ pr_path' path
  fun pr_access (LVAR lvar) = "LVAR(" ^ pr_lvar lvar ^ ")"
  |   pr_access (SLOT slot) = "SLOT(" ^ pr_slot slot ^ ")"
  |   pr_access (PATH path) = "PATH(" ^ pr_path path ^ ")"
  |   pr_access (INLINE po) = "INLINE(" ^ P.pr_primop po ^ ")"

end  (* structure Access *)

(*
structure Access= AccessF(structure Intmap= Intmap
                                structure Symbol= Symbol);

*)
