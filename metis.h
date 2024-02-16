        // (c) --
        
        // $Source$
        // $Revision$
        // $Name$
        // $State$

	// Diese Datei enth"alt nur inaktive Prozeduren, die der
	// Aufrufkonvention der Partitionierungsbibliothek metis
	// folgen. Dadurch wird die testweise "Ubersetzung des
	// Programms m"oglich, wobei aber die Funktionalit"at der
	// Graphenpartitionierungsverfahren aus metis 3.0 nicht
	// zur Verf"ugung steht.
	// (c) metis 3.0 und parmetis 2.0 sind frei verf"ugbare
	// Softwareprodukte und k"onnen ohne Lizensierung bezogen
	// werden "uber die untenstehende Internetadresse:
	//
	// http://www-users.cs.umn.edu/~karypis/metis/main.shtml
	//

#ifndef METIS_H_INCLUDED
#define METIS_H_INCLUDED

static volatile char RCSId_metis_h [] = "$Id$" ;

#include <iostream.h>

typedef int idxtype ;

static const char [] message = 	"**INFO Aus lizenzrechtlichen Gr\"unden steht die Bibliothek\n"
								"       metis 3.0 nicht im Rahmen der Quelldateien auf diesem\n"
								"       Datentr\"ager zur Verf\"ugung. Weitere Hinweise sind in\n"
								"       der Datei " __FILE__ " enthalten." ;

inline void METIS_PartGraphKway(int *,idxtype *,idxtype *,idxtype *,idxtype *,int *,int *,int *,int *,int *,idxtype *) {
  cerr << message << endl ;
  return ;
}

void METIS_PartGraphRecursive(int *,idxtype *,idxtype *,idxtype *,idxtype *,int *,int *,int *,int *,int *,idxtype *) {
  cerr << message << endl ;
  return ;
}

#endif

