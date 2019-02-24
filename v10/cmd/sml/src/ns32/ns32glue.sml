(* Copyright 1989 by AT&T Bell Laboratories *)
structure NS32MC : CODEGENERATOR = 
struct
  structure CM = NS32CM(NS32MCode.Coder)
  structure G = CPScomp(CM)
  fun generate lexp =
	(G.compile lexp; NS32MCode.finish())
end

structure NS32AC : ASSEMBLER =
struct
  structure CM = NS32CM(NS32AsCode)
  structure AssemGen = CPScomp(CM)
  fun generate(lexp,stream) = (NS32Assem.outfile := stream;
			       AssemGen.compile lexp)
end

structure IntNS32 = IntShare(structure Machm = NS32MC
			    val fileExtension = ".n32"
			   );
structure CompNS32 = Batch(structure M=NS32MC and A=NS32AC)
