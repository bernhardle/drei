	// (c) bernhard schupp 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/gitter_pll_sti.cc,v $
	// $Revision: 2.6 $
	// $Name:  $
	// $State: Exp $
	// $Date: 1998/12/13 13:00:43 $
	
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <strstream.h>
#include <iterator.h>
#include <functional.h>
#include <algorithm.h>
#include <numeric.h>

#include "gitter_pll_sti.h"
#include "walk.h"

static volatile char RCSId_gitter_pll_sti_cc [] = "$Id: gitter_pll_sti.cc,v 2.6 1998/12/13 13:00:43 yc23 Exp yc23 $" ;

int __STATIC_myrank = -1 ;
int __STATIC_turn   = -1 ;
int __STATIC_phase  = -1 ;

pair < IteratorSTI < GitterPll :: vertex_STI > *, IteratorSTI < GitterPll :: vertex_STI > *> GitterPll :: 
	iteratorTT (const GitterPll :: vertex_STI *, int l) {

  vector < IteratorSTI < vertex_STI > * > _iterators_inner, _iterators_outer ;

  _iterators_inner.push_back (new AccessIteratorTT < vertex_STI > :: InnerHandle (containerPll (), l)) ;
  _iterators_outer.push_back (new AccessIteratorTT < vertex_STI > :: OuterHandle (containerPll (), l)) ;
  {
    AccessIteratorTT < hedge_STI > :: InnerHandle mie (containerPll (), l) ;
    AccessIteratorTT < hedge_STI > :: OuterHandle moe (containerPll (), l) ;
    Insert < AccessIteratorTT < hedge_STI > :: InnerHandle, 
	TreeIterator < hedge_STI, has_int_vertex < hedge_STI > > > lie (mie) ;
    Insert < AccessIteratorTT < hedge_STI > :: OuterHandle, 
	TreeIterator < hedge_STI, has_int_vertex < hedge_STI > > > loe (moe) ;
    _iterators_inner.push_back (new Wrapper < Insert < AccessIteratorTT < hedge_STI > :: InnerHandle, 
	TreeIterator < hedge_STI, has_int_vertex < hedge_STI > > >, InternalVertex > (lie)) ;
    _iterators_outer.push_back (new Wrapper < Insert < AccessIteratorTT < hedge_STI > :: OuterHandle,
	TreeIterator < hedge_STI, has_int_vertex < hedge_STI > > >, InternalVertex > (loe)) ;
  }
  {
    AccessIteratorTT < hface_STI > :: InnerHandle mfi (containerPll (), l) ;
    AccessIteratorTT < hface_STI > :: OuterHandle mfo (containerPll (), l) ;
    {
      Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
	TreeIterator < hface_STI, has_int_vertex < hface_STI > > > lfi (mfi) ;
      Insert < AccessIteratorTT < hface_STI > :: OuterHandle, 
	TreeIterator < hface_STI, has_int_vertex < hface_STI > > > lfo (mfo) ;
      _iterators_inner.push_back (new Wrapper < Insert < AccessIteratorTT < hface_STI > :: InnerHandle,
	TreeIterator < hface_STI, has_int_vertex < hface_STI > > >, InternalVertex > (lfi)) ;
      _iterators_outer.push_back (new Wrapper < Insert < AccessIteratorTT < hface_STI > :: OuterHandle,
	TreeIterator < hface_STI, has_int_vertex < hface_STI > > >, InternalVertex > (lfo)) ;
    }
    {
      Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
	TreeIterator < hface_STI, has_int_edge < hface_STI > > > lfi (mfi) ;
      Insert < AccessIteratorTT < hface_STI > :: OuterHandle, 
	TreeIterator < hface_STI, has_int_edge < hface_STI > > > lfo (mfo) ;
      Wrapper < Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge > dlfi (lfi) ;
      Wrapper < Insert < AccessIteratorTT < hface_STI > :: OuterHandle,
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge > dlfo (lfo) ;
      Insert < Wrapper < Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >,
	TreeIterator < hedge_STI, has_int_vertex < hedge_STI > > > vdlfi (dlfi) ;
      Insert < Wrapper < Insert < AccessIteratorTT < hface_STI > :: OuterHandle,
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >,
	TreeIterator < hedge_STI, has_int_vertex < hedge_STI > > > vdlfo (dlfo) ;
      _iterators_inner.push_back (new Wrapper < Insert < Wrapper < 
	Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >,
	TreeIterator < hedge_STI, has_int_vertex < hedge_STI > > >, InternalVertex > (vdlfi)) ;
      _iterators_outer.push_back (new Wrapper < 
	Insert < Wrapper < Insert < AccessIteratorTT < hface_STI > :: OuterHandle,
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >,
	TreeIterator < hedge_STI, has_int_vertex < hedge_STI > > >, InternalVertex > (vdlfo)) ;
    }
  }
  return pair < IteratorSTI < vertex_STI > *, IteratorSTI < vertex_STI > * > 
	(new VectorAlign < vertex_STI > (_iterators_inner), new VectorAlign < vertex_STI > (_iterators_outer)) ;
}

