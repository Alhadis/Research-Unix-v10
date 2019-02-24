(* Copyright 1989 by AT&T Bell Laboratories *)
structure Dummy : ASSEMBLY =
  struct
      datatype datalist = DATANIL | DATACONS of (string * string * datalist)
      type func = unit
      datatype funclist = FUNC of (func * string * funclist)
      type object = unit
      structure A = struct
	val array = InLine.cast()
	val callc = InLine.cast()
	val create_b = InLine.cast()
	val create_s = InLine.cast()
	val floor = InLine.cast()
	val logb = InLine.cast()
	val scalb =  InLine.cast()
	val syscall =  InLine.cast()
      end
      exception Div
      exception Interrupt
      exception Overflow
      exception Real of string
      exception SystemCall of string
      exception UnboundTable
      val array0 = InLine.cast()
      val bytearray0 = InLine.cast()
      val collected = InLine.cast()
      val collectedfrom = InLine.cast()
      val current = InLine.cast()
      val datalist = InLine.cast()
      val errorstrings = InLine.cast()
      val external = InLine.cast()
      val gcmessages = InLine.cast()
      val gcprof = InLine.cast()
      val majorcollections = InLine.cast()
      val minorcollections = InLine.cast()
      val opsys = InLine.cast()
      val pstruct = InLine.cast()
      val ratio = InLine.cast()
      val softmax = InLine.cast()
end

structure Core = CoreFunc(Dummy)
