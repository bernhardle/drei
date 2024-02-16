	// (c) bernhard schupp 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/euler.h,v $
	// $Revision: 2.3 $
	// $Name:  $
	// $State: Exp $
	// $Date$

#ifndef EULER_H_INCLUDED
#define EULER_H_INCLUDED
#include <assert.h>
#include <math.h>
#include <iostream.h>
#include <vector.h>
#include "serialize.h"

static volatile char RCSId_euler_h [] = "$Id: euler.h,v 2.3 1998/12/01 17:13:41 yc23 Exp yc23 $" ;

class EulerSolver {
  public :
  
	// Enumerierter Typ f"ur die Verfeinerungsanforderung
	// auf den Elementen f"ur den Eulerl"oser.
  
    typedef enum { none = 0, crs = 1, ref = 2 } req_t ;

	// F"unfkomponentiger Erhaltungsvektor f"ur das Eulerverfahren
	// in 3D.

    class Vec5 {
      double u [5] ;
      public :
        inline Vec5 () ;
        inline Vec5 (double,double,double,double,double) ;
        inline Vec5 (const Vec5 &) ;
        inline const Vec5 & operator += (const Vec5 &) ;
        inline const Vec5 & operator -= (const Vec5 &) ;
        inline const Vec5 & operator *= (const double) ;
        inline const Vec5 & operator /= (const double) ;
        inline Vec5 operator + (const Vec5 &) const ;
        inline Vec5 operator - (const Vec5 &) const ;
        inline const Vec5 & operator = (const Vec5 &) ;
        inline double operator [] (int) const ;
        inline double & operator [] (int) ;
        inline double rho () const ;
        inline double vdot (const double (&)[3]) const ;
        inline double pressure () const ;
	
	// F"ur die c - fluss - interfaces:
	
	inline operator double * () ;
	inline operator const double * () const ;
	
      friend ostream & operator << (ostream &, const Vec5 &) ;
      friend istream & operator >> (istream &, Vec5 &) ;
    } ;

	// Klasse mit den elementweise konstanten Eulerdaten, bestehend
	// aus dem Erhaltungsvektor, dem Updatevektor und Methoden zum
	// Zugriff auf die physikalischen Gr"ossen. Dieses Datum kann
	// f"ur alle Elementtypen verwendet werden. Eine Unterscheidung
	// ist erst im Fall einer h"oheren Approximationsordnung n"otig.

    class EulerData : public Serializable {
      Vec5 _a, _b ;
      public :
        inline EulerData () ;
        inline void print (ostream &) const ;
        inline void read (istream &) ;
        inline const Vec5 & initial (const Vec5 &) ;
        inline const Vec5 & ucon () const ;
	      inline const Vec5 & uupd () const ;
        inline double pressure () const ;
        inline double rho () const ;
        inline double energy () const ;
        inline double vdot (const double (&)[3]) const ;
        inline const Vec5 & add (const Vec5 &) ;
        inline const Vec5 & sub (const Vec5 &) ;
        inline void update (double,double) ;
      public :
        void writeObject (ObjectStream &) const ;
        bool readObject (ObjectStream &) throw (ObjectStream :: EOFException) ;
    } ;

	// Abstrakte Schnittstellen f"ur die Interpolationsobjekte auf
	// Dreiecks- und Vierecksfl"achen.

    class InterpolationSimplex2D {
      public :
        typedef const double (&coord_t)[3] ;
        typedef Vec5 val_t ;
        virtual ~InterpolationSimplex2D () {}
        virtual val_t operator () (coord_t) const = 0 ;
    } ;

    class InterpolationCube2D {
      public :
        typedef const double (&coord_t)[2] ;
        typedef Vec5 val_t ;
        virtual ~InterpolationCube2D () {}
        virtual val_t operator () (coord_t) const = 0 ;
    } ;

	// Das nullte - Ordnung Interpolationsobjekt kann gleicherma"sen
	// f"ur Dreicksfl"achen und Vierecksfl"achen verwendet werden.

