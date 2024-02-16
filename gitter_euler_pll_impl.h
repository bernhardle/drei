	// (c) bernhard schupp 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/gitter_euler_pll_impl.h,v $
	// $Revision: 2.2 $
	// $Name:  $
	// $State: Exp $
	// $Date$

#ifndef GITTER_EULER_PLL_IMPL_H_INCLUDED
#define GITTER_EULER_PLL_IMPL_H_INCLUDED

#include "gitter_euler_impl.h"
#include "gitter_pll_impl.h"

static volatile char RCSId_gitter_euler_pll_impl_h [] = "$Id: gitter_euler_pll_impl.h,v 2.2 1998/10/29 11:39:18 yc23 Exp yc23 $" ;

template < class A > class BndEulerIntPllX ;
template < class A > class BndEulerIntPllXMacro ;

class TetraEulerPllX : public TetraPllXBase {
  public :
    TetraEulerPllX (mytetra_t & t) : TetraPllXBase (t) {}
   ~TetraEulerPllX () {}
    virtual void writeDynamicState (ObjectStream &) const ;
} ;

class TetraEulerPllXMacro : public TetraPllXBaseMacro {
  public :
    TetraEulerPllXMacro (mytetra_t & t) : TetraPllXBaseMacro (t) {}
   ~TetraEulerPllXMacro () {}
    virtual void writeDynamicState (ObjectStream &) const ;
  protected :
    virtual void inlineData (ObjectStream &) throw (ObjectStream :: EOFException) ;
    virtual void xtractData (ObjectStream &) throw (ObjectStream :: EOFException) ;
} ;

class HexaEulerPllX : public HexaPllBaseX {
  public :
    HexaEulerPllX (myhexa_t & h) : HexaPllBaseX (h) {}
   ~HexaEulerPllX () {}
    virtual void writeDynamicState (ObjectStream &) const ;
} ;

class HexaEulerPllXMacro : public HexaPllBaseXMacro {
  public :
    HexaEulerPllXMacro (myhexa_t & h) : HexaPllBaseXMacro (h) {}
   ~HexaEulerPllXMacro () {}
    virtual void writeDynamicState (ObjectStream &) const ;
  protected :
    virtual void inlineData (ObjectStream &) throw (ObjectStream :: EOFException) ;
    virtual void xtractData (ObjectStream &) throw (ObjectStream :: EOFException) ;
} ;

class GitterEulerPll : public GitterBasisPll, public GitterEuler {

	// typedef auf den Typ des Interpolationsobjekts in den Randelementen
	// an den Schnittfl"achen zwischen den Teilgittern. Vorsicht die
	// Geschichte ist aber noch nicht ganz gar: In Zeile 61 und 67 in
	// gitter_euler_pll_impl.cc ist n"amlich noch nicht klar, wie das
	// Element erkennt, f"ur welche Fl"ache das Interpolationsobjekt
	// zu konstruieren ist (Gegenstand einer weiteren Verbesserung).
	
  public :
  
    typedef EulerSolver :: InterpolationCube2Dzero interpolationCube2DintIMPL_t ;
    typedef EulerSolver :: InterpolationCube2Dzero interpolationSimplex2DintIMPL_t ;

  public :
    class ObjectsEulerPll : public GitterEuler :: ObjectsEuler {
      public :
        typedef GitterBasisPll :: ObjectsPll :: hedge1_IMPL hedge1_IMPL ;
      public :
        class Hface3EulerPll : public Hface3Euler {
	  protected :
	    typedef hedge1_IMPL inneredge_t ;
	    inline Hface3EulerPll (myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int) ;
	  public :
	    typedef FacePllBaseX < hface3_GEO > mypllx_t ;
	    virtual FacePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const FacePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t _pllx ;
	  friend mypllx_t ;
	} ;
        typedef Hface3Top < Hface3EulerPll > hface3_IMPL ;
 
        class Hface3EulerPllMacro : public hface3_IMPL {
	  public :
	    typedef FacePllBaseXMacro < hface3_GEO > mypllx_t ;
	    Hface3EulerPllMacro (myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int) ;
	   ~Hface3EulerPllMacro () ;
	    virtual FacePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const FacePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
	} ;
      public :
        class Hface4EulerPll : public Hface4Euler {
	  protected :
	    typedef hedge1_IMPL inneredge_t ;
	    inline Hface4EulerPll (myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int) ;
	  public :
	    typedef FacePllBaseX < hface4_GEO > mypllx_t ;
	    virtual FacePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const FacePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t _pllx ;
	  friend mypllx_t ;
	} ;
        typedef Hface4Top < Hface4EulerPll > hface4_IMPL ;
 
