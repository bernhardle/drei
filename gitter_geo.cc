	// (c) bernhard schupp, 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/optimized/RCS/gitter_geo.cc,v $
	// $Revision: 2.6 $
	// $Name:  $
	// $State: Exp $

#include <stdlib.h>
#include <assert.h>
#include <iostream.h>
#include <fstream.h>
#include <map.h>

#include "mapp_cube_3d.h"
#include "mapp_tetra_3d.h"
#include "gitter_sti.h"
#include "walk.h"

static volatile char RCSId_gitter_geo_cc [] = "$Id: gitter_geo.cc,v 2.6 1998/12/01 17:13:41 yc23 Exp yc23 $" ;

extern "C" { double drand48 (void) ; }

const pair < Gitter :: Geometric :: hasFace3 *, int > Gitter :: Geometric :: hface3 :: face3Neighbour :: null 
  = pair < Gitter :: Geometric :: hasFace3 *, int > (Gitter :: Geometric :: InternalHasFace3 ()(0), -1) ;

const int Gitter :: Geometric :: Tetra :: prototype [4][3] = {{1,3,2},{0,2,3},{0,3,1},{0,1,2}} ;

const pair < Gitter :: Geometric :: hasFace4 *, int > Gitter :: Geometric :: hface4 :: face4Neighbour :: null 
  = pair < Gitter :: Geometric :: hasFace4 *, int > (Gitter :: Geometric :: InternalHasFace4 ()(0), -1) ;

	//  Prototyp des Hexaeders wie er im Programm verwendet wird.
	//  Eckpunkte und Seitenflaechen:
	//
	//
	// 	        7---------6
	//	       /.        /|
	// 	      / .  1    / |
	// 	     /  .      /  |
	// 	    4---------5   | <-- 4 (hinten)
	//    5 --> |   .     | 3 |
	//    	    |   3.....|...2
	//          |  .      |  /
	//          | .   2   | / <-- 0 (unten)
	//          |.        |/
	//          0---------1
	//

const int Gitter :: Geometric :: Hexa :: prototype [6][4] = {{0,3,2,1},{4,5,6,7},{0,1,5,4},{1,2,6,5},{2,3,7,6},{0,4,7,3}} ;

int Gitter :: Geometric :: Hexa :: test () const {
  static const int v0[8][2] = {{0,0},{0,1},{0,2},{0,3},{1,0},{1,1},{1,2},{1,3}} ;
  static const int v1[8][2] = {{2,0},{4,1},{3,1},{2,1},{2,3},{2,2},{3,2},{4,2}} ;
  static const int v2[8][2] = {{5,0},{5,3},{4,0},{3,0},{5,1},{3,3},{4,3},{5,2}} ;
  int nfaults = 0 ;
  {
    for(int i = 0 ; i < 8 ; i ++ ) {
      int i0 = v0[i][0], j0 = v0[i][1] ;
      int i1 = v1[i][0], j1 = v1[i][1] ;
      int i2 = v2[i][0], j2 = v2[i][1] ;
      if(myvertex (i0, j0) != myvertex (i1, j1)) {
        cerr << "**FEHLER auf level: " << level () << " " ;
	cerr << "vertex (" << i0 << "," << j0 << ") != vertex (" << i1 << "," << j1 << ")";
	cerr << "\t(" << i0 << "," << j0 << ") =" << myvertex(i0,j0) << " " << twist (i0) ;
	cerr << "\t(" << i1 << "," << j1 << ") =" << myvertex(i1,j1) << " " << twist (i1) ;
	cerr << endl ; 
	nfaults ++ ;
      }
      if(myvertex (i0, j0) != myvertex (i2, j2)) {
        cerr << "**FEHLER auf level: " << level () << " " ;
	cerr << "vertex (" << i0 << "," << j0 << ") != vertex (" << i2 << "," << j2 << ")" ;
	cerr << "\t(" << i0 << "," << j0 << ") =" << myvertex(i0,j0) << " " << twist (i0) ;
	cerr << "\t(" << i2 << "," << j2 << ") =" << myvertex(i2,j2) << " " << twist (i1) ;
	cerr << endl;
	nfaults ++ ;
      }
    }
  }
  return nfaults ;
}

int Gitter :: Geometric :: Hexa :: tagForGlobalRefinement () {
  return (request (myrule_t :: iso8), 1) ;
}

static inline bool insideBall (const double (&p)[3], const double (&c)[3], double r) {
  return (((p [0] - c [0]) * (p [0] - c [0]) + (p [1] - c [1]) * (p [1] - c [1])
       	 + (p [2] - c [2]) * (p [2] - c [2])) < (r * r)) ? true : false ;
}

