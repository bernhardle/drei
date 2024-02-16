	// (c) bernhard schupp, 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/gitter_euler_impl.h,v $
	// $Revision: 2.3 $
	// $Name:  $
	// $State: Exp $
	// $Date$

#ifndef GITTER_EULER_IMPL_H_INCLUDED
#define GITTER_EULER_IMPL_H_INCLUDED

#include "euler.h"
#include "gitter_impl.h"

static volatile char RCSId_gitter_euler_impl_h [] = "$Id: gitter_euler_impl.h,v 2.3 1998/12/01 17:13:41 yc23 Exp yc23 $" ;

class GitterEuler : public GitterBasis {
  public :
  
  	// typedef auf die Schnittstellenklassen der zu verwendenden Inter-
	// polationsobjekte auf Dreiecks- und Vierecksfl"achen.
  
      typedef EulerSolver :: InterpolationSimplex2D interpolationSimplex2DIF_t ;
      typedef EulerSolver :: InterpolationCube2D    interpolationCube2DIF_t ;
  
	// typedef auf die Klassen zur Berechnung der Eigenwerte des Systems

      typedef FabsEigenCalc < interpolationSimplex2DIF_t > fabsEigenCalcSimplex2D_t ;
      typedef FabsEigenCalc < interpolationCube2DIF_t >    fabsEigenCalcCube2D_t ;

	// typedef auf die Klassen zur Berechnung der Fl"usse am Sliprand

      typedef SlipCalc < interpolationSimplex2DIF_t > slipCalcSimplex2D_t ;
      typedef SlipCalc < interpolationCube2DIF_t >    slipCalcCube2D_t ;

	// typedef auf die Klassen zur Berechnung der numerischen Fl"usse

      typedef FluxCalcSimplex2DSW 	fluxCalcSimplex2D_t ;
      typedef FluxCalcCube2DSW 		fluxCalcCube2D_t ;

	// typedef auf die Klassen zur Berechnung der Dichtespr"unge.

      typedef DifferenceCalc < interpolationSimplex2DIF_t > differenceSimplex2D_t ;
      typedef DifferenceCalc < interpolationCube2DIF_t >    differenceCube2D_t ;
            
  public :
  
    class ObjectsEuler : public Objects {
      public :
        class Hface3Euler : public Hface3Empty {
	  protected :
	    typedef Gitter :: Geometric :: VertexGeo myvertex_t ;
	    typedef Gitter :: Geometric :: hedge1_GEO myhedge_t ;
            typedef VertexEmpty innervertex_t ;
            typedef hedge1_IMPL inneredge_t ;
	    inline Hface3Euler (myhedge_t *,int,myhedge_t *,int,myhedge_t *,int) ;
           ~Hface3Euler () {}
	  public :
	    EulerSolver :: Vec5 euler3dCalcFlux (double) ;
	    EulerSolver :: Vec5 euler3dCalcFlux (double,const EulerSolver :: InterpolationSimplex2D &, const EulerSolver :: InterpolationSimplex2D &) const ;
          public :
            EulerSolver :: req_t euler3dJumpCriterion () ;
            EulerSolver :: req_t euler3dJumpCriterion (const EulerSolver :: InterpolationSimplex2D &, const EulerSolver :: InterpolationSimplex2D &) ;
	} ;
        typedef Hface3Top < Hface3Euler > hface3_euler_IMPL ;
        
        class Hface4Euler : public Hface4Empty {
          protected :
	    typedef Gitter :: Geometric :: VertexGeo myvertex_t ;
	    typedef Gitter :: Geometric :: hedge1_GEO myhedge_t ;
            typedef VertexEmpty innervertex_t ;
            typedef hedge1_IMPL inneredge_t ;
            inline Hface4Euler (myhedge_t *,int,myhedge_t *,int,myhedge_t *,int,myhedge_t *,int) ;
           ~Hface4Euler () {}
          public :
            EulerSolver :: Vec5 euler3dCalcFlux (double) ;
            EulerSolver :: Vec5 euler3dCalcFlux (double,const EulerSolver :: InterpolationCube2D &, const EulerSolver :: InterpolationCube2D &) const ;
          public :
            EulerSolver :: req_t euler3dJumpCriterion () ;
            EulerSolver :: req_t euler3dJumpCriterion (const EulerSolver :: InterpolationCube2D &, const EulerSolver :: InterpolationCube2D &) ;
        } ;
        typedef Hface4Top < Hface4Euler > hface4_euler_IMPL ;

