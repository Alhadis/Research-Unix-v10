(* Copyright 1989 by AT&T Bell Laboratories *)
(* FILEPATHS: a hopefully general way of interpreting arguments to "import"
	      so that we can have relative paths. This will be different
	      for different systems; I provide a unix version, and if anybody
	      does a port to VAX/VMS or MS-DOS (snigger) the FilePaths()
	      functor will have to change.
							NICK, Apr '89
 *)

signature FILEPATHS =
  sig
    type Filepath
    val defaultPath: Filepath		(* If I open "foo", this is where it
					   comes from: result of "pwd"? *)
    exception ImpliedPath		(* Raise on ~nobody/... *)
    val impliedPath:
      Filepath * string -> {validName: string, newPath: Filepath}
					(* Given a current path and some
					   filename, return a filename I
					   can use, plus the implied path
					   to use within that file. *)
  end;
