	// (c) bernhard schupp 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/gitter_pll_impl.cc,v $
	// $Revision: 2.6 $
	// $Name:  $
	// $State: Exp $
	// $Date: 1998/12/13 13:00:43 $

#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <strstream.h>

#include <set.h>
#include <map.h>
#include <functional.h>
#include <algorithm.h>

#include "mapp_cube_3d.h"
#include "mapp_tetra_3d.h"
#include "gitter_pll_impl.h"
#include "gitter_hexa_top_pll.h"
#include "gitter_tetra_top_pll.h"

static volatile char RCSId_gitter_pll_impl_cc [] = "$Id: gitter_pll_impl.cc,v 2.6 1998/12/13 13:00:43 yc23 Exp yc23 $" ;

const linkagePattern_t VertexPllBaseX :: nullPattern ;

VertexPllBaseX :: VertexPllBaseX (myvertex_t & v, linkagePatternMap_t & m) : _v (v), _map (m), _lpn (), _moveTo (), _ref () {
  linkagePatternMap_t :: iterator pos = _map.find (nullPattern) ;
  _lpn = (pos != _map.end ()) ? pos : _map.insert (pair < const linkagePattern_t, int > (nullPattern,0)).first ;
  (*_lpn).second ++ ;
  return ;
}

VertexPllBaseX :: ~VertexPllBaseX () {
  assert (_moveTo.size () == 0) ;
  (*_lpn).second -- ;
  return ;
}

vector < int > VertexPllBaseX :: estimateLinkage () const {
  return vector < int > ((*_lpn).first) ;
}

LinkedObject :: Identifier VertexPllBaseX :: getIdentifier () const {
  return Identifier (myvertex().ident ()) ;
}

bool VertexPllBaseX :: setLinkage (vector < int > lp) {
  (*_lpn).second -- ;
  sort (lp.begin (), lp.end (), less < int > ()) ;
  linkagePatternMap_t :: iterator pos = _map.find (lp) ;
  _lpn = (pos != _map.end ()) ? pos : _map.insert (pair < const linkagePattern_t, int > (lp,0)).first ;
  (*_lpn).second ++ ;
  return true ;
}

void VertexPllBaseX :: unattach2 (int i) {
  assert (_moveTo.find (i) != _moveTo.end ()) ;
  if ( -- _moveTo [i] == 0) _moveTo.erase (i) ;
  return ;
}

void VertexPllBaseX :: attach2 (int i) {
  map < int, int, less < int > > :: iterator pos = _moveTo.find (i) ;
  if (pos == _moveTo.end ()) {
    _moveTo.insert (pair < const int, int > (i,1)) ;
  } else {
    (*pos).second ++ ;
  }
  return ;
}

bool VertexPllBaseX :: packAll (vector < ObjectStream > & osv) {
  bool action (false) ;
  for (map < int, int, less < int > > :: const_iterator i = _moveTo.begin () ; i != _moveTo.end () ; i ++) {
    int j = (*i).first ;
    assert ((osv.begin () + j) < osv.end ()) ;
    osv [j].writeObject (VERTEX) ;
    osv [j].writeObject (myvertex ().ident ()) ;
    osv [j].writeObject (myvertex ().Point ()[0]) ;
    osv [j].writeObject (myvertex ().Point ()[1]) ;
    osv [j].writeObject (myvertex ().Point ()[2]) ;
    action = true ;
  }
  return action ;
}

void VertexPllBaseX :: unpackSelf (ObjectStream &, bool i) {
  if (i) {
  }
  return ;
}

vector < int > EdgePllBaseX :: estimateLinkage () const {
  return (abort (), vector < int > ()) ;
}

LinkedObject :: Identifier EdgePllBaseX :: getIdentifier () const {
  return (abort (), LinkedObject :: Identifier  ()) ;
}

void EdgePllBaseX :: getRefinementRequest (ObjectStream & os) const {
  os.writeObject (int(myhedge1 ().getrule ())) ;
  return ;
}

bool EdgePllBaseX :: setRefinementRequest (ObjectStream & os) {
  int i ;
  try {
    os.readObject (i) ;
  } catch (ObjectStream :: EOFException) {
    cerr << "**FEHLER (FATAL) EOF gelesen in " << __FILE__ << " " << __LINE__ << endl ;
    abort () ;
  }
  return myhedge1_t :: myrule_t (i) == myhedge1_t :: myrule_t :: nosplit ? 
  	false : (myhedge1 ().refineImmediate (myhedge1_t :: myrule_t (i)), true) ;
}

void EdgePllBaseX :: unattach2 (int) {
  abort () ;
  return ;
}

void EdgePllBaseX :: attach2 (int) {
  abort () ;
  return ;
}

bool EdgePllBaseX :: packAll (vector < ObjectStream > &) {
  return (abort (), false) ;
}

void EdgePllBaseX :: unpackSelf (ObjectStream &,bool) {
  abort () ;
  return ;
}

bool EdgePllBaseX :: lockAndTry () {
  _lockCRS = true ;
  return myhedge1().coarse () ;
}

