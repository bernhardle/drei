	// (c) bernhard schupp 1997

	// $Source$
	// $Revision$
	// $Name$
	// $State$
	// $Date$

#include "mpAccess_MPI.h"
#include "gitter_euler_pll_impl.h"

int main(int argc, char ** argv, const char ** envp) {
  MPI_Init (&argc, &argv) ;
  EulerSolver :: modifyGlobalConstants ("const_ffs.dat") ;
  {
    MpAccessMPI mpa (MPI_COMM_WORLD) ;

    GitterEulerPll grd (argv [1], mpa) ;
    
    cout << "\n  -----------------------------------------------\n\n" ;
    cout << "   Daten / Gitter nach < " << argv [2] << ".# > schreiben\n" ;
    
    if (argc > 3) {
      cout << "   Neustart aus < " << argv [3] << " > versuchen\n" ;
      grd.restore (argv [3]) ;
//      grd.fullIntegrityCheck () ;
    } else {  
      grd.initialConstValue (EulerSolver :: Vec5 (1.4, 4.2, 0.0, 0.0, 8.8)) ;
    }
    cout << "\n  ---------------------------------------------ok\n" << endl ;
    grd.printsize () ;
    grd.runEulerSolver (EulerSolver :: ENDTIME, EulerSolver :: CFL, EulerSolver :: STEPOUTPUT, 
    			argv [2], EulerSolver :: CHECKPOINTSTRIDE) ;

    {
      char fileName [2049] ;
      sprintf (fileName, "%s.%u", argv [2], grd.step ()) ;
      grd.backup (fileName) ;
    }
  }
  MPI_Finalize () ;
  return 0 ;
}
