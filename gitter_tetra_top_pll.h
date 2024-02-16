	// (c) bernhard schupp 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/gitter_tetra_top_pll.h,v $
	// $Revision: 2.2 $
	// $Name:  $
	// $State: Exp $
	// $Date$
	
#ifndef GITTER_TETRA_TOP_PLL_H_INCLUDED
#define GITTER_TETRA_TOP_PLL_H_INCLUDED

#include "parallel.h"
#include "gitter_tetra_top.h"

static volatile char RCSId_gitter_tetra_top_pll_h [] = "$Id: gitter_tetra_top_pll.h,v 2.2 1998/12/13 13:01:23 yc23 Exp yc23 $" ;

template < class A, class MX > class Hbnd3PllExternal : public Hbnd3Top < A > {
  public :
    typedef MX mypllx_t ;
  protected :
    typedef typename A :: myhface3_t myhface3_t ;
  public :
    inline Hbnd3PllExternal (myhface3_t *, int) ;
    inline ~Hbnd3PllExternal () ;
    ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
    const ElementPllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
    void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
  private :
    mypllx_t * _mxt ;
} ;

template < class A, class X, class MX > class Hbnd3PllInternal {
  public :
    class HbndPll : public A {
      public :
        typedef X mypllx_t ;
      protected :
        typedef typename A :: myhface3_t myhface3_t ;
	typedef typename A :: balrule_t balrule_t ;
	typedef typename A :: bnd_t     bnd_t ;
        inline HbndPll (myhface3_t *, int) ;
       ~HbndPll () {}
        virtual bool bndNotifyBalance (balrule_t,int) ;
	virtual bool lockedAgainstCoarsening () const ;
      public :
        bnd_t bndtype () const ;
        ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
        const ElementPllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
        void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
      private :
        mypllx_t _ext ;
    } ;
    typedef class HbndPll micro_t ;
  public :
    class HbndPllMacro : public Hbnd3Top < micro_t > {
      public :
        typedef MX mypllx_t ;
      protected :
        typedef typename A :: myhface3_t myhface3_t ;
	virtual bool bndNotifyBalance (balrule_t,int) ;
	virtual bool lockedAgainstCoarsening () const ;
      public :
        HbndPllMacro (myhface3_t *,int) ;
       ~HbndPllMacro () ;
        ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
        const ElementPllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
        void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
      private :
        mypllx_t * _mxt ;
    } ;
    typedef class HbndPllMacro macro_t ;
} ;


	//
	//    #    #    #  #          #    #    #  ######
	//    #    ##   #  #          #    ##   #  #
	//    #    # #  #  #          #    # #  #  #####
	//    #    #  # #  #          #    #  # #  #
	//    #    #   ##  #          #    #   ##  #
	//    #    #    #  ######     #    #    #  ######
	//


template < class A, class MX > inline Hbnd3PllExternal < A, MX > :: Hbnd3PllExternal (myhface3_t * f, int t) : Hbnd3Top < A > (0,f,t), _mxt (new MX (*this)) {
  restoreFollowFace () ;
  return ;
}

template < class A, class MX > inline Hbnd3PllExternal < A, MX > :: ~Hbnd3PllExternal () {
  delete _mxt ;
  _mxt = 0 ;
  return ;
}

template < class A, class MX > ElementPllXIF_t & Hbnd3PllExternal < A, MX > :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_mxt) ;
  return * _mxt ;
}

template < class A, class MX > const ElementPllXIF_t & Hbnd3PllExternal < A, MX > :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_mxt) ;
  return * _mxt ;
}

template < class A, class MX > void Hbnd3PllExternal < A, MX > :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _mxt ;
  _mxt = 0 ;
  return ;
}

template < class A, class X, class MX > inline Hbnd3PllInternal < A, X, MX > :: HbndPll :: HbndPll (myhface3_t * f, int t) : A (f,t), _ext (*this) {
  return ;
}

template < class A, class X, class MX > Hbnd3PllInternal < A, X, MX > :: HbndPll :: bnd_t Hbnd3PllInternal < A, X, MX > :: HbndPll ::  bndtype () const {
  return Gitter :: hbndseg_STI :: closure ;
}

template < class A, class X, class MX > ElementPllXIF_t & Hbnd3PllInternal < A, X, MX > :: HbndPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _ext ;
}

template < class A, class X, class MX > const ElementPllXIF_t & Hbnd3PllInternal < A, X, MX > :: HbndPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _ext ;
}

template < class A, class X, class MX > void Hbnd3PllInternal < A, X, MX > :: HbndPll :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  abort () ;
  return ;
}

template < class A, class X, class MX > bool Hbnd3PllInternal < A, X, MX > :: HbndPll :: bndNotifyBalance (balrule_t r, int w) {
  if (r == balrule_t :: iso4) {
    _ext.notifyBalance (r,w) ;
    return true ;
  } else {
    cerr << "**WARNUNG Balancierungsanforderung vom Typ " << r << " ignoriert,\n" ;
    cerr << "  weil nicht vorgesehen. In " << __FILE__ << " " << __LINE__ << endl ;
    return false ;
  }
}

template < class A, class X, class MX > bool Hbnd3PllInternal < A, X, MX > :: HbndPll :: lockedAgainstCoarsening () const {
  return _ext.lockedAgainstCoarsening () ;
}

template < class A, class X, class MX > Hbnd3PllInternal < A, X, MX > :: HbndPllMacro :: HbndPllMacro (myhface3_t * f, int t) : Hbnd3Top < micro_t > (0,f,t), _mxt (new MX (*this)) {
  restoreFollowFace () ;
  return ;
}

template < class A, class X, class MX > Hbnd3PllInternal < A, X, MX > :: HbndPllMacro :: ~HbndPllMacro () {
  delete _mxt ;
  _mxt = 0 ;
  return ;
}

template < class A, class X, class MX > ElementPllXIF_t & Hbnd3PllInternal < A, X, MX > :: HbndPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_mxt) ;
  return * _mxt ;
}

template < class A, class X, class MX > const ElementPllXIF_t & Hbnd3PllInternal < A, X, MX > :: HbndPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_mxt) ;
  return * _mxt ;
}

template < class A, class X, class MX > void Hbnd3PllInternal < A, X, MX > :: HbndPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _mxt ;
  _mxt = 0 ;
  return ;
}

template < class A, class X, class MX > bool Hbnd3PllInternal < A, X, MX > :: HbndPllMacro :: bndNotifyBalance (balrule_t r, int w) {
 if (r == balrule_t :: iso4) {
    assert (_mxt) ;
    _mxt->notifyBalance (r,w) ;
    return true ;
  } else {
    cerr << "**WARNUNG Balancierungsanforderung vom Typ " << r << " ignoriert,\n" ;
    cerr << "  weil nicht vorgesehen. In " << __FILE__ << " " << __LINE__ << endl ;
    return false ;
  }
}

template < class A, class X, class MX > bool Hbnd3PllInternal < A, X, MX > :: HbndPllMacro :: lockedAgainstCoarsening () const {
  assert (_mxt) ;
  return _mxt->lockedAgainstCoarsening () ;
}

#endif	// GITTER_TETRA_TOP_PLL_H_INCLUDED
