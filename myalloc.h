	//	(c) christian badura 1998
	//      -*- C++ -*-

	// $Source$
	// $Revision$
	// $Name$
	// $State$

#ifndef MYALLOC_H_INCLUDED
#define MYALLOC_H_INCLUDED

#include <memory.h>    // Def. von size_t, malloc (), free ()

static volatile char RCSId_myalloc_h [] = "$Id$" ;

class MyAlloc {
  static const long MAX_HOLD_ADD ;
  static const double MAX_HOLD_MULT ;
  static long _init ;
  public :
    class Initializer {
	// initializer versucht, die statischen Objekte der Speicherverwaltung
	// vor allem anderen zu initialisieren, damit keine Fehler auftreten,
	// falls statische Objekte irgendwo Instanzen mit MyAlloc als Basis-
	// klasse angelegen.
      public :
        Initializer () ;
       ~Initializer () ;
    } ;
    class OutOfMemoryException { };
  friend Initializer :: Initializer () ;
  friend Initializer :: ~Initializer () ;
  protected :
    MyAlloc () {}
   ~MyAlloc () {}
  public :
    void * operator new (size_t) throw (OutOfMemoryException) ;
    void operator delete (void *,size_t) ;
} ;

static MyAlloc :: Initializer allocatorInitializer ;

#endif // MYALLOC_H_INCLUDED
