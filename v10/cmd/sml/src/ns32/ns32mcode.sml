(* Copyright 1989 by AT&T Bell Laboratories *)
structure NS32MCode : NS32MCODER = struct

structure Jumps = struct
    datatype JumpKind = DISPL of int
		      | LNGDISPL of int
		      | LABPTR of int
fun isquick k = (k<=7 andalso k>= ~8)

fun eword i =
	if i<0 then eword(65536+i)
	else chr(i mod 256) ^ chr(i div 256)

fun erword i =
	if i<0 then erword(65536+i)
	else chr(i div 256) ^ chr(i mod 256)

fun elong i =
        if i<0 
	  then let val a = ~i;
		   val b = a mod 65536;
		   val c = a div 65536;
		in eword(~b) ^ eword(~c + if b=0 then 0 else ~1)
	       end
	  else eword(i mod 65536) ^ eword(i div 65536)

val emitlong = elong

fun intsize(i) =
        if i<=63 andalso i>= ~64
	    then 1
	else if i<=8191 andalso i>= ~8192
	    then 2
	else 4;

fun Ldisp (len,d) =
	if len=1 andalso d<=63 andalso d>= ~64
	    then if d>=0 then chr(d)
		         else chr(128+d)
	    else if len=2 andalso d<=8191 andalso d>= ~8192
		then if d>=0 then erword(128*256+d)
			     else erword(128*256+d+16384)
	    else if d>=0
		then erword(192*256+(d div 65536)) ^ erword(d mod 65536)
	    else let val a = ~d
		     val b = a mod 65536
		     val c = a div 65536
		  in erword(~c + if b=0 then 0 else ~1) ^ erword(~b)
		 end

fun sizejump(LABPTR _,oldsize,s,d) = 4
  | sizejump(LNGDISPL _,oldsize,s,d) = 4
  | sizejump(DISPL i,oldsize,s,d) =
	case oldsize of
	    0 => intsize(d-s+i)
	  | 1 => 2     (* I question this! -- A. Appel *)
	  | _ => 4

fun emitjump(LABPTR i,4,s,d) = elong(d-s+i)
  | emitjump(DISPL i,sz,s,d) = Ldisp(sz,d-s+i)	(* initial pc is i bytes back *)
  | emitjump(LNGDISPL i,4,s,d) = Ldisp(4,d-s+i)	(* initial pc is i bytes back *)
  | emitjump(_,sz,s,d) = ErrorMsg.impossible "bad size in emitjump"

 end (* Jumps *)

structure Emitter : BACKPATCH = Backpatch(Jumps)

structure Coder : NS32CODER = struct

open Emitter Jumps

fun emitbyte i = emitstring(chr i)
fun signedbyte i = emitbyte(if i<0 then 256+i else i)
fun emitword i = emitstring(eword i)
fun emitrword i = emitstring(erword i)
fun emitlong i = emitstring(elong i)

val mkjump = jump

fun emitdisp d =
	if d<=63 andalso d>= ~64
	    then ((if d>=0 then emitbyte(d)
		          else emitbyte(128+d));
		  1)
	    else if d<=8191 andalso d>= ~8192
		then ((if d>=0 then emitrword(128*256+d)
			      else emitrword(128*256+d+16384));
		      2)
	    else if d>=0
		then (emitrword(192*256+(d div 65536));
		      emitrword(d mod 65536);
		      4)
	    else let val a = ~d
		     val b = a mod 65536
		     val c = a div 65536
		  in emitrword(~c + if b=0 then 0 else ~1);
		     emitrword(~b);
		     4
		 end

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

(* This is identical to M68PrimReal except that emitword is different,
   and the bias is off by two. *)
(* This is broken, because it is really the VAX G-float rep, not
   the NS32 IEEE rep. *)
structure NS32PrimReal : PRIMREAL =
struct
val significant = 53 (* 52 + redundant 1/2 bit *)
fun outofrange s = ErrorMsg.complain("Real constant "^s^" out of range")
(* Convert a portion of a boolean array to the appropriate integer. *)
exception Bits
fun bits(a,start,width) =
    let fun b true = 1
	  | b false = 0
	fun f 0 = b (a sub start)
	  | f n = b (a sub (start+n)) + 2 * f(n-1)
    in  if Array.length a < start+width orelse start < 0 orelse width < 0
	then raise Bits
	else f (width-1)
    end
