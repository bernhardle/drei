	// (c) bernhard schupp 1997 - 1998

	// $Source$
	// $Revision$
	// $Name$
	// $State$

#include <iostream.h>

#include "gitter_euler_impl.h"

static volatile char RCSId_main_cc [] = "$Id$" ;

int main (int argc, char ** argv, char **) {
  {
    GitterEuler part (argv[1]) ;
    
    part.initialConstValue (EulerSolver :: Vec5 (1.0, 2.0, 3.0, 4.0, 5.0)) ;

    {
      LeafIterator  < Gitter :: hface_STI > w (part) ;
      for (w->first() ; ! w->done() ; w->next ())
        w->item ().pull__euler_data ().print(cout) ;
    }
  }
  return 0 ;
}
