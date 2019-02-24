(* Copyright 1989 by AT&T Bell Laboratories *)
functor NS32CM(V : NS32CODER) : CMACHINE = struct

structure V' : sig datatype Register = Genreg of int
				     | FloatReg of int
				     | FP
				     | SP
				     | SB
				     | PC

		   eqtype Label sharing type Label = V.Label
		   datatype Size = Byte | Word | Long
			
		   datatype EA = Direct of Register
			       | Topofstack
			       | Displace of int * Register
			       | Immed of int
			       | Immedlab of Label
			       | Abs of int
			       | OffAddress of Label * int
			       | Index of EA * Register * Size

		end = V
open V' System.Tags

datatype condition = NEQ | EQL | LEQ | GEQ | LSS | GTR

fun address lab = OffAddress(lab,0)	(* old Address style *)

fun defer(Direct r) = Displace(0,r)
  | defer(Immedlab lab) = address lab
  | defer(Displace z) = ErrorMsg.impossible "defer of displace in cpsns32"
  | defer _ = ErrorMsg.impossible "defer in cpsns32"

val sp' = SP
val exnptr = Displace(0, SB)
val dataptr as Direct dataptr' = Direct(FP)
val arithtemp as Direct arithtemp' = Direct(Genreg 6)
val arithtemp2 = Direct(Genreg 7)
val storeptr = Displace(4, SB)
val standardclosure = Direct(Genreg 2)
val standardarg = Direct(Genreg 0)
val standardcont = Direct(Genreg 1)
val miscregs : EA list = map (Direct o Genreg) [3,4]
val ptrtemp = Direct(Genreg 5)

fun newlabel() = Immedlab(V.newlabel())
fun emitlab(i,Immedlab lab) = V.emitlab(i,lab)
fun define (Immedlab lab) = V.define lab
fun beginStdFn _ = ()

val align = V.align
val mark = V.mark

fun move(Immedlab l, dest as Direct(Genreg x)) = V.lea(address l, dest)
  | move(Immedlab l, dest) = (* This via ptrtemp probably not needed BUGBUG *)
	    (V.lea(address l, ptrtemp);
	     move(ptrtemp, dest))
(* Only runtime knows about SB and MOD registers.
   We use SB as a base for our pseudo-registers.
  | move(src as Direct(SB), dest) = V.sprl(src, dest)
  | move(src, dest as Direct(SB)) = V.lprl(src, dest)
 *)
  | move(src as Direct(FP), dest) = V.sprl(src, dest)
  | move(src, dest as Direct(FP)) = V.lprl(src, dest)
  | move x = V.movl x

(* checkLimit (n):
 * Generate code to check the heap limit to see if there is enough free space
 * to allocate n bytes.
 *)
