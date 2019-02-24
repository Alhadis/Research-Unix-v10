(* Copyright 1989 by AT&T Bell Laboratories *)
structure VaxAssem = struct val outfile = ref std_out end

structure VaxAsCode  : VAXCODER = struct

open System.Tags VaxAssem

val offset = ref 0

type Label = string

local val i = ref 0 in
fun newlabel () = (i := !i + 1; "L" ^ makestring (!i))
end

fun itoa (i:int) = if i < 0 then "-" ^ makestring (~i)
		   else makestring i

datatype Register = reg of int

val r0 = reg 0
val r1 = reg 1
val r2 = reg 2
val r3 = reg 3
val r4 = reg 4
val r5 = reg 5
val r6 = reg 6
val r7 = reg 7
val r8 = reg 8
val r9 = reg 9
val r10 = reg 10
val r11 = reg 11
val r12 = reg 12
val r13 = reg 13
val sp = reg 14
val pc = reg 15

datatype EA = direct of Register
	    | autoinc of Register
	    | autodec of Register
	    | displace of int * Register
	    | deferred of int * Register
	    | immed of int
	    | immedlab of Label
	    | address of Label
	    | index of EA * Register

fun emit s = output (!outfile) s

fun newline () = (emit "\n"   (*  ; emit(makestring(!offset)); emit "\t" *) )

fun emitreg (reg 15) = emit "pc"
  | emitreg (reg 14) = emit "sp"
  | emitreg (reg r) = emit ("r" ^ itoa r)

fun emitarg (direct r) = emitreg r
  | emitarg (autoinc r) = (emit "("; emitreg r; emit ")+")
  | emitarg (autodec r) = (emit "-("; emitreg r; emit ")")
  | emitarg (immed i) = emit ("$" ^ itoa i)
  | emitarg (displace(0,r)) = (emit "("; emitreg r; emit ")")
  | emitarg (displace(i,r)) = (emit (itoa i); emit "("; emitreg r; emit ")")
  | emitarg (deferred(i,r)) = (emit ("*"^itoa i); emit "("; emitreg r; emit ")")
  | emitarg (address lab) = emit lab
  | emitarg (index(ea,r)) = (emitarg ea; emit "["; emitreg r; emit "]")
  | emitarg (immedlab lab) = (emit "BOGUS:"; emit lab)

fun emit1arg (a) = (emitarg a; newline())

fun emit2arg (a,b) = (emitarg a; emit ","; emitarg b; newline())

fun emit3arg (a,b,c) =
	(emitarg a; emit ","; emitarg b; emit ","; emitarg c; newline())

fun pure (autoinc _) = false
  | pure (autodec _) = false
  | pure _ = true

fun args23(f2,f3) (args as (a,b,c)) = 
	if b=c andalso pure b then (f2(a,b)) else f3 args

fun emitbarg (displace(n,reg 15)) = (emit "$"; emit(makestring n); newline())
  | emitbarg a = emit1arg a

fun align () = emit ".align 2\n"

local val p = makestring power_tags
      val t = makestring tag_backptr
in
fun mark () = let val lab = newlabel()
	      in  emit lab;
		  emit ": .long ((";
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
fun emitstring s = (emit ".ascii \""; emit(a_str s); emit "\"\n")
fun realconst s = (emit ".gfloat "; emit s; emit "\n")
fun emitlong (i : int) = (emit ".long "; emit(makestring i); emit "\n")

fun emitlab (offset,l2) = 
	(emit "5: .long "; emit l2; emit "-5b";
	 if offset < 0 then (emit "-"; emit (makestring (~offset)))
	               else (emit "+"; emit (makestring offset));
	 emit "\n")

fun jbr arg = (emit "jbr "; emit1arg arg)
fun bbc (immed 0, arg1, arg2) = (emit "blbc "; emit2arg(arg1,arg2))
  | bbc args = (emit "bbc "; emit3arg args)
fun bbs (immed 0, arg1, arg2) = (emit "blbs "; emit2arg(arg1,arg2))
  | bbs args = (emit "bbs "; emit3arg args)

fun movb args = (emit "movb "; emit2arg args)
fun movzbl args = (emit "movzbl "; emit2arg args)

fun pushal arg = (emit "pushal "; emit1arg arg)

fun addl2 args = (emit "addl2 "; emit2arg args)

fun moval (arg, autodec sp) = pushal arg
  | moval (args as (displace(i, reg p),direct (reg q))) =
	    if p=q andalso i> ~128 andalso i < 128
		then addl2(immed i, direct(reg p))
	        else (emit "moval "; emit2arg args)
  | moval args = (emit "moval "; emit2arg args)

fun movl(immedlab l, arg) = moval(address l, arg)
  | movl (arg, autodec sp) = (emit "pushl "; emit1arg arg)
  | movl (immed 0, arg) = (emit "clrl "; emit1arg arg)
  | movl args = (emit "movl "; emit2arg args)

fun movq args = (emit "movq "; emit2arg args)

fun rsb () = emit "rsb\n"
fun cmpl args = (emit "cmpl "; emit2arg args)
fun addl3 args = (emit "addl3 "; emit3arg args)
val addl3 = args23 (addl2,addl3)
fun subl2 args = (emit "subl2 "; emit2arg args)
fun subl3 args = (emit "subl3 "; emit3arg args)
val subl3 = args23 (subl2,subl3)
fun bisl3 args = (emit "bisl3 "; emit3arg args)
fun bicl3 args = (emit "bicl3 "; emit3arg args)
fun xorl3 args = (emit "xorl3 "; emit3arg args)
fun ashl args = (emit "ashl "; emit3arg args)
fun mull2 args = (emit "mull2 "; emit2arg args)
fun divl3 args = (emit "divl3 "; emit3arg args)
fun divl2 args = (emit "divl2 "; emit2arg args)
val divl3 = args23 (divl2,divl3)
fun jmp (arg as address lab) = jbr arg
  | jmp arg = (emit "jmp "; emit1arg arg)
fun brb arg = (emit "brb "; emitbarg arg)
fun brw arg = (emit "brw "; emitbarg arg)

fun beql arg = (emit "beql "; emitbarg arg)
fun bneq arg = (emit "bneq "; emitbarg arg)
fun jne arg = (emit "jneq "; emit1arg arg)
fun bgeq arg = (emit "bgeq "; emitbarg arg)
fun bgtr arg = (emit "bgtr "; emitbarg arg)
fun blss arg = (emit "blss "; emitbarg arg)
fun bleq arg = (emit "bleq "; emitbarg arg)
fun sobgeq arg = (emit "sobgeq "; emit2arg arg)

fun movg args = (emit "movg "; emit2arg args)
fun mnegg args = (emit "mnegg "; emit2arg args)
fun addg3 args = (emit "addg3 "; emit3arg args)
fun subg3 args = (emit "subg3 "; emit3arg args)
fun mulg3 args = (emit "mulg3 "; emit3arg args)
fun divg3 args = (emit "divg3 "; emit3arg args)
fun cmpg args = (emit "cmpg "; emit2arg args)

fun push arg = movl(arg,autodec sp)
fun pusha arg = moval(arg,autodec sp)
fun pop arg = movl(autoinc sp,arg)

val comment = emit

end (* structure AsCode *)