pair < IteratorSTI < GitterPll :: hedge_STI > *, IteratorSTI < GitterPll :: hedge_STI > * > GitterPll ::
	iteratorTT (const GitterPll :: hedge_STI *, int l) {
  AccessIteratorTT < hedge_STI > :: InnerHandle mdi (containerPll (), l) ;
  AccessIteratorTT < hedge_STI > :: OuterHandle mdo (containerPll (), l) ;

  Insert < AccessIteratorTT < hedge_STI > :: InnerHandle, TreeIterator < hedge_STI, is_leaf < hedge_STI > > > ei (mdi) ;  
  Insert < AccessIteratorTT < hedge_STI > :: OuterHandle, TreeIterator < hedge_STI, is_leaf < hedge_STI > > > eo (mdo) ;
  
  AccessIteratorTT < hface_STI > :: InnerHandle mfi (containerPll (), l) ;
  AccessIteratorTT < hface_STI > :: OuterHandle mfo (containerPll (), l) ;

  Insert < AccessIteratorTT < hface_STI > :: InnerHandle, TreeIterator < hface_STI, has_int_edge < hface_STI > > > fimi (mfi) ;
  Insert < AccessIteratorTT < hface_STI > :: OuterHandle, TreeIterator < hface_STI, has_int_edge < hface_STI > > > fimo (mfo) ;

  Wrapper < Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge > dfimi (fimi) ;
  Wrapper < Insert < AccessIteratorTT < hface_STI > :: OuterHandle,
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge > dfimo (fimo) ;

  Insert < Wrapper < Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >, 
	TreeIterator < hedge_STI, is_leaf < hedge_STI > > > eifi (dfimi) ;

  Insert < Wrapper < Insert < AccessIteratorTT < hface_STI > :: OuterHandle,
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >,
	TreeIterator < hedge_STI, is_leaf < hedge_STI > > > eifo (dfimo) ;

  return pair < IteratorSTI < hedge_STI > *, IteratorSTI < hedge_STI > * > 
    (new AlignIterator < Insert < AccessIteratorTT < hedge_STI > :: InnerHandle, TreeIterator < hedge_STI, is_leaf < hedge_STI > > >,
	Insert < Wrapper < Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >, 
	TreeIterator < hedge_STI, is_leaf < hedge_STI > > >, hedge_STI > (ei,eifi),
     new AlignIterator < Insert < AccessIteratorTT < hedge_STI > :: OuterHandle, TreeIterator < hedge_STI, is_leaf < hedge_STI > > >,
	Insert < Wrapper < Insert < AccessIteratorTT < hface_STI > :: OuterHandle,
	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >,
	TreeIterator < hedge_STI, is_leaf < hedge_STI > > >, hedge_STI > (eo, eifo)) ;
}

pair < IteratorSTI < GitterPll :: hface_STI > *, IteratorSTI < GitterPll :: hface_STI > *> 
	GitterPll :: iteratorTT (const GitterPll :: hface_STI *, int l) {
  AccessIteratorTT < hface_STI > :: InnerHandle mif (containerPll (), l) ;
  AccessIteratorTT < hface_STI > :: OuterHandle mof (containerPll (), l) ;
  return pair < IteratorSTI < hface_STI > *, IteratorSTI < hface_STI > * >
	(new Insert < AccessIteratorTT < hface_STI > :: InnerHandle, TreeIterator < hface_STI, is_leaf < hface_STI > > > (mif),
	 new Insert < AccessIteratorTT < hface_STI > :: OuterHandle, TreeIterator < hface_STI, is_leaf < hface_STI > > > (mof)) ;
}

void GitterPll :: printSizeTT () {
  cout << "\n GitterPll :: printSizeTT () \n\n" ;
  mpAccess ().printLinkage (cout) ;
  cout << endl ;
  { for (int l = 0 ; l < mpAccess ().nlinks () ; l ++ ) {
    LeafIteratorTT < vertex_STI > w (*this, l) ;
    cout << "me: " << mpAccess ().myrank () << " link: " << l << " vertices: [inner|outer] " << w.inner ().size () << " " << w.outer ().size () << endl ;
  }}
  { for (int l = 0 ; l < mpAccess ().nlinks () ; l ++ ) {
    LeafIteratorTT < hedge_STI > w (*this, l) ;
    cout << "me: " << mpAccess ().myrank () << " link: " << l << " kanten:   [inner|outer] " << w.inner ().size () << " " << w.outer ().size () << endl ;
  }}
  { for (int l = 0 ; l < mpAccess ().nlinks () ; l ++ ) {
    LeafIteratorTT < hface_STI > w (*this, l) ;
    cout << "me: " << mpAccess ().myrank () << " link: " << l << " fl\"achen: [inner|outer] " << w.inner ().size () << " " << w.outer ().size () << endl ;
  }}
  return ;
}

void GitterPll :: printsize () {
  const int me = mpAccess ().myrank (), np = mpAccess ().psize (), nl = mpAccess ().nlinks () ;
  
  if (debugOption (10)) Gitter :: printsize () ;
  vector < int > n ;
  {
    int sum = 0 ;
    for (int i = 0 ; i < nl ; i ++)
      sum += LeafIteratorTT < vertex_STI > (*this, i).outer ().size () ;
    n.push_back (LeafIterator < vertex_STI > (*this)->size() - sum) ;
  }
  {
    int sum = 0 ;
    for (int i = 0 ; i < nl ; i ++)
      sum += LeafIteratorTT < hedge_STI > (*this, i).outer ().size () ;
    n.push_back (LeafIterator < hedge_STI > (*this)->size() - sum) ;
  }
  int sumCutFaces = 0 ;
  {
    int sum = 0 ;
    for (int i = 0 ; i < nl ; i ++) {
      LeafIteratorTT < hface_STI > w (*this, i) ;
      sum += w.outer ().size () ;
      sumCutFaces += w.outer ().size () ;
      sumCutFaces += w.inner ().size () ;
    }
    n.push_back (LeafIterator < hface_STI > (*this)->size() - sum) ;
  }
  n.push_back (LeafIterator < helement_STI > (*this)->size()) ;
  n.push_back (LeafIterator < hbndseg_STI > (*this)->size() - sumCutFaces) ;
  vector < vector < int > > in = mpAccess ().gcollect (n) ;
  assert (in.size () == np) ;
  if (me == 0) {
    int nv = 0, nd = 0, nf = 0, ne = 0, nb = 0 ;
    for (int i = 0 ; i < np ; i ++ ) {
      nv += (in [i])[0] ;
      nd += (in [i])[1] ;
      nf += (in [i])[2] ;
      ne += (in [i])[3] ;
      nb += (in [i])[4] ;
    }
    cout << "\n GitterPll :: printSize () : \n\n" ;
    cout << " - Elemente ......... "  << ne << "\n" ;
    cout << " - R\"ander .......... " << nb << "\n" ;
    cout << " - Fl\"achen ......... " << nf << "\n" ;
    cout << " - Kanten ........... "  << nd << "\n" ;
    cout << " - Vertices ......... "  << nv << "\n" ;
    cout << endl ;
  }
  return ;
}

void GitterPll :: fullIntegrityCheck () {
  const int nl = mpAccess ().nlinks (), me = mpAccess ().myrank () ;
  int start = clock () ;
  Gitter :: fullIntegrityCheck () ;
  containerPll().fullIntegrityCheck (mpAccess ()) ;
  if (debugOption (0)) {
    cout << "**INFO GitterPll :: fullIntegrityCheck () used: " << (float)((float)(clock() - start)/(float)(CLOCKS_PER_SEC)) << " sec." << endl ;
  }
  return ;
}