fun checkLimit maxAllocSize = (
      V.comment ("begin fun, max alloc = "^(makestring maxAllocSize)^"\n");
    (* Check the heap limit by writing to the dataptr+maxAllocSize-4 *)
      move(Immed 0, Displace(maxAllocSize-4, dataptr')))

val emitlong = V.emitlong
val realconst = V.realconst
val emitstring = V.emitstring

fun jmpindexb lab = V.jump(Index(defer lab, arithtemp', Byte))

fun record(vl, z) =
    let open CPS
	val len = List.length vl
	fun f(i,nil) = ()
	  | f(i,(Direct r, SELp(j,p))::rest) = f(i,(Displace(j*4,r),p)::rest)
	  | f(i,(Immedlab l, p)::rest) = (V.lea(address l, ptrtemp);
				          f(i, (ptrtemp,p)::rest))
	  | f(i,(x,OFFp 0)::rest) = (move(x,Displace(i*4,dataptr'));
				     f(i-1,rest))
	  | f(i,(Direct r, OFFp j)::rest) = (V.lea(Displace(j*4,r),
						   Displace(i*4,dataptr'));
						 f(i-1,rest))
	  | f(i,(x,p)::rest) = (move(x,ptrtemp); f(i,(ptrtemp,p)::rest))
      in
	 f(len - 2, rev vl);
	 move(dataptr,z);
         V.lea(Displace(4*len, dataptr'), ptrtemp);
         move(ptrtemp, dataptr)
(*         V.addl(Immed(4*len), dataptr)	*)
     end

fun select(i, Direct r, s) = move(Displace(i*4,r),s)
  | select(0, a, s) = move(defer a, s)	(* BUGBUG *)

fun offset(i, Direct r, s) = V.lea(Displace(i*4,r),s)

exception Three
fun three opcode (a,b,c) = 
	    if b=c then opcode(a,b) 
	    else if a=c then (move(a,ptrtemp); three opcode(ptrtemp,b,c))
	    else (move(b,c); opcode(a,c))

(* opcode is commutative *)
fun threec opcode (a,b,c) = 
	     if b=c then opcode(a,c)
	     else if a=c then opcode(b,c)
	     else (move(b,c); opcode(a,c))

(* opcode(x,y) is bop(y,anti(x)) *)
fun threeab opcode anti bop (a,b,c) = 
	     if b=c then opcode(a,c)
	     else if a=c then (anti(c,c); bop(b,c))
	     else (move(b,c); opcode(a,c))

(* The use of three here is silly.
   If opcode is commutative, then only one move should ever
   be necessary, not two, as in the middle case of above.
   Also note that we have few registers, so folding is likely.
   The only non-commutative 3-operation here is subl3.
   Subtraction may require a move, but in this case we should
   do a negate instead of a move, then add.
 *)

val addl3 = threec V.addl
val addl3t = addl3
val subl3 = threeab V.subl V.negl V.addl
val subl3t = subl3
fun ashl(s, r, d as Direct(Genreg _)) =
        (if r<>d then move(r,d) else ();
         V.ashl(s,d))
fun ashr(Immed i, b, c) = ashl(Immed (~i), b, c)
  | ashr(a,b,c) = (V.negl(a,ptrtemp);
		   ashl(ptrtemp,b,c))
val mull2 = V.mull
val mull2t = mull2
val divl2 = V.divl
val andb = threec V.andl
val orb = threec V.orl
val xorb = threec V.xorl
fun notb (a,b) = V.coml(a,b)

fun fetchindexl(Direct x,y,Immed k) = move(Displace(k+k-2,x),y)
  | fetchindexl(Direct x,y,Direct z) = move(Index(Displace(~2,x),z,Word),y)
  | fetchindexl(Immedlab lab, y, Direct z) =
	move(Index(OffAddress(lab,~2), z, Word), y)
fun storeindexl(x,y, Immed 1) = move(x, defer y)
  | storeindexl(x, Direct y, Immed k) = move(x, Displace(k+k-2,y))
  | storeindexl(x, Direct y, Direct z) = move(x,Index(Displace(~2,y),z,Word))
fun fetchindexb(v,w) = V.movzbl(Index(defer v, arithtemp', Byte),w)
fun storeindexb(v,w) = V.movb(v,Index(defer w, arithtemp', Byte))

fun finishreal(y) = (V.movl(Immed(8*power_tags + tag_string),
			    Displace(~4,dataptr'));
		     move(dataptr,y);
		     V.lea(Displace(4*3, dataptr'), ptrtemp);
		     move(ptrtemp, dataptr))
(*                     V.addl(Immed(4*3), dataptr)) *)

val fp0 = FloatReg 0

fun float f (a,b,c) =
    (V.movg(defer a, Direct fp0);
     f(defer b, Direct fp0);
     V.movg(Direct fp0, defer dataptr);
     finishreal c)

fun mnegg(x,y) = (V.negg(defer x, defer dataptr); finishreal y)

val mulg3 = float V.mulg
val divg3 = float V.divg
val addg3 = float V.addg
val subg3 = float V.subg

fun cbranch NEQ = V.bne
  | cbranch EQL = V.beq
  | cbranch LEQ = V.ble
  | cbranch GEQ = V.bge
  | cbranch LSS = V.blt
  | cbranch GTR = V.bgt

fun ibranch (cond, op1, op2, label) =
	(V.cmpl(op1, op2); cbranch cond (defer label))

fun gbranch (cond, op1, op2, label) =
	(V.cmpg(defer op1, defer op2); cbranch cond (defer label))

fun defer' j = fn x => j(defer x)
val jmp = defer' V.jump
val bbs = fn(x,y,l) => (V.tbit(x,y); V.bfs(defer l))

val immed = Immed
fun isimmed(Immed i) = SOME i
  | isimmed _ = NONE

fun isreg(Direct(Genreg i)) = SOME i
(*  | isreg(Direct(FP)) = SOME(8)		(* What?  BUGBUG *) *)
  | isreg _ = NONE

fun eqreg (a: EA) b = a=b

fun profile(i,incr) = if i >= Profile.PROFSIZE
			then ErrorMsg.impossible ("Bad profiling in ns32: trying "
						  ^makestring i^" with size "
						  ^makestring Profile.PROFSIZE)
			else V.addl(Immed incr, Displace(4*i,sp')) (* MSPACE mode *) 

val comment = V.comment
end
