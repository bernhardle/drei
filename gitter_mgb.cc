	// (c) bernhard schupp 1997 - 1998

	// $Source$
	// $Revision$
	// $Name$
	// $State$

#include <assert.h>
#include <time.h>
#include <strstream.h>
#include <iterator.h>
#include <vector.h>
#include <utility.h>
#include <set.h>
#include <map.h>
#include <algorithm.h>

#include "gitter_sti.h"
#include "gitter_mgb.h"

static volatile char RCSId_gitter_mgb_cc [] = "$Id$" ;

pair < Gitter :: Geometric :: VertexGeo *, bool > MacroGridBuilder :: InsertUniqueVertex (double x, double y, double z, int i) {
  vertexMap_t :: const_iterator hit = _vertexMap.find (i) ;
  if (hit == _vertexMap.end ()) {
    VertexGeo * v = myBuilder ().insert_vertex (x,y,z,i) ;
    _vertexMap [i] = v ;
    return pair < VertexGeo *, bool > (v,true) ;
  } else {
    return pair < VertexGeo *, bool > ((*hit).second, false) ;
  }
}
 
pair < Gitter :: Geometric :: hedge1_GEO *, bool > MacroGridBuilder :: InsertUniqueHedge1 (int l, int r) {
  if (l > r) { 
    int i = l ; l = r ; r = i ;
  }
  edgeKey_t key (l,r) ;
  edgeMap_t :: const_iterator hit = _edgeMap.find (key) ;
  if (hit == _edgeMap.end ()) {
    vertexMap_t :: const_iterator a = _vertexMap.find (l), b = _vertexMap.find (r), end = _vertexMap.end () ;
    if (a == end || b == end) abort () ;
    hedge1_GEO * h = myBuilder ().insert_hedge1 ((*a).second,(*b).second) ;
    _edgeMap [key] = h ;
    return pair < hedge1_GEO *, bool > (h,true) ;
  } else {
    return pair < hedge1_GEO *, bool > ((*hit).second,false) ;
  }
}

pair < Gitter :: Geometric :: hface3_GEO *, bool > MacroGridBuilder :: InsertUniqueHface3 (int (&v)[3]) {
  cyclicReorder (v,v+3) ;
  faceKey_t key (v[0],v[1],v[2]) ;
  faceMap_t :: const_iterator hit = _face3Map.find (key) ;
  if (hit == _face3Map.end ()) {
    const int check = _edgeMap.size () ;
    hedge1_GEO * edge [3] ;
    int dire [3] = { 0, 0, 1 } ;
    edge [0] = InsertUniqueHedge1 (v[0],v[1]).first ;
    edge [1] = InsertUniqueHedge1 (v[1],v[2]).first ;
    edge [2] = InsertUniqueHedge1 (v[2],v[0]).first ;
    hface3_GEO * f3 = myBuilder ().insert_hface3 (edge,dire) ;
    _face3Map [key] = f3 ;
    return pair < hface3_GEO *, bool > (f3,true) ;
  } else {
    return pair < hface3_GEO *, bool > ((hface3_GEO *)(*hit).second,false) ;
  }
}

pair < Gitter :: Geometric :: hface4_GEO *, bool > MacroGridBuilder :: InsertUniqueHface4 (int (&v)[4]) {
  cyclicReorder (v,v+4) ;
  faceKey_t key (v[0],v[1],v[2]) ;
  faceMap_t :: const_iterator hit = _face4Map.find (key) ;
  if (hit == _face4Map.end ()) {
    hedge1_GEO * edge [4] ;
    int dire [4], check = _edgeMap.size () ;
    edge [0] = InsertUniqueHedge1 (v[0],v[1]).first ;
    edge [1] = InsertUniqueHedge1 (v[1],v[2]).first ;
    edge [2] = InsertUniqueHedge1 (v[2],v[3]).first ;
    edge [3] = InsertUniqueHedge1 (v[3],v[0]).first ;  
    dire [0] = v[0] < v[1] ? 0 : 1 ;
    dire [1] = v[1] < v[2] ? 0 : 1 ;
    dire [2] = v[2] < v[3] ? 0 : 1 ;
    dire [3] = v[3] < v[0] ? 0 : 1 ;
    hface4_GEO * f4 = myBuilder ().insert_hface4 (edge,dire) ;
    _face4Map [key] = f4 ;
    return pair < hface4_GEO *, bool > (f4,true) ;
  } else {
    return pair < hface4_GEO *, bool > ((hface4_GEO *)(*hit).second,false) ;
  }
}

