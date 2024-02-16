	// (c) bernhard schupp, 1997 - 1998
	
	// $Source: /home/dfs/yc20/yc23/3/hier/developers/aufbau/RCS/lock.h,v $
	// $Revision: 1.1 $
	// $Name:  $
	// $State: Exp $

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream.h>

	// Einfache Klasse, die w"ahrend ihrer Lebnsdauer ein
	// Lockfile mit einem vorgegebenen Namen (Pfad) h"alt.

class FSLock {
  char * _fname ;
  public :
    FSLock (const char * = "") ;
   ~FSLock () ;
} ;

inline FSLock :: FSLock (const char * name) : _fname (0) {
  _fname = new char [strlen(name) + 100] ;
  assert (_fname) ;
  sprintf (_fname, "%s.lock", name) ;
  FILE * fp = fopen (_fname, "w") ;
  if (fp == NULL) {
    delete [] _fname ;
    _fname = 0 ;
    cerr << "**WARNUNG (IGNORIERT) Lockfile konnte nicht erzeugt werden" << endl ;
  }
  return ;
}

inline FSLock :: ~FSLock () {
  if (_fname) {
    int test = remove (_fname) ;
    if (test != 0) {
      cerr << "**WARNUNG (IGNORIERT) Lockfile konnte nicht gel\"oscht werden." << endl ;
    }
  }
  return ;
}

