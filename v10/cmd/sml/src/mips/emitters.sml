(* Copyright 1989 by AT&T Bell Laboratories *)
structure Emitters : EMITTERS = struct
    type emitter_triple = (int * int -> unit) * (int -> string -> unit)
					* (string -> unit)
    local 
        val so_far = ref nil : string list ref
        fun squirrel s = so_far := s :: !so_far
        fun emit_byte n = squirrel (chr n)
    in
        fun emit_string n s = squirrel (substring(s,0,n))
                	handle e =>
                		(print "?exception "; print (System.exn_name e);
                		 print (" in emitters.emit_string "^
        				(Integer.makestring n) ^ " \""^s^"\"\n");
                		 raise e)

        fun emit_pair_little(hi,lo) =
            let open Bits
                fun emit_word(n) =
                  (emit_byte(andb(n,255));emit_byte(andb(rshift(n,8),255)))
            in  (emit_word(lo);emit_word(hi))
            end

        fun emit_pair_big(hi,lo) =
             let open Bits
                fun emit_word(n) =
                  (emit_byte(andb(rshift(n,8),255));emit_byte(andb(n,255)))
            in  (emit_word(hi);emit_word(lo))
            end
        

	fun emitted_string () =
	    let val s = implode (rev (!so_far))
	    in  so_far := nil; s
	    end
    end
    structure BigReal = MipsReal(struct val emit_word = emit_pair_big end)
    structure LittleReal =MipsReal(struct val emit_word = emit_pair_little end)
    val LittleEndian = (emit_pair_little,emit_string,LittleReal.realconst)
    val BigEndian = (emit_pair_big,emit_string,BigReal.realconst)
    val address = ref 0		(* address of next instruction in words *)
    val decode_real_ptr = ref NONE : ((int * int) * string -> unit) option ref
    				   (* used to emit asm code for a real word *)
        
    val real_least = ref NONE : (int * int) option ref
    				   (* least significant word of real *)
    val real_string = ref ""
    fun emit_real_word w =
        let val decode_real = case !decode_real_ptr of 
    		  SOME f => f 
    		| NONE => ErrorMsg.impossible "missed real decoder in mips asm"
        in
            case !real_least of 
        	    NONE => real_least := SOME w
              | SOME least => 
    		(decode_real(least,"[low  word of "^(!real_string)^"]");
    		 decode_real(w,"[high word of "^(!real_string)^"]"))
        end
    
    structure AsmReal = MipsReal(struct val emit_word = emit_real_word end)
    
    fun MipsAsm stream =
        let fun say s = (output stream s; flush_out stream)
            fun printaddr addrref = 
               let val n = !addrref
               in  (if n<10 then "  " else if n < 100 then " " else "") 
                    ^ (Integer.makestring n) 
               end
            local 
                open Bits
                fun hexdigit n = 
                    let val d = andb(n,15)
                    in  if d <= 9 then chr(d+ord("0"))
                                  else chr(d-10+ord("a"))
                    end
                fun hex1 n = hexdigit(rshift(n,4))^hexdigit(n)
                fun hex2 n = hex1(rshift(n,8))^hex1(n)
    	    fun hex4 n = hex2(rshift(n,16))^hex2(n)
            in
                fun hex(hi,lo) = hex2(hi) ^ hex2(lo)
    	    fun printaddr addrref = 
               	let val n = 4 * (!addrref)	(* address in bytes *)
               	in "0x" ^ (hex4 n) 
               end
            end
            fun decode x = (
                    say ((printaddr address) ^ ": (" ^ (hex x) ^") " 
                     ^ (MipsDecode.decode x));
               address := !address + 1; ()
               )
    	fun do_decode_real(w,s) = (
                    say ((printaddr address) ^ ": (" ^ (hex w) ^") " 
                     ^ s ^ "\n");
               address := !address + 1; ()
               )
    	fun decode_real s = (real_string := s; AsmReal.realconst s)
            fun decode_string n s =
                if n > 0 then
                    (say ((printaddr address) 
                                ^ ": \"" ^substring(s,0,4) ^"\"\n");
                       address := !address + 1;
                       decode_string (n-4) (substring(s,4,String.length(s)-4))
                       )
                else ()
        in
    	decode_real_ptr := SOME do_decode_real;
            (decode,decode_string,decode_real) : emitter_triple
        end
end