int Gitter :: Geometric :: Hexa :: tagForBallRefinement (const double (&center)[3], double radius, int limit) {
  bool hit = false ;
  for (int i = 0 ; i < 8 ; i ++) {
    const double (&p)[3] = myvertex (i)->Point () ;
    if (insideBall (p,center,radius)) { hit = true ; break ; }
  }
  if (!hit) {
    const int resolution = 50 ;
    TrilinearMapping map (myvertex(0)->Point(), myvertex(1)->Point(),
        myvertex(2)->Point(), myvertex(3)->Point(), myvertex(4)->Point(),
        myvertex(5)->Point(), myvertex(6)->Point(), myvertex(7)->Point()) ;
    double p [3] ;
    for (int i = 0 ; i < resolution ; i ++ ) {
      map.map2world (2.0 * drand48 () - 1.0, 2.0 * drand48 () - 1.0, 2.0 * drand48 () - 1.0, p) ;
      if (insideBall (p,center,radius)) { hit = true ; break ; }
    }
  }
  return hit ? (level () < limit ? (request (myrule_t :: iso8), 1) 
  	     : (request (myrule_t :: nosplit), 0)) : (request (myrule_t :: crs), 1) ;
}

int Gitter :: Geometric :: Tetra :: test () const {
  cerr << "**WARNUNG (IGNORIERT) Tetra :: test () nicht implementiert, in " <<  __FILE__ << " " << __LINE__  << endl;
  return 0 ;
}

int Gitter :: Geometric :: Tetra :: tagForGlobalRefinement () {
  return (request (myrule_t :: iso8), 1) ;
}

int Gitter :: Geometric :: Tetra :: tagForBallRefinement (const double (&center)[3], double radius, int limit) {
  bool hit = false ;
  for (int i = 0 ; i < 4 ; i ++) {
    const double (&p)[3] = myvertex (i)->Point () ;
    if (insideBall (p,center,radius)) { hit = true ; break ; }
  }
  if (!hit) {
    const int resolution = 50 ;
    LinearMapping map (myvertex(0)->Point(), myvertex(1)->Point(),
        		myvertex(2)->Point(), myvertex(3)->Point()) ;
    double p [3] ;
    for (int i = 0 ; i < resolution ; i ++ ) {
      double b1 = drand48 () ;
      double b2 = (1.0 - b1) * drand48 () ;
      double b3 = (1.0 - b1 - b2) * drand48 () ;
      double b4 = 1.0 - b1 - b2 - b3 ;
      
	// Sind das "uberhaupt Zufallspunkte ? Nein. Leider nicht.
	
      map.map2world (b1, b2, b3, b4, p) ;
      if (insideBall (p,center,radius)) { hit = true ; break ; }
    }
  }
  return hit ? (level () < limit ? (request (myrule_t :: iso8), 1) 
  	     : (request (myrule_t :: nosplit), 0)) : (request (myrule_t :: crs), 1) ;
}

Gitter :: Geometric :: BuilderIF :: ~BuilderIF () {
  if (iterators_attached ()) 
    cerr << "**WARNUNG (IGNORIERT) beim L\"oschen von BuilderIF: Iterator-Z\"ahler [" << iterators_attached () << "]" << endl ;
  {for (list < hexa_GEO * > :: iterator i = _hexaList.begin () ; i != _hexaList.end () ; delete (*i++)) ; }
  {for (list < tetra_GEO * > :: iterator i = _tetraList.begin () ; i != _tetraList.end () ; delete (*i++)) ; }
  {for (list < hbndseg4_GEO * > :: iterator i = _hbndseg4List.begin () ; i != _hbndseg4List.end () ; delete (*i++)) ; }
  {for (list < hbndseg3_GEO * > :: iterator i = _hbndseg3List.begin () ; i != _hbndseg3List.end () ; delete (*i++)) ; }
  {for (list < hface4_GEO * > :: iterator i = _hface4List.begin () ; i != _hface4List.end () ; delete (*i++)) ; }
  {for (list < hface3_GEO * > :: iterator i = _hface3List.begin () ; i != _hface3List.end () ; delete (*i++)) ; }
  {for (list < hedge1_GEO * > :: iterator i = _hedge1List.begin () ; i != _hedge1List.end () ; delete (*i++)) ; }
  {for (list < VertexGeo * > :: iterator i = _vertexList.begin () ; i != _vertexList.end () ; delete (*i++)) ; }
}

