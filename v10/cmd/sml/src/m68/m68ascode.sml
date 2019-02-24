(* Copyright 1989 by AT&T Bell Laboratories *)
structure M68Assem = struct val outfile = ref std_out end

structure M68AsCode : M68CODER = struct

open System.Tags M68Assem 

(* DEBUG
fun diag (s : string) f x =
	f x handle e =>
		(print "?exception "; print (System.exn_name e);
		 print " in m68ascode."; print s; print "\n";
		 raise e)
*)

val offset = ref 0

type Label = string

local val i = ref 0 in
fun newlabel () = (i := !i + 1; "L" ^ makestring (!i))
end

fun itoa (i:int) = if i < 0 then "-" ^ makestring (~i)
		   else makestring i

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

val d0 = DataReg 0
and d1 = DataReg 1
and d2 = DataReg 2
and d3 = DataReg 3
and d4 = DataReg 4
and d5 = DataReg 5
and d6 = DataReg 6
and d7 = DataReg 7
and a0 = AddrReg 0
and a1 = AddrReg 1
and a2 = AddrReg 2
and a3 = AddrReg 3
and a4 = AddrReg 4
and a5 = AddrReg 5
and a6 = AddrReg 6
and sp = AddrReg 7
and fp0 = FloatReg 0
and fp1 = FloatReg 1
and fp2 = FloatReg 2
and fp3 = FloatReg 3
and fp4 = FloatReg 4
and fp5 = FloatReg 5
and fp6 = FloatReg 6
and fp7 = FloatReg 7

fun emit s = output (!outfile) s

fun emitreg (DataReg 0) = emit "d0"
  | emitreg (DataReg 1) = emit "d1"
  | emitreg (DataReg 2) = emit "d2"
  | emitreg (DataReg 3) = emit "d3"
  | emitreg (DataReg 4) = emit "d4"
  | emitreg (DataReg 5) = emit "d5"
  | emitreg (DataReg 6) = emit "d6"
  | emitreg (DataReg 7) = emit "d7"
  | emitreg (AddrReg 0) = emit "a0"
  | emitreg (AddrReg 1) = emit "a1"
  | emitreg (AddrReg 2) = emit "a2"
  | emitreg (AddrReg 3) = emit "a3"
  | emitreg (AddrReg 4) = emit "a4"
  | emitreg (AddrReg 5) = emit "a5"
  | emitreg (AddrReg 6) = emit "a6"
  | emitreg (AddrReg 7) = emit "sp"
  | emitreg (FloatReg 0) = emit "fp0"
  | emitreg (FloatReg 1) = emit "fp1"
  | emitreg (FloatReg 2) = emit "fp2"
  | emitreg (FloatReg 3) = emit "fp3"
  | emitreg (FloatReg 4) = emit "fp4"
  | emitreg (FloatReg 5) = emit "fp5"
  | emitreg (FloatReg 6) = emit "fp6"
  | emitreg (FloatReg 7) = emit "fp7"
  | emitreg PC = emit "pc"

(* DEBUG val emitreg = diag "emitreg" emitreg *)

fun sizeint i =
	if i < 128 andalso i > ~129 then Byte
	else if i < 32768 andalso i > ~32769 then Word
	else Long

(* DEBUG val sizeint = diag "sizeint" sizeint *)

fun emitarg (Immed i) = (emit "#"; emit (itoa i))
  | emitarg (Abs i) = emit (itoa i)
  | emitarg (Direct r) = emitreg r
  | emitarg (Displace (ra as AddrReg _,0)) = (emitreg ra; emit "@")
  | emitarg (PostInc ra) = (emitreg ra; emit "@+")
  | emitarg (PreDec ra) = (emitreg ra; emit "@-")
  | emitarg (Displace (r as DataReg _, i)) = 
	    (emit"@("; emit(itoa i); emit ","; emitreg r; emit ":L:1)")
  | emitarg (Displace (r as AddrReg _,i)) =
	(emitreg r;
	 emit "@(";
	 emit (itoa i);
	 emit ")")
  | emitarg (Index (ra,disp,r,s)) =
	(emitreg ra;
	 emit "@(";
	 emit (itoa disp);
	 emit ",";
	 emitreg r;
	 emit ":L:";
	 emit (case s of Byte => "1" | Word => "2" | Long => "4");    
	 emit ")")
  | emitarg (Address lab) = emit lab

(* DEBUG val emitarg = diag "emitarg" emitarg *)

fun emit2arg (a,b) = (emitarg a; emit ","; emitarg b; emit "\n")

(* DEBUG val emit2arg = diag "emit2arg" emit2arg *)

fun emit1arg (a) = (emitarg a; emit "\n")

fun align () = emit ".align 2\n"

(* DEBUG val align = diag "align" align *)

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

(* DEBUG val mark = diag "mark" mark *)

fun define lab = (emit lab; emit ":\n")
(* DEBUG val define = diag "define" define *)
fun oct i = let val m = Integer.makestring
	    in  m(i div 64)^m((i div 8)mod 8)^m(i mod 8) end
