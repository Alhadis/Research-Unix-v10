(* Copyright 1989 by AT&T Bell Laboratories *)
signature LEX = sig
    type lexer
    val mkLex : {stream : instream, interactive : bool} -> 
                {nextToken : Token.token ref,
                 prompt : string ref,
                 advance : unit -> unit}
end

structure Lex : LEX =
struct
 type lexer ={nextToken : Token.token ref,
                 prompt : string ref,
                 advance : unit -> unit}

 fun mkLex{stream,interactive} =
  let val prompt = ref ""
      val lexarg = {comLevel = ref 0, lineNum = ErrorMsg.lineNum,
                      complain = ErrorMsg.complain}

      val nextToken = ref Token.SEMICOLON

      val doprompt = ref true

      fun getline _ =(if !doprompt then (output std_out (!prompt);
					 flush_out std_out;
					 doprompt := false)
				   else ();
		      let val s = input_line stream
		      in doprompt := (ordof(s,size s - 1)=ord("\n")
				     handle Ord => false);
		         s
		      end)

     val get = Mlex.makeLexer(if interactive then getline else input stream)
		lexarg
     fun advance() = nextToken := get()

  in {nextToken=nextToken, prompt=prompt, advance=advance}
 end
    
end
