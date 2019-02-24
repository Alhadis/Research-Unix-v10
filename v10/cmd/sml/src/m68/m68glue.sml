(* Copyright 1989 by AT&T Bell Laboratories *)
structure M68MC : CODEGENERATOR = 
struct
  structure CM = M68CM(M68MCode.Coder)
  structure G = CPScomp(CM)
  fun generate lexp = (G.compile lexp; M68MCode.finish())
end

structure M68AC : ASSEMBLER =
struct
  structure CM = M68CM(M68AsCode)
  structure AssemGen = CPScomp(CM)
  fun generate(lexp,stream) = (M68Assem.outfile := stream;
			       AssemGen.compile lexp)
end

structure IntM68 = IntShare(structure Machm = M68MC
			    val fileExtension = ".m68"
			   );
structure CompM68 = Batch(structure M=M68MC and A=M68AC)
