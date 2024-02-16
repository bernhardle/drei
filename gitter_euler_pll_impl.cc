	// (c) bernhard schupp, 1997 - 1998

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/gitter_euler_pll_impl.cc,v $
	// $Revision: 2.3 $
	// $Name:  $
	// $State: Exp $
	// $Date$

#include <float.h>
#include <stdio.h>
#include <time.h>
#include <fstream.h>

#include "gitter_euler_pll_impl.h"
#include "walk.h"
#include "gitter_tetra_top_pll.h"
#include "gitter_hexa_top_pll.h"

static volatile char RCSId_gitter_euler_pll_impl_cc [] = "$Id: gitter_euler_pll_impl.cc,v 2.3 1998/12/09 17:11:41 yc23 Exp yc23 $" ;

void TetraEulerPllX :: writeDynamicState (ObjectStream & os) const {
  TetraPllXBase :: writeDynamicState (os) ;
  os.writeObject (EulerSolver :: InterpolationCube2Dzero (mytetra ().accessEulerData ())) ;
  return ;
}

void TetraEulerPllXMacro :: writeDynamicState (ObjectStream & os) const {
  TetraPllXBase :: writeDynamicState (os) ;
  os.writeObject (EulerSolver :: InterpolationCube2Dzero (mytetra ().accessEulerData ())) ;
  return ;
}

void TetraEulerPllXMacro :: inlineData (ObjectStream & os) throw (ObjectStream :: EOFException) {
  TreeIterator < Gitter :: helement_STI, is_leaf < Gitter :: helement_STI > > w (mytetra ()) ;
  for (w.first () ; ! w.done () ; w.next ())
    os.writeObject (w.item ().accessEulerData ()) ;
  return ;
}

void TetraEulerPllXMacro :: xtractData (ObjectStream & os) throw (ObjectStream :: EOFException) {
  TreeIterator < Gitter :: helement_STI, is_leaf < Gitter :: helement_STI > > w (mytetra ()) ;
  for (w.first () ; ! w.done () ; w.next ())
    os.readObject (w.item ().accessEulerData ()) ;
  return ;
}

	// Mit den zwei nachfolgenden Methoden und auch den analogen f"ur den
	// Tetraeder gibt es leider noch eine kleine Schwierigkeit auszur"aumen.
	// 

void HexaEulerPllX :: writeDynamicState (ObjectStream & os) const {
  HexaPllBaseX :: writeDynamicState (os) ;
  os.writeObject (EulerSolver :: InterpolationCube2Dzero (myhexa ().accessEulerData ())) ;
  return ;
}

void HexaEulerPllXMacro :: writeDynamicState (ObjectStream & os) const {
  HexaPllBaseX :: writeDynamicState (os) ;
  os.writeObject (EulerSolver :: InterpolationCube2Dzero (myhexa ().accessEulerData ())) ;
  return ;
}

void HexaEulerPllXMacro :: inlineData (ObjectStream & os) throw (ObjectStream :: EOFException) {
  TreeIterator < Gitter :: helement_STI, is_leaf < Gitter :: helement_STI > > w (myhexa ()) ;
  for (w.first () ; ! w.done () ; w.next ()) {
    const EulerSolver :: Vec5 & v = w.item ().accessEulerData ().ucon () ;
    os.writeObject (v [0]) ;
    os.writeObject (v [1]) ;
    os.writeObject (v [2]) ;
    os.writeObject (v [3]) ;
    os.writeObject (v [4]) ;
  }
  return ;
}

void HexaEulerPllXMacro :: xtractData (ObjectStream & os) throw (ObjectStream :: EOFException) {
  TreeIterator < Gitter :: helement_STI, is_leaf < Gitter :: helement_STI > > w (myhexa ()) ;
  for (w.first () ; ! w.done () ; w.next ()) {
    double a, b, c, d, e ;
    os.readObject (a) ;
    os.readObject (b) ;
    os.readObject (c) ;
    os.readObject (d) ;
    os.readObject (e) ;
    w.item ().accessEulerData ().initial (EulerSolver :: Vec5 (a,b,c,d,e)) ;
  }
  return ;
}