bool EdgePllBaseX :: lockedAgainstCoarsening () const {
  return _lockCRS ;
}

bool EdgePllBaseX :: unlockAndResume (bool r) {
  _lockCRS = false ;
  bool x ;
  if (r) {
    x = myhedge1().coarse () ;
  }
  else {
    x = false ;
  }
  return x ;
}

EdgePllBaseXMacro :: EdgePllBaseXMacro (myhedge1_t & e) : EdgePllBaseX (e), _moveTo (), _ref () {
  return ;
}

EdgePllBaseXMacro :: ~EdgePllBaseXMacro () {
  assert (0 == _moveTo.size ()) ;
  return ;
}

vector < int > EdgePllBaseXMacro :: estimateLinkage () const {
  vector < int > est ;
  vector < int > l0 = myhedge1 ().myvertex(0)->accessPllX ().estimateLinkage () ;
  vector < int > l1 = myhedge1 ().myvertex(1)->accessPllX ().estimateLinkage () ;
  set_intersection (l0.begin (), l0.end (), l1.begin (), l1.end (), back_inserter (est), less < int > ()) ;
  return est ;
}

LinkedObject :: Identifier EdgePllBaseXMacro :: getIdentifier () const {
  return Identifier (myhedge1 ().myvertex (0)->ident (), myhedge1 ().myvertex (1)->ident ()) ;
}

void EdgePllBaseXMacro :: unattach2 (int i) {
  assert (_moveTo.find (i) != _moveTo.end ()) ;
  if ( -- _moveTo [i] == 0) _moveTo.erase (i) ;
  myhedge1 ().myvertex (0)->accessPllX ().unattach2 (i) ;
  myhedge1 ().myvertex (1)->accessPllX ().unattach2 (i) ;
  return ;
}

void EdgePllBaseXMacro :: attach2 (int i) {
  map < int, int, less < int > > :: iterator pos = _moveTo.find (i) ;
  if (pos == _moveTo.end ()) {
    _moveTo.insert (pair < const int, int > (i,1)) ;
  } else {
    (*pos).second ++ ;
  }
  myhedge1 ().myvertex (0)->accessPllX ().attach2 (i) ;
  myhedge1 ().myvertex (1)->accessPllX ().attach2 (i) ;
  return ;
}

bool EdgePllBaseXMacro :: packAll (vector < ObjectStream > & osv) {
  bool action (false) ;
  for (map < int, int, less < int > > :: const_iterator i = _moveTo.begin () ; i != _moveTo.end () ; i ++) {
    int j = (*i).first ;
    assert ((osv.begin () + j) < osv.end ()) ;
    osv [j].writeObject (EDGE1) ;
    osv [j].writeObject (myhedge1 ().myvertex (0)->ident ()) ;
    osv [j].writeObject (myhedge1 ().myvertex (1)->ident ()) ;
    {
      strstream s ;
      myhedge1 ().backup (s) ;
      for (int c = s.get () ; ! s.eof () ; c = s.get ()) osv [j].writeObject (c) ;
      osv [j].writeObject (-1) ;
    }
    action = true ;
  }
  return action ;
}

void EdgePllBaseXMacro :: unpackSelf (ObjectStream & os, bool i) {
  strstream s ;
  int c ;
  try {
    for (os.readObject (c) ; c != -1 ; os.readObject (c)) s.put ((char)c) ;
  } catch (ObjectStream :: EOFException) {
    cerr << "**FEHLER (FATAL) EOF gelesen in " << __FILE__ << " " << __LINE__ << endl ;
    abort () ;
  }
  if (i) {
    myhedge1 ().restore (s) ;
    assert (!s.eof ()) ;
  }
  return ;
}

ElementPllXIF_t & ElementPllBaseX :: accessOuterPllX (ElementPllXIF_t & x) {
  return x ;
}

ElementPllXIF_t & ElementPllBaseX :: accessInnerPllX (ElementPllXIF_t &) {
  return * this ;
}

const ElementPllXIF_t & ElementPllBaseX :: accessOuterPllX (const ElementPllXIF_t & x) const {
  return x ;
}

const ElementPllXIF_t & ElementPllBaseX :: accessInnerPllX (const ElementPllXIF_t &) const {
  return * this ;
}

bool ElementPllBaseX :: ldbUpdateGraphVertex (LoadBalancer :: DataBase &) {
  return (abort (), false) ;
}

int ElementPllBaseX :: ldbVertexIndex () const {
  return (abort (), 0) ;
}

int & ElementPllBaseX :: ldbVertexIndex () {
  return (abort (), *(int *)0) ;
}

void ElementPllBaseX :: writeStaticState (ObjectStream &) const {
  return ;
}

void ElementPllBaseX :: readStaticState (ObjectStream &) {
  abort () ;
  return ;
}

void ElementPllBaseX :: readDynamicState (ObjectStream &) {
  abort () ;
  return ;
}

