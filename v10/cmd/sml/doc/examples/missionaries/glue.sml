(* glue.sml *)

structure SolveDF = Search(structure States = MandC and Buffer = DF)

structure SolveBF = Search(structure States = MandC and Buffer = BF)

fun go_df () = SolveDF.search MandC.initial

fun go_bf () = SolveBF.search MandC.initial