pair < Gitter :: Geometric :: tetra_GEO *, bool > MacroGridBuilder :: InsertUniqueTetra (int (&v)[4]) {
  elementKey_t key (v [0], v [1], v [2], v [3]) ;
  elementMap_t :: const_iterator hit = _tetraMap.find (key) ;
  if (hit == _tetraMap.end ()) {
    hface3_GEO * face [4] ;
    int twst [4] ;
    for (int fce = 0 ; fce < 4 ; fce ++ ) {
      int x [3] ;
      x [0] = v [Tetra :: prototype [fce][0]] ;
      x [1] = v [Tetra :: prototype [fce][1]] ;
      x [2] = v [Tetra :: prototype [fce][2]] ;
      twst [fce] = cyclicReorder (x,x+3) ;
      face [fce] =  InsertUniqueHface3 (x).first ;
    }
    tetra_GEO * t = myBuilder ().insert_tetra (face,twst) ;
    assert (t) ;
    _tetraMap [key] = t ;
    return pair < tetra_GEO *, bool > (t,true) ;
  } else {
    return pair < tetra_GEO *, bool > ((tetra_GEO *)(*hit).second,false) ;
  }
}

void MacroGridBuilder :: removeElement (const elementKey_t & k) {
  elementMap_t :: iterator hit = _tetraMap.find (k) ;
  if (hit == _tetraMap.end ()) {
    hit = _hexaMap.find (k) ;
    assert (hit != _hexaMap.end ()) ;
    hexa_GEO * hx = (hexa_GEO *)(*hit).second ;
    for (int i = 0 ; i < 6 ; i ++) {
      _hbnd4Int [faceKey_t (hx->myhface4 (i)->myvertex (0)->ident (), hx->myhface4 (i)->myvertex (1)->ident (), 
    	hx->myhface4 (i)->myvertex (2)->ident ())] = new pair < hface4_GEO *, int > (hx->myhface4 (i), hx->twist (i)) ;
    }
    delete hx ;
    _hexaMap.erase (hit) ;
  } else {
    assert (_hexaMap.find (k) == _hexaMap.end ()) ;
    tetra_GEO * tr = (tetra_GEO *)(*hit).second ;
    for (int i = 0 ; i < 4 ; i ++) {
    _hbnd3Int [faceKey_t (tr->myhface3 (i)->myvertex (0)->ident (), tr->myhface3 (i)->myvertex (1)->ident (), 
    	tr->myhface3 (i)->myvertex (2)->ident ())] = new pair < hface3_GEO *, int > (tr->myhface3 (i), tr->twist (i)) ;
    }
    delete tr ;
    _tetraMap.erase (hit) ;
  } 
  return ;
}

void MacroGridBuilder :: removeTetra (int (&v)[4]) {
  elementMap_t :: iterator hit = _tetraMap.find (elementKey_t (v[0], v[1], v[2], v[3])) ;
  assert (hit != _tetraMap.end ()) ;
  tetra_GEO * tr = (tetra_GEO *)(*hit).second ;
  for (int i = 0 ; i < 4 ; i ++) {
    _hbnd3Int [faceKey_t  (tr->myhface3 (i)->myvertex (0)->ident (), tr->myhface3 (i)->myvertex (1)->ident (), 
    	tr->myhface3 (i)->myvertex (2)->ident ())] = new pair < hface3_GEO *, int > (tr->myhface3 (i), tr->twist (i)) ;
  }
  delete tr ;
  _tetraMap.erase (hit) ;
  return ;
}

pair < Gitter :: Geometric :: hexa_GEO *, bool > MacroGridBuilder :: InsertUniqueHexa (int (&v)[8]) {
  elementKey_t key (v [0], v [1], v [3], v[4]) ;
  elementMap_t :: const_iterator hit = _hexaMap.find (key) ;
  if (hit == _hexaMap.end ()) {
    hface4_GEO * face [6] ;
    int twst [6] ;
    for (int fce = 0 ; fce < 6 ; fce ++) {
      int x [4] ;
      x [0] = v [Hexa :: prototype [fce][0]] ;
      x [1] = v [Hexa :: prototype [fce][1]] ;
      x [2] = v [Hexa :: prototype [fce][2]] ;
      x [3] = v [Hexa :: prototype [fce][3]] ;
      twst [fce] = cyclicReorder (x,x+4) ;
      face [fce] =  InsertUniqueHface4 (x).first ;
    }
    hexa_GEO * hx = myBuilder ().insert_hexa (face,twst) ;
    _hexaMap [key] = hx ;
    return pair < hexa_GEO *, bool > (hx,true) ;
  } else {
    return pair < hexa_GEO *, bool > ((hexa_GEO *)(*hit).second,false) ;
  }
}

