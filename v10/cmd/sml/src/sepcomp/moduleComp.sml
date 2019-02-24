(* Copyright 1989 by AT&T Bell Laboratories *)
(* moduleComp.sml *)
(* Modules compiler for New Jersey ML.   Nick Rothwell, LFCS, January 1989. *)
(* New version, doing proper lambda nesting for declarations and imports,
   rather than the old clatty concatenation stuff. *)

functor ModuleComp(structure Absyn: BAREABSYN
		     sharing Absyn = BareAbsyn
		   structure Lambda: LAMBDA
	           structure Opt: OPT sharing Opt.L = Lambda
		   structure Machm: CODEGENERATOR
		  ): MODULE_COMPILER =
  struct
    structure Lambda = Lambda
    structure Absyn = Absyn

    val bug = ErrorMsg.impossible

     (* Warning:  if DEBUG is true, then exportFn tends to grab the whole
        compiler.   --Appel *)
    val DEBUG = false

    type lvar = Access.lvar
    type lexp = Lambda.lexp
    type code = string

    fun assert(msg, cond) =
      if cond then () else bug("assert: " ^ msg)

    fun pr s = (output std_out s; flush_out std_out)

    fun debug s = if DEBUG then pr s else ()


    abstype LambDynModule =
	    LAMBDYNMODULE of {entry: Entry, lvars: lvar list} list
			(* These are in reverse order of declaration. *)
	and CodeDynModule =
	    CODEDYNMODULE of {self: code, subModules: CodeDynModule list}
			(* The code is for a function:
			   "(M1, ..., Mn) -> looker -> array*rubbish"
			   where each Mi is a "looker -> array*rubbish" *)

	and Entry = LAMBDAentry of lexp->lexp		(* unclosed *)
		  | IMPORTentry of CodeDynModule
    with
      val emptyDynModule = LAMBDYNMODULE []

      exception AddDeclaration
      fun addDeclaration(dec, lvars, LAMBDYNMODULE entries) =
        let
	  val newEntry =
	    {entry=LAMBDAentry(Translate.transDec dec), lvars=rev lvars}
        in
	  if !ErrorMsg.anyErrors then
	    raise AddDeclaration
	  else
	    LAMBDYNMODULE(newEntry :: entries)
	end

      fun importDynModule(LAMBDYNMODULE inner, LAMBDYNMODULE outer) =
        LAMBDYNMODULE(inner @ outer)

      fun abstractDynModule(codeDynModule, lvars) =
        LAMBDYNMODULE [{entry=IMPORTentry codeDynModule, lvars=lvars}]

     (* compiler: takes a LambDynModule to a CodeDynModule. We tie together
        all the lambdas as a set of nested LET declarations. Where we import
	already-compiled modules, these are abstracted to be arguments to the
	final lambda expression. *)

      fun inventLvar name =
        let
	  val saving = !System.Control.saveLvarNames
	  val _ = (System.Control.saveLvarNames := true)
	  val lvar = Access.namedLvar(Symbol.symbol(name ^ ":"))
	  val _ = (System.Control.saveLvarNames := saving)
	in
	  lvar
	end

     (* Some local lambda-building stuff. This should probably go in a wee
        structure of its own. *)

      local
        open Lambda
      in
        fun apply(lvar1, lvar2) = APP(VAR lvar1, VAR lvar2)

	fun let_1(lvar, exp1, exp2) = APP(FN(lvar, exp2), exp1)

        fun fn_N(name, lvars, exp) =
	  let
	    val vecLvar = inventLvar name
	    val vecExp = VAR vecLvar
	    fun doit(lv :: rest, n) =
	          let_1(lv, SELECT(n, vecExp), doit(rest, n+1))
	      | doit(nil, _) = exp
	  in
	    FN(vecLvar, doit(lvars, 0))
	  end

        fun let_N(name, lvars, exp1, exp2) =
	  APP(fn_N(name, lvars, exp2), exp1)

	fun record lvars = RECORD(map VAR lvars)
      end

     (* foldLambda: takes a list of entries (assumed to be in same order
	as the actual module declarations), and generates one big lambda.
	We close it later. foldLambda returns
	{lambda, imports, importLvars}. *)

      fun foldLambda({entry=LAMBDAentry thisFn, lvars} :: rest,
		     looker, allVars
		    ) =
	    let
	      val {lambda=next, imports, importLvars} =
		foldLambda(rest, looker, lvars @ allVars)
	    in
	      {lambda=thisFn next,	(* apply this fn->lambda to the
					   argument (that's how transDec
					   works...) *)
	       imports=imports,
	       importLvars=importLvars
	      }
	    end

	| foldLambda({entry=IMPORTentry subMod, lvars} :: rest,
		     looker, allVars
		    ) =
	    let
	      val modLvar =
		inventLvar("importModule/" ^ makestring(length rest))
	      val {lambda=next, imports, importLvars} =
		foldLambda(rest, looker, lvars @ allVars)
	    in
	      {lambda=let_N("importEntry", lvars, apply(modLvar,looker), next),
	       imports=subMod :: imports,
	       importLvars = modLvar :: importLvars
	      }
	    end

	| foldLambda(nil, _, allVars) =
	    {lambda=record(rev allVars), imports=nil, importLvars=nil}
					(* Make sure the lvars comprising
					   the eventual record are the
					   right way round!! *)

     (* codegen compiles a lambda to a machine-code string. *)

      exception Codegen
      fun codegen lambda: string =
	  let
	      val _ = debug "codegen...";
	      val executable = Machm.generate lambda
	      val _ = debug("done (" ^ makestring(size executable) ^ ").\n");
	  in
	      if !ErrorMsg.anyErrors then raise Codegen else executable
	  end

      fun spaces n =
        let
          fun spaces'(0, sp) = debug sp
            | spaces'(n, sp) = spaces'(n-1, " " ^ sp)
	in
	  spaces'(n, "")
	end

      fun printCodeDynModule indent (CODEDYNMODULE{self, subModules}) =
        (spaces indent;
	 debug("CodeDynModule, self=[..." ^ makestring(size self) ^ "...]\n");
	 spaces indent;
	 debug "sub-modules=(\n";
	 app (printCodeDynModule(indent+3)) subModules;
	 spaces indent;
	 debug ")\n"
	)

      val printNums = app (fn i: int => debug(makestring i ^ " "))

      fun printLambEntry{entry=LAMBDAentry lexpFn, lvars} =
            (debug "*LAMBDA*\n";
	     MCprint.printLexp(lexpFn(record nil));
	     debug ";\n   lvars=[ ";
	     printNums lvars;
	     debug "].\n"
	    )
	| printLambEntry{entry=IMPORTentry dyn, lvars} =
	    (debug "*IMPORT*\n";
	     printCodeDynModule 6 dyn;
	     debug "   lvars=[ ";
	     printNums lvars;
	     debug "].\n"
	    )

      exception CompileDynModule
      fun compileDynModule opt (LAMBDYNMODULE entries): CodeDynModule =
	let
	  val _ =
	    if DEBUG then (debug "compileDynModule:\n";
			   app printLambEntry entries
			  )
	    else ()

	  val looker = inventLvar "looker"

	  val {lambda=openLambda, imports, importLvars} =
	    foldLambda(rev entries, looker, nil)
		(* reverse "entries" because a LambDynModule has the most
		   recent declaration at the front, and we want to nest the
		   lambda with the old stuff outermost - as well as get the
		   correct ordering of the lvars in the final result. *)

	  val _ = if !ErrorMsg.anyErrors then raise CompileDynModule else ()
		(* Just check for any errors during last part of transDec. *)

	  val closedLambda =
	    Opt.bareCloseTop{lambda=openLambda,
	    		     looker=looker,
			     extras=ProcessFile.getCore(),
			     keepFree=looker :: importLvars
			    }
		(* Close the lambda, using the already-generated looker lvar -
		   this had to be generated in advance, since each imported
		   module is activated as M(looker). We DON'T want to close
		   with respect to the modLvars, since we're just about to
		   lambda-bind these. Keep the looker's lvar free as well,
		   otherwise it tries to find itself... Gnnnh! *)

	  val finalLambda =
	    fn_N("imports", importLvars, closedLambda)
		(* That's what we want: "fn (M1, ..., Mn) => fn looker => ..."
		   where the Mi's we apply to are each a "fn looker => ...".
		   In fact, I suspect that "imports" and "importLvars" are
		   both in reverse order, but as long as they're
		   coherent... *)

          val _ =
	    if DEBUG then (debug "Final (unopt) lambda:\n";
			   MCprint.printLexp finalLambda;
			   debug ".\n"
			  )
	    else ()

	  val optLambda = opt finalLambda

	  val code =
	    codegen optLambda
	    handle Codegen => raise CompileDynModule
	in
	  CODEDYNMODULE{self=code, subModules=imports}
	end

     (* primeDynModule: returns a "fn lookup => ...result...". If the module
        doesn't import anything, then we just apply the code to NIL. If the
	module imports M1...Mn, then we prime these, and build an array of
	them. *)

      type object = System.Unsafe.object
      type looker = lvar -> object
      type result = object array
      type module = looker -> result

      fun primeDynModule(CODEDYNMODULE{self, subModules}): module =
        let
	  val me: module array -> looker -> result =
	    System.Unsafe.boot self

	  val subModules: module array =
	    arrayoflist(map primeDynModule subModules)
	in
	  me subModules
	end

      fun debugLooker looker =
        fn lvar: int => (debug("[looker(" ^ makestring lvar ^ ")]\n");
			 looker lvar
			)

      fun executeDynModule codeDynModule looker: object array =
	let
	  val _ = debug "priming... "
	  val primed = primeDynModule codeDynModule
	  val _ = debug "executing... "
	  val result = primed(if DEBUG then debugLooker looker else looker)
	  in
	    debug "done\n";
	    result
	  end
    end
  end;
