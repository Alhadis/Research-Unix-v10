(* Copyright 1989 by AT&T Bell Laboratories *)
signature M68CODER = sig

eqtype Label

val newlabel : unit -> Label

datatype Register = DataReg of int
		  | AddrReg of int
		  | FloatReg of int
		  | PC

datatype Size = Byte | Word | Long

datatype EA = Direct of Register
	    | PostInc of Register
	    | PreDec of Register
	    | Displace of Register * int
	    | Index of Register * int * Register * Size
	    | Immedlab of Label
	    | Immed of int
	    | Abs of int
	    | Address of Label

val d0 : Register
val d1 : Register
val d2 : Register
val d3 : Register
val d4 : Register
val d5 : Register
val d6 : Register
val d7 : Register
val a0 : Register
val a1 : Register
val a2 : Register
val a3 : Register
val a4 : Register
val a5 : Register
val a6 : Register
val sp : Register
val fp0 : Register
val fp1 : Register
val fp2 : Register
val fp3 : Register
val fp4 : Register
val fp5 : Register
val fp6 : Register
val fp7 : Register

val align : unit -> unit
val mark : unit -> unit
val define : Label -> unit
val emitstring : string -> unit
val realconst : string -> unit
val emitlong : int -> unit

val emitlab : int * Label -> unit

val rts : unit -> unit
val exg : (EA * EA) -> unit
val movl : (EA * EA) -> unit
val orl : (EA * EA) -> unit
val andl : (EA * EA) -> unit
val eorl : (EA * EA) -> unit
val addl : (EA * EA) -> unit
val subl : (EA * EA) -> unit
val cmpl : (EA * EA) -> unit
val btst : (EA * EA) -> unit
val pea : EA -> unit
val lea : (EA * EA) -> unit
val jne : EA -> unit
val jeq : EA -> unit
val jra : EA -> unit
val jbsr : EA -> unit

val jgt : EA -> unit
val jge : EA -> unit
val jlt : EA -> unit
val jle : EA -> unit
val asrl : EA * EA -> unit
val asll : EA * EA -> unit
val divl : EA * EA -> unit
val mull : EA * EA -> unit
val movb : EA * EA -> unit

val fjne : EA -> unit
val fjeq : EA -> unit
val fjgt : EA -> unit
val fjge : EA -> unit
val fjlt : EA -> unit
val fjle : EA -> unit

val fcmpd : EA * EA -> unit
val faddd : EA * EA -> unit
val fsubd : EA * EA -> unit
val fmuld : EA * EA -> unit
val fdivd : EA * EA -> unit
val fnegd : EA * EA -> unit
val fmoved : EA * EA -> unit

val trapv : unit -> unit
val trapmi : unit -> unit

val exg : EA * EA -> unit

val push : EA -> unit
val pusha : EA -> unit
val pop : EA -> unit

val comment : string -> unit

end (* signature CODER *)

signature M68MCODER = 
sig 
    structure Coder : M68CODER
    val finish : unit -> string
end