void MacroGridBuilder :: removeHexa (int (&v)[8]) {
  elementMap_t :: iterator hit = _hexaMap.find (elementKey_t (v[0], v[1], v[3], v[4])) ;
  assert (hit != _hexaMap.end ()) ;
  hexa_GEO * hx = (hexa_GEO *)(*hit).second ;
  for (int i = 0 ; i < 6 ; i ++) {
    _hbnd4Int [faceKey_t (hx->myhface4 (i)->myvertex (0)->ident (), hx->myhface4 (i)->myvertex (1)->ident (), 
    	hx->myhface4 (i)->myvertex (2)->ident ())] = new pair < hface4_GEO *, int > (hx->myhface4 (i), hx->twist (i)) ;
  }
  delete hx ;
  _hexaMap.erase (hit) ;
  return ;
}

bool MacroGridBuilder :: InsertUniqueHbnd3 (int (&v)[3], Gitter :: hbndseg_STI ::bnd_t bt) {
  int twst = cyclicReorder (v,v+3) ;
  faceKey_t key (v [0], v [1], v [2]) ;
  if (bt == Gitter :: hbndseg_STI :: closure) {
    if (_hbnd3Int.find (key) == _hbnd3Int.end ()) {
      hface3_GEO * face =  InsertUniqueHface3 (v).first ;
      _hbnd3Int [key] = (void *) new pair < hface3_GEO *, int > (face,twst) ;
      return true ;
    }
  } else {
    if (_hbnd3Map.find (key) == _hbnd3Map.end ()) {
      hface3_GEO * face =  InsertUniqueHface3 (v).first ;
      hbndseg3_GEO * hb3 = myBuilder ().insert_hbnd3 (face,twst,bt) ;
      _hbnd3Map [key] = hb3 ;
      return true ;
    }
  }
  return false ;
}

bool MacroGridBuilder :: InsertUniqueHbnd4 (int (&v)[4], Gitter :: hbndseg_STI ::bnd_t bt) {
  int twst = cyclicReorder (v,v+4) ;
  faceKey_t key (v [0], v [1], v [2]) ;
  if (bt == Gitter :: hbndseg_STI :: closure) {
    if (_hbnd4Int.find (key) == _hbnd4Int.end ()) {
      hface4_GEO * face =  InsertUniqueHface4 (v).first ;
      _hbnd4Int [key] = (void *) new pair < hface4_GEO *, int > (face,twst) ;
      return true ;
    }
  } else {
    if (_hbnd4Map.find (key) == _hbnd4Map.end ()) {
      hface4_GEO * face =  InsertUniqueHface4 (v).first ;
      hbndseg4_GEO * hb4 = myBuilder ().insert_hbnd4 (face,twst,bt) ;
      _hbnd4Map [key] = hb4 ;
      return true ;
    }
  }
  return false ;
}