    class InterpolationCube2Dzero : public InterpolationSimplex2D, public InterpolationCube2D, public Serializable {
      public :
        typedef InterpolationSimplex2D :: val_t val_t ;
        inline InterpolationCube2Dzero (const EulerData &) ;
        inline InterpolationCube2Dzero (const val_t &) ;
        inline InterpolationCube2Dzero () ;
        inline ~InterpolationCube2Dzero () ;
        inline val_t operator () (InterpolationSimplex2D :: coord_t) const ;
	inline val_t operator () (InterpolationCube2D :: coord_t) const ;
	void writeObject (ObjectStream &) const ;
        bool readObject (ObjectStream &) throw (ObjectStream :: EOFException) ;
      private :
        val_t _base ;
    } ;

	// Die Implementierungen f"ur erste Ordnung (liniear, bilinear)
	// Interpolationsobjekte auf Dreiecks- und Vierecksfl"achen.

    class InterpolationSimplex2Dfirst : public InterpolationSimplex2D, public Serializable {
      public :
        typedef InterpolationSimplex2D :: val_t val_t ;
        inline InterpolationSimplex2Dfirst (const val_t &) ;
        inline InterpolationSimplex2Dfirst () ;
        inline ~InterpolationSimplex2Dfirst () ;
        val_t operator () (coord_t) const ;
	void writeObject (ObjectStream &) const ;
	bool readObject (ObjectStream &) throw (ObjectStream :: EOFException) ;
      private :
        val_t _d0, _d1, _d2 ;
    } ;

    class InterpolationCube2Dfirst : public InterpolationCube2D, public Serializable {
      public :
        typedef InterpolationCube2D :: val_t val_t ;
        inline InterpolationCube2Dfirst (const val_t &) ;
        inline InterpolationCube2Dfirst () ;
        inline ~InterpolationCube2Dfirst () ;
        val_t operator () (coord_t) const ;
	void writeObject (ObjectStream &) const ;
	bool readObject (ObjectStream &) throw (ObjectStream :: EOFException) ;
      private :
        val_t _d0, _d1, _d2, _d3 ;
    } ;

  public :
    static const double GAMMA, g1, s2 ;
    static double CFL, ENDTIME, JUMP_H, JUMP_L ;
    static int STEPOUTPUT, MAXLEVEL, CHECKPOINTSTRIDE ;
  public :
    static void modifyGlobalConstants (const char *) ;
} ;

	// Diese Schablone erzeugt zu einem Interpolationsobjekt
	// (Typparameter) ein anderes, das die an der Integrations-
	// Fl"ache gespiegelten Geschwindigkeiten liefert, Dichte
	// und Energie unver"andert. Wird bei der Realisierung der
	// Reflektionsrandbedingung ben"otigt.

template < class A> class MirrorInterpolation : public A {
   typedef A interpolation_t ;
   typedef typename A :: val_t val_t ;
   typedef typename A :: coord_t coord_t ;
   const interpolation_t & _ip ;
   const double (&_nn)[3] ;
   public :
     inline MirrorInterpolation (const interpolation_t &, const double (&)[3]) ;
     inline ~MirrorInterpolation () ;
     inline val_t operator () (coord_t) const ;
} ;

template < class A > class SlipCalc : public EulerSolver {

	// Template zum Berechnen der Flu"sfunktion am Sliprand
	// Typargument ist die Schnittstellenklasse der Inter-
	// polationsobjekte.

  public :
    typedef typename A :: val_t val_t ;
    typedef typename A :: coord_t coord_t ;
    typedef A interpolation_t ;
    typedef struct a {
      const interpolation_t * _d ;
      a () { assert (0) ; }
      a (const interpolation_t & d) : _d (&d) {}
      const interpolation_t & operator * () const { return * _d ; }
    } arg_t ;
    inline val_t operator () (coord_t,const double (&)[3], const arg_t &) ;
} ;

	// Template zum Berechnen der Flu"sfunktion am Reflektionsrand
	// Typargument ist die Schnittstellenklasse der Inter-
	// polationsobjekte. Es gibt zwei spiegelbildliche Realisierungen,
	// die je nach Numerierungskonvention auf der Fl"ache verwendet
	// werden.

