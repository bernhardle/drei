	// (c) bernhard schupp 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/main_pll.cc,v $
	// $Revision: 1.101 $
	// $Name:  $
	// $State: Exp $
	
#include <ulimit.h>
	
#include <time.h>
#include <stdio.h>
#include <iostream.h>

#include "mpAccess_MPI.h"
#include "gitter_pll_impl.h"

static volatile char RCSId_main_pll_cc [] = "$Id: main_pll.cc,v 1.101 1998/10/29 10:43:08 yc23 Exp yc23 $" ;

int main (int argc, char ** argv, char **) {
  MPI_Init (&argc, &argv) ;
  {
  
    MpAccessMPI mpa (MPI_COMM_WORLD) ;
  
    if (!mpa.myrank ()) {
      cout << "\n  ------------------------------------\n\n" ;
      cout <<   "  main_pll () :                       \n" ;
      cout <<   "  Makrogitter  aus < " << argv [1] << " > lesen\n" ;
      if (argc == 4) {
        cout << "  verfeinerung aus < " << argv [3] << " > lesen\n" ;
      }
      cout << "\n  ------------------------------------\n" << endl ;
    }
    
    
    GitterBasisPll part (argv[1], mpa) ;

    if (argc == 4) part.restore (argv [3]) ;
    part.printsize () ;
//    part.fullIntegrityCheck () ;

    for (int i = 0 ; i < (argc > 2 ? atoi (argv [2]) : 0 ) ; i ++ ) {
      part.refineRandom (.1 * (double)mpa.myrank ()/(double)mpa.psize ()) ;
//      part.fullIntegrityCheck () ;
      part.printsize () ;
    }

    part.backup ("final.bak") ;
    
  }
  MPI_Finalize () ;
  
  return 0 ;
}
