(* Copyright 1989 by AT&T Bell Laboratories *)
(* sparcglue.sml
 *
 * J.H. Reppy (3/15/89)
 * Cornell University
 * Ithaca, NY  14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   03/15/89  created
 *   11/20/89  changed to use new code generator
 *)

structure SparcMC : CODEGENERATOR = 
struct
    structure SparcC = Coder(structure M = SparcInstr and E = SparcMCEmit)
    structure MachGen = CPScomp(SparcCM(structure C = SparcC))
    fun generate lexp = (
	  MachGen.compile lexp;
	  SparcC.finish();
	  SparcMCode.getCodeString())
end

structure SparcAC : ASSEMBLER =
struct
    structure SparcC = Coder(structure M = SparcInstr and E = SparcAsEmit)
    structure AssemGen = CPScomp(SparcCM(structure C = SparcC))
    fun generate (lexp, stream) = (
	  SparcAsCode.outfile := stream;
	  AssemGen.compile lexp;
	  SparcC.finish())
end

structure IntSparc = IntShare(structure Machm = SparcMC
		  val fileExtension = ".spa"
		 )
structure CompSparc = Batch(structure M=SparcMC and A=SparcAC)
