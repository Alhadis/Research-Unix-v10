(* Copyright 1989 by AT&T Bell Laboratories *)
structure ErrorMsg : ERRORMSG = struct

    exception Syntax

    val anyErrors = ref false
    val lineNum = ref 1
    val fileName = ref ""
    val debugging = System.Control.debugging

    fun say (msg: string) =
	(if !System.interactive then ()
	 else (print (!fileName); print ", "; print "line ";
               print (makestring(!lineNum)); print ": ");
         print msg;
	 print "\n";
         ())

    fun warn msg = say ("Warning: " ^ msg)

    fun complain (msg: string) =
	(say ("Error: " ^ msg); anyErrors := true)
 
    fun condemn msg = (complain msg; raise Syntax)
    fun impossible msg = condemn("Compiler bug: " ^ msg)

    fun flaggedmsg (flg : bool ref) (msg : string) =
	let val printit = !flg
	in  if printit then (print msg; print "\n")
	    else ();
	    printit
	end
    val debugmsg = flaggedmsg debugging

end  (* structure ErrorMsg *)

