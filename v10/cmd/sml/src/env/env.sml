(* Copyright 1989 by AT&T Bell Laboratories *)
structure Env : ENV =
struct
  open PrintUtil ErrorMsg Symbol Access (* also uses: IntStrMap, Basics *)
  type binding = Basics.binding

  type info = {path: int list, strenv: Basics.strenv}
  type symtable = binding IntStrMap.intstrmap
  type key = int * string

  val debugLook = System.Control.debugLook
  val debugCollect = System.Control.debugCollect

  val defaultInfo = {path = []:int list, strenv = Basics.DIR}

  fun name(_,s) = s

  exception Unbound
	and Unboundrec
  exception UnboundTable = System.Unsafe.Assembly.UnboundTable

  datatype 'a locality = LOCAL of 'a | GLOBAL of 'a

  datatype env
    = TBL of
       {info: info, table: symtable, prev: env ref, isopen: bool}
    | REC of env ref
    | STR of env ref
    | NIL

  fun prevEnv(TBL{prev,...}) = prev
    | prevEnv(REC prev) = prev
    | prevEnv(STR prev) = prev
    | prevEnv _ = impossible "prevEnv"

  fun printEnv(TBL{prev,...}) = (print "TBL\n  "; printEnv(!prev))
    | printEnv(REC prev) = (print "REC\n  "; printEnv(!prev))
    | printEnv(STR prev) = (print "STR\n  "; printEnv(!prev))
    | printEnv(NIL) = ()

  fun printEnvs(e) = (print "Env:\n  "; printEnv e)

  fun eqEnv(NIL,NIL) = true
    | eqEnv(REC r, REC r') = r = r'
    | eqEnv(STR r, STR r') = r = r'
    | eqEnv(TBL{prev=r,...}, TBL{prev=r',...}) = r = r'
    | eqEnv _ = false

  fun appenv f (newenv,oldenv) =
      let fun loop env =
	  if eqEnv(env,oldenv)
	  then ()
	  else case env
		 of TBL{table,prev=ref env',...} => (IntStrMap.app f table; loop env')
		  | REC(ref env') => loop env'
		  | STR(ref env') => loop env'
		  | NIL => ()
       in loop newenv
      end

 (* the global variable containing the current environment *)
  val env = ref(NIL)
  fun current() = !env

  fun closeCurrentNewEnv() =
      case current()
       of TBL{info,table,prev,isopen=true} =>
	    TBL{info=info,table=table,prev=prev,isopen=false}
        | _ => impossible "Env.closeCurrentNewEnv()"

  fun newTable() = IntStrMap.new(32, UnboundTable) : symtable

  fun openOld (info: info, table: symtable) : unit =
      env := TBL{info=info, table=table, prev=ref(!env), isopen=false}

  fun openNew (info: info, table: symtable) : unit =
      env := TBL{info=info, table=table, prev=ref(!env), isopen=true}

  fun openRec () : unit = 
      env := REC(ref(!env))

  fun openStr () : unit =
      env := STR(ref(!env))

  fun popSTR(STR(ref e)) = e
    | popSTR(TBL{prev=ref e,...}) = popSTR e
    | popSTR(REC(ref e)) = popSTR e
    | popSTR NIL = impossible "popSTR"

  fun closeStr () = env := popSTR(!env)

  fun openScope () : env = 
      let val oldenv = !env
       in env := TBL{info=defaultInfo,table=newTable(),prev=ref oldenv,isopen=true};
	  oldenv
      end

  fun resetEnv (e) = env := e

  fun add (binder) =
      case !env
	of TBL{table,isopen=true,...} => IntStrMap.add table binder
	 | e =>
	     let val table = newTable() : symtable
	      in IntStrMap.add table binder;
		 env := TBL{info=defaultInfo,table=table,prev=ref e,isopen=true}
	     end

  fun collectTable (collector) = 
      let fun save (REC(ref e)) = save e
	    | save (TBL{info,table,prev=ref e,...}) =
		(save e;
		 IntStrMap.app (fn (binder) => collector(binder,info)) table)
            | save (STR(ref e)) = env := e
	    | save _ = impossible "Env.collectTable.save"
       in save(!env)
      end

  fun splice (local':env, in':env) = 
      (* remove bindings between env in' and env local' *)
      prevEnv(in') := local'

  fun lookEnv (e:env, (key: key)) =
      let fun look1 (TBL{info,table,prev=ref e,...}) =
	        ((IntStrMap.map table key, info)
		 handle UnboundTable => look1 e)
	    | look1 (REC(ref e)) = look1 e
	    | look1 (STR(ref e)) = look1 e
	    | look1 NIL = 
		(ErrorMsg.flaggedmsg debugLook
		   ("lookEnv failed (global): "^name(key)^"\n");
		 raise Unbound)
       in look1(e)
      end

  fun look k = lookEnv (!env, k)

  fun lookStrLocal (key: key) = 
      let fun look1 (TBL{info,table,prev=ref e,...}) =
		     ((IntStrMap.map table key, info)
		       handle UnboundTable => look1 e)
	    | look1 (REC(ref e)) = look1 e
	    | look1 (STR(ref e)) =
		(ErrorMsg.flaggedmsg debugLook
		   ("lookStrLocal failed (structure): "^name(key)^"\n");
		 raise Unbound)
	    | look1 NIL = 
		(ErrorMsg.flaggedmsg debugLook
		   ("lookStrLocal failed (global): "^name(key)^"\n");
		 raise Unbound)
       in look1(!env)
      end

  fun lookRec (key : key) =
      (* searches global env for index id
         marks result as LOCAL or GLOBAL depending on whether binding is
	   found within rec boundary or not
	 used for initial var/con lookup in expressions (namespace=0) *)
      let fun look1 (TBL{info,table,prev=ref e,...}) =
	        (LOCAL(IntStrMap.map table key, info)
		 handle UnboundTable => look1 e)
	    | look1 (REC(ref e)) =
	        (GLOBAL(lookEnv(e,key))
		 handle Unbound => raise Unboundrec)
	    | look1 (STR(ref e)) = look1 e
	    | look1 NIL = 
		(ErrorMsg.flaggedmsg debugLook
		   ("lookRec failed : "^name(key)^"\n");
		 raise Unbound)
       in look1(!env)
      end

  fun lookRecLocal (key: key) = 
      (* searches global env upto nearest rec boundary
	 used when attempting to patch variables in expressions *)
      let fun look1 (TBL{info,table,prev=ref e,...}) =
	        ((IntStrMap.map table key, info)
		 handle UnboundTable => look1 e)
	    | look1 (REC(ref e)) = raise Unboundrec
	    | look1 (STR(ref e)) = look1 e
	    | look1 NIL = raise Unbound
       in look1(!env)
      end

  (* environment management for toplevel loop *)
  val restoreEnv : env ref = ref NIL
  fun restore () = resetEnv(!restoreEnv)
  fun commit () = restoreEnv := openScope()
  fun previous () = !restoreEnv

  (* consolidating adjacent open tables *)
  fun consolidateEnv(e as TBL{table,prev=ref(pre),isopen=true,...}) =
        (case pre
	   of TBL{isopen=true,...} =>
		let val ans as (_, adder) = consolidateEnv pre
		 in IntStrMap.app adder table; ans
		end
	    | _ => (e,IntStrMap.add table))
    | consolidateEnv(e) = (e,(fn x => ()))

  fun consolidate () = 
      env := let val (x,_) = consolidateEnv(!env) in x end

  fun foldEnv (f: env * 'a -> 'a) (newenv:env) (oldenv:env) (base:'a) : 'a =
      if eqEnv(newenv,oldenv)
      then base
      else f(newenv, foldEnv f (!(prevEnv newenv)) oldenv base)

  fun collectEnv(newenv, oldenv) = 
      foldEnv (fn (TBL{table,...},acc) =>
		    (IntStrMap.app (IntStrMap.add acc) table; acc)
		| (REC _,acc) => acc
		| (STR _,acc) => acc
		| (NIL,acc)   => acc
	      ) newenv oldenv (newTable())

  fun reset() = (env := NIL; restoreEnv := NIL)

  fun popModule(oldenv:env): symtable =
      (* Extract prefix of environment above oldenv (corresponding
	 to module bindings).  oldenv will normally be pervasiveEnv.
	 Then reset env to savedEnv. *)
      collectEnv(!env,oldenv)
            
end (* EnvFunc *)
