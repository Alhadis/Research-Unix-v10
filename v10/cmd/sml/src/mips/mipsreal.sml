(* Copyright 1989 by AT&T Bell Laboratories *)
functor MipsReal(E: sig val emit_word : int * int -> unit end) : REALCONST = 
struct
    open E
    val halfwords = ref nil : int list ref		(* halfwords already out *)
    val count = ref 0				(* length of halfwords *)
    fun reset_state () = (halfwords := nil; count := 0)
    fun add_half h = (count := !count + 1; halfwords := h :: (!halfwords))

    fun emit_four [lowest,low,high,highest] = 
    			(emit_word(low,lowest);emit_word(highest,high))
      | emit_four _ = ErrorMsg.impossible "bad floating pt constant in mips"

    fun emit_half h = 
        if !count = 3 then (emit_four (h::(!halfwords)); reset_state())
        else add_half h

    structure IEEERealConst =
	RealConst(IEEEReal(struct val emitWord = emit_half end))
    val realconst = IEEERealConst.realconst
end