(* DEBUG val oct = diag "oct" oct *)
fun c_char "\n" = "\\n"
  | c_char "\t" = "\\t"
  | c_char "\\" = "\\\\"
  | c_char "\"" = "\\\""
  | c_char c = if ord c < 32 then "\\"^oct(ord c) else c
fun a_str s = implode(map c_char (explode s))
fun emitstring s = (emit ".ascii \""; emit(a_str s); emit "\"\n")
fun realconst s = (emit ".double 0r"; emit s; emit "\n")
fun emitlong (i : int) = (emit ".long "; emit(makestring i); emit "\n")

fun emitlab (offset,l2) = 
	(emit "5: .long "; emit l2; emit "-5b";
	 if offset < 0 then (emit "-"; emit (makestring (~offset)))
	               else (emit "+"; emit (makestring offset));
	 emit "\n")

exception Illegal

fun rts () = emit "rts\n"

fun exg (arg as (Direct(AddrReg a),Direct(AddrReg b))) =
	if a = b then ()
	else (emit "exg "; emit2arg arg)
  | exg (arg as (Direct(DataReg a),Direct(DataReg b))) =
	if a = b then ()
	else (emit "exg "; emit2arg arg)
  | exg (arg as (Direct(AddrReg b),Direct(DataReg a))) =
	(emit "exg "; emit2arg arg)
  | exg (arg as (Direct(DataReg a),Direct(AddrReg b))) =
	(emit "exg "; emit2arg arg)

fun pea (Direct _) = raise Illegal
  | pea (PreDec _) = raise Illegal
  | pea (PostInc _) = raise Illegal
  | pea (Immed _) = raise Illegal
  | pea (arg as (Address lab)) =
	(emit "pea "; emit1arg arg)
  | pea (arg) =
	(emit "pea "; emit1arg arg)

fun movl (_,Immed _) = raise Illegal
  (* labels not implemented *)
  (* MOVEQ/MOVE *)
  | movl (src as (Immed i),dest as (Direct(DataReg d))) =
	(case sizeint i of
	    Byte => (emit "moveq "; emit2arg (src,dest))
	  | _ => (emit "movl "; emit2arg (src,dest)))
  (* MOVEA *)
  | movl (src,dest as (Direct(AddrReg a))) =
	(emit "movl "; emit2arg (src,dest))
  (* general MOVE *)
  | movl (src,dest) =
	(emit "movl "; emit2arg (src,dest))

fun addl (_,Immed _) = raise Illegal
  (* ADDQ/ADDA *)
  | addl (src as (Immed i),dest as (Direct(AddrReg a))) =
	if i <= 8 andalso i >= 1 then
		(emit "addql "; emit2arg(src,dest))
	else	(emit "addl "; emit2arg(src,dest))
  | addl (src,dest as (Direct(AddrReg a))) =
	(emit "addl "; emit2arg (src,dest))
  (* ADDQ/ADDI *)
  | addl (src as (Immed i),dest) =
	if i <= 8 andalso i >= 1 then
		(emit "addql "; emit2arg (src,dest))
	else	(emit "addl "; emit2arg (src,dest))
  (* general ADD *)
  | addl (src,dest as (Direct(DataReg d))) = 
	(emit "addl "; emit2arg (src,dest))
  | addl (src as (Direct(DataReg d)),dest) =
	(emit "addl "; emit2arg (src,dest))

fun lea (Direct _,_) = raise Illegal
  | lea (PreDec _,_) = raise Illegal
  | lea (PostInc _,_) = raise Illegal
  | lea (Immed _,_) = raise Illegal
  | lea (src as Address _,dest as Direct(AddrReg _)) =
	(emit "lea "; emit2arg (src,dest))
  | lea (src,dest as (Direct(AddrReg a))) =
	(emit "lea "; emit2arg (src,dest))
  | lea (Displace(a, i), dest as (Direct(DataReg _))) =    (* fake lea to data register *)
	(movl(Direct(a), dest); addl(Immed(i), dest))
  | lea _ = raise Illegal

(* DEBUG val lea = diag "lea" lea *)

fun subl (_,Immed _) = raise Illegal
  (* SUBQ/SUBA *)
  | subl (src  as (Immed i),dest as (Direct(AddrReg a))) =
	if i <= 8 andalso i >= 1 then
		(emit "subql "; emit2arg(src,dest))
	else	(emit "subl "; emit2arg(src,dest))
  (* SUBA *)
  | subl (src,dest as (Direct(AddrReg a))) =
	(emit "subl "; emit2arg(src,dest))
  (* SUBQ/SUBI *)
  | subl (src as (Immed i),dest) =
	if i <= 8 andalso i >= 1 then
		(emit "subql "; emit2arg(src,dest))
	else	(emit "subl "; emit2arg(src,dest))
  (* general SUB *)
  | subl (src,dest as (Direct(DataReg d))) = 
	(emit "subl "; emit2arg(src,dest))
  | subl (src as (Direct(DataReg d)),dest) = 
	(emit "subl "; emit2arg(src,dest))

