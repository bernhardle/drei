	// (c) mario ohlberger, 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/gitter_tetra_top.h,v $
	// $Revision: 2.5 $
	// $Name:  $
	// $State: Exp $
	// $Date$

#ifndef GITTER_TetraTop_H_INCLUDED
#define GITTER_TetraTop_H_INCLUDED

#include "gitter_hexa_top.h"

static volatile char RCSId_gitter_tetra_top_h [] = "$Id: gitter_tetra_top.h,v 2.5 1998/12/01 17:13:41 yc23 Exp yc23 $" ;

template < class A > class Hface3Top : public A {
  protected :
    typedef Hface3Top < A > 		innerface_t ;
    typedef typename A :: inneredge_t 	inneredge_t;
    typedef typename A :: innervertex_t innervertex_t;
    typedef typename A :: myhedge1_t 	myhedge1_t ;
    typedef typename A :: myrule_t      myrule_t ;
  private :
    innerface_t * _dwn, * _bbb ;
    inneredge_t * _ed ;
//    innervertex_t * _cv ; Es gibt bei den bisherigen Regeln keinen inneren Knoten
    int _lvl ;
    myrule_t _rule ;
    inline myhedge1_t * subedge1 (int,int) ;
    inline const myhedge1_t * subedge1 (int,int) const ;
    void split_e01 () ;
    void split_e12 () ;
    void split_e20 () ;
    void split_iso4 () ;
  public :
    inline Hface3Top (int,myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int) ;
    virtual inline ~Hface3Top () ;
    innervertex_t * subvertex (int) ;
    const innervertex_t * subvertex (int) const ;
    inneredge_t * subedge1 (int) ;
    const inneredge_t * subedge1 (int) const ;
    innerface_t * subface3 (int) ;
    const innerface_t * subface3 (int) const ;
    int level () const ;
    innervertex_t * innerVertex () ;
    const innervertex_t * innerVertex () const ;
    inneredge_t * innerHedge () ;
    const inneredge_t * innerHedge () const ;
    innerface_t * down () ;
    const innerface_t * down () const ;
    innerface_t * next () ;
    const innerface_t * next () const ;
    void append (innerface_t * f) ;
  public:
    virtual myrule_t getrule () const ;
    virtual bool refine (myrule_t,int) ;
    virtual void refineImmediate (myrule_t) ;
    virtual bool coarse () ;
  public :
    virtual void backup (ostream &) const ;
    virtual void restore (istream &) ;
} ;

template < class A > class Hbnd3Top : public A {
  protected :
    typedef Hbnd3Top < A > 		innerbndseg_t ;
    typedef typename A :: myhface3_t 	myhface3_t ;
    typedef typename A :: balrule_t	balrule_t ;
    bool refineLikeElement (balrule_t) ;
    inline void append (innerbndseg_t *) ;
  private :
    innerbndseg_t * _bbb, * _dwn ;
    int _lvl ;
    void split_e01 () ;
    void split_e12 () ;
    void split_e20 () ;
    void split_iso4 () ;
    inline bool coarse () ;
  public :
    inline Hbnd3Top (int,myhface3_t *,int) ;
    inline virtual ~Hbnd3Top () ;
    bool refineBalance (balrule_t,int) ;
    bool bndNotifyCoarsen () ;
    void restoreFollowFace () ;
    inline int level () const ;
    inline innerbndseg_t * next () ;
    inline innerbndseg_t * down () ;
    inline const innerbndseg_t * next () const ;
    inline const innerbndseg_t * down () const ;
} ;

template < class A > class TetraTop : public A {
  protected :
    typedef TetraTop < A > 		innertetra_t  ;
    typedef typename A :: innervertex_t innervertex_t ;
    typedef typename A :: inneredge_t 	inneredge_t ;
    typedef typename A :: innerface_t 	innerface_t ;
    typedef typename A :: myhedge1_t 	myhedge1_t ;
    typedef typename A :: myhface3_t 	myhface3_t ;
    typedef typename A :: myrule_t 	myrule_t ;
    typedef typename A :: balrule_t     balrule_t ;
    inline void refineImmediate (myrule_t) ;
    inline void append (innertetra_t * h) ;
  private :
    innertetra_t * _dwn, * _bbb ;
    innerface_t * _fc ;
    inneredge_t * _ed ;
//    innervertex_t * _cv ; Es gibt bei den bisherigen Regeln keinen inneren Knoten
    int _lvl ;
    myrule_t _req, _rule ;
  private :
    void split_e01 () ;
    void split_e12 () ;
    void split_e20 () ;
    void split_e23 () ;
    void split_e30 () ;
    void split_e31 () ;
    void split_iso8 () ;
  protected :
    myhedge1_t * subedge1 (int,int) ;
    const myhedge1_t * subedge1 (int,int) const ;
    myhface3_t * subface3 (int,int) ;
    const myhface3_t * subface3 (int i, int j) const ;
  public:
    inline TetraTop (int,myhface3_t *,int,myhface3_t *,int,myhface3_t *,int,myhface3_t *,int) ;
    virtual inline ~TetraTop () ;
    inline innertetra_t * down () ;
    inline const innertetra_t * down () const ;
    inline innertetra_t * next () ;
    inline const innertetra_t * next () const ;
    inline innervertex_t * innerVertex () ;
    inline const innervertex_t * innerVertex () const ;
    inline inneredge_t * innerHedge () ;
    inline const inneredge_t * innerHedge () const ;
    inline innerface_t * innerHface () ;
    inline const innerface_t * innerHface () const ;
    inline int level () const ;
  public :
    myrule_t getrule () const ;
    bool refine () ;
    void request (myrule_t) ;
    bool refineBalance (balrule_t,int) ;
    bool coarse () ;
    bool bndNotifyCoarsen () ;
    void backupCMode (ostream &) const ;
    void backup (ostream &) const ;
    void restore (istream &) ;
};


	//
	//    #    #    #  #          #    #    #  ######
	//    #    ##   #  #          #    ##   #  #
	//    #    # #  #  #          #    # #  #  #####
	//    #    #  # #  #          #    #  # #  #
	//    #    #   ##  #          #    #   ##  #
	//    #    #    #  ######     #    #    #  ######
	//


// #     #                                  #####  #######
// #     #  ######    ##     ####   ###### #     #    #      ####   #####
// #     #  #        #  #   #    #  #            #    #     #    #  #    #
// #######  #####   #    #  #       #####   #####     #     #    #  #    #
// #     #  #       ######  #       #            #    #     #    #  #####
// #     #  #       #    #  #    #  #      #     #    #     #    #  #
// #     #  #       #    #   ####   ######  #####     #      ####   #


template < class A > Hface3Top < A > :: innerface_t * Hface3Top < A > :: down () {
  return _dwn ;
}

template < class A > const Hface3Top < A > :: innerface_t * Hface3Top < A > :: down () const {
  return _dwn ;
}

template < class A > Hface3Top < A > :: innerface_t * Hface3Top < A > :: next () {
  return _bbb ;
}

template < class A > const Hface3Top < A > :: innerface_t * Hface3Top < A > :: next () const {
  return _bbb ;
}

template < class A > int Hface3Top < A > :: level () const {
  return _lvl ;
}