        class EulerBnd3 : public Hbnd3Default {
	  protected :
	    inline EulerBnd3 (hface3_GEO *, int) ;
	   ~EulerBnd3 () {}
          public :
            EulerSolver :: Vec5 euler3dCalcFlux (double, hface3 *,pair < hasFace3 *,int >,int) ;
            virtual EulerSolver :: Vec5 euler3dCalcFlux (double, hface3 *,const EulerSolver :: InterpolationSimplex2D &,int) = 0 ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace3 *,int >) ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D &) ;
          public :
            EulerSolver :: req_t euler3dJumpCriterion (hface3 *, pair < hasFace3 *, int > p, int) ;
            EulerSolver :: req_t euler3dJumpCriterion (hface3 *, const EulerSolver :: InterpolationSimplex2D &, int) ;
        } ;
	
        class EulerBnd4 : public Hbnd4Default {
	  protected :
	    inline EulerBnd4 (hface4_GEO *, int) ;
	   ~EulerBnd4 () {}
          public :
            EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *,pair < hasFace4 *,int >,int) ;
            virtual EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *,const EulerSolver :: InterpolationCube2D &,int) = 0 ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace4 *,int >) ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D &) ;
          public :
            EulerSolver :: req_t euler3dJumpCriterion (hface4 *, pair < hasFace4 *, int > p, int) ;
            EulerSolver :: req_t euler3dJumpCriterion (hface4 *, const EulerSolver :: InterpolationCube2D &, int) ;
        } ;

	//    #    #    #  ######  #        ####   #    #
	//    #    ##   #  #       #       #    #  #    #
	//    #    # #  #  #####   #       #    #  #    #
	//    #    #  # #  #       #       #    #  # ## #
	//    #    #   ##  #       #       #    #  ##  ##
	//    #    #    #  #       ######   ####   #    #

        class InflowBnd3 : public EulerBnd3 {
          static const EulerSolver :: Vec5 outer ;
          protected :
            inline InflowBnd3 (hface3_GEO *,int) ;
	   ~InflowBnd3 () {}
	  public :
            inline bnd_t bndtype () const ;
            EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *,pair < hasFace3 *, int>,int) ;
            EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *,const EulerSolver :: InterpolationSimplex2D &,int) ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace3 *,int >) ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D &) ;
        } ;
        typedef Hbnd3Top < InflowBnd3 > hbnd3_inflow_IMPL ;

        class InflowBnd4 : public EulerBnd4 {
          static const EulerSolver :: Vec5 outer ;
          protected :
            inline InflowBnd4 (hface4_GEO *,int) ;
	   ~InflowBnd4 () {}
	  public :
            inline bnd_t bndtype () const ;
            EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *, pair < hasFace4 *, int>, int) ;
            EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *,const EulerSolver :: InterpolationCube2D &,int) ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace4 *,int >) ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D &) ;
        } ;
        typedef Hbnd4Top < InflowBnd4 > hbnd4_inflow_IMPL ;

	//  ####   #    #   #####  ######  #        ####   #    #
	// #    #  #    #     #    #       #       #    #  #    #
	// #    #  #    #     #    #####   #       #    #  #    #
	// #    #  #    #     #    #       #       #    #  # ## #
	// #    #  #    #     #    #       #       #    #  ##  ##
	//  ####    ####      #    #       ######   ####   #    #


        class OutflowBnd3 : public EulerBnd3 {
          protected :
            inline OutflowBnd3 (hface3_GEO *, int) ;
	   ~OutflowBnd3 () {}
	  public :
            inline bnd_t bndtype () const ;
            EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *, pair < hasFace3 *, int>, int) ;
            EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *,const EulerSolver :: InterpolationSimplex2D &,int) ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace3 *,int >) ;
            EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D &) ;
        } ;
        typedef Hbnd3Top < OutflowBnd3 > hbnd3_outflow_IMPL ;

      class OutflowBnd4 : public EulerBnd4 {
        protected :
          inline OutflowBnd4 (hface4_GEO *, int) ;
	 ~OutflowBnd4 () {}
	public :
          inline bnd_t bndtype () const ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *, pair < hasFace4 *, int>, int) ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *,const EulerSolver :: InterpolationCube2D &,int) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace4 *,int >) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D &) ;
      } ;
      typedef Hbnd4Top < OutflowBnd4 > hbnd4_outflow_IMPL ;

	//  ####   #          #    #####
	// #       #          #    #    #
	//  ####   #          #    #    #
	//      #  #          #    #####
	// #    #  #          #    #
	//  ####   ######     #    #
	//
	// Die Klassen f"ur die Behandlung der Spliprandbedingung
	// (No - flux - b.c.) auf Dreiecks- und Vierecksrandfl"achen.

      class SlipBnd3 : public EulerBnd3 {
        protected :
          inline SlipBnd3 (hface3_GEO *, int) ;
	 ~SlipBnd3 () {}
	public :
          inline bnd_t bndtype () const ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *, pair < hasFace3 *, int>, int) ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *, const EulerSolver :: InterpolationSimplex2D &,int) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace3 *,int >) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D &) ;
      } ;
      typedef Hbnd3Top < SlipBnd3 > hbnd3_slip_IMPL ;

      class SlipBnd4 : public EulerBnd4 {
        protected :
          inline SlipBnd4 (hface4_GEO *, int) ;
	 ~SlipBnd4 () {}
	public :
          inline bnd_t bndtype () const ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *, pair < hasFace4 *, int>, int) ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *, const EulerSolver :: InterpolationCube2D &,int) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace4 *,int >) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D &) ;
      } ;
      typedef Hbnd4Top < SlipBnd4 > hbnd4_slip_IMPL ;

	// #####   ######  ######  #       ######   ####    #####
	// #    #  #       #       #       #       #    #     #
	// #    #  #####   #####   #       #####   #          #
	// #####   #       #       #       #       #          #
	// #   #   #       #       #       #       #    #     #
	// #    #  ######  #       ######  ######   ####      #
	//
	// Die Klassen f"ur die Behandlung der Symmetrierandbedingung
	// (Reflektion) auf Dreiecks- und Vierecksrandfl"achen.

      class ReflectBnd3 : public EulerBnd3 {
        protected :
          inline ReflectBnd3 (hface3_GEO *, int) ;
	 ~ReflectBnd3 () {}
	public :
          inline bnd_t bndtype () const ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *, pair < hasFace3 *, int>, int) ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *,const EulerSolver :: InterpolationSimplex2D &,int) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace3 *,int >) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D &) ;
      } ;
      typedef Hbnd3Top < ReflectBnd3 > hbnd3_reflect_IMPL ;

      class ReflectBnd4 : public EulerBnd4 {
        protected :
          inline ReflectBnd4 (hface4_GEO *, int) ;
	 ~ReflectBnd4 () {}
	public :
          inline bnd_t bndtype () const ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *, pair < hasFace4 *, int>, int) ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *,const EulerSolver :: InterpolationCube2D &,int) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace4 *,int >) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D &) ;
      } ;
      typedef Hbnd4Top < ReflectBnd4 > hbnd4_reflect_IMPL ;

      class TetraEuler : public tetra_GEO {
        EulerSolver :: EulerData _data ;
        double _volume ;
        bool _tag ;
        TetraEuler * down () = 0 ;
	const TetraEuler * down () const = 0 ;
	TetraEuler * next () = 0 ;
	const TetraEuler * next () const = 0 ;
	protected :
	  typedef VertexEmpty       innervertex_t ;
          typedef hedge1_IMPL       inneredge_t ;
	  typedef hface3_euler_IMPL innerface_t ;
	  inline int postRefinement () ;
	  inline int preCoarsening () ;
	  TetraEuler (hface3_GEO*,int,hface3_GEO*,int,hface3_GEO*,int,hface3_GEO*,int) ;
	 ~TetraEuler () {}
	  inline void setRequest (EulerSolver :: req_t) ;
	  inline double volume () const ;
          double euler3dUpdateData (double,double,double) ;
	  inline EulerSolver :: EulerData & accessEulerData () ;
          inline const EulerSolver :: EulerData & accessEulerData () const ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *,pair < hasFace3 *,int >,int) ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface3 *,const EulerSolver :: InterpolationSimplex2D &,int) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace3 *,int >) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D &) ;
          EulerSolver :: req_t euler3dJumpCriterion (hface3 *, pair < hasFace3 *, int >, int) ;
          EulerSolver :: req_t euler3dJumpCriterion (hface3 *, const EulerSolver :: InterpolationSimplex2D &, int) ;
      } ;
      typedef TetraTop < TetraEuler > tetra_euler_IMPL ;

      class HexaEuler : public hexa_GEO {
        EulerSolver :: EulerData _data ;
        double _volume ;
        bool _tag ;
	HexaEuler * down () = 0 ;
        const HexaEuler * down () const = 0 ;
	HexaEuler * next () = 0 ;
        const HexaEuler * next () const = 0 ;
        protected :
          typedef VertexEmpty innervertex_t ;
          typedef hedge1_IMPL inneredge_t ;
          typedef hface4_euler_IMPL innerface_t ;
          inline int postRefinement () ;
	  inline int preCoarsening () ;
          HexaEuler (hface4_GEO*,int,hface4_GEO*,int,hface4_GEO*,int,hface4_GEO*,int,hface4_GEO*,int,hface4_GEO*,int) ;
         ~HexaEuler () {}
          inline void setRequest (EulerSolver :: req_t) ;
	public :
          inline double volume () const ;
          double euler3dUpdateData (double,double,double) ;
	  inline EulerSolver :: EulerData & accessEulerData () ;
          inline const EulerSolver :: EulerData & accessEulerData () const ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *,pair < hasFace4 *,int >,int) ;
          EulerSolver :: Vec5 euler3dCalcFlux (double,hface4 *,const EulerSolver :: InterpolationCube2D &,int) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (pair < hasFace4 *,int >) ;
          EulerSolver :: Vec5 euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D &) ;
          EulerSolver :: req_t euler3dJumpCriterion (hface4 *, pair < hasFace4 *, int >, int) ;
          EulerSolver :: req_t euler3dJumpCriterion (hface4 *, const EulerSolver :: InterpolationCube2D &, int) ;
      } ;
      typedef HexaTop < HexaEuler > hexa_euler_IMPL ;
      
    } ;
  public :
    class MacrogitterEuler : public MacroGitterBasis {
      protected :
        hface3_GEO * insert_hface3 (hedge1_GEO *(&)[3],int (&)[3]) ;
        hface4_GEO * insert_hface4 (hedge1_GEO *(&)[4],int (&)[4]) ;
	hbndseg3_GEO * insert_hbnd3 (hface3_GEO *,int,Gitter :: hbndseg_STI :: bnd_t) ;
        hbndseg4_GEO * insert_hbnd4 (hface4_GEO *,int,Gitter :: hbndseg_STI :: bnd_t) ;
	tetra_GEO * insert_tetra (hface3_GEO *(&)[4],int (&)[4]) ;
        hexa_GEO * insert_hexa (hface4_GEO *(&)[6],int (&)[6]) ;
      public :
        inline MacrogitterEuler (istream &) ;
        inline MacrogitterEuler () ;
        virtual inline ~MacrogitterEuler () ;
    } ;
  private :
    MacrogitterEuler * _macrogitter ;
  private :
  
	// Die instanzenvariablen _deltat, _time, _step werden nur von addUpdate () ge"andert
	
    double _deltat ;
    double _time ;
    int _step ;
    virtual inline Makrogitter & container () ;
    virtual inline const Makrogitter & container () const ;
  protected :
    virtual void backupData (ostream &) ;
    virtual void restoreData (istream &) ;
    virtual bool estimateRefinement () ;
    virtual void calcFlux () ;
    virtual double addUpdate (double) ;
    virtual void oneTimeStep (double, bool = true) ;
  public :
    GitterEuler (const char *) ;
    GitterEuler () ;
    virtual ~GitterEuler () ;
  public :
    virtual void backupCMode (ostream &) ;
    virtual void backupCMode (const char *) ;
    virtual void backup (ostream &) ;
    virtual void backup (const char *) ;
    virtual void restore (istream &) ;
    virtual void restore (const char *) ;
  public :
    inline double deltat () const ;
    inline double time () const ;
    inline int step () const ;
    virtual void initialConstValue (const EulerSolver :: Vec5 &) ;
    virtual void runEulerSolver (double,double,int,const char *, int = -1) ;
} ;


	//
	//    #    #    #  #          #    #    #  ######
	//    #    ##   #  #          #    ##   #  #
	//    #    # #  #  #          #    # #  #  #####
	//    #    #  # #  #          #    #  # #  #
	//    #    #   ##  #          #    #   ##  #
	//    #    #    #  ######     #    #    #  ######
	//