void MacroGridBuilder :: cubeHexaGrid (int n, ostream & out) {

	// cubeHexaGrid () ist eine statische Methode, die einen ASCII Strom
	// mit einem gleichm"assigen Hexaedernetz auf dem Einheitsw"urfel
	// [0,1]^3 beschreibt, wobei <n> die Aufl"osung der Raumrichtungen
	// vorgibt: Es entstehen n^3 Hexaederelemente, und (n+1)^3 Knoten.
	// Es ist als Servicemethode f"ur die 'ball' und 'ball_pll' Test-
	// programme n"otig und deshalb in der MacrogridBuilder Klasse 
	// beheimatet.

  const int bndtype = -1 ;
  out.setf(ios::fixed, ios::floatfield) ;
  out.precision (14) ;
  n = n < 0 ? 0 : n ;
  int npe = n + 1 ;
  out << (npe * npe * npe) << endl ;
  double delta = 1.0 / (double)(n) ;
  {
    for(int i = 0 ; i < npe ; i ++) {
      for(int j = 0 ; j < npe ; j ++) {
        for(int k = 0 ; k < npe ; k ++) {
          out << double(k * delta) << "  "  << double(j * delta) << "  " << double(i * delta) << "\n" ;
        }
      }
    }
  }
  out << n * n * n << "\n" ;
  {
    for(int i = 0 ; i < n ; i ++) {
      int ipea = (i + 1) * npe * npe, ia = i * npe * npe ;
      for(int j = 0 ; j < n ; j ++) {
        int jpea = (j + 1) * npe, ja = j * npe ;  
        for(int k = 0 ; k < n ; k ++) {
          int kpe = k + 1 ;  
          out << k + ia + ja << "  " << kpe + ia + ja << "  "  << kpe + ia + jpea << "  " << k + ia + jpea << "  "         
              << k + ja + ipea << "  " << kpe + ja + ipea << "  " << kpe + ipea + jpea << "  " << k + ipea + jpea << "\n" ; 
        }  
      }    
    }
    out << endl ;  
  }
  out << 6 * n * n << endl ;
  {	// unten und oben
    int l = n * npe * npe ;
    for(int j = 0 ; j < n ; j ++) {
      int jpea = (j + 1) * npe, ja = j * npe ;
      for(int k = 0 ; k < n ; k ++) {
        int kpe = k + 1 ;
        out << bndtype << "  " << 4 << "  " << (kpe + ja) << "  " << (kpe + jpea) << "  "
            << (k + jpea) << "  " << (k + ja) << "\n" << bndtype << "  " << 4 << "  "
            << (k + jpea + l) << "  " << (kpe + jpea + l) << "  " << (kpe + ja + l) << "  " << (k + ja + l) << "\n" ;
      }
    }
    out << endl ;
  }
  {	// links und rechts
    int l = n * npe ;
    for(int j = 0 ; j < n ; j ++) {
      int jpea = (j + 1) * npe * npe, ja = j * npe * npe ;
      for(int ka = 0 ; ka < n ; ka ++) {
        int kpea = (ka + 1) ;
        out << bndtype << "  " << 4 << "  " << ka + jpea << "  " << kpea + jpea << "  " 
            << kpea + ja << "  " << ka + ja << "\n" << bndtype << "  " << 4 << "  " 
	    << kpea + ja + l << "  " << kpea + jpea + l << "  " << ka + jpea + l << "  " << ka + ja + l << "\n" ;
      }
    }
    out << endl ;
  }
  {	// hinten und vorne
    int l = n ;
    for(int j = 0 ; j < n ; j ++) {
      int jpea = (j + 1) * npe * npe, ja = j * npe * npe ;
      for(int k = 0 ; k < n ; k ++) {
        int kpea = (k + 1) * npe, ka = k * npe ;
        out << bndtype << "  " << 4 << "  " << kpea + ja << "  " << kpea + jpea << "  " 
            << ka + jpea << "  " << ka + ja << "\n" << bndtype << "  " << 4 << "  " 
            << ka + jpea + l << "  " << kpea + jpea + l << "  " << kpea + ja + l << "  " << ka + ja + l << "\n" ;	
      }
    }
    out << endl ;
  }
  return ;
}

