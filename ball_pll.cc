	// (c) bernhard schupp, 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/ball_pll.cc,v $
	// $Revision: 2.2 $
	// $Name:  $
	// $State: Exp $
	// $Date$

	// Testprogramm zum Durchschieben eines B"allchens
	// durch das Gitter l"angs der x-Achse. Das B"allchen
	// l"auft ausserhalb des W"urfels [0,1]^3 los und 
	// bleibt dann ausserhalb des W"urfels stehen. Das
	// Programm kann auch parallel verwendet werden.
	// Vgl. auch ball.cc (ausschliesslich seriell).

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream.h>

#include "mpAccess_MPI.h"
#include "gitter_pll_impl.h"

static volatile char RCSId_ball_pll_cc [] = "$Id: ball_pll.cc,v 2.2 1998/10/29 11:39:18 yc23 Exp yc23 $" ;

int main (int argc, char ** argv, char **) {
  MPI_Init (&argc, &argv) ;
  {
    MpAccessMPI mpa (MPI_COMM_WORLD) ;
  
    cout << "\n  -------------------------------------------------------------\n" ;
    cout <<   "  ball_pll () :                                                    \n" ;
    cout <<   "  Grobgitter des W\"urfels aus < " << argv [1] << " > einlesen\n" ;
    GitterBasisPll part (argv[1], mpa) ;
    cout << "\n  -------------------------------------------------------------\n" << endl ;
    
    int res = 100, lim = 3, stp = 1 ;
 
    cout << "  Anzahl der Schritte eingeben [1 ...] " << endl ;
    cin >> res ;
    cout << "  Limit f\"ur das Level eingeben [0 ...] " << endl ;
    cin >> lim ;
    cout << "  Anzahl der zu schreibenden Schritte [0 ... " << res << "] " << endl ;
    cin >> stp ;
   
    assert (res >= 1 && lim > 0 && stp >= 0) ;
    const int s = stp ? (res + stp - 1) / stp : 0 ;
    const double rad = 0.15, inc = (1. + 2.4 * rad)/double (res) ;
    double cen [3] = {- 1.2 * rad, .5, .5} ;
    cout << "\n\n  B\"allchen mit Radius " << rad << " in " << res << " Schritten durch den W\"urfel schieben ..." << endl ;
    for (int i = 0 ; i < res + 1; i ++) {
      int start = clock () ;
      part.refineBall (cen,rad,lim) ;
      if (!mpa.myrank ()) cout << "  " << i << ". Schritt, Mittelpunkt bei (" << cen [0] << "," << cen [1] << "," << cen [2] << ")" << endl ;
      cen [0] += inc ;
      if (s ? !(i % s) : 0) {
        part.printsize () ;
//        char name [1025] ;
//        sprintf (name, "ball_pll_%u.bak", i) ;
//        part.backup (name) ;
      }
    }
  }
  MPI_Finalize () ;
  
  return 0 ;
}