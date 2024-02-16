	// (c) bernhard schupp 1997 - 1998
	
	// $Source: /home/dfs/yc20/yc23/3/hier/optimized/RCS/gitter_euler_impl.cc,v $
	// $Revision: 2.4 $
	// $Name:  $
	// $State: Exp $
	
#include <stdio.h>
#include <time.h>
#include <iostream.h>
#include <fstream.h>

#include "euler.h"
#include "mapp_cube_3d.h"
#include "mapp_tetra_3d.h"
#include "gitter_euler_impl.h"

static volatile char RCSId_gitter_euler_impl_cc [] = "$Id: gitter_euler_impl.cc,v 2.4 1998/12/02 08:55:54 yc23 Exp yc23 $" ;

const EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd3 :: outer = EulerSolver :: Vec5 ( 1.4, 4.2, 0.0, 0.0, 8.8 ) ;
const EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd4 :: outer = EulerSolver :: Vec5 ( 1.4, 4.2, 0.0, 0.0, 8.8 ) ;

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: Hface3Euler :: euler3dCalcFlux (double time) {
  return nb.front ().first->euler3dCalcFlux (time, this, nb.rear(), nb.front ().second) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: Hface3Euler :: euler3dJumpCriterion () {
  return nb.front ().first->euler3dJumpCriterion (this, nb.rear (), nb.front ().second) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: Hface3Euler :: euler3dCalcFlux (double, const EulerSolver :: InterpolationSimplex2D & ul, const EulerSolver :: InterpolationSimplex2D & ur) const {
  return quadraturTriang2D < fluxCalcSimplex2D_t > (LinearSurfaceMapping(myvertex (0)->Point(),
  	myvertex (1)->Point(), myvertex (2)->Point()))
	.integrate1 (EulerSolver :: Vec5 (.0,.0,.0,.0,.0), fluxCalcSimplex2D_t :: arg_t (ul,ur)) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: Hface3Euler :: euler3dJumpCriterion (const EulerSolver :: InterpolationSimplex2D & ul, const EulerSolver :: InterpolationSimplex2D & ur) {
  double jump (quadraturTriang2D < differenceSimplex2D_t > (LinearSurfaceMapping(myvertex (0)->Point(),
  	myvertex (1)->Point(), myvertex (2)->Point())).integrate1 (0.0, differenceSimplex2D_t:: arg_t (ul,ur))) ;
  return (EulerSolver :: JUMP_H < jump) ? ((level () < EulerSolver :: MAXLEVEL) ? EulerSolver :: ref : EulerSolver :: none) 
                                        : ((jump < EulerSolver :: JUMP_L) ? EulerSolver :: crs : EulerSolver :: none) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: Hface4Euler :: euler3dCalcFlux (double time) {
  return nb.front ().first->euler3dCalcFlux (time, this, nb.rear(), nb.front ().second) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: Hface4Euler :: euler3dJumpCriterion () {
  return nb.front ().first->euler3dJumpCriterion (this, nb.rear (), nb.front ().second) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: Hface4Euler :: euler3dCalcFlux (double, const EulerSolver :: InterpolationCube2D & ul, const EulerSolver :: InterpolationCube2D & ur) const {
  return QuadraturCube2D < fluxCalcCube2D_t > (BilinearSurfaceMapping(myvertex (0)->Point(), myvertex (1)->Point(),
	 myvertex (2)->Point(), myvertex (3)->Point()))
	 .integrate1 (EulerSolver :: Vec5 (.0,.0,.0,.0,.0), fluxCalcCube2D_t :: arg_t (ul,ur)) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: Hface4Euler :: euler3dJumpCriterion (const EulerSolver :: InterpolationCube2D & ul, const EulerSolver :: InterpolationCube2D & ur) {
  double jump (QuadraturCube2D < differenceCube2D_t > (BilinearSurfaceMapping(myvertex (0)->Point(), myvertex (1)->Point(),
	 			myvertex (2)->Point(), myvertex (3)->Point())).integrate1 (0.0,differenceCube2D_t:: arg_t (ul,ur))) ;
  return (EulerSolver :: JUMP_H < jump) ? ((level () < EulerSolver :: MAXLEVEL) ? EulerSolver :: ref : EulerSolver :: none) 
                                        : ((jump < EulerSolver :: JUMP_L) ? EulerSolver :: crs : EulerSolver :: none) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: EulerBnd3 :: euler3dCalcFlux (double, hface3 *,pair < hasFace3 *,int > p,int) {
  return p.first->euler3dCalcFluxBnd (pair < hasFace3 *, int > (InternalHasFace3 ()(this), 0)) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: EulerBnd3 :: euler3dCalcFluxBnd (pair < hasFace3 *,int >) {
  return (abort (), EulerSolver :: Vec5 ()) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: EulerBnd3 :: euler3dJumpCriterion (hface3 *, pair < hasFace3 *, int >, int) {
  return EulerSolver :: crs ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: EulerBnd3 :: euler3dJumpCriterion (hface3 *,const EulerSolver :: InterpolationSimplex2D &,int) {
  return EulerSolver :: crs ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: EulerBnd3 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D &) {
  return (abort (), EulerSolver :: Vec5 ()) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: EulerBnd4 :: euler3dCalcFlux (double, hface4 *,pair < hasFace4 *,int > p, int) {
  return p.first->euler3dCalcFluxBnd (pair < hasFace4 *, int > (InternalHasFace4 ()(this), 0)) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: EulerBnd4 :: euler3dCalcFluxBnd (pair < hasFace4 *,int >) {
  return (abort (), EulerSolver :: Vec5 ()) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: EulerBnd4 :: euler3dJumpCriterion (hface4 *, pair < hasFace4 *, int >, int) {
  return EulerSolver :: crs ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: EulerBnd4 :: euler3dJumpCriterion (hface4 *,const EulerSolver :: InterpolationCube2D &,int) {
  return EulerSolver :: crs ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: EulerBnd4 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D &) {
  return (abort (), EulerSolver :: Vec5 ()) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd3 :: euler3dCalcFlux (double time, hface3 * face, pair < hasFace3 *, int > p,int) {
  EulerSolver :: InterpolationCube2Dzero ip (outer) ;
  const EulerSolver :: InterpolationSimplex2D & ipr (ip) ;
  return p.first->euler3dCalcFlux (time, face, ipr, p.second) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd3 :: euler3dCalcFlux (double time, hface3 * face, const EulerSolver :: InterpolationSimplex2D & inner, int) {
  EulerSolver :: InterpolationCube2Dzero ip (outer) ;
  const EulerSolver :: InterpolationSimplex2D & ipr (ip) ;
  return face->euler3dCalcFlux (time, inner, ipr) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd3 :: euler3dCalcFluxBnd (pair < hasFace3 *,int > p) {
  return EulerBnd3 :: euler3dCalcFluxBnd (p) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd3 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D & x) {
  return EulerBnd3 :: euler3dCalcFluxBnd (x) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd4 :: euler3dCalcFlux (double time, hface4 * face, pair < hasFace4 *, int > p, int) {
  return p.first->euler3dCalcFlux (time, face, EulerSolver :: InterpolationCube2Dzero (outer), p.second) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd4 :: euler3dCalcFlux (double time, hface4 * face, const EulerSolver :: InterpolationCube2D & inner, int) {
  return face->euler3dCalcFlux (time, inner, EulerSolver :: InterpolationCube2Dzero (outer)) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd4 :: euler3dCalcFluxBnd (pair < hasFace4 *,int > p) {
  return EulerBnd4 :: euler3dCalcFluxBnd (p) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: InflowBnd4 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D & x) {
  return EulerBnd4 :: euler3dCalcFluxBnd (x) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: OutflowBnd3 :: euler3dCalcFlux (double t, hface3 * f, pair < hasFace3 *, int> p, int i) {
  return EulerBnd3 :: euler3dCalcFlux (t,f,p,i) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: OutflowBnd3 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D & d) {
  return quadraturTriang2D < fluxCalcSimplex2D_t > 
    (LinearSurfaceMapping(myhface3(0)->myvertex(0)->Point(), myhface3(0)->myvertex(1)->Point(),
    myhface3(0)->myvertex(2)->Point())).integrate1 (EulerSolver :: Vec5 (), fluxCalcSimplex2D_t :: arg_t(d,d)) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: OutflowBnd3 :: euler3dCalcFluxBnd (pair < hasFace3 *,int > p) {
  return EulerBnd3 :: euler3dCalcFluxBnd (p) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: OutflowBnd3 :: euler3dCalcFlux (double, hface3 *,const EulerSolver :: InterpolationSimplex2D & d,int) {
  return euler3dCalcFluxBnd (d) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: OutflowBnd4 :: euler3dCalcFlux (double t, hface4 * f, pair < hasFace4 *, int> p, int i) {
  return EulerBnd4 :: euler3dCalcFlux (t,f,p,i) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: OutflowBnd4 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D & d) {
  return QuadraturCube2D < fluxCalcCube2D_t > 
    (BilinearSurfaceMapping(myhface4(0)->myvertex(0)->Point(), myhface4(0)->myvertex(1)->Point(),
    myhface4(0)->myvertex(2)->Point(), myhface4(0)->myvertex(3)->Point()))
    .integrate1 (EulerSolver :: Vec5 (.0,.0,.0,.0,.0), fluxCalcCube2D_t :: arg_t(d,d)) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: OutflowBnd4 :: euler3dCalcFluxBnd (pair < hasFace4 *,int > p) {
  return EulerBnd4 :: euler3dCalcFluxBnd (p) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: OutflowBnd4 :: euler3dCalcFlux (double, hface4 *,const EulerSolver :: InterpolationCube2D & d,int) {
  return euler3dCalcFluxBnd (d) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: SlipBnd3 :: euler3dCalcFlux (double t, hface3 * f, pair < hasFace3 *, int> p, int i) {
  return EulerBnd3 :: euler3dCalcFlux (t,f,p,i) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: SlipBnd3 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D & d) {
  return quadraturTriang2D < slipCalcSimplex2D_t > (LinearSurfaceMapping
    (myhface3(0)->myvertex(0)->Point(), myhface3(0)->myvertex(1)->Point(), myhface3(0)->myvertex(2)->Point()))
    .integrate1 (EulerSolver :: Vec5 (.0,.0,.0,.0,.0), d) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: SlipBnd3 :: euler3dCalcFlux (double ,hface3 *,const EulerSolver :: InterpolationSimplex2D & d,int) {
  return euler3dCalcFluxBnd (d) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: SlipBnd3 :: euler3dCalcFluxBnd (pair < hasFace3 *,int > p) {
  return EulerBnd3 :: euler3dCalcFluxBnd (p) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: SlipBnd4 :: euler3dCalcFlux (double t, hface4 * f, pair < hasFace4 *, int> p, int i) {
  return EulerBnd4 :: euler3dCalcFlux (t,f,p,i) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: SlipBnd4 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D & d) {
  return QuadraturCube2D < slipCalcCube2D_t > 
    (BilinearSurfaceMapping(myhface4(0)->myvertex(0)->Point(), myhface4(0)->myvertex(1)->Point(),
    myhface4(0)->myvertex(2)->Point(), myhface4(0)->myvertex(3)->Point())).integrate1 (EulerSolver :: Vec5 (.0,.0,.0,.0,.0), d) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: SlipBnd4 :: euler3dCalcFlux (double ,hface4 *,const EulerSolver :: InterpolationCube2D & d,int) {
  return euler3dCalcFluxBnd (d) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: SlipBnd4 :: euler3dCalcFluxBnd (pair < hasFace4 *,int > p) {
  return EulerBnd4 :: euler3dCalcFluxBnd (p) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: ReflectBnd3 :: euler3dCalcFlux (double t,hface3 * f, pair < hasFace3 *, int> p, int i) {
  return EulerBnd3 :: euler3dCalcFlux (t,f,p,i) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: ReflectBnd3 :: euler3dCalcFluxBnd (pair < hasFace3 *,int > p) {
  return EulerBnd3 :: euler3dCalcFluxBnd (p) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: ReflectBnd3 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D & d) {
  return quadraturTriang2D < ReflectCalcRight < fluxCalcSimplex2D_t > > 
    (LinearSurfaceMapping(myhface3(0)->myvertex(0)->Point(), myhface3(0)->myvertex(1)->Point(),myhface3(0)->myvertex(2)->Point()))
    .integrate1 (EulerSolver :: Vec5 (.0,.0,.0,.0,.0), typename ReflectCalcRight < fluxCalcSimplex2D_t > :: arg_t (d)) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: ReflectBnd3 :: euler3dCalcFlux (double, hface3 *,const EulerSolver :: InterpolationSimplex2D & d,int) {
  return quadraturTriang2D < ReflectCalcLeft < fluxCalcSimplex2D_t > > 
    (LinearSurfaceMapping(myhface3(0)->myvertex(0)->Point(),myhface3(0)->myvertex(1)->Point(),myhface3(0)->myvertex(2)->Point()))
    .integrate1 (EulerSolver :: Vec5 (.0,.0,.0,.0,.0), typename ReflectCalcLeft < fluxCalcSimplex2D_t > :: arg_t (d)) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: ReflectBnd4 :: euler3dCalcFlux (double t,hface4 * f, pair < hasFace4 *, int> p, int i) {
  return EulerBnd4 :: euler3dCalcFlux (t,f,p,i) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: ReflectBnd4 :: euler3dCalcFluxBnd (pair < hasFace4 *,int > p) {
  return EulerBnd4 :: euler3dCalcFluxBnd (p) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: ReflectBnd4 :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D & d) {
  return QuadraturCube2D < ReflectCalcRight < fluxCalcCube2D_t > > (BilinearSurfaceMapping
  (myhface4(0)->myvertex(0)->Point(), myhface4(0)->myvertex(1)->Point(),myhface4(0)->myvertex(2)->Point(),myhface4(0)->myvertex(3)->Point()))
    .integrate1 (EulerSolver :: Vec5 (.0,.0,.0,.0,.0), typename ReflectCalcRight < fluxCalcCube2D_t > :: arg_t (d)) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: ReflectBnd4 :: euler3dCalcFlux (double, hface4 *,const EulerSolver :: InterpolationCube2D & d,int) {
  return QuadraturCube2D < ReflectCalcLeft < fluxCalcCube2D_t > > (BilinearSurfaceMapping 
  (myhface4(0)->myvertex(0)->Point(), myhface4(0)->myvertex(1)->Point(),myhface4(0)->myvertex(2)->Point(),myhface4(0)->myvertex(3)->Point()))
    .integrate1 (EulerSolver :: Vec5 (.0,.0,.0,.0,.0), typename ReflectCalcLeft < fluxCalcCube2D_t > :: arg_t (d)) ;
}

GitterEuler :: ObjectsEuler ::  TetraEuler :: TetraEuler (hface3_GEO * f0, int t0, hface3_GEO * f1, int t1, hface3_GEO * f2, int t2,
	hface3_GEO * f3, int t3): Gitter :: Geometric :: tetra_GEO (f0,t0,f1,t1,f2,t2,f3,t3), _volume (quadraturTetra3D < VolumeCalc > 
	(LinearMapping (myvertex(0)->Point(), myvertex(1)->Point(), myvertex(2)->Point(), myvertex(3)->Point())).integrate1 (0.0)) {
  return ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: TetraEuler :: euler3dCalcFlux (double time, hface3 * face, pair < hasFace3 *,int > p,int) {
  if (!leaf ()) abort () ;
  EulerSolver :: InterpolationCube2Dzero ip (_data.ucon ()) ;
  const EulerSolver :: InterpolationSimplex2D & ipr (ip) ;
  return _data.add (p.first->euler3dCalcFlux (time, face, ipr, p.second)) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: TetraEuler :: euler3dJumpCriterion (hface3 * face, pair < hasFace3 *, int > p,int) {
  EulerSolver :: InterpolationCube2Dzero ip (_data.ucon ()) ;
  const EulerSolver :: InterpolationSimplex2D & ipr (ip) ;
  EulerSolver :: req_t r = p.first->euler3dJumpCriterion (face, ipr, p.second) ;
  setRequest (r) ;
  return r ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: TetraEuler :: euler3dCalcFlux (double time, hface3 * face, const EulerSolver :: InterpolationSimplex2D & ul, int i) {
  if (!leaf ()) abort () ;
  return _data.sub (face->euler3dCalcFlux (time, ul, EulerSolver :: InterpolationCube2Dzero (_data.ucon ()))) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: TetraEuler :: euler3dJumpCriterion (hface3 * face, const EulerSolver :: InterpolationSimplex2D & ul, int i) {
  EulerSolver :: req_t r = face->euler3dJumpCriterion (ul,EulerSolver :: InterpolationCube2Dzero (_data.ucon ())) ;
  setRequest (r) ;
  return r ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: TetraEuler :: euler3dCalcFluxBnd (pair < hasFace3 *,int > p) {
  if (!leaf ()) abort () ;
  return _data.sub (p.first->euler3dCalcFluxBnd (EulerSolver :: InterpolationCube2Dzero (_data.ucon ()))) ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: TetraEuler :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationSimplex2D &) {
  return (abort (), EulerSolver :: Vec5 ()) ;
}

double GitterEuler :: ObjectsEuler :: TetraEuler ::  euler3dUpdateData (double deltat, double time, double cfl) {
  request (myrule_t :: crs) ;
  _tag = true ;
  _data.update (deltat, volume ()) ;
  (_data.rho() < .0) ? (_data.print (cerr), abort(), 0) : 0 ;
  (_data.pressure() < .0) ? (_data.print (cerr), abort(), 0) : 0 ;
  double maxfabseigen = .0 ;
  for(int i = 0 ; i < 4 ; i++) {
    double fabseigen = quadraturTriang2D < fabsEigenCalcSimplex2D_t > 
	(LinearSurfaceMapping(myhface3 (i)->myvertex (0)->Point (), 
	myhface3 (i)->myvertex (1)->Point (),myhface3 (i)->myvertex (2)->Point ()))
	.integrate1 (0.0, EulerSolver :: InterpolationCube2Dzero (_data.ucon ())) ;
    maxfabseigen = fabseigen > maxfabseigen ? fabseigen : maxfabseigen  ;
  }
  return cfl * volume () / maxfabseigen ;
}

GitterEuler :: ObjectsEuler ::  HexaEuler :: HexaEuler (hface4_GEO * f0, int t0, hface4_GEO * f1, int t1,
	hface4_GEO * f2, int t2, hface4_GEO * f3, int t3, hface4_GEO * f4, int t4, hface4_GEO * f5, int t5)
  : Gitter :: Geometric :: hexa_GEO (f0,t0,f1,t1,f2,t2,f3,t3,f4,t4,f5,t5), _volume (QuadraturCube3D < VolumeCalc > 
  (TrilinearMapping (myvertex(0)->Point(), myvertex(1)->Point(), myvertex(2)->Point(), myvertex(3)->Point(),
  myvertex(4)->Point(), myvertex(5)->Point(), myvertex(6)->Point(), myvertex(7)->Point())).integrate2 (0.0)) {
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: HexaEuler :: euler3dCalcFlux (double time, hface4 * face, pair < hasFace4 *,int > p, int ) {
  if (!leaf ()) abort () ;
  return _data.add (p.first->euler3dCalcFlux (time, face, EulerSolver :: InterpolationCube2Dzero (_data.ucon ()), p.second)) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: HexaEuler :: euler3dJumpCriterion (hface4 * face, pair < hasFace4 *, int > p, int) {
  EulerSolver :: req_t r = p.first->euler3dJumpCriterion (face, EulerSolver :: InterpolationCube2Dzero (_data.ucon ()), p.second) ; 
  setRequest (r) ;
  return r ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: HexaEuler :: euler3dCalcFlux (double time, hface4 * face, const EulerSolver :: InterpolationCube2D & ul, int i) {
  if (!leaf ()) abort () ;
  return _data.sub (face->euler3dCalcFlux (time, ul, EulerSolver :: InterpolationCube2Dzero (_data.ucon ()))) ;
}

EulerSolver :: req_t GitterEuler :: ObjectsEuler :: HexaEuler :: euler3dJumpCriterion (hface4 * face, const EulerSolver :: InterpolationCube2D & ul, int i) {
  EulerSolver :: req_t r = face->euler3dJumpCriterion (ul,EulerSolver :: InterpolationCube2Dzero (_data.ucon ())) ;
  setRequest (r) ;
  return r ;
}

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: HexaEuler :: euler3dCalcFluxBnd (pair < hasFace4 *,int > p) {
  if (!leaf ()) abort () ;
  return _data.sub (p.first->euler3dCalcFluxBnd (EulerSolver :: InterpolationCube2Dzero (_data.ucon ()))) ;
} 

EulerSolver :: Vec5 GitterEuler :: ObjectsEuler :: HexaEuler :: euler3dCalcFluxBnd (const EulerSolver :: InterpolationCube2D &) {
  return (abort (), EulerSolver :: Vec5 ()) ;
}

double GitterEuler :: ObjectsEuler :: HexaEuler ::  euler3dUpdateData (double deltat, double time, double cfl) {
  request (myrule_t :: crs) ;
  _tag = true ;
  _data.update (deltat, volume ()) ;
  (_data.rho() < .0) ? (_data.print (cerr), abort(), 0) : 0 ;
  (_data.pressure() < .0) ? (_data.print (cerr), abort(), 0) : 0 ;
  double maxfabseigen = .0 ;
  for(int i = 0 ; i < 6 ; i++) {
    double fabseigen = QuadraturCube2D < fabsEigenCalcCube2D_t > 
	(BilinearSurfaceMapping(myhface4(i)->myvertex(0)->Point(), myhface4(i)->myvertex(1)->Point(),
	myhface4(i)->myvertex(2)->Point(), myhface4(i)->myvertex(3)->Point()))
	.integrate1 (0.0, EulerSolver :: InterpolationCube2Dzero (_data.ucon ())) ;
    maxfabseigen = fabseigen > maxfabseigen ? fabseigen : maxfabseigen  ;
  }
  return cfl * volume () / maxfabseigen ;
}

GitterEuler :: GitterEuler (const char * file) : _macrogitter (0), _time (.0), _deltat (.0), _step (0) {
  assert (debugOption (10) ? (cout << " GitterEuler :: GitterEuler (const char *) : \"offnet < " << file << " > " << endl, 1) : 1) ;
  ifstream in (file) ;
  if (in) {
    _macrogitter = new MacrogitterEuler (in) ;
  } else {
    cerr << "**WARNUNG (IGNORIERT) GitterEuler :: GitterEuler (const char *) : Datei < " ;
    cerr << file << " > konnte nicht g\"offnet werden. Leeres Gitter erzeugt." << endl ;
    _macrogitter = new MacrogitterEuler () ;
  }
  assert (_macrogitter) ;
  notifyMacroGridChanges () ;
  return ;
}

GitterEuler :: GitterEuler () : _macrogitter (0), _time (.0), _deltat (.0), _step (0) {
  _macrogitter = new MacrogitterEuler () ;
  assert (_macrogitter) ;
  notifyMacroGridChanges () ;
}

GitterEuler :: ~GitterEuler () {
  delete _macrogitter ;
}

double GitterEuler :: addUpdate (double cfl) {
  LeafIterator < helement_STI > w (*this) ;
  double y = 1000.0 ;
  for( w->first() ; ! w->done() ; w->next()) {
    double x = w->item().euler3dUpdateData (deltat (), time (), cfl) ;
    y = y < x ? y : x ;
  }
  return y ;
}

bool GitterEuler :: estimateRefinement () {

	// Die bool Variable action gibt an, ob das adapt () aufgerufen
	// werden soll (nachfolgend). Adaption geschieht immer dann,
	// wenn mind. ein Element zum Verfeinern markiert wurde.
	// Damit bei fehlender Verfeinerung eine eventuell n"otige
	// Vergr"oberung auch stattfindet, wird nach jedem
	// zehnten Zeitschritt das Gitter unbedingt angepasst.

  bool action ((step () % 10 == 0) ? true : false) ;
  LeafIterator < hface_STI > w (*this) ;
  for (w->first () ; ! w->done () ; w->next ())
    action |= w->item ().euler3dJumpCriterion () == EulerSolver :: ref ? true : false ;
  return action ;
}

void GitterEuler :: calcFlux () {
  LeafIterator < hface_STI > w (*this) ;
  for(w->first() ; ! w->done() ; w->next()) w->item().euler3dCalcFlux (time()) ;
  return ;
}

void GitterEuler :: initialConstValue (const EulerSolver :: Vec5 & u) {
  LeafIterator < helement > w (*this) ;
  for (w->first () ; ! w->done () ; w->next ()) w->item ().accessEulerData ().initial (u) ;
  return ;
}

void GitterEuler :: backupData (ostream & out) {
  LeafIterator < helement_STI > w (*this) ;
  for(w->first() ; ! w->done() ; w->next()) w->item().accessEulerData ().print (out) ;
  out << setprecision (14) << _time << endl ;
  if (out.fail ()) {
    cerr << "**WARNUNG (IGNORIERT) Fehler beim Schreiben der Daten in " << __FILE__ << " Zeile " << __LINE__ << endl ;
  }
  return ;
}

void GitterEuler :: backupCMode (ostream & out) {
  Gitter :: backupCMode (out) ;
  if (out.fail ()) {
    cerr << "**WARNUNG (IGNORIERT) Fehler beim Schreiben des Gitters in " << __FILE__ << " Zeile " << __LINE__ << endl ;
  }
  backupData (out) ;
  return ;
}

void GitterEuler :: backupCMode (const char * fileName) {
  Gitter :: backupCMode (fileName) ;
  return ;
}

void GitterEuler :: backup (ostream & out) {

	// Erkl"arung siehe Gitter :: backup (const char *) in gitter_sti.cc ;

  Gitter :: backup (out) ;
  backupData (out) ;
  return ;
}

void GitterEuler :: backup (const char * fileName) {

	// Erkl"arung siehe Gitter :: backup (const char *) in gitter_sti.cc ;

  Gitter :: backup (fileName) ;
  return ;
}

void GitterEuler :: restoreData (istream & in) { 
  LeafIterator < helement_STI > w (*this) ;
  for (w->first () ; ! w->done () ; w->next ()) w->item ().accessEulerData ().read (in) ;
  in >> _time ;
  if (in.fail ()) {
    cerr << "**WARNUNG (IGNORIERT) Fehler beim Einlesen der Daten in " << __FILE__ << " Zeile " << __LINE__ << endl ;
  }
  return ;
}

void GitterEuler :: restore (istream & in) {

	// Erkl"arung siehe Gitter :: backup (const char *) in gitter_sti.cc ;

  Gitter :: restore (in) ;
  if (in.fail ()) {
    cerr << "**FEHLER (FATAL) Einlesen des Gitters hat nicht geklappt " << __FILE__ << " Zeile " << __LINE__ << endl ;
    abort () ;
  }
  restoreData (in) ;
  return ;
}

void GitterEuler :: restore (const char * fileName) {

	// Erkl"arung siehe Gitter :: backup (const char *) in gitter_sti.cc ;

  Gitter :: restore (fileName) ;
  return ;
}

void GitterEuler :: oneTimeStep (double cfl, bool verboseFlag) {

	// oneTimeStep (..) ist der Zeitschrittreiber f"ur das explizite
	// Verfahren. Er berechnet zun"achst aus den alten Daten das Ver-
	// feinerungskriterium, f"uhrt dan die Gitteranpassung durch und
	// erzeugt auf den Elementen dann die Daten des neuen Zeitschritts.

  long start = clock () ;
  _time += deltat () ;
  bool ac = estimateRefinement () ;
  long l1 = clock () ;
  if (ac) adapt () ;
  long l2 = clock () ;
  calcFlux () ;
  long l3 = clock () ;
  _deltat = addUpdate (cfl) ;
  long end = clock () ;
  
  float u1 = (float)(l1 - start)/(float)(CLOCKS_PER_SEC) ;
  float u2 = (float)(l2 - l1)/(float)(CLOCKS_PER_SEC) ;
  float u3 = (float)(l3 - l2)/(float)(CLOCKS_PER_SEC) ;
  float u4 = (float)(end - l3)/(float)(CLOCKS_PER_SEC) ;
  float all = (float)(end - start)/(float)(CLOCKS_PER_SEC) ;
  if (verboseFlag) {
    cout.precision (7) ;
    cout << " [step|time|deltat] " << step () << " " << time () << " " << deltat () ;
    cout.precision (3) ;
    cout << " [est|adp|flx|upd|all]  " << u1 << " " << u2 << " " << u3 << " " << u4 << " " << all << endl ;
  }
  _step ++ ;
  return ;
}

void GitterEuler :: runEulerSolver (double endTime, double cfl, int backupStride, const char * outFile, int checkpointStride) {

	// Ist der Treiber f"ur das Verfahren zur Bestimmung der neuen
	// Daten zum Endzeitpunkt endTime. Diese Methode bringt, an-
	// gewendet auf GitterEuler die Instanz in den neuen Zustand,
	// der durch Vorw"artsintegration der kompressiblen Euler-
	// gleichungen berechnet wird.
	// ! Konvention: Die Daten auf den Elementen des aktuellen 
	// Zeitschritts step () sind die zeitlich st"uckweise konstante 
	// L"osung  f"ur das Zeitintervall [time (),time () + deltat ()]

  bool b (false) ;

  if ((!(cfl > .0)) || (!(endTime > .0) || (!(backupStride > 0 || backupStride == -1)))) {
    cerr << "**WARNUNG (IGNORIERT) Falsche Parameter in runEulerSolver (..)" << endl ;
    return ;
  }
  while ((time () + deltat ()) < endTime) {
  
  	// Einen Zeitschritt weiterrechnen, und ...
  
    oneTimeStep (cfl) ;
    
    	// alle 16 Schritte die Gr"osse des Gitters ausgeben, damit
	// diese in den Statistiken dargestellt werden kann.
    
    if (step () % 16 == 0) {
      printsize () ;
    }
    
    	// alle 'checkpointStride' Schritte die Checkpointdateien schreiben,
	// falls die Anwendung das Zeitlimit "uberschreitet oder abst"urzt.
	
    if (checkpointStride != -1) {
      if (step () % checkpointStride == 0) {
        if (b) {
          backup ("data1.bak") ;
        } else {
          backup ("data0.bak") ;
        }
	cout << "**INFO GitterEuler :: runEulerSolver () Checkpointdateien geschrieben." << endl ;
        b = !b ;
      }
    }
    	// und alle 'backupStride' Schritte die Daten und das
	// Gitter als numerierte Dateien rausschreiben.
    
    if (backupStride != -1 ) {
      if (step () % backupStride == 0) {
        printsize () ;
        char * fileName = new char [strlen (outFile) + 200] ;
        sprintf (fileName, "%s.%u", outFile, step ()) ;
        backup (fileName) ;
//	sprintf (fileName, "%s_CMode.%u", outFile, step ()) ;
//	backupCMode (fileName) ;
	delete [] fileName ;
      }
    }
  }
  return ;
}

Gitter :: Geometric :: hface3_GEO * GitterEuler :: MacrogitterEuler :: insert_hface3 (hedge1_GEO *(&e)[3], int (&s)[3]) {
  return new ObjectsEuler :: hface3_euler_IMPL (0,e[0],s[0],e[1],s[1],e[2],s[2]) ;
}

Gitter :: Geometric :: hface4_GEO * GitterEuler :: MacrogitterEuler :: insert_hface4 (hedge1_GEO *(&e)[4], int (&s)[4]) {
  return new ObjectsEuler :: hface4_euler_IMPL (0,e[0],s[0],e[1],s[1],e[2],s[2],e[3],s[3]) ;
}

Gitter :: Geometric :: tetra_GEO * GitterEuler :: MacrogitterEuler :: insert_tetra (hface3_GEO *(&f)[4], int (&t)[4]) {
  return new ObjectsEuler ::  tetra_euler_IMPL (0,f[0],t[0],f[1],t[1],f[2],t[2],f[3],t[3]) ;
}

Gitter :: Geometric :: hexa_GEO * GitterEuler :: MacrogitterEuler :: insert_hexa (hface4_GEO *(&f)[6], int (&t)[6]) {
  return new ObjectsEuler ::  hexa_euler_IMPL (0,f[0],t[0],f[1],t[1],f[2],t[2],f[3],t[3],f[4],t[4],f[5],t[5]) ;
}

Gitter :: Geometric :: hbndseg3_GEO * GitterEuler :: MacrogitterEuler :: insert_hbnd3 (hface3_GEO * f, int t, Gitter :: hbndseg_STI :: bnd_t type) {
  switch (type) {
    case Gitter :: hbndseg_STI :: inflow :
      return new GitterEuler :: ObjectsEuler :: hbnd3_inflow_IMPL (0, f, t) ;
    case Gitter :: hbndseg_STI :: outflow :
      return new GitterEuler :: ObjectsEuler :: hbnd3_outflow_IMPL (0, f, t) ;
    case Gitter :: hbndseg_STI :: slip :
      return new GitterEuler :: ObjectsEuler :: hbnd3_slip_IMPL (0, f, t) ;
    case Gitter :: hbndseg_STI :: sym_xz :
    case Gitter :: hbndseg_STI :: sym_xy :
    case Gitter :: hbndseg_STI :: sym_yz :
    case Gitter :: hbndseg_STI :: reflect :
      return new GitterEuler :: ObjectsEuler :: hbnd3_reflect_IMPL (0, f, t) ;
    default :
      cerr << "WARNUNG GitterEuler :: MacrogitterEuler :: insert_hbnd3 () erzeugt ein Randelement von unbekanntem Typ: " << (int)(type) << endl ;
      return MacroGitterBasis :: insert_hbnd3 (f,t,type) ;
  }
}

Gitter :: Geometric :: hbndseg4_GEO * GitterEuler :: MacrogitterEuler :: insert_hbnd4 (hface4_GEO * f, int t, Gitter :: hbndseg_STI :: bnd_t type) {
  switch (type) {
    case Gitter :: hbndseg_STI :: inflow :
      return new GitterEuler :: ObjectsEuler :: hbnd4_inflow_IMPL (0, f, t) ;
    case Gitter :: hbndseg_STI :: outflow :
      return new GitterEuler :: ObjectsEuler :: hbnd4_outflow_IMPL (0, f, t) ;
    case Gitter :: hbndseg_STI :: slip :
      return new GitterEuler :: ObjectsEuler :: hbnd4_slip_IMPL (0, f, t) ;
    case Gitter :: hbndseg_STI :: sym_xz :
    case Gitter :: hbndseg_STI :: sym_xy :
    case Gitter :: hbndseg_STI :: sym_yz :
    case Gitter :: hbndseg_STI :: reflect :
      return new GitterEuler :: ObjectsEuler :: hbnd4_reflect_IMPL (0, f, t) ;
    default :
      cerr << "WARNUNG GitterEuler :: MacrogitterEuler :: insert_hbnd4 () erzeugt ein Randelement von unbekanntem Typ: " << (int)(type) << endl ;
      return MacroGitterBasis :: insert_hbnd4 (f,t,type) ;
  }
}
