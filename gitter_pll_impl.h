	// (c) bernhard schupp 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/gitter_pll_impl.h,v $
	// $Revision: 2.6 $
	// $Name:  $
	// $State: Exp $
	// $Date: 1998/12/13 13:00:43 $
	
#ifndef GITTER_PLL_IMPL_H_INCLUDED
#define GITTER_PLL_IMPL_H_INCLUDED

static volatile char RCSId_gitter_pll_impl_h [] = "$Id: gitter_pll_impl.h,v 2.6 1998/12/13 13:00:43 yc23 Exp yc23 $" ;

#include <vector.h>
#include <set.h>
#include <algorithm.h>

#include "myalloc.h"
#include "gitter_impl.h"
#include "gitter_pll_sti.h"
#include "gitter_pll_ldb.h"
#include "walk.h"

	// Der vector < int > wird als sog. linkagepattern, also als
	// Verbindungsmuster eingesetzt. Die Verbindungsmuster werden
	// nicht in jeder Parallelerweiterung gespeichert sondern in
	// einem zentralen Container im verteilten Grobgitter, dem
	// 'linkagePatternMap' und mit Z"ahlung der Referenzen 
	// verwaltet. Die Methode secondScan () l"oscht dann immer
	// wieder die unreferenzierten Verbindungsmuster aus dem
	// Container. Es gibt "ubrigens kein firstScan () mehr ...

typedef vector < int > linkagePattern_t ;
typedef map < linkagePattern_t, int, less < linkagePattern_t > > linkagePatternMap_t ;

class VertexPllBaseX : public VertexPllXIF, public MyAlloc {
  protected :
    typedef Gitter :: Geometric :: VertexGeo myvertex_t ;
    inline myvertex_t & myvertex () ;
    inline const myvertex_t & myvertex () const ;
  public :
    VertexPllBaseX (myvertex_t &,linkagePatternMap_t &) ;
   ~VertexPllBaseX () ;
    virtual vector < int > estimateLinkage () const ;
    virtual bool setLinkage (vector < int >) ;
    virtual Identifier getIdentifier () const ;
  public :
    virtual void attach2 (int) ;
    virtual void unattach2 (int) ;
    virtual bool packAll (vector < ObjectStream > &) ;
    virtual void unpackSelf (ObjectStream &, bool) ;
  private :
    static const linkagePattern_t nullPattern ;
    Refcount _ref ;
    myvertex_t & _v ;
    linkagePatternMap_t & _map ;
    linkagePatternMap_t :: iterator _lpn ;
    map < int, int, less < int > > _moveTo ;
} ;

class EdgePllBaseX : public EdgePllXIF, public MyAlloc {
  protected :
    typedef Gitter :: Geometric :: hedge1_GEO myhedge1_t ;
    inline myhedge1_t & myhedge1 () ;
    inline const myhedge1_t & myhedge1 () const ;
  public :
    inline EdgePllBaseX (myhedge1_t &) ;
    inline ~EdgePllBaseX () ;
    virtual vector < int > estimateLinkage () const ;
    virtual Identifier getIdentifier () const ;
    virtual void getRefinementRequest (ObjectStream &) const ;
    virtual bool setRefinementRequest (ObjectStream &) ;
  public :
    virtual bool lockAndTry () ;
    virtual bool unlockAndResume (bool) ;
    virtual bool lockedAgainstCoarsening () const ;
  public :
    virtual void attach2 (int) ;
    virtual void unattach2 (int) ;
    virtual bool packAll (vector < ObjectStream > &) ;
    virtual void unpackSelf (ObjectStream &, bool) ;
  private :
    myhedge1_t & _edge ;
    bool _lockCRS ;
} ;

class EdgePllBaseXMacro : public EdgePllBaseX {
  public :
    EdgePllBaseXMacro (myhedge1_t &) ;
   ~EdgePllBaseXMacro () ;
    virtual vector < int > estimateLinkage () const ;
    virtual Identifier getIdentifier () const ;
  public :
    virtual void attach2 (int) ;
    virtual void unattach2 (int) ;
    virtual bool packAll (vector < ObjectStream > &) ;
    virtual void unpackSelf (ObjectStream &, bool) ;
  private :
    map < int, int, less < int > > _moveTo ;
    Refcount _ref ;
} ;

template < class A > class FacePllBaseX : public FacePllXIF, public MyAlloc {
  protected :
    typedef A myhface_t ;
    inline myhface_t & myhface () ;
    inline const myhface_t & myhface () const ;
  public :
    inline FacePllBaseX (myhface_t &) ;
    inline ~FacePllBaseX () {}
    virtual vector < int > estimateLinkage () const ;
    virtual Identifier getIdentifier () const ;
    virtual vector < int > checkParallelConnectivity () const ;
  public :
    virtual ElementPllXIF_t & accessOuterPllX () ;
    virtual const ElementPllXIF_t & accessOuterPllX () const ;
    virtual ElementPllXIF_t & accessInnerPllX () ;
    virtual const ElementPllXIF_t & accessInnerPllX () const ;
  public :
    virtual void writeStaticState (ObjectStream &) const ;
    virtual void readStaticState (ObjectStream &) ;
  public :
    virtual bool ldbUpdateGraphEdge (LoadBalancer :: DataBase &) ;
    virtual void attach2 (int) ;
    virtual void unattach2 (int) ;
    virtual bool packAll (vector < ObjectStream > &) ;
    virtual void unpackSelf (ObjectStream &, bool) ;
  private :
    myhface_t & _face ;
} ;