        class Hface4EulerPllMacro : public hface4_IMPL {
	  public :
	    typedef FacePllBaseXMacro < hface4_GEO > mypllx_t ;
	    Hface4EulerPllMacro (myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int) ;
	   ~Hface4EulerPllMacro () ;
	    virtual FacePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const FacePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
	} ;
      public :
        class TetraEulerPll : public TetraEuler {
	  protected :
	    typedef hedge1_IMPL inneredge_t ;
	    typedef hface3_IMPL innerface_t ;
	  public :
	    typedef TetraEulerPllX mypllx_t ;
	    inline TetraEulerPll (myhface3_t *,int,myhface3_t *,int,myhface3_t *,int,myhface3_t *,int) ;
            inline ~TetraEulerPll () {}
	    virtual ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const ElementPllXIF_t &accessPllX () const throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t _pllx ;
	  friend mypllx_t ;
	} ;
	typedef TetraTop < TetraEulerPll > tetra_IMPL ;

        class TetraEulerPllMacro : public tetra_IMPL {
	  public :
	    typedef TetraEulerPllXMacro mypllx_t ;
	    TetraEulerPllMacro (myhface3_t *,int,myhface3_t *,int,myhface3_t *,int,myhface3_t *,int) ;
	   ~TetraEulerPllMacro () ;
	    virtual ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const ElementPllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
	} ;
      public :
        class HexaEulerPll : public HexaEuler {
	  protected :
	    typedef hedge1_IMPL inneredge_t ;
	    typedef hface4_IMPL innerface_t ;
	  public :
	    typedef HexaEulerPllX mypllx_t ;
	    inline HexaEulerPll (myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int) ;
            inline ~HexaEulerPll () {}
	    virtual ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const ElementPllXIF_t &accessPllX () const throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t _pllx ;
	  friend mypllx_t ;
	} ;
	typedef HexaTop < HexaEulerPll > hexa_IMPL ;

