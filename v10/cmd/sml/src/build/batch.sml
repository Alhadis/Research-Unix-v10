(* Copyright 1989 by AT&T Bell Laboratories *)
functor Batch(structure M: CODEGENERATOR and A:ASSEMBLER)  : sig end =
struct

val pr = output std_out
open PrintUtil ProcessFile

(* command parsing *)

fun skip_white stream =
    case lookahead stream
      of " " => (input stream 1; skip_white stream)
       | "\t" => (input stream 1; skip_white stream)
       | "\n" => (input stream 1; skip_white stream)
       | _ => ()

fun getword stream =
    let val nextchar = input stream 1
     in case nextchar
	  of "" => ""
	   | " " => ""
	   | "\t" => ""
	   | "\n" => ""
	   | _ => nextchar ^ getword stream
    end

(* The commandline interpreter *)

val dir = ref ""
val globalhandle = ref true
val dumpCore = ref false

fun compile fname = 
    let val file = !dir ^ fname
        fun p(lexp,s) = 
	    let val code = M.generate lexp
		val outfile = open_out(s ^ ".mo")
	     in output outfile code; close_out outfile
	    end
     in pr ("[Compiling " ^ file ^ "]\n"); process(file, SOME p)
    end

fun assemble s = 
    let val file = !dir ^ s
        fun p(lexp,s) = 
	    let val f = open_out(s ^ ".s")
	     in A.generate (lexp, f); close_out f
	    end
     in pr ("[Assembling " ^ file ^ "]\n"); process(file, SOME p)
    end

fun load s = 
    let val file = !dir ^ s
    in pr ("[Loading " ^ file ^ "]\n"); ProcessFile.load file
    end

fun export s = 
    let val file = !dir ^ s
    in pr("[Exporting to " ^ file ^ "]\n"); exportML file; pr "hello there\n"
    end

exception Notfound_Compile of string
local open System.Control 
      open CG Profile
      val flags = [
		("internals",internals),
		("tailrecur",tailrecur),
		("recordopt",recordopt),
		("tail",tail),
		("profile",profile),
		("closureprint",closureprint),
		("hoist",hoist),
		("reduce",reduce),
		("foldconst",foldconst),
		("etasplit",etasplit),
		("comment",comment),
		("alphac",alphac),
		("printsize",printsize),
		("scheduling",scheduling),
		("MC.printArgs",MC.printArgs),
		("MC.printRet",MC.printRet),
		("MC.bindContainsVar",MC.bindContainsVar),
		("MC.bindExhaustive",MC.bindExhaustive),
		("MC.matchExhaustive",MC.matchExhaustive),
		("MC.matchRedundant",MC.matchRedundant),
		("MC.expandResult",MC.expandResult),
		("saveLvarNames",Access.saveLvarNames),
		("saveLambda",saveLambda),
                ("printit",printit),
		("debugging",debugging),
		("debugLook",debugLook),
		("debugBind",debugBind),
		("timings",timings),
		("dumpCore",dumpCore),
		("globalhandle",globalhandle),
		("profiling",profiling)]
in
fun getflag f =
    let fun get nil = raise Notfound_Compile f
	  | get ((name,flag)::tl) = if f=name then flag else get tl
     in get flags
    end

fun printflags () =
    (pr "[Flags:\n";
     app (fn(name,flag:bool ref) => (pr name; pr " = "; print(!flag); pr "\n"))
	 flags;
     pr "]\n")
end

fun toggle "" = printflags()
  | toggle arg =
    let val flag = getflag arg
	val new = not(!flag)
    in pr ("["^arg^" := "^makestring new^"]\n"); flag := new
    end

fun lsave () = (toggle "saveLambda"; toggle "saveLvarNames")

fun atoi s =
    let val dtoi = fn "0" => 0 | "1" => 1 | "2" => 2 | "3" => 3 | "4" => 4
		    | "5" => 5 | "6" => 6 | "7" => 7 | "8" => 8 | "9" => 9
		    | _ => (pr "[garbled integer input]\n"; raise ProcessFile.Stop)
    in case explode s
	of "~" :: s' => ~ (revfold (fn(a,b) => b * 10 + dtoi a) s' 0)
	 | s' => revfold (fn(a,b) => b * 10 + dtoi a) s' 0
    end

fun gcmessage() =
    let val f = System.Control.Runtime.gcmessages
    in f := (!f + 1) mod 4; pr "[gcmessages := "; print(!f); pr "]\n"
    end

fun summary() =
    (System.Stats.summary();
     pr(makestring(!System.Control.CG.knowngen));
     pr " knowngen\n";
     pr(makestring(!System.Control.CG.knowncl));
     pr " knowncl\n";
     pr(makestring(!System.Control.CG.stdgen));
     pr " stdgen\n";
     ())

