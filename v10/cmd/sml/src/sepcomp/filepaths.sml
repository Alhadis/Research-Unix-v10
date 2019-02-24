(* Copyright 1989 by AT&T Bell Laboratories *)
(* FILEPATHS: maintain default paths (file prefixes, essentially) when
 	      opening nested imported files. *)

(* Trivial FILEPATHS example:

functor TrivPaths(): FILEPATHS =
struct
  type Filepath = unit
  val defaultPath = ()
  exception ImpliedPath
  fun impliedPath(_, filename) = {validName=filename, newPath=()}
end
*)
(* UNIX version: Any filename which starts with "/" is assumed to be
		 absolute, and implies that entire directory. 
		 Any filename which starts with "./" causes a search from
		 the current working directory into the subdirectories.
		 Any other filename (xxx/yyy/...) is relative to the
		 current path.
 *)

functor UnixPaths(): FILEPATHS =
struct

  val DEBUG = false
  val debug = 
      if DEBUG then fn str => output std_out ("<" ^ str ^ ">\n")
      else fn _ => ()

  type Filepath = string		(* Yes, I know, pretty trivial,
					 but I'd like to keep it abstract,
					 all the same. *)
  exception ImpliedPath

  val defaultPath = ""

  (* divideSlash: divide a full filename into path and name. Keep the "/"
     on the path. *)

  fun divideSlash filename =
      let val revChars = rev(explode filename)
	  fun stripSlash(nil, name) = (nil, name)
	    | stripSlash(p as "/" :: rest, name) = (rev p, name)
	    | stripSlash(ch :: rest, name) = stripSlash(rest, ch :: name)
       in stripSlash(revChars, nil)
      end

  fun impliedPath(oldPath, oldName) =
      let val (pathchs, namechs) = divideSlash oldName
	  val path = implode pathchs
	  val name =  implode namechs
       in case pathchs
	    of "/" :: _ => {validName=path^name, newPath=path}
	     | "." :: "/" :: _ => {validName=path^name, newPath=path}
	     | _ => {validName=oldPath^path^name, newPath=oldPath^path}
      end

  val impliedPath =
      if DEBUG then
	fn (oldPath, oldName) =>
	  let val {validName, newPath} = impliedPath(oldPath, oldName)
	   in debug("impliedPath(oldPath=" ^ oldPath ^ ", oldName=" ^ oldName
		    ^ ") -> (" ^ validName ^ ", " ^ newPath ^ ")");
	      {validName=validName, newPath=newPath}
	  end
      else impliedPath

end (* functor UnixPaths *)