template < class A > Hface3Top < A > :: innervertex_t * Hface3Top < A > :: innerVertex () {
  return 0 ;
}

template < class A > const Hface3Top < A > :: innervertex_t * Hface3Top < A > :: innerVertex () const {
  return 0 ;
}

template < class A > Hface3Top < A > :: inneredge_t * Hface3Top < A > :: innerHedge () {
  return _ed ;
}

template < class A > const Hface3Top < A > :: inneredge_t * Hface3Top < A > :: innerHedge () const {
  return _ed ;
}

template < class A > Hface3Top < A > :: innervertex_t * Hface3Top < A > :: subvertex (int) {
  assert (getrule() == myrule_t :: iso4) ;
  return 0 ;
}

template < class A > const Hface3Top < A > :: innervertex_t * Hface3Top < A > :: subvertex (int) const {
  assert (getrule() == myrule_t :: iso4) ;
  return 0 ;
}

template < class A > Hface3Top < A > :: myhedge1_t * Hface3Top < A > :: subedge1 (int i,int j) {
  assert(j == 0 || j == 1) ;
  return myhedge1 (i)->subedge1 (j ? 1 - twist(i) : twist(i)) ;
}

template < class A > const Hface3Top < A > :: myhedge1_t * Hface3Top < A > :: subedge1 (int i,int j) const {
  assert(j == 0 || j == 1) ;
  return myhedge1 (i)->subedge1 (j ? 1 - twist(i) : twist(i)) ;
}

template < class A > Hface3Top < A > :: inneredge_t * Hface3Top < A > :: subedge1 (int n) {
  inneredge_t * e = _ed ;
  for (int i = 0 ; i < n ; i ++ ) e = e ? e->next () : 0 ;
  assert (e) ;
  return e ;
}

template < class A > const Hface3Top < A > :: inneredge_t * Hface3Top < A > :: subedge1 (int n) const {
  inneredge_t * e = _ed ;
  for (int i = 0 ; i < n ; i ++ ) e = e ? e->next () : 0 ;
  assert (e) ;
  return e ;
}

template < class A > Hface3Top < A > :: innerface_t * Hface3Top < A > :: subface3 (int n) {
  innerface_t * f = down() ;
  for (int i = 0 ; i < n ; i++ ) f = f ? f->next () : 0 ;
  assert (f) ;
  return f ;
}

template < class A > const Hface3Top < A > :: innerface_t * Hface3Top < A > :: subface3 (int n) const {
  const innerface_t * f = down () ;
  for (int i = 0 ; i < n ; i++ ) f = f ? f->next () : 0 ;
  assert (f) ;
  return f ;
}

template < class A > void Hface3Top < A > :: append (innerface_t * f) {
  assert (!_bbb) ;
  _bbb = f ;
  return ;
}

template < class A > Hface3Top < A > :: myrule_t Hface3Top < A > :: getrule () const {
  return myrule_t (_rule) ;
}

template < class A > void Hface3Top < A > :: split_e01 () {
  int l = 1 + level () ;
  myvertex_t * ev0 = myhedge1(1)->subvertex (2) ;
  assert(ev0) ;
  inneredge_t * e0 = new inneredge_t (l, ev0, myvertex (2)) ;
  assert( e0 ) ;
  innerface_t * f0 = new innerface_t (l, myhedge1(2), twist(2), subedge1(0,0), twist(0) , e0, 0) ;
  innerface_t * f1 = new innerface_t (l, e0, 1, subedge1(0,1), twist(0), myhedge1(1),  twist(1)) ;
  assert (f0 && f1 ) ;
  f0->append(f1) ;
  _ed  = e0 ;
  _dwn = f0 ;
  _rule = myrule_t :: e01 ;
  return ;
}

template < class A > void Hface3Top < A > :: split_e12 () {
  int l = 1 + level () ;
  myvertex_t * ev0 = myhedge1(1)->subvertex (0) ;
  assert(ev0) ;
  inneredge_t * e0 = new inneredge_t (l, ev0, myvertex (0)) ;
  assert( e0 ) ;
  innerface_t * f0 = new innerface_t (l, myhedge1(0), twist(0), subedge1(1,0), twist(1) , e0, 0) ;
  innerface_t * f1 = new innerface_t (l, e0, 1, subedge1(1,1), twist(1), myhedge1(2),  twist(2)) ;
  assert (f0 && f1 ) ;
  f0->append(f1) ;
  _ed  = e0 ;
  _dwn = f0 ;
  _rule = myrule_t :: e12 ;
  return ;
}

template < class A > void Hface3Top < A > :: split_e20 () {
  int l = 1 + level () ;
  myvertex_t * ev0 = myhedge1(1)->subvertex (1) ;
  assert(ev0) ;
  inneredge_t * e0 = new inneredge_t (l, ev0, myvertex (1)) ;
  assert( e0 ) ;
  innerface_t * f0 = new innerface_t (l, myhedge1(1), twist(1), subedge1(2,0), twist(2) , e0, 0) ;
  innerface_t * f1 = new innerface_t (l, e0, 1, subedge1(2,1), twist(2), myhedge1(0),  twist(0)) ;
  assert (f0 && f1 ) ;
  f0->append(f1) ;
  _ed  = e0 ;
  _dwn = f0 ;
  _rule = myrule_t :: e20 ;
  return ;
}

template < class A > void Hface3Top < A > :: split_iso4 () {
  int l = 1 + level () ;
  myvertex_t * ev0 = myhedge1(0)->subvertex (0) ;
  myvertex_t * ev1 = myhedge1(1)->subvertex (0) ;
  myvertex_t * ev2 = myhedge1(2)->subvertex (0) ;
  assert(ev0 && ev1 && ev2 ) ;
  inneredge_t * e0 = new inneredge_t (l, ev0, ev1) ;
  inneredge_t * e1 = new inneredge_t (l, ev1, ev2) ;
  inneredge_t * e2 = new inneredge_t (l, ev2, ev0) ;
  assert( e0 && e1 && e2 ) ;
  e0->append(e1) ;
  e1->append(e2) ;
  innerface_t * f0 = new innerface_t (l, subedge1(0,0), twist(0), e2, 1, subedge1(2,1), twist(2)) ;
  innerface_t * f1 = new innerface_t (l, subedge1(0,1), twist(0), subedge1(1,0), twist(1), e0, 1) ;
  innerface_t * f2 = new innerface_t (l, e1, 1, subedge1(1,1), twist(1), subedge1(2,0), twist(2)) ;
  innerface_t * f3 = new innerface_t (l, e0, 0, e1, 0, e2, 0) ;
  assert (f0 && f1 && f2 && f3) ;
  f0->append(f1) ;
  f1->append(f2) ;
  f2->append(f3) ;
  _ed  = e0 ;
  _dwn = f0 ;
  _rule = myrule_t :: iso4 ;
  return ;
}

template < class A > inline Hface3Top < A > :: Hface3Top (int l, myhedge1_t * e0, 
	int t0, myhedge1_t * e1, int t1, myhedge1_t * e2, int t2) : 
	A (e0, t0, e1, t1, e2, t2), 
	_dwn (0), _bbb (0), /*_cv (0), */_ed (0), _lvl (l), _rule (myrule_t :: nosplit) {
  return ;
}

