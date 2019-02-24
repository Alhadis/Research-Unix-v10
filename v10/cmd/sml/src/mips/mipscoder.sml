(* Copyright 1989 by AT&T Bell Laboratories *)
structure MipsCoder : MIPSCODER = struct

    type Label = int ref

  datatype Register = Reg of int

  datatype EA = Direct of Register
              | Immed of int
              | Immedlab of Label

  datatype size = Byte | Word | Floating
    datatype muldiv = MULT | DIV
  
  datatype instr = 
      STRINGCONST of string               (* constants *)
    | REALCONST of string
    | EMITLONG of int
  
    | DEFINE of Label                     (* labels *)
    | EMITLAB of int * Label
  
    | SLT of Register * EA * Register     (* control flow *)
    | BEQ of bool * Register * Register * Label
    | JUMP of Register 
    | SLT_D of Register * Register
    | SEQ_D of Register * Register
    | BCOP1 of bool * Label
  
    | NOP (* no-op for delay slot *)
  
    | ADD of Register * EA * Register     (* arithmetic *)
    | AND of Register * EA * Register
    | OR  of Register * EA * Register
    | XOR of Register * EA * Register
    | SUB of Register * Register * Register
    | MULDIV of muldiv * Register * Register
    | MFLO of Register    (* mflo instruction used with
                             64-bit multiply and divide *)
    | MFHI of Register
  
    | NEG_D of Register * Register
    | MUL_D of Register * Register * Register
    | DIV_D of Register * Register * Register
    | ADD_D of Register * Register * Register
    | SUB_D of Register * Register * Register
  
    | MOVE of EA * Register    (* put something into a register *)
    | LDI_32 of int * Register (* load in a big immediate constant (>16 bits) *)
    | LUI of Register * int    (* Mips lui instruction *)
  
    | LOAD of size * Register * EA * int  (* load and store *)
    | STORE  of size * Register * EA * int
  
    | SLL of EA * Register * Register     (* shift *)
    | SRA of EA * Register * Register
  
    | COMMENT of string                   (* generates nothing *)
    | MARK                                (* a backpointer *)

    val kept = ref nil : instr list ref
    fun keep f a = kept := f a :: !kept
    fun delay f a = kept := NOP :: f a :: !kept
    fun keeplist l = kept := l @ !kept
  
  structure M = struct
      val emitstring = keep STRINGCONST     (* literals *)
      val realconst = keep REALCONST
      val emitlong = keep EMITLONG
    
        fun newlabel () = ref 0
        val define = keep DEFINE
        val emitlab = keep EMITLAB                   (* labels *)
    
      val slt = keep SLT                    (* control flow *)
      val beq = delay BEQ
      val jump = delay JUMP
      val slt_double = delay SLT_D
      val seq_double = delay SEQ_D
      val bcop1 = delay BCOP1
    
      val add = keep ADD                    (* arithmetic *)
      val and' = keep AND
      val or = keep OR
      val xor = keep XOR
      val op sub = keep SUB
        fun muldiv f (a,b,c) = keeplist [MFLO c, MULDIV (f,a,b)]
        val op div = muldiv DIV
        val mult = muldiv MULT
        val mfhi = keep MFHI
    
      val neg_double = keep NEG_D
      val mul_double = keep MUL_D
      val div_double = keep DIV_D
      val add_double = keep ADD_D
      val sub_double = keep SUB_D
    
      val move = keep MOVE
    
      fun lbu (a,b,c) = delay LOAD (Byte,a,b,c) (* load and store *)
      fun lw (a,b,c)  = delay LOAD (Word,a,b,c)
      fun lwc1 (a,b,c)  = delay LOAD (Floating,a,b,c)
      fun sb (a,b,c)  = keep STORE (Byte,a,b,c)
      fun sw (a,b,c)  = keep STORE (Word,a,b,c)
      fun swc1 (a,b,c)  = delay STORE (Floating,a,b,c)
      val lui = keep LUI
    
      val sll = keep SLL                    (* shift *)
      val sra = keep SRA
    
      fun align() = ()                      (* never need to align on MIPS *)
      val mark = keep (fn () => MARK)
      val comment = keep COMMENT
  end

  open M

  fun get (SOME x) = x 
    | get NONE = ErrorMsg.impossible "missing pcptr in mipscoder"
  
  fun needs_a_pcptr(_,SLT(_,Immedlab _,_)) = true
    | needs_a_pcptr(_,ADD(_,Immedlab _,_)) = true
    | needs_a_pcptr(_,AND(_,Immedlab _,_)) = true
    | needs_a_pcptr(_,OR(_,Immedlab _,_)) = true
    | needs_a_pcptr(_,XOR(_,Immedlab _,_)) = true
    | needs_a_pcptr(_,MOVE(Immedlab _,_)) = true
    | needs_a_pcptr(_,LOAD(_,_,Immedlab _,_)) = true
    | needs_a_pcptr(_,STORE(_,_,Immedlab _,_)) = true
    | needs_a_pcptr(_,SLL(Immedlab _,_,_)) = true
    | needs_a_pcptr(_,SRA(Immedlab _,_,_)) = true
    | needs_a_pcptr(1, BEQ _) = false  (* small BEQ's dont need pcptr *)
    | needs_a_pcptr(_, BEQ _) = true   (* but large ones do *)
    | needs_a_pcptr(1, BCOP1 _) = false  (* small BCOP1's dont need pcptr *)
    | needs_a_pcptr(_, BCOP1 _) = true   (* but large ones do *)
    | needs_a_pcptr _ = false
  fun pass emitters =
  let fun makepcptr(i,x) = 
           (* may need to emit NOP for delay slot if next instr is branch *)
    let val size = case x of ((_,BEQ _)::rest) => 2 
  			 | ((_,BCOP1 _)::rest) => 2 
  			 | _ => 1
    in  case emitters of NONE                     => () 
                       | SOME (emit, _, _ ) => (
  			      emit(Opcodes.bltzal(0,0));
  			      if size=2 then emit(Opcodes.add(0,0,0)) else ());
        gen(i+size, SOME (i+2), x)
    end
  and gen(i,_,nil) = i
    | gen(i, _, (_,DEFINE lab) :: rest) = (lab := i; gen(i,NONE, rest))
                          (* invalidate the pc pointer at labels *)
    (* may want to do special fiddling with NOPs *)
    | gen(pos, pcptr, x as ((sizeref as ref size, inst) :: rest)) =
         if (pcptr=NONE andalso needs_a_pcptr(size, inst)) then makepcptr(pos,x)
         else case emitters of
            SOME (emit : int*int -> unit, emit_string : int -> string -> unit,
  	        emit_real : string -> unit) =>
               let fun gen1() = gen(pos+size,pcptr,rest) 
	                                       (* generate the rest of the [[instr]]s *)
	           open Bits
	           open Opcodes
	           val tempreg = 1
		   val pcreg = 31
	           fun emitlong i = emit(rshift(i,16), andb(i,65535))
		                                   (* emit one long word (no sign fiddling) *)
		   fun split i = let val hi = rshift(i,16) and lo = andb(i,65535)
		                        in if lo<32768 then (hi,lo) else (hi+1, lo-65536)
		                       end
		   
		   fun arith (opr, rform, iform) =
		      let fun ar (Reg op1, Direct (Reg op2), Reg result) = 
		                   gen1(emit(rform(result,op1,op2)))
		            | ar (Reg op1, Immed op2, Reg result) =
		                   (case size of
		                       1 (* 16 bits *) => gen1(emit(iform(result,op1,op2)))
		                     | 3 (* 32 bits *) => 
		                        gen(pos,pcptr,
		                             (ref 2, LDI_32(op2, Reg tempreg))::
		                             (ref 1, opr(Reg op1, Direct(Reg tempreg), Reg result))::
		                             rest)
		                     | _ => gen(ErrorMsg.impossible 
		                                   "bad size in arith Immed in mipscoder")
		                   )
		            | ar (Reg op1, Immedlab (ref op2), Reg result) =
		                   gen(pos, pcptr, 
		                         (ref (size-1), 
		                               ADD(Reg pcreg,Immed(4*(op2-(get pcptr))), Reg tempreg))::
		                         (ref 1, opr(Reg op1, Direct(Reg tempreg), Reg result))::
		                         rest)
		      in  ar
		      end
		   fun float3double instruction (Reg op1,Reg op2,Reg result) =
		      gen1(emit(instruction(D_fmt,result,op1,op2)))
		   fun domove (Direct (Reg src), Reg dest) = gen1(emit(add(dest,src,0)))
		     | domove (Immed src, Reg dest) =
		           (case size of
		               1 (* 16 bits *) => gen1(emit(addi(dest,0,src)))
		             | 2 (* 32 bits *) => 
		   			gen(pos,pcptr,(ref 2, LDI_32(src, Reg dest))::rest)
		             | _ => gen(ErrorMsg.impossible "bad size in domove Immed in mipscoder")
		           )
		     | domove (Immedlab (ref src), Reg dest) =
		           gen(pos, pcptr, 
		                 (ref size, 
		                       ADD(Reg pcreg,Immed(4*(src-(get pcptr))), Reg dest))::rest)
		   fun memop(rform,Reg dest, Direct (Reg base), offset) =
		         (case size
		          of 1 => gen1(emit(rform(dest,offset,base)))
		           | 3 => let val (hi,lo) = split offset
		                  in  gen1(emit(lui(tempreg,hi));       (* tempreg = hi << 16 *)
		                           emit(add(tempreg,base,tempreg));(* tempreg += base *)
		                           emit(rform(dest,lo,tempreg)) (* load dest,lo(tempreg) *)
		                          )
		                  end
		           | _ => gen1(ErrorMsg.impossible "bad size in memop Direct in mipscoder")
		          )
		     | memop(rform,Reg dest, Immed address, offset) =
		         (case size
		          of 1 => gen1(emit(rform(dest,offset+address,0)))
		           | 2 => let val (hi,lo) = split (offset+address)
		                  in  gen1(emit(lui(tempreg,hi)); 
		                           emit(rform(dest,lo,tempreg))
		                          )
		                  end
		           | _ => gen1(ErrorMsg.impossible "bad size in memop Immed in mipscoder")
		          )
		     | memop(rform,Reg dest, Immedlab (ref lab), offset) =
		         memop(rform, Reg dest, Direct (Reg pcreg), offset+4*(lab - get pcptr))
	       in  case inst of
	             STRINGCONST s => 
		           let val s' = s ^ "\000\000\000\000"
		           in  gen1(emit_string (4*size) s')
		                                         (* doesn't know Big vs Little-Endian *)
		           end
		   | REALCONST s => gen1(emit_real s)  (* floating pt constant *)
		   | EMITLONG i => gen1(emitlong i)
		   | DEFINE _ => gen1(ErrorMsg.impossible "generate code for DEFINE in mipscoder")
		   | EMITLAB(i, ref d) => gen1(emitlong((d-pos)*4+i))
		   | ADD stuff => arith (ADD,add,addi) stuff
		   | AND stuff => arith (AND,and',andi) stuff
		   | OR  stuff => arith (OR,or,ori) stuff
		   | XOR stuff => arith (XOR,xor,xori) stuff
		   | SUB (Reg op1, Reg op2, Reg result) => gen1(emit(sub(result,op1,op2)))
		   | MULDIV(DIV, Reg op1, Reg op2) => gen1(emit(div(op1,op2)))
		   | MULDIV(MULT,Reg op1, Reg op2) => gen1(emit(mult(op1,op2)))
		   | MFLO(Reg result) => gen1(emit(mflo(result)))
		   | MFHI(Reg result) => gen1(emit(mfhi(result)))
		   | NEG_D (Reg op1,Reg result) => gen1(emit(neg_fmt(D_fmt,result,op1)))
		   | MUL_D x => float3double mul_fmt x
		   | DIV_D x => float3double div_fmt x
		   | ADD_D x => float3double add_fmt x
		   | SUB_D x => float3double sub_fmt x
		   
		   
		   | MOVE stuff => domove stuff
		   | LDI_32 (immedconst, Reg dest) =>
		            let val (hi,lo) = split immedconst
		            in  gen1(emit(lui(dest,hi));emit(addi(dest,dest,lo)))
		            end 
		   | LUI (Reg dest,immed16) => gen1(emit(lui(dest,immed16)))
		   
		   | SLT stuff => arith (SLT,slt,slti) stuff
		   | BEQ(b, Reg op1, Reg op2, ref dest) =>
		       if size = 1 then 
		            gen1(emit((if b then beq else bne)(op1,op2,dest-(pos+1))))
		       else gen(pos,pcptr,
		                     (ref 1, BEQ(not b, Reg op1, Reg op2, ref(pos+size)))
		                     ::(ref (size-2), 
		                         ADD(Reg pcreg, Immed(4*(dest-(get pcptr))), Reg tempreg))
		                     ::(ref 1, JUMP(Reg tempreg))
		                     ::rest)
		   | JUMP(Reg dest) => gen1(emit(jr(dest)))
		   | SLT_D (Reg op1, Reg op2) => 
		        gen1(emit(c_lt(D_fmt,op1,op2)))
		   | SEQ_D (Reg op1, Reg op2) => 
		        gen1(emit(c_seq(D_fmt,op1,op2)))
		   | BCOP1(b, ref dest) =>
		       let fun bc1f offset = cop1(8,0,offset)
		   	fun bc1t offset = cop1(8,1,offset)
		       in  if size = 1 then 
		                gen1(emit((if b then bc1t else bc1f)(dest-(pos+1))))
		           else gen(pos,pcptr,
		                     (ref 1, BCOP1(not b, ref(pos+size)))
		                     ::(ref (size-2), 
		                         ADD(Reg pcreg, Immed(4*(dest-(get pcptr))), Reg tempreg))
		                     ::(ref 1, JUMP(Reg tempreg))
		                     ::rest)
		       end
		   | NOP => gen1(emit(add(0,0,0)))         (* one of the many MIPS no-ops *)
		   | LOAD  (Byte,dest,address,offset) => memop(lbu,dest,address,offset)
		   | LOAD  (Word,dest,address,offset) => memop(lw,dest,address,offset)
		   | LOAD  (Floating,dest,address,offset) => memop(lwc1,dest,address,offset)
		   | STORE (Byte,dest,address,offset) => memop(sb,dest,address,offset)
		   | STORE (Word,dest,address,offset) => memop(sw,dest,address,offset)
		   | STORE (Floating,dest,address,offset) => memop(swc1,dest,address,offset)
		   | SLL (Immed shamt, Reg op1, Reg result) => gen1(
		           if (shamt >= 0 andalso shamt < 32) then emit(sll(result,op1,shamt))
		           else ErrorMsg.impossible ("bad sll shamt "
		   		^ (Integer.makestring shamt) ^ " in mipscoder"))
		   | SLL (Direct(Reg shamt), Reg op1, Reg result) => 
		           gen1(emit(sllv(result,op1,shamt)))
		   | SLL (Immedlab _,_,_) => ErrorMsg.impossible "sll shamt is Immedlab in mipscoder"
		   | SRA (Immed shamt, Reg op1, Reg result) => gen1(
		           if (shamt >= 0 andalso shamt < 32) then emit(sra(result,op1,shamt))
		           else ErrorMsg.impossible ("bad sra shamt "
		   		^ (Integer.makestring shamt) ^ " in mipscoder"))
		   | SRA (Direct(Reg shamt), Reg op1, Reg result) =>
		           gen1(emit(srav(result,op1,shamt)))
		   | SRA (Immedlab _,_,_) => ErrorMsg.impossible "sra shamt is Immedlab in mipscoder"
		   | COMMENT _ => gen1()
		   | MARK => gen1(
		       let open System.Tags
		       in  emitlong((pos+1) * power_tags + tag_backptr)
		       end)
	       end
          | NONE =>
               let fun easize (Direct _) = 1
		     | easize (Immed i) = if abs(i)<32768 then 1 else 3
		     | easize (Immedlab(ref lab)) = 1 + easize(Immed (4*(lab-(get pcptr))))
		   fun movesize (Direct _) = 1
		     | movesize (Immed i) = if abs(i)<32768 then 1 else 2
		     | movesize (Immedlab(ref lab)) = easize(Immed (4*(lab-(get pcptr))))
		   
		   fun adrsize(_, Reg _, Direct _, offset) = 
		               if abs(offset)<32768 then 1 else 3
		     | adrsize(_, Reg _, Immed address, offset) = 
		               if abs(address+offset) < 32768 then 1 else 2
		     | adrsize(x, Reg dest, Immedlab (ref lab), offset) =
		               adrsize(x, Reg dest, Direct (Reg 0  (* pcreg in code *) ), 
		                       offset+4*(lab-(get pcptr)))
	           val newsize = case inst of
	                 STRINGCONST s => Integer.div(String.length(s)+3,4)
		       | REALCONST _ => 2
		       | EMITLONG _ => 1
		       | DEFINE _ => ErrorMsg.impossible "generate code for DEFINE in mipscoder"
		       | EMITLAB _ => 1
		       | ADD(_, ea, _) => easize ea
		       | AND(_, ea, _) => easize ea
		       | OR (_, ea, _) => easize ea
		       | XOR(_, ea, _) => easize ea
		       | SUB _ => 1
		       | MULDIV _ => 1 
		       | MFLO _ => 1
		       | MFHI _ => 1
		       | NEG_D _ => 1
		       | MUL_D _ => 1
		       | DIV_D _ => 1
		       | ADD_D _ => 1
		       | SUB_D _ => 1
		       | MOVE (src,_) => movesize src
		       | LDI_32 _ => 2
		       | LUI _ => 1
		       | SLT(_, ea, _) => easize ea
		       | BEQ(_,_,_,ref dest) => 
		               if abs((pos+1)-dest) < 32768 then 1 (* single instruction *)
		               else 2+easize (Immed (4*(dest-(get pcptr))))
		       | JUMP _ => 1
		       | SLT_D _ => 1
		       | SEQ_D _ => 1
		       | BCOP1(_,ref dest) => 
		               if abs((pos+1)-dest) < 32768 then 1 (* single instruction *)
		               else 2+easize (Immed (4*(dest-(get pcptr))))
		       | NOP => 1
		       | LOAD  x => adrsize x
		       | STORE x => adrsize x
		       | SLL _ => 1  
		       | SRA _ => 1
		       | COMMENT _ => 0
		       | MARK => 1                     (* backpointer takes one word *)
	       in  if newsize > size then sizeref := newsize else ();
	           gen(pos+(!sizeref) (* BUGS -- was pos+size*),pcptr,rest)
	       end
  in  gen
  end
  
  fun prepare instrs =
   let fun add_positions(done, inst::rest) =  
                   add_positions( (ref 0, inst) :: done, rest)
         | add_positions(done, nil) = done
  
       val instrs' = add_positions(nil, instrs) (* reverse and add [[ref int]]s*)
  
       fun passes(oldsize) = 
                  (* make passes with no emission until size is stable*)
          let val size = pass NONE (0,NONE,instrs')
          in  if size=oldsize then size
              else passes size
          end
    in {size = passes 0, stream = instrs'}
    end
  
  fun assemble emitters instrs =
          pass (SOME emitters) (0,NONE,#stream (prepare instrs))
  
  
  fun codegen emitters = (
      assemble emitters (!kept);
        kept := nil
      )

  fun printstats stream
   {inst : int, code : int, data : int, 
    load : int, branch : int, compare : int, size : int} =
      let val print = output stream
  	val nop = load+branch+compare
  	val bltzal = size - (code + data)
  	val code = code + bltzal
  	val I = Integer.makestring
	  val R = Real.makestring
	  exception Printf
	  fun sprintf format values =
	      let fun merge([x],nil) = [x]
	            | merge(nil,nil) = nil
	            | merge(x::y,z::w) = x::z:: merge(y,w)
	            | merge _ = raise Printf
	      in  implode(merge(format,values))
	      end
  
  	fun P x = substring(makestring(100.0 * x),0,4)  (* percent *)
  	fun printf f d = print (sprintf f d)
      in  printf ["Counted "," instrs in "," words (",
  				" code, "," data)\n" ^
  		"Used "," NOPs ("," load, "," branch,"," compare) and "," bltzals\n" ^
  		"","% of code words were NOPs; ","% were bltzals\n" ^
  		"","% of all words were code; ","% of all words were NOPs\n"]
  	       [I inst, I size, I code, I data, 
                  I nop, I load, I branch,  I compare, I bltzal,
  		P (real nop / real code), P (real bltzal / real code),
  		P (real code / real size), P (real nop / real size)]
  	handle Overflow => print "[Overflow in computing Mips stats]\n"
  	     | Real s => print ("[FPE ("^s^") in computing Mips stats]\n")
      end
  		
  val iscode = fn
      STRINGCONST _ => false
    | REALCONST _ => false
    | EMITLONG _ => false
    | DEFINE _ => false
    | EMITLAB _ => false
  
    | SLT _ => true
    | BEQ _ => true
    | JUMP _ => true
    | NOP => true
    | SLT_D _ => true
    | SEQ_D _ => true
    | BCOP1 _ => true
  
    | ADD _ => true
    | AND _ => true
    | OR  _ => true
    | XOR _ => true
    | SUB _ => true
    | MULDIV _ => true
    | MFLO _ => true
    | MFHI _ => true
  
    | NEG_D _ => true
    | MUL_D _ => true
    | DIV_D _ => true
    | ADD_D _ => true
    | SUB_D _ => true
  
    | MOVE _ => true
    | LDI_32 _ => true
    | LUI _ => true
  
    | LOAD _ => true
    | STORE  _ => true
  
    | SLL _ => true
    | SRA _ => true
  
    | COMMENT _ => false
    | MARK => false
  
  fun addstats (counts as {inst,code,data,load,branch,compare}) =
    fn nil => counts
     | (sizeref,first)::(_,NOP)::rest => addstats
            {inst=inst+2, code=code+(!sizeref)+1, data=data,
             load=load+ (case first of LOAD _ => 1 | _ => 0),
             branch=branch +(case first of BEQ _ => 1 | JUMP _ => 1 
  				       | BCOP1 _ => 1 | _ => 0),
  	   compare=compare+(case first of SLT_D _ => 1 | SEQ_D _ => 1 
  					| _ => 0)
            } rest
     | (sizeref,first)::rest => addstats
            {inst=inst+1, 
             code = code + if iscode(first) then !sizeref else 0,
             data = data + if not (iscode first) then !sizeref else 0,
             load=load,
             branch=branch,
  	   compare=compare
            } rest
  
  
  fun codestats outfile =
      let val {size,stream=instrs} = prepare (!kept)
  	val zero = {inst=0, code=0, data=0, load=0, branch=0, compare=0}
          val counts as {inst,code,data,load,branch,compare} = 
  						addstats zero instrs
      in  printstats outfile 
  	    {inst=inst,code=code,data=data,
  	     load=load,branch=branch,compare=compare,size=size}
      end
  	

end (* MipsInstr *)
