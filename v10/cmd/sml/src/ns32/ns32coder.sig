(* Copyright 1989 by AT&T Bell Laboratories *)
signature NS32CODER = sig

eqtype Label

val newlabel : unit -> Label

datatype Register = Genreg of int
		  | FloatReg of int
		  | FP
		  | SP
		  | SB
		  | PC

datatype Size = Byte | Word | Long

datatype EA = Direct of Register
	    | Topofstack
	    | Displace of int * Register
	    | Immed of int
	    | Immedlab of Label
	    | Abs of int
	    | OffAddress of Label * int
	    | Index of EA * Register * Size

val r0 : Register
val r1 : Register
val r2 : Register
val r3 : Register
val r4 : Register
val r5 : Register
val r6 : Register
val r7 : Register
val fp : Register
val sp : Register
val sb : Register
val pc : Register
val fp0 : Register

val align : unit -> unit
val mark : unit -> unit
val define : Label -> unit
val emitstring : string -> unit
val realconst : string -> unit
val emitlong : int -> unit

val emitlab : int * Label -> unit

val lprl : EA * EA -> unit
val sprl : EA * EA -> unit
val tbit : EA * EA -> unit
val bfs : EA -> unit
val movl : EA * EA -> unit
val movql : EA * EA -> unit
val movb : EA * EA -> unit
val movzbl : EA * EA -> unit
val lea : EA * EA -> unit
val addl : EA * EA -> unit
val subl : EA * EA -> unit
val negl : EA * EA -> unit
val ashl : EA * EA -> unit
val andl : EA * EA -> unit
val orl : EA * EA -> unit
val xorl : EA * EA -> unit
val coml : EA * EA -> unit
val mull : EA * EA -> unit
val divl : EA * EA -> unit
val jump : EA -> unit
val beq : EA -> unit
val bne : EA -> unit
val bge : EA -> unit
val bgt : EA -> unit
val blt : EA -> unit
val ble : EA -> unit
val cmpl : EA * EA -> unit

val movg : EA * EA -> unit
val negg : EA * EA -> unit
val addg : EA * EA -> unit
val subg : EA * EA -> unit
val mulg : EA * EA -> unit
val divg : EA * EA -> unit
val cmpg : EA * EA -> unit

val comment : string -> unit

end (* signature NS32CODER *)

signature NS32MCODER = 
sig 
    structure Coder : NS32CODER
    val finish : unit -> string
end