        class HexaEulerPllMacro : public hexa_IMPL {
	  public :
	    typedef HexaEulerPllXMacro mypllx_t ;
	    HexaEulerPllMacro (myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int) ;
	   ~HexaEulerPllMacro () ;
	    virtual ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const ElementPllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
	} ;
      public :
        class EulerPllInternalBnd3 : public EulerBnd3 {
          int _touched ;
	  virtual EulerPllInternalBnd3 * down () = 0 ;
          virtual EulerPllInternalBnd3 * next () = 0 ;
          virtual const EulerPllInternalBnd3 * down () const = 0 ;
          virtual const EulerPllInternalBnd3 * next () const = 0 ;
          public :
	    typedef interpolationSimplex2DintIMPL_t myinterpolation_t ;
            inline EulerPllInternalBnd3 (myhface3_t *, int) ;
            int postRefinement () ;
	    virtual inline bnd_t bndtype () const ;
          public :
            EulerSolver :: Vec5 euler3dCalcFlux (double, hface3 *, pair < hasFace3 *,int >,int) ;
            EulerSolver :: Vec5 euler3dCalcFlux (double, hface3 *, const EulerSolver :: InterpolationSimplex2D &, int) ;
	  private :
	    myinterpolation_t _ipObj ;
	  friend class BndEulerIntPllX < EulerPllInternalBnd3 > ;
	  friend class BndEulerIntPllXMacro < EulerPllInternalBnd3 > ;
        } ;
      public :
        class EulerPllInternalBnd4 : public EulerBnd4 {
          int _touched ;
	  virtual EulerPllInternalBnd4 * down () = 0 ;
          virtual EulerPllInternalBnd4 * next () = 0 ;
          virtual const EulerPllInternalBnd4 * down () const = 0 ;
          virtual const EulerPllInternalBnd4 * next () const = 0 ;
          public :
	    typedef interpolationCube2DintIMPL_t myinterpolation_t ;
            inline EulerPllInternalBnd4 (myhface4_t *, int) ;
            int postRefinement () ;
	    virtual inline bnd_t bndtype () const ;
          public :
            EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *, pair < hasFace4 *,int >,int) ;
            EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *, const EulerSolver :: InterpolationCube2D &, int) ;
	  private :
	    myinterpolation_t _ipObj ;
	  friend class BndEulerIntPllX < EulerPllInternalBnd4 > ;
	  friend class BndEulerIntPllXMacro < EulerPllInternalBnd4 > ;
        } ;
    } ;
  public :
    class MacroGitterEulerPll : public GitterBasisPll :: MacroGitterBasisPll , public GitterEuler :: MacrogitterEuler {
      protected :
        virtual VertexGeo * insert_vertex (double, double, double, int, int) ;
        virtual hedge1_GEO * insert_hedge1 (VertexGeo *, VertexGeo *) ;
	virtual hface3_GEO * insert_hface3 (hedge1_GEO *(&)[3], int (&)[3]) ;
        virtual hface4_GEO * insert_hface4 (hedge1_GEO *(&)[4], int (&)[4]) ;
	virtual tetra_GEO * insert_tetra (hface3_GEO *(&)[4], int (&)[4]) ;
        virtual hexa_GEO * insert_hexa (hface4_GEO *(&)[6], int (&)[6]) ;
	virtual hbndseg3_GEO * insert_hbnd3 (hface3_GEO *, int, Gitter :: hbndseg_STI :: bnd_t) ;
        virtual hbndseg4_GEO * insert_hbnd4 (hface4_GEO *, int, Gitter :: hbndseg_STI :: bnd_t) ;
      public :
        MacroGitterEulerPll (istream &) ;
        MacroGitterEulerPll () ;
    } ;
  private :
    MacroGitterEulerPll * _macrogitter ;
    inline Makrogitter & container () ;
    inline const Makrogitter & container () const ;
    inline MacroGitterPll & containerPll () ;
    inline const MacroGitterPll & containerPll () const ;
    double addUpdate (double) ;
  protected :
    bool estimateRefinement () ;
    void oneTimeStep (double, bool = true) ;
    void transferInterpolationObjects () ;
  public :
    virtual void backupCMode (ostream &) ;
    virtual void backupCMode (const char *) ;
    virtual void backup (ostream &) ;
    virtual void backup (const char *) ;
    virtual void restore (istream &) ;
    virtual void restore (const char *) ;
    GitterEulerPll (const char *, MpAccessLocal &) ;
    GitterEulerPll (MpAccessLocal &) ;
   ~GitterEulerPll () ;
} ;

	// Typparameter sind der Typ des Randelements und 
	// der des Interpolationsobjekts.

template < class A > class BndEulerIntPllX : public BndsegPllBaseXClosure < A > {
  protected :
    typedef A myhbnd_t ;
    typedef typename A :: myinterpolation_t myinterpolation_t ;
    inline myhbnd_t & myhbnd () ;
    inline const myhbnd_t & myhbnd () const ;
  public :
    inline BndEulerIntPllX (myhbnd_t &) ;
   ~BndEulerIntPllX () {}
    virtual void readDynamicState (ObjectStream &) ;
  private :
    myhbnd_t & _hbd ;
} ;

template < class A > class BndEulerIntPllXMacro : public BndsegPllBaseXMacroClosure < A > {
  protected :
    typedef A myhbnd_t ;
    typedef typename A :: myinterpolation_t myinterpolation_t ;
    inline myhbnd_t & myhbnd () ;
    inline const myhbnd_t & myhbnd () const ;
  public :
    inline BndEulerIntPllXMacro (myhbnd_t &) ;
   ~BndEulerIntPllXMacro () {}
    virtual void readDynamicState (ObjectStream &) ;
  private :
    myhbnd_t & _hbd ;
} ;


	//
	//    #    #    #  #          #    #    #  ######
	//    #    ##   #  #          #    ##   #  #
	//    #    # #  #  #          #    # #  #  #####
	//    #    #  # #  #          #    #  # #  #
	//    #    #   ##  #          #    #   ##  #
	//    #    #    #  ######     #    #    #  ######
	//


