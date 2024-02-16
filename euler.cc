	// (c) bernhard schupp 1997 - 1998
	
	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/euler.cc,v $
	// $Revision: 2.3 $
	// $Name:  $
	// $State: Exp $
	// $Date$

#include <fstream.h>
#include "euler.h"

static volatile char RCSId_euler_cc [] = "$Id: euler.cc,v 2.3 1998/12/01 17:13:41 yc23 Exp yc23 $" ;

const double EulerSolver :: s2 = 1.41421356237309510000 ;
const double EulerSolver :: GAMMA = 1.4 ;
const double EulerSolver :: g1 = 0.4 ;

double EulerSolver :: CFL = 0.4 ;
double EulerSolver :: ENDTIME    = 5.0/3.0 ;
int    EulerSolver :: STEPOUTPUT = 100 ;
int    EulerSolver :: MAXLEVEL = 2 ;
double EulerSolver :: JUMP_H = 0.15 ;
double EulerSolver :: JUMP_L = 0.10 ;
int    EulerSolver :: CHECKPOINTSTRIDE = -1 ;

ostream & operator << (ostream & out, const EulerSolver :: Vec5 & v) {
  out.precision (14) ;
  out << v.u [0] << " " << v.u [1] << " " << v.u [2] << " " << v.u[3] << " " << v.u [4] << endl ;
  return out ;
}

istream & operator >> (istream & in, EulerSolver :: Vec5 & v) {
  in >> v.u [0] >> v.u [1] >> v.u [2] >> v.u [3] >> v.u [4] ;
  return in ;
}

void EulerSolver :: EulerData :: writeObject (ObjectStream & os) const {
  os.writeObject (_a [0]) ;
  os.writeObject (_a [1]) ;
  os.writeObject (_a [2]) ;
  os.writeObject (_a [3]) ;
  os.writeObject (_a [4]) ;
  return ;
}

bool EulerSolver :: EulerData :: readObject (ObjectStream & os) throw (ObjectStream :: EOFException) {
  os.readObject (_a [0]) ;
  os.readObject (_a [1]) ;
  os.readObject (_a [2]) ;
  os.readObject (_a [3]) ;
  os.readObject (_a [4]) ;
  return true ;
}

void EulerSolver :: InterpolationCube2Dzero :: writeObject (ObjectStream & os) const {
  os.writeObject (_base [0]) ;
  os.writeObject (_base [1]) ;
  os.writeObject (_base [2]) ;
  os.writeObject (_base [3]) ;
  os.writeObject (_base [4]) ;
  return ;
}

bool EulerSolver :: InterpolationCube2Dzero :: readObject (ObjectStream & os) throw (ObjectStream :: EOFException) {
  os.readObject (_base [0]) ;
  os.readObject (_base [1]) ;
  os.readObject (_base [2]) ;
  os.readObject (_base [3]) ;
  os.readObject (_base [4]) ;
  return true ;
}

void EulerSolver :: InterpolationSimplex2Dfirst :: writeObject (ObjectStream & os) const {
  for (int i = 0 ; i < 5 ; i ++) {
    os.writeObject (_d0 [i]) ;
    os.writeObject (_d1 [i]) ;
    os.writeObject (_d2 [i]) ;
  }
  return ;
}

bool EulerSolver :: InterpolationSimplex2Dfirst :: readObject (ObjectStream & os) throw (ObjectStream :: EOFException) {
  for (int i = 0 ; i < 5 ; i++) {
    os.readObject (_d0 [i]) ;
    os.readObject (_d1 [i]) ;
    os.readObject (_d2 [i]) ;
  }
  return false ;
}

EulerSolver :: InterpolationSimplex2Dfirst :: val_t EulerSolver :: InterpolationSimplex2Dfirst :: operator () (coord_t x) const {
  Vec5 res = _d0 ;
  res *= x [0] ;
  res += (Vec5(_d1) *= x [1]) ;
  res += (Vec5(_d2) *= x [2]) ;
  return res /= 3.0 ;
}

void EulerSolver :: InterpolationCube2Dfirst :: writeObject (ObjectStream & os) const {
  for (int i = 0 ; i < 5 ; i ++) {
    os.writeObject (_d0 [i]) ;
    os.writeObject (_d1 [i]) ;
    os.writeObject (_d2 [i]) ;
    os.writeObject (_d3 [i]) ;
  }
  return ;
}

