structure Sample =
   struct
      val run = fn filename =>
          let val in_str = open_in filename
	      val lexer =  Mlex.makeLexer (input in_str)
	      val p = (Calc.HDR.lineno := 0;
		       Calc.parse lexer (5,5))
	   in (close_in in_str; p)
	   end
    
      fun run_std_in () =
	    (* more suitable for interactive use *)
	let val lexer = Mlex.makeLexer (fn _ => input_line std_in)
	    val p = (Calc.HDR.lineno := 0;
		       Calc.parse lexer (0,0))
	 in p
        end
   end