void GitterPll :: backupCMode (const char * file) {
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: backupCMode (const char * = \""
  			<< (file ? file : "null") << "\")" << endl, 1) : 1) ;
  char * extendedName = new char [strlen (file) + 20] ;
  sprintf (extendedName, "%s.%d", file, mpAccess ().myrank ()) ;
  Gitter :: backupCMode (extendedName) ;
  delete [] extendedName ;
  return ;
}

void GitterPll :: backupCMode (ostream & out) {
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: backupCMode (ostream &)" << endl, 1) : 1) ;
  Gitter :: backupCMode (out) ;
  return ;
}

void GitterPll :: backup (const char * file) {
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: backup (const char * = \"" 
  			<< (file ? file : "null") << "\")" << endl, 1) : 1) ;
  char * extendedName = new char [strlen (file) + 20] ;
  sprintf (extendedName, "%s.%d", file, mpAccess ().myrank ()) ;
  Gitter :: backup (extendedName) ;
  delete [] extendedName ;
  return ;
}

void GitterPll :: backup (ostream & out) {
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: backup (ostream &)" << endl, 1) : 1) ;
  Gitter :: backup (out) ;
  return ;
}

void GitterPll :: restore (const char * file) {
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: restore (const char * = \"" << file << "\")" << endl, 1) : 1) ;
  char * extendedName = new char [strlen (file) + 20] ;
  sprintf (extendedName, "%s.%d", file, mpAccess ().myrank ()) ;
  Gitter :: restore (extendedName) ;
  delete [] extendedName ;
  return ;
}

void GitterPll :: restore (istream & in) {
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: restore (istream &)" << endl, 1) : 1) ;
  Gitter :: restore (in) ;
  return ;
}

pair < IteratorSTI < Gitter :: vertex_STI > *, IteratorSTI < Gitter :: vertex_STI > * >
  GitterPll :: MacroGitterPll :: iteratorTT (const vertex_STI *, int i) {
  assert (i < _vertexTT.size ()) ;
  return pair < IteratorSTI < vertex_STI > *, IteratorSTI < vertex_STI > * > 
	(new listSmartpointer__to__iteratorSTI < vertex_STI > (_vertexTT [i].first), 
         new listSmartpointer__to__iteratorSTI < vertex_STI > (_vertexTT [i].second)) ;
}

pair < IteratorSTI < Gitter :: vertex_STI > *, IteratorSTI < Gitter :: vertex_STI > * >
  GitterPll :: MacroGitterPll :: iteratorTT (const pair < IteratorSTI < vertex_STI > *, IteratorSTI < vertex_STI > * > & p, int) {
  return pair < IteratorSTI < vertex_STI > *, IteratorSTI < vertex_STI > * > 
	(new listSmartpointer__to__iteratorSTI < vertex_STI > (*(const listSmartpointer__to__iteratorSTI < vertex_STI > *)p.first),
         new listSmartpointer__to__iteratorSTI < vertex_STI > (*(const listSmartpointer__to__iteratorSTI < vertex_STI > *)p.second)) ;
}

pair < IteratorSTI < Gitter :: hedge_STI > *, IteratorSTI < Gitter :: hedge_STI > * >
  GitterPll :: MacroGitterPll :: iteratorTT (const hedge_STI *, int i) {
  assert (i < _hedgeTT.size ()) ;
  return pair < IteratorSTI < hedge_STI > *, IteratorSTI < hedge_STI > * > 
	(new listSmartpointer__to__iteratorSTI < hedge_STI > (_hedgeTT [i].first),
         new listSmartpointer__to__iteratorSTI < hedge_STI > (_hedgeTT [i].second)) ;
}

pair < IteratorSTI < Gitter :: hedge_STI > *, IteratorSTI < Gitter :: hedge_STI > * > 
  GitterPll :: MacroGitterPll :: iteratorTT (const pair < IteratorSTI < hedge_STI > *, IteratorSTI < hedge_STI > * > & p, int) {
  return pair < IteratorSTI < hedge_STI > *, IteratorSTI < hedge_STI > * > 
	(new listSmartpointer__to__iteratorSTI < hedge_STI > (*(const listSmartpointer__to__iteratorSTI < hedge_STI > *)p.first),
         new listSmartpointer__to__iteratorSTI < hedge_STI > (*(const listSmartpointer__to__iteratorSTI < hedge_STI > *)p.second)) ;
}

pair < IteratorSTI < Gitter :: hface_STI > *, IteratorSTI < Gitter :: hface_STI > * >
  GitterPll :: MacroGitterPll :: iteratorTT (const hface_STI *, int i) {
  assert (i < _hfaceTT.size ()) ;
  return pair < IteratorSTI < hface_STI > *, IteratorSTI < hface_STI > * > 
	(new listSmartpointer__to__iteratorSTI < hface_STI > (_hfaceTT [i].first),
         new listSmartpointer__to__iteratorSTI < hface_STI > (_hfaceTT [i].second)) ;
}

pair < IteratorSTI < Gitter :: hface_STI > *, IteratorSTI < Gitter :: hface_STI > * > 
  GitterPll :: MacroGitterPll :: iteratorTT (const pair < IteratorSTI < hface_STI > *, IteratorSTI < hface_STI > * > & p, int) {
  return pair < IteratorSTI < hface_STI > *, IteratorSTI < hface_STI > * >
    (new listSmartpointer__to__iteratorSTI < hface_STI > (*(const listSmartpointer__to__iteratorSTI < hface_STI > *)p.first),
     new listSmartpointer__to__iteratorSTI < hface_STI > (*(const listSmartpointer__to__iteratorSTI < hface_STI > *)p.second)) ;
}