template < class A > class ReflectCalcLeft : public A {
  public :
    typedef typename A :: interpolation_t interpolation_t ;
    typedef typename A :: coord_t coord_t ;
    typedef typename A :: val_t val_t ;
    typedef typename SlipCalc < interpolation_t > :: arg_t arg_t ;
    inline val_t operator () (coord_t,const double (&)[3], const arg_t &) ;
} ;

template < class A > class ReflectCalcRight : public A {
  public :
    typedef typename A :: interpolation_t interpolation_t ;
    typedef typename A :: coord_t coord_t ;
    typedef typename A :: val_t val_t ;
    typedef typename SlipCalc < interpolation_t > :: arg_t arg_t ;
    inline val_t operator () (coord_t,const double (&)[3], const arg_t &) ;
} ;

	// Klasse f"ur das Funktionenobjekt zur Berechnung des
	// Betrags der Eigenvektoren (max. Zeitschrittweite).
	// Typparameter ist die Klasse des Interpolationsobjekts.

template < class A > class FabsEigenCalc : public EulerSolver {
  public :
    struct v {
      double x ;
      v (double y = 0.0) : x(y) {}
     ~v () {}
      const v & operator += (const v & y) { x += y.x ; return y ; }
      const v & operator *= (const double z) { x *= z ; return * this ; }
      operator double () const { return x ; }
    } ;
    typedef struct v val_t ;
    typedef typename A :: coord_t coord_t ;
    typedef typename SlipCalc < A > :: arg_t arg_t ;
    inline val_t operator () (coord_t,const double (&)[3],const arg_t &) ;
} ;

template < class A > class DifferenceCalc {

	// Klasse f"ur ein Funktionenobjekt zur Berechnung des
	// Dichtesprungs zwischen linker und rechter Fl"ache.
	// Typparameter ist die -*- Schnittstellenklasse -*- des
	// Interpolationsobjekts.

  public :
    typedef A interpolation_t ;
    typedef typename A :: coord_t coord_t ;
    struct arg {
      const interpolation_t * _a, * _b ;
      arg () : _a (0), _b (0) { assert (0) ; }
      arg (const interpolation_t & a, const interpolation_t & b) : _a (&a), _b (&b) { }
    } ;
    typedef struct arg arg_t ;
    typedef typename FabsEigenCalc < A > :: val_t val_t ;
    DifferenceCalc () {}
   ~DifferenceCalc () {}
    inline val_t operator () (coord_t, const double (&)[3], arg_t) ;
} ;

class FluxCalcSW : public EulerSolver {

	// Basisklasse mit den Kernroutinen der Steger-Warming Flussfunktion
	// die statische Methode sw_fluss (..) berechnet den Fluss mit links-
	// und rechtsseitigem Wert und der Normalen. R"uckgabeparameter ist
	// der berechnete Wert. Im wesentlichen aus der Arbeit von Monika
	// Wierse, s.a. swflux.cc.

  private :
    static int matrizen_third_vector(double [5],double [5], double *,double *,double,double,double) ;
    static int matrizen_fourth_vector(double [5],double [5],double *,double *, double,double,double) ;
    static int cplusvalues_con (const double [3], const double [5],double [5], double [3],double *,double *,double ,int *) ;
    static int cminusvalues_con (const double [3], const double *,double *, double *,double *,double *,double ,int *) ;
  protected :
    static void fluss (const double [5],const double [5], const double [3], double [5]) ;
  public :
    typedef class EulerSolver :: Vec5 val_t ;
    static const char * whatis () { return "steger - warming" ; }
} ;