inline GitterEuler :: ObjectsEuler :: Hface3Euler :: Hface3Euler (hedge1_GEO * e0,int s0, hedge1_GEO * e1, int s1, hedge1_GEO * e2, int s2)
  : GitterBasis :: Objects :: Hface3Empty (e0,s0,e1,s1,e2,s2) {
  return ;
}

inline GitterEuler :: ObjectsEuler :: Hface4Euler :: Hface4Euler (hedge1_GEO * e0,int s0, hedge1_GEO * e1, int s1, hedge1_GEO * e2, int s2, hedge1_GEO * e3, int s3)
  : GitterBasis :: Objects :: Hface4Empty (e0,s0,e1,s1,e2,s2,e3,s3) {
  return ;
}

inline GitterEuler :: ObjectsEuler :: EulerBnd3 :: EulerBnd3 (hface3_GEO * f, int t) 
  : GitterBasis :: Objects :: Hbnd3Default (f,t) {
  return ;
}

inline GitterEuler :: ObjectsEuler :: EulerBnd4 :: EulerBnd4 (hface4_GEO * f, int t) 
  : GitterBasis :: Objects :: Hbnd4Default (f,t) {
  return ;
}

inline GitterEuler :: ObjectsEuler :: InflowBnd3 :: InflowBnd3 (hface3_GEO * f, int t) 
  : GitterEuler :: ObjectsEuler :: EulerBnd3 (f,t) {
  return ;
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEuler :: ObjectsEuler :: InflowBnd3 :: bndtype () const {
  return inflow ; 
}

inline GitterEuler :: ObjectsEuler :: InflowBnd4 :: InflowBnd4 (hface4_GEO * f, int t) 
  : GitterEuler :: ObjectsEuler :: EulerBnd4 (f,t) {
  return ;
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEuler :: ObjectsEuler :: InflowBnd4 :: bndtype () const {
  return inflow ; 
}

inline GitterEuler :: ObjectsEuler :: OutflowBnd3 :: OutflowBnd3 (hface3_GEO * f, int t)
  : GitterEuler :: ObjectsEuler :: EulerBnd3 (f,t) {
  return ;
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEuler :: ObjectsEuler :: OutflowBnd3 :: bndtype () const {
  return outflow ; 
}

inline GitterEuler :: ObjectsEuler :: OutflowBnd4 :: OutflowBnd4 (hface4_GEO * f, int t)
  : GitterEuler :: ObjectsEuler :: EulerBnd4 (f,t) {
  return ;
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEuler :: ObjectsEuler :: OutflowBnd4 :: bndtype () const {
  return outflow ; 
}

inline GitterEuler :: ObjectsEuler :: SlipBnd3 :: SlipBnd3 (hface3_GEO * f, int t)
  : GitterEuler :: ObjectsEuler :: EulerBnd3 (f,t) {
  return ;
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEuler :: ObjectsEuler :: SlipBnd3 :: bndtype () const {
  return slip ; 
}

inline GitterEuler :: ObjectsEuler :: SlipBnd4 :: SlipBnd4 (hface4_GEO * f, int t)
  : GitterEuler :: ObjectsEuler :: EulerBnd4 (f,t) {
  return ;
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEuler :: ObjectsEuler :: SlipBnd4 :: bndtype () const {
  return slip ; 
}

inline GitterEuler :: ObjectsEuler :: ReflectBnd3 :: ReflectBnd3 (hface3_GEO * f, int t)
  : GitterEuler :: ObjectsEuler :: EulerBnd3 (f,t) {
  return ;
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEuler :: ObjectsEuler :: ReflectBnd3 :: bndtype () const {
  return reflect ; 
}

inline GitterEuler :: ObjectsEuler :: ReflectBnd4 :: ReflectBnd4 (hface4_GEO * f, int t)
  : GitterEuler :: ObjectsEuler :: EulerBnd4 (f,t) {
  return ;
}

inline Gitter :: hbndseg_STI :: bnd_t GitterEuler :: ObjectsEuler :: ReflectBnd4 :: bndtype () const {
  return reflect ; 
}

inline double GitterEuler :: ObjectsEuler :: TetraEuler :: volume () const {
  return _volume ;
}

inline int GitterEuler :: ObjectsEuler :: TetraEuler :: postRefinement () {
  for (TetraEuler * e = down () ; e ; e = e->next ()) e->_data.initial (_data.ucon()) ;
  _data.initial (EulerSolver :: Vec5 (.0,.0,.0,.0,.0)) ;
  return 0 ;
}

inline int GitterEuler :: ObjectsEuler :: TetraEuler :: preCoarsening () {
  EulerSolver :: Vec5 x (.0, .0, .0, .0, .0), y ;
  for (TetraEuler * e = down () ; e ; e = e->next ()) {
    y = e->_data.ucon() ;
    x += y *= e->volume () ;
  }
  _data.initial (x *= 1.0/volume ()) ;
  return 0 ;
}

inline void GitterEuler :: ObjectsEuler :: TetraEuler :: setRequest (EulerSolver :: req_t r) {
  switch (r) {
    case EulerSolver :: ref :
      request (myrule_t :: iso8) ;
      _tag = false ;
      break ;
    case EulerSolver :: none :
      if (_tag) request (myrule_t :: nosplit) ;
      _tag = false ;
      break ;
    case EulerSolver :: crs :
      break ;
    default :
      assert (0) ;
      break ;
  }
  return ;
}

inline EulerSolver :: EulerData & GitterEuler :: ObjectsEuler :: TetraEuler :: accessEulerData () {
  return _data ;
}

inline const EulerSolver :: EulerData & GitterEuler :: ObjectsEuler :: TetraEuler :: accessEulerData () const {
  return _data ;
}

inline double GitterEuler :: ObjectsEuler :: HexaEuler :: volume () const {
  return _volume ;
}

inline int GitterEuler :: ObjectsEuler :: HexaEuler :: postRefinement () {
  for (HexaEuler * e = down () ; e ; e = e->next ()) e->_data.initial (_data.ucon()) ;
  _data.initial (EulerSolver :: Vec5 (.0,.0,.0,.0,.0)) ;
  return 0 ;
}

inline int GitterEuler :: ObjectsEuler :: HexaEuler :: preCoarsening () {
  EulerSolver :: Vec5 x (.0, .0, .0, .0, .0), y ;
  for (HexaEuler * e = down () ; e ; e = e->next ()) {
    y = e->_data.ucon() ;
    x += y *= e->volume () ;
  }
  _data.initial (x *= 1.0/volume ()) ;
  return 0 ;
}

inline void GitterEuler :: ObjectsEuler :: HexaEuler :: setRequest (EulerSolver :: req_t r) {
  switch (r) {
    case EulerSolver :: ref :
      request (myrule_t :: iso8) ;
      _tag = false ;
      break ;
    case EulerSolver :: none :
      if (_tag) request (myrule_t :: nosplit) ;
      _tag = false ;
      break ;
    case EulerSolver :: crs :
      break ;
    default :
      assert (0) ;
      break ;
  }
  return ;
}

inline EulerSolver :: EulerData & GitterEuler :: ObjectsEuler :: HexaEuler :: accessEulerData () {
  return _data ;
}

inline const EulerSolver :: EulerData & GitterEuler :: ObjectsEuler :: HexaEuler :: accessEulerData () const {
  return _data ;
}

inline Gitter :: Makrogitter & GitterEuler :: container () {
  return * _macrogitter ;
}

inline const Gitter :: Makrogitter & GitterEuler :: container () const {
  return * _macrogitter ;
}

inline GitterEuler :: MacrogitterEuler :: MacrogitterEuler () {
}

inline GitterEuler :: MacrogitterEuler :: MacrogitterEuler (istream & in) {
  macrogridBuilder (in) ;
}

inline GitterEuler :: MacrogitterEuler :: ~MacrogitterEuler () {
}

inline double GitterEuler :: deltat () const {
  return _deltat ;
}

inline double GitterEuler :: time () const {
  return _time ;
}

inline int GitterEuler :: step () const {
  return _step ;
}

#endif