bool GitterPll :: refine () {
  assert (debugOption (5) ? (cout << "**INFO GitterPll :: refine () " << endl, 1) : 1) ;
  const int nl = mpAccess ().nlinks (), start = clock () ;
  bool state = false ;
  vector < vector < hedge_STI * > > innerEdges (nl), outerEdges (nl) ;
  vector < vector < hface_STI * > > innerFaces (nl), outerFaces (nl) ;
  {
  	// Erst die Zeiger auf alle Fl"achen und Kanten mit paralleler
	// Mehrdeutigkeit sichern, da die LeafIteratorTT < . > nach dem 
	// Verfeinern auf gitter nicht mehr stimmen werden. Die Technik
	// ist zul"assig, da keine mehrfache Verfeinerung entstehen kann.
	
    {for (int l = 0 ; l < nl ; l ++) {
      LeafIteratorTT < hface_STI > fw (*this,l) ;
      LeafIteratorTT < hedge_STI > dw (*this,l) ;
      for (fw.outer ().first () ; ! fw.outer().done () ; fw.outer ().next ())
        outerFaces [l].push_back (& fw.outer ().item ()) ;
      for (fw.inner ().first () ; ! fw.inner ().done () ; fw.inner ().next ())
        innerFaces [l].push_back (& fw.inner ().item ()) ;
      for (dw.outer ().first () ; ! dw.outer().done () ; dw.outer ().next ())
        outerEdges [l].push_back (& dw.outer ().item ()) ;
      for (dw.inner ().first () ; ! dw.inner ().done () ; dw.inner ().next ())
        innerEdges [l].push_back (& dw.inner ().item ()) ;
    }}
    	// jetzt normal verfeinern und den Status der Verfeinerung
	// [unvollst"andige / vollst"andige Verfeinerung] sichern.
    
    __STATIC_phase = 1 ;
    
    state = Gitter :: refine () ;
       
	// Phase des Fl"achenausgleichs an den Schnittfl"achen des
	// verteilten Gitters. Weil dort im sequentiellen Fall pseudorekursive
	// Methodenaufrufe vorliegen k"onnen, muss solange iteriert werden,
	// bis die Situation global station"ar ist.
	
    __STATIC_phase = 2 ;
	
    bool repeat (false) ;
    _refineLoops = 0 ;
    do {
      repeat = false ;
      {
        vector < ObjectStream > osv (nl) ;
        try {
	  for (int l = 0 ; l < nl ; l ++) {
            {for (vector < hface_STI * > :: const_iterator i = outerFaces [l].begin () ;
              i != outerFaces [l].end () ; (*i ++)->accessPllX ().accessOuterPllX ().getRefinementRequest (osv [l])) ; }
            {for (vector < hface_STI * > :: const_iterator i = innerFaces [l].begin () ;
      	      i != innerFaces [l].end () ; (*i ++)->accessPllX ().accessOuterPllX ().getRefinementRequest (osv [l])) ; }
          }
	} catch (Parallel :: AccessPllException) {
          cerr << "**FEHLER (FATAL) AccessPllException in " << __FILE__ << " " << __LINE__ << endl ; abort () ;
        }
	
        osv = mpAccess ().exchange (osv) ;
	
        try {
	  for (int l = 0 ; l < nl ; l ++) {
            {for (vector < hface_STI * > :: const_iterator i = innerFaces [l].begin () ;
              i != innerFaces [l].end () ; repeat |= (*i ++)->accessPllX ().accessOuterPllX ().setRefinementRequest (osv [l])) ; }
            {for (vector < hface_STI * > :: const_iterator i = outerFaces [l].begin () ;
              i != outerFaces [l].end () ; repeat |= (*i ++)->accessPllX ().accessOuterPllX ().setRefinementRequest (osv [l])) ; }
          }
	} catch (Parallel :: AccessPllException) {
          cerr << "**FEHLER (FATAL) AccessPllException in " << __FILE__ << " " << __LINE__ << endl ; abort () ;
        }
      }

      _refineLoops ++ ;
    } while (mpAccess ().gmax (repeat ? 1 : 0)) ;

	// Jetzt noch die Kantensituation richtigstellen, es gen"ugt ein Durchlauf,
	// weil die Verfeinerung einer Kante keine Fernwirkungen hat. Vorsicht: Die
	// Kanten sind bez"uglich ihrer Identifikation sternf"ormig organisiert, d.h.
	// es muss die Verfeinerungsinformation einmal am Eigent"umer gesammelt und
	// dann wieder zur"ucktransportiert werden, eine einfache L"osung, wie bei
	// den Fl"achen (1/1 Beziehung) scheidet aus.

    __STATIC_phase = 3 ;

    {
      vector < ObjectStream > osv (nl) ;
      {for (int l = 0 ; l < nl ; l ++) 
        for (vector < hedge_STI * > :: const_iterator i = outerEdges [l].begin () ;
          i != outerEdges [l].end () ; (*i ++)->accessPllX ().getRefinementRequest (osv [l])) ;
      }
      osv = mpAccess ().exchange (osv) ;
      {for (int l = 0 ; l < nl ; l ++)
        for (vector < hedge_STI * > :: const_iterator i = innerEdges [l].begin () ;
      	  i != innerEdges [l].end () ; (*i ++)->accessPllX ().setRefinementRequest (osv [l])) ;
      }
    }		// ~vector < ObjectStream > ...	
    {
      vector < ObjectStream > osv (nl) ;
      {for (int l = 0 ; l < nl ; l ++)
        for (vector < hedge_STI * > :: const_iterator i = innerEdges [l].begin () ;
          i != innerEdges [l].end () ; (*i ++)->accessPllX ().getRefinementRequest (osv [l])) ;
      }
      osv = mpAccess ().exchange (osv) ;
      {for (int l = 0 ; l < nl ; l ++)
        for (vector < hedge_STI * > :: const_iterator i = outerEdges [l].begin () ;
      	  i != outerEdges [l].end () ; (*i ++)->accessPllX ().setRefinementRequest (osv [l])) ;
      }
    }		// ~vector < ObjectStream > ...	
  }
  
  __STATIC_phase = -1 ;
  
  return state ;
}

