(* Copyright 1989 by AT&T Bell Laboratories *)
functor M68CM(V : M68CODER) : CMACHINE = struct

structure V' : sig 
			datatype Register = DataReg of int
					  | AddrReg of int
					  | FloatReg of int
					  | PC
			
		        type Label sharing type Label = V.Label
			datatype Size = Byte | Word | Long
			
			datatype EA = Direct of Register
				    | PostInc of Register
				    | PreDec of Register
				    | Displace of Register * int
				    | Index of Register * int * Register * Size
				    | Immed of int
				    | Immedlab of Label
				    | Abs of int
				    | Address of Label

		end = V
open V'

datatype condition = NEQ | EQL | LEQ | GEQ | LSS | GTR

(* +DEBUG *)
fun diag (s : string) f x =
	f x handle e =>
		(print "?exception "; print (System.exn_name e);
		 print " in m68."; print s; print "\n";
		 raise e)
(* -DEBUG *)

fun defer(Direct r) = Displace(r,0)
  | defer(Immedlab lab) = Address lab
  | defer _ = ErrorMsg.impossible "defer in cpsm68"

(* DEBUG *) val defer = diag "defer" defer 

val exnptr = Direct(DataReg 7)
val dataptr as Direct dataptr' = Direct(AddrReg 6)
val arithtemp as Direct arithtemp' = Direct(DataReg 1)
val arithtemp2 = Direct(DataReg 2)
val arithtemp3 = Direct(DataReg 4)
val storeptr = Direct(DataReg 6)
val standardclosure = Direct(AddrReg 2)
val standardarg = Direct(AddrReg 0)
val standardcont = Direct(AddrReg 1)
val miscregs = map (Direct o AddrReg) [3,4]
val datalimit = Direct(DataReg 5)

val ptrtemp2 = Direct(DataReg 3)
val ptrtemp as Direct ptrtemp' = Direct(AddrReg 5)

fun reg(Direct r) = r

fun newlabel() = Immedlab(V.newlabel())
(* DEBUG *) val newlabel = diag "newlabel" newlabel 
fun emitlab(i,Immedlab lab) = V.emitlab(i,lab)
fun define (Immedlab lab) = V.define lab

fun beginStdFn _ = ()

(* checkLimit (n):
 * Generate code to check the heap limit to see if there is enough free space
 * to allocate n bytes.
 *)
fun checkLimit maxAllocSize = (
      V.comment ("check limit, max alloc = "^(makestring maxAllocSize)^"\n");
    (* Check the heap limit register *)
      if (maxAllocSize <= 4096)
	then (
	  V.cmpl(dataptr, datalimit);
	  V.trapmi())
	else (
	  V.movl (dataptr, arithtemp3);
	  V.addl(Immed(maxAllocSize-4096), arithtemp3);
	  V.cmpl(arithtemp3, datalimit);
	  V.trapmi()))

val align = V.align
val mark = V.mark
fun move(Immedlab l, dest as Direct(AddrReg x)) = V.lea(Address l, dest)
  | move(Immedlab l, dest) = 
	    (V.lea(Address l, ptrtemp);
	     move(ptrtemp,dest))
  | move(Displace(DataReg(d), i), dest) =
	    (V.movl(Direct(DataReg(d)), ptrtemp);
	     move(Displace(reg(ptrtemp), i), dest))
