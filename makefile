#	// (c) bernhard schupp 1997 - 1998
#	//
#	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/makefile,v $
#	// $Revision: 2.12 $
#	// $Name:  $
#	// $State: Exp $
#	// $Id: makefile,v 2.12 1998/12/21 10:53:30 yc23 Exp yc23 $
#	// $Date: 1998/12/21 10:53:30 $
#	//
#	// Makefile mit besonderen Einstellungen f"ur die IBM SP mit xlC C Set++ Compiler von IBM
#	// Das makefile kann eventuell nur mit einer neueren Version des gmake (3.77) verwendet
#	// werden.
#	// Das Makefile wird von GNU Make version 3.77 oder besser verstanden.

.SUFFIXES : .cc .h

all:
	@@ echo "WAS soll ich machen ? ball, main_test, main_euler, ball_pll, main_pll, main_euler_pll "

.PHONY : clean

.cc.o :
	$(CXX) -c $(CFLAGS) $<

.h.h :
	$(CO) $<

#	// cc ist der ANSI C Compiler, CC der ANSI C++ Compiler oder auch der IBM xlC
#	// PCC ist der C++ Compiler f"ur MPI basierte Anwendungen, d.h. mit automatischem
#	// include von <mpi.h>, der Pfad auf dem die STL gefunden wird ist entsprechend
#	// zu "andern, das define typename=<nix> hilft dem IBM Compiler "uber den
#	// unbekannten Namen hinweg, ANSI C++ Compiler verlangen das aber so.

cc      = /bin/time xlc -w -qlanglvl=ansi -qtune=pwr2 -qarch=pwr2 -Dtypename= -DIBM_XLC
CXX     = /bin/time xlC -w -I$(HOME)/stl -qlanglvl=extended -qinitauto=ff -qtune=pwr2 -qarch=pwr2 -Dtypename= -DIBM_XLC
PCC	= /bin/time mpCC -w -I$(HOME)/stl -qlanglvl=extended -qinitauto=ff -qtune=pwr2 -qarch=pwr2 -Dtypename= -DIBM_XLC

#	// Das Flag -DNDEBUG entfernt alle assert()'s und einige andere Tests
#	// aus dem Programm, wodurch Compilation und Ausf"uhrung beschleunigt
#	// werden. Zur Fehlersuche rausnehmen.

CFLAGS  = -DNDEBUG

#	// Die Linkerflags und die Includepfade f"ur die Deklartionsdateien
#	// der Graphenpartitionierer Party und Metis m"ussen
#	// den lokalen Gegebenheiten angepasst werden.

MAXDATA = 512000000
LDFLAGS = -L$(HOME)/3/hier/PARTY_1.1/bin/AIX/ -L$(HOME)/3/hier/metis-3.0/ -bmaxdata:$(MAXDATA)
PARTLIB = -lparty -lmetis
PARTINC = -I$(HOME)/3/hier/PARTY_1.1/include/ -I$(HOME)/3/hier/metis-3.0/Lib/ -I.

myalloc.o 	: myalloc.cc myalloc.h
mapp_cube_3d.o 	: mapp_cube_3d.cc mapp_cube_3d.h
mapp_tetra_3d.o	: mapp_tetra_3d.cc mapp_tetra_3d.h

#	// INTFHDR sind die Interface Headerfiles, die in jedem Fall ben"otigt werden.
#	// GRIDHDR enthalten alle Deklarationen des seriellen Basispakets f"ur
#	// 	   Iterationsschnittstellen und Geometriesockelklassen, usw.
#	// IMPLHDR Sind die Deklarationsdateien, mit denen ein leeres Gitter aufgebaut
#	//	   werden kann.

INTF_HDR = euler.h parallel.h key.h serialize.h
GRID_HDR = gitter_sti.h myalloc.h $(INTF_HDR)
IMPL_HDR = gitter_impl.h $(GRID_HDR) gitter_hexa_top.h gitter_tetra_top.h mapp_cube_3d.h mapp_tetra_3d.h

ADP = gitter_sti.o gitter_geo.o gitter_mgb.o mapp_cube_3d.o mapp_tetra_3d.o euler.o myalloc.o

gitter_sti.o 	: gitter_sti.cc $(GRID_HDR) walk.h lock.h
gitter_geo.o 	: gitter_geo.cc $(GRID_HDR) walk.h mapp_cube_3d.h mapp_tetra_3d.h
gitter_mgb.o 	: gitter_mgb.cc $(GRID_HDR) gitter_mgb.h key.h

main_test.o  	: main_test.cc $(IMPL_HDR)
ball.o  	: ball.cc gitter_mgb.h $(IMPL_HDR)

main_test 	: main_test.o $(ADP)
	$(CXX) -o $@@ $(CFLAGS) $(LDFLAGS) $^ $(ADDLIBS)

ball 		: ball.o $(ADP)
	$(CXX) -o $@@ $(CFLAGS) $(LDFLAGS) $^ $(ADDLIBS)