void GitterPll :: coarse () {
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: coarse () " << endl, 1) : 1) ;
  const int nl = mpAccess ().nlinks () ;
  
  {
    vector < vector < hedge_STI * > > innerEdges (nl), outerEdges (nl) ;
    vector < vector < hface_STI * > > innerFaces (nl), outerFaces (nl) ;
	
    for (int l = 0 ; l < nl ; l ++) {
    
	// Zun"achst werden f"ur alle Links die Zeiger auf Gitterojekte mit
	// Mehrdeutigkeit gesichert, die an der Wurzel einer potentiellen
	// Vergr"oberungsoperation sitzen -> es sind die Knoten in der Hierarchie,
	// deren Kinder alle Bl"atter sind. Genau diese Knoten sollen gegen"uber
	// der Vergr"oberung blockiert werden und dann die Vergr"oberung falls
	// sie zul"assig ist, sp"ater durchgef"uhrt werden (pending) ;
    
      AccessIteratorTT < hface_STI > :: InnerHandle mfwi (containerPll (),l) ;
      AccessIteratorTT < hface_STI > :: OuterHandle mfwo (containerPll (),l) ;
      AccessIteratorTT < hedge_STI > :: InnerHandle mdwi (containerPll (),l) ;
      AccessIteratorTT < hedge_STI > :: OuterHandle mdwo (containerPll (),l) ;
      
	// Die inneren und a"usseren Iteratoren der potentiell vergr"oberungsf"ahigen
	// Fl"achen "uber den Grobgitterfl"achen. In den Elementen passiert erstmal
	// nichts, solange nicht mit mehrfachen Grobgitterelementen gearbeitet wird.
      
      Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
      	TreeIterator < hface_STI, childs_are_leafs < hface_STI > > > fwi (mfwi) ;
      Insert < AccessIteratorTT < hface_STI > :: OuterHandle, 
      	TreeIterator < hface_STI, childs_are_leafs < hface_STI > > > fwo (mfwo) ;
      
	// Die inneren und a"usseren Iteratoren der potentiell vergr"oberungsf"ahigen
	// Kanten "uber den Grobgitterkanten.
      
      Insert < AccessIteratorTT < hedge_STI > :: InnerHandle, 
      	TreeIterator < hedge_STI, childs_are_leafs < hedge_STI > > > dwi (mdwi) ;
      Insert < AccessIteratorTT < hedge_STI > :: OuterHandle, 
      	TreeIterator < hedge_STI, childs_are_leafs < hedge_STI > > > dwo (mdwo) ;

	// Die inneren und a"usseren Iteratoren der potentiell vergr"oberungsf"ahigen
	// Kanten "uber den Grobgitterfl"achen. Diese Konstruktion wird beim Tetraeder-
	// gitter notwendig, weil dort keine Aussage der Form:
	//

      Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
      	TreeIterator < hface_STI, has_int_edge < hface_STI > > > efi (mfwi) ;
      Insert < AccessIteratorTT < hface_STI > :: OuterHandle, 
      	TreeIterator < hface_STI, has_int_edge < hface_STI > > > efo (mfwo) ;
      Wrapper < Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
      	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge > eifi (efi) ;
      Wrapper < Insert < AccessIteratorTT < hface_STI > :: OuterHandle, 
      	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge > eifo (efo) ;
      Insert < Wrapper < Insert < AccessIteratorTT < hface_STI > :: InnerHandle, 
      	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >,
	TreeIterator < hedge_STI, childs_are_leafs < hedge_STI > > > dfi (eifi) ;
      Insert < Wrapper < Insert < AccessIteratorTT < hface_STI > :: OuterHandle, 
      	TreeIterator < hface_STI, has_int_edge < hface_STI > > >, InternalEdge >,
	TreeIterator < hedge_STI, childs_are_leafs < hedge_STI > > > dfo (eifo) ;

	// Die 'item ()' Resultatwerte (Zeiger) werden in Vektoren gesichert, weil die
	// Kriterien die zur Erzeugung der Iteratoren angewendet wurden (Filter) nach
	// einer teilweisen Vergr"oberung nicht mehr g"ultig sein werden, d.h. die 
	// Iterationsobjekte "andern w"ahrend der Vergr"oberung ihre Eigenschaften.
	// Deshalb werden sie auch am Ende des Blocks aufgegeben. Der Vektor 'cache'
	// ist zul"assig, weil kein Objekt auf das eine Referenz im 'cache' vorliegt
	// beseitigt werden kann. Sie sind alle ein Niveau darunter.

      for (fwi.first () ; ! fwi.done () ; fwi.next ()) innerFaces [l].push_back (& fwi.item ()) ;
      for (fwo.first () ; ! fwo.done () ; fwo.next ()) outerFaces [l].push_back (& fwo.item ()) ;
      for (dwo.first () ; ! dwo.done () ; dwo.next ()) outerEdges [l].push_back (& dwo.item ()) ;
      for (dfo.first () ; ! dfo.done () ; dfo.next ()) outerEdges [l].push_back (& dfo.item ()) ;
      for (dwi.first () ; ! dwi.done () ; dwi.next ()) innerEdges [l].push_back (& dwi.item ()) ;
      for (dfi.first () ; ! dfi.done () ; dfi.next ()) innerEdges [l].push_back (& dfi.item ()) ;
    }
    try {
	// Erstmal alles was mehrdeutig ist, gegen die drohende Vergr"oberung sichern.
	// Danach werden sukzessive die Fl"achenlocks aufgehoben, getestet und
	// eventuell vergr"obert, dann das gleiche Spiel mit den Kanten.

      for (int l = 0 ; l < nl ; l ++) {
        {for (vector < hedge_STI * > :: iterator i = outerEdges [l].begin () ;
            i != outerEdges [l].end () ; (*i ++)->accessPllX ().lockAndTry ()) ; }
        {for (vector < hedge_STI * > :: iterator i = innerEdges [l].begin () ;
      	    i != innerEdges [l].end () ; (*i ++)->accessPllX ().lockAndTry ()) ; }
        {for (vector < hface_STI * > :: iterator i = outerFaces [l].begin () ;
            i != outerFaces [l].end () ; (*i ++)->accessPllX ().accessOuterPllX ().lockAndTry ()) ; }
        {for (vector < hface_STI * > :: iterator i = innerFaces [l].begin () ;
      	    i != innerFaces [l].end () ; (*i ++)->accessPllX ().accessOuterPllX ().lockAndTry ()) ; }
      }
      
      	// Gitter :: coarse () ist elementorientiert, d.h. die Vergr"oberung auf Fl"achen und
	// Kanten wird nur durch Vermittlung eines sich vergr"obernden Knotens in der Element-
	// hierarchie angestossen. In allen gegen Vergr"oberung 'gelockten' Fl"achen und Kanten
	// wird die angeforderte Operation zur"uckgewiesen, um erst sp"ater von aussen nochmals
	// angestossen zu werden.
      
      __STATIC_phase = 4 ;
      
      Gitter :: coarse () ;
      
    } catch (Parallel :: AccessPllException) {
      cerr << "**FEHLER (FATAL) AccessPllException beim Vergr\"obern der Elementhierarchie oder\n" ;
      cerr << "  beim locken der Fl\"achen- bzw. Kantenb\"aume aufgetreten. In " << __FILE__ << " " << __LINE__ << endl ;
      abort () ;
    }
    try {
    
	// Phase des Fl"achenausgleichs des verteilten Vergr"oberungsalgorithmus
	// alle Schnittfl"achenpaare werden daraufhin untersucht, ob eine
	// Vergr"oberung in beiden Teilgittern durchgef"uhrt werden darf,
	// wenn ja, wird in beiden Teilgittern vergr"obert und der Vollzug
	// getestet.
	
      __STATIC_phase = 5 ;
    
      vector < vector < int > > clean (nl) ;
      {
        vector < vector < int > > inout (nl) ;
        {for (int l = 0 ; l < nl ; l ++)
          for (vector < hface_STI * > :: iterator i = outerFaces [l].begin () ; i != outerFaces [l].end () ; i ++)
            inout [l].push_back ((*i)->accessPllX ().accessOuterPllX ().lockAndTry ()) ;
        }
        inout = mpAccess ().exchange (inout) ;
        {for (int l = 0 ; l < nl ; l ++) {
          clean [l] = vector < int > (innerFaces [l].size (), long (true)) ;
	  vector < int > :: iterator j = clean [l].begin (), k = inout [l].begin () ;
          for (vector < hface_STI * > :: iterator i = innerFaces [l].begin () ; i != innerFaces [l].end () ; i ++, j++, k++) {
	    assert (j != clean [l].end ()) ; assert (k != inout [l].end ()) ;
	    (*j) &= (*k) && (*i)->accessPllX ().accessOuterPllX ().lockAndTry () ;
	  }
        }}
      }
      {
        vector < vector < int > > inout (nl) ;
        {for (int l = 0 ; l < nl ; l ++) {
          vector < int > :: iterator j = clean [l].begin () ;
	  for (vector < hface_STI * > :: iterator i = innerFaces [l].begin () ; i != innerFaces [l].end () ; i ++, j++) {
	    inout [l].push_back (*j) ;
	    (*i)->accessPllX ().accessOuterPllX ().unlockAndResume (bool (*j)) ;
	  }
        }}
      
        inout = mpAccess ().exchange (inout) ;
      
        {for (int l = 0 ; l < nl ; l ++) {
          vector < int > :: iterator j = inout [l].begin () ;
          for (vector < hface_STI * > :: iterator i = outerFaces [l].begin () ; i != outerFaces [l].end () ; i ++, j++) {
	    assert (j != inout [l].end ()) ;
	    (*i)->accessPllX ().accessOuterPllX ().unlockAndResume (bool (*j)) ;
	  }
        }}
      }
    } catch (Parallel :: AccessPllException) {
      cerr << "**FEHLER (FATAL) AccessPllException beim Vergr\"obern der Fl\"achenb\"aume\n" ;
      cerr << "  aufgetreten. In " << __FILE__ << " " << __LINE__ << endl ;
      abort () ;
    }
    try {
    
	// Phase des Kantenausgleichs im parallelen Vergr"oberungsalgorithmus:
	
      __STATIC_phase  = 6 ;
    
	// Weil hier jede Kante nur eindeutig auftreten darf, muss sie in einem
	// map als Adresse hinterlegt werden, dann k"onnen die verschiedenen
	// Refcounts aus den verschiedenen Links tats"achlich global miteinander
	// abgemischt werden. Dazu werden zun"achst alle eigenen Kanten auf ihre
	// Vergr"oberbarkeit hin untersucht und dieser Zustand (true = vergr"oberbar
	// false = darf nicht vergr"obert werden) im map 'clean' hinterlegt. Dazu
	// kommt noch ein zweiter 'bool' Wert, der anzeigt ob die Kante schon ab-
	// schliessend vergr"obert wurde oder nicht. 
    
      map < hedge_STI *, pair < bool, bool >, less < hedge_STI * > > clean ;
      
      {for (int l = 0 ; l < nl ; l ++)
        for (vector < hedge_STI * > :: iterator i = innerEdges [l].begin () ; i != innerEdges [l].end () ; i ++)
	  if (clean.find (*i) == clean.end ()) clean [*i] = pair < bool, bool > ((*i)->accessPllX ().lockAndTry (), true) ;
      }
      {
        vector < vector < int > > inout (nl) ;
        {for (int l = 0 ; l < nl ; l ++)
          for (vector < hedge_STI * > :: iterator i = outerEdges [l].begin () ; i != outerEdges [l].end () ; i ++)
            inout [l].push_back ((*i)->accessPllX ().lockAndTry ()) ;
	}
        inout = mpAccess ().exchange (inout) ;
        {for (int l = 0 ; l < nl ; l ++) {
	  vector < int > :: const_iterator j = inout [l].begin () ;
          for (vector < hedge_STI * > :: iterator i = innerEdges [l].begin () ; i != innerEdges [l].end () ; i ++, j++) {
	    assert (j != inout [l].end ()) ;
            assert (clean.find (*i) != clean.end ()) ;
            if (*j == false) clean [*i] = pair < bool, bool > (false, clean[*i].second) ; 
	  }
        }}
      }
      {
        vector < vector < int > > inout (nl) ;
        {for (int l = 0 ; l < nl ; l ++) {
	  for (vector < hedge_STI * > :: iterator i = innerEdges [l].begin () ; i != innerEdges [l].end () ; i ++) {
	    assert (clean.find (*i) != clean.end ()) ;
	    pair < bool, bool > & a = clean [*i] ;
	    inout [l].push_back (a.first) ;
	    if (a.second) {
	
	// Wenn wir hier sind, kann die Kante tats"achlich vergr"obert werden, genauer gesagt,
	// sie wird es auch und der R"uckgabewert testet den Vollzug der Aktion. Weil aber nur
	// einmal vergr"obert werden kann, und die Iteratoren 'innerEdges [l]' aber eventuell
	// mehrfach "uber eine Kante hinweglaufen, muss diese Vergr"oberung im map 'clean'
	// vermerkt werden. Dann wird kein zweiter Versuch unternommen.
	
	      a.second = false ;
	      bool b = (*i)->accessPllX ().unlockAndResume (a.first) ;
	      assert (b == a.first) ;
	    }
	  }
        }}
        inout = mpAccess ().exchange (inout) ;
        {for (int l = 0 ; l < nl ; l ++) {
          vector < int > :: iterator j = inout [l].begin () ;
          for (vector < hedge_STI * > :: iterator i = outerEdges [l].begin () ; i != outerEdges [l].end () ; i ++, j++) {
	    assert (j != inout [l].end ()) ;
	    
	// Selbe Situation wie oben, aber der Eigent"umer der Kante hat mitgeteilt, dass sie
	// vergr"obert werden darf und auch wird auf allen Teilgebieten also auch hier. Der
	// Vollzug der Vergr"oberung wird durch den R"uckgabewert getestet.
	    
	    bool b = (*i)->accessPllX ().unlockAndResume (bool (*j)) ;
	    assert (b == bool (*j)) ;
	  }
        }}
      }
    } catch (Parallel :: AccessPllException) {
      cerr << "**FEHLER (FATAL) AccessPllException beim Vergr\"obern der Kantenb\"aume\n" ;
      cerr << "  aufgetreten. In " << __FILE__ << " " << __LINE__ << endl ;
      abort () ;
    }
  }
  
  __STATIC_phase = -1 ;
  
  return ;
}