void ElementPllBaseX :: unattach2 (int) {
  abort () ;
  return ;
}

void ElementPllBaseX :: attach2 (int) {
  abort () ;
  return ;
}

bool ElementPllBaseX :: packAll (vector < ObjectStream > &) { 
  return (abort (), false) ;
}

void ElementPllBaseX :: packAsBnd (int,int,ObjectStream &) const {
  abort () ;
  return ;
}

void ElementPllBaseX :: unpackSelf (ObjectStream &, bool) {
  abort () ;
  return ;
}

bool ElementPllBaseX :: erasable () const {
  return (abort (), false) ;
}

void ElementPllBaseX :: getRefinementRequest (ObjectStream &) {
  abort () ;
  return ;
}

bool ElementPllBaseX :: setRefinementRequest (ObjectStream &) {
  return (abort (), false) ;
}

bool ElementPllBaseX :: lockAndTry () {
  return (abort (), false) ;
}

bool ElementPllBaseX :: unlockAndResume (bool) {
  return (abort (), false) ;
}
/*
bool ElementPllBaseX :: lockedAgainstCoarsening () const {
  return (abort (), false) ;
}
*/
void TetraPllXBase :: writeDynamicState (ObjectStream & os) const {
  static const double x = 1./4. ;
  double p [3] ;
  LinearMapping (mytetra ().myvertex (0)->Point (), mytetra ().myvertex (1)->Point (),
    		 mytetra ().myvertex (2)->Point (), mytetra ().myvertex (3)->Point ())
   	 	.map2world (x,x,x,x,p) ;
  os.writeObject (p [0]) ;
  os.writeObject (p [1]) ;
  os.writeObject (p [2]) ;
  return ;
}

TetraPllXBaseMacro :: TetraPllXBaseMacro (mytetra_t & t) : TetraPllXBase (t), _ldbVertexIndex (-1), _moveTo (), _erasable (false) {
  static const double x = 1./4. ;
  LinearMapping (mytetra ().myvertex (0)->Point (), mytetra ().myvertex (1)->Point (),
    		 mytetra ().myvertex (2)->Point (), mytetra ().myvertex (3)->Point ())
   	 	.map2world (x,x,x,x,_center) ;
  return ;
}

TetraPllXBaseMacro :: ~TetraPllXBaseMacro () {
  vector < int > v ;
  {for (map < int, int, less < int > > :: const_iterator i = _moveTo.begin () ; i != _moveTo.end () ; v.push_back ((*i++).first)) ;}
  {for (vector < int > :: const_iterator i = v.begin () ; i != v.end () ; unattach2 (*i++)) ;}
  return ;
}

int TetraPllXBaseMacro :: ldbVertexIndex () const {
  return _ldbVertexIndex ;
}

int & TetraPllXBaseMacro :: ldbVertexIndex () {
  return _ldbVertexIndex ;
}

bool TetraPllXBaseMacro :: ldbUpdateGraphVertex (LoadBalancer :: DataBase & db) {
  db.vertexUpdate (LoadBalancer :: GraphVertex (ldbVertexIndex (), 
  		TreeIterator < Gitter :: helement_STI, is_leaf < Gitter :: helement_STI > > (mytetra ()).size (), _center)) ;
  return true ;
}

void TetraPllXBaseMacro :: writeStaticState (ObjectStream & os) const {
  os.writeObject (ldbVertexIndex ()) ;
  return ;
}

void TetraPllXBaseMacro :: unattach2 (int i) {
  assert (_moveTo.find (i) != _moveTo.end ()) ;
  if ( -- _moveTo [i] == 0) _moveTo.erase (i) ;
  mytetra ().myhface3 (0)->accessPllX ().unattach2 (i) ;
  mytetra ().myhface3 (1)->accessPllX ().unattach2 (i) ;
  mytetra ().myhface3 (2)->accessPllX ().unattach2 (i) ;
  mytetra ().myhface3 (3)->accessPllX ().unattach2 (i) ;
  return ;
}

void TetraPllXBaseMacro :: attach2 (int i) {
  map < int, int, less < int > > :: iterator pos = _moveTo.find (i) ;
  if (pos == _moveTo.end ()) {
    _moveTo.insert (pair < const int, int > (i,1)) ;
  } else {
    if ((*pos).first == i) {
      cerr << "  TetraPllXBaseMacro :: attach2 () WARNUNG versuchte mehrfache Zuweisung ignoriert " << endl ;
      return ;
    }
  }
  mytetra ().myhface3 (0)->accessPllX ().attach2 (i) ;
  mytetra ().myhface3 (1)->accessPllX ().attach2 (i) ;
  mytetra ().myhface3 (2)->accessPllX ().attach2 (i) ;
  mytetra ().myhface3 (3)->accessPllX ().attach2 (i) ;
  return ;
}