FacePllXIF_t & GitterEulerPll :: ObjectsEulerPll :: Hface3EulerPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _pllx ;
}

const FacePllXIF_t & GitterEulerPll :: ObjectsEulerPll :: Hface3EulerPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _pllx ;
}

GitterEulerPll :: ObjectsEulerPll :: Hface3EulerPllMacro :: Hface3EulerPllMacro (myhedge1_t * e0, int s0, myhedge1_t * e1, int s1, myhedge1_t * e2, int s2)
	: GitterEulerPll :: ObjectsEulerPll :: hface3_IMPL (0,e0,s0,e1,s1,e2,s2), _pllx (new mypllx_t (*this)) {
  return ;
}

GitterEulerPll :: ObjectsEulerPll :: Hface3EulerPllMacro :: ~Hface3EulerPllMacro () {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

FacePllXIF_t & GitterEulerPll :: ObjectsEulerPll :: Hface3EulerPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const FacePllXIF_t & GitterEulerPll :: ObjectsEulerPll :: Hface3EulerPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterEulerPll :: ObjectsEulerPll :: Hface3EulerPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

FacePllXIF_t & GitterEulerPll :: ObjectsEulerPll :: Hface4EulerPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _pllx ;
}

const FacePllXIF_t & GitterEulerPll :: ObjectsEulerPll :: Hface4EulerPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _pllx ;
}

GitterEulerPll :: ObjectsEulerPll :: Hface4EulerPllMacro :: Hface4EulerPllMacro 
	(myhedge1_t * e0, int s0, myhedge1_t * e1, int s1, myhedge1_t * e2, int s2, myhedge1_t * e3, int s3)
	: GitterEulerPll :: ObjectsEulerPll :: hface4_IMPL (0,e0,s0,e1,s1,e2,s2,e3,s3), 
	_pllx (new mypllx_t (*this)) {
  return ;
}

GitterEulerPll :: ObjectsEulerPll :: Hface4EulerPllMacro :: ~Hface4EulerPllMacro () {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

FacePllXIF_t & GitterEulerPll :: ObjectsEulerPll :: Hface4EulerPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const FacePllXIF_t & GitterEulerPll :: ObjectsEulerPll :: Hface4EulerPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterEulerPll :: ObjectsEulerPll :: Hface4EulerPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

ElementPllXIF_t & GitterEulerPll :: ObjectsEulerPll :: TetraEulerPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _pllx ;
}

const ElementPllXIF_t & GitterEulerPll :: ObjectsEulerPll :: TetraEulerPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _pllx ;
}

GitterEulerPll :: ObjectsEulerPll :: TetraEulerPllMacro :: TetraEulerPllMacro (myhface3_t * f0, int t0, myhface3_t * f1, int t1, 
	 myhface3_t * f2, int t2, myhface3_t * f3, int t3)
	: GitterEulerPll :: ObjectsEulerPll :: tetra_IMPL (0,f0,t0,f1,t1,f2,t2,f3,t3), _pllx (new mypllx_t (*this)) {
  return ;
}

