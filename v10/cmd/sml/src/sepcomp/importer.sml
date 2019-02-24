(* Copyright 1989 by AT&T Bell Laboratories *)
(* importer.sml    608567950   46    20    100444  17742     `*)
(* Importer: Isolation of Mads' original code from Interact() into a separate
   functor. Numerous extensions, Make system, etc. etc. (NICK) *)

functor Importer(structure FilePaths: FILEPATHS
		 val fileExtension: string
		 structure ModuleComp: MODULE_COMPILER
		    sharing ModuleComp.Lambda = Lambda
		        and ModuleComp.Absyn = BareAbsyn
		        and type ModuleComp.lvar = Access.lvar)
		: IMPORTER =
struct

  open PrintUtil Access Basics Stampset Env

  val gcmessages = System.Control.Runtime.gcmessages 
       (* The message "Major collection... abandoned" is
	  annoying me, so I'm hosing it. NICK *)

  fun all pred list =
      fold (fn (this, res) => pred this andalso res) list true

  val DEBUG = false
  val debug = 
      if DEBUG then fn str => output std_out ("<" ^ str ^ ">\n")
      else fn _ => ()

  val TRACE_IO = false
  val open_in =
      if TRACE_IO
      then fn name => (debug("open_in \\" ^ name ^ "\\"); open_in name)
      else open_in

  val open_out =
      if TRACE_IO
      then (fn name => (debug("open_out \\" ^ name ^ "\\"); open_out name))
      else open_out

  exception Import of string
     (* A single exception for any failure to
	import (barring compiler bugs). compSource
	requires a protective coating so that it
	doesn't leave the global static environment
	in a funny state. *)


  (* Feedback messages. If anybody's interested, files which may
     cause failures, or may cause nested reads, are done as:

       [reading fred.sml]
       [closing fred.sml]

     Ones which shouldn't (eg. reading from a binary) produce:

       [reading fred.bin... done]
   *)

  fun reading(file, indent) =
      (tab indent; print("[reading " ^ file ^ "]\n"))
  fun reading1(file, indent) =
      (tab indent; print("[reading " ^ file ^ "... "); flush_out std_out)
  fun writing(file, indent) =
      (tab indent; print("[writing " ^ file ^ "]\n"))
  fun writing1(file, indent) =
      (tab indent; print("[writing " ^ file ^ "... "); flush_out std_out)
  fun closing(file, indent) =
      (tab indent; print("[closing " ^ file ^ "]\n"))
  fun done() = print "done]\n"

  fun fail(msg, verdict) =
     (print("import: " ^ msg ^ "\n"); raise Import verdict)

 (* impliedPath: derived from FilePaths.impliedPath, but catches
    ImpliedPath if a "~"-filename fails to translate. *)

  fun impliedPath(oldPath, oldName) =
      FilePaths.impliedPath(oldPath, oldName)
      handle FilePaths.ImpliedPath =>
	fail("couldn't translate path in: " ^ oldName, "open")

  type LambDynModule = ModuleComp.LambDynModule
  type CodeDynModule = ModuleComp.CodeDynModule
  type lvar = Access.lvar

  datatype ToplevelFns =
     TOPLEVEL_FNS of
       {bind: lvar * System.Unsafe.object -> unit,
	lookup: lvar -> System.Unsafe.object,
	parse: Lex.lexer -> BareAbsyn.dec,
	getvars: BareAbsyn.dec -> lvar list,
	opt: Lambda.lexp -> Lambda.lexp}

  datatype statModule =
      STATmodule of {table: symtable, lvars: Basics.Access.lvar list}


    (* Rename the lvars, and shift the stamps, of the static module.
       Only signature and functor bindings are accepted.
       For each functor binding, a fresh lvar will be chosen; hence
       at run-time, several imports of the same functor will presumably
       lead to a new copy of the code of that functor *)

  fun importModule(STATmodule{table,lvars}) : lvar list = 
      let val newlvars = map (fn _ => mkLvar()) lvars
	  fun lookup x =
	      let fun f(a::ar, b::br) = if a=x then b else f(ar,br)
		    | f _ = ErrorMsg.impossible "importModule 1"
	       in f(lvars,newlvars)
	      end
	  fun renBinding(SIGbind(SIGvar{name,binding})) =
		SIGbind(SIGvar
			{name=name, 
			 binding=ModUtil.shiftSigStamps(newStampsets(),binding)})
	    | renBinding(FCTbind(FCTvar{name,access=LVAR lvar, binding})) =
	        FCTbind(FCTvar{name = name, access= LVAR(lookup lvar),
			 binding = ModUtil.shiftFctStamps binding})
            | renBinding _ = ErrorMsg.impossible "importModule 2"
       in IntStrMap.app (fn (i,s,b) => add(i,s,renBinding b)) table;
	  newlvars
      end

 (* New code (NICK) - I store the static information (StatModule) and
    dynamic information (CodeDynModule) in one object, so that I can blast
    out the entire thing as a single object into a file. Foo.sml now gets
    compiled into Foo.vax/Foo.m68/..., which contains
    both. The object stored in the file is a pair: the first element is a
    "version number" for the data structures, the second is whatever needs
    storing (currently a record of {statModule, dynModule, imports}).
    If this version number changes, I have to recompile. *)

  type BinFormat = {statModule: statModule,
		    dynModule: CodeDynModule,
		    imports: string list}

  val blastRead: instream -> BinFormat = System.Unsafe.blast_read
  val blastWrite: (outstream * BinFormat) -> unit = System.Unsafe.blast_write

  val blastWrite =    (* Silent version. *)
      fn (stream, obj) =>
	 let val oldmsgs = !gcmessages
	  in gcmessages := 0;
	     blastWrite(stream, obj);
	     gcmessages := oldmsgs
	 end

  val BIN_VERSION = System.version ^ " - LAMBDA v0 " ^ fileExtension^ "\n"
       (* This is stored as the first line of the
	  binary file. Be sure to increment it whenever the structure
	  of any of the stored data objects changes. It cannot
	  contain any \n characters, except at the end where
	  one is required.  *)

  fun tryOpenIn filename: instream option =
      SOME(open_in filename) handle Io _ => NONE

  fun createBinary(indent, filename,
		   statModule: statModule,
		   dynModule: CodeDynModule,
		   imports: string list) : unit =
      let val fullName = filename ^ ".bin"
	  val outstream =
	       open_out fullName
	       handle Io _ => fail("couldn't open " ^ fullName ^ " for output",
				   "open")
       in writing1(fullName, indent);
	  output outstream BIN_VERSION;
	  blastWrite(outstream,
	  	     {statModule=statModule,
		      dynModule=dynModule,
		      imports=imports});
	  close_out outstream;
	  done()
      end

  val statPrinter: statModule -> string = (fn _ => "")
   (* this definition causes createTextual to have no effect *)

  fun createTextual(indent, filename, statModule): unit =
      case statPrinter statModule  (* currently always "" *)
	of "" => ()   (* Do NOTHING if the print function is a dummy *)
	 | text =>
	   let val fullName = filename ^ ".lstat"
	       val outstream =
		    open_out fullName
		    handle Io _ =>
		      fail("couldn't open " ^ fullName ^ " for output", "open")
	    in writing1(fullName, indent);
	       output outstream text;
	       close_out outstream;
	       done()
	   end

 (* We must do a syntactic check that the source declarations in a module
    are just functor and signature declarations (or sequences thereof),
    otherwise the renaming routines will fall over later. Importer is the
    place to do it, where we still have a fighting chance of a putting
    out a decent diagnostic message. We don't allow IMPORT - that should
    have been dealt with earlier. *)

  fun kosherModuleDecl dec =
      case dec
	of BareAbsyn.FCTdec _ => true
	 | BareAbsyn.SIGdec _ => true
	 | BareAbsyn.SEQdec decs =>	(* ALL must be kosher. *)
	     all kosherModuleDecl decs
	 | _ => false

  fun badModuleDecl() = ErrorMsg.condemn "expecting SIGNATURE/FUNCTOR/IMPORT"

 (* uptodate should be memo'd sometime, since it's quite expensive. *)
  fun uptodate (path, myBinTime) name =
      let val {newPath, validName} = impliedPath(path, name)
	  val _ = debug("uptodate(quotedName=" ^ name
			^ ", validName=" ^ name ^ ")?")
	  val trySml = tryOpenIn(validName ^ ".sml")
	  val tryBin = tryOpenIn(validName ^ ".bin")
       in case (trySml, tryBin)
	   of (SOME source, SOME binary) =>
		let val srcTime = mtime source
		    val binTime = mtime binary
		    val _ = debug("uptodate(" ^ validName ^ "):\
				  \ src time = " ^ makestring srcTime
				  ^ ", bin time = " ^ makestring binTime)
		 in if srcTime >= binTime	(* binary out of date *)
		       orelse binTime >= myBinTime
			       (* Some other branch of the Make
				  task compiled this under me...? *)
		    then (close_in source; close_in binary; false)
		    else   (* source is older; check imports *)
		      let val _ = close_in source
			  val fullName = validName ^ ".bin"
			  val binVersion = input_line binary
		       in if binVersion <> BIN_VERSION
			  then (close_in binary; false)
			   (* can't trust "imports" : chicken out *)
			  else let val {imports, ...} = 
				       blastRead binary before close_in binary
			        in all (uptodate (newPath, myBinTime)) imports
			       end
		      end
		 end

	    | (SOME source, NONE) =>	(* No bin: force recompile *)
		(close_in source; false)

	    | (NONE, SOME binary) =>	(* No source: trust for now... *)
		(close_in binary; true)

	    | (NONE, NONE) =>
		fail("cannot find source or binary\
		     \ of required module " ^ validName,
		     "open")
      end (* uptodate *)

fun getModule(name,pervasives,TOPLEVEL_FNS{bind,lookup,parse,getvars,opt})
      : statModule * CodeDynModule =
    let fun getModule'(parents, indent, path, name) =
	 (* "parents" is a depth-first list of filenames used for
	    a circularity check. "indent" is for cosmetic purposes. *)
	let val {validName as filename, newPath as path} = impliedPath(path, name)
	    val _ = if exists (fn x  => x = filename) parents
		    then fail("self-referential import of " ^ validName, "open")
		    else ()
	    val parents = filename :: parents
	    val _ = debug("getModule'(name=" ^ name ^ ")")

	    fun compSource0(source: instream) : statModule * CodeDynModule =
		let val lex = Lex.mkLex{stream=source,interactive=false}
		    fun loop(dynModule, lvars, imports)
			: LambDynModule * lvar list * string list =
			(case parse lex (*  (Lex.toplevel := true; parse()) *)
			  of BareAbsyn.IMPORTdec names => 
			      let fun loop'([], dynMod, lvars, imports) =
					(dynMod, lvars, imports)
				    | loop'(name::rest, dynMod, lvars, imports)=
					let val {newPath, ...} = impliedPath(path, name)
					    val (stat, codeDyn) =
						getModule'(parents, indent+2,
							  newPath, name)
					    val newLvars = importModule stat
					    val lambDyn = ModuleComp.abstractDynModule
							    (codeDyn, newLvars)
					    val dynMod' = ModuleComp.importDynModule
							    (lambDyn, dynMod)
					 in loop'(rest, dynMod', lvars @ newLvars,
						  name :: imports)
					end
			       in loop(loop'(names, dynModule, lvars, imports))
			      end

			   | absyn => (* normal program *)
			       if kosherModuleDecl absyn
			       then let val newLvars = getvars absyn
					val newMod = ModuleComp.addDeclaration
						       (absyn, newLvars, dynModule)
						     handle ModuleComp.AddDeclaration =>
						     fail("error during translate",
							  "translate")
				     in loop(newMod, lvars @ newLvars, imports)
				    end
			       else badModuleDecl())

			handle Parse.Eof => (dynModule, lvars, imports)
			     | Import x  => raise Import x
					 (* Resignal nested Import probs. *)
			     | Io x => raise Import("unexpected: Io(" ^ x ^ ")")
			     | exn => raise Import("compile-time exception: "
						   ^ System.exn_name exn)

		    val (lambDynModule, lvars, imports) =
			  loop(ModuleComp.emptyDynModule, [], [])
		    val statModule= STATmodule{table=Env.popModule(pervasives),
					       lvars=lvars}
		    val dynModule = ModuleComp.compileDynModule opt lambDynModule
				    handle ModuleComp.CompileDynModule =>
				      fail("code generation failed", "codegen")
		 in createBinary(indent, filename, statModule,
				 dynModule, imports)
		      handle Import _ => (); (* make failed writes nonfatal... *)
		    createTextual(indent, filename, statModule) (* no-op *)
		      handle Import _ => ();
		    (statModule, dynModule)
		end  (* fun compSource *)

	    fun compSource(source) =
		let val _ = debug(filename ^ ": source only")
		    val fullName = filename ^ ".sml"
		    val _ = reading(fullName, indent)

		    val oldfile = !ErrorMsg.fileName
		    val oldlinenum = !ErrorMsg.lineNum
		    val oldinteractive = !System.interactive
		    val savedEnv = Env.current()
		    fun cleanup () =
		       (closing(fullName, indent);
		        close_in source;
			ErrorMsg.fileName := oldfile;
			ErrorMsg.lineNum := oldlinenum;
			System.interactive := oldinteractive;
			Env.resetEnv savedEnv)
		 in ErrorMsg.fileName := fullName;
		    ErrorMsg.lineNum := 1;
		    System.interactive := false;
		    Env.resetEnv pervasives;
		    (compSource0(source) before cleanup())
		    handle exp => (cleanup(); raise exp)
		end

	 in case (tryOpenIn(filename ^ ".sml"), tryOpenIn(filename ^ ".bin"))
	      of (SOME source, NONE) =>  (* Source only: Compile! *)
		   compSource(source)
	       | (SOME source, SOME binary) =>
		   let val srcTime = mtime source
		       val binTime = mtime binary
		       val _ = debug(filename ^ ": src dated " ^ makestring srcTime
				     ^ ", bin dated " ^ makestring binTime)
		    in if srcTime >= binTime   (* (">=" for safety) *)
		       then (* binary out of date? *)
			 (tab indent;
			  print("[" ^ filename ^ ".bin is out of date;\
				\ recompiling]\n");
			  close_in binary;
			  compSource(source))
		       else (* bin is newer: what about the things imported? *)
			 let val _ = debug(filename ^ ": checking imports")
			     val fullName = filename ^ ".bin"
			     val _ = reading1(fullName, indent)
			     val binVersion = input_line binary
			  in if (binVersion <> BIN_VERSION)
			     then (print "]\n";
				   tab indent;
				   print("[" ^ fullName ^ " is the wrong format;\
				     \ recompiling]\n");
				   closing(fullName, indent);
				   close_in binary;
				   compSource(source))
			     else let val {statModule, dynModule, imports} =
					    blastRead binary
				      fun allOk imports =
					  all (uptodate (path, binTime)) imports
					  handle exn =>
					    (print "]\n";
					     closing(fullName, indent);
					     close_in binary;
					     close_in source;
					     raise exn)
				   in if not(allOk imports)
				      then (print "]\n";
					    tab indent;
					    print("[import(s) of " ^ filename
					      ^ " are out of date; recompiling]\n");
					    closing(fullName, indent);
					    close_in binary;
					    compSource(source))
				       else (* All OK: use the binary. *)
					    (debug(filename ^ ": all up to date");
					     close_in source;
					     close_in binary;
					     done();
					     (statModule, dynModule))
				  end
			 end
		   end

	       | (NONE, SOME binary) =>
		   let val _ = debug(filename ^ ": binary only")
		       val fullName = filename ^ ".bin"
		       val _ = reading1(fullName, indent)
		       val binVersion = input_line binary
		    in if binVersion = BIN_VERSION
		       then let val {statModule, dynModule, ...} = blastRead binary
			     in close_in binary;
				done();
				(statModule, dynModule)
			    end
		       else (print "]\n";	(* Outstanding message... *)
			     tab indent;
			     print("[" ^ fullName ^ " is the wrong format;\
				    \ recompiling]\n");
			     closing(fullName, indent);
			     close_in binary;
			     case tryOpenIn(filename ^ ".sml")
			       of SOME source =>
				    compSource(source)
				| NONE =>
				  fail(fullName ^ " is out of date, and can't\
				       \ open " ^ filename ^ ".sml",
				       "open"))
		   end

	       | (NONE, NONE) => fail("cannot open " ^ filename ^ ".sml", "open")
	end (* getModule' *)
     in getModule'([],0,FilePaths.defaultPath,name)
    end  (* getModule *)

  fun getAndExecModule(filename, pervasives,
		       toplevelFns as TOPLEVEL_FNS{bind, lookup, ...}) : unit = 
      let val (statModule as STATmodule{table, ...}, compDynModule) =
	        getModule(filename, pervasives, toplevelFns)
       in let val newlvars = importModule statModule 
	      (* adds the static bindings of the module to the
		 static environment*)

	      val result =
		  ModuleComp.executeDynModule compDynModule lookup
		  handle exn =>	(* Local handle for module execution (NICK). *)
		    fail("execution of module raised exception "
			 ^ System.exn_name exn
			 ^ "\n\t(static bindings of module take no effect)\n",
			 "uncaught exception")

	      fun bindlvars (i,v::r) =
		    (bind(v,result sub i);
		     bindlvars (i+1,r))
		| bindlvars (_,nil) = ()

	   in bindlvars(0,newlvars);	(* add new runtime bindings *)
	      Env.commit();			(* accept static bindings *)
	      PrintDec.printBindingTbl table
	  end
	  handle  (* Exceptions other than ones raised through module execution. *)
		Interrupt => raise Interrupt
	      | exn => ErrorMsg.impossible("addAndExecModule: exn ("
					   ^ System.exn_name exn ^ ")??")
      end

end (* functor Importer *)
