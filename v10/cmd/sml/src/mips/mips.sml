(* Copyright 1989 by AT&T Bell Laboratories *)
functor MipsCM(MipsC : MIPSCODER) : CMACHINE = struct

    open MipsC System.Tags

    fun three f (Direct x, ea, Direct y) = f(x,ea,y)
      | three f (ea, Direct x, Direct y) = f(x,ea,y)
      | three f _ =ErrorMsg.impossible "neither arg to three f is register in mips"


    val immed = Immed
    fun isimmed(Immed i) = SOME i
      | isimmed _ = NONE
    
    fun isreg(Direct(Reg i)) = SOME i | isreg _ = NONE
    fun eqreg (a: EA) b = a=b
    


    val standardarg = Direct(Reg 2)
    val standardcont = Direct(Reg 3)
    val standardclosure = Direct(Reg 4)
    val miscregs = map (Direct o Reg) [5,6,7,8,9,10,11,12,13,14,
                                       15,16,17,18,19]
    val storeptr as Direct storeptr' = Direct(Reg 22)
    val dataptr  as Direct dataptr'  = Direct(Reg 23)
    val exnptr = Direct(Reg 30)
    
      (* internal use only *)
    val my_arithtemp as Direct my_arithtemp'= Direct(Reg 20) 
    val my_ptrtemp as Direct my_ptrtemp' = Direct(Reg 21)
    
      (* exported for external use *)
    val arithtemp as Direct arithtemp' = Direct(Reg 24) 
    val arithtemp2 as Direct arithtemp2'= Direct(Reg 25)


    fun move (src,Direct dest) = M.move(src, dest)
      | move _ = ErrorMsg.impossible "destination of move not register in mips"
    val align = M.align
    val mark = M.mark
    
    val emitlong = M.emitlong
    val realconst = M.realconst
    val emitstring = M.emitstring

    fun emitlab(i,Immedlab lab) = M.emitlab(i,lab)
      | emitlab _ = ErrorMsg.impossible "bad emitlab arg in mips"
    fun newlabel() = Immedlab(M.newlabel())
    fun define (Immedlab lab) = M.define lab
      | define _ = ErrorMsg.impossible "bad define arg in mips"
    fun record(vl, Direct z) =
        let open CPS
            val len = List.length vl
            fun f(i,nil) = ()
              | f(i,(r, SELp(j,p))::rest) = (* follow ptrs to get the item *)
                    (M.lw(my_ptrtemp', r, j*4); f(i,(my_ptrtemp,p)::rest))
              | f(i,(Direct r,OFFp 0)::rest) =  (* simple store, last first *) 
                    (M.sw(r, dataptr, i*4); f(i-1,rest))
              | f(i,(Direct r, OFFp j)::rest) = 
                    (M.add(r, Immed(4*j), my_ptrtemp'); 
                                    f(i,(my_ptrtemp,OFFp 0)::rest))
              | f(i,(ea,p)::rest) = (* convert to register-based *)
                    (M.move(ea, my_ptrtemp'); f(i,(my_ptrtemp,p)::rest))
          in f(len - 1, rev vl); (* store first word in [[0(dataptr')]] *)
             M.add(dataptr', Immed 4, z);
             M.add(dataptr', Immed(4*len), dataptr')
         end
       | record _ = ErrorMsg.impossible "result of record not register in mips"
    
    fun select(i, r, Direct s) = M.lw(s, r, i*4)
      | select _ = ErrorMsg.impossible "result of select not register in mips"
    
    fun offset(i, Direct r, Direct s) = M.add(r,Immed(i*4), s)
      | offset _ = ErrorMsg.impossible "nonregister arg to offset in mips"
    (* fetchindexb(x,y) fetches a byte: y <- mem[x+arithtemp]
            y cannot be arithtemp *)
    fun fetchindexb(x,Direct y) =
        (M.add(arithtemp',x,my_arithtemp');    
         M.lbu(y,my_arithtemp,0))
      | fetchindexb _ = ErrorMsg.impossible "fetchb result not register in mips"
    
    (* storeindexb(x,y) stores a byte: mem[y+arithtemp] <- x; *)
    fun storeindexb(Direct x,y) =
        (M.add(arithtemp',y,my_arithtemp');
         M.sb(x,my_arithtemp,0))
      | storeindexb _ = ErrorMsg.impossible "storeb arg not register in mips"
    
    (* jmpindexb(x)    pc <- (x+arithtemp) *)
    fun jmpindexb x = (M.add(arithtemp',x,my_arithtemp');
                         M.jump(my_arithtemp'))

       (* fetchindexl(x,y,z) fetches a word:   y <- mem[x+2*(z-1)] *)
       (* storeindexl(x,y,z) stores a word:    mem[y+2*(z-1)] <- x *)
    
    fun fetchindexl(x,Direct y, Direct z) = 
          (M.sll(Immed 1,z,my_arithtemp');
           M.add(my_arithtemp',x,my_arithtemp');
           M.lw(y, my_arithtemp,~2))
      | fetchindexl(x,Direct y, Immed z) = M.lw(y, x, z+z-2)
      | fetchindexl _ = ErrorMsg.impossible "fetchl result not register in mips"
    
    fun storeindexl(Direct x,y, Immed 1) = M.sw(x,y,0)
      | storeindexl(Direct x,y,Direct z) = 
        (M.sll(Immed 1,z,my_arithtemp');
         M.add(my_arithtemp',y,my_arithtemp');
         M.sw(x, my_arithtemp,~2))
      | storeindexl(Direct x,y,Immed z) = M.sw(x,y,z+z-2)
    
      | storeindexl(Direct _,_,Immedlab _) =
    	ErrorMsg.impossible "storeindexl(Direct _,_,Immedlab _) in mips"
    
      | storeindexl(Immedlab label,y,z) =
        (M.move(Immedlab label,my_ptrtemp');
         storeindexl(my_ptrtemp,y,z))
    
      | storeindexl(Immed constant,y,offset) =
    	(M.move(Immed constant,my_ptrtemp');
    	 storeindexl(my_ptrtemp,y,offset))

    val addl3 = three M.add
    
    fun subl3(Immed k, x, y) = addl3(x, Immed(~k), y)
      | subl3(Direct x, Direct y, Direct z) = M.sub(y,x,z)
      | subl3(x, Immed k, dest) = 
                (M.move(Immed k, my_arithtemp');
                 subl3(x, my_arithtemp, dest))
      | subl3 _ = ErrorMsg.impossible "subl3 args don't match in mips"
    
    fun mull2(Direct x, Direct y) = M.mult(y,x,y)
      | mull2(Immed x, Direct y) = (M.move(Immed x,my_arithtemp');
    				M.mult(y,my_arithtemp',y))
      | mull2 _ = ErrorMsg.impossible "mull2 args don't match in mips"
    fun divl2(Direct x, Direct y) = M.div(y,x,y)
      | divl2(Immed x, Direct y) = (M.move(Immed x,my_arithtemp');
    				M.div(y,my_arithtemp',y))
      | divl2 _ = ErrorMsg.impossible "divl2 args don't match in mips"

    fun ashr(shamt, Direct op1, Direct result) = M.sra(shamt,op1,result)
      | ashr(shamt, Immed op1, Direct result) = 
    	(M.move(Immed op1,my_arithtemp'); M.sra(shamt,my_arithtemp',result))
      | ashr _ = ErrorMsg.impossible "ashr args don't match in mips"
    fun ashl(shamt, Direct op1, Direct result) = M.sll(shamt,op1,result)
      | ashl(shamt, Immed op1, Direct result) = 
    	(M.move(Immed op1,my_arithtemp'); M.sll(shamt,my_arithtemp',result))
      | ashl _ = ErrorMsg.impossible "ashl args don't match in mips"
    val addl3t = addl3
    val subl3t = subl3
    val ashlt = ashl
    fun mull2t(x,y as Direct y') = 
        let val ok = M.newlabel()
        in  mull2(x,y);
    	M.mfhi(my_arithtemp');
    	M.slt(y',Direct (Reg 0),my_ptrtemp'); (* 0 or 1 OK in pointer *)
    	M.add(my_arithtemp',my_ptrtemp,my_arithtemp');
    	M.beq(true,my_arithtemp',Reg 0,ok);    (* OK if not overflow *)
    	M.lui(my_arithtemp',32767);
    	M.add(my_arithtemp',my_arithtemp,my_arithtemp');  (* overflows *)
    	M.define(ok)
        end
      | mull2t _ = ErrorMsg.impossible "result of mull2t not register in mips"

    val orb = three M.or
    val andb = three M.and'
    fun notb (a,b) = subl3(a, Immed ~1, b) (* ~1 - a == one's complement *)
    val xorb = three M.xor
    datatype condition = NEQ | EQL | LEQ | GEQ | LSS | GTR
    local 
    fun makeibranch reverse = 
    let
    fun ibranch (cond, Immed a, Immed b, Immedlab label) =
                if (case cond of EQL => a=b | NEQ => a<>b | LSS => a<b |
                                 LEQ => a<=b | GTR => a>b | GEQ => a>=b)
                    then M.beq(true,Reg 0, Reg 0, label) else ()
      | ibranch (NEQ, Direct r, Direct s, Immedlab label) =
                        M.beq(false, r, s, label)
      | ibranch (NEQ, Direct r, x, Immedlab label) =
                        (M.move(x, my_arithtemp');
                         M.beq(false, r, my_arithtemp', label))
      | ibranch (EQL, Direct r, Direct s, Immedlab label) =
                        M.beq(true, r, s, label)
      | ibranch (EQL, Direct r, x, Immedlab label) =
                        (M.move(x, my_arithtemp');
                         M.beq(true, r, my_arithtemp', label))
      | ibranch (LSS, Direct r, x, Immedlab lab) =
                    (M.slt(r,x,my_arithtemp');
                     M.beq(false,Reg 0, my_arithtemp',lab))
      | ibranch (GEQ, Direct r, x, Immedlab lab) =
                    (M.slt(r,x,my_arithtemp'); 
                     M.beq(true,Reg 0, my_arithtemp',lab))
      | ibranch (GTR, x, Direct r, Immedlab lab) =
                    (M.slt(r,x,my_arithtemp'); 
                     M.beq(false,Reg 0, my_arithtemp',lab))
      | ibranch (LEQ, x, Direct r, Immedlab lab) =
                    (M.slt(r,x,my_arithtemp'); 
                     M.beq(true,Reg 0, my_arithtemp',lab))
    (* These two cases added to prevent infinite reversal *)
      | ibranch (GTR, Direct r, x, Immedlab lab) =
    		(M.move(x, my_arithtemp');
    		 M.slt(my_arithtemp',Direct r,my_arithtemp');
    		 M.beq(false,Reg 0,my_arithtemp',lab))
      | ibranch (LEQ, Direct r, x, Immedlab lab) =
    		(M.move(x, my_arithtemp');
    		 M.slt(my_arithtemp',Direct r,my_arithtemp');
    		 M.beq(true,Reg 0,my_arithtemp',lab))
      | ibranch (_, Immedlab _, Immedlab _, _) = 
                    ErrorMsg.impossible "bad ibranch args 1 in mips"
      | ibranch (_, Immedlab _, _, _) = 
                    ErrorMsg.impossible "bad ibranch args 1a in mips"
      | ibranch (_, _, Immedlab _, _) = 
                    ErrorMsg.impossible "bad ibranch args 1b in mips"
      | ibranch (_, _, _, Direct _) = 
                    ErrorMsg.impossible "bad ibranch args 2 in mips"
      | ibranch (_, _, _, Immed _) = 
                    ErrorMsg.impossible "bad ibranch args 3 in mips"
      | ibranch (cond, x, y, l) = 
            let fun rev LEQ = GEQ
                  | rev GEQ = LEQ
                  | rev LSS = GTR
                  | rev GTR = LSS
                  | rev NEQ = NEQ
                  | rev EQL = EQL
            in  if reverse then (makeibranch false) (rev cond, y,x,l) 
    	    else ErrorMsg.impossible "infinite ibranch reversal in mips"
    	
            end
    in ibranch
    end
    in
    val ibranch = makeibranch true
    end
        
    fun jmp (Direct r) = M.jump(r)
      | jmp (Immedlab lab) = M.beq(true,Reg 0,Reg 0,lab)
      | jmp (Immed i) = ErrorMsg.impossible "jmp (Immed i) in mips"
    
    
            (* branch on bit set *)
    fun bbs (Immed k, Direct y, Immedlab label) =
            (M.and'(y,Immed (Bits.lshift(1,k)),my_arithtemp');
             M.beq(false,my_arithtemp',Reg 0,label))
      | bbs _ = ErrorMsg.impossible "bbs args don't match in mips"


    val floatop1 = Reg 0
    val floatop2 = Reg 2
    val floatresult = Reg 0
    
    val real_tag = Immed(8*System.Tags.power_tags + System.Tags.tag_string)
    
    fun store_float(Reg n,ea,offset) = 
        if n mod 2 <> 0 then ErrorMsg.impossible "bad float reg in mips"
        else (M.swc1(Reg (n+1),ea,offset+4);M.swc1(Reg n,ea,offset))
    
    fun finish_real (Direct result) = (
        store_float(floatresult,dataptr,4);
        M.move(real_tag,my_arithtemp');
        M.sw(my_arithtemp',dataptr,0);
        M.add(dataptr',Immed 4,result);
        M.add(dataptr',Immed 12,dataptr'))
      | finish_real _ = 
         ErrorMsg.impossible "ptr to result of real operation not register in mips"
    
    fun load_float(Reg dest,src,offset) =
        if dest mod 2 <> 0 then ErrorMsg.impossible "bad float reg in mips"
        else (M.lwc1(Reg dest,src,offset); M.lwc1(Reg (dest+1),src,offset+4))
    
    fun two_float instruction (op1,result) = (
        load_float(floatop1,op1,0);
        instruction(floatop1,floatresult);
        finish_real(result))
    
    fun three_float instruction (op1,op2,result) = (
        load_float(floatop1,op1,0);
        load_float(floatop2,op2,0);
        instruction(floatop1,floatop2,floatresult);
        finish_real(result))
    
    val mnegg = two_float M.neg_double
    val mulg3 = three_float M.mul_double
    val divg3 = three_float M.div_double
    val addg3 = three_float M.add_double
    val subg3 = three_float M.sub_double
    
    
    local
        fun compare(LSS,op1,op2) = (M.slt_double(op1,op2); true)
          | compare(GEQ,op1,op2) = (M.slt_double(op1,op2); false)
          | compare(EQL,op1,op2) = (M.seq_double(op1,op2); true)
          | compare(NEQ,op1,op2) = (M.seq_double(op1,op2); false)
          | compare(LEQ,op1,op2) = compare(GEQ,op2,op1)
          | compare(GTR,op1,op2) = compare(LSS,op2,op1)
    in
        fun gbranch (cond, op1, op2, Immedlab label) = (
                load_float(floatop1,op1,0);
                load_float(floatop2,op2,0);
                M.bcop1(compare(cond,floatop1,floatop2),label))
          | gbranch _ = ErrorMsg.impossible "insane gbranch target in mips.nw"
    end
    	
    
    fun checkLimit max_allocation = M.sw(Reg 0, dataptr, max_allocation-4)
			      (* store zero in last location to be used *)
    


    fun beginStdFn _ = ()           (* do nothing, just like the Vax *)
    
    fun profile(i,incr) = ()


    val comment = M.comment

(* +DEBUG *)
    fun diag (s : string) f x =
    	f x handle e =>
    		(print "?exception "; print (System.exn_name e);
    		 print " in mips."; print s; print "\n";
    		 raise e)
    
    val emitlab = diag "emitlab" emitlab
    val define = diag "define" define
    
    
    val record = diag "record" record
    val select = diag "select" select
    val offset = diag "offset" offset
    
    val fetchindexb = diag "fetchindexb" fetchindexb
    val storeindexb = diag "storeindexb" storeindexb
    val jmpindexb = diag "jmpindexb" jmpindexb
    
    
    val fetchindexl = diag "fetchindexl" fetchindexl
    val storeindexl = diag "storeindexl" storeindexl
    
    
    val ashr = diag "ashr" ashr
    val ashl = diag "ashl" ashl
    
    val orb = diag "orb" orb
    val andb = diag "andb" andb
    val notb = diag "notb" notb
    val xorb = diag "xorb" xorb
    
    
    val addl3 = diag "addl3" addl3
    val subl3 = diag "subl3" subl3
    val mull2 = diag "mull2" mull2
    val divl2 = diag "divl2" divl2
    
    
    val addl3t = diag "addl3t" addl3t
    val subl3t = diag "subl3t" subl3t
    val mull2t = diag "mull2t" mull2t
    val ashlt = diag "ashlt" ashlt
    
    
    val ibranch = diag "ibranch" ibranch
    val jmp = diag "jmp" jmp
    val bbs = diag "bbs" bbs

(* -DEBUG *)

end (* MipsCM *)