bool TetraPllXBaseMacro :: packAll (vector < ObjectStream > & osv) {
  for (map < int, int, less < int > > :: const_iterator i = _moveTo.begin () ; i != _moveTo.end () ; i ++) {
    int j = (*i).first ;
    assert ((osv.begin () + j) < osv.end ()) ;
    assert (_moveTo.size () == 1) ;
    osv [j].writeObject (TETRA) ;
    osv [j].writeObject (mytetra ().myvertex (0)->ident ()) ;
    osv [j].writeObject (mytetra ().myvertex (1)->ident ()) ;
    osv [j].writeObject (mytetra ().myvertex (2)->ident ()) ;
    osv [j].writeObject (mytetra ().myvertex (3)->ident ()) ;
    {
      strstream s ;
      mytetra ().backup (s) ;
      for (int c = s.get () ; ! s.eof () ; c = s.get ()) osv [j].writeObject (c) ;
      osv [j].writeObject (-1) ;
      inlineData (osv [j]) ;
    }
    _erasable = true ;
    return true ;
  }
  return false ;
}

void TetraPllXBaseMacro :: packAsBnd (int fce, int who, ObjectStream & os) const {
  bool hit = _moveTo.size () == 0 ? true : false ;
  for (map < int, int, less < int > > :: const_iterator i = _moveTo.begin () ; i != _moveTo.end () ; i ++ )
    if ((*i).first != who) hit = true ;
  if (hit) {
    os.writeObject (HBND3INT) ;
    os.writeObject (Gitter :: hbndseg :: closure) ;
    os.writeObject (mytetra ().myvertex (fce,0)->ident ()) ;
    os.writeObject (mytetra ().myvertex (fce,1)->ident ()) ;
    os.writeObject (mytetra ().myvertex (fce,2)->ident ()) ;
  }
  return ;
}

void TetraPllXBaseMacro :: unpackSelf (ObjectStream & os, bool i) {
  assert (i) ;
  strstream s ;
  int c ;
  try {
    for (os.readObject (c) ; c != -1 ; os.readObject (c)) s.put ((char)c) ;
  } catch (ObjectStream :: EOFException) {
    cerr << "**FEHLER (FATAL) EOF gelesen in " << __FILE__ << " " << __LINE__ << endl ;
    abort () ;
  }
  if (i) {
    mytetra ().restore (s) ;
    assert (!s.eof ()) ;
    xtractData (os) ;
  }
  return ;
}

bool TetraPllXBaseMacro :: erasable () const {
  return _erasable ;
}

void HexaPllBaseX :: writeDynamicState (ObjectStream & os) const {
  double p [3] ;
  TrilinearMapping (myhexa ().myvertex (0)->Point (), myhexa ().myvertex (1)->Point (),
    myhexa ().myvertex (2)->Point (), myhexa ().myvertex (3)->Point (), myhexa ().myvertex (4)->Point (),
    myhexa ().myvertex (5)->Point (), myhexa ().myvertex (6)->Point (), myhexa ().myvertex (7)->Point ())
    .map2world (.0,.0,.0,p) ;
  os.writeObject (p [0]) ;
  os.writeObject (p [1]) ;
  os.writeObject (p [2]) ;
  return ;
}

HexaPllBaseXMacro :: HexaPllBaseXMacro (myhexa_t & h) : HexaPllBaseX (h), _ldbVertexIndex (-1), _moveTo (), _erasable (false) {
  double p [3] ;
  TrilinearMapping (myhexa ().myvertex (0)->Point (), myhexa ().myvertex (1)->Point (),
    myhexa ().myvertex (2)->Point (), myhexa ().myvertex (3)->Point (), myhexa ().myvertex (4)->Point (),
    myhexa ().myvertex (5)->Point (), myhexa ().myvertex (6)->Point (), myhexa ().myvertex (7)->Point ())
    .map2world (.0,.0,.0,_center) ;
  return ;
}

HexaPllBaseXMacro :: ~HexaPllBaseXMacro () {
  vector < int > v ;
  {for (map < int, int, less < int > > :: const_iterator i = _moveTo.begin () ; i != _moveTo.end () ; v.push_back ((*i++).first)) ;}
  {for (vector < int > :: const_iterator i = v.begin () ; i != v.end () ; unattach2 (*i++)) ;}
  return ;
}

int HexaPllBaseXMacro :: ldbVertexIndex () const {
  return _ldbVertexIndex ;
}

int & HexaPllBaseXMacro :: ldbVertexIndex () {
  return _ldbVertexIndex ;
}

bool HexaPllBaseXMacro :: ldbUpdateGraphVertex (LoadBalancer :: DataBase & db) {
  db.vertexUpdate (LoadBalancer :: GraphVertex (ldbVertexIndex (), 
  		TreeIterator < Gitter :: helement_STI, is_leaf < Gitter :: helement_STI > > (myhexa ()).size (), _center)) ;
  return true ;
}

void HexaPllBaseXMacro :: writeStaticState (ObjectStream & os) const {
  os.writeObject (ldbVertexIndex ()) ;
  return ;
}

