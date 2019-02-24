#include "check.h"

int count = 0 ;

class small
{ public:
	int foo;
	small(){};
	~small(){count++ ;};
};

class big
{ public:
	int foo;
	small contained;
	big(){};
	~big(){count++;} ;
};

template<class contained_type> class para
{ public:
	int foo;
	contained_type contained;
	para(){};
	~para(){count++;} ;
};

typedef para<small> ct;

template<class contained_type> class para2
{ public:
	int foo;
	contained_type contained;
	para2(){};
	~para2(){count++;} ;
};


main()
{
  start_test(__FILE__) ;

  {
    small s;
  }
  check(count == 1) ;
  count = 0 ;
  {
    big b;
  }
  check(count == 2) ;
  count = 0 ;

  {
    para<small> p;
  }
  check(count == 2);
  count = 0 ;
  
  {
    para2<ct> p2;
  }
  check (count == 3) ;

  end_test() ;
  
};