template < class A > class FacePllBaseXMacro : public FacePllBaseX < A > {
  enum { ENDOFSTREAM = -1 } ;
  public :
    typedef typename FacePllBaseX < A > :: myhface_t myhface_t ;
    inline FacePllBaseXMacro (myhface_t &) ;
    inline ~FacePllBaseXMacro () ;
    virtual vector < int > estimateLinkage () const ;
    virtual Identifier getIdentifier () const ;
  public :
    virtual bool ldbUpdateGraphEdge (LoadBalancer :: DataBase &) ;
    virtual void attach2 (int) ;
    virtual void unattach2 (int) ;
    virtual bool packAll (vector < ObjectStream > &) ;
    virtual void unpackSelf (ObjectStream &, bool) ;
  private :
    map < int, int, less < int > > _moveTo ;
    Refcount _ref ;
} ;

class ElementPllBaseX : public ElementPllXIF, public MyAlloc {
  protected :
    enum { ENDOFSTREAM = -1 } ;
    
	// Alle Methoden in dieser Klasse sind Dummies und erzeugen
	// Laufzeitfehler. Sie m"ussen von abgeleiteten Klassen 
	// mit den richtigen Inhalten "uberschrieben werden.

  public :
    ElementPllXIF_t & accessOuterPllX (ElementPllXIF_t &) ;
    const ElementPllXIF_t & accessOuterPllX (const ElementPllXIF_t &) const ;
    ElementPllXIF_t & accessInnerPllX (ElementPllXIF_t &) ;
    const ElementPllXIF_t & accessInnerPllX (const ElementPllXIF_t &) const ;
  public :
    void writeStaticState (ObjectStream &) const ;
    void readStaticState (ObjectStream &) ;
    virtual void writeDynamicState (ObjectStream &) const  = 0 ;
    void readDynamicState (ObjectStream &) ;
  public :
    int ldbVertexIndex () const ;
    int & ldbVertexIndex () ;
    bool ldbUpdateGraphVertex (LoadBalancer :: DataBase &) ;
  protected :
    virtual void inlineData (ObjectStream &) throw (ObjectStream :: EOFException) {}
    virtual void xtractData (ObjectStream &) throw (ObjectStream :: EOFException) {}
  public :
    void attach2 (int) ;
    void unattach2 (int) ;
    bool packAll (vector < ObjectStream > &) ;
    void packAsBnd (int,int,ObjectStream &) const ;
    void unpackSelf (ObjectStream &,bool) ;
    bool erasable () const ;
  public :
    void getRefinementRequest (ObjectStream &) ;
    bool setRefinementRequest (ObjectStream &) ;
  public :
    bool lockAndTry () ;
    bool unlockAndResume (bool) ;
} ;

class TetraPllXBase : public ElementPllBaseX {
  public :
    typedef Gitter :: Geometric :: tetra_GEO mytetra_t ;
    inline mytetra_t & mytetra () ;
    inline const mytetra_t & mytetra () const ;
  public :
    inline TetraPllXBase (mytetra_t &) ;
    inline ~TetraPllXBase () {}
  public :
    void writeDynamicState (ObjectStream &) const ;
  private :
    mytetra_t & _tetra ;
} ;

class TetraPllXBaseMacro : public TetraPllXBase {
  public :
    TetraPllXBaseMacro (mytetra_t &) ;
   ~TetraPllXBaseMacro () ;
  protected :
    virtual void inlineData (ObjectStream &) throw (ObjectStream :: EOFException) {}
    virtual void xtractData (ObjectStream &) throw (ObjectStream :: EOFException) {}
  public :
    virtual void writeStaticState (ObjectStream &) const ;
    virtual int ldbVertexIndex () const ;
    virtual int & ldbVertexIndex () ;
    virtual bool ldbUpdateGraphVertex (LoadBalancer :: DataBase &) ;
  public :
    virtual void attach2 (int) ;
    virtual void unattach2 (int) ;
    virtual bool packAll (vector < ObjectStream > &) ;
    virtual void packAsBnd (int,int,ObjectStream &) const ;
    virtual void unpackSelf (ObjectStream &, bool) ;
    virtual bool erasable () const ;
  private :
    int _ldbVertexIndex ;
    map < int, int, less < int > > _moveTo ;
    bool _erasable ;
    double _center [3] ;
} ;

class HexaPllBaseX : public ElementPllBaseX {
  protected :
    typedef Gitter :: Geometric :: hexa_GEO myhexa_t ;
    inline myhexa_t & myhexa () ;
    inline const myhexa_t & myhexa () const ;
  public :
    inline HexaPllBaseX (myhexa_t &) ;
    inline ~HexaPllBaseX () {}
    void writeDynamicState (ObjectStream &) const ;
  private :
    myhexa_t & _hexa ;
} ;

class HexaPllBaseXMacro : public HexaPllBaseX {
  public :
    HexaPllBaseXMacro (myhexa_t &) ;
   ~HexaPllBaseXMacro () ;
    virtual void writeStaticState (ObjectStream &) const ;
  public :
    virtual int ldbVertexIndex () const ;
    virtual int & ldbVertexIndex () ;
    virtual bool ldbUpdateGraphVertex (LoadBalancer :: DataBase &) ;
  public :
    virtual void attach2 (int) ;
    virtual void unattach2 (int) ;
    virtual bool packAll (vector < ObjectStream > &) ;
    virtual void packAsBnd (int,int,ObjectStream &) const ;
    virtual void unpackSelf (ObjectStream &, bool) ;
    virtual bool erasable () const ;
  protected :
    virtual void inlineData (ObjectStream &) throw (ObjectStream :: EOFException) {}
    virtual void xtractData (ObjectStream &) throw (ObjectStream :: EOFException) {}
  private :
    int _ldbVertexIndex ;
    map < int, int, less < int > > _moveTo ;
    bool _erasable ;
    double _center [3] ;
} ;

class BndsegPllBaseX : public ElementPllBaseX {
  public :
    void writeDynamicState (ObjectStream &) const { abort () ; }
    ElementPllXIF_t & accessOuterPllX (ElementPllXIF_t &) ;
    const ElementPllXIF_t & accessOuterPllX (const ElementPllXIF_t &) const ;
    ElementPllXIF_t & accessInnerPllX (ElementPllXIF_t &) ;
    const ElementPllXIF_t & accessInnerPllX (const ElementPllXIF_t &) const ;
} ;