class FluxCalcCube2DSW : public FluxCalcSW {
  public :
    typedef EulerSolver :: InterpolationCube2D interpolation_t ;
    typedef EulerSolver :: InterpolationCube2D :: coord_t coord_t ;
    typedef struct a {
      const interpolation_t * ul, * ur ;
      a () { assert(0) ; }
      a (const interpolation_t & l, const interpolation_t & r) : ul (&l), ur (&r) { }
      const interpolation_t & left () const { return * ul ; }
      const interpolation_t & right () const { return * ur ; }
    } arg_t ;
    inline val_t operator () (coord_t, const double (&nrm)[3], const arg_t & arg) const ;
} ;

class FluxCalcSimplex2DSW : public FluxCalcSW {
  public :
    typedef EulerSolver :: InterpolationSimplex2D interpolation_t ;
    typedef EulerSolver :: InterpolationSimplex2D :: coord_t coord_t ;
    typedef struct a {
      const interpolation_t * _ul, * _ur ;
      a () { assert (0) ; }
      a (const interpolation_t & l, const interpolation_t & r) : _ul (&l), _ur (&r) {}
      const interpolation_t & left () const { return * _ul ; }
      const interpolation_t & right () const { return * _ur ; }
    } arg_t ;
    inline val_t operator () (coord_t, const double (&)[3], const arg_t &) const ;
} ;

	//
	//    #    #    #  #          #    #    #  ######
	//    #    ##   #  #          #    ##   #  #
	//    #    # #  #  #          #    # #  #  #####
	//    #    #  # #  #          #    #  # #  #
	//    #    #   ##  #          #    #   ##  #
	//    #    #    #  ######     #    #    #  ######
	//


inline EulerSolver :: Vec5 :: Vec5 () {
  u [0] = .0 ;
  u [1] = .0 ;
  u [2] = .0 ;
  u [3] = .0 ;
  u [4] = .0 ;
  return ;
}

inline EulerSolver :: Vec5 :: Vec5 (double a, double b, double c, double d, double e) {
  u [0] = a ;
  u [1] = b ;
  u [2] = c ;
  u [3] = d ;
  u [4] = e ;
  return ;
}

inline EulerSolver :: Vec5 :: Vec5 (const Vec5 & x) {
  u [0] = x.u [0] ;
  u [1] = x.u [1] ;
  u [2] = x.u [2] ;
  u [3] = x.u [3] ;
  u [4] = x.u [4] ;
  return ;
}

inline EulerSolver :: Vec5 EulerSolver :: Vec5 :: operator + (const Vec5 & x) const {
  return Vec5 (u [0] + x.u [0], u [1] + x.u [1], u [2] + x.u [2], u [3] + x.u [3], u [4] + x.u [4]) ;
}