void MacroGridBuilder :: generateRawHexaImage (istream & in, ostream & os) {

	// generateRawHexaImage () ist im nur ein Adapter, der aus den bisherigen
	// Hexaederdateiformaten ein entsprechendes 'rohes' Dateiformat f"ur den
	// Macrogridinflator erzeugt. Damit bleibt die Option erhalten das Format
	// der rohen Dateien auf weitere Elemente auszudehnen und zu modifizieren,
	// ohne die Kompatibilit"at zu den alten Hexaederdateien zu verlieren.
	// Das alte Format sieht im wesentlichen so aus:
	//
	// <Anzahl der Knoten : int >							/* 1.Zeile der Datei
	// <x-Koordinate : float>  <y-Koordinate : float>  <z-Koordinate : float>	/* f"ur den ersten Knoten
	// ...										/* f"ur den letzten Knoten
	// <Anzahl der Elemente : int>
	// <KnotenNr. 0: int> ... <KnotenNr. 7: int>					/* f"ur das erste Hexaederelement
	// ...										/* f"ur das letzte Hexaederelement
	// <Anzahl der Randfl"achen : int>
	// <Randtyp>  4  <KnotenNr. 0> ... <KnotenNr. 3>				/* f"ur die erste Randfl"ache
	// ...										/* f"ur die letzte Randfl"ache
	// <Identifier f"ur den 0. Knoten : int>					/* Identifierliste ist im seriellen
	// ...										/* Verfahren oder beim Aufsetzen aus
	// <Identifier f"ur den letzten Knoten : int>					/* einem Gitter optional, sonst muss
	//										/* jeder Vertex eine eigene Nummer haben
	
  const int start = clock () ;
  int nv = 0, ne = 0, nb = 0 ;
  int (* vnum)[8] = 0, (* bvec)[5] = 0, * pident = 0 ;
  double (* coord)[3] = 0 ;
  {
    in >> nv ;
    coord = new double [nv][3] ;
    assert (coord) ;
    for (int i = 0 ; i < nv ; i ++) in >> coord [i][0] >> coord [i][1] >> coord [i][2] ;
  }
  {
    in >> ne ;
    vnum = new int [ne][8] ;
    assert (vnum) ;
    for (int i = 0 ; i < ne ; i ++ )
      in >> vnum [i][0] >> vnum [i][1] >> vnum [i][2] >> vnum [i][3] >> vnum [i][4] >> vnum [i][5] >> vnum [i][6] >> vnum [i][7] ;
  }
  {
    in >> nb ;
    bvec = new int [nb][5] ;
    assert (bvec) ;
    for (int i = 0 ; i < nb ; i ++) {
      int n ;
      in >> bvec [i][4] >> n >> bvec [i][0] >> bvec [i][1] >> bvec [i][2] >> bvec [i][3] ;
      assert (n == 4) ;
    }
  }
  assert (in.good ()) ;
  pident = new int [nv] ;
  {
    int dummy ;
    for (int i = 0 ; i < nv ; i ++ ) in >> pident [i] >> dummy ; 
  }
  if (!in.good()) {
    cerr << "**WARNUNG (IGNORIERT) MacroGridBuilder :: generateRawHexaImage () " ;
    cerr << "Identifierliste unvollst\"andig oder nicht vorhanden. Daher keine parallele " ;
    cerr << "Identifikation falls aus mehreren Gittern geladen wurde." << endl ;
    for (int i = 0 ; i < nv ; i ++ ) pident [i] = i ;
  }
  {
    os << nv << endl ;
    for (int i = 0 ; i < nv ; i ++ )
      os << pident [i] << " " << coord [i][0] << " " << coord [i][1] << " " << coord [i][2] << endl ;
  }
  {
    os << ne << endl ;
    for (int i = 0 ; i < ne ; i ++)
      os << 8 << " " << pident [vnum [i][0]] << " " << pident [vnum [i][1]] << " "
	 << pident [vnum [i][2]] << " " << pident [vnum [i][3]] << " " << pident [vnum [i][4]] << " " 
	 << pident [vnum [i][5]] << " " << pident [vnum [i][6]] << " " << pident [vnum [i][7]] << endl ;
  }
  {
    os << nb << endl ;
    for (int i = 0 ; i < nb ; i ++)
      os << 4 << " " << pident [bvec [i][0]] << " " << pident [bvec [i][1]] << " "
	 << pident [bvec [i][2]] << " " << pident [bvec [i][3]] << " " << bvec [i][4] << endl ;
  }
  delete [] vnum ;
  delete [] coord ;
  delete [] bvec ;
  delete [] pident ;
  if (debugOption (4))
    cout << "**INFO MacroGridBuilder :: generateRawHexaImage () used: " << (float)(clock () - start)/(float)(CLOCKS_PER_SEC) << " sec." << endl ;
  return ;
}

void MacroGridBuilder :: generateRawTetraImage (istream & in, ostream & os) {
	
  const int start = clock () ;
  int nv = 0, ne = 0, nb = 0 ;
  int (* vnum)[4] = 0, (* bvec)[4] = 0, * pident = 0 ;
  double (* coord)[3] = 0 ;
  {
    in >> nv ;
    coord = new double [nv][3] ;
    assert (coord) ;
    for (int i = 0 ; i < nv ; i ++) in >> coord [i][0] >> coord [i][1] >> coord [i][2] ;
  }
  {
    in >> ne ;
    vnum = new int [ne][4] ;
    assert (vnum) ;
    for (int i = 0 ; i < ne ; i ++ )
      in >> vnum [i][0] >> vnum [i][1] >> vnum [i][2] >> vnum [i][3] ;
  }
  {
    in >> nb ;
    bvec = new int [nb][4] ;
    assert (bvec) ;
    for (int i = 0 ; i < nb ; i ++) {
      int n ;
      in >> bvec [i][3] >> n >> bvec [i][0] >> bvec [i][1] >> bvec [i][2] ;
      assert (n == 3) ;
    }
  }
  assert (in.good ()) ;
  pident = new int [nv] ;
  {
    int dummy ;
    for (int i = 0 ; i < nv ; i ++ ) in >> pident [i] >> dummy ; 
  }
  if (!in.good()) {
    cerr << "**WARNUNG (IGNORIERT) MacroGridBuilder :: generateRawTetraImage () " ;
    cerr << "Identifierliste unvollst\"andig oder nicht vorhanden. Daher keine parallele " ;
    cerr << "Identifikation falls aus mehreren Gittern geladen wurde." << endl ;
    for (int i = 0 ; i < nv ; i ++ ) pident [i] = i ;
  }
  {
    os << nv << endl ;
    for (int i = 0 ; i < nv ; i ++ )
      os << pident [i] << " " << coord [i][0] << " " << coord [i][1] << " " << coord [i][2] << endl ;
  }
  {
    os << ne << endl ;
    for (int i = 0 ; i < ne ; i ++)
      os << 4 << " " << pident [vnum [i][0]] << " " << pident [vnum [i][1]] << " "
	 		<< pident [vnum [i][2]] << " " << pident [vnum [i][3]] << endl ;
  }
  {
    os << nb << endl ;
    for (int i = 0 ; i < nb ; i ++)
      os << 3 << " " << pident [bvec [i][0]] << " " << pident [bvec [i][1]] << " "
	 << pident [bvec [i][2]] << " " << " " << bvec [i][3] << endl ;
  }
  delete [] vnum ;
  delete [] coord ;
  delete [] bvec ;
  delete [] pident ;
  if (debugOption (4))
    cout << "**INFO MacroGridBuilder :: generateRawHexaImage () used: " << (float)(clock () - start)/(float)(CLOCKS_PER_SEC) << " sec." << endl ;
  return ;
}