fun eorl (_, Direct(AddrReg _)) = raise Illegal
  | eorl args = (emit "eorl "; emit2arg args)

fun orl (_, Direct(AddrReg _)) = raise Illegal
  | orl args = (emit "orl "; emit2arg args)

fun andl (_, Direct(AddrReg _)) = raise Illegal
  | andl args = (emit "andl "; emit2arg args)

fun divl args = (emit "divl "; emit2arg args)
fun mull args = (emit "mull "; emit2arg args)
fun asll (Immed 1, arg) = (emit "asll "; emit1arg arg)
  | asll args = (emit "asll "; emit2arg args)
fun asrl (Immed 1, arg) = (emit "asrl "; emit1arg arg)
  | asrl args = (emit "asrl "; emit2arg args)
fun movb (Immed 0, arg) = (emit "clrb "; emit1arg arg)
  | movb args = (emit "movb "; emit2arg args)

fun cmpl (src, dest as (Immed i)) =
	(print "?bad cmpl\n"; emit "| BAD cmpl "; emit2arg(src, dest))
  (* CMP *)
  | cmpl (src,dest as (Direct(DataReg d))) =
	(emit "cmpl "; emit2arg (src,dest))
  (* CMPA *)
  | cmpl (src,dest as (Direct(AddrReg a))) =
	(emit "cmpl "; emit2arg (src,dest))
  (* CMPI *)
  | cmpl (src as (Immed i),dest) =
	(emit "cmpl "; emit2arg (src,dest))
  (* CMPM *)
  | cmpl (src as (PostInc(AddrReg y)),dest as (PostInc(AddrReg x))) =
	(emit "cmpm "; emit2arg (src,dest))

fun btst (_,Direct(AddrReg _)) = raise Illegal
  | btst (_,Immed _) = raise Illegal
  | btst (src as (Direct(DataReg d)),dest) =
	(emit "btst "; emit2arg (src,dest))
  | btst (src as (Immed i),dest) =
	(emit "btst "; emit2arg (src,dest))


fun jne (arg as (Address lab)) = (emit "jne "; emit1arg arg)
fun jeq (arg as (Address lab)) = (emit "jeq "; emit1arg arg)
fun jgt (arg as (Address lab)) = (emit "jgt "; emit1arg arg)
fun jge (arg as (Address lab)) = (emit "jge "; emit1arg arg)
fun jlt (arg as (Address lab)) = (emit "jlt "; emit1arg arg)
fun jle (arg as (Address lab)) = (emit "jle "; emit1arg arg)

fun jra (arg as (Address lab)) =
	(emit "jra "; emit1arg arg)
  | jra (arg as (Displace(AddrReg a,i))) =
	(emit "jra "; emit1arg arg)
  | jra (arg as Index _) =
	(emit "jra "; emit1arg arg)

fun jbsr (arg as (Address lab)) =
	(emit "jbsr "; emit1arg arg)
  | jbsr (arg as (Displace(AddrReg _,_))) =
	(emit "jbsr "; emit1arg arg)

(* 68881 float operations *)
(* Some src/dest combinations are illegal, but not caught here. *)
fun fjne (arg as (Address lab)) = (emit "fjne "; emit1arg arg)
fun fjeq (arg as (Address lab)) = (emit "fjeq "; emit1arg arg)
fun fjgt (arg as (Address lab)) = (emit "fjgt "; emit1arg arg)
fun fjge (arg as (Address lab)) = (emit "fjge "; emit1arg arg)
fun fjlt (arg as (Address lab)) = (emit "fjlt "; emit1arg arg)
fun fjle (arg as (Address lab)) = (emit "fjle "; emit1arg arg)

fun fcmpd (arg as (src,dest)) = (emit "fcmpd "; emit2arg arg)
fun faddd (arg as (src,dest)) = (emit "faddd "; emit2arg arg)
fun fsubd (arg as (src,dest)) = (emit "fsubd "; emit2arg arg)
fun fmuld (arg as (src,dest)) = (emit "fmuld "; emit2arg arg)
fun fdivd (arg as (src,dest)) = (emit "fdivd "; emit2arg arg)
fun fnegd (arg as (src,dest)) = (emit "fnegd "; emit2arg arg)
fun fmoved (arg as (src,dest as Direct(FloatReg f))) =
	(emit "fmoved "; emit2arg arg)
  | fmoved (arg as (src as Direct(FloatReg f),dest)) =
	(emit "fmoved "; emit2arg arg)

fun trapv() = emit "trapv\n"
fun trapmi() = emit "trapmi\n"

fun exg args = (emit "exg "; emit2arg args)

fun push ea = movl(ea,PreDec sp)

fun pop ea = movl(PostInc sp,ea)

val pusha = pea

val comment = emit

end (* structure AsCode *)