fun emitreal (sign,frac,exp) =
    let val exponent = exp + 1022
	fun emit () =
	    let val word0 =
	        case frac sub 0 of (* zero? *)
		  true => Bits.orb(Bits.lshift(sign,15),
			  	   Bits.orb(Bits.lshift(exponent,4),
				   	    bits(frac,1,4)))
		| false => 0
		val word1 = bits(frac,5,16)
		val word2 = bits(frac,21,16)
		val word3 = bits(frac,37,16)
	    in  emitword word3;
		emitword word2;
		emitword word1;
		emitword word0
	    end
    in  if exponent < 1 orelse exponent > 2047
	then outofrange "" (* A hack *)
	else emit()
    end
end
structure NS32RealConst = RealConst(NS32PrimReal)
open NS32RealConst

(* The label value, offset i, will be backpatched pc-relative. *)
fun emitlab (i,lab) = mkjump(LABPTR i, lab)

fun procreg (Direct FP) = 8
  | procreg (Direct SB) = 10
  | procreg _ = ErrorMsg.impossible "procreg in ns32mcode"

fun imsize k = if k<0 then 3
		else if k<128 then 0
		else 1

fun quick k = if k>=0 then k else k+16

fun gen (Direct(Genreg r)) = r
  | gen (Direct(FloatReg r)) = r
  | gen (Direct other) = ErrorMsg.impossible "gen direct in ns32mcode"
  | gen (Displace(d,Genreg r)) = r+8
  | gen (Displace(d,FP)) = 24
  | gen (Displace(d,SP)) = 25
  | gen (Displace(d,SB)) = 26
  | gen (Displace(d,PC)) = 27
  | gen (Displace(d,other)) = ErrorMsg.impossible "gen displace in ns32mcode"
  | gen (Topofstack) = 23
  | gen (Immed k) = 20
  | gen (Abs k) = 21
  | gen (Index(p,Genreg r,Byte)) = 28
  | gen (Index(p,Genreg r,Word)) = 29
  | gen (Index(p,Genreg r,Long)) = 30
  | gen (Index(p,other,sz)) = ErrorMsg.impossible "gen indexreg in ns32mcode"
(* Labels are always pc-relative, see jump widget in emitextn. *)
  | gen (OffAddress li) = 27 (* assuming all labels are done pc-relative *)
  | gen (Immedlab lab) = ErrorMsg.impossible "gen immedlab in ns32mcode"

fun emitimmed d =
	if d>=0
	    then (emitrword(d div 65536);
		  emitrword(d mod 65536))
	    else let val a = ~d
		     val b = a mod 25536
		     val c = a div 25536
		  in emitrword(~c + if b=0 then 0 else ~1);
		     emitrword(~b)
		 end


fun extnjumps (OffAddress _) = true
  | extnjumps (Index(OffAddress _,_,_)) = true
  | extnjumps _ = false

fun emitextnc (_,Direct _) = 0
  | emitextnc (_,Displace(d,_)) = emitdisp(d)
  | emitextnc (_,Topofstack) = 0
  | emitextnc (_,Immed k) = (emitimmed(k); 4)
  | emitextnc (_,Abs k) = emitdisp(k)
  | emitextnc (_,Index(Index _,_,_)) = ErrorMsg.impossible "illegal extn mode in ns32mcode"
  | emitextnc (off,Index(p,Genreg r,sz)) = (emitbyte(r+8*gen(p));
					     emitextnc(off+1,p)) + 1
  | emitextnc (_,Index(p,other,sz)) = ErrorMsg.impossible "illegal extn reg in ns32mcode" 
  | emitextnc (off,OffAddress(lab,i)) = (mkjump(LNGDISPL(i+off),lab); 4)
  | emitextnc (_,Immedlab lab) = ErrorMsg.impossible "immedlab extn in ns32mcode"  

fun emitextn (_,Direct _) = ()
  | emitextn (_,Displace(d,_)) = (emitdisp(d); ())
  | emitextn (_,Topofstack) = ()
  | emitextn (_,Immed k) = emitimmed(k)
  | emitextn (_,Abs k) = (emitdisp(k); ())
  | emitextn (_,Index(Index _,_,_)) = ErrorMsg.impossible "illegal extn mode in ns32mcode"
  | emitextn (off,Index(p,Genreg r,sz)) = (emitbyte(r+8*gen(p));
					     emitextn(off+1,p))
  | emitextn (_,Index(p,other,sz)) = ErrorMsg.impossible "illegal extn reg in ns32mcode" 
  | emitextn (off,OffAddress(lab,i)) = mkjump(DISPL(i+off),lab)
  | emitextn (_,Immedlab lab) = ErrorMsg.impossible "immedlab extn in ns32mcode"  

fun shortinstr(b,(src,dest)) =
	(emitword(b+64*gen(dest)+2048*gen(src));
	 if extnjumps(dest)
	     then let val off = emitextnc(2,src)
		   in emitextn(2+off,dest); ()
		  end
	     else (emitextn(2,src); emitextn(0,dest)))