gitter_euler_impl.o : gitter_euler_impl.cc gitter_euler_impl.h $(IMPL_HDR)

MP  = mpAccess.o mpAccess_MPI.o
PAR = gitter_pll_impl.o gitter_pll_sti.o gitter_pll_idn.o gitter_pll_ldb.o gitter_pll_mgb.o $(MP) $(ADP)

#	// MPHDR 	Sind die Headerfiles, die die Schnittstellen zum Message-Passing
#	//	 	System beschreiben.
#	// PLL_GRID_HDR ist f"ur das verteilte Gitter analog zu GRIDHDR
#	// PLL_IMPL_HDR  "    "   "      "        "      "    " IMPLHDR

MP_HDR       = mpAccess.h serialize.h
PLL_GRID_HDR = gitter_pll_sti.h mpAccess.h gitter_pll_ldb.h $(GRID_HDR) 
PLL_IMPL_HDR = gitter_pll_impl.h walk.h gitter_hexa_top_pll.h gitter_tetra_top_pll.h $(IMPL_HDR) $(PLL_GRID_HDR) $(MP_HDR) gitter_pll_ldb.h mpAccess_MPI.h myalloc.h

gitter_pll_impl.o : gitter_pll_impl.cc $(PLL_IMPL_HDR)
gitter_pll_sti.o  : gitter_pll_sti.cc $(PLL_GRID_HDR) walk.h
gitter_pll_idn.o  : gitter_pll_idn.cc $(PLL_GRID_HDR)
gitter_pll_mgb.o  : gitter_pll_mgb.cc $(PLL_GRID_HDR) gitter_mgb.h key.h
gitter_pll_ldb.o  : gitter_pll_ldb.cc gitter_pll_ldb.h $(PLL_GRID_HDR) metis.h party_lib.h
	$(CXX) $(CFLAGS) $(PARTINC) -c $<

mpAccess.o	: mpAccess.cc mpAccess.h

mpAccess_MPI.o	: mpAccess_MPI.cc mpAccess_MPI.h $(MP_HDR)
	$(PCC) -c $(CFLAGS) $<

main_pll.o 	: main_pll.cc $(PLL_IMPL_HDR)
	$(PCC) -c $(CFLAGS) $<
ball_pll.o 	: ball_pll.cc $(PLL_IMPL_HDR)
	$(PCC) -c $(CFLAGS) $<
main_pll 	: main_pll.o $(PAR)
	$(PCC) $(CFLAGS) $(LDFLAGS) -o $@@ $^ $(PARTLIB) $(ADDLIBS) -lmpi -lm
ball_pll 	: ball_pll.o $(PAR) 
	$(PCC) $(CFLAGS) $(LDFLAGS) -o $@@ $^ $(PARTLIB) $(ADDLIBS) -lmpi -lm

#	// EULER_HDR 	Die Deklarationsdatei f"ur die Basisimplementierungen
#	// 		des Eulerl"osers in Elementen, Fl"achen und R"andern.

EULER_HDR = gitter_euler_impl.h $(IMPL_HDR) 

euler.o             : euler.cc euler.h serialize.h
swflux.o	    : swflux.cc euler.h
osher3d2.o          : osher3d2.cc euler.h
gitter_euler_impl.o : gitter_euler_impl.cc $(EULER_HDR) walk.h 

#	// In der Bibliothek libflux.a sollten die verschiedenen numerischen Fl"usse
#	// abgelegt werden. Zum Umfang der Quelldateien geh"ort aber nur der Fl"uss
#	// von Steger und Warming.

libflux.a : swflux.o
	ar -ru libflux.a swflux.o

EULER = main_euler.o gitter_euler_impl.o $(ADP) libflux.a

main_euler.o : main_euler.cc gitter_euler_impl.h $(EULER_HDR)
main_euler : $(EULER)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o main_euler $(EULER) $(ADDLIBS) -lm

gitter_euler_pll_impl.o : gitter_euler_pll_impl.cc gitter_euler_pll_impl.h $(EULER_HDR) walk.h gitter_hexa_top_pll.h gitter_hexa_top.h $(PLL_IMPL_HDR)

EULER_PLL = main_euler_pll.o gitter_euler_pll_impl.o gitter_euler_impl.o $(PAR) libflux.a

main_euler_pll.o : main_euler_pll.cc gitter_euler_pll_impl.h gitter_euler_impl.h $(MP_HDR) $(PLL_IMPL_HDR) $(EULER_HDR)
	$(PCC) -c $(CFLAGS) $<
main_euler_pll : $(EULER_PLL)
	$(PCC) $(CFLAGS) $(LDFLAGS) -o main_euler_pll $(EULER_PLL) $(PARTLIB) -lmpi -lm

clean :
	rm -f *.o core libflux.a ;\
	rm -rf ti_files;\
	rcsclean
