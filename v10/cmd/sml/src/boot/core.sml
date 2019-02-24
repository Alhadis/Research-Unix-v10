(* Copyright 1989 by AT&T Bell Laboratories *)
(*
The following are already in the symbol table:
     1) Magical words that can be free in signatures (from PrimTypes):
		int string bool unit real list array ref exn
     2) Built-in constructors (from PrimTypes):
		:: nil ref true false
     3) Built-in structures:
		PrimTypes InLine
	The InLine structure is not typed (all values have type alpha).
All matches in this file should be exhaustive; the match and bind exceptions
 are not defined at this stage of bootup, so any uncaught match will cause
 an unpredictable error.
*)


functor CoreFunc(Assembly : ASSEMBLY) =
struct
    structure Assembly = Assembly

    exception Bind
    exception Match

  val bufsize = 1024
    val current = Assembly.current
    val other = Assembly.A.create_b 20
    val toplevel = Assembly.A.create_b 18
    val gc = InLine.cast Assembly.gcprof

  val std_in = {filid = 0, pos = ref 0, len = ref 0, name="<std_in>",
		closed = ref false, buf = Assembly.A.create_b bufsize,
		tty = ref true}
  val std_out ={filid = 1, pos = ref 0, name="<std_out>",
		closed = ref false, buf = Assembly.A.create_b bufsize,
		tty = ref true}

structure Refs = struct
    val debugInterface = ref ()
    val getDebugf = ref ()
    val debug1 = ref false
    val use_f = ref (fn (_ : string) => ())
    val use_s = ref (fn () => ())
    val lookup_r = ref (fn i:int => ())
    val compreturn = ref nil : string list ref
    val interactive = ref true
    val prLambda = ref (fn () => ())
    val zerotime = (0,0)
    val lines = ref 0
    val parse = ref zerotime
    val translate = ref zerotime
    val codeopt = ref zerotime
    val convert = ref zerotime
    val hoistx = ref zerotime
    val cpsopt = ref zerotime
    val closure = ref zerotime
    val globalfix = ref zerotime
    val spill = ref zerotime
    val codegen = ref zerotime
    val freemap = ref zerotime
    val execution = ref zerotime
    val outstreams = ref [std_out]
    val instreams = ref [std_in]
    val reduce_r = ref (fn a : (unit -> unit) => a)
    val printArgs = ref false
    val printRet = ref false
    val bindContainsVar = ref true
    val bindExhaustive = ref true
    val matchExhaustive = ref true
    val matchRedundant = ref true
    val expandResult = ref false
    val trapv = ref true
    val tailrecur = ref true
    val recordopt = ref true
    val tail = ref true
    val profile = ref false
    val closureprint = ref false
    val closureStrategy = ref 1
    val rounds = ref 1
    val path = ref false
    val hoist = ref true
    val reduce = ref true
    val bodysize = ref 0
    val reducemore = ref 15
    val alphac = ref true
    val comment = ref false
    val knowngen = ref 0
    val stdgen = ref 0
    val knowncl = ref 0
    val foldconst = ref true
    val interp = ref false
    val etasplit = ref true
    val printit = ref false
    val printsize = ref false
    val scheduling = ref true
    val printDepth = ref 5
    val stringDepth = ref 70
    val signatures = ref true
    val profiling = ref false
    val globalProfile = ref [other]
    val debugging = ref false
    val primaryPrompt = ref "- "
    val secondaryPrompt = ref "= "
    val internals = ref false
    val weakUnderscore = ref false
    val debugLook = ref false
    val debugCollect = ref false
    val debugBind = ref false
    val saveLambda = ref false
    val saveLvarNames = ref false
    val timings = ref false
    val reopen = ref false
    val pstruct = ref {core = (), initial=(), math=()}
end

    fun getDebug x = InLine.! Refs.getDebugf x

    fun stringequal(a,b) =
     if InLine.ieql(a,b) then true
     else InLine.boxed a andalso InLine.boxed b andalso
      let val len = InLine.slength a
       in if InLine.ieql(len,InLine.slength b)
	   then let fun f 0 = true
		      | f i = let val j = InLine.-(i,1)
			       in if InLine.ieql(InLine.ordof(a,j),
				                 InLine.ordof(b,j))
			           then f j else false
			      end
		 in f len
		end
	   else false
       end

    local
       val ieql = InLine.ieql and cast = InLine.cast and sub = InLine.subscript
       and boxed = InLine.boxed and op * = InLine.*
       and + = InLine.+  and  - = InLine.-
     in fun polyequal(a : 'a, b : 'a) = ieql(a,b)
      orelse
      (boxed a andalso boxed b
       andalso
	  (* assumes identical tags, with string=embedded *)
	  let 
(* We must test fields in reverse order, because data-constructor tags
   occur after the data-constructed values! *)
	      fun m ~1 = true
		| m i = polyequal(sub(a,i), sub(b,i)) andalso m(-(i,1))
	  in  case InLine.andb(sub(a,~1),7)
		of 0 (* tag_record div 2 *) => m(-(InLine.alength a, 1))
	         | 4 (* tag_array div 2 *) => false
		 | 5 (* tag_bytearray div 2 *) => false
		 | _ => stringequal(cast a, cast b)
	  end)
  end

end