void HexaPllBaseXMacro :: unattach2 (int i) {
  assert (_moveTo.find (i) != _moveTo.end ()) ;
  if ( -- _moveTo [i] == 0) _moveTo.erase (i) ;
  myhexa ().myhface4 (0)->accessPllX ().unattach2 (i) ;
  myhexa ().myhface4 (1)->accessPllX ().unattach2 (i) ;
  myhexa ().myhface4 (2)->accessPllX ().unattach2 (i) ;
  myhexa ().myhface4 (3)->accessPllX ().unattach2 (i) ;
  myhexa ().myhface4 (4)->accessPllX ().unattach2 (i) ;
  myhexa ().myhface4 (5)->accessPllX ().unattach2 (i) ;
  return ;
}

void HexaPllBaseXMacro :: attach2 (int i) {
  map < int, int, less < int > > :: iterator pos = _moveTo.find (i) ;
  if (pos == _moveTo.end ()) {
    _moveTo.insert (pair < const int, int > (i,1)) ;
  } else {
    if ((*pos).first == i) {
      cerr << "  HexaPllBaseXMacro :: attach2 () WARNUNG versuchte mehrfache Zuweisung ignoriert " << endl ;
      return ;
    }
  }
  myhexa ().myhface4 (0)->accessPllX ().attach2 (i) ;
  myhexa ().myhface4 (1)->accessPllX ().attach2 (i) ;
  myhexa ().myhface4 (2)->accessPllX ().attach2 (i) ;
  myhexa ().myhface4 (3)->accessPllX ().attach2 (i) ;
  myhexa ().myhface4 (4)->accessPllX ().attach2 (i) ;
  myhexa ().myhface4 (5)->accessPllX ().attach2 (i) ;
  return ;
}

bool HexaPllBaseXMacro :: packAll (vector < ObjectStream > & osv) {
  for (map < int, int, less < int > > :: const_iterator i = _moveTo.begin () ; i != _moveTo.end () ; i ++) {
    int j = (*i).first ;
    assert ((osv.begin () + j) < osv.end ()) ;
    assert (_moveTo.size () == 1) ;
    osv [j].writeObject (HEXA) ;
    osv [j].writeObject (myhexa ().myvertex (0)->ident ()) ;
    osv [j].writeObject (myhexa ().myvertex (1)->ident ()) ;
    osv [j].writeObject (myhexa ().myvertex (2)->ident ()) ;
    osv [j].writeObject (myhexa ().myvertex (3)->ident ()) ;
    osv [j].writeObject (myhexa ().myvertex (4)->ident ()) ;
    osv [j].writeObject (myhexa ().myvertex (5)->ident ()) ;
    osv [j].writeObject (myhexa ().myvertex (6)->ident ()) ;
    osv [j].writeObject (myhexa ().myvertex (7)->ident ()) ;
    {
      strstream s ;
      myhexa ().backup (s) ;
      for (int c = s.get () ; ! s.eof () ; c = s.get ()) osv [j].writeObject (c) ;
      osv [j].writeObject (-1) ;
      inlineData (osv [j]) ;
    }
    _erasable = true ;
    return true ;
  }
  return false ;
}

void HexaPllBaseXMacro :: packAsBnd (int fce, int who, ObjectStream & os) const {
  bool hit = _moveTo.size () == 0 ? true : false ;
  for (map < int, int, less < int > > :: const_iterator i = _moveTo.begin () ; i != _moveTo.end () ; i ++ )
    if ((*i).first != who) hit = true ;
  if (hit) {
    os.writeObject (HBND4INT) ;
    os.writeObject (Gitter :: hbndseg :: closure) ;
    os.writeObject (myhexa ().myvertex (fce,0)->ident ()) ;
    os.writeObject (myhexa ().myvertex (fce,1)->ident ()) ;
    os.writeObject (myhexa ().myvertex (fce,2)->ident ()) ;
    os.writeObject (myhexa ().myvertex (fce,3)->ident ()) ;
  }
  return ;
}

void HexaPllBaseXMacro :: unpackSelf (ObjectStream & os, bool i) {
  assert (i) ;
  strstream s ;
  int c ;
  try {
    for (os.readObject (c) ; c != -1 ; os.readObject (c)) s.put ((char)c) ;
  } catch (ObjectStream :: EOFException) {
    cerr << "**FEHLER (FATAL) EOF gelesen in " << __FILE__ << " " << __LINE__ << endl ;
    abort () ;
  }
  if (i) {
    myhexa ().restore (s) ;
    assert (!s.eof ()) ;
    xtractData (os) ;
  }
  return ;
}

bool HexaPllBaseXMacro :: erasable () const {
  return _erasable ;
}

ElementPllXIF_t & BndsegPllBaseX :: accessOuterPllX (ElementPllXIF_t &) {
  return * this ;
}

ElementPllXIF_t & BndsegPllBaseX :: accessInnerPllX (ElementPllXIF_t & x) {
  return x ;
}

const ElementPllXIF_t & BndsegPllBaseX :: accessOuterPllX (const ElementPllXIF_t &) const {
  return * this ;
}