template < class A > inline Hface3Top < A > :: ~Hface3Top () {
  if (_bbb) delete _bbb ;
  if (_dwn) delete _dwn ;
  if (_ed) delete _ed ;
//  if (_cv) delete _cv ;
  return ;
}

template < class A > void Hface3Top < A > :: refineImmediate (myrule_t r) {
// cerr << "Hface3Top < A > :: refineImmediate (" << r << ")" << this << endl ;
  if (r != getrule ()) {
    assert (getrule () == myrule_t :: nosplit) ;
    switch(r) {
      case myrule_t :: e01 :
        myhedge1 (0)->refineImmediate (myhedge1_t :: myrule_t (myhedge1_t :: myrule_t :: iso2).rotate (twist (0))) ;
	split_e01 () ;
	break ;
      case myrule_t :: e12 :
        myhedge1 (1)->refineImmediate (myhedge1_t :: myrule_t (myhedge1_t :: myrule_t :: iso2).rotate (twist (0))) ;
	split_e12 () ;
	break ;
      case myrule_t :: e20 :
        myhedge1 (2)->refineImmediate (myhedge1_t :: myrule_t (myhedge1_t :: myrule_t :: iso2).rotate (twist (0))) ;
	split_e20 () ;
	break ;
      case myrule_t :: iso4 :
        myhedge1 (0)->refineImmediate (myhedge1_t :: myrule_t (myhedge1_t :: myrule_t :: iso2).rotate (twist (0))) ;
        myhedge1 (1)->refineImmediate (myhedge1_t :: myrule_t (myhedge1_t :: myrule_t :: iso2).rotate (twist (1))) ;
        myhedge1 (2)->refineImmediate (myhedge1_t :: myrule_t (myhedge1_t :: myrule_t :: iso2).rotate (twist (2))) ;
        split_iso4 () ;
        break ;
      default :
        cerr << "**FEHLER (FATAL) falsche Verfeinerungsregel [" << r << "] in " << __FILE__ << " " << __LINE__ << endl ;
        abort () ;
        break ;
    }
    postRefinement () ;
  }
  return ;
}

template < class A > bool Hface3Top < A > :: refine (myrule_t r, int twist) {
  if (r != getrule ()) {
    assert (getrule () == myrule_t :: nosplit ? 1 : 
    	(cerr << "**FEHLER beim Verfeinern mit Regel " << r << " auf " << getrule () << endl, 0)) ;
    switch(r) {
      case myrule_t :: e01 :
      case myrule_t :: e12 :
      case myrule_t :: e20 :
      case myrule_t :: iso4 :
      {
	bool a = twist < 0 ? nb.front ().first->refineBalance (r,nb.front ().second)
                           : nb.rear ().first->refineBalance (r,nb.rear ().second) ;
	if (a) {	
	  if (getrule () == myrule_t :: nosplit) {
	    refineImmediate (r) ;
	    {for (innerface_t * f = down () ; f ; f = f->next ()) f->nb = nb ; }
	  } else {
	  
	// Als Test absichern, da"s die Verfeinerung durchgekommen ist. Im
	// anisotropen Fall darf das so nicht mehr gemacht werden.
	  
	    assert (getrule () == r) ;
	  }
	  return true ;
	} else {
	  return false ;
	}
      }
      default :
        cerr << "**WARNUNG (IGNORIERT) falsche Verfeinerungsregel gefunden: " ;
        cerr << "[" << r << "] in " << __FILE__ << " " << __LINE__ << endl ;
        return false ;
    }
  }
  return true ;
}

template < class A > int Hface3Top < A > :: coarse () {
  innerface_t * f = down () ;
  if (!f) return false ;
  bool x = true ;
  do {

	// Falls eine Kind-Fl"ache noch referenziert wird, kann
	// nicht auf diesem Level vergr"obert werden.
	// Daher wird nur die nichtkonforme Nachbarschaft ver-
	// vollst"andigt, die eventuell durch Elementvergr"oberung
	// durcheinander gekommen war. Die Vergr"oberung geht dann
	// auf das n"achste Level "uber.
  
    if (f->ref) {
      if (f->ref == 1) f->nb.complete (nb) ;
      f->coarse () ;
      x = false ;
    }
  } while (f = f->next()) ;
  if (x) {
    
	// Hier wird tats"achlich vergr"obert, d.h. alle Kinder 
	// werden beseitigt, und das Bezugsobjekt wird zum neuen
	// Blatt im Baum.
    
    delete _dwn ; 
    _dwn = 0 ;
    delete _ed ;
    _ed = 0 ;
//    delete _cv ;
//    _cv = 0 ;
    _rule = myrule_t :: nosplit ;
    {for (int i = 0 ; i < 3 ; i ++ ) myhedge1 (i)->coarse () ; }
  }
  return x ;
}

template < class A > void Hface3Top < A > :: backup (ostream & os) const {
  os.put ((char) getrule ()) ;
  {for (const inneredge_t * e = innerHedge () ; e ; e = e->next ()) e->backup (os) ; }
  {for (const innerface_t * c = down () ; c ; c = c->next ()) c->backup (os) ; }
  return ;
}

template < class A > void Hface3Top < A > :: restore (istream & is) {
  refineImmediate (myrule_t ((char) is.get ())) ;
  {for (inneredge_t * e = innerHedge () ; e ; e = e->next ()) e->restore (is) ; }
  {for (innerface_t * c = down () ; c ; c = c->next ()) c->restore (is) ; }
  return ;
}

// #     #                          #####  #######
// #     #  #####   #    #  #####  #     #    #      ####   #####
// #     #  #    #  ##   #  #    #       #    #     #    #  #    #
// #######  #####   # #  #  #    #  #####     #     #    #  #    #
// #     #  #    #  #  # #  #    #       #    #     #    #  #####
// #     #  #    #  #   ##  #    # #     #    #     #    #  #
// #     #  #####   #    #  #####   #####     #      ####   #

template < class A > inline Hbnd3Top < A > :: Hbnd3Top (int l, myhface3_t * f, int i) 
	: A (f, i), _bbb (0), _dwn (0), _lvl (l) {
  return ;
}

template < class A > inline Hbnd3Top < A > :: ~Hbnd3Top () {
  if (_bbb) delete _bbb ; 
  if (_dwn) delete _dwn ;
  return ; 
}

template < class A > inline int Hbnd3Top < A > :: level () const {
  return _lvl ;
}

template < class A > Hbnd3Top < A > :: innerbndseg_t * Hbnd3Top < A > :: next () { 
  return _bbb ;
}

template < class A > const Hbnd3Top < A > :: innerbndseg_t * Hbnd3Top < A > :: next () const { 
  return _bbb ;
}

template < class A > Hbnd3Top < A > :: innerbndseg_t * Hbnd3Top < A > :: down () { 
  return _dwn ;
}

template < class A > const Hbnd3Top < A > :: innerbndseg_t * Hbnd3Top < A > :: down () const { 
  return _dwn ;
}

template < class A > inline void Hbnd3Top < A > :: append (innerbndseg_t * b) {
  assert (_bbb == 0) ;
  _bbb = b ;
  return ;
}