GitterEulerPll :: ObjectsEulerPll :: TetraEulerPllMacro :: ~TetraEulerPllMacro () {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

ElementPllXIF_t & GitterEulerPll :: ObjectsEulerPll :: TetraEulerPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const ElementPllXIF_t & GitterEulerPll :: ObjectsEulerPll :: TetraEulerPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterEulerPll :: ObjectsEulerPll :: TetraEulerPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

ElementPllXIF_t & GitterEulerPll :: ObjectsEulerPll :: HexaEulerPll :: accessPllX () throw (Parallel :: AccessPllException) {
  return _pllx ;
}

const ElementPllXIF_t & GitterEulerPll :: ObjectsEulerPll :: HexaEulerPll :: accessPllX () const throw (Parallel :: AccessPllException) {
  return _pllx ;
}

GitterEulerPll :: ObjectsEulerPll :: HexaEulerPllMacro :: HexaEulerPllMacro (myhface4_t * f0, int t0, myhface4_t * f1, int t1, 
	 myhface4_t * f2, int t2, myhface4_t * f3, int t3, myhface4_t * f4, int t4, myhface4_t * f5, int t5)
	: GitterEulerPll :: ObjectsEulerPll :: hexa_IMPL (0,f0,t0,f1,t1,f2,t2,f3,t3,f4,t4,f5,t5),
	  _pllx (new HexaEulerPllXMacro (*this)) {
  return ;
}

GitterEulerPll :: ObjectsEulerPll :: HexaEulerPllMacro :: ~HexaEulerPllMacro () {
  delete _pllx ;
  return ;
}

ElementPllXIF_t & GitterEulerPll :: ObjectsEulerPll :: HexaEulerPllMacro :: accessPllX () throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

const ElementPllXIF_t & GitterEulerPll :: ObjectsEulerPll :: HexaEulerPllMacro :: accessPllX () const throw (Parallel :: AccessPllException) {
  assert (_pllx) ;
  return * _pllx ;
}

void GitterEulerPll :: ObjectsEulerPll :: HexaEulerPllMacro :: detachPllXFromMacro () throw (Parallel :: AccessPllException) {
  delete _pllx ;
  _pllx = 0 ;
  return ;
}

int GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd3 :: postRefinement () {
  for (EulerPllInternalBnd3 * b = down () ; b ; b = b->next ()) {
    b->_ipObj = _ipObj ;
    b->_touched = 1 ;
  }
  return 0 ;
}

EulerSolver :: Vec5 GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd3 :: euler3dCalcFlux (double time, hface3_GEO * face, pair < hasFace3 *, int > p,int) {
  assert (_touched) ;
  return p.first->euler3dCalcFlux (time, face, _ipObj, p.second) ;
}

EulerSolver :: Vec5 GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd3 :: euler3dCalcFlux (double time, hface3_GEO * face, const EulerSolver :: InterpolationSimplex2D & ul,int) {
  assert (_touched) ;
  return face->euler3dCalcFlux (time, ul, _ipObj) ;
}

int GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd4 :: postRefinement () {
  for (EulerPllInternalBnd4 * b = down () ; b ; b = b->next ()) {
    b->_ipObj = _ipObj ;
    b->_touched = 1 ;
  }
  return 0 ;
}

EulerSolver :: Vec5 GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd4 :: euler3dCalcFlux (double time, hface4_GEO * face, pair < hasFace4 *, int > p,int) {
  assert (_touched) ;
  return p.first->euler3dCalcFlux (time, face, _ipObj, p.second) ;
}

EulerSolver :: Vec5 GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd4 :: euler3dCalcFlux (double time, hface4_GEO * face, const EulerSolver :: InterpolationCube2D & ul,int) {
  assert (_touched) ;
  return face->euler3dCalcFlux (time, ul, _ipObj) ;
}

void GitterEulerPll :: transferInterpolationObjects () {

	// transferInterpolationObjects () schafft die Interpolationsobjekte zu
	// den aktuellen Zeitschrittdaten in die anliegenden Randelemente, die
	// auf anderen Teilgittern die entsprechende Randfl"ache abschliessen.

  const int nl = mpAccess ().nlinks () ;
  {
    vector < vector < double > > inout (nl) ;
    {for (int l = 0 ; l < nl ; l ++ ) {
      LeafIteratorTT < hface_STI > w (*this, l) ;
      for (w.inner ().first () ; ! w.inner (). done () ; w.inner ().next ()) {
        w.inner ().item ().accessPllX ().accessInnerPllX () ;
      }
      for (w.outer ().first () ; ! w.outer ().done () ; w.outer ().next ()) {
        w.outer ().item ().accessPllX ().accessInnerPllX () ;
      }
    }}
    inout = mpAccess ().exchange (inout) ;
    {for (int l = 0 ; l < nl ; l ++ ) {
      LeafIteratorTT < hface_STI > w (*this, l) ;
      for (w.outer ().first () ; ! w.outer ().done () ; w.outer ().next ()) {
        w.outer ().item ().accessPllX ().accessOuterPllX () ;
      }
      for (w.inner ().first () ; ! w.inner (). done () ; w.inner ().next ()) {
        w.inner ().item ().accessPllX ().accessOuterPllX () ;
      }
    }}
  }
  return ;
}

bool GitterEulerPll :: estimateRefinement () {

	// Diese Methode "uberschreibt das estimateRefinement () aus
	// Gittereuler und bildet gleichzeitig die Vereinigung der
	// bool R"uckgabewerte, die anzeigen ob irgendwelche Markierungen
	// gesetzt worden sind.

  return (bool)mpAccess ().gmax ((int)GitterEuler :: estimateRefinement ()) ;
}

void GitterEulerPll :: oneTimeStep (double cfl, bool verboseFlag) {

	// Diese Methode "uberschreibt den Zeitschritttreiber aus GitterEuler
	// und f"uhrt dabei noch den notwendigen Datenaustausch durch.

  const int me = mpAccess().myrank () ;
  exchangeDynamicState () ;
  GitterEuler :: oneTimeStep (cfl, verboseFlag) ;  
  return ;
}

void GitterEulerPll :: backupCMode (ostream & out) {
  GitterEuler :: backupCMode (out) ;
  return ;
}

void GitterEulerPll :: backupCMode (const char * fileName) {
  assert (debugOption (20) ? (cout << "  GitterEulerPll :: backupCMode (const char * = \""
  			<< (fileName ? fileName : "null") << "\")" << endl, 1) : 1) ;
  char * extendedName = new char [strlen (fileName) + 20] ;
  sprintf (extendedName, "%s.%d", fileName, mpAccess ().myrank ()) ;
  GitterEuler :: backupCMode (extendedName) ;
  delete [] extendedName ;
  return ;
}

void GitterEulerPll :: backup (ostream & out) {

	// Erkl"arung siehe Gitter :: backup (const char *) in gitter_sti.cc ;

  GitterEuler :: backup (out) ;
  return ;
}

void GitterEulerPll :: restore (istream & in) {

	// Erkl"arung siehe Gitter :: backup (const char *) in gitter_sti.cc ;

  Gitter :: restore (in) ;
  GitterEuler :: restoreData (in) ;
  if (fabs (time () - mpAccess ().gmax (time ())) > DBL_EPSILON) {
    cerr << "**FEHLER (FATAL) Inkonsistente Zeit aufgetreten [" << time () 
         << "] nach dem Restore in " << __FILE__ << " Zeilee " << __LINE__ << endl ;
    abort () ;
  }
  return ;
}

void GitterEulerPll :: backup (const char * fileName) {

	// Erkl"arung siehe Gitter :: backup (const char *) in gitter_sti.cc ;

  char * extendedFileName = new char [strlen (fileName) + 1024] ;
  assert (extendedFileName) ;
  sprintf (extendedFileName, "%s.%u", fileName, mpAccess ().myrank ()) ;
  GitterEuler :: backup (extendedFileName) ;
  delete [] extendedFileName ;
  return ;
}

void GitterEulerPll :: restore (const char * fileName) {

	// Erkl"arung siehe Gitter :: backup (const char *) in gitter_sti.cc ;

  char * extendedFileName = new char [strlen (fileName) + 1024] ;
  assert (extendedFileName) ;
  sprintf (extendedFileName, "%s.%u", fileName, mpAccess ().myrank ()) ;
  Gitter :: restore (extendedFileName) ;
  delete [] extendedFileName ;
  return ;
}

double GitterEulerPll :: addUpdate (double cfl) {

	// Das verteilte addUpdate () bildet gleichzeitig das Minimum "uber
	// die Zeitschrittweieten auf allen Teilgittern.

  return mpAccess ().gmin (GitterEuler :: addUpdate (cfl)) ;
}

GitterEulerPll :: GitterEulerPll (MpAccessLocal & mpa) : GitterBasisPll (mpa), _macrogitter (0) {
  _macrogitter = new MacroGitterEulerPll () ;
  assert (_macrogitter) ;
  notifyMacroGridChanges () ;
  return ;
}

GitterEulerPll :: GitterEulerPll (const char * file, MpAccessLocal & mpa) : GitterBasisPll (mpa), _macrogitter (0) {
  {
    char * n = new char [strlen(file) + 20] ;
    assert (n) ;
    sprintf (n, "%s.%u", file, mpAccess().myrank ()) ;
    ifstream in (n) ;
    if (in.good()) {
      if (debugOption (1)) cout << "**INFO GitterEulerPll :: GitterEulerPll (2) : \"offnet < " << n << " > " << endl ;
      _macrogitter = new MacroGitterEulerPll (in) ;
    } else {
      cerr << "**WARNUNG (IGNORIERT) GitterEulerPll :: GitterEulerPll () : Datei < " ;
      cerr << n << " > konnte nicht ge\"offnet werden - leeres Gitter erzeugt." << endl ;
      _macrogitter = new MacroGitterEulerPll () ;
    }
    assert (_macrogitter) ;
    delete [] n ;
    notifyMacroGridChanges () ;
  }
  return ;
}

GitterEulerPll :: ~GitterEulerPll () {
  delete _macrogitter ;
}

Gitter :: Geometric :: VertexGeo * GitterEulerPll :: MacroGitterEulerPll :: insert_vertex (double x, double y, double z, int i, int o) {
  return GitterBasisPll :: MacroGitterBasisPll :: insert_vertex (x,y,z,i,o) ;
}

Gitter :: Geometric :: hedge1_GEO * GitterEulerPll :: MacroGitterEulerPll :: insert_hedge1 (VertexGeo * a, VertexGeo * b) {
  return GitterBasisPll :: MacroGitterBasisPll :: insert_hedge1 (a,b) ;
}

Gitter :: Geometric :: hface3_GEO * GitterEulerPll :: MacroGitterEulerPll :: insert_hface3 (hedge1_GEO *(&e)[3], int (&s)[3]) {
  return new GitterEulerPll :: ObjectsEulerPll :: Hface3EulerPllMacro (e[0],s[0],e[1],s[1],e[2],s[2]) ;
}

Gitter :: Geometric :: hface4_GEO * GitterEulerPll :: MacroGitterEulerPll :: insert_hface4 (hedge1_GEO *(&e)[4], int (&s)[4]) {
  return new GitterEulerPll :: ObjectsEulerPll :: Hface4EulerPllMacro (e[0],s[0],e[1],s[1],e[2],s[2],e[3],s[3]) ;
}

Gitter :: Geometric :: tetra_GEO *  GitterEulerPll :: MacroGitterEulerPll :: insert_tetra (hface3_GEO *(&f)[4], int (&t)[4]) {
  return new GitterEulerPll :: ObjectsEulerPll :: TetraEulerPllMacro (f[0],t[0],f[1],t[1],f[2],t[2],f[3],t[3]) ;
}

Gitter :: Geometric :: hexa_GEO * GitterEulerPll :: MacroGitterEulerPll :: insert_hexa (hface4_GEO *(&f)[6], int (&t)[6]) {
  return new GitterEulerPll :: ObjectsEulerPll :: HexaEulerPllMacro (f[0],t[0],f[1],t[1],f[2],t[2],f[3],t[3],f[4],t[4],f[5],t[5]) ;
}

Gitter :: Geometric :: hbndseg3_GEO * GitterEulerPll :: MacroGitterEulerPll :: insert_hbnd3 (hface3_GEO * f, int t, Gitter :: hbndseg_STI :: bnd_t typ) {
  switch (typ) {
    case Gitter :: hbndseg_STI :: closure :
      return new Hbnd3PllInternal < GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd3, BndEulerIntPllX < GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd3 > , BndEulerIntPllXMacro < GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd3 > > :: macro_t (f,t) ;
    case Gitter :: hbndseg_STI :: inflow :
      return new Hbnd3PllExternal < GitterEuler :: ObjectsEuler :: InflowBnd3, BndsegPllBaseXMacro < hbndseg3_GEO > > (f,t) ;
    case Gitter :: hbndseg_STI :: outflow :
      return new Hbnd3PllExternal < GitterEuler :: ObjectsEuler :: OutflowBnd3, BndsegPllBaseXMacro < hbndseg3_GEO > > (f,t) ;
    case Gitter :: hbndseg_STI :: slip :
      return new Hbnd3PllExternal < GitterEuler :: ObjectsEuler :: SlipBnd3, BndsegPllBaseXMacro < hbndseg3_GEO > > (f,t) ;
    case Gitter :: hbndseg_STI :: sym_xz :
    case Gitter :: hbndseg_STI :: sym_xy :
    case Gitter :: hbndseg_STI :: sym_yz :
    case Gitter :: hbndseg_STI :: reflect :
      return new Hbnd3PllExternal < GitterEuler :: ObjectsEuler :: ReflectBnd3, BndsegPllBaseXMacro < hbndseg3_GEO > > (f,t) ;
    default :
      cerr << "**WARNUNG (IGNORIERT) GitterEuler :: MacroGitterEulerPll :: insert_hbnd3 () Anforderung von unbekanntem Typ: " << (int)(typ) << endl ;
      return GitterBasisPll :: MacroGitterBasisPll :: insert_hbnd3 (f,t,typ) ;
  }
}

Gitter :: Geometric :: hbndseg4_GEO * GitterEulerPll :: MacroGitterEulerPll :: insert_hbnd4 (hface4_GEO * f, int t, Gitter :: hbndseg_STI :: bnd_t typ) {
  switch (typ) {
    case Gitter :: hbndseg_STI :: closure :
      return new Hbnd4PllInternal < GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd4, BndEulerIntPllX < GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd4 > , BndEulerIntPllXMacro < GitterEulerPll :: ObjectsEulerPll :: EulerPllInternalBnd4 > > :: macro_t (f,t) ;
    case Gitter :: hbndseg_STI :: inflow :
      return new Hbnd4PllExternal < GitterEuler :: ObjectsEuler :: InflowBnd4, BndsegPllBaseXMacro < hbndseg4_GEO > > (f,t) ;
    case Gitter :: hbndseg_STI :: outflow :
      return new Hbnd4PllExternal < GitterEuler :: ObjectsEuler :: OutflowBnd4, BndsegPllBaseXMacro < hbndseg4_GEO > > (f,t) ;
    case Gitter :: hbndseg_STI :: slip :
      return new Hbnd4PllExternal < GitterEuler :: ObjectsEuler :: SlipBnd4, BndsegPllBaseXMacro < hbndseg4_GEO > > (f,t) ;
    case Gitter :: hbndseg_STI :: sym_xz :
    case Gitter :: hbndseg_STI :: sym_xy :
    case Gitter :: hbndseg_STI :: sym_yz :
    case Gitter :: hbndseg_STI :: reflect :
      return new Hbnd4PllExternal < GitterEuler :: ObjectsEuler :: ReflectBnd4, BndsegPllBaseXMacro < hbndseg4_GEO > > (f,t) ;
    default :
      cerr << "**WARNUNG (IGNORIERT) GitterEuler :: MacroGitterEulerPll :: insert_hbnd4 () creating bndseg of unknown type: " << (int)(typ) << endl ;
      return GitterBasisPll :: MacroGitterBasisPll :: insert_hbnd4 (f,t,typ) ;
  }
}

GitterEulerPll :: MacroGitterEulerPll :: MacroGitterEulerPll () {
  return ;
}

GitterEulerPll :: MacroGitterEulerPll :: MacroGitterEulerPll (istream & in) {
  macrogridBuilder (in) ;
  return ;
}