template < class A > class BndsegPllBaseXMacro : public BndsegPllBaseX {
  protected :
    typedef A                       myhbnd_t ;
    typedef typename A :: myhface_t myhface_t ;
    typedef typename A :: balrule_t balrule_t ;
    inline myhbnd_t & myhbnd () ;
    inline const myhbnd_t & myhbnd () const ;
  public :
    inline BndsegPllBaseXMacro (myhbnd_t &) ;
  public :
    virtual int ldbVertexIndex () const ;
    virtual int & ldbVertexIndex () ;
  public :
    virtual void packAsBnd (int,int,ObjectStream &) const ;
  private :
    myhbnd_t & _hbnd ;
} ;

template < class A > class BndsegPllBaseXClosure : public BndsegPllBaseX {
  protected :
    typedef A                       myhbnd_t ;
    typedef typename A :: myhface_t myhface_t ;
    typedef typename A :: balrule_t balrule_t ;
    inline myhbnd_t & myhbnd () ;
    inline const myhbnd_t & myhbnd () const ;
  public :
    inline BndsegPllBaseXClosure (myhbnd_t &) ;
   ~BndsegPllBaseXClosure () {}
    void readDynamicState (ObjectStream &) ;
    void getRefinementRequest (ObjectStream &) ;
    bool setRefinementRequest (ObjectStream &) ;
  public :
    bool lockAndTry () ;
    bool unlockAndResume (bool) ;
  public :
    virtual void notifyBalance (balrule_t,int) ;
    virtual bool lockedAgainstCoarsening () const ;
  private :
    myhbnd_t & _hbnd ;
    double _center [3] ;
    balrule_t _rul ;
    bool _lockCRS ;
} ;

template < class A > class BndsegPllBaseXMacroClosure : public BndsegPllBaseXClosure < A > {
  public :
    typedef A                       myhbnd_t ;
    typedef typename A :: myhface_t myhface_t ;
    inline BndsegPllBaseXMacroClosure (myhbnd_t &) ;
    virtual void readStaticState (ObjectStream &) ;
  public :
    virtual int   ldbVertexIndex () const ;
    virtual int & ldbVertexIndex () ;
  public :
    virtual void packAsBnd (int,int,ObjectStream &) const ;
  private :
    int _extGraphVertexIndex ;
} ;

class GitterBasisPll : public Gitter :: Geometric, public GitterPll {
  public :  
    class ObjectsPll : public GitterBasis :: Objects {
      public :
        class VertexPllImplMacro : public VertexEmptyMacro {
	  public :
	    typedef VertexPllBaseX mypllx_t ;
          public :
            VertexPllImplMacro (double,double,double,int,linkagePatternMap_t &) ;
	   ~VertexPllImplMacro () ;
            virtual VertexPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const VertexPllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
        } ;
      public :

        class Hedge1EmptyPll : public Hedge1Empty {
	  protected :
	    inline bool lockedAgainstCoarsening () const ;
	  public :
	    typedef EdgePllBaseX mypllx_t ;
	    inline Hedge1EmptyPll (myvertex_t *,myvertex_t *) ;
	    virtual EdgePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const EdgePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t _pllx ;
	  friend mypllx_t ;
	} ;
	typedef Hedge1Top < Hedge1EmptyPll > hedge1_IMPL ;
      public :
        class Hedge1EmptyPllMacro : public hedge1_IMPL {
	  public :
	    typedef EdgePllBaseXMacro mypllx_t ;
	    inline Hedge1EmptyPllMacro (myvertex_t *,myvertex_t *) ;
	   ~Hedge1EmptyPllMacro () ;
	    virtual EdgePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const EdgePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
	} ;

      public :
        class Hface3EmptyPll : public Hface3Empty {
	  protected :
	    typedef hedge1_IMPL inneredge_t ;
	  public :
	    typedef FacePllBaseX < hface3_GEO > mypllx_t ;
	    inline Hface3EmptyPll (myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int) ;
	    virtual FacePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const FacePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t _pllx ;
	  friend mypllx_t ;
	} ;
        typedef Hface3Top < Hface3EmptyPll > hface3_IMPL ;
	
        class Hface3EmptyPllMacro : public hface3_IMPL {
	  public :
	    typedef FacePllBaseXMacro < hface3_GEO > mypllx_t ;
	    Hface3EmptyPllMacro (myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int) ;
	   ~Hface3EmptyPllMacro () ;
	    virtual FacePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const FacePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
	} ;

      public :
        class Hface4EmptyPll : public Hface4Empty {
	  protected :
	    typedef hedge1_IMPL inneredge_t ;
	  public :
	    typedef FacePllBaseX < hface4_GEO > mypllx_t ;
	    inline Hface4EmptyPll (myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int) ;
	    virtual FacePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const FacePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	  private:
	    mypllx_t _pllx ;
	  friend mypllx_t ;
	} ;
        typedef Hface4Top < Hface4EmptyPll > hface4_IMPL ;
	
        class Hface4EmptyPllMacro : public hface4_IMPL {
	  public :
	    typedef FacePllBaseXMacro < hface4_GEO > mypllx_t ;
	    Hface4EmptyPllMacro (myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int,myhedge1_t *,int) ;
	   ~Hface4EmptyPllMacro () ;
	    virtual FacePllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const FacePllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
	} ;
      public :

        class TetraEmptyPll : public TetraEmpty {
	  protected :
	    typedef hedge1_IMPL inneredge_t ;
	    typedef hface3_IMPL innerface_t ;
	  public :
	    typedef TetraPllXBase mypllx_t ;
	    inline TetraEmptyPll (myhface3_t *,int,myhface3_t *,int,myhface3_t *,int,myhface3_t *,int) ;
           ~TetraEmptyPll () {}
	    virtual ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const ElementPllXIF_t &accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
          private :
	    mypllx_t _pllx ;
	} ;
	typedef TetraTop < TetraEmptyPll > tetra_IMPL ;

