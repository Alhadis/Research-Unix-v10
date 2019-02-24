(* Copyright 1989 by AT&T Bell Laboratories *)
(* namespace.sig *)

signature NAMESPACE =
sig

  val namespaces: int
  val varSpace: int
  val tycSpace: int
  val tyvSpace: int
  val sigSpace: int
  val strSpace: int
  val fctSpace: int
  val fixSpace: int

  val varIndex: Symbol.symbol -> int
  val conIndex: Symbol.symbol -> int
  val tycIndex: Symbol.symbol -> int
  val tyvIndex: Symbol.symbol -> int
  val sigIndex: Symbol.symbol -> int
  val strIndex: Symbol.symbol -> int
  val fctIndex: Symbol.symbol -> int
  val fixIndex: Symbol.symbol -> int

  val varKey: Symbol.symbol -> int * string
  val conKey: Symbol.symbol -> int * string
  val tycKey: Symbol.symbol -> int * string
  val tyvKey: Symbol.symbol -> int * string
  val sigKey: Symbol.symbol -> int * string
  val strKey: Symbol.symbol -> int * string
  val fctKey: Symbol.symbol -> int * string
  val fixKey: Symbol.symbol -> int * string

  val key: int * Symbol.symbol -> int * string

end (* signature NAMESPACE *)
