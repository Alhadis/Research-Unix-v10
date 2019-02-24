(* Copyright 1989 by AT&T Bell Laboratories *)
(* Modules compiler for New Jersey ML.   Nick Rothwell, LFCS, January 1989. *)

(* Generic signature to encompass the current slot-based representation, as well
   as the still-to-be-written lambda-based representation. *)

signature MODULE_COMPILER =
   sig
       type lvar sharing type lvar = int

       structure Absyn: BAREABSYN
         sharing Absyn = BareAbsyn
				(* Needed because Translate is hard-wired. *)

       structure Lambda: LAMBDA

       type LambDynModule	(* Dynamic module with lambdas, lvars, etc. *)
       type CodeDynModule	(* Compiled (native code) module - no lvars. *)

       val emptyDynModule: LambDynModule	(* Starting state. *)

       exception AddDeclaration
       val addDeclaration: Absyn.dec * lvar list * LambDynModule -> LambDynModule

       val abstractDynModule: CodeDynModule * lvar list -> LambDynModule
		(* abstractDynModule takes a compiled module (presumably from an
		   "import"), and returns a lambda-based module with the code
		   module treated as an argument. We attach the lvars to the code
		   module, to allow reference from declarations below the
		   "import". *)

       exception CompileDynModule
       val compileDynModule:
         (Lambda.lexp -> Lambda.lexp) -> LambDynModule -> CodeDynModule
		(* Close and fold down all the lambdas, generate code. Takes
		   a lambda-optimising function as argument. *)

       val executeDynModule: CodeDynModule ->
	   (lvar -> System.Unsafe.object) -> System.Unsafe.object array
		(* Execute the module, given the lookup function
		   (which I'll use to get at true globals like the
		   pervasives). *)

       val importDynModule: (LambDynModule * LambDynModule) -> LambDynModule
		(* importDynModule takes a module state, and embeds it
		   into an enclosing module state. *)
   end;