bool EulerSolver :: InterpolationCube2Dfirst :: readObject (ObjectStream & os) throw (ObjectStream :: EOFException) {
  for (int i = 0 ; i < 5 ; i++) {
    os.readObject (_d0 [i]) ;
    os.readObject (_d1 [i]) ;
    os.readObject (_d2 [i]) ;
    os.readObject (_d3 [i]) ;
  }
  return false ;
}

EulerSolver :: Vec5 EulerSolver :: InterpolationCube2Dfirst :: operator () (const double (&x)[2]) const {
  assert (!(fabs(x [0]) > 1.)) ;
  assert (!(fabs(x [1]) > 1.)) ;
  Vec5 res = Vec5 (.0, .0, .0, .0, .0) ;
  res += (Vec5(_d0) *= ((1. - x [0]) * (1. - x [1]))) ;
  res += (Vec5(_d1) *= ((1. - x [0]) * (1. + x [1]))) ;
  res += (Vec5(_d2) *= ((1. + x [0]) * (1. + x [1]))) ;
  res += (Vec5(_d3) *= ((1. + x [0]) * (1. - x [1]))) ;
  res *= .25 ;
  return res ;
}

void EulerSolver :: modifyGlobalConstants (const char * name) {
  double x ;
  int i ;
  cout << "\n EulerSolver :: modifyGlobalConstants () :   \n" ;
  ifstream in (name) ;
  if (in) {
    cout << "  lesen aus Datei < " << name << " > \n" ;
    in >> x ;
    cout << "   CFL : .............. " ;
    if (in.good ()) {
      cout << CFL << " -> " << x << "\n" ;
      CFL = x ;
    } else {
      cout << CFL << " -> " << CFL << "\n" ;
    }
    in >> x ;
    cout << "   ENDTIME : .......... " ;
    if (in.good ()) {
      cout << ENDTIME << " -> " << x << "\n" ;
      ENDTIME = x ;
    } else {
      cout << ENDTIME << " -> " << ENDTIME << "\n" ;
    }
    in >> i ;
    cout << "   STEPOUTPUT : ....... " ;
    if (in.good ()) {
      cout << STEPOUTPUT << " -> " << i << "\n" ;
      STEPOUTPUT = i ;
    } else {
      cout << STEPOUTPUT << " -> " << STEPOUTPUT << "\n" ;
    }
    in >> x ;
    cout << "   JUMP_H : ........... " ;
    if (in.good ()) {
      cout << JUMP_H << " -> " << x << "\n" ;
      JUMP_H = x ;
    } else {
      cout << JUMP_H << " -> " << JUMP_H << "\n" ;
    }
    in >> x ;
    cout << "   JUMP_L : ........... " ;
    if (in.good ()) {
      cout << JUMP_L << " -> " << x << "\n" ;
      JUMP_L = x ;
    } else {
      cout << JUMP_L << " -> " << JUMP_L << "\n" ;
    }
    in >> i ;
    cout << "   MAXLEVEL : ......... " ;
    if (in.good ()) {
      cout << MAXLEVEL << " -> " << i << "\n" ;
      MAXLEVEL = i ;
    } else {
      cout << MAXLEVEL << " -> " << MAXLEVEL << "\n" ;
    }
    in >> i ;
    cout << "   CHECKPOINTSTRIDE : . " ;
    if (in.good ()) {
      cout << CHECKPOINTSTRIDE << " -> " << i << "\n" ;
      CHECKPOINTSTRIDE = i ;
    } else {
      cout << CHECKPOINTSTRIDE << " -> " << CHECKPOINTSTRIDE << "\n" ;
    }
    cout << endl ;
  }
  else {
    cout << "  Datei < " << name << " > konnte nicht g\"offnet werden " << endl ;
    cout << "  Default-Werte werden eingesetzt\n" ; 
    cout << "   CFL : .............. " << CFL << "\n" ;
    cout << "   ENDTIME : .......... " << ENDTIME << "\n" ;
    cout << "   STEPOUTPUT : ....... " << STEPOUTPUT << "\n" ;
    cout << "   JUMP_H : ........... " << JUMP_H << "\n" ;
    cout << "   JUMP_L : ........... " << JUMP_L << "\n" ;
    cout << "   MAXLEVEL : ......... " << MAXLEVEL << "\n" ;
    cout << "   CHECKPOINTSTRIDE : . " << CHECKPOINTSTRIDE << "\n" ;
    cout << endl ;
  }
}