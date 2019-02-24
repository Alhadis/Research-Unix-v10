(* Copyright 1989 by AT&T Bell Laboratories *)
(* namespace.sml *)

structure NameSpace: NAMESPACE =
struct

  open Symbol

  (* there are 7 namespaces, numbered 0 to 6 *)
  val namespaces = 7
  val varSpace = 0
  val tycSpace = 1
  val tyvSpace = 2
  val sigSpace = 3
  val strSpace = 4
  val fctSpace = 5
  val fixSpace = 6

  fun varIndex(id: symbol) = number(id)*namespaces
  fun conIndex(id: symbol) = number(id)*namespaces
  fun tycIndex(id: symbol) = number(id)*namespaces+1
  fun tyvIndex(id: symbol) = number(id)*namespaces+2
  fun sigIndex(id: symbol) = number(id)*namespaces+3
  fun strIndex(id: symbol) = number(id)*namespaces+4
  fun fctIndex(id: symbol) = number(id)*namespaces+5
  fun fixIndex(id: symbol) = number(id)*namespaces+6

  fun varKey(id: symbol) = (number(id)*namespaces, name(id))
  fun conKey(id: symbol) = (number(id)*namespaces, name(id))
  fun tycKey(id: symbol) = (number(id)*namespaces+1, name(id))
  fun tyvKey(id: symbol) = (number(id)*namespaces+2, name(id))
  fun sigKey(id: symbol) = (number(id)*namespaces+3, name(id))
  fun strKey(id: symbol) = (number(id)*namespaces+4, name(id))
  fun fctKey(id: symbol) = (number(id)*namespaces+5, name(id))
  fun fixKey(id: symbol) = (number(id)*namespaces+6, name(id))

  fun key(namespace:int, id:symbol) = 
      (namespaces*(number id)+namespace, name id)

end (* NameSpace *)
