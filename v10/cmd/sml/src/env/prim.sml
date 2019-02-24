(* Copyright 1989 by AT&T Bell Laboratories *)
structure Prim : sig structure Access : ACCESS
		     structure Basics : BASICS
		     val primTypes : Basics.Structure
		     val inLine : Basics.Structure
		     val inLineName : Access.primop -> string
		     val pure : Access.primop -> bool
		     val special : Access.access -> bool
		 end = 
struct

   structure Access = Access
   structure Basics = Basics
   
   open Access Basics BasicTypes Env NameSpace

   val symbol = Symbol.symbol

(* primTypes structure *)

   val typesTable = newTable()

   fun entercon(s: string, c: datacon) =
       IntStrMap.add typesTable
         (varIndex(symbol s),s,CONbind c)

   fun entertyc(s : string, t : tycon ref) =
       IntStrMap.add typesTable 
         (tycIndex(symbol s),s,TYCbind t) (*  *)
   
   val primTypes = (
	entertyc("bool", boolTycon);
	entercon("true", trueDcon);
	entercon("false", falseDcon);
	
	entertyc("list", listTycon);
	entercon("::", consDcon);
	IntStrMap.add typesTable
	  (fixIndex(symbol "::"),
	   "::",FIXbind(FIXvar{name=symbol "::",binding=infixright 5}));
	entercon("nil", nilDcon);
	
	entertyc("ref", refTycon);
	entercon("ref", refDcon);

	entertyc("unit", unitTycon);
	entertyc("int", intTycon);
	entertyc("real", realTycon);
	entertyc("cont", contTycon);
	entertyc("array", arrayTycon);
	entertyc("string", stringTycon);

        entertyc("exn",exnTycon);

	mkSTR([symbol "PrimTypes"], typesTable, DIR, Stampset.globalStamps))


(* inLine structure *)

   val inLineTable = newTable()

   val bottom = POLYty{sign=[{weakness=infinity,eq=false}],
		       tyfun=TYFUN{arity=1,body=VARty(mkTyvar(IBOUND 0))}}

   val primopNames = [
        ("callcc",P.callcc),
        ("throw",P.throw),
	("delay",P.delay),
	("force",P.force),
	("!",P.!),
	("*",P.*),
	("+",P.+),
	("-",P.-),
	(":=",P.:=),
	("<",P.<),
	("<=",P.<=),
	(">",P.>),
	(">=",P.>=),
	("alength",P.alength),
	("boxed",P.boxed),
	("div",P.div),
	("orb",P.orb),
	("andb",P.andb),
	("xorb",P.xorb),
	("lshift",P.lshift),
	("rshift",P.rshift),
	("notb",P.notb),
	("cast",P.cast),
	("=",P.eql),
	("fadd",P.fadd),
	("fdiv",P.fdiv),
	("feql",P.feql),
	("fge",P.fge),
	("fgt",P.fgt),
	("fle",P.fle),
	("flt",P.flt),
	("fmul",P.fmul),
	("fneg",P.fneg),
	("fneq",P.fneq),
	("fsub",P.fsub),
	("gethdlr",P.gethdlr),
	("ieql",P.ieql),
	("ineq",P.ineq),
	("<>",P.neq),
	("makeref",P.makeref),
	("ordof",P.ordof),
	("profile",P.profile),
	("sethdlr",P.sethdlr),
	("slength",P.slength),
	("store",P.store),
	("subscript",P.subscript),
	("unboxedassign",P.unboxedassign),
	("unboxedupdate",P.unboxedupdate),
	("update",P.update),
	("~",P.~) ]

   fun enter( s : string, p : primop ) =
       let val name = symbol s
        in IntStrMap.add inLineTable
              (varIndex name,
	       s,VARbind(VALvar{access=INLINE p,
			        name=[name],typ=ref bottom}))
       end

   val inLine =
       (app enter primopNames;
        mkSTR([symbol "InLine"], inLineTable, DIR, Stampset.globalStamps))

   fun inLineName p =
       let fun find [] = ErrorMsg.impossible "Bad primop name"
	     | find ((s,p1)::rest) = if p1=p then s else find rest
        in find primopNames
       end

 val pure =
   fn P.:= => false
    | P.! => false (****)
    | P.subscript => false (****)
    | P.store => false
    | P.unboxedassign => false
    | P.unboxedupdate => false
    | P.update => false
    | P.callcc => false
    | P.~ => false (* these must be here because they may raise *)
    | P.+ => false
    | P.- => false
    | P.* => false
    | P.div => false
    | P.fneg => false
    | P.fadd => false
    | P.fsub => false
    | P.fmul => false
    | P.fdiv => false
    | P.lshift => false
    | P.force => false
    | _ => true

   fun special(INLINE P.eql) = true
     | special(INLINE P.neq) = true
     | special(INLINE P.:=) = true
     | special(INLINE P.update) = true
     | special _ = false

end (* structure Prim *)