void GitterPll :: adapt () {
  __STATIC_myrank = mpAccess ().myrank () ;
  __STATIC_turn ++ ;
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: adapt ()" << endl, 1) : 1) ;
  assert (! iterators_attached ()) ;
  int start = clock () ;
  refine () ;
  int lap = clock () ;
  coarse () ;
  int end = clock () ;
  if (debugOption (1)) {
    float u1 = (float)(lap - start)/(float)(CLOCKS_PER_SEC) ;
    float u2 = (float)(end - lap)/(float)(CLOCKS_PER_SEC) ;
    float u3 = (float)(end - start)/(float)(CLOCKS_PER_SEC) ;
    cout << "**INFO GitterPll :: adapt () [ref (loops)|cse|all] " << u1 << " ("
         << _refineLoops << ") " << u2 << " " << u3 << endl ;
  }
  notifyGridChanges () ;
  loadBalancerGridChangesNotify () ;
  return ;
}

void GitterPll :: MacroGitterPll :: fullIntegrityCheck (MpAccessLocal & mpa) {
  const int nl = mpa.nlinks (), me = mpa.myrank () ;
  int start = clock () ;
  try {
    vector < vector < int > > inout (nl) ;

    {for (int l = 0 ; l < nl ; l ++) {
      AccessIteratorTT < hface_STI > :: InnerHandle w (*this,l) ;
      for ( w.first () ; ! w.done () ; w.next ()) {
        vector < int > i = w.item ().accessPllX ().checkParallelConnectivity () ;
        copy (i.begin (), i.end (), back_inserter (inout [l])) ;
      }
    }}
    inout = mpa.exchange (inout) ;
    {for (int l = 0 ; l < nl ; l ++) {
      vector < int > :: const_iterator pos = inout [l].begin () ;
      AccessIteratorTT < hface_STI > :: OuterHandle w (*this,l) ;
      for (w.first () ; ! w.done () ; w.next ()) {
        vector < int > t1 = w.item ().accessPllX ().checkParallelConnectivity () ;
        vector < int > t2 (t1.size (), 0) ;
        copy (pos, pos + t1.size (), t2.begin ()) ;
        pos += t1.size () ;
        if (t1 != t2) {
          cerr << "fehler an gebiet " << me << " : " ;
#ifdef IBM_XLC
          copy (t1.begin (), t1.end (), ostream_iterator < int /* , char */ > (cerr, "-")) ;
#else
          copy (t1.begin (), t1.end (), ostream_iterator < int , char > (cerr, "-")) ;
#endif
	  cerr << "\t" ;
#ifdef IBM_XLC
          copy (t2.begin (), t2.end (), ostream_iterator < int /* , char */ > (cerr, "-")) ;
#else
          copy (t2.begin (), t2.end (), ostream_iterator < int , char > (cerr, "-")) ;
#endif
          cerr << endl ;
        }
      }
    }}
  } catch (Parallel ::  AccessPllException) {
    cerr << "**FEHLER (FATAL) Parallel :: AccessPllException entstanden in: " << __FILE__ << " " << __LINE__ << endl ;
  }
  return ;
}