inline GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd3 :: EulerPllInternalBnd3 (Gitter :: Geometric :: hbndseg3_GEO :: myhface3_t * f, int t)
  : GitterEuler :: ObjectsEuler :: EulerBnd3 (f,t), _touched (0) {
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd3 :: bndtype () const {
  return closure ;
}

inline GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd4 :: EulerPllInternalBnd4 (Gitter :: Geometric :: hbndseg4_GEO :: myhface4_t * f, int t)
  : GitterEuler :: ObjectsEuler :: EulerBnd4 (f,t), _touched (0) {
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd4 :: bndtype () const {
  return closure ;
}

inline GitterEulerPll :: ObjectsEulerPll :: Hface3EulerPll :: Hface3EulerPll 
	(myhedge1_t * e0, int s0, myhedge1_t * e1, int s1, myhedge1_t * e2, int s2) :
  	GitterEuler :: ObjectsEuler :: Hface3Euler (e0,s0,e1,s1,e2,s2), _pllx (*this) {
  return ;
}

inline GitterEulerPll :: ObjectsEulerPll :: Hface4EulerPll :: Hface4EulerPll 
	(myhedge1_t * e0, int s0, myhedge1_t * e1, int s1, myhedge1_t * e2, int s2, myhedge1_t * e3, int s3) :
  	GitterEuler :: ObjectsEuler :: Hface4Euler (e0,s0,e1,s1,e2,s2,e3,s3), _pllx (*this) {
  return ;
}

inline GitterEulerPll :: ObjectsEulerPll :: TetraEulerPll :: TetraEulerPll (myhface3_t * f0, int t0, myhface3_t * f1, int t1,
	myhface3_t * f2, int t2, myhface3_t * f3, int t3) : GitterEuler :: ObjectsEuler :: TetraEuler (f0,t0,f1,t1,f2,t2,f3,t3)
	, _pllx (*this) {
  return ;
}

inline GitterEulerPll :: ObjectsEulerPll :: HexaEulerPll :: HexaEulerPll (myhface4_t * f0, int t0, myhface4_t * f1, int t1,
	myhface4_t * f2, int t2, myhface4_t * f3, int t3, myhface4_t * f4, int t4, myhface4_t * f5, int t5)
	: GitterEuler :: ObjectsEuler :: HexaEuler (f0,t0,f1,t1,f2,t2,f3,t3,f4,t4,f5,t5), _pllx (*this) {
  return ;
}

inline Gitter :: Makrogitter & GitterEulerPll :: container () {
  return * _macrogitter ;
}

inline const Gitter :: Makrogitter & GitterEulerPll :: container () const {
  return * _macrogitter ;
}

inline GitterEulerPll :: MacroGitterPll & GitterEulerPll :: containerPll () {
  return * _macrogitter ;
}

inline const GitterEulerPll :: MacroGitterPll & GitterEulerPll :: containerPll () const {
  return * _macrogitter ;
}

template < class A > inline BndEulerIntPllX < A > :: BndEulerIntPllX (myhbnd_t & b) : BndsegPllBaseXClosure < A > (b), _hbd (b) {
  return ;
}

template < class A > inline BndEulerIntPllX < A > :: myhbnd_t & BndEulerIntPllX < A > :: myhbnd () {
  return _hbd ;
}

template < class A > inline const BndEulerIntPllX < A > :: myhbnd_t & BndEulerIntPllX < A > :: myhbnd () const {
  return _hbd ;
}

template < class A > void BndEulerIntPllX < A > :: readDynamicState (ObjectStream & os) {
  BndsegPllBaseXClosure < A > :: readDynamicState (os) ;
  os.readObject (myhbnd ()._ipObj) ;
  myhbnd ()._touched = 1 ;
  return ;
}

template < class A > void BndEulerIntPllXMacro < A > :: readDynamicState (ObjectStream & os) {
  BndsegPllBaseXClosure < A > :: readDynamicState (os) ;
  os.readObject (myhbnd ()._ipObj) ;
  myhbnd ()._touched = 1 ;
  return ;
}

template < class A > inline BndEulerIntPllXMacro < A > :: BndEulerIntPllXMacro (myhbnd_t & b) : BndsegPllBaseXMacroClosure < A > (b), _hbd (b) {
  return ;
}

template < class A > inline BndEulerIntPllXMacro < A > :: myhbnd_t & BndEulerIntPllXMacro < A > :: myhbnd () {
  return _hbd ;
}

template < class A > inline const  BndEulerIntPllXMacro < A > :: myhbnd_t & BndEulerIntPllXMacro < A > :: myhbnd () const {
  return _hbd ;
}

#endif