(* let's hope that ptrtemp never shows up in both src and dest! *)
  | move(src, Address l) =
	      (V.lea(Address l, ptrtemp);
	       move(src, Displace(reg(ptrtemp), 0)))
  | move x = V.movl x
(* DEBUG *) val move = diag "move" move 

val emitlong = V.emitlong
val realconst = V.realconst
val emitstring = V.emitstring

fun ashl(s as Immed k, r, d as Direct(DataReg _)) =
    (if r<>d then move(r,d) else (); V.asll(s,d))
  | ashl(s as Direct(DataReg _),r,d) =
    (move(r,arithtemp3); V.asll(s,arithtemp3); move(arithtemp3,d))
  | ashl(s as Immed k,r,d) =
    (move(r,arithtemp3);
     if k>8
       then (move(s,arithtemp2); V.asll(arithtemp2,arithtemp3))
       else V.asll(s,arithtemp3);
     move(arithtemp3,d))

(* DEBUG *) val ashl = diag "ashl" ashl 

fun ashr(s as Immed k, r, d as Direct(DataReg _)) =
    (if r<>d then move(r,d) else ();
     if k>8 then (move(s,arithtemp3); V.asrl(arithtemp3,d)) else V.asrl(s,d))
  | ashr(s as Direct(DataReg _),r,d) =
    (move(r,arithtemp3); V.asrl(s,arithtemp3); move(arithtemp3,d))
  | ashr(s as Immed k,r,d) =
    (move(r,arithtemp3); 
     if k>8
     then (move(s,arithtemp2); V.asrl(arithtemp2,arithtemp3))
     else V.asrl(s,arithtemp3);
     move(arithtemp3,d))

fun jmpindexb lab = V.jra(Index(PC,2,arithtemp',Byte))
(* DEBUG *) val jmpindexb = diag "jmpindexb" jmpindexb 

fun record(vl, z) =
    let open CPS
	fun f (Direct r, SELp(j,p)) = f(Displace(r,j*4),p)
	  | f (Immedlab l, p) = (V.lea(Address l, ptrtemp); f(ptrtemp,p))
	  | f (x,OFFp 0) = V.movl(x, PostInc dataptr')
	  | f (Direct r, OFFp j) = (V.lea(Displace(r,j*4),ptrtemp);
				    f(ptrtemp,OFFp 0))
	  | f (x,p) = (V.movl(x,ptrtemp); f(ptrtemp,p))
      in
	 app f vl;
	 V.lea(Displace(dataptr',~4*(List.length(vl)-1)),z)
     end

fun select(i, Direct r, s) = move(Displace(r,i*4),s)
  | select(0, a, s) = move(defer a,s)

fun offset(i, Direct r, s) = V.lea(Displace(r,i*4),s)
(* DEBUG *) val select = diag "select" select 
(* DEBUG *) val offset = diag "offset" offset 

exception Three
fun three opcode (a as Direct(AddrReg _), b as Direct(AddrReg _),
		  c as Direct(AddrReg _)) =
                (three opcode(a,b,arithtemp3); move(arithtemp3,c))
  | three opcode (a,b,c) = 
	    if b=c then opcode(a,b) 
	    else if a=c then (move(a,arithtemp3); three opcode(arithtemp3,b,c))
	    else (move(b,c); opcode(a,c))

fun threet opcode (a,b,c as Direct(AddrReg _)) =
                (threet opcode(a,b,arithtemp3); move(arithtemp3,c))
  | threet opcode (a,b,c) = 
	    if b=c then (opcode(a,b); V.trapv())
	    else if a=c then (move(a,arithtemp3); threet opcode(arithtemp3,b,c))
	    else (move(b,c); opcode(a,c); V.trapv())

fun three' opcode (a as Immed _,b,c as Direct(DataReg _)) =
	    three opcode(a,b,c)
  | three' opcode (a as Direct(AddrReg _),b,c) =
	    (move(b,arithtemp3); move(a,arithtemp2);
	     opcode(arithtemp2,arithtemp3); move(arithtemp3,c))
  | three' opcode (a,b,c) =
	    (move(b,arithtemp3); opcode(a,arithtemp3); move(arithtemp3,c))

fun orb(a as Immed k,b,c as Direct(DataReg _)) =
    if k<65538
    then if k<=0
         then raise Match
	 else if b=c then V.orl(a,b) else (move(b,c); V.orl(a,c))
    else (move(a,arithtemp3);
	  if b=c then V.orl(arithtemp3,b) else (move(b,c); V.orl(arithtemp3,c)))
  | orb(a as Direct(AddrReg _),b,c) =
	    (move(b,arithtemp3); move(a,arithtemp2);
	     V.orl(arithtemp2,arithtemp3); move(arithtemp3,c))
  | orb(a as Immed k,b,c) =
    if k<65536
    then if k<=0
         then raise Match
	 else (move(b,arithtemp3); V.orl(a,arithtemp3); move(arithtemp3,c))
    else (move(a,arithtemp2);
	  move(b,arithtemp3);
	  V.orl(arithtemp2,arithtemp3);
	  move(arithtemp3,c))
  | orb(a,b,c) = (move(b,arithtemp3); V.orl(a,arithtemp3); move(arithtemp3,c))

fun xorb(a as Immed k,b,c as Direct(DataReg _)) =
    if k<65536
    then if k<=0
         then raise Match
	 else if b=c then V.eorl(a,b) else (move(b,c); V.eorl(a,c))
    else (move(a,arithtemp3);
	  if b=c then V.eorl(arithtemp3,b) else (move(b,c); V.eorl(arithtemp3,c)))
  | xorb(a as Direct(AddrReg _),b,c) =
	    (move(b,arithtemp3); move(a,arithtemp2);
	     V.eorl(arithtemp2,arithtemp3); move(arithtemp3,c))
  | xorb(a as Immed k,b,c) =
    if k<65538
    then if k<=0
         then raise Match
	 else (move(b,arithtemp3); V.eorl(a,arithtemp3); move(arithtemp3,c))
    else (move(a,arithtemp2);
	  move(b,arithtemp3);
	  V.eorl(arithtemp2,arithtemp3);
	  move(arithtemp3,c))
  | xorb(a,b,c) = (move(b,arithtemp3); V.eorl(a,arithtemp3); move(arithtemp3,c))

fun notb(a,b) = (move(Immed ~1,arithtemp3); V.subl(a,arithtemp3);
		 move(arithtemp3,b))
val andb = three' V.andl
val addl3 = three V.addl
val addl3t = threet V.addl
val subl3 = three V.subl
val subl3t = threet V.subl
val mull2 = V.mull
fun mull2t x = (mull2 x; V.trapv())
val divl2 = V.divl

exception Fetchindexb
fun fetchindexb(Direct x,y) = (if y=arithtemp then raise Fetchindexb else ();
			       move(Immed 0,y); 
			       V.movb(Index(x,0,arithtemp',Byte),y))
(* DEBUG *) val fetchindexb = diag "fetchindexb" fetchindexb 
fun storeindexb(x, Direct y) = V.movb(x,Index(y,0,arithtemp',Byte))
(* DEBUG *) val storeindexb = diag "storeindexb" storeindexb 
fun fetchindexl(Direct x,y,Immed k) = move(Displace(x,k+k-2),y)
  | fetchindexl(Direct x,y,Direct z) = move(Index(x,~2,z,Word),y)
  | fetchindexl(Immedlab lab, y, Direct z) = 
	    (* this is a hack, since it depends on lab being PC+6 *)
		    move(Index(PC,4,z,Word), y);
(* DEBUG *) val fetchindexl = diag "fetchindexl" fetchindexl 
fun storeindexl(x, y, Immed 1) = move(x, defer y)
  | storeindexl(x, Direct y, Immed k) = move(x, Displace(y,k+k-2))
  | storeindexl(x, Direct y, Direct z) = move(x,Index(y,~2,z,Word))
(* DEBUG *) val storeindexl = diag "storeindexl" storeindexl 

val fp0 = FloatReg 0

fun finishreal(c) =  (V.cmpl(dataptr,datalimit);
		      V.trapmi();
		      V.movl(Immed(8*System.Tags.power_tags
				    + System.Tags.tag_string),
		             PostInc dataptr');
		     V.movl(dataptr,c);
		     V.fmoved(Direct fp0, PostInc dataptr'))

fun float f (a,b,c) =
    (V.fmoved(defer a, Direct fp0);
     f(defer b, Direct fp0);
     finishreal c)

fun mnegg (a,c) = (V.fnegd(defer a, Direct fp0); finishreal c)

val mulg3 = float V.fmuld
val divg3 = float V.fdivd
val addg3 = float V.faddd
val subg3 = float V.fsubd

fun cbranch NEQ = V.jne
  | cbranch EQL = V.jeq
  | cbranch LEQ = V.jle
  | cbranch GEQ = V.jge
  | cbranch LSS = V.jlt
  | cbranch GTR = V.jgt

fun fbranch NEQ = V.fjne
  | fbranch EQL = V.fjeq
  | fbranch LEQ = V.fjle
  | fbranch GEQ = V.fjge
  | fbranch LSS = V.fjlt
  | fbranch GTR = V.fjgt

fun rev LEQ = GEQ
  | rev GEQ = LEQ
  | rev LSS = GTR
  | rev GTR = LSS
  | rev NEQ = NEQ
  | rev EQL = EQL

fun ibranch (cond, op1 as Immed _, op2, label) =
	(V.cmpl(op1, op2); cbranch (rev cond) (defer label))
  | ibranch (cond, op1, op2, label) =
	(V.cmpl(op2, op1); cbranch cond (defer label))

fun gbranch (cond, op1, op2, label) =
	(V.fmoved(defer op1,Direct fp0);
         V.fcmpd(defer op2, Direct fp0);
         fbranch cond (defer label))

fun defer' j = fn x => j(defer x)
val jmp = defer' V.jra
fun bbs (x,dest as Direct(AddrReg _) ,l) = (move(dest,ptrtemp2);
					    bbs(x,ptrtemp2,l))
  | bbs (x,y,l) = (V.btst(x,y); V.jne(defer l))
(* DEBUG *) val bbs = diag "bbs" bbs 

val immed = Immed
fun isimmed(Immed i) = SOME i
  | isimmed _ = NONE
fun isreg(Direct(AddrReg i)) = SOME i
  | isreg(Direct(DataReg i)) = SOME(i+8)
  | isreg _		= NONE

fun eqreg (a: EA) b = a=b
(* DEBUG *) val eqreg = diag "eqreg" eqreg 

fun profile(index,incr) = V.addl(Immed incr, Displace(V.sp,4*index))

val comment = V.comment
end