const ElementPllXIF_t & BndsegPllBaseX :: accessInnerPllX (const ElementPllXIF_t & x) const {
  return x ;
}

GitterBasisPll :: ObjectsPll :: VertexPllImplMacro :: VertexPllImplMacro (double x, double y, double z, int i, linkagePatternMap_t & map) 
  : GitterBasis :: Objects :: VertexEmptyMacro (x,y,z,i), _pllx (new mypllx_t (*this,map)) {
  return ;
}

GitterBasisPll :: ObjectsPll :: VertexPllImplMacro :: ~VertexPllImplMacro () {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

VertexPllXIF_t & GitterBasisPll :: ObjectsPll :: VertexPllImplMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const VertexPllXIF_t & GitterBasisPll :: ObjectsPll :: VertexPllImplMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterBasisPll :: ObjectsPll :: VertexPllImplMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

EdgePllXIF_t & GitterBasisPll :: ObjectsPll :: Hedge1EmptyPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _pllx ;
}

const EdgePllXIF_t & GitterBasisPll :: ObjectsPll :: Hedge1EmptyPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _pllx ;
}

GitterBasisPll :: ObjectsPll :: Hedge1EmptyPllMacro :: Hedge1EmptyPllMacro (myvertex_t * a, myvertex_t * b) :
	GitterBasisPll :: ObjectsPll :: hedge1_IMPL (0, a, b), _pllx (new mypllx_t (*this)) {
  return ;
}

GitterBasisPll :: ObjectsPll :: Hedge1EmptyPllMacro :: ~Hedge1EmptyPllMacro () {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

EdgePllXIF_t & GitterBasisPll :: ObjectsPll :: Hedge1EmptyPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const EdgePllXIF_t & GitterBasisPll :: ObjectsPll :: Hedge1EmptyPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterBasisPll :: ObjectsPll :: Hedge1EmptyPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

FacePllXIF_t & GitterBasisPll :: ObjectsPll :: Hface3EmptyPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _pllx ;
}

const FacePllXIF_t & GitterBasisPll :: ObjectsPll :: Hface3EmptyPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _pllx ;
}

GitterBasisPll :: ObjectsPll :: Hface3EmptyPllMacro :: Hface3EmptyPllMacro (myhedge1_t * e0, int s0, myhedge1_t * e1, int s1, myhedge1_t * e2, int s2)
	: GitterBasisPll :: ObjectsPll :: hface3_IMPL (0,e0,s0,e1,s1,e2,s2), _pllx (new mypllx_t (*this)) {
  return ;
}

GitterBasisPll :: ObjectsPll :: Hface3EmptyPllMacro :: ~Hface3EmptyPllMacro () {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

FacePllXIF_t & GitterBasisPll :: ObjectsPll :: Hface3EmptyPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const FacePllXIF_t & GitterBasisPll :: ObjectsPll :: Hface3EmptyPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterBasisPll :: ObjectsPll :: Hface3EmptyPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

FacePllXIF_t & GitterBasisPll :: ObjectsPll :: Hface4EmptyPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _pllx ;
}

const FacePllXIF_t & GitterBasisPll :: ObjectsPll :: Hface4EmptyPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _pllx ;
}

GitterBasisPll :: ObjectsPll :: Hface4EmptyPllMacro :: Hface4EmptyPllMacro (myhedge1_t * e0, int s0, myhedge1_t * e1, int s1, myhedge1_t * e2, int s2, myhedge1_t * e3, int s3)
	: GitterBasisPll :: ObjectsPll :: hface4_IMPL (0,e0,s0,e1,s1,e2,s2,e3,s3), _pllx (new mypllx_t (*this)) {
  return ;
}

GitterBasisPll :: ObjectsPll :: Hface4EmptyPllMacro :: ~Hface4EmptyPllMacro () {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

FacePllXIF_t & GitterBasisPll :: ObjectsPll :: Hface4EmptyPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const FacePllXIF_t & GitterBasisPll :: ObjectsPll :: Hface4EmptyPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterBasisPll :: ObjectsPll :: Hface4EmptyPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

ElementPllXIF_t & GitterBasisPll :: ObjectsPll :: TetraEmptyPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _pllx ;
}

const ElementPllXIF_t & GitterBasisPll :: ObjectsPll :: TetraEmptyPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _pllx ;
}

void GitterBasisPll :: ObjectsPll :: TetraEmptyPll :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  abort () ;
  return ;
}

GitterBasisPll :: ObjectsPll :: TetraEmptyPllMacro :: TetraEmptyPllMacro (myhface3_t * f0, int t0, myhface3_t * f1, int t1, myhface3_t * f2, int t2, myhface3_t * f3, int t3)
	: GitterBasisPll :: ObjectsPll :: tetra_IMPL (0,f0,t0,f1,t1,f2,t2,f3,t3), _pllx (new mypllx_t (*this)) {
  return ;
}

