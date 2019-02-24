(* Copyright 1989 by AT&T Bell Laboratories *)
signature VAXCODER = sig

eqtype Label

val newlabel : unit -> Label

datatype Register = reg of int

datatype EA = direct of Register
	    | autoinc of Register
	    | autodec of Register
	    | displace of int * Register
	    | deferred of int * Register
	    | immed of int
	    | immedlab of Label
	    | address of Label
	    | index of EA * Register

val r0 : Register
val r1 : Register
val r2 : Register
val r3 : Register
val r4 : Register
val r5 : Register
val r6 : Register
val r7 : Register
val r8 : Register
val r9 : Register
val r10 : Register
val r11 : Register
val r12 : Register
val r13 : Register
val sp : Register
val pc : Register

val align : unit -> unit
val mark : unit -> unit
val define : Label -> unit
val emitstring : string -> unit
val realconst : string -> unit
val emitlong : int -> unit

val emitlab : int * Label -> unit

val jne : EA -> unit
val jbr : EA -> unit
val bbc : EA * EA * EA -> unit
val bbs : EA * EA * EA -> unit
val movl : EA * EA -> unit
val movq : EA * EA -> unit
val movb : EA * EA -> unit
val movzbl : EA * EA -> unit
val moval : EA * EA -> unit
val pushal : EA -> unit
val rsb : unit -> unit
val cmpl : EA * EA -> unit
val addl2 : EA * EA -> unit
val addl3 : EA * EA * EA -> unit
val subl2 : EA * EA -> unit
val subl3 : EA * EA * EA -> unit
val bisl3 : EA * EA * EA -> unit
val bicl3 : EA * EA * EA -> unit
val ashl : EA * EA * EA -> unit
val xorl3 : EA * EA * EA -> unit
val mull2 : EA * EA -> unit
val divl3 : EA * EA * EA -> unit
val divl2 : EA * EA -> unit
val jmp : EA -> unit
val brb : EA -> unit
val brw : EA -> unit
val beql : EA -> unit
val bneq : EA -> unit
val bgeq : EA -> unit
val bgtr : EA -> unit
val blss : EA -> unit
val bleq : EA -> unit
val sobgeq : EA * EA -> unit

val movg : EA * EA -> unit
val mnegg : EA * EA -> unit
val addg3 : EA * EA * EA -> unit
val subg3 : EA * EA * EA -> unit
val mulg3 : EA * EA * EA -> unit
val divg3 : EA * EA * EA -> unit
val cmpg : EA * EA -> unit

val push : EA -> unit
val pusha : EA -> unit
val pop : EA -> unit

val comment : string -> unit

end (* signature CODER *)

signature VAXMCODER = 
sig 
    structure Coder : VAXCODER
    val finish : unit -> string
end
