(* Copyright 1989 by AT&T Bell Laboratories *)
structure NS32Assem = struct val outfile = ref std_out end

structure NS32AsCode  : NS32CODER = struct

open System.Tags NS32Assem

val offset = ref 0

type Label = string

local val i = ref 0 in
fun newlabel () = (i := !i + 1; "L" ^ makestring (!i))
end

fun itoa (i:int) = if i < 0 then "-" ^ makestring (~i)
		   else makestring i

datatype Register = Genreg of int
		  | FloatReg of int
		  | FP
		  | SP
		  | SB
		  | PC

val r0 = Genreg 0
val r1 = Genreg 1
val r2 = Genreg 2
val r3 = Genreg 3
val r4 = Genreg 4
val r5 = Genreg 5
val r6 = Genreg 6
val r7 = Genreg 7
val fp = FP
val sp = SP
val sb = SB
val pc = PC
val fp0 = FloatReg 0

datatype Size = Byte | Word | Long

datatype EA = Direct of Register
	    | Topofstack
	    | Displace of int * Register
	    | Immed of int
	    | Immedlab of Label
	    | Abs of int
	    | OffAddress of Label * int
	    | Index of EA * Register * Size

fun address lab = OffAddress(lab,0)	(* old Address style *)

fun emit s = output (!outfile) s
(* fun emit s = (output std_out s; output (!outfile) s)	DEBUG *)

fun newline () = (emit "\n"   (*  ; emit(makestring(!offset)); emit "\t" *) )

fun emitopc opc = (emit "\t"; emit opc; emit "\t")

fun emitreg (Genreg r) = emit ("r" ^ itoa r)
  | emitreg (FloatReg r) = emit ("f" ^ itoa r)
  | emitreg (FP) = emit "fp"
  | emitreg (SP) = emit "sp"
  | emitreg (SB) = emit "sb"
  | emitreg (PC) = emit "pc"

fun emitsize (Byte) = emit "B"
  | emitsize (Word) = emit "W"
  | emitsize (Long) = emit "L"

fun emitarg (Direct r) = emitreg r
  | emitarg (Immed i) = emit ("$" ^ itoa i)
  | emitarg (Displace(0,r)) = (emit "("; emitreg r; emit ")")
  | emitarg (Displace(i,r)) = (emit (itoa i); emit "("; emitreg r; emit ")")
  | emitarg (OffAddress(lab,i)) = (emit lab;
				   if i>0 then emit ("+" ^ makestring i)
				   else if i<0 then emit (itoa i)
				   else ())
  | emitarg (Index(ea,r,sz)) = (emitarg ea; emit "["; emitreg r; emit ":";
				emitsize sz; emit "]") 
  | emitarg (Topofstack) = (emit "TOS")
  | emitarg (Immedlab lab) = (emit "BOGUS:"; emit lab)
  | emitarg (Abs n) = emit ("BOGUSABS:" ^ itoa(n))

fun emit1arg (a) = (emitarg a; newline())

fun emit2arg (a,b) = (emitarg a; emit ","; emitarg b; newline())

fun align () = emit "\t.align\t2\n"

local val p = makestring power_tags
      val t = makestring tag_backptr
in
fun mark () = let val lab = newlabel()
	      in  emit lab;
		  emit ":\t.long\t((";
		  emit lab;
		  emit "-base)/4+1)*";   (* STRING dependency *)
		  emit p;
		  emit "+";
		  emit t;
		  emit "\n"
	      end
end

fun define lab = (emit lab; emit ":\n")
fun oct i = let val m = Integer.makestring
	    in  m(i div 64)^m((i div 8)mod 8)^m(i mod 8) end
fun c_char "\n" = "\\n"
  | c_char "\t" = "\\t"
  | c_char "\\" = "\\\\"
  | c_char "\"" = "\\\""
  | c_char c = if ord c < 32 then "\\"^oct(ord c) else c
fun a_str s = implode(map c_char (explode s))
fun emitstring s = (emit "\t.ascii\t\""; emit(a_str s); emit "\"\n")
fun realconst s = (emit "\t.gfloat\t"; emit s; emit "\n")
fun emitlong (i : int) = (emit "\t.long\t"; emit(makestring i); emit "\n")

fun emitlab (offset,l2) = 
	(emit "5:\t.long\t"; emit l2; emit "-5b";
	 if offset < 0 then (emit "-"; emit (makestring (~offset)))
	               else (emit "+"; emit (makestring offset));
	 emit "\n")

fun isquick k = (k<=7 andalso k>= ~8)

fun lprl (src,preg) = (emitopc "lprl"; emit2arg (preg,src))
fun sprl args = (emitopc "sprl"; emit2arg args)
fun tbit (arg1 as Immed k, arg2) = (emitopc "tbitl"; emit2arg(arg1,arg2))
  | tbit args = (emitopc "tbitl"; emit2arg args)
fun bfs arg = (emitopc "bfs"; emit1arg arg)

fun movql args = (emitopc "movql"; emit2arg args)

fun movb args = (emitopc "movb"; emit2arg args)
fun movzbl args = (emitopc "movzbl"; emit2arg args)

fun lea args = (emitopc "addr"; emit2arg args)

fun movl (Immedlab l, arg) = lea(address l, arg)
  | movl (args as (Immed k, dest)) = 
	if isquick(k)
	    then movql args
	    else (emitopc "movl"; emit2arg args)
  | movl args = (emitopc "movl"; emit2arg args)

fun addl (args as (Immed k, dest)) =
	if isquick(k)
	    then (emitopc "addql"; emit2arg args)
	    else (emitopc "addl"; emit2arg args)
  | addl args = (emitopc "addl"; emit2arg args)
fun subl args = (emitopc "subl"; emit2arg args)
fun negl args = (emitopc "negl"; emit2arg args)
fun ashl args = (emitopc "ashl"; emit2arg args)
fun andl args = (emitopc "andl"; emit2arg args)
fun orl args = (emitopc "orl"; emit2arg args)
fun xorl args = (emitopc "xorl"; emit2arg args)
fun coml args = (emitopc "coml"; emit2arg args)
fun mull args = (emitopc "mull"; emit2arg args)
fun divl args = (emitopc "divl"; emit2arg args)

fun br arg = (emitopc "br"; emit1arg arg)
fun jump (arg as OffAddress _) = br arg
  | jump arg = (emitopc "jump"; emit1arg arg)
fun beq arg = (emitopc "beq"; emit1arg arg)
fun bne arg = (emitopc "bne"; emit1arg arg)
fun bge arg = (emitopc "bge"; emit1arg arg)
fun bgt arg = (emitopc "bgt"; emit1arg arg)
fun blt arg = (emitopc "blt"; emit1arg arg)
fun ble arg = (emitopc "ble"; emit1arg arg)
fun cmpl (args as (Immed k,arg2)) =
	if isquick(k)
	    then (emitopc "cmpql"; emit2arg args)
	    else (emitopc "cmpl"; emit2arg args)
  | cmpl args = (emitopc "cmpl"; emit2arg args)


fun movg args = (emitopc "movg"; emit2arg args)
fun negg args = (emitopc "negg"; emit2arg args)
fun addg args = (emitopc "addg"; emit2arg args)
fun subg args = (emitopc "subg"; emit2arg args)
fun mulg args = (emitopc "mulg"; emit2arg args)
fun divg args = (emitopc "divg"; emit2arg args)
fun cmpg args = (emitopc "cmpg"; emit2arg args)


val comment = emit

end (* structure NS32AsCode *)
