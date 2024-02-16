	// (c) bernhard schupp 1997 - 1998

	// $Source$
	// $Revision$
	// $Name$
	// $State$
	
#include <assert.h>
#include <vector.h>
#include <functional.h>
#include <utility.h>
#include <map.h>
#include <algorithm.h>
#include "key.h"
#include "gitter_sti.h"

static volatile char RCSId_gitter_mgb_h [] = "$Id$" ;

template < class RandomAccessIterator > inline int cyclicReorder (RandomAccessIterator begin, RandomAccessIterator end) {
  RandomAccessIterator middle = min_element (begin,end) ;
  int pos = middle == begin ? 0 : (rotate (begin,middle,end), (end - middle)) ;
  if (*(begin + 1) < *(end - 1)) return pos ;
  else {
    reverse (begin,end) ;
    rotate (begin,end - 1,end) ;
    return - pos - 1 ;
  }
}

class MacroGridBuilder : protected Gitter :: Geometric {
  protected :
    typedef long 		vertexKey_t ;
    typedef pair < int, int > 	edgeKey_t ;
    typedef Key3 < int > 	faceKey_t ;
    typedef Key4 < int > 	elementKey_t ;

    typedef map < vertexKey_t , VertexGeo *, less < vertexKey_t > > 	vertexMap_t ;
    typedef map < edgeKey_t, hedge1_GEO *, less < edgeKey_t > >      	edgeMap_t ;
    typedef map < faceKey_t, void *, less < faceKey_t > >            	faceMap_t ;
    typedef map < elementKey_t, void *, less < elementKey_t > >      	elementMap_t ;
  
    vertexMap_t  _vertexMap ;
    edgeMap_t    _edgeMap ;
    faceMap_t    _face4Map, _face3Map, _hbnd3Map, _hbnd3Int, _hbnd4Map, _hbnd4Int ;
    elementMap_t _hexaMap, _tetraMap ;
  
    inline BuilderIF & myBuilder () ;
    inline const BuilderIF & myBuilder () const ;
    void removeElement (const elementKey_t &) ;
  public :
    virtual pair < VertexGeo *, bool >  InsertUniqueVertex (double, double, double, int) ;
    virtual pair < hedge1_GEO *, bool > InsertUniqueHedge1 (int,int) ;
    virtual pair < hface3_GEO *, bool > InsertUniqueHface3 (int (&)[3]) ;
    virtual pair < hface4_GEO *, bool > InsertUniqueHface4 (int (&)[4]) ;
    virtual pair < tetra_GEO *, bool >  InsertUniqueTetra (int (&)[4]) ;
    virtual pair < hexa_GEO *, bool >   InsertUniqueHexa (int (&)[8]) ;
    virtual bool InsertUniqueHbnd3 (int (&)[3], Gitter :: hbndseg :: bnd_t) ;
    virtual bool InsertUniqueHbnd4 (int (&)[4], Gitter :: hbndseg :: bnd_t) ;
    virtual void removeHexa (int (&)[8]) ;
    virtual void removeTetra (int (&)[4]) ;
  public :
    static bool debugOption (int) ;
    static void generateRawHexaImage (istream &, ostream &) ;
    static void generateRawTetraImage (istream &, ostream &) ;
    static void cubeHexaGrid (int, ostream &) ;
    MacroGridBuilder (BuilderIF &) ;
    virtual ~MacroGridBuilder () ;
    void inflateMacroGrid (istream &) ;
    void backupMacroGrid (ostream &) ;
  private :
    BuilderIF & _mgb ;
} ;


	//
	//    #    #    #  #          #    #    #  ######
	//    #    ##   #  #          #    ##   #  #
	//    #    # #  #  #          #    # #  #  #####
	//    #    #  # #  #          #    #  # #  #
	//    #    #   ##  #          #    #   ##  #
	//    #    #    #  ######     #    #    #  ######
	//

inline Gitter :: Geometric :: BuilderIF & MacroGridBuilder :: myBuilder () {
  return _mgb ;
}

inline const Gitter :: Geometric :: BuilderIF & MacroGridBuilder :: myBuilder () const {
  return _mgb ;
}

inline bool MacroGridBuilder :: debugOption (int level) {
  return (getenv ("VERBOSE_MGB") ? ( atoi (getenv ("VERBOSE_MGB")) > level ? true : (level == 0)) : false) ;
}