MacroGridBuilder :: MacroGridBuilder (BuilderIF & b) : _mgb (b) {
  {
    for (list < VertexGeo * > :: iterator i = myBuilder ()._vertexList.begin () ;
      i != myBuilder ()._vertexList.end () ; myBuilder ()._vertexList.erase (i ++)) 
      	_vertexMap [(*i)->ident ()] = (*i) ;
  }
  {
    for (list < hedge1_GEO * > :: iterator i = myBuilder ()._hedge1List.begin () ;
      i != myBuilder ()._hedge1List.end () ; myBuilder ()._hedge1List.erase (i ++)) {
      long k = (*i)->myvertex (0)->ident (), l = (*i)->myvertex (1)->ident () ;
      _edgeMap [edgeKey_t (k < l ? k : l, k < l ? l : k)] = (*i) ;
    }
  }
  {for (list < hface3_GEO * > :: iterator i = myBuilder ()._hface3List.begin () ; i != myBuilder ()._hface3List.end () ;
     myBuilder ()._hface3List.erase (i ++)) {
      _face3Map [faceKey_t ((*i)->myvertex (0)->ident (),(*i)->myvertex (1)->ident (), (*i)->myvertex (2)->ident ())] = (*i) ;
  }}
  {
    for (list < hface4_GEO * > :: iterator i = myBuilder ()._hface4List.begin () ; i != myBuilder ()._hface4List.end () ; 
      myBuilder ()._hface4List.erase (i ++)) _face4Map [faceKey_t ((*i)->myvertex (0)->ident (),(*i)->myvertex (1)->ident (),
      	(*i)->myvertex (2)->ident ())] = (*i) ;
  }
  {for (list < hbndseg4_GEO * > :: iterator i = myBuilder ()._hbndseg4List.begin () ; i != myBuilder ()._hbndseg4List.end () ; myBuilder ()._hbndseg4List.erase (i++)) {
    faceKey_t key ((*i)->myhface4 (0)->myvertex (0)->ident (), (*i)->myhface4 (0)->myvertex (1)->ident (), (*i)->myhface4 (0)->myvertex (2)->ident ()) ;
    if ((*i)->bndtype () == Gitter :: hbndseg_STI :: closure) {
      _hbnd4Int [key] = (void *) new pair < hface4_GEO *, int > ((*i)->myhface4 (0), (*i)->twist (0)) ;
      delete (*i) ;
    } else {
      _hbnd4Map [key] = (*i) ;
    }
  }}
  {for (list < hbndseg3_GEO * > :: iterator i = myBuilder ()._hbndseg3List.begin () ; i != myBuilder ()._hbndseg3List.end () ;
    myBuilder ()._hbndseg3List.erase (i++)) {
    faceKey_t key ((*i)->myhface3 (0)->myvertex (0)->ident (), (*i)->myhface3 (0)->myvertex (1)->ident (), (*i)->myhface3 (0)->myvertex (2)->ident ()) ;
    if ((*i)->bndtype () == Gitter :: hbndseg_STI :: closure) {
      _hbnd3Int [key] = (void *) new pair < hface3_GEO *, int > ((*i)->myhface3 (0), (*i)->twist (0)) ;
      delete (*i) ;
    } else {
      _hbnd3Map [key] = (*i) ;
    }
  }}
  {for (list < tetra_GEO * > :: iterator i = myBuilder ()._tetraList.begin () ; i != myBuilder ()._tetraList.end () ; 
      myBuilder ()._tetraList.erase (i++)) {
      _tetraMap [elementKey_t ((*i)->myvertex (0)->ident (), (*i)->myvertex (1)->ident (), 
      		 (*i)->myvertex (2)->ident (), (*i)->myvertex (3)->ident ())] = (*i) ;
  }}
  {
    for (list < hexa_GEO * > :: iterator i = myBuilder ()._hexaList.begin () ; i != myBuilder ()._hexaList.end () ; 
      myBuilder ()._hexaList.erase (i++)) _hexaMap [elementKey_t ((*i)->myvertex (0)->ident (), (*i)->myvertex (1)->ident (), 
      						(*i)->myvertex (3)->ident (), (*i)->myvertex (4)->ident ())] = (*i) ;
  }
  return ; 
}