template < class A > void Hbnd3Top < A > :: split_e01 () {
  int l = 1 + level () ;
  innerbndseg_t * b0 = new innerbndseg_t (l, subface3 (0,0), twist (0)) ;
  innerbndseg_t * b1 = new innerbndseg_t (l, subface3 (0,1), twist (0)) ;
  assert (b0 && b1) ;
  b0->append(b1) ;
  _dwn = b0 ;
  return ;
}

template < class A > void Hbnd3Top < A > :: split_e12 () {
  int l = 1 + level () ;
  innerbndseg_t * b0 = new innerbndseg_t (l, subface3 (0,0), twist (0)) ;
  innerbndseg_t * b1 = new innerbndseg_t (l, subface3 (0,1), twist (0)) ;
  assert (b0 && b1) ;
  b0->append(b1) ;
  _dwn = b0 ;
  return ;
}

template < class A > void Hbnd3Top < A > :: split_e20 () {
  int l = 1 + level () ;
  innerbndseg_t * b0 = new innerbndseg_t (l, subface3 (0,0), twist (0)) ;
  innerbndseg_t * b1 = new innerbndseg_t (l, subface3 (0,1), twist (0)) ;
  assert (b0 && b1) ;
  b0->append(b1) ;
  _dwn = b0 ;
  return ;
}

template < class A > void Hbnd3Top < A > :: split_iso4 () {
  int l = 1 + level () ;
  innerbndseg_t * b0 = new innerbndseg_t (l, subface3 (0,0), twist (0)) ;
  innerbndseg_t * b1 = new innerbndseg_t (l, subface3 (0,1), twist (0)) ;
  innerbndseg_t * b2 = new innerbndseg_t (l, subface3 (0,2), twist (0)) ;
  innerbndseg_t * b3 = new innerbndseg_t (l, subface3 (0,3), twist (0)) ;
  assert (b0 && b1 && b2 && b3) ;
  b0->append(b1) ;
  b1->append(b2) ;
  b2->append(b3) ;
  _dwn = b0 ;
  return ;
}

template < class A > bool Hbnd3Top < A > :: coarse () {
  innerbndseg_t * b = down () ;
  if (!b) return false ;
  bool x = true ;
  do {
    if(b->myhface3(0)->ref > 1) (b->coarse (), x = false) ;
  } while (b = b->next()) ;
  if (x) {
    if (!lockedAgainstCoarsening ()) {
      preCoarsening () ;
      delete _dwn ;
      _dwn = 0 ;
      myhface3 (0)->coarse () ;
    }
  }
  return x ;
}

template < class A > inline bool Hbnd3Top < A > :: bndNotifyCoarsen () {
  return coarse () ;
}

template < class A > inline bool Hbnd3Top < A > :: refineBalance (balrule_t r, int b) {

	// Die Methode refineBalance () f"uhrt auf dem Randabschluss entweder
	// unbedingt die Verfeinerung durch, da im Verlauf der Verfeinerung keine
	// weiteren Anforerungen mehr an den Randabschluss  gerichtet werden 
	// ODER gibt die Verfeinerung als nicht erf"ullt zur"uck: Dann liegt
	// es am Aufrufer die Verfeinerung nochmals anzuforern.

  assert (b == 0) ;
  assert (leaf ()) ;
  if (!bndNotifyBalance (r,b)) {
  
  	// Hier kann der innere Rand [parallel] die Verfeinerung
	// verhindern, damit z.B. das Durchverfeinern im anisotropen
	// Fall erstmal nicht stattfindet, wenn nicht klar ist, wie die
	// weitere Rekursion aussieht. Dazu muss auf dem Niveau der Klasse
	// des Template-Arguments die Methode bndNotifyBalance () "uber-
	// schrieben werden. Die Defaultmethode liefert immer 'true'.
  
    return false ;
  } else {
    if(r == myrule_t :: iso4) {
    
    	// Der Rand verfeinert unbedingt die anliegende Fl"ache und dann
	// sich selbst, weil die Anforderung durch die Fl"ache kam, und
	// dahinter keine Balancierung stattfinden muss.
    
      myhface3 (0)->refineImmediate (r) ;
      split_iso4 () ;
    } else if (r == myrule_t :: e01) {
      myhface3 (0)->refineImmediate (r) ;
      split_e01 () ;
    } else if (r == myrule_t :: e12) {
      myhface3 (0)->refineImmediate (r) ;
      split_e12 () ;
    } else if (r == myrule_t :: e20) {
      myhface3 (0)->refineImmediate (r) ;
      split_e20 () ;
    } else {
      cerr << "**FEHLER (FATAL, weil nicht vorgesehen) beim Verfeinern am " ;
      cerr << "Randst\"uck mit der Regel [" << r << "] in " ;
      cerr << __FILE__ << " " << __LINE__ << endl ;
      abort () ;
    }
    
    	// postRefinement () gibt die M"oglichkeit auf dem Niveau des
	// Template-Arguments eine Methode aufzurufen, um eventuelle
	// Operationen auf dem verfeinerten Randst"uck durchzuf"uhren.
    
    postRefinement () ;
    return true ;
  }
}

template < class A > inline bool Hbnd3Top < A > :: refineLikeElement (balrule_t r) {

	// Mit der Methode refineLikeElement () verh"alt sich ein Randabschluss
	// in der Verfeinerung wie ein Element: Es wird zuerst gepr"uft ob eine
	// Balancierung der Vererfeinerung durch die Fl"ache hindurch erfolgreich
	// ist und nur genau dann die Verfeinerung durchgef"uhrt mit R"uckgabewert
	// 'true'. Diese Methode bedient eigentlich nur die parallele Verfeinerung
	// kann aber auch auf jedem beliebigen Randelement im seriellen Fall auf-
	// gerufen werden ohne Schaden anzurichten: Eine 1-Level Verfeinerung am
	// Rand ist jedoch wirkungslos, da sie beim n"achsten Vergr"obern wieder
	// aufgel"ost ist. Erst die mehrfache Anwendung f"uhrt durch die 
	// Balancierung zu einer "Anderung am Elementgitter.

  if (r == myrule_t :: nosplit) {
    cerr << "**WARNUNG (IGNORIERT) beim Versuch mit nosplit zu Verfeinern" ;
    cerr << "  in " << __FILE__ << " " << __LINE__ << endl ;
    
    	// Eine Anforderung mit nosplit zu Verfeinern nur erf"ullt,
	// falls die zugeh"orige Fl"achenregel auch nosplit ist, sonst
	// wird die Anforderung als nicht erf"ullt zur"uckgegeben.
    
    return getrule () == balrule_t :: nosplit ? true : false ;
  } else {
    if (getrule () == r) {
    
    	// Alles schon wie es sein soll -> true.
    
      return true ;
    } else {
  
	// Der nachfolgende Test bezieht sich auf die Verfeinerungssituation
	// der Fl"ache, da getrule () auf myhface3 (0)->getrule () umgeleitet
	// ist.
	
      assert (getrule () == myrule_t :: nosplit) ;
      switch (r) {
      case balrule_t :: e01 :
        if (!myhface3 (0)->refine (balrule_t (balrule_t :: e01).rotate (twist (0)), twist (0))) return false ;
	split_e01 () ;
	return true ;
      case balrule_t :: e12 :
        if (!myhface3 (0)->refine (balrule_t (balrule_t :: e12).rotate (twist (0)), twist (0))) return false ;
	split_e12 () ;
	return true ;
      case balrule_t :: e20 :
        if (!myhface3 (0)->refine (balrule_t (balrule_t :: e20).rotate (twist (0)), twist (0))) return false ;
	split_e20 () ;
	return true ;
      case balrule_t :: iso4 :
        if (!myhface3 (0)->refine (balrule_t (balrule_t :: iso4).rotate (twist (0)), twist (0))) return false ;
        split_iso4 () ;
        return true ;
      default :
        cerr << "**WARNUNG (FEHLER IGNORIERT) falsche Verfeinerungsregel [" << getrule () ;
        cerr << "] (ignoriert) in " << __FILE__ << " " << __LINE__ << endl ;
        return false ;
      }
    }
  }
}

