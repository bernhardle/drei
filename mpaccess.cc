	// (c) bernhard schupp 1997 - 1998

	// $Source$
	// $Revision$
	// $Name$
	// $State$

#include <functional.h>
#include <algorithm.h>
#include "mpAccess.h"

static volatile char RCSId_mpAccess_cc [] = "$Id$" ;

vector < int > MpAccessLocal :: dest () const {
  vector < int > d (linkage.size ()) ;
  for (map < int, int, less < int > > :: const_iterator i = linkage.begin () ; 
  	i != linkage.end () ; i ++) d [(*i).second] = (*i).first ;
  return d ;
}

void MpAccessLocal :: printLinkage (ostream & out) const {
  out << "  MpAccessLocal :: printLinkage () " << myrank () << " -> " ;
  { for (map < int, int, less < int > > :: const_iterator i = linkage.begin () ; 
  	i != linkage.end () ; i ++) out << (*i).first << " " ; }
  out << endl ;
  return ;
}

int MpAccessLocal :: insertRequestSymetric (set < int, less < int > > req) {
  const int me = myrank () ;
  req.erase (me) ;
  vector < int > out ;
  {for (set < int, less < int > > :: const_iterator i = req.begin () ; i != req.end () ; i ++ )
    if (linkage.find (*i) == linkage.end ()) out.push_back (*i) ; }
  vector < vector < int > > in = gcollect (out) ;
  { for (vector < int > :: const_iterator i = out.begin () ; i != out.end () ; i ++ )
    if (linkage.find (*i) == linkage.end ()) {
      int n = linkage.size () ;
      linkage [*i] = n ;
    }
  }
  int cnt = 0 ;
  { for (int i = 0 ; i < psize () ; i ++ )
    if (in [i].end() != find (in [i].begin(), in [i].end(), me)) {
      assert (i != me) ;
      if (linkage.find (i) == linkage.end ()) {
        int n = linkage.size () ;
        linkage [i] = n ;
        cnt ++ ; 
      }
    }
  }
  return cnt ;
}
