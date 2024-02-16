	// (c) bernhard schupp 1997 - 1998

	// $Source$
	// $Revision$
	// $Name$
	// $State$

#ifndef MPACCESS_H_INCLUDED
#define MPACCESS_H_INCLUDED

#include <assert.h>
#include <iostream.h>
#include <strstream.h>
#include <vector.h>
#include <functional.h>
#include <set.h>
#include <map.h>
#include "serialize.h"

static volatile char RCSId_mpAccess_h [] = "$Id$" ;

class MpAccessGlobal {
  public :
    inline virtual ~MpAccessGlobal () ;
    virtual int psize () const = 0 ;
    virtual int myrank () const = 0 ;
    virtual int barrier () const = 0 ;
    virtual int gmax (int) const = 0 ;
    virtual int gmin (int) const = 0 ;
    virtual int gsum (int) const = 0 ;
    virtual double gmax (double) const = 0 ;
    virtual double gmin (double) const = 0 ;
    virtual double gsum (double) const = 0 ;
    virtual vector < int > gcollect (int) const = 0 ;
    virtual vector < double > gcollect (double) const = 0 ;
    virtual vector < vector < int > > gcollect (const vector < int > &) const = 0 ;
    virtual vector < vector < double > > gcollect (const vector < double > &) const = 0 ;
    virtual vector < ObjectStream > gcollect (const ObjectStream &) const = 0 ;
} ;

class MpAccessLocal : public MpAccessGlobal {
  map < int, int, less < int > > linkage ;
  public :
    inline virtual ~MpAccessLocal () ;
    void printLinkage (ostream &) const ;
    inline void removeLinkage () ;
    inline int nlinks () const ;
    inline int link (int) const ;
    vector < int > dest () const ;
    int insertRequestSymetric (set < int, less < int > >) ;
    virtual vector < vector < int > > exchange (const vector < vector < int > > &) const = 0 ;
    virtual vector < vector < double > > exchange (const vector < vector < double > > &) const = 0 ;
    virtual vector < vector < char > > exchange (const vector < vector < char > > &) const = 0 ;
    virtual vector < ObjectStream > exchange (const vector < ObjectStream > &) const = 0 ;
} ;


	//
	//    #    #    #  #          #    #    #  ######
	//    #    ##   #  #          #    ##   #  #
	//    #    # #  #  #          #    # #  #  #####
	//    #    #  # #  #          #    #  # #  #
	//    #    #   ##  #          #    #   ##  #
	//    #    #    #  ######     #    #    #  ######
	//

inline MpAccessGlobal :: ~MpAccessGlobal () {
}

inline MpAccessLocal :: ~MpAccessLocal () {
}

inline int MpAccessLocal :: link (int i) const {
  assert (linkage.end () != linkage.find (i)) ;
  return (* linkage.find (i)).second ;
}

inline int MpAccessLocal :: nlinks () const {
  return linkage.size () ;
}

inline void MpAccessLocal :: removeLinkage () {
  linkage.erase (linkage.begin (), linkage.end ()) ;
  return ;
}

#endif