template < class A > void Hbnd3Top < A > :: restoreFollowFace () {

	// retoreFollowFace () veranlasst das Randelement sich am 
	// bestehenden Fl"achenbaum wiederherzustellen durch die
	// entsprechende Verfeinerung.
	
  myhface3_t & f (*myhface3 (0)) ;
  if (!f.leaf ()) {
    balrule_t r = f.getrule () ;
    switch (r) {
      case myrule_t :: e01 :
        split_e01 () ;
	break ;
      case myrule_t :: e12 :
        split_e12 () ;
	break ;
      case myrule_t :: e20 :
        split_e20 () ;
	break ;
      case myrule_t :: iso4 :
        split_iso4 () ;
	break ;
      default :
        cerr << "**FEHLER (FATAL) beim Verfeinern am Randst\"uck mit der Regel [" << r << "] in " << __FILE__ << " " << __LINE__ << endl ;
        abort () ;
	break ;
    }
    postRefinement () ;
    {for (innerbndseg_t * b = down () ; b ; b = b->next ()) b->restoreFollowFace () ; }
  }
  return ;
}

// #######                                 #######
//    #     ######   #####  #####     ##      #      ####   #####
//    #     #          #    #    #   #  #     #     #    #  #    #
//    #     #####      #    #    #  #    #    #     #    #  #    #
//    #     #          #    #####   ######    #     #    #  #####
//    #     #          #    #   #   #    #    #     #    #  #
//    #     ######     #    #    #  #    #    #      ####   #

template < class A > inline TetraTop < A > :: TetraTop (int l, myhface3_t * f0, int t0,
	myhface3_t * f1, int t1, myhface3_t * f2, int t2, myhface3_t * f3, int t3) 
	: A (f0, t0, f1, t1, f2, t2, f3, t3), _dwn (0), _bbb (0), _ed (0), _fc (0), _lvl (l), 
	_rule (myrule_t :: nosplit) {
  return ;
}

template < class A > inline TetraTop < A > :: ~TetraTop () {
  if (_bbb) delete _bbb ;
  if (_dwn) delete _dwn ;
  if (_fc) delete _fc ;
  if (_ed) delete _ed ;
  return ;
}

template < class A > inline int TetraTop < A > :: level () const {
  return _lvl ;
}

template < class A > inline TetraTop < A > :: innertetra_t * TetraTop < A > :: down () {
  return _dwn ;
}

template < class A > inline const TetraTop < A > :: innertetra_t * TetraTop < A > :: down () const {
  return _dwn ;
}

template < class A > inline TetraTop < A > :: innertetra_t * TetraTop < A > :: next () {
  return _bbb ;
}

template < class A > inline const TetraTop < A > :: innertetra_t * TetraTop < A > :: next () const {
  return _bbb ;
}

template < class A > inline TetraTop < A > :: innervertex_t * TetraTop < A > :: innerVertex () {
  return 0 ; //	_cv 
}

template < class A > inline const TetraTop < A > :: innervertex_t * TetraTop < A > :: innerVertex () const {
  return 0 ;
}

template < class A > inline TetraTop < A > :: inneredge_t * TetraTop < A > :: innerHedge () {
  return _ed ;
}

template < class A > inline const TetraTop < A > :: inneredge_t * TetraTop < A > :: innerHedge () const {
  return _ed ;
}

template < class A > inline TetraTop < A > :: innerface_t * TetraTop < A > :: innerHface () {
  return _fc ;
}

template < class A > inline const TetraTop < A > :: innerface_t * TetraTop < A > :: innerHface () const {
  return _fc ;
}

template < class A > inline void TetraTop < A > :: append (TetraTop < A > * h) {
  assert (_bbb == 0) ;
  _bbb = h ;
  return ;
}

template < class A > TetraTop < A > :: myhedge1_t * TetraTop < A > :: subedge1 (int i, int j) {
  switch (myhface3(i)->getrule()) {
  case myhface3_t :: myrule_t :: e01 :
    assert( j == 0 );
    return myhface3 (i)->subedge1 (0) ;
  case myhface3_t :: myrule_t :: e12 :
    assert( j == 0 );
    return myhface3 (i)->subedge1 (0) ;
  case myhface3_t :: myrule_t :: e20 :
    assert( j == 0 );
    return myhface3 (i)->subedge1 (0) ;
  case myhface3_t :: myrule_t :: iso4 :
    assert( j < 3 );
    return ((twist (i) < 0) ? myhface3 (i)->subedge1 ((8 - j + twist (i)) % 3) : myhface3 (i)->subedge1 ((j + twist (i)) % 3)) ;
  case myhface3_t :: myrule_t :: nosplit :
    cerr << "**FEHLER (FATAL): subedge1 () auf nicht in verfeinerter Fl\"ache aufgerufen. In " << __FILE__ << " " << __LINE__ << endl ;
// cerr << this << " " << myhface3 (0) << " " << myhface3 (1) << " " << myhface3 (2) << endl ;
    abort () ;
    return 0 ;
  }
  return 0 ;
}

template < class A > const TetraTop < A > :: myhedge1_t * TetraTop < A > :: subedge1 (int i, int j) const {
  return ((TetraTop < A > *)this)->subedge1 (i,j) ;
}