        class TetraEmptyPllMacro : public tetra_IMPL {
	  public :
	    typedef TetraPllXBaseMacro mypllx_t ;
	    TetraEmptyPllMacro (myhface3_t *,int,myhface3_t *,int,myhface3_t *,int,myhface3_t *,int) ;
	   ~TetraEmptyPllMacro () ;
	    virtual ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const ElementPllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
	} ;

      public :
        class HexaEmptyPll : public HexaEmpty {
	  protected :
	    typedef hedge1_IMPL inneredge_t ;
	    typedef hface4_IMPL innerface_t ;
	  public :
	    typedef HexaPllBaseX mypllx_t ;
	    inline HexaEmptyPll (myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int) ;
            inline ~HexaEmptyPll () {}
	    virtual ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const ElementPllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
          private :
	    mypllx_t _pllx ;
	} ;
	typedef HexaTop < HexaEmptyPll > hexa_IMPL ;

        class HexaEmptyPllMacro : public hexa_IMPL {
	  public :
	    typedef HexaPllBaseXMacro mypllx_t ;
	    HexaEmptyPllMacro (myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int,myhface4_t *,int) ;
	   ~HexaEmptyPllMacro () ;
	    virtual ElementPllXIF_t & accessPllX () throw (Parallel :: AccessPllException) ;
	    virtual const ElementPllXIF_t & accessPllX () const throw (Parallel :: AccessPllException) ;
	    virtual void detachPllXFromMacro () throw (Parallel :: AccessPllException) ;
	
	// Hier, und auch beim Tetraeder besteht die M"oglichkeit das refine
	// auf dem Grobgitterelement zu "uberschreiben, um gegebenenfalls noch
	// andere Funktionen mit aufzuklemmen:  
	// virtual int refine () ;
	  
	  private :
	    mypllx_t * _pllx ;
	  friend mypllx_t ;
	} ;
	
	// Die Randelemente des verteilten Gitters werden aus Templates 
	// in 'gitter_hexa_top_pll.h' und 'gitter_tetra_top_pll.h' erzeugt
	// indem diese die Randelementklassen des sequentiellen Verfahrens
	// "ubernehmen und mit passenden Extendern anreichern.
	
    } ;

  public :
    class MacroGitterBasisPll : public MacroGitterPll, public GitterBasis :: MacroGitterBasis {
      protected :
        linkagePatternMap_t _linkagePatterns ;
        set < int, less < int > > secondScan () ;
      protected :
        int iterators_attached () const ;
        VertexGeo   * insert_vertex (double,double,double,int,int = 0) ;
        hbndseg4_GEO * insert_hbnd4  (hface4_GEO *, int,Gitter :: hbndseg_STI :: bnd_t) ;
	hbndseg3_GEO * insert_hbnd3  (hface3_GEO *, int,Gitter :: hbndseg_STI :: bnd_t) ;
        hedge1_GEO   * insert_hedge1 (VertexGeo *, VertexGeo *) ;
        hface4_GEO   * insert_hface4 (hedge1_GEO *(&)[4], int (&)[4]) ;
	hface3_GEO   * insert_hface3 (hedge1_GEO *(&)[3], int (&)[3]) ;
        hexa_GEO     * insert_hexa (hface4_GEO *(&)[6], int (&)[6]) ;
	tetra_GEO    * insert_tetra (hface3_GEO *(&)[4], int (&)[4]) ;
        IteratorSTI < vertex_STI > * iterator (const vertex_STI *) const ;
        IteratorSTI < vertex_STI > * iterator (const IteratorSTI < vertex_STI > *) const ;
        IteratorSTI < hedge_STI > * iterator (const hedge_STI *) const ;
        IteratorSTI < hedge_STI > * iterator (const IteratorSTI < hedge_STI > *) const ;
        IteratorSTI < hface_STI > * iterator (const hface_STI *) const ;
        IteratorSTI < hface_STI > * iterator (const IteratorSTI < hface_STI > *) const ;
        IteratorSTI < helement_STI > * iterator (const helement_STI *) const ;
        IteratorSTI < helement_STI > * iterator (const IteratorSTI < helement_STI > *) const ;
        IteratorSTI < hbndseg_STI > * iterator (const hbndseg_STI *) const ;
        IteratorSTI < hbndseg_STI > * iterator (const IteratorSTI < hbndseg_STI > *) const ;
      public :
        MacroGitterBasisPll (istream &) ;
        MacroGitterBasisPll () ;
       ~MacroGitterBasisPll () ;
    } ;
  private :
    MpAccessLocal & _mpaccess ;
    MacroGitterPll * _macrogitter ;
  protected :
    virtual inline MpAccessLocal & mpAccess () ;
    virtual inline const MpAccessLocal & mpAccess () const ;
    virtual inline Makrogitter & container () ;
    virtual inline const Makrogitter & container () const ;
    virtual inline MacroGitterPll & containerPll () ;
    virtual inline const MacroGitterPll & containerPll () const ;
    GitterBasisPll (MpAccessLocal & ) ;
  public :
    GitterBasisPll (const char *, MpAccessLocal &) ;
    virtual ~GitterBasisPll () ;
// sp"ater   virtual void adapt () ;
} ;


	//
	//    #    #    #  #          #    #    #  ######
	//    #    ##   #  #          #    ##   #  #
	//    #    # #  #  #          #    # #  #  #####
	//    #    #  # #  #          #    #  # #  #
	//    #    #   ##  #          #    #   ##  #
	//    #    #    #  ######     #    #    #  ######
	//

inline VertexPllBaseX :: myvertex_t & VertexPllBaseX :: myvertex () {
  return _v ;
}

inline const VertexPllBaseX :: myvertex_t & VertexPllBaseX :: myvertex () const {
  return _v ;
}