void GitterPll :: exchangeDynamicState () {

	// Die Methode wird jedesmal aufgerufen, wenn sich der dynamische
	// Zustand des Gitters ge"andert hat: Verfeinerung und alle Situationen
	// die einer "Anderung des statischen Zustands entsprechen. Sie wird in
	// diesem Fall NACH dem Update des statischen Zustands aufgerufen, und
	// kann demnach von einem korrekten statischen Zustand ausgehen. F"ur
	// Methoden die noch h"aufigere Updates erfordern m"ussen diese in der
	// Regel hier eingeschleift werden.

  const int nl = mpAccess ().nlinks () ;
  const int start = clock () ;
  try {
    vector < ObjectStream > osv (nl) ;
    {for (int l = 0 ; l < nl ; l ++) {
      LeafIteratorTT < hface_STI > w (*this,l) ;
      for (w.inner ().first () ; ! w.inner ().done () ; w.inner ().next ())
        w.inner ().item ().accessPllX ().accessInnerPllX ().writeDynamicState (osv [l]) ;
      for (w.outer ().first () ; ! w.outer ().done () ; w.outer ().next ())
        w.outer ().item ().accessPllX ().accessInnerPllX ().writeDynamicState (osv [l]) ;
    }}
    osv = mpAccess ().exchange (osv) ;
    {for (int l = 0 ; l < nl ; l ++ ) {
      LeafIteratorTT < hface_STI > w (*this,l) ;
      for (w.outer ().first () ; ! w.outer ().done () ; w.outer ().next ())
        w.outer ().item ().accessPllX ().accessOuterPllX ().readDynamicState (osv [l]) ;
      for (w.inner ().first () ; ! w.inner ().done () ; w.inner ().next ())
        w.inner ().item ().accessPllX ().accessOuterPllX ().readDynamicState (osv [l]) ;
    }}
  } catch (Parallel ::  AccessPllException) {
    cerr << "  FEHLER Parallel :: AccessPllException entstanden in: " << __FILE__ << " " << __LINE__ << endl ;
  }
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: exchangeDynamicState () used " 
  	<< (float)(clock () - start)/(float)(CLOCKS_PER_SEC) << " sec. " << endl, 1) : 1 ) ;
  return ;
}