fun longinstr(w,(src,dest)) =
	(let val gd = gen(dest)
	  in emitword(w+16384*(gd mod 4));
	     emitbyte((gd div 4) + 8*gen(src))
	 end;
	 if extnjumps(dest)
	     then let val off = emitextnc(3,src)
		   in emitextn(3+off,dest); ()
		  end
	     else (emitextn(3,src); emitextn(0,dest)))


fun lprl (src,preg) = (emitword(111+128*procreg(preg)+2048*gen(src));
		       emitextn(2,src); ())
fun sprl (preg,dest) = (emitword(47+128*procreg(preg)+2048*gen(dest));
			emitextn(2,dest); ())
fun tbit (off as Immed k,base) = shortinstr(55,(off,base))
  | tbit (off,base) = ErrorMsg.impossible "bad tbit in ns32mcode"

fun bfs (OffAddress(lab,i)) = (emitbyte(138);
			 mkjump(DISPL(1+i), lab))

fun movql (Immed k,dest) = (emitword(95+128*quick(k)+2048*gen(dest));
			    emitextn(2,dest); ())
  | movql (src,dest) = ErrorMsg.impossible "illegal movql in ns32mcode"

(* Size problem, must detect immediate. *)
fun movb (src as Immed k, dest) =
	(emitword(20+64*gen(dest)+2048*gen(src));
	 if k>127 orelse k< ~128
	     then ErrorMsg.impossible "illegal movb in ns32mcode"
	     else signedbyte(k);	(* force to byte size *)
	 emitextn(3,dest); ())
  | movb args = shortinstr(20,args)

(* Size problem, but will always be moving from mem, so ok. *)
fun movzbl args = longinstr(206+256*24,args)

fun lea args = shortinstr(39,args)

fun movl (Immedlab l, arg) = lea(address l, arg)
  | movl (args as (Immed k, dest)) = 
	if isquick(k)
	    then movql args
	    else shortinstr(23,args)
  | movl args = shortinstr(23,args)

(* Really addql for case Immed 1 *)
fun addl (args as (Immed k, dest)) =
	if isquick(k)
	    then (emitword(15+128*quick(k)+2048*gen(dest));
		  emitextn(2,dest); ())
	    else shortinstr(3,args)
  | addl args = shortinstr(3,args)
fun subl args = shortinstr(35,args)
fun negl args = longinstr(78+256*35,args)
(* Size problem, must force count to byte size *)
fun ashl (count as Immed k, dest) =
	(let val gd = gen(dest)
	  in emitword(78+256*7+16384*(gd mod 4));
	     emitbyte((gd div 4) + 8*gen(count))
	 end;
	 if k>127 orelse k< ~128
	     then ErrorMsg.impossible "illegal ashl in ns32mcode"
	     else signedbyte(k);	(* force to byte size *)
	 emitextn(4,dest); ())
  | ashl args = longinstr(78+256*7,args)
fun andl args = shortinstr(43,args)
fun orl args = shortinstr(27,args)
fun xorl args = shortinstr(59,args)
fun coml args = longinstr(78+256*55,args)
fun mull args = longinstr(128+78+256*35,args)
fun divl args = longinstr(128+78+256*63,args)

fun br (OffAddress(lab,i)) = (emitbyte(234);
			      mkjump(DISPL(1+i),lab))

fun jump (arg as OffAddress _) = br arg
  | jump arg = (emitword(127+512+2048*gen(arg));
		emitextn(2,arg); ())

fun bfs (OffAddress(lab,i)) = (emitbyte(138);
			       mkjump(DISPL(1+i), lab))

local fun condj(c) =
	fn (OffAddress(lab,i)) => (emitbyte(10+16*c);
				   mkjump(DISPL(1+i), lab))
	 | Displace(k, PC) => (emitbyte(10+16*c);
			       emitdisp(k); ())
 in val beq = condj(0)
    val bne = condj(1)
    val bge = condj(13)
    val bgt = condj(6)
    val blt = condj(12)
    val ble = condj(7)
end

fun cmpl (args as (Immed k,arg2)) =
	if isquick(k)
	    then (emitword(31+128*quick(k)+2048*gen(arg2));
		  emitextn(2, arg2); ())
	    else shortinstr(7,args)
  | cmpl args = shortinstr(7,args)

fun movg args = longinstr(190+256*4,args)
fun negg args = longinstr(190+256*20,args)
fun addg args = longinstr(190,args)
fun subg args = longinstr(190+256*16,args)
fun mulg args = longinstr(190+256*48,args)
fun divg args = longinstr(190+256*32,args)
fun cmpg args = longinstr(190+256*8,args)

fun comment _ = ()

end (* Coder *)

val finish = Emitter.finish

end (* structure MCode *)