inline EdgePllBaseX :: EdgePllBaseX (myhedge1_t & e) : _edge (e), _lockCRS (false) {
  return ;
}

inline EdgePllBaseX :: ~EdgePllBaseX () {
#ifndef DNDEBUG
	// Falls die nachfolgende Situation eintritt, ist massiv was faul im
	// parallelen Vergr"oberungsalgorithmus: Eine Kante, die gegen Ver-
	// gr"oberung gesperrt war, ist gel"oscht worden. Bestenfalls h"atten
	// die Kinder gel"oscht werden d"urfen, aber nur falls der lock auf-
	// gehoben wird.

  if (_lockCRS) {
    cerr << "**FEHLER (FATAL) in Datei " << __FILE__ << " Zeile " << __LINE__ << endl ;
    abort () ;
  }
#endif
  return ;
}

inline EdgePllBaseX :: myhedge1_t & EdgePllBaseX :: myhedge1 () {
  return _edge ;
}

inline const EdgePllBaseX :: myhedge1_t & EdgePllBaseX :: myhedge1 () const {
  return _edge ;
}

template < class A > inline FacePllBaseX < A > :: FacePllBaseX (myhface_t & f) : _face (f) {
  return ;
}

template < class A > inline FacePllBaseX < A > :: myhface_t & FacePllBaseX < A > :: myhface () {
  return _face ;
}

template < class A > inline const FacePllBaseX < A > :: myhface_t & FacePllBaseX < A > :: myhface () const {
  return _face ;
}
template < class A > vector < int > FacePllBaseX < A > :: estimateLinkage () const {
  return (abort (), vector < int > ()) ;
}

template < class A > LinkedObject :: Identifier FacePllBaseX < A > :: getIdentifier () const {
  return (abort (), LinkedObject :: Identifier ()) ;
}

template < class A > vector < int >  FacePllBaseX < A > :: checkParallelConnectivity () const {
  vector < int > v (A :: polygonlength + 1) ;
  int i ;
  for (i = 0 ; i < A :: polygonlength ; i ++)
    v [i] = myhface ().myvertex (0)->ident () ;
  v [i] = myhface ().level () ;
  return v ;
}

template < class A > ElementPllXIF_t & FacePllBaseX < A > :: accessOuterPllX () {
  return myhface ().nb.front ().first->accessPllX ().accessOuterPllX (myhface ().nb.rear ().first->accessPllX ()) ;
}

template < class A > const ElementPllXIF_t & FacePllBaseX < A > :: accessOuterPllX () const {
  return myhface ().nb.front ().first->accessPllX ().accessOuterPllX (myhface ().nb.rear ().first->accessPllX ()) ;
}

template < class A > ElementPllXIF_t & FacePllBaseX < A > :: accessInnerPllX () {
  return myhface ().nb.front ().first->accessPllX ().accessInnerPllX (myhface ().nb.rear ().first->accessPllX ()) ;
}

template < class A > const ElementPllXIF_t & FacePllBaseX < A > :: accessInnerPllX () const {
  return myhface ().nb.front ().first->accessPllX ().accessInnerPllX (myhface ().nb.rear ().first->accessPllX ()) ;
}

template < class A > void FacePllBaseX < A > :: writeStaticState (ObjectStream &) const {
  abort () ;
  return;
}

template < class A > void FacePllBaseX < A > :: readStaticState (ObjectStream &) {
  abort () ;
  return ;
}

template < class A > bool FacePllBaseX < A > :: ldbUpdateGraphEdge (LoadBalancer :: DataBase & db) {
  return (abort (), false) ;
}

template < class A > void FacePllBaseX < A > :: unattach2 (int) {
  abort () ;
  return ;
}

template < class A > void FacePllBaseX < A > :: attach2 (int) {
  abort () ;
  return ;
}

template < class A > bool FacePllBaseX < A > :: packAll (vector < ObjectStream > &) {
  return (abort (), false) ;
}

template < class A > void FacePllBaseX < A > :: unpackSelf (ObjectStream &, bool ) {
  abort () ;
  return ;
}

template < class A > inline FacePllBaseXMacro < A > :: FacePllBaseXMacro (myhface_t & f) : FacePllBaseX < A > (f), _moveTo (), _ref () {
  return ;
}

template < class A > inline FacePllBaseXMacro < A > :: ~FacePllBaseXMacro () {
  assert (0 == _moveTo.size ()) ;
  return ;
}

template < class A > vector < int > FacePllBaseXMacro < A > :: estimateLinkage () const {

	// Diese Methode sch"atzt den Verbindungsstern der Grobgitterfl"ache,
	// indem sie die Schnittmenge der Verbindungssterne der anliegenden
	// Grobgitterknoten bildet. Je besser die Sch"atzung, desto schneller
	// arbeitet das Identifikationsmodul. Falls keine Sch"atzung m"oglich
	// ist, kann man auch einfach aller log. Teilgiternummern in einem 
	// Vektor zur"uckgeben. Dann geht die Identifikation eben langsam.

  vector < int > t1, t2, est ;
  vector < int > l0 = myhface ().myhedge1 (0)->accessPllX ().estimateLinkage () ;
  vector < int > l1 = myhface ().myhedge1 (1)->accessPllX ().estimateLinkage () ;
  vector < int > l2 = myhface ().myhedge1 (2)->accessPllX ().estimateLinkage () ;
  vector < int > l3 = myhface ().myhedge1 (A :: polygonlength == 3 ? 2 : 3)->accessPllX ().estimateLinkage () ;
  set_intersection (l0.begin (), l0.end (), l1.begin (), l1.end (), back_inserter (t1), less < int > ()) ;
  set_intersection (l2.begin (), l2.end (), l3.begin (), l3.end (), back_inserter (t2), less < int > ()) ;
  set_intersection (t1.begin (), t1.end (), t2.begin (), t2.end (), back_inserter (est), less < int > ()) ;
  return est ;
}