MacroGridBuilder :: ~MacroGridBuilder () {
  {for (elementMap_t :: iterator i = _hexaMap.begin () ; i != _hexaMap.end () ; _hexaMap.erase (i++))
    myBuilder ()._hexaList.push_back ((hexa_GEO *)(*i).second) ;
  }
  {for (elementMap_t :: iterator i = _tetraMap.begin () ; i != _tetraMap.end () ; _tetraMap.erase (i++))
    myBuilder ()._tetraList.push_back ((tetra_GEO *)(*i).second) ;
  }
  {for (faceMap_t :: iterator i = _hbnd4Map.begin () ; i != _hbnd4Map.end () ; )
    if (((hbndseg4_GEO *)(*i).second)->myhface4 (0)->ref == 1) {
      delete (hbndseg4_GEO *)(*i).second ;
      _hbnd4Map.erase (i++) ;
    } else {
      myBuilder ()._hbndseg4List.push_back ((hbndseg4_GEO *)(*i ++).second) ;
    }
  }
  {for (faceMap_t :: iterator i = _hbnd3Map.begin () ; i != _hbnd3Map.end () ; )
    if (((hbndseg3_GEO *)(*i).second)->myhface3 (0)->ref == 1) {
      delete (hbndseg3_GEO *)(*i).second ;
      _hbnd3Map.erase (i++) ;
    } else {
      myBuilder ()._hbndseg3List.push_back ((hbndseg3_GEO *)(*i ++).second) ;
    }
  }
  {for (faceMap_t :: iterator i = _hbnd4Int.begin () ; i != _hbnd4Int.end () ; i ++) {
    const pair < hface4_GEO *, int > & p = * (pair < hface4_GEO *, int > *)(*i).second ;
    if (p.first->ref == 1) {
      hbndseg4_GEO * hb4 = myBuilder ().insert_hbnd4 (p.first,p.second,Gitter :: hbndseg_STI :: closure) ;
      myBuilder ()._hbndseg4List.push_back (hb4) ;
    }
    delete (pair < hface4_GEO *, int > *)(*i).second ;
  }}
  {for (faceMap_t :: iterator i = _hbnd3Int.begin () ; i != _hbnd3Int.end () ; i ++) {
    const pair < hface3_GEO *, int > & p = * (pair < hface3_GEO *, int > *)(*i).second ;
    if (p.first->ref == 1) {
      hbndseg3_GEO * hb3 = myBuilder ().insert_hbnd3 (p.first,p.second,Gitter :: hbndseg_STI :: closure) ;    
      myBuilder ()._hbndseg3List.push_back (hb3) ;
    }
    delete (pair < hface3_GEO *, int > *)(*i).second ;
  }}
  {for (faceMap_t :: iterator i = _face4Map.begin () ; i != _face4Map.end () ; )
    if (!((hface4_GEO *)(*i).second)->ref) {
      delete (hface4_GEO *)(*i).second ;
      _face4Map.erase (i++) ;
    } else {
      assert (((hface4_GEO *)(*i).second)->ref == 2) ;
      myBuilder ()._hface4List.push_back ((hface4_GEO *)(*i ++).second ) ;
    }
  }
  {for (faceMap_t :: iterator i = _face3Map.begin () ; i != _face3Map.end () ; ) {
    if (!((hface3_GEO *)(*i).second)->ref) {
      delete (hface3_GEO *)(*i).second ;
      _face3Map.erase (i++) ;
    } else {
      assert (((hface3_GEO *)(*i).second)->ref == 2) ;
      myBuilder ()._hface3List.push_back ((hface3_GEO *)(*i ++).second ) ;
    }
  }}
  {for (edgeMap_t :: iterator i = _edgeMap.begin () ; i != _edgeMap.end () ; )
    if (!(*i).second->ref) {
      delete (*i).second ;
      _edgeMap.erase (i++) ;
    } else {
      assert ((*i).second->ref >= 2) ;
      myBuilder ()._hedge1List.push_back ((*i ++).second) ;
    }
  }
  {for (vertexMap_t :: iterator i = _vertexMap.begin () ; i != _vertexMap.end () ; )
    if (!(*i).second->ref) {
      delete (*i).second ;
      _vertexMap.erase (i++) ;
    } else {
      assert ((*i).second->ref >= 3) ;
      myBuilder ()._vertexList.push_back ((*i ++).second) ;
    }
  }
  myBuilder ()._modified = true ;	// wichtig !
  return ;
}

