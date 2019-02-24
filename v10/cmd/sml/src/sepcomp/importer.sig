(* Copyright 1989 by AT&T Bell Laboratories *)
(* importer.sig: this is all the Interact functor needs from the import
   mechanism. *)

signature IMPORTER =
   sig
      exception Import of string
			(* Raised for any sensible failure to import. *)

      type lvar sharing type lvar = Access.lvar

     (* We essentially have mutual recursion between Interact and Importer -
        Interact needs to be able to import things, and Importer needs to
	be able to implement a top-level loop. The ideal solution is to put
	the functions bind, lookup, etc. into a common sub-module. The slightly
	hacked solution, here, is to model the recursion by passing part of
	Interact's environment (the functions) in a record to Importer. *)

      datatype ToplevelFns =
         TOPLEVEL_FNS of {bind: lvar * System.Unsafe.object -> unit,
			  lookup: lvar -> System.Unsafe.object,
			  parse: Lex.lexer -> BareAbsyn.dec,
			  getvars: BareAbsyn.dec -> lvar list,
			  opt: Lambda.lexp -> Lambda.lexp
			 }

      val getAndExecModule: string * Env.env * ToplevelFns -> unit
   end