template < class A > LinkedObject :: Identifier FacePllBaseXMacro < A > :: getIdentifier () const {

	// Diese Methode erzeugt den Schl"ussel einer Grobgitterfl"ache im Auftrag
	// des Identifikationsmoduls 'gitter_pll_idn.cc'. Der Schl"ussel besteht
	// aus drei Integer Werten 'key.h', die aus der globelen Ausrichtung der
	// Fl"ache folgen und auch f"ur Fl"achen mit mehr als drei Knoten korrekt
	// (d.h. gleiche Fl"ache in versch. Teilgittern -> gleicher Schl"ussel) sind.

  return Identifier (myhface ().myvertex (0)->ident (), myhface ().myvertex (1)->ident (), myhface ().myvertex (2)->ident ()) ;
}

template < class A > bool FacePllBaseXMacro < A > :: ldbUpdateGraphEdge (LoadBalancer :: DataBase & db) {
  
	// Diese Methode erzeugt eine Kante im Graphen f"ur die Berechnung
	// der Neupartitionierung, der sie das Gewicht der Anzahl aller feinsten
	// Fl"achen "uber der verwalteten Grobgitterfl"ache gibt.
  
  db.edgeUpdate ( LoadBalancer :: GraphEdge 
  	(((const myhface_t :: myconnect_t *)myhface ().nb.front ().first)->accessPllX ().ldbVertexIndex (),
  	 ((const myhface_t :: myconnect_t *)myhface ().nb.rear ().first)->accessPllX ().ldbVertexIndex (),
  	 TreeIterator < Gitter :: hface_STI, is_leaf < Gitter :: hface_STI > > (myhface ()).size ())) ;
  return true ;
}

template < class A > void FacePllBaseXMacro < A > :: unattach2 (int i) {

	// Diese Methode bindet die Fl"ache von einer Zuweisung zu einem neuen
	// Teilgitter ab. D.h. der Eintrag in der Zuweisungsliste wird gel"oscht,
	// und dann wird die M"oglichkeit an die anliegenden Kanten weitervermittelt.

  assert (_moveTo.find (i) != _moveTo.end ()) ;
  if ( -- _moveTo [i] == 0) _moveTo.erase (i) ;
  {for (int j = 0 ; j < A :: polygonlength ; j ++) myhface ().myhedge1 (j)->accessPllX ().unattach2 (i) ;}
  return ;
}

template < class A > void FacePllBaseXMacro < A > :: attach2 (int i) {
  map < int, int, less < int > > :: iterator pos = _moveTo.find (i) ;
  if (pos == _moveTo.end ()) {
    _moveTo.insert (pair < const int, int > (i,1)) ;
  } else {
    (*pos).second ++ ;
  }
  {for (int j = 0 ; j < A :: polygonlength ; j ++) myhface ().myhedge1 (j)->accessPllX ().attach2 (i) ;}
  return ;  
}

template < class A > bool FacePllBaseXMacro < A > :: packAll (vector < ObjectStream > & osv) {

	// Die Methode packAll () verpackt die Fl"ache auf alle Datenstr"ome,
	// die zu Teilgittern f"uhren, an die sie zugewiesen wurde mit attach2 ().
	// Ausserdem geht die Methode noch an die anliegenden Elemente (Randelemente)
	// "uber.

  bool action = false ;
  for (map < int, int, less < int > > :: const_iterator i = _moveTo.begin () ; i != _moveTo.end () ; i ++) {
    int j = (*i).first ;
    assert ((osv.begin () + j) < osv.end ()) ;
    
	// "Ubergangsl"osung:
    
    if (A :: polygonlength == 4) osv [j].writeObject (FACE4) ;
    else if (A :: polygonlength == 3) osv [j].writeObject (FACE3) ;
    else abort () ;
    
    {
      for (int k = 0 ; k < A :: polygonlength ; k ++)
        osv [j].writeObject (myhface ().myvertex (k)->ident ()) ;
    }
    try {
    
	// Sicherheitshalber testen, ob das ENDOFSTREAM Tag nicht auch
	// mit einer Verfeinerungsregel identisch ist - sonst gibt's
	// nachher beim Auspacken nur garbage.
    
      assert (! myhface_t :: myrule_t (ENDOFSTREAM).isValid ()) ;
    
    	// Der ganze Baum der Verfeinerungsregeln wird jetzt in den
	// Stringstream geschrieben (anstelle einer Datei) und dann
	// in den Datenstrom als 'integer' hineinkonvertiert.
    
      strstream s ;
      myhface ().backup (s) ;
      {for (int c = s.get () ; ! s.eof () ; c = s.get ()) osv [j].writeObject (c) ;}
      osv [j].writeObject (ENDOFSTREAM) ;
    } catch (ObjectStream :: OutOfMemoryException) {
      cerr << "**FEHLER (FATAL) ObjectStream :: OutOfMemoryException aufgetreten in " << __FILE__ << " " << __LINE__ << endl ;
      abort () ;
    }
    try {
    
 	// Wenn die Fl"ache auf den j. Strom des Lastverschiebers
	// geschrieben wurde, dann mu"ussen auch die anliegenden
	// Elemente daraufhin untersucht werden, ob sie sich nicht
	// als Randelemente dorthin schreiben sollen - das tun sie
	// aber selbst.
    
      myhface ().nb.front ().first->accessPllX ().packAsBnd (myhface ().nb.front ().second, j, osv [j]) ;
      myhface ().nb.rear ().first->accessPllX ().packAsBnd (myhface ().nb.rear ().second, j, osv [j]) ;
    } catch (Parallel :: AccessPllException) {
      cerr << "**FEHLER (FATAL) AccessPllException aufgetreten in " << __FILE__ << " " << __LINE__ << ". Ende." << endl ;
      abort () ;
    }
    action = true ;
  }
  return action ;
}