val execs =
       [("lsave",lsave),
	("summary",summary),
	("prFun",fn () =>
		ProcessFile.prFun(atoi(skip_white std_in; getword std_in))),
	("gcmessages",gcmessage),
	("setratio",fn () =>
		let val i = atoi(skip_white std_in; getword std_in)
		in  pr "[ratio := "; print i; pr "]\n";
		    System.Control.Runtime.ratio := i
		end),
	("setsoftmax",fn () =>
		let val i = atoi(skip_white std_in; getword std_in)
		in  pr "[softmax := "; print i; pr "]\n";
		    System.Control.Runtime.softmax := i
		end),
	("setbodysize",fn () =>
		let val i = atoi(skip_white std_in; getword std_in)
		in  pr "[bodysize := "; print i; pr "]\n";
		    System.Control.CG.bodysize := i
		end),
	("setrounds",fn () =>
		let val i = atoi(skip_white std_in; getword std_in)
		in  pr "[rounds := "; print i; pr "]\n";
		    System.Control.CG.rounds := i
		end),
	("setreducemore",fn () =>
		let val i = atoi(skip_white std_in; getword std_in)
		in  pr "[reducemore := "; print i; pr "]\n";
		    System.Control.CG.reducemore := i
		end),
	("setclosureStrategy",fn () =>
		let val i = atoi(skip_white std_in; getword std_in)
		in  pr "[closureStrategy := "; print i; pr "]\n";
		    CGoptions.closureStrategy := i
		end),
	("printslots",fn () => ProcessFile.printslots(skip_white std_in;
						   getword std_in)),
	("flushstdout",fn () => set_term_out(std_out,true)),
	("dumpMap",ProcessFile.dumpMap),
	("asBoot",fn () => (ProcessFile.bootEnv assemble; ())),
	("mBoot",fn () => (ProcessFile.bootEnv compile; ())),
	("primeEnv",ProcessFile.primeEnv),
	("clear",System.Control.Profile.clear),
	("reset",System.Control.Profile.reset),
	("report",fn () => System.Control.Profile.report std_out),
	("profileOff",System.Control.Profile.profileOff),
	("profileOn",System.Control.Profile.profileOn)]

fun getexec f =
    let fun get nil = raise Notfound_Compile f
	  | get ((name,exec)::tl) = if f=name then exec else get tl
     in get execs
    end

fun printexecs () =
    (pr "[Available execs:\n";
     app (fn ("setbodysize",_) =>
	       (pr "setbodysize <int> (currently ";
	        Integer.print(!System.Control.CG.bodysize); pr ")\n")
	   | ("setreducemore",_) =>
	       (pr "setreducemore <int> (currently ";
		Integer.print(!System.Control.CG.reducemore); pr ")\n")
	   | ("setclosureStrategy",_) =>
	       (pr "setclosureStrategy <int> (currently ";
		Integer.print(!CGoptions.closureStrategy);
		pr ")\n")
	   | ("prFun",_) => pr "prFun <lvar>\n"
	   | ("printslots",_) => pr "printslots <structure>\n"
	   | (name,_) => (pr name; pr "\n"))
	 execs;
     pr "]\n")

fun execute "" = printexecs()
  | execute arg =
    let val exec = getexec arg
    in  pr("["^arg^"()]\n");
	exec()
    end

fun help() = pr "\
\!file      => compile the file.\n\
\*file      => assemble the file.\n\
\<file      => parse the file.\n\
\>file      => export to a file.\n\
\%          => print the last generated lambda.\n\
\#word      => comment; ignored.\n\
\@directory => look for files in a directory.  directory should end in /.\n\
\~function  => execute a function.\n\
\^flag      => toggle a flag.\n\
\?          => print this help message.\n"

fun interp "" = ()
  | interp word =
    let val arg = substring(word,1,size word - 1) handle Substring => ""
    in  (case substring(word,0,1) of
 	      "!" => compile arg
	    | "*" => assemble arg
	    | "<" => load arg
	    | ">" => export arg
	    | "%" => ProcessFile.prLambda()
	    | "#" => ()			(* comment *)
	    | "@" => dir := arg		(* change load directory *)
	    | "~" => execute arg	(* execute function *)
	    | "^" => toggle arg		(* toggle flag *)
	    | "?" => help()		
	    |  _  => pr ("[What is \""^word^"\"?]\n")
	) handle e as Notfound_Compile f =>
		   (pr("[flag \""^f^"\" not recognized]\n");
		    raise e)
    end

fun interp1 word =
    if !globalhandle
    then (interp word
	  handle ProcessFile.Stop =>
		  (pr "[Failed on ";
		   pr_mlstr word;
		   pr "]\n";
		   flush_out std_out)
	       | e =>
	          (pr "[Failed on ";
		   pr_mlstr word; pr " with ";
		   pr(System.exn_name e); pr "]\n";
		   flush_out std_out))
    else interp word
	 handle e =>
	   (pr "[Failed on ";
	    pr_mlstr word; pr " with ";
	    pr(System.exn_name e); pr "]\n";
	    flush_out std_out;
	    if !dumpCore
	    then (toggle "globalhandle";
		  toggle "dumpCore";
		  pr "[Saving state]\n[Exporting to sml.save]\n";
		  flush_out std_out;
		  if exportML "sml.save"
		   then pr "hello there\n"
		   else (summary(); raise e))
	    else raise e)
		 
(* command-line interpreter top-level loop *)
fun toplevel () =
    if end_of_stream std_in
    then ()
    else (skip_white std_in;
	  if (end_of_stream std_in)
	  then () 
	  else (interp1(getword std_in); toplevel ()))

(* load the pervasives (no .mo files generated) *)
val _ = ProcessFile.bootEnv load

(* start up command interpreter *)
val _ = (pr "hello there\n"; toplevel ())

end
