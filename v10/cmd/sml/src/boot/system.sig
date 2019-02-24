(* Copyright 1989 by AT&T Bell Laboratories *)
signature RUNTIMECONTROL =
  sig
    val collected : int ref
    val collectedfrom : int ref
    val gcmessages : int ref
    val majorcollections : int ref
    val minorcollections : int ref
    val ratio : int ref
    val softmax : int ref
  end

signature MCCONTROL =
  sig
    val printArgs : bool ref
    val printRet : bool ref
    val bindContainsVar : bool ref
    val bindExhaustive : bool ref
    val matchExhaustive : bool ref
    val matchRedundant : bool ref
    val expandResult : bool ref
  end

signature CGCONTROL =
  sig
    structure M68 : sig val trapv : bool ref end
    val tailrecur : bool ref
    val recordopt : bool ref
    val tail : bool ref
    val profile : bool ref
    val closureprint : bool ref
    val closureStrategy : int ref
    val rounds : int ref
    val path : bool ref
    val hoist : bool ref
    val reduce : bool ref
    val bodysize : int ref
    val reducemore : int ref
    val alphac : bool ref
    val comment : bool ref
    val knowngen : int ref
    val stdgen : int ref
    val knowncl : int ref
    val foldconst : bool ref
    val etasplit : bool ref
    val printit : bool ref
    val printsize : bool ref
    val scheduling : bool ref
  end

signature PRINTCONTROL =
  sig
    val printDepth : int ref
    val stringDepth : int ref
    val signatures : bool ref
  end

signature DEBUG =
  sig
    val debugging : bool ref
    val getDebugf : (int -> 'a) ref
    val interface : (int -> ('a -> 'b)) ref
  end

signature PROFILE =
  sig
    structure IO : sig type outstream end
    val profiling : bool ref	      (* controls profiling mode of compilation *)
    val profileOn : unit -> unit      (* turn interrupt timer on *)
    val profileOff : unit -> unit     (* turn interrupt timer off *)
    val clear : unit -> unit          (* clear profiling database *)
    val reset : unit -> unit          (* reset profiling counts to zero *)
    val report : IO.outstream -> unit (* print profiling report to stream *)
  end

signature PROFILEINTERNALS = 
  sig 
    val add : 'a -> unit
    val setToplevel : unit -> unit
    val setOther : unit -> unit
    structure P : PROFILE
  end

signature CONTROL =
  sig
    structure Runtime : RUNTIMECONTROL
    structure MC : MCCONTROL
    structure CG : CGCONTROL
    structure Print : PRINTCONTROL
    structure ProfileInternals : PROFILEINTERNALS
    structure Profile : PROFILE
    structure Debug : DEBUG
    val prLambda : (unit -> unit) ref
    val debugging : bool ref
    val primaryPrompt : string ref
    val secondaryPrompt : string ref
    val internals : bool ref
    val weakUnderscore : bool ref
    val interp : bool ref
    val debugLook : bool ref
    val debugCollect : bool ref
    val debugBind : bool ref
    val saveLambda : bool ref
    val saveLvarNames : bool ref
    val timings : bool ref
    val reopen : bool ref
  end

signature TIMER =
  sig  
    datatype time = TIME of {sec : int, usec : int}
    type timer
    val start_timer : unit -> timer
    val check_timer : timer -> time
    val check_timer_gc: timer -> time
    val makestring : time -> string
    val add_time : time * time -> time
  end

signature TAGS =
  sig
    val width_tags : int
    val power_tags : int
    val tag_record : int
    val tag_array : int
    val tag_bytearray : int
    val tag_string : int
    val tag_embedded : int
    val tag_suspension : int
    val tag_backptr : int
    val tag_forwarded : int
  end

signature STATS =
  sig
    structure Timer : TIMER
    val lines : int ref
    val parse : Timer.time ref
    val translate : Timer.time ref
    val codeopt : Timer.time ref
    val convert : Timer.time ref
    val hoist : Timer.time ref
    val cpsopt : Timer.time ref
    val closure : Timer.time ref
    val globalfix : Timer.time ref
    val spill : Timer.time ref
    val codegen : Timer.time ref
    val freemap : Timer.time ref
    val execution : Timer.time ref
    val update : Timer.time ref * Timer.time -> unit
    val summary : unit -> unit
  end

signature UNSAFE =
  sig
    type object
    structure Assembly : ASSEMBLY
    val boxed : 'a -> bool
    val ordof : 'a * int -> int
    val slength : 'a -> int
    val store : 'a * int * int -> unit
    val subscript : 'a array * int -> 'a
    val update : 'a array * int * 'a -> unit
    val delay : int * 'a -> 'a
    val force : 'a -> 'a
    val boot : string -> ('a -> 'b)
    val cast : 'a -> 'b
    val syscall : int * string list * int -> int
    val blast_write : 'outstream * 'a -> unit
    val blast_read :  'instream -> 'a
    val create_s : int -> string
    val store_s : string * int * int -> unit
    val lookup_r : (int -> object) ref
    val lookup : int -> object
    val isolate : (unit->'a) -> 'a
    val pstruct : {core: object, initial: object, math: object} ref
    exception Boxity
    val tuple : object -> object array
    val string : object -> string
    val real : object -> real
    val int : object -> int
    datatype datalist = DATANIL | DATACONS of (string * string * datalist)
    val datalist : datalist
  end

signature SYSTEM =
  sig
    structure ByteArray : BYTEARRAY
    structure Control : CONTROL
    structure Tags : TAGS
    structure Timer : TIMER
    structure Stats : STATS
    structure Unsafe : UNSAFE
    val exn_name : exn -> string
    val version : string
    val interactive : bool ref
    val cleanup : unit -> unit  (* closes all files *)
    val system : string -> unit
    val cd : string -> unit
    val argv : unit -> string list
    val environ : unit -> string list
  end