template < class A > void FacePllBaseXMacro < A > :: unpackSelf (ObjectStream & os, bool i) {

	// Die Methode wird eine Fl"ache aus dem Datenstrom rekonstruieren,
	// die der MacrogridBuilder angelegt hat, oder sie wird nur die
	// Verfeinerungsregeln aus dem Strom nehmen, falls schon vorher
	// eine identische Fl"ache bestand. Dies wird vom MacrogridBuilder
	// durch die 'bool' Variable mitgeteilt. Trotzdem k"onnen auch auf
	// einen bestehenden Fl"achenbaum durch die Lastverschiebung neue
	// Daten aufgebracht werden - dies ist dann hier zu realisieren.

  strstream s ;
  int c ;
  try {
    for (os.readObject (c) ; c != ENDOFSTREAM ; os.readObject (c)) s.put (char(c)) ;
  } catch (ObjectStream :: EOFException) {
    cerr << "**FEHLER EOF gelesen in " << __FILE__ << " " << __LINE__ << endl ;
    abort () ;
  }
  if (i) {
  
	// Sobald der Stringstream mit den 'byte' Verfeinerungsregeln
	// voll ist, kann mit dem normalen restore der ganze Fl"achen-
	// baum wieder hochgezogen werden. Analog zur Wiederherstellung
	// aus einer Datei.
  
    myhface ().restore (s) ;
    assert (!s.eof ()) ;
  }
  return ;
}

inline TetraPllXBase :: TetraPllXBase (mytetra_t & t) : _tetra (t) {
  return ;
}

inline TetraPllXBase :: mytetra_t & TetraPllXBase :: mytetra () {
  return _tetra ;
}

inline const TetraPllXBase :: mytetra_t & TetraPllXBase :: mytetra () const {
  return _tetra ;
}

inline HexaPllBaseX :: HexaPllBaseX (myhexa_t & h) : _hexa (h) {
  return ;
}

inline HexaPllBaseX :: myhexa_t & HexaPllBaseX :: myhexa () {
  return _hexa ;
}

inline const HexaPllBaseX :: myhexa_t & HexaPllBaseX :: myhexa () const {
  return _hexa ;
}


template < class A > inline BndsegPllBaseXMacro < A > :: BndsegPllBaseXMacro (myhbnd_t & b) : _hbnd (b) {
  return ;
}

template < class A > inline BndsegPllBaseXMacro < A > :: myhbnd_t & BndsegPllBaseXMacro < A > :: myhbnd () {
  return _hbnd ;
}

template < class A > inline const BndsegPllBaseXMacro < A > :: myhbnd_t & BndsegPllBaseXMacro < A > :: myhbnd () const {
  return _hbnd ;
}

template < class A > int BndsegPllBaseXMacro < A > ::ldbVertexIndex () const {
  return -1 ;
}

template < class  A > int & BndsegPllBaseXMacro < A > :: ldbVertexIndex () {
  return (abort (), *(int *)0) ;
}

template < class A > void BndsegPllBaseXMacro < A > :: packAsBnd (int fce, int who, ObjectStream & os) const {
  assert (!fce) ;
  if (myhface_t :: polygonlength == 3) os.writeObject (HBND3EXT) ;
  else if (myhface_t :: polygonlength == 4) os.writeObject (HBND4EXT) ;
  else abort () ;
  os.writeObject (myhbnd ().bndtype ()) ;
  {for (int i = 0 ; i < myhface_t :: polygonlength ; i++) os.writeObject (myhbnd ().myvertex (fce,i)->ident ()) ; }
  return ;
}

template < class A > inline BndsegPllBaseXClosure < A > :: BndsegPllBaseXClosure (myhbnd_t & b) : _hbnd (b), _lockCRS (false) {
  return ;
}

template < class A > inline BndsegPllBaseXClosure < A > :: myhbnd_t & BndsegPllBaseXClosure < A > :: myhbnd () {
  return _hbnd ;
}

template < class A > inline const BndsegPllBaseXClosure < A > :: myhbnd_t & BndsegPllBaseXClosure < A > :: myhbnd () const {
  return _hbnd ;
}

template < class A > inline void BndsegPllBaseXClosure < A > :: notifyBalance (balrule_t r,int) {
  _rul = r ;
  return ;
}

template < class A > void BndsegPllBaseXClosure < A > :: getRefinementRequest (ObjectStream & os) {
  os.writeObject (int (_rul)) ;
  _rul = balrule_t :: nosplit ;
  return ;
}

template < class A > bool BndsegPllBaseXClosure < A > :: setRefinementRequest (ObjectStream & os) {

	// Die Methode schickt die Regel aus dem Nachbargitter in das
	// eigene Teilgitter hinein. Sie liefert "true" immer dann, wenn
	// sich das eigene Gitter g"andert hat -> f"uhrt zu einer weiteren
	// Iteration des parallelen refine ().

  int i ;
  try {
    os.readObject (i) ;
  } catch (ObjectStream :: EOFException) {
    cerr << "**FEHLER (FATAL) BndsegPllBaseXClosure :: setRefinementRequest (..)\n" ;
    cerr << "  EOF gelesen. In " << __FILE__ << " " << __LINE__ << endl ;
    abort () ;
  }
  balrule_t r (i)  ;
  if (r == balrule_t :: nosplit) {
    return false ;
  } else {
    if (myhbnd ().getrule () == r) {
      return false ;
    } else {
      if (myhbnd ().refineLikeElement (r)) {
      
    	// Verfeinerung erfolgreich
    
        return true ;
      } else {
      
      	// Verfeinerung verhindert irgendwo im Gitter. Dies ist ein Vorbehalt
	// f"ur den parallelen anisotropen Verfeinerungsalgorithmus. Daher
	// sollte die Situation im isotropen Fall nicht auftreten.
      
        cerr << "**FEHLER (FATAL, weil nicht vorgesehen): Verfeinerung wurde\n" ;
        cerr << "  verhindert am inneren Rand. In " << __FILE__ << " " << __LINE__ << endl ;
	abort () ;
      }
    }
  }
  return (abort (), false) ;
}

