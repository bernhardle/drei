	// (c) bernhard schupp 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/main_test.cc,v $
	// $Revision: 3.14 $
	// $Name:  $
	// $State: Exp $
	// $Date$
	
#include <iostream.h>

#include "gitter_impl.h"

static volatile char RCSId_main_test_cc [] = "$Id: main_test.cc,v 3.14 1998/10/29 10:43:08 yc23 Exp yc23 $" ;

int main(int argc, char ** argv, char ** envp) {

  cout << "\n  ------------------------------------\n\n" ;
  cout <<   "  main_test () :                      \n" ;
  cout <<   "  Grobgitter  aus < " << argv [1] << " > lesen" << endl ;
  GitterBasisImpl grd (argv[1]) ;
  if (argc == 4) {
    cout << "  Verfeinerung aus < " << argv [3] << " > lesen" << endl ;
    grd.restore (argv [3]) ;
  }
  cout << "\n  ------------------------------------\n" << endl ;

  grd.printsize () ;

//  grd.fullIntegrityCheck () ;
  for (int i = 0 ; i < atoi (argv[2]) ; i ++ ) {
//    grd.refineGlobal () ;
    grd.refineRandom (.05) ; 
//    grd.fullIntegrityCheck () ;
    grd.printsize () ;
  }
  grd.backup ("final.bak") ;
  return 0 ;
}