IteratorSTI < Gitter :: vertex_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const vertex_STI *) const {
  ListIterator < VertexGeo > w (_vertexList) ;
  return new Wrapper < ListIterator < VertexGeo >, InternalVertex > (w) ;
}

IteratorSTI < Gitter :: vertex_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const IteratorSTI < vertex_STI > * w) const {
  return new Wrapper < ListIterator < VertexGeo >, InternalVertex > (*(const Wrapper < ListIterator < VertexGeo >, InternalVertex > *) w) ;
}

IteratorSTI < Gitter :: hedge_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const hedge_STI *) const {
  ListIterator < hedge1_GEO > w (_hedge1List) ;
  return new Wrapper < ListIterator < hedge1_GEO >, InternalEdge > (w) ;
}

IteratorSTI < Gitter :: hedge_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const IteratorSTI < hedge_STI > * w) const {
  return new Wrapper < ListIterator < hedge1_GEO >, InternalEdge > (*(const Wrapper < ListIterator < hedge1_GEO >, InternalEdge > *)w) ;
}

IteratorSTI < Gitter :: hface_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const hface_STI *) const {
  ListIterator < hface4_GEO > w1 (_hface4List) ;
  ListIterator < hface3_GEO > w2 (_hface3List) ;
  return new AlignIterator < ListIterator < hface4_GEO >, ListIterator < hface3_GEO >, hface_STI > (w1,w2) ;
}

IteratorSTI < Gitter :: hface_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const IteratorSTI < hface_STI > * w) const {
  return new AlignIterator < ListIterator < hface4_GEO >, ListIterator < hface3_GEO >, hface_STI > 
  	(*(const AlignIterator < ListIterator < hface4_GEO >, ListIterator < hface3_GEO >, hface_STI > *)w) ;
}

IteratorSTI < Gitter :: hbndseg_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const hbndseg_STI *) const {
  ListIterator < hbndseg4_GEO > w1 (_hbndseg4List) ;
  ListIterator < hbndseg3_GEO > w2 (_hbndseg3List) ;
  return new AlignIterator < ListIterator < hbndseg4_GEO >, ListIterator < hbndseg3_GEO >, hbndseg_STI > (w1,w2) ;
}

IteratorSTI < Gitter :: hbndseg_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const IteratorSTI < hbndseg_STI > * w) const {
  return new AlignIterator < ListIterator < hbndseg4_GEO >, ListIterator < hbndseg3_GEO >, hbndseg_STI > 
            (*(const AlignIterator < ListIterator < hbndseg4_GEO >, ListIterator < hbndseg3_GEO >, hbndseg_STI > *)w) ;;
}

IteratorSTI < Gitter :: helement_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const helement_STI *) const {
  ListIterator < hexa_GEO > w1 (_hexaList) ;
  ListIterator < tetra_GEO > w2 (_tetraList) ;
  return new AlignIterator < ListIterator < hexa_GEO >, ListIterator < tetra_GEO >, helement_STI > (w1,w2) ;
}

IteratorSTI < Gitter :: helement_STI > * Gitter :: Geometric :: BuilderIF :: iterator (const IteratorSTI < helement_STI > * w) const {
  return new AlignIterator < ListIterator < hexa_GEO >, ListIterator < tetra_GEO >, helement_STI >
  		(*(const AlignIterator < ListIterator < hexa_GEO >, ListIterator < tetra_GEO >, helement_STI > *)w) ;
}