template < class A > TetraTop < A > ::  myhface3_t * TetraTop < A > :: subface3 (int i, int j) {
  switch (myhface3(i)->getrule()) {
  case myhface3_t :: myrule_t :: e01 :
    assert( j < 2 );
    if ( twist(i) == 0 ||  twist(i) == 1 ||  twist(i) == -1 )
      return myhface3(i)->subface3(j) ;
    if ( twist(i) == 2 ||  twist(i) == -2 ||  twist(i) == -3 )
      return myhface3(i)->subface3(!j) ;
      cerr << __FILE__ << " " << __LINE__ << "myhface3(i)->subface3()" << endl;
      return 0;
  case myhface3_t :: myrule_t :: e12 :
    assert( j < 2 );
    if ( twist(i) == 0 ||  twist(i) == 2 ||  twist(i) == -3 )
      return myhface3(i)->subface3(j) ;
    if ( twist(i) == -1 ||  twist(i) == 1 ||  twist(i) == -2 )
      return myhface3(i)->subface3(!j) ;
    cerr << __FILE__ << " " << __LINE__ << "myhface3(i)->subface3()" << endl;
    return 0;
  case myhface3_t :: myrule_t :: e20 :
    assert( j < 2 );
    if ( twist(i) == 1 ||  twist(i) == 2 ||  twist(i) == -2 )
      return myhface3(i)->subface3(j) ;
    if ( twist(i) == 0 ||  twist(i) == -1 ||  twist(i) == -3 )
      return myhface3(i)->subface3(!j) ;
    cerr << __FILE__ << " " << __LINE__ << "myhface3(i)->subface3()" << endl;
    return 0;
  case myhface3_t :: myrule_t :: iso4 :
    assert( j < 4 );
    if ( j == 3 )
      return myhface3(i)->subface3(3);
    if ( j < 3 )
      return myhface3(i)->subface3(twist(i) < 0 ? (7 - j + twist(i)) % 3 : (j + twist(i)) % 3) ;
  case myhface3_t :: myrule_t :: nosplit :
    cerr << "**FEHLER (FATAL): subface3 () auf nicht verfeinerter Fl\"ache aufgerufen. In " << __FILE__ << " " << __LINE__ << endl ;
    abort () ;
    return 0 ;
  default:
    cerr << "**FEHLER (FATAL): Falsche Verfeinerungsregel [" << myhface3(i)->getrule() << "] in "__FILE__ << " " << __LINE__ << endl ;
    abort() ;
  } 
  return 0 ;
}

template < class A > const TetraTop < A > ::  myhface3_t * TetraTop < A > :: subface3 (int i, int j) const {
  return ((TetraTop < A > *)this)->subface3 (i,j) ;
}

template < class A > void TetraTop < A > :: split_e01 () {
  int l = 1 + level () ;
  innerface_t * f0 = new innerface_t (l, subedge1 (3, 3), 1, subedge1 (0, 3), 0, subedge1 (2, 2), 0) ;
  assert(f0 ) ;
  innertetra_t * h0 = new innertetra_t (l, subface3(0, 0), twist (0), f0, 0, myhface3(2), twist (2), subface3(3, 0), twist (3)) ;
  innertetra_t * h1 = new innertetra_t (l, subface3(0, 1), twist (0), myhface3(1), twist (1), f0, 1, subface3(3, 1), twist (3)) ;
  assert(h0 && h1 ) ;
  h0->append(h1) ;
  _fc = f0 ;
  _dwn = h0 ;
  return ;
}

template < class A > void TetraTop < A > :: split_e12 () {
  int l = 1 + level () ;
  innerface_t * f0 = new innerface_t (l, subedge1 (3, 3), 1, subedge1 (0, 3), 0, subedge1 (2, 2), 0) ;
  assert(f0 ) ;
  innertetra_t * h0 = new innertetra_t (l, subface3(0, 0), twist (0), f0, 0, myhface3(2), twist (2), subface3(3, 0), twist (3)) ;
  innertetra_t * h1 = new innertetra_t (l, subface3(0, 1), twist (0), myhface3(1), twist (1), f0, 1, subface3(3, 1), twist (3)) ;
  assert(h0 && h1 ) ;
  h0->append(h1) ;
  _fc = f0 ;
  _dwn = h0 ;
  _rule = myrule_t :: e12 ;
  return ;
}

template < class A > void TetraTop < A > :: split_e20 () {
  int l = 1 + level () ;
  innerface_t * f0 = new innerface_t (l, subedge1 (3, 3), 1, subedge1 (0, 3), 0, subedge1 (2, 2), 0) ;
  assert(f0 ) ;
  innertetra_t * h0 = new innertetra_t (l, subface3(0, 0), twist (0), f0, 0, myhface3(2), twist (2), subface3(3, 0), twist (3)) ;
  innertetra_t * h1 = new innertetra_t (l, subface3(0, 1), twist (0), myhface3(1), twist (1), f0, 1, subface3(3, 1), twist (3)) ;
  assert(h0 && h1 ) ;
  h0->append(h1) ;
  _fc = f0 ;
  _dwn = h0 ;
  _rule = myrule_t :: e20 ;
  return ;
}

template < class A > void TetraTop < A > :: split_e23 () {
  int l = 1 + level () ;
  innerface_t * f0 = new innerface_t (l, subedge1 (3, 3), 1, subedge1 (0, 3), 0, subedge1 (2, 2), 0) ;
  assert(f0 ) ;
  innertetra_t * h0 = new innertetra_t (l, subface3(0, 0), twist (0), f0, 0, myhface3(2), twist (2), subface3(3, 0), twist (3)) ;
  innertetra_t * h1 = new innertetra_t (l, subface3(0, 1), twist (0), myhface3(1), twist (1), f0, 1, subface3(3, 1), twist (3)) ;
  assert(h0 && h1 ) ;
  h0->append(h1) ;
  _fc = f0 ;
  _dwn = h0 ;
  _rule = myrule_t :: e23 ;
  return ;
}

template < class A > void TetraTop < A > :: split_e30 () {
  int l = 1 + level () ;
  innerface_t * f0 = new innerface_t (l, subedge1 (3, 3), 1, subedge1 (0, 3), 0, subedge1 (2, 2), 0) ;
  assert(f0 ) ;
  innertetra_t * h0 = new innertetra_t (l, subface3(0, 0), twist (0), f0, 0, myhface3(2), twist (2), subface3(3, 0), twist (3)) ;
  innertetra_t * h1 = new innertetra_t (l, subface3(0, 1), twist (0), myhface3(1), twist (1), f0, 1, subface3(3, 1), twist (3)) ;
  assert(h0 && h1 ) ;
  h0->append(h1) ;
  _fc = f0 ;
  _dwn = h0 ;
  _rule = myrule_t :: e30 ;
  return ;
}

template < class A > void TetraTop < A > :: split_e31 () {
  int l = 1 + level () ;
  innerface_t * f0 = new innerface_t (l, subedge1 (3, 3), 1, subedge1 (0, 3), 0, subedge1 (2, 2), 0) ;
  assert(f0 ) ;
  innertetra_t * h0 = new innertetra_t (l, subface3(0, 0), twist (0), f0, 0, myhface3(2), twist (2), subface3(3, 0), twist (3)) ;
  innertetra_t * h1 = new innertetra_t (l, subface3(0, 1), twist (0), myhface3(1), twist (1), f0, 1, subface3(3, 1), twist (3)) ;
  assert(h0 && h1 ) ;
  h0->append(h1) ;
  _fc = f0 ;
  _dwn = h0 ;
  _rule = myrule_t :: e31 ;
  return ;
}

