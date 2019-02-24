(* Copyright 1989 by AT&T Bell Laboratories *)
(* shareglue: a functor application common to vaxglue and m68glue, just to
   avoid duplication of this linking phase. (NICK) *)

functor IntShare(structure Machm: CODEGENERATOR
		 val fileExtension: string
		) =
   Interact(structure Machm = Machm
	    structure Importer =
	       Importer(structure FilePaths = UnixPaths()
			val fileExtension = fileExtension
			structure ModuleComp =
			   ModuleComp(structure Absyn = BareAbsyn
				      structure Lambda = Lambda
			              structure Opt = Opt
				      structure Machm = Machm)))