void Gitter :: Geometric :: BuilderIF :: backupCMode (ostream & os) const {

	// Das Compatibility Mode Backup sichert das Makrogitter genau
	// dann, wenn es zwischenzeitlich ge"andert wurde, was beim
	// Neuanlegen und bei der Lastverteilung der Fall ist.

  map < VertexGeo *, int, less < VertexGeo * > > vm ;
  os.setf (ios::fixed, ios::floatfield) ;
  os.precision (16) ;
  
	// Bisher enth"alt die erste Zeile der Datei entweder "!Tetraeder"
	// oder "!Hexaeder" je nachdem, ob ein reines Tetraeder- oder
	// Hexaedernetz vorliegt. Gemischte Netze sind bez"uglich ihres
	// Dateiformats noch nicht spezifiziert.
  
  if (_tetraList.size () == 0) {
    os << "!Hexaeder" << endl ;
  } else if (_hexaList.size () == 0 && _tetraList.size () != 0) {
    os << "!Tetraeder" << endl ;
  } else {
    cerr << "**WARNUNG (IGNORIERT) Gitter :: Geometric :: BuilderIF :: backupCMode (ostream &)" ;
    cerr << "  schreibt nur entweder reine Hexaedernetze oder reine Tetraedernetze." ;
    cerr << " In " << __FILE__ << " " << __LINE__ << endl ;
  }
  
	// In jedem Fall die Vertexkoordinaten rausschreiben.
  
  os << _vertexList.size () << endl ;
  {
    int index (0) ;
    for (list < VertexGeo * > :: const_iterator i = _vertexList.begin () ; i != _vertexList.end () ; i ++) {
      os << (*i)->Point ()[0] << " " << (*i)->Point ()[1] << " " << (*i)->Point ()[2] << endl ;
      vm [*i] = index ++ ;
    }
  }
  if (_tetraList.size () == 0) {
    assert (_hbndseg3List.size () == 0) ;
    os << _hexaList.size () << endl ;
    {
      for (list < hexa_GEO * > :: const_iterator i = _hexaList.begin () ; i != _hexaList.end () ; i ++ ) {
        for (int j = 0 ; j < 8 ; os << vm [(*i)->myvertex (j ++)] << "  ") ;
        os << endl ;
      }
    }
    os << _hbndseg4List.size () << endl ;
    {
      for (list < hbndseg4_GEO * > :: const_iterator i = _hbndseg4List.begin () ; i != _hbndseg4List.end () ; i ++) {
        os << -(int)(*i)->bndtype () << "  " << 4 << "  " ;
        for (int j = 0 ; j < 4 ; os << vm [(*i)->myvertex (0,j ++)] << "  ") ;
        os << endl ;
      }
    }
  } else if (_hexaList.size () == 0 && _tetraList.size () != 0) {
    os << _tetraList.size () << endl ;
    {
      for (list < tetra_GEO * > :: const_iterator i = _tetraList.begin () ; i != _tetraList.end () ; i ++ ) {
        for (int j = 0 ; j < 4 ; os << vm [(*i)->myvertex (j ++)] << "  ") ;
        os << endl ;
      }
    }
    os << _hbndseg3List.size () << endl ;
    {
      for (list < hbndseg3_GEO * > :: const_iterator i = _hbndseg3List.begin () ; i != _hbndseg3List.end () ; i ++) {
        os << -(int)(*i)->bndtype () << "  " << 3 << "  " ;
        for (int j = 0 ; j < 3 ; os << vm [(*i)->myvertex (0,j ++)] << "  ") ;
        os << endl ;
      }
    }
  }
  {
  	// Die Vertexidentifierliste hinten anh"angen, damit ein verteiltes
	// Grobgitter wieder zusammengefunden wird.
  
    for (list < VertexGeo * > :: const_iterator i = _vertexList.begin () ; i != _vertexList.end () ; i ++)
      os << (*i)->ident () << " " << -1 << endl ;
  }
	// Die Modified - Markierung zur"ucksetzen.
	// Bisher leider noch mit 'cast around const' feature.
  
  ((BuilderIF *)this)->_modified = false ;
  return ;
}

void Gitter :: Geometric :: BuilderIF :: backupCMode (const char * fileName) const {
  if (_modified) {
    char * name = new char [strlen (fileName) + 20] ;
    sprintf (name, "%s.makro", fileName) ;
    ofstream out (name) ;
    if (out) {
      backupCMode (out) ;
    } else {
      cerr << "**WARNUNG (IGNORIERT) in Gitter :: Geometric :: BuilderIF :: backupCMode (const char *)" ;
      cerr << " beim Anlegen der Datei < " << name << " > in " << __FILE__ << " " << __LINE__ << endl ;
    }
    delete [] name ;
  }
  return ;
}

void Gitter :: Geometric :: BuilderIF :: backup (ostream & os) const {

	// Man sollte sich erstmal darauf einigen, wie ein allgemeines Gitterdateiformat 
	// auszusehen hat f"ur Hexaeder, Tetraeder und alle m"oglichen Pyramiden.
	// Solange leiten wir die Methodenaufrufe auf backupCMode (..) um.
	
  cerr << "**WARNUNG (IGNORIERT) Gitter :: Geometric :: BuilderIF :: backup (ostream &) " ;
  cerr << " nach Gitter :: Geometric :: BuilderIF :: backupCMode (ostream &) umgeleitet " << endl ;
  
  backupCMode (os) ;
  return ;
}

void Gitter :: Geometric :: BuilderIF :: backup (const char * fileName) const {

  cerr << "**WARNUNG (IGNORIERT) Gitter :: Geometric :: BuilderIF :: backup (const char *) " ;
  cerr << " nach Gitter :: Geometric :: BuilderIF :: backupCMode (const char *) umgeleitet " << endl ;

  backupCMode (fileName) ;
  return ;
}
