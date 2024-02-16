      	// (c) bernhard schupp, 1997 - 1998
	
        // $Source$
        // $Revision$
        // $Name$
        // $State$

#include <iostream.h>

#include "gitter_euler_impl.h"

int main (int argc, const char ** argv, const char ** envp) {
  EulerSolver :: modifyGlobalConstants ("const_ffs.dat") ;
  {
    GitterEuler grd (argv [1]) ;

    {

      cout << "\n  -----------------------------------------------\n\n" ;
      cout << "   Daten / Gitter nach < " << argv [2] << " > schreiben" << endl ;
      
      if (argc > 3) {
        cout << "   Neustart aus < " << argv [3] << " > versuchen" << endl ;
        grd.restore (argv [3]) ;
      } else {
        grd.initialConstValue (EulerSolver :: Vec5 (1.4, 4.2, 0.0, 0.0, 8.8)) ;
	cout << "   Alle Elemente mit (1.4,4.2,0.0,0.0,8.8) initialisieren\n" ;
      }
      cout << "\n  -----------------------------------------------\n" << endl ;
    }
    grd.printsize () ;
    grd.runEulerSolver (EulerSolver :: ENDTIME, EulerSolver :: CFL, 
      	EulerSolver :: STEPOUTPUT, argv [2], EulerSolver :: CHECKPOINTSTRIDE) ;

    grd.backup ("data.final") ;
  }
  return 0 ;  
}