GitterBasisPll :: ObjectsPll :: TetraEmptyPllMacro :: ~TetraEmptyPllMacro () {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

ElementPllXIF_t & GitterBasisPll :: ObjectsPll :: TetraEmptyPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const ElementPllXIF_t & GitterBasisPll :: ObjectsPll :: TetraEmptyPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterBasisPll :: ObjectsPll :: TetraEmptyPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

ElementPllXIF_t & GitterBasisPll :: ObjectsPll :: HexaEmptyPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _pllx ;
}

const ElementPllXIF_t & GitterBasisPll :: ObjectsPll :: HexaEmptyPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _pllx ;
}

void GitterBasisPll :: ObjectsPll :: HexaEmptyPll :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  abort () ;
  return ;
}

GitterBasisPll :: ObjectsPll :: HexaEmptyPllMacro :: HexaEmptyPllMacro 
	(myhface4_t * f0, int t0, myhface4_t * f1, int t1, myhface4_t * f2, int t2, 
	 myhface4_t * f3, int t3, myhface4_t * f4, int t4, myhface4_t * f5, int t5)
	: GitterBasisPll :: ObjectsPll :: hexa_IMPL (0,f0,t0,f1,t1,f2,t2,f3,t3,f4,t4,f5,t5), _pllx (new mypllx_t (*this)) {
  return ;
}

