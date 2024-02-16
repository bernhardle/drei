	// (c) bernhard schupp 1997 - 1998
	
	// $Source$
	// $Revision$
	// $Name$
	// $State$
	
	// Testprogramm zum Durchschieben eines B"allchens
	// durch das Gitter l"angs der x-Achse. Das B"allchen
	// l"auft ausserhalb des W"urfels [-1,1]^3 los und 
	// bleibt dann ausserhalb des W"urfels stehen.

#include <stdlib.h>
#include <iostream.h>
#include <strstream.h>
#include "gitter_mgb.h"
#include "gitter_impl.h"

static volatile char RCSId_ball_cc [] = "$Id$" ;

int main(int argc, char ** argv, char ** envp) {

  cout << "\n  ------------------------------------\n\n" ;
  cout <<   "  ball :                              \n" ;
  cout << "\n  ------------------------------------\n" << endl ;

  strstream vfile ;

  int n = 0 ;
  cout << "  Aufl\"osung aller Raumrichtungen im W\"urfel eingeben [0 ...] " ;
  cin >> n ;
  
  MacroGridBuilder :: cubeHexaGrid (n, vfile) ;

  GitterBasisImpl grd (vfile) ;

  int res = 100, lim = 1, stp = 1 ;
  cout << "  Anzahl der Schritte eingeben [1 ...] " ;
  cin >> res ;
  cout << "  Limit f\"ur das Level eingeben [0 ...] " ;
  cin >> lim ;
  cout << "  Anzahl der zu schreibenden Schritte [0 ... " << res << "] " ;
  cin >> stp ;
  assert (res >= 1 && lim > 0 && stp >= 0) ;
  const int s = stp ? (res + stp - 1) / stp : 0 ;
  const double rad = 0.07, inc = (1. + 2.4 * rad)/double (res) ;
  double cen [3] = {- 1.2 * rad, .5, .5} ;
  cout << "  B\"allchen mit Radius " << rad << " durchschieben ..." << endl ;
  for (int i = 0 ; i < res + 1; i ++) {
    grd.refineBall (cen,rad,lim) ;
    cout << "  " << i << ". Schritt, Mittelpunkt bei (" << cen [0] << "," << cen [1] << "," << cen [2] << ")" << endl ;
    cen [0] += inc ;
    if (s ? !(i % s) : 0) {
      grd.printsize () ;
      char name [1025] ;
      sprintf (name, "schritt_%u.bak", i) ;
      grd.backupCMode (name) ;
    }
  }
  return 0 ;
}