template < class A > void TetraTop < A > :: split_iso8 () {
  typedef typename A :: myvertex_t  myvertex_t;
  typedef typename A :: inneredge_t inneredge_t;
  int l = 1 + level () ; 
  myvertex_t * e31 = myhface3 (0)->myhedge1 ((twist(0) < 0) ? ((9+twist(0))%3) : (twist(0)%3))->subvertex (0) ;
  myvertex_t * e20 = myhface3 (1)->myhedge1 ((twist(1) < 0) ? ((9+twist(1))%3) : (twist(1)%3))->subvertex (0) ;
  assert(e31 && e20);
  inneredge_t * e0 = new inneredge_t (l, e31, e20) ;
  assert(e0) ;
  innerface_t * f0 = new innerface_t (l, subedge1 (3, 2), ((twist(3)>=0)?1:0), subedge1 (1, 2), ((twist(1)>=0)?1:0), subedge1 (2, 2), ((twist(2)>=0)?1:0) ) ;
  innerface_t * f1 = new innerface_t (l, subedge1 (3, 0), ((twist(3)>=0)?1:0), subedge1 (2, 1), ((twist(2)>=0)?1:0), subedge1 (0, 2), ((twist(0)>=0)?1:0) ) ;
  innerface_t * f2 = new innerface_t (l, subedge1 (3, 1), ((twist(3)>=0)?1:0), subedge1 (0, 1), ((twist(0)>=0)?1:0), subedge1 (1, 0), ((twist(1)>=0)?1:0) ) ;
  innerface_t * f3 = new innerface_t (l, subedge1 (2, 0), ((twist(2)>=0)?0:1), subedge1 (0, 0), ((twist(0)>=0)?0:1), subedge1 (1, 1), ((twist(1)>=0)?0:1) ) ;
  innerface_t * f4 = new innerface_t (l, e0, 0, subedge1 (3, 2), ((twist(3)>=0)?0:1), subedge1 (2, 1), ((twist(2)>=0)?1:0)) ;
  innerface_t * f5 = new innerface_t (l, e0, 0, subedge1 (3, 1), ((twist(3)>=0)?1:0), subedge1 (0, 2), ((twist(0)>=0)?0:1)) ; 
  innerface_t * f6 = new innerface_t (l, e0, 0, subedge1 (1, 0), ((twist(1)>=0)?0:1), subedge1 (0, 0), ((twist(0)>=0)?1:0)) ;
  innerface_t * f7 = new innerface_t (l, e0, 0, subedge1 (1, 2), ((twist(1)>=0)?1:0), subedge1 (2, 0), ((twist(2)>=0)?0:1)) ;
  assert(f0 && f1 && f2 && f3 && f4 && f5 && f6 && f7) ;
  f0->append(f1) ;
  f1->append(f2) ;
  f2->append(f3) ;
  f3->append(f4) ;
  f4->append(f5) ;
  f5->append(f6) ;
  f6->append(f7) ;
  innertetra_t * h0 = new innertetra_t (l, f0, -1, subface3(1, 0), twist(1), subface3(2, 0), twist(2), subface3(3, 0), twist(3)) ;
  innertetra_t * h1 = new innertetra_t (l, subface3(0, 0), twist(0), f1, -3, subface3(2, 2), twist(2), subface3(3, 1), twist(3)) ;
  innertetra_t * h2 = new innertetra_t (l, subface3(0, 2), twist(0), subface3(1, 1), twist(1), f2, -1, subface3(3, 2), twist(3)) ;
  innertetra_t * h3 = new innertetra_t (l, subface3(0, 1), twist(0), subface3(1, 2), twist(1), subface3(2, 1), twist(2), f3, 0) ;
  innertetra_t * h4 = new innertetra_t (l, f7, -3, subface3(2, 3), ((twist(2)>=0) ? ((twist(2)+2)%3) : twist(2)) , f4, 2, f0, 0) ; 	
  innertetra_t * h5 = new innertetra_t (l, f4, -3, f1, 0, f5, 2, subface3(3, 3), ((twist(3)>=0) ? (twist(3)+1)%3 : (twist(3)-1)%3-1)) ;
  innertetra_t * h6 = new innertetra_t (l, f3, -1, f6, -3, subface3(1, 3), ((twist(1)>=0) ? twist(1) : twist(1)%3-1), f7, 1) ;
  innertetra_t * h7 = new innertetra_t (l, subface3(0, 3), ((twist(0)>=0) ? (twist(0)+1)%3 : (twist(0)-1)%3-1), f5, -3, f2, 0, f6, 1) ;
  assert(h0 && h1 && h2 && h3 && h4 && h5 && h6 && h7) ;
  h0->append(h1) ;
  h1->append(h2) ;
  h2->append(h3) ;
  h3->append(h4) ;
  h4->append(h5) ;
  h5->append(h6) ;
  h6->append(h7) ;
  _ed = e0 ;
  _fc = f0 ;
  _dwn = h0 ;
  _rule = myrule_t :: iso8 ;
  return ;
}

template < class A > TetraTop < A > :: myrule_t TetraTop < A > :: getrule () const {
  return myrule_t (_rule) ;
}

template < class A > void TetraTop < A > :: request (myrule_t r) {
  assert (r.isValid ()) ;
  _req = r ;
  return ;
}

template < class A > void TetraTop < A > :: refineImmediate (myrule_t r) {
  assert (getrule () == myrule_t :: nosplit) ;
  switch(r) {
    case myrule_t :: e01 :
      myhface3 (2)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (2))) ;
      myhface3 (3)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (3))) ;
    case myrule_t :: e12 :
      myhface3 (0)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (0))) ;
      myhface3 (3)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (3))) ;
    case myrule_t :: e20 :
      myhface3 (1)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (1))) ;
      myhface3 (3)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (3))) ;
    case myrule_t :: e23 :
      myhface3 (0)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (0))) ;
      myhface3 (1)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (1))) ;
    case myrule_t :: e30 :
      myhface3 (1)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (1))) ;
      myhface3 (2)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (2))) ;
    case myrule_t :: e31 :
      myhface3 (0)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (0))) ;
      myhface3 (2)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (2))) ;
    case myrule_t :: iso8 :
    
	// Das refineImmediate (..) auf allen Fl"achen wird vom tetra :: refine (..)
	// zwar nicht ben"otigt, da schliesslich alle Fl"achen sauber sind wenn
	// "uberall hface3 :: refine (..) true geliefert hat, wohl aber z.B. von
	// restore () oder abgeleiteten Funktionen die eine direkte Verfeinerung
	// erzwingen m"ussen und d"urfen.
    
      {for (int i = 0 ; i < 4 ; i ++)
        myhface3 (i)->refineImmediate (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: iso4).rotate (twist (i))) ; }
      split_iso8 () ;
      break ;
    default :
      cerr << "**FEHLER (FATAL) beim unbedingten Verfeinern mit unbekannter Regel: " ;
      cerr << "[" << r << "]. In " << __FILE__ << __LINE__ << endl ;
      abort () ;
      break ;
  }
  postRefinement () ;
  return ;
}