GitterBasisPll :: ObjectsPll :: HexaEmptyPllMacro :: ~HexaEmptyPllMacro () {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

ElementPllXIF_t & GitterBasisPll :: ObjectsPll :: HexaEmptyPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const ElementPllXIF_t & GitterBasisPll :: ObjectsPll :: HexaEmptyPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterBasisPll :: ObjectsPll :: HexaEmptyPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

GitterBasisPll :: MacroGitterBasisPll :: MacroGitterBasisPll (istream & in) : GitterPll :: MacroGitterPll () {
  macrogridBuilder (in) ;
  return ;
}

GitterBasisPll :: MacroGitterBasisPll :: MacroGitterBasisPll () : GitterPll :: MacroGitterPll () {
  return ;
}

GitterBasisPll :: MacroGitterBasisPll :: ~MacroGitterBasisPll () {
  try {
    {
      AccessIterator < helement_STI > :: Handle w (*this) ;
      for (w.first () ; ! w.done () ; w.next ()) w.item ().detachPllXFromMacro () ;
    }
    {
      AccessIterator < hface_STI > :: Handle w (*this) ;
      for (w.first () ; ! w.done () ; w.next ()) w.item ().detachPllXFromMacro () ;
    }
    {
      AccessIterator < hedge_STI > :: Handle w (*this) ;
      for (w.first () ; ! w.done () ; w.next ()) w.item ().detachPllXFromMacro () ;
    }
    {
      AccessIterator < vertex_STI > :: Handle w (*this) ;
      for (w.first () ; ! w.done () ; w.next ()) w.item ().detachPllXFromMacro () ;
    }
  } catch (Parallel :: AccessPllException) {
    cerr << "**WARNUNG (AUSNAHME IGNORIERT) in " << __FILE__ << " " << __LINE__ << endl ;
  }
  {
    for (linkagePatternMap_t :: iterator p = _linkagePatterns.begin () ; p != _linkagePatterns.end () ; p ++) 
      assert ((*p).second == 0) ;
    _linkagePatterns.erase (_linkagePatterns.begin (), _linkagePatterns.end ()) ;
  }
  return ;
}

set < int, less < int > > GitterBasisPll :: MacroGitterBasisPll :: secondScan () {
  set < int, less < int > > s ;
  int n = 0 ;
  for (linkagePatternMap_t :: iterator p = _linkagePatterns.begin () ; p != _linkagePatterns.end () ; ) {
    if ((*p).second) {
      for (vector < int > :: const_iterator i = (*p).first.begin () ; i != (*p).first.end () ; s.insert (*i++)) ;
      p ++ ;
    } else {
      _linkagePatterns.erase (p ++) ;
      n ++ ;
    }
  }
  assert (debugOption (20) ? (cout << "  GitterBasisPll :: MacroGitterBasisPll :: secondScan () deleted " << n << " patterns" << endl, 1) : 1) ;
  return s ;
}

Gitter :: Geometric :: VertexGeo * GitterBasisPll :: MacroGitterBasisPll :: insert_vertex (double x,double y,double z,int i, int) {
  return new ObjectsPll :: VertexPllImplMacro (x,y,z,i,_linkagePatterns) ;
}

Gitter :: Geometric :: hedge1_GEO * GitterBasisPll :: MacroGitterBasisPll :: insert_hedge1 (VertexGeo *a, VertexGeo *b) {
  return new ObjectsPll :: Hedge1EmptyPllMacro (a,b) ;
}

Gitter :: Geometric :: hface4_GEO * GitterBasisPll :: MacroGitterBasisPll :: insert_hface4 (hedge1_GEO *(&e)[4], int (&s)[4]) {
  return new ObjectsPll :: Hface4EmptyPllMacro (e [0], s [0], e [1], s [1], e [2], s [2], e [3], s [3]) ;
}

Gitter :: Geometric :: hface3_GEO * GitterBasisPll :: MacroGitterBasisPll :: insert_hface3 (hedge1_GEO *(&e)[3], int (&s)[3]) {
  return new ObjectsPll :: Hface3EmptyPllMacro (e [0], s [0], e [1], s [1], e [2], s [2]) ;
}

Gitter :: Geometric :: hexa_GEO * GitterBasisPll :: MacroGitterBasisPll :: insert_hexa (hface4_GEO *(&f)[6], int (&t)[6]) {
  return new ObjectsPll :: HexaEmptyPllMacro (f [0], t[0], f [1], t[1], f [2], t[2], f[3], t[3], f[4], t[4], f[5], t[5]) ;
}

Gitter :: Geometric :: Tetra * GitterBasisPll :: MacroGitterBasisPll :: insert_tetra (hface3_GEO *(&f)[4], int (&t)[4]) {
  return new ObjectsPll :: TetraEmptyPllMacro (f [0], t[0], f [1], t[1], f [2], t[2], f[3], t[3]) ;
}

Gitter :: Geometric :: hbndseg4_GEO * GitterBasisPll :: MacroGitterBasisPll :: insert_hbnd4 (hface4_GEO * f, int t, Gitter :: hbndseg_STI :: bnd_t b) {
  if (b == Gitter :: hbndseg_STI :: closure) {
    return new Hbnd4PllInternal < GitterBasis :: Objects :: Hbnd4Default, BndsegPllBaseXClosure < hbndseg4_GEO > , BndsegPllBaseXMacroClosure < hbndseg4_GEO > > :: macro_t (f,t) ;
  } else {
    return new Hbnd4PllExternal < GitterBasis :: Objects :: Hbnd4Default, BndsegPllBaseXMacro < hbndseg4_GEO > > (f,t) ;
  }
}

Gitter :: Geometric :: hbndseg3_GEO * GitterBasisPll :: MacroGitterBasisPll :: insert_hbnd3 (hface3_GEO * f, int t, Gitter :: hbndseg_STI :: bnd_t b) {
  if (b == Gitter :: hbndseg_STI :: closure) {
    return new Hbnd3PllInternal < GitterBasis :: Objects :: Hbnd3Default, BndsegPllBaseXClosure < hbndseg3_GEO > , BndsegPllBaseXMacroClosure < hbndseg3_GEO > > :: macro_t (f,t) ;
  } else {
    return new Hbnd3PllExternal < GitterBasis :: Objects :: Hbnd3Default, BndsegPllBaseXMacro < hbndseg3_GEO > > (f,t) ;
  }
}

IteratorSTI < Gitter :: vertex_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const vertex_STI * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

IteratorSTI < Gitter :: vertex_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const IteratorSTI < vertex_STI > * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

IteratorSTI < Gitter :: hedge_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const hedge_STI * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

IteratorSTI < Gitter :: hedge_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const IteratorSTI < hedge_STI > * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

IteratorSTI < Gitter :: hface_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const hface_STI * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

IteratorSTI < Gitter :: hface_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const IteratorSTI < hface_STI > * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

IteratorSTI < Gitter :: helement_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const helement_STI * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

IteratorSTI < Gitter :: helement_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const IteratorSTI < helement_STI > * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

IteratorSTI < Gitter :: hbndseg_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const hbndseg_STI * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

IteratorSTI < Gitter :: hbndseg_STI > * GitterBasisPll :: MacroGitterBasisPll :: iterator (const IteratorSTI < hbndseg_STI > * a) const {
  return GitterBasis :: MacroGitterBasis :: iterator (a) ;
}

GitterBasisPll :: GitterBasisPll (MpAccessLocal & mpa) : _mpaccess (mpa), _macrogitter (0) {
  _macrogitter = new MacroGitterBasisPll () ;
  assert (_macrogitter) ;
  notifyMacroGridChanges () ;
  return ;
}

GitterBasisPll :: GitterBasisPll (const char * f, MpAccessLocal & mpa) : _mpaccess (mpa), _macrogitter (0) {
  assert (debugOption (20) ? (cout << "GitterBasisPll :: GitterBasisPll (const char * = \"" << f << "\" ...)" << endl, 1) : 1) ;
  {
    char * extendedName = new char [strlen(f) + 200] ;
    sprintf (extendedName, "%s.%u", f, mpa.myrank ()) ;
    ifstream in (extendedName) ;
    if (in) {
      _macrogitter = new MacroGitterBasisPll (in) ;
    } else {
      cerr << "  GitterBasisPll :: GitterBasisPll () Datei: " << extendedName 
      	   << " kann nicht gelesen werden. In " << __FILE__ << " Zeile " << __LINE__ << endl ;
      _macrogitter = new MacroGitterBasisPll () ;
    }
    delete [] extendedName ;
  }
  assert (_macrogitter) ;
  notifyMacroGridChanges () ;
  return ;
}

GitterBasisPll :: ~GitterBasisPll () {
  delete _macrogitter ;
  return ;
}
