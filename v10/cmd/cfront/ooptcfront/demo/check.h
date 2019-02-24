#include <stdio.h>

#define check(p)  if (!(p))printf("\nerror(%d): test failed at line %d",++error, __LINE__) 


int error ;

extern "C" void exit(int) ;
static char *test_file ;

void start_test(char *file) 
{
  test_file = file ;
  printf ("\n*Begin Testing %s\n", file) ;
}

void end_test() 
{
  printf ("\n*End Testing %s", test_file) ;
  if (error)
    printf (" *** %d errors ***\n", error) ;
  else printf (" no errors detected\n") ;
  
  exit(error) ;
}
