(* Copyright 1989 by AT&T Bell Laboratories *)
structure MipsDecode = struct
val andb = Bits.andb
fun rshift(op1,amt) = 
    if amt<0 then Bits.lshift(op1,0-amt)
    else Bits.rshift(op1,amt)
fun THEop'(A1,A2) = 0 + rshift(andb(A1,65535),10)
fun THErs(A1,A2) = 0 + rshift(andb(A1,1023),5)
fun THErt(A1,A2) = 0 + rshift(andb(A1,31),0)
fun THEimmed(A1,A2) = let val n = andb(rshift(A2,0),65535) + 0
in if n < 32768 then n else n - 65536
end
fun THEoffset(A1,A2) = let val n = andb(rshift(A2,0),65535) + 0
in if n < 32768 then n else n - 65536
end
fun THEbase(A1,A2) = 0 + rshift(andb(A1,1023),5)
fun THEtarget(A1,A2) = andb(rshift(A2,0),65535) + rshift(andb(A1,1023),~16)
fun THErd(A1,A2) = andb(rshift(A2,11),31) + 0
fun THEshamt(A1,A2) = andb(rshift(A2,6),31) + 0
fun THEfunct(A1,A2) = andb(rshift(A2,0),63) + 0
fun THEcond(A1,A2) = 0 + rshift(andb(A1,31),0)
fun THEft(A1,A2) = 0 + rshift(andb(A1,31),0)
fun THEfmt(A1,A2) = 0 + rshift(andb(A1,1023),5)
fun THEfs(A1,A2) = andb(rshift(A2,11),31) + 0
fun THEfd(A1,A2) = andb(rshift(A2,6),31) + 0
fun decode(A1,A2) = let
val rt = THErt(A1,A2)
val Srt = Integer.makestring rt
val base = THEbase(A1,A2)
val Sbase = Integer.makestring base
val immed = THEimmed(A1,A2)
val Simmed = Integer.makestring immed
val target = THEtarget(A1,A2)
val Starget = Integer.makestring target
val fd = THEfd(A1,A2)
val Sfd = Integer.makestring fd
val fmt = THEfmt(A1,A2)
val Sfmt = Integer.makestring fmt
val fs = THEfs(A1,A2)
val Sfs = Integer.makestring fs
val ft = THEft(A1,A2)
val Sft = Integer.makestring ft
val op' = THEop'(A1,A2)
val Sop' = Integer.makestring op'
val funct = THEfunct(A1,A2)
val Sfunct = Integer.makestring funct
val rd = THErd(A1,A2)
val Srd = Integer.makestring rd
val cond = THEcond(A1,A2)
val Scond = Integer.makestring cond
val shamt = THEshamt(A1,A2)
val Sshamt = Integer.makestring shamt
val offset = THEoffset(A1,A2)
val Soffset = Integer.makestring offset
val rs = THErs(A1,A2)
val Srs = Integer.makestring rs
val do_special =
(case funct of
   0 => "sll " ^ "rd = " ^ Srd ^ ","  ^ "rt = " ^ Srt ^ ","  ^ "shamt = " ^ Sshamt ^ "\n"
 | 2 => "srl " ^ "rd = " ^ Srd ^ ","  ^ "rt = " ^ Srt ^ ","  ^ "shamt = " ^ Sshamt ^ "\n"
 | 3 => "sra " ^ "rd = " ^ Srd ^ ","  ^ "rt = " ^ Srt ^ ","  ^ "shamt = " ^ Sshamt ^ "\n"
 | 4 => "sllv " ^ "rd = " ^ Srd ^ ","  ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ "\n"
 | 6 => "srlv " ^ "rd = " ^ Srd ^ ","  ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ "\n"
 | 7 => "srav " ^ "rd = " ^ Srd ^ ","  ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ "\n"
 | 8 => "jr " ^ "rs = " ^ Srs ^ "\n"
 | 9 => "jalr " ^ "rs = " ^ Srs ^ ","  ^ "rd = " ^ Srd ^ "\n"
 | 12 => "syscall " ^ "\n"
 | 13 => "break " ^ "\n"
 | 16 => "mfhi " ^ "rd = " ^ Srd ^ "\n"
 | 17 => "mthi " ^ "rs = " ^ Srs ^ "\n"
 | 18 => "mflo " ^ "rd = " ^ Srd ^ "\n"
 | 19 => "mtlo " ^ "rs = " ^ Srs ^ "\n"
 | 24 => "mult " ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 25 => "multu " ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 26 => "div " ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 27 => "divu " ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 32 => "add " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 33 => "addu " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 34 => "sub " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 35 => "subu " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 36 => "and' " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 37 => "or " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 38 => "xor " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 39 => "nor " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 42 => "slt " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | 43 => "sltu " ^ "rd = " ^ Srd ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ "\n"
 | _ => "unknown special\n"
   ) 
val do_bcond =
(case cond of
   0 => "bltz " ^ "rs = " ^ Srs ^ ","  ^ "offset = " ^ Soffset ^ "\n"
 | 1 => "bgez " ^ "rs = " ^ Srs ^ ","  ^ "offset = " ^ Soffset ^ "\n"
 | 16 => "bltzal " ^ "rs = " ^ Srs ^ ","  ^ "offset = " ^ Soffset ^ "\n"
 | 17 => "bgezal " ^ "rs = " ^ Srs ^ ","  ^ "offset = " ^ Soffset ^ "\n"
 | _ => "unknown bcond\n"
   ) 