inline EulerSolver :: Vec5 EulerSolver :: Vec5 :: operator - (const Vec5 & x) const {
  return Vec5 (u [0] - x.u [0], u [1] - x.u [1], u [2] - x.u [2], u [3] - x.u [3], u [4] - x.u [4]) ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: Vec5 :: operator += (const Vec5 & y) {
  u [0] += y.u [0] ;
  u [1] += y.u [1] ;
  u [2] += y.u [2] ;
  u [3] += y.u [3] ;
  u [4] += y.u [4] ;
  return y ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: Vec5 :: operator -= (const Vec5 & y) {
  u [0] -= y.u [0] ;
  u [1] -= y.u [1] ;
  u [2] -= y.u [2] ;
  u [3] -= y.u [3] ;
  u [4] -= y.u [4] ;
  return y ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: Vec5 :: operator *= (const double x) {
  u [0] *= x ;
  u [1] *= x ;
  u [2] *= x ;
  u [3] *= x ;
  u [4] *= x ;
  return * this ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: Vec5 :: operator /= (const double x) {
  u [0] /= x ;
  u [1] /= x ;
  u [2] /= x ;
  u [3] /= x ;
  u [4] /= x ;
  return * this ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: Vec5 :: operator = (const Vec5 & y) {
  u [0] = y.u [0] ;
  u [1] = y.u [1] ;
  u [2] = y.u [2] ;
  u [3] = y.u [3] ;
  u [4] = y.u [4] ;
  return y ;
}

inline double EulerSolver :: Vec5 :: operator [] (int i) const {
  return u [i] ;
}

inline double & EulerSolver :: Vec5 :: operator [] (int i) {
  return u [i] ;
}

inline double EulerSolver :: Vec5 ::  rho () const {
  return u [0] ;
}

inline double EulerSolver :: Vec5 :: pressure () const {
  return g1 * (u [4] - 0.5 / u [0] * (u [1] * u [1] + u [2] * u [2] + u [3] * u [3] )) ;
}

inline double EulerSolver :: Vec5 :: vdot (const double (&x)[3]) const {
  return u[1] * x[0] / u[0] + u[2] * x [1] / u[0] + u[3] * x[2] / u[0] ;
}

inline EulerSolver :: Vec5 :: operator double * () {
  return u ;
}

inline EulerSolver :: Vec5 :: operator const double * () const {
  return u ;
}

inline EulerSolver :: EulerData :: EulerData () : _a (), _b () {
  return ;
}

inline double EulerSolver :: EulerData :: pressure () const {
  return _a.pressure () ;
}

inline double EulerSolver :: EulerData :: vdot (const double (&x)[3]) const {
  return _a.vdot (x) ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: EulerData :: initial (const Vec5 & x) {
  return (_a = x) ;
}

inline void EulerSolver :: EulerData :: print (ostream & out) const {
  out << _a ;
  return ;
}

inline void EulerSolver :: EulerData :: read (istream & in) {
  in >> _a ;
  return ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: EulerData :: add (const Vec5 & u) {
  _b += u ;
  return u ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: EulerData :: sub (const Vec5 & u) {
  _b -= u ;
  return u ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: EulerData :: ucon () const {
  return _a ;
}

inline const EulerSolver :: Vec5 & EulerSolver :: EulerData :: uupd () const {
  return _b ;
}

inline double EulerSolver :: EulerData :: rho () const {
  return _a.rho () ;
}

inline double EulerSolver :: EulerData :: energy () const {
  return _a [4] ;
}

inline void EulerSolver :: EulerData :: update (double deltat, double volume) {
  _a -= (_b *= (deltat / volume)) ;
  _b = Vec5 () ;
}

inline EulerSolver :: InterpolationCube2Dzero :: InterpolationCube2Dzero (const Vec5 & u) : _base (u) {
  assert (u.rho () > 0.0) ;
  assert (u.pressure () > 0.0) ;
  return ;
}

inline EulerSolver :: InterpolationCube2Dzero :: InterpolationCube2Dzero (const EulerData & d) : _base (d.ucon ()) {
  return ;
}

inline EulerSolver :: InterpolationCube2Dzero :: InterpolationCube2Dzero () : _base () {
  return ;
}

inline EulerSolver :: InterpolationCube2Dzero :: ~InterpolationCube2Dzero () {
  return ;
}

inline EulerSolver :: Vec5 EulerSolver :: InterpolationCube2Dzero :: operator () (const double (&x)[2]) const {
  return _base ;
}

inline EulerSolver :: Vec5 EulerSolver :: InterpolationCube2Dzero :: operator () (const double (&bary)[3]) const {
  return _base ;
}

inline EulerSolver :: InterpolationSimplex2Dfirst :: InterpolationSimplex2Dfirst (const val_t & u) : _d0(u), _d1(u), _d2(u) {
  return ;
}

inline EulerSolver :: InterpolationSimplex2Dfirst :: InterpolationSimplex2Dfirst () : _d0 (), _d1 (), _d2 ()  {
  return ;
}

inline EulerSolver :: InterpolationSimplex2Dfirst :: ~InterpolationSimplex2Dfirst () {
  return ;
}

inline EulerSolver :: InterpolationCube2Dfirst :: InterpolationCube2Dfirst (const Vec5 & u) : _d0(u), _d1(u), _d2(u), _d3(u) {
  return ;
}

inline EulerSolver :: InterpolationCube2Dfirst :: InterpolationCube2Dfirst () : _d0 (), _d1 (), _d2 (), _d3 () {
  return ;
}

inline EulerSolver :: InterpolationCube2Dfirst :: ~InterpolationCube2Dfirst () {
  return ;
}

template < class A > inline MirrorInterpolation < A > :: MirrorInterpolation (const interpolation_t & ip, const double (&n)[3]) : _ip (ip), _nn (n) {
  return ;
}

template < class A > inline MirrorInterpolation < A > :: ~MirrorInterpolation () {
  return ;
}

template < class A > inline  MirrorInterpolation < A > :: val_t MirrorInterpolation < A > :: operator () (coord_t x) const {
  double n2 = _nn [0] * _nn [0] + _nn [1] * _nn [1] + _nn [2] * _nn [2] ;
  double y = 2.0 / n2 * _ip (x).vdot (_nn) ;
  return val_t (_ip (x)[0], _ip (x)[1] - y * _nn [0], _ip (x)[2] - y * _nn [1], _ip (x)[3] - y * _nn [2], _ip (x)[4]) ;
}

template < class A > inline SlipCalc < A > :: val_t SlipCalc < A > :: operator () (coord_t x, const double (&n)[3], const arg_t & arg) {
  return val_t (0.0, n [0] * (*arg)(x).pressure (), n [1] * (*arg)(x).pressure (), n [2] * (*arg)(x).pressure (), 0.0) ;
}

template < class A > inline ReflectCalcLeft < A > :: val_t ReflectCalcLeft < A > :: operator () (coord_t p, const double (&n)[3], const arg_t & arg) {
  return A :: operator () (p, n, A :: arg_t (*arg, MirrorInterpolation < interpolation_t > (*arg, n))) ;
}

template < class A > inline ReflectCalcRight < A > :: val_t ReflectCalcRight < A > :: operator () (coord_t p, const double (&n)[3], const arg_t & arg) {
  return A :: operator () (p, n, A :: arg_t (MirrorInterpolation < interpolation_t > (*arg, n), *arg)) ;
}

template < class A > DifferenceCalc < A > :: val_t DifferenceCalc < A > :: operator () (coord_t x, const double (&)[3], arg_t d) {
  return fabs ((*d._a)(x)[0] - (*d._b)(x)[0]) + fabs ((*d._a)(x)[1] - (*d._b)(x)[1]) + fabs ((*d._a)(x)[2] - (*d._b)(x)[2]) 
		+ fabs ((*d._a)(x)[3] - (*d._b)(x)[3]) + fabs ((*d._a)(x)[4] - (*d._b)(x)[4]) ;
}

template < class A > inline FabsEigenCalc < A > :: val_t FabsEigenCalc < A > :: operator () (coord_t x, const double (&n)[3], const arg_t & arg) {
  return sqrt (GAMMA * (*arg)(x).pressure () / (*arg)(x).rho () * (n [0] * n [0] + n [1] * n [1] + n [2] * n [2])) + fabs ((*arg)(x).vdot (n)) ;
}

inline FluxCalcCube2DSW :: val_t FluxCalcCube2DSW :: operator () (coord_t x, const double (&nrm)[3], const arg_t & arg) const {
  val_t res ;
  fluss (arg.left ()(x), arg.right ()(x), nrm, res) ;
  return res ;
}

inline FluxCalcSimplex2DSW :: val_t FluxCalcSimplex2DSW :: operator () (coord_t x, const double (&nrm)[3], const arg_t & arg) const {
  val_t res ;
  fluss (arg.left ()(x), arg.right ()(x), nrm, res) ;
  return res ;
}

#endif