void MacroGridBuilder :: inflateMacroGrid (istream & rawInput) {
  const int start = clock () ;
  {
    int nv = 0 ;
    rawInput >> nv ;
    for (int i = 0 ; i < nv ; i ++ ) {
      int id ;
      double x, y, z ;
      rawInput >> id >> x >> y >> z ;
      InsertUniqueVertex (x,y,z,id) ;
    }
  }
  {
    int ne = 0 ;
    rawInput >> ne ;
    for (int i = 0 ; i < ne ; i ++ ) {
      int elementType ;
      rawInput >> elementType ;
      if (elementType == 8) {
        int v [8] ;
        rawInput >> v [0] >> v [1] >> v [2] >> v [3] >> v [4] >> v [5] >> v [6] >> v [7] ;
	InsertUniqueHexa (v) ;
      } else if (elementType == 4) {
        int v [4] ;
        rawInput >> v [0] >> v [1] >> v [2] >> v [3] ;
        InsertUniqueTetra (v) ;
      } else {
        abort () ;
      }
    }
  }
  {
    int nb = 0 ;
    rawInput >> nb ;
    for (int i = 0 ; i < nb ; i ++) {
      int polygonLen ;
      rawInput >> polygonLen ;
      if (polygonLen == 4) {
        int bt, v [4] ;
        rawInput >> v [0] >> v [1] >> v [2] >> v [3] >> bt ;
        InsertUniqueHbnd4 (v,(Gitter :: hbndseg :: bnd_t)(-bt)) ;
      } else if (polygonLen == 3) {
        int bt, v [3] ;
        rawInput >> v [0] >> v [1] >> v [2] >> bt ;
        InsertUniqueHbnd3 (v,(Gitter :: hbndseg :: bnd_t)(-bt)) ;
      } else {
	cerr << " MacroGridBuilder :: inflateMacroGrid (istream &) FEHLER (fatal): kann" ;
        cerr << " Randelement mit Polygonl\"ange " << polygonLen << " NICHT erzeugen " << endl ;
        abort () ;
      }
    }
  }
  if (debugOption (3)) {
    cout << "**INFO MacroGridBuilder :: inflateMacroGrid () used: " ;
    cout << (float)(clock () - start)/(float)(CLOCKS_PER_SEC) << " sec." << endl ;
  }
  return ;
}

void Gitter :: Geometric :: BuilderIF :: macrogridBuilder (istream & in) {
  strstream raw ;
  MacroGridBuilder mm (*this) ;
  int c = in.get () ;
  assert (!in.eof ()) ;
  in.putback (c) ;
  assert (in.good ()) ;
  if (c == int ('!')) {
  
	// Kommentar gefunden: Die erste Zeile in den strstreambuf buf lesen
	// und auf 'Tetraeder' oder 'Hexaeder' untersuchen.

    strstreambuf buf ;
    in.get () ;		// Das Kommentarzeichen wird entfernt.
    in.get (buf) ;
    in.get () ;		// Der folgende Zeilenumbruchwird auch entfernt.
    istream is (& buf) ;
    char * str = new char [in.gcount () + 1] ;
    assert (str) ;
    is >> str ;		// Das erste Wort nach dem Kommentar steht jetzt in str.
    			// Alle weiteren k"onnen noch aus is gelesen werden, das
			// array str ist so lang wie die gesamte Zeile in 'buf'.

    if (0 == strcmp (str, "Tetraeder")) {
    
    			// Versuchen wir's mal mit Tetraedern
    
      MacroGridBuilder :: generateRawTetraImage (in,raw) ;
    } else if (0 == strcmp (str, "Hexaeder")) {
    
			// oder andernfalls mit Hexaedern.
    
      MacroGridBuilder :: generateRawHexaImage (in,raw) ;
    } else {
      cerr << "**WARNUNG (IGNORIERT) Unbekannter Kommentar zum Gitterformat: " << str ;
      cerr << " In : " << __FILE__ << " " << __LINE__ << endl ;
      return ;
    }
  } else {
    cerr << "**WARNUNG (IGNORIERT) Kein Bezeichner f\"ur das Dateiformat vorhanden.\n" ;
    cerr << "  -> Versuche es als Hexaedernetz zu lesen." << endl ;
    MacroGridBuilder :: generateRawHexaImage (in,raw) ;
  }
  mm.inflateMacroGrid (raw) ;
  return ;
}
