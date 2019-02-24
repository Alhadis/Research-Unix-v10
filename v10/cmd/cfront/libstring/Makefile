/*
 * jes c++ string lib makefile
 */

CC = CC

#if release(bsd)
CCFLAGS += -DBSD
#endif

libstring.a :: Makefile makefile String.h list.h string.c sysfns.c\
		stringCtor.c Memcpy.c error.c rep.c list.c generic.c\
		voidP_list.c voidP_out.c list_sort.c Regexp.h Regexp.c

testIt :: testIt.c pair_list.h pair_list.c libstring.a

test1 :: test1.c libstring.a

test2 :: test2.c libstring.a

test3 :: pair.h test3.c pairST_list.c libstring.a

test4 :: test4.c string_list.c s_l_o.c libstring.a

test5 :: test5.c string_list.c s_l_o.c libstring.a

test6 :: test6.c string_list.c s_l_o.c libstring.a

test7 :: test7.c string_list.c s_l_o.c libstring.a

test8 :: test8.c string_list.c s_l_o.c libstring.a

test10 :: test10.c string_list.c libstring.a

test11 :: test11.c string_list.c libstring.a

test12 :: test12.c pair_list.c libstring.a

test13 :: test13.c pairST_list.c libstring.a

test14 :: test14.c libstring.a

test16 :: test16.c libstring.a

test17 :: test17.c libstring.a

test18 :: test18.c libstring.a

libstring.a .o : (cfrontC)