template < class A > bool TetraTop < A > :: refine () {
  myrule_t r = _req ;
  if (r != myrule_t :: crs && r != myrule_t :: nosplit) {
    if (r != getrule ()) {
      assert (getrule () == myrule_t :: nosplit) ;
      _req = myrule_t :: nosplit ;
      switch (r) {
        case myrule_t :: crs :
        case myrule_t :: nosplit :
          return true ;
        case myrule_t :: e01 :
          if (!myhface3 (2)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (2)), twist (2))) return false ;
          if (!myhface3 (3)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (3)), twist (3))) return false ;
          break ;
	case myrule_t :: e12 :
          if (!myhface3 (0)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (0)), twist (0))) return false ;
          if (!myhface3 (3)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (3)), twist (3))) return false ;
          break ;
	case myrule_t :: e20 :
          if (!myhface3 (1)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (1)), twist (1))) return false ;
          if (!myhface3 (3)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (3)), twist (3))) return false ;
          break ;
	case myrule_t :: e23 :
          if (!myhface3 (0)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (0)), twist (0))) return false ;
          if (!myhface3 (1)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (1)), twist (1))) return false ;
          break ;
	case myrule_t :: e30 :
          if (!myhface3 (1)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (1)), twist (1))) return false ;
          if (!myhface3 (2)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (2)), twist (2))) return false ;
          break ;
	case myrule_t :: e31 :
          if (!myhface3 (0)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (0)), twist (0))) return false ;
          if (!myhface3 (2)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: e01).rotate (twist (2)), twist (2))) return false ;		  
          break ;
	case myrule_t :: iso8 :
	  {for (int i = 0 ; i < 4 ; i ++ )
            if (!myhface3 (i)->refine (myhface3_t :: myrule_t (myhface3_t :: myrule_t :: iso4).rotate (twist (i)), twist (i))) return false ; }
          break ;
        default :
          cerr << "**WARNUNG (FEHLER IGNORIERT) falsche Verfeinerungsregel [" << getrule () ;
          cerr << "] (ignoriert) in " << __FILE__ << " " << __LINE__ << endl ;
          return false ;
      }
      
	// Vorsicht: Im Fall eines konformen Verfeinerers mu"s hier die entstandene Verfeinerung
	// untersucht werden und dann erst das Element danach verfeinert werden.
      
      refineImmediate (r) ;
      return true ;
    }
  }
  return true ;
}

template < class A > bool TetraTop < A > :: refineBalance (balrule_t r, int fce) {
  if (r != balrule_t :: iso4) {
    cerr << "**WARNUNG (IGNORIERT) in TetraTop < A > :: refineBalance (..) nachschauen, Datei " 
    	 << __FILE__ << " Zeile " << __LINE__ << endl ;
	 
	// Bisher kann die Balancierung nur die isotrope Achtelung handhaben,
	// falls mehr gew"unscht wird muss es hier eingebaut werden. Im Moment wird
	// die Balancierung einfach verweigert, d.h. die Verfeinerung des anfordernden
	// Elements f"allt flach.
    return false ;
  }
  if (getrule () == myrule_t :: nosplit) {
    if (! myhface3 (fce)->leaf ()) {
      for (int i = 0 ; i < 4 ; i ++)
        if (i != fce)
          if (!myhface3 (i)->refine (balrule_t (balrule_t :: iso4).rotate (twist (i)), twist (i))) 
	    return false ;
      _req = myrule_t :: nosplit ;
      refineImmediate (myrule_t :: iso8) ;
    }
  }
  return true ;
}

template < class A > bool TetraTop < A > :: coarse () {
  if (leaf ()) {
    assert (_req == myrule_t :: nosplit || _req == myrule_t :: crs) ;
    myrule_t w = _req ;
    _req = myrule_t :: nosplit ;
    if (w != myrule_t :: crs) return false ;
    for (int i = 0 ; i < 4 ; i ++) if (!myhface3 (i)->leaf ()) return false ;
    return true ;
  } else {
    assert (_req == myrule_t :: nosplit) ;
    bool x = true ;
    {for (innertetra_t * h = down () ; h ; h = h->next ()) x &= h->coarse () ; }
    if (x) {
      preCoarsening () ;
      delete _dwn ; 
      _dwn = 0 ;
      delete _fc ;
      _fc = 0 ;
      delete _ed ;
      _ed = 0 ;
//      delete _cv ; 
//      _cv = 0 ;
      _rule = myrule_t :: nosplit ;
      {
        for (int i = 0 ; i < 4 ; i ++ ) {
	  myneighbour (i).first->bndNotifyCoarsen () ;
          myhface3 (i)->coarse () ;
	}
      }
      return false ;
    }
  }
  return false ;
}

template < class A > bool TetraTop < A > :: bndNotifyCoarsen () {
  return true ;
}

template < class A > void TetraTop < A > :: backupCMode (ostream & os) const {

	// Das backup im alten Stil, d.h. levelweise die Verfeinerungsregeln
	// vom Gitter runterschreiben. Diese Technik wird nur f"ur das backup
	// noch unterst"utzt, um die Daten mit "alteren Konstruktionen visual.
	// zu k"onnen.

  os << getrule () << " " ;
  return ;
}

template < class A > void TetraTop < A > :: backup (ostream & os) const {
  os.put ((char) getrule ()) ;
  {for (const inneredge_t * e = innerHedge () ; e ; e = e->next ()) e->backup (os) ; }
  {for (const innerface_t * f = innerHface () ; f ; f = f->next ()) f->backup (os) ; }
  {for (const innertetra_t * c = down () ; c ; c = c->next ()) c->backup (os) ; }
  return ;
}

template < class A > void TetraTop < A > :: restore (istream & is) {

	// restore () stellt den Elementbaum aus der Verfeinerungs-
	// geschichte wieder her. Es ruft refine () auf und testet
	// auf den korrekten Vollzug der Verfeinerung. Danach werden
	// die inneren Gitterteile restore'd.

  myrule_t r ((char) is.get ()) ;
  assert(getrule() == myrule_t :: nosplit) ;
  if (r == myrule_t :: nosplit) {
  
	// Vorsicht: beim restore m"ussen sich sowohl Element als auch
	// Randelement um die Korrektheit der Nachbarschaft k"ummern,
	// und zwar dann wenn sie "on the top" sind (= die gelesene
	// Verfeinerungsregel ist nosplit). (s.a. beim Randelement)
	// Die nachfolgende L"osung ist weit davon entfernt, sch"on
	// zu sein - leider. Eventuell wird mit der Verbesserung der
	// Behandlung der nichtkonf. Situationen mal eine "Anderung
	// n"otig.
  
    for (int i = 0 ; i < 4 ; i ++) {
      myhface3_t & f (*myhface3 (i)) ;
      if (!f.leaf ()) {
        switch (f.getrule ()) {
	  case balrule_t :: e01 :
	  case balrule_t :: e12 :
	  case balrule_t :: e20 :
	    {for (int j = 0 ; j < 2 ; j ++) f.subface3 (j)->nb.complete (f.nb) ;}
	    break ;
	  case balrule_t :: iso4 :
            {for (int j = 0 ; j < 4 ; j ++) f.subface3 (j)->nb.complete (f.nb) ;}
	    break ;
	  default :
	    abort () ;
	    break ;
	}
      }
    }
  } else {

	// Auf dem Element gibt es kein refine (myrule_t) deshalb mu"s erst
	// request (myrule_t) und dann refine () durchgef"uhrt werden.
  
    request (r) ;
    refine () ;
    assert (getrule() == r) ;
    {for (inneredge_t * e = innerHedge () ; e ; e = e->next ()) e->restore (is) ; }
    {for (innerface_t * f = innerHface () ; f ; f = f->next ()) f->restore (is) ; }
    {for (innertetra_t * c = down () ; c ; c = c->next ()) c->restore (is) ; }
  }
  return ;
}

#endif	//	GITTER_TetraTop_H_INCLUDED
