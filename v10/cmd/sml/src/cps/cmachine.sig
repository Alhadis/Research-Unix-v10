(* Copyright 1989 by AT&T Bell Laboratories *)
signature CMACHINE = sig

type EA
(* the following kinds of effective address are assumed:
    register;
    immediate label:  
	This mode doesn't in fact exist on the Vax or the MC68020,
        but it can be simulated (e.g. by "move address" instructions).
    immediate integer literal;  *)

val immed : int -> EA		(* makes the immediate integer mode *)
val isimmed : EA -> int option
val isreg : EA -> int option
val eqreg : EA -> EA -> bool

(* DEDICATED REGISTERS *)
(* The following registers can hold pointers or properly tagged integers *)
val exnptr : EA	    (* the current exception-handler *)
val storeptr : EA   (* the head of the store-list of modified words *)
(* val dataptr : EA    the boundary between allocated and unused *)

(* The following registers may not hold pointers, and may hold untagged ints *)
val arithtemp : EA
val arithtemp2 : EA

(* The following registers are not dedicated, and must be all disjoint *)
val standardclosure : EA
val standardarg : EA
val standardcont : EA
val miscregs : EA list

val move : EA * EA -> unit  (* move(a,b)    a -> b *)

val align : unit -> unit  (* ensure that next code is on 4-byte boundary *)
val mark: unit -> unit    (* insert a gc-tag in the code so that next address
			     may be moved into a record *)
val emitlong : int -> unit (* put an 4-byte integer literal into the code *)
val realconst : string -> unit  (* put a floating literal into the code *)
val emitstring : string -> unit (* put a literal string into the code
				   (just the chars, no descriptor or length) *)
val emitlab : int * EA -> unit  (* L3: emitlab(k,L2) is equivalent to
				   L3: emitlong(k+L2-L3) *)

val newlabel : unit -> EA	(* create a new label (but don't define it) *)
val define : EA -> unit  (* Associate a label with a point in the code *)

(* checkLimit (n):
 * Generate code to check the heap limit to see if there is enough free space
 * to allocate n bytes.
 *)
val checkLimit : int -> unit

(* beginStdFn (cl, lab):
 * Note the beginning of a standard function with entry label lab, and
 * register cl containing its closure.
 *)
val beginStdFn : (EA * EA) -> unit (* Mark beginning of standard function *)

val jmp : EA -> unit	  (* unconditional jump to the address specified *)

val record : (EA * CPS.accesspath) list * EA -> unit
		 (* makes a new record, puts address of it
		    into the destination specified by the second arg.
		    The contents are numbered from ~1 and up. *)

val select : int * EA * EA -> unit  (* select(i,x,y) = y <- mem[x+4*i] *)
val offset : int * EA * EA -> unit  (* offset(i,x,y) = y <- x+4*i *)

val fetchindexb : EA * EA -> unit   (* fetchindexb(x,y) fetches a byte:
				        y <- mem[x+arithtemp]
					y cannot be arithtemp *)
val storeindexb : EA * EA -> unit   (* storeindexb(x,y) stores a byte:
					mem[y+arithtemp] <- x; *)
val jmpindexb : EA -> unit	    (* jmpindexb(x)    pc <- (x+arithtemp) *)
val fetchindexl : EA * EA * EA -> unit   (* fetchindexl(x,y,z) fetches a word:
					   y <- mem[x+2*(z-1)] *)
val storeindexl : EA * EA * EA -> unit   (* storeindexl(x,y,z) stores a word:
	    					mem[y+2*(z-1)] <- x *)

val ashl : EA * EA * EA -> unit  (* shift left: count, src, dest;
				     shift count is non-negative *)
val ashr : EA * EA * EA -> unit  (* shift right: count, src, dest;
				     shift count is non-negative *)
	   
val orb :  EA * EA * EA -> unit  (* bitwise or *)
val andb :  EA * EA * EA -> unit  (* bitwise and *)
val xorb :  EA * EA * EA -> unit  (* bitwise xor *)
val notb :  EA * EA -> unit  (* bitwise complement *)

(* the following instructions are really just vax instructions *)
val addl3 : EA * EA * EA -> unit
val subl3 : EA * EA * EA -> unit	(* subl3(a,b,c):  c <- (b - a) *)

val divl2 : EA * EA -> unit		(* divl2(a,b):  b <- (b div a) *)

(* integer arithmetic with overflow trapping *)
val addl3t : EA * EA * EA -> unit
val subl3t : EA * EA * EA -> unit	(* subl3t(a,b,c):  c <- (b - a) *)
val mull2t : EA * EA -> unit

val bbs	  : EA * EA * EA -> unit

datatype condition = NEQ | EQL | LEQ | GEQ | LSS | GTR

(* ibranch (cond, a, b, lab):  pc <- lab if (a <cond> b). *)
val ibranch : condition * EA * EA * EA -> unit

(* these are almost vax instructions, except that in each case
   the EA's are the addresses of the operands, not the values *)
val mnegg  : EA * EA -> unit
val mulg3 : EA * EA * EA -> unit
val divg3 : EA * EA * EA -> unit
val addg3 : EA * EA * EA -> unit
val subg3 : EA * EA * EA -> unit
val gbranch : condition * EA * EA * EA -> unit

val profile : int * int -> unit
val comment : string -> unit
end