void GitterPll :: exchangeStaticState () {

	// Die Methode wird jedesmal aufgerufen, wenn sich der statische
	// Zustand (d.h. der Zustand, der mit dem Makrogitter verbunden ist)
	// ge"andert hat: Makrogitteraufbau und Lastvertielung. Der statische
	// Zustand darf durch Verfeinerung und h"ohere Methoden nicht beeinflusst
	// sein.

  const int start = clock () ;
  try {
    const int nl = mpAccess ().nlinks () ;
    vector < ObjectStream > osv (nl) ;
    {for (int l = 0 ; l < nl ; l ++ ) {
      AccessIteratorTT < hface_STI > :: InnerHandle wi (containerPll (),l) ;
      AccessIteratorTT < hface_STI > :: OuterHandle wo (containerPll (),l) ;
      for (wi.first () ; ! wi.done () ; wi.next ())
        wi.item ().accessPllX ().accessInnerPllX ().writeStaticState (osv [l]) ;
      for (wo.first () ; ! wo.done () ; wo.next ())
        wo.item ().accessPllX ().accessInnerPllX ().writeStaticState (osv [l]) ;
    }}
    osv = mpAccess ().exchange (osv) ;
    {for (int l = 0 ; l < nl ; l ++) {
      AccessIteratorTT < hface_STI > :: InnerHandle wi (containerPll (),l) ;
      AccessIteratorTT < hface_STI > :: OuterHandle wo (containerPll (),l) ;
      for (wo.first () ; ! wo.done () ; wo.next ())
        wo.item ().accessPllX ().accessOuterPllX ().readStaticState (osv [l]) ;
      for (wi.first () ; ! wi.done () ; wi.next ())
        wi.item ().accessPllX ().accessOuterPllX ().readStaticState (osv [l]) ;
    }}
  } catch (Parallel ::  AccessPllException) {
    cerr << "  FEHLER Parallel :: AccessPllException entstanden in: " << __FILE__ << " " << __LINE__ << endl ;
  }
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: exchangeStaticState () used " 
  	<< (float)(clock () - start)/(float)(CLOCKS_PER_SEC) << " sec. " << endl, 1) : 1 ) ;
  return ;
}

void GitterPll :: loadBalancerGridChangesNotify () {
  assert (debugOption (20) ? (cout << "**GitterPll :: loadBalancerGridChangesNotify () " << endl, 1) : 1) ;
  const int start = clock (), me = mpAccess ().myrank (), np = mpAccess ().psize () ;
  LoadBalancer :: DataBase db ;
  {
    AccessIterator < hface_STI > :: Handle w (containerPll ()) ;
    for (w.first () ; ! w.done () ; w.next ()) w.item ().accessPllX ().ldbUpdateGraphEdge (db) ;
  }
  {
    AccessIterator < helement_STI > :: Handle w (containerPll ()) ;
    for (w.first () ; ! w.done () ; w.next ()) w.item ().accessPllX ().ldbUpdateGraphVertex (db) ;
  }
  bool neu = false ;
  {
	// Kriterium, wann eine Lastneuverteilung vorzunehmen ist:
	// 
	// load  - eigene ElementLast
	// mean  - mittlere ElementLast
	// nload - Lastverh"altnis

	
    double load = db.accVertexLoad () ;
    vector < double > v (mpAccess ().gcollect (load)) ;
    double mean = accumulate (v.begin (), v.end (), 0.0) / double (np) ;

    for (vector < double > :: iterator i = v.begin () ; i != v.end () ; i ++)
      neu |= (*i > mean ? (*i > (_ldbOver * mean) ? true : false) : (*i < (_ldbUnder * mean) ? true : false)) ;
  }
  if (neu) {
    if (mpAccess ().gmax (_ldbMethod)) {
      repartitionMacroGrid (db) ;
      notifyMacroGridChanges () ;
    }
  }
  return ;
}

void GitterPll :: loadBalancerMacroGridChangesNotify () {

	// Diese Methode beschreibt die Reaktion des Lastverteilers bzw.
	// seiner Datengrundlage auf "Anderungen des Grobgitters, d.h.
	// auf "Anderungen in der Grobgitterverteilung, Gr"osse usw.

  assert (debugOption (20) ? (cout << "**INFO GitterPll :: loadBalancerMacroGridChangesNotify () " << endl, 1) : 1) ;
  int cnt = 0 ;
  AccessIterator < helement_STI > :: Handle w (containerPll ()) ;
  vector < int > sizes = mpAccess ().gcollect (w.size ()) ;
  for (int i = 0 ; i < mpAccess ().myrank () ; cnt += sizes [i++]) ;
  for (w.first () ; ! w.done () ; w.next ()) w.item ().accessPllX ().ldbVertexIndex () = cnt ++ ;
  return ;
}

void GitterPll :: notifyGridChanges () {
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: notifyGridChanges () " << endl, 1) : 1 ) ;
  Gitter :: notifyGridChanges () ;
  exchangeDynamicState () ;
  return ;
}

void GitterPll :: notifyMacroGridChanges () {
  assert (debugOption (20) ? (cout << "**INFO GitterPll :: notifyMacroGridChanges () " << endl, 1) : 1 ) ;
  Gitter :: notifyMacroGridChanges () ;
  Gitter :: notifyGridChanges () ;
  containerPll ().identification (mpAccess ()) ;
  loadBalancerMacroGridChangesNotify () ;
  exchangeStaticState () ;
  exchangeDynamicState () ;
  return ;
}

GitterPll :: GitterPll () : _ldbOver (1.2), _ldbUnder (0.0), _ldbMethod (LoadBalancer :: DataBase :: METIS_PartGraphKway) {
  ifstream in ("lastverteilung.cfg") ;
  if (in) {
    int i ;
    in >> _ldbUnder ;
    in >> _ldbOver ;
    in >> i;
    _ldbMethod = (LoadBalancer :: DataBase :: method) i ;
  } else {
    cerr << "**WARNUNG (IGNORIERT) Datei < lastverteilung.cfg > konnte nicht ge\"offnet werden. Es werden die Standardwerte verwendet." << endl ;
  }
  cout << "**INFO GitterPll :: GitterPll () " << _ldbUnder << " < [Balance der Verteilung] < " << _ldbOver << ",\n" 
       << "       Methode zur Partitionierung \"" << LoadBalancer :: DataBase :: methodToString (_ldbMethod) << "\"" << endl ;
  return ;
}