val do_cop1 =
(case funct of
   0 => "add_fmt " ^ "fmt = " ^ Sfmt ^ ","  ^ "fd = " ^ Sfd ^ ","  ^ "fs = " ^ Sfs ^ ","  ^ "ft = " ^ Sft ^ "\n"
 | 1 => "sub_fmt " ^ "fmt = " ^ Sfmt ^ ","  ^ "fd = " ^ Sfd ^ ","  ^ "fs = " ^ Sfs ^ ","  ^ "ft = " ^ Sft ^ "\n"
 | 2 => "mul_fmt " ^ "fmt = " ^ Sfmt ^ ","  ^ "fd = " ^ Sfd ^ ","  ^ "fs = " ^ Sfs ^ ","  ^ "ft = " ^ Sft ^ "\n"
 | 3 => "div_fmt " ^ "fmt = " ^ Sfmt ^ ","  ^ "fd = " ^ Sfd ^ ","  ^ "fs = " ^ Sfs ^ ","  ^ "ft = " ^ Sft ^ "\n"
 | 5 => "abs_fmt(??? unknown format???)\n"
 | 6 => "mov_fmt(??? unknown format???)\n"
 | 7 => "neg_fmt " ^ "fmt = " ^ Sfmt ^ ","  ^ "fd = " ^ Sfd ^ ","  ^ "fs = " ^ Sfs ^ "\n"
 | 32 => "cvt_s(??? unknown format???)\n"
 | 33 => "cvt_d(??? unknown format???)\n"
 | 36 => "cvt_w(??? unknown format???)\n"
 | 48 => "c_f(??? unknown format???)\n"
 | 49 => "c_un(??? unknown format???)\n"
 | 50 => "c_eq(??? unknown format???)\n"
 | 51 => "c_ueq(??? unknown format???)\n"
 | 52 => "c_olt(??? unknown format???)\n"
 | 53 => "c_ult(??? unknown format???)\n"
 | 54 => "c_ole(??? unknown format???)\n"
 | 55 => "c_ule(??? unknown format???)\n"
 | 56 => "c_sf(??? unknown format???)\n"
 | 57 => "c_ngle(??? unknown format???)\n"
 | 58 => "c_seq " ^ "fmt = " ^ Sfmt ^ ","  ^ "fs = " ^ Sfs ^ ","  ^ "ft = " ^ Sft ^ "\n"
 | 59 => "c_ngl(??? unknown format???)\n"
 | 60 => "c_lt " ^ "fmt = " ^ Sfmt ^ ","  ^ "fs = " ^ Sfs ^ ","  ^ "ft = " ^ Sft ^ "\n"
 | 61 => "c_nge(??? unknown format???)\n"
 | 62 => "c_le(??? unknown format???)\n"
 | 63 => "c_ngt(??? unknown format???)\n"
 | _ => "unknown cop1\n"
   ) 
in
(case op' of
   0 => do_special
 | 1 => do_bcond
 | 2 => "j " ^ "target = " ^ Starget ^ "\n"
 | 3 => "jal " ^ "target = " ^ Starget ^ "\n"
 | 4 => "beq " ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ "\n"
 | 5 => "bne " ^ "rs = " ^ Srs ^ ","  ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ "\n"
 | 6 => "blez " ^ "rs = " ^ Srs ^ ","  ^ "offset = " ^ Soffset ^ "\n"
 | 7 => "bgtz " ^ "rs = " ^ Srs ^ ","  ^ "offset = " ^ Soffset ^ "\n"
 | 8 => "addi " ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "immed = " ^ Simmed ^ "\n"
 | 9 => "addiu " ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "immed = " ^ Simmed ^ "\n"
 | 10 => "slti " ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "immed = " ^ Simmed ^ "\n"
 | 11 => "sltiu " ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "immed = " ^ Simmed ^ "\n"
 | 12 => "andi " ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "immed = " ^ Simmed ^ "\n"
 | 13 => "ori " ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "immed = " ^ Simmed ^ "\n"
 | 14 => "xori " ^ "rt = " ^ Srt ^ ","  ^ "rs = " ^ Srs ^ ","  ^ "immed = " ^ Simmed ^ "\n"
 | 15 => "lui " ^ "rt = " ^ Srt ^ ","  ^ "immed = " ^ Simmed ^ "\n"
 | 16 => "cop0(??? unknown format???)\n"
 | 17 => do_cop1
 | 18 => "cop2(??? unknown format???)\n"
 | 19 => "cop3(??? unknown format???)\n"
 | 32 => "lb " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 33 => "lh " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 34 => "lwl " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 35 => "lw " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 36 => "lbu " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 37 => "lhu " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 38 => "lwr " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 40 => "sb " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 41 => "sh " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 42 => "swl " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 43 => "sw " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 46 => "swr " ^ "rt = " ^ Srt ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 48 => "lwc0(??? unknown format???)\n"
 | 49 => "lwc1 " ^ "ft = " ^ Sft ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 50 => "lwc2(??? unknown format???)\n"
 | 51 => "lwc3(??? unknown format???)\n"
 | 56 => "swc0(??? unknown format???)\n"
 | 57 => "swc1 " ^ "ft = " ^ Sft ^ ","  ^ "offset = " ^ Soffset ^ ","  ^ "base = " ^ Sbase ^ "\n"
 | 58 => "swc2(??? unknown format???)\n"
 | 59 => "swc3(??? unknown format???)\n"
 | _ => "unknown opcode\n"
   ) 
end
end (* Decode *)