template < class A > void BndsegPllBaseXClosure < A > :: readDynamicState (ObjectStream & os) {
  try {
    os.readObject (_center [0]) ;
    os.readObject (_center [1]) ;
    os.readObject (_center [2]) ;
  } catch (ObjectStream :: EOFException) {
    cerr << "**FEHLER (FATAL) EOF gelesen in " << __FILE__ << " " << __LINE__ << endl ;
    abort () ;
  }
  return ;
}

template < class A > bool BndsegPllBaseXClosure < A > :: lockAndTry () {
  _lockCRS = true ;
  return myhbnd ().bndNotifyCoarsen () ;
}

template < class A > bool BndsegPllBaseXClosure < A > :: lockedAgainstCoarsening () const {
  return _lockCRS ;
}

template < class A > bool BndsegPllBaseXClosure < A > :: unlockAndResume (bool r) {
  _lockCRS = false ;
  bool x ;
  if (r) {
    x = myhbnd ().bndNotifyCoarsen () ;
  }
  else {
    x = false ;
  }
  return x ;
}

template < class A > inline BndsegPllBaseXMacroClosure < A > :: BndsegPllBaseXMacroClosure (myhbnd_t & b)
  : BndsegPllBaseXClosure < A > (b), _extGraphVertexIndex (-1) {
  return ;
}

template < class A > int BndsegPllBaseXMacroClosure < A > :: ldbVertexIndex () const {
  return _extGraphVertexIndex ;
}

template < class A > int & BndsegPllBaseXMacroClosure < A > :: ldbVertexIndex () {
  return _extGraphVertexIndex ;
}

template < class A > void BndsegPllBaseXMacroClosure < A > :: readStaticState (ObjectStream & os) {
  try {
    os.readObject (_extGraphVertexIndex) ;
  } catch (ObjectStream :: EOFException) {
    cerr << "**FEHLER EOF gelesen in " << __FILE__ << " " << __LINE__ << endl ;
    abort () ;
  }
  assert (_extGraphVertexIndex >= 0) ;
  return ;
}

template < class A > void BndsegPllBaseXMacroClosure < A > :: packAsBnd (int fce, int who, ObjectStream & os) const {
  assert (!fce) ;
  assert (myhbnd ().bndtype () == Gitter :: hbndseg :: closure) ;
  if (myhface_t :: polygonlength == 3) os.writeObject (HBND3INT) ;
  else if (myhface_t :: polygonlength == 4) os.writeObject (HBND4INT) ;
  else abort () ;
  os.writeObject (myhbnd ().bndtype ()) ;
  {for (int i = 0 ; i < myhface_t :: polygonlength ; i++) os.writeObject (myhbnd ().myvertex (fce,i)->ident ()) ; }
  return ;
}

GitterBasisPll :: ObjectsPll :: Hedge1EmptyPll :: Hedge1EmptyPll (VertexGeo * a, VertexGeo * b) :
  GitterBasis :: Objects :: Hedge1Empty (a,b), _pllx (*this) {
  return ;
}

inline bool GitterBasisPll :: ObjectsPll :: Hedge1EmptyPll :: lockedAgainstCoarsening () const {
 return accessPllX ().lockedAgainstCoarsening () ;
}

inline GitterBasisPll :: ObjectsPll :: Hface3EmptyPll :: Hface3EmptyPll (myhedge1_t * e0, int s0, myhedge1_t * e1, int s1, myhedge1_t * e2, int s2) :
	GitterBasis :: Objects :: Hface3Empty (e0,s0,e1,s1,e2,s2), _pllx (*this) {
  return ;
}

inline GitterBasisPll :: ObjectsPll :: Hface4EmptyPll :: Hface4EmptyPll 
	(myhedge1_t * e0, int s0, myhedge1_t * e1, int s1, myhedge1_t * e2, int s2, myhedge1_t * e3, int s3) :
	GitterBasis :: Objects :: Hface4Empty (e0,s0,e1,s1,e2,s2,e3,s3), _pllx (*this) {
  return ;
}

inline GitterBasisPll :: ObjectsPll :: TetraEmptyPll :: TetraEmptyPll (myhface3_t * f0, int t0, myhface3_t * f1, int t1, myhface3_t * f2, int t2, myhface3_t * f3, int t3)
	: GitterBasis :: Objects :: TetraEmpty (f0,t0,f1,t1,f2,t2,f3,t3), _pllx (*this) {
  return ;
}

inline GitterBasisPll :: ObjectsPll :: HexaEmptyPll :: HexaEmptyPll (myhface4_t * f0, int t0, myhface4_t * f1, int t1, myhface4_t * f2, int t2, myhface4_t * f3, int t3, myhface4_t * f4, int t4, myhface4_t * f5, int t5)
	: GitterBasis :: Objects :: HexaEmpty (f0,t0,f1,t1,f2,t2,f3,t3,f4,t4,f5,t5), _pllx (*this) {
  return ;
}

inline int GitterBasisPll :: MacroGitterBasisPll :: iterators_attached () const {
  return GitterBasis :: MacroGitterBasis :: iterators_attached () + MacroGitterPll :: iterators_attached () ;
}

inline MpAccessLocal & GitterBasisPll :: mpAccess () {
  return _mpaccess ;
}

inline const MpAccessLocal & GitterBasisPll :: mpAccess () const {
  return _mpaccess ;
}

inline GitterBasisPll :: Makrogitter & GitterBasisPll :: container () {
  return * _macrogitter ;
}

inline const GitterBasisPll :: Makrogitter & GitterBasisPll :: container () const {
  return * _macrogitter ;
}

inline GitterBasisPll :: MacroGitterPll & GitterBasisPll :: containerPll () {
  return * _macrogitter ;
}

inline const GitterBasisPll :: MacroGitterPll & GitterBasisPll :: containerPll () const {
  return * _macrogitter ;
}
#endif
