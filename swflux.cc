	// (c) monika wierse, 1996

	// $Source: /home/dfs/yc20/yc23/3/hier/developers/RCS/swflux.cc,v $
	// $Revision: 2.2 $
	// $Name:  $
	// $State: Exp $

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "euler.h"

static volatile char RCSId_swflux_cc [] = "$Id: swflux.cc,v 2.2 1998/10/29 11:39:18 yc23 Exp yc23 $" ;

/*----------------------------------------------------------------------*/  
/*                                                                      */
/*   Dimensionsabh"angige  Routinen  f"ur STEGER & WARMING FLUSS        */                                   
/*                                                                      */
/*----------------------------------------------------------------------*/

static void mult_with_vector(double mtv[5],double mtinvv[5],
	double eigen,const double v[5], double u[5]) 

/* Berechnung des Produktes mt * eigen * mtinv * v, wobei eigen die
   Eintraege einer Diagonalmatrix sind */

{  int  j , k ;

/* mtv * eigen * mtinv * v*/
 
   for ( j = 0 ; j < 5 ; j ++){
     u[j] = 0. ;
     for ( k = 0 ; k < 5 ; k ++)
       u[j] += mtv[j]*eigen*mtinvv[k]*v[k] ;
   }
}

static int matrizen_mult_normal_mult_u( double r[3], 
	const double u[5], double p, double v[3], double sum1[5]) {
			/* Berechnung von f(u)*r */ 
  double wert ;
  wert = u[4] + p ;
  sum1[0] +=  u[1]*r[0] + u[2]*r[1] + u[3]*r[2];
  sum1[1] +=  (u[1]*v[0]+p)*r[0]  + u[2]*v[0]*r[1] + u[3]*v[0]*r[2];
  sum1[2] +=  u[1]*v[1]*r[0]  +     (u[2]*v[1]+p)*r[1] + u[3]*v[1]*r[2] ;
  sum1[3] +=  u[1]*v[2]*r[0]  +      u[2]*v[2]*r[1] + (u[3]*v[2]+p)*r[2];
  sum1[4] +=  wert*v[0]*r[0]  + wert*v[1]*r[1] +wert*v[2]*r[2];
  return( 0);
}

int FluxCalcSW :: matrizen_third_vector(double mtv[5],double mtinvv[5],
	double *u,double *r, double d,double c,double qq)
     
/*      Rueckgabe der jeweils 3 Vektoren der Matrizen mt und mtinv */
     
{ double dc,dd2,wert,dd2c,qqd2,help,help1,g1ddc ;
  double ur,r1dd,r2dd,r0dd;

  dd2 = d * 0.5 ;
  g1ddc = g1/ (d*c) ; 
  dd2c = dd2/c;
  dc = d * c ;
  qqd2 = qq*0.5 ;
  ur = u[0] * r[0] + u[1] * r[1] + u[2] * r[2] ;
  wert = dd2*(qqd2/c + c/g1) ;

  r0dd = r[0]/d ;
  r1dd = r[1]/d ;
  r2dd = r[2]/d ;

 
  mtv[0] = dd2c ;
  help = r[0]*dd2 ;
  help1 = dd2c * u[0];
  mtv[1] = help1 + help;
 
  help = r[1]*dd2 ;
  help1 = u[1] * dd2c;
  mtv[2] = help1 +  help;
 
  help = r[2]*dd2 ;
  help1 = u[2] * dd2c;
  mtv[3] = help1 + help;
 
  mtv[4] = wert+dd2*ur;
 
  ur  =  ur/d ;
  help = g1*qqd2/dc ;
  
  mtinvv[0] = -ur + help ;
  mtinvv[1] = r0dd - g1ddc * u[0] ;
  mtinvv[2] = r1dd - g1ddc * u[1] ;
  mtinvv[3] = r2dd - g1ddc * u[2] ;  
  mtinvv[4] = g1ddc ;
  return(0) ;
}

int FluxCalcSW :: matrizen_fourth_vector(double mtv[5],double mtinvv[5],
	double *u,double *r, double d,double c,double qq)
	     
/*      Belegung der Matrizen mt und mtinv */
     
{ double dc,dd2,dd2c,wert,help,help1,g1ddc ;
  double ur,r1dd,r2dd,r0dd,qqd2;

  dd2 = d * 0.5 ;
  g1ddc = g1/ (d*c) ; 
  dd2c = dd2/c;
  dc = d * c ;
 
 
  qqd2 = qq*0.5 ;
  ur = u[0] * r[0] + u[1] * r[1] + u[2] * r[2] ;
  wert = dd2*(qqd2/c + c/g1) ;


  r0dd = r[0]/d ;
  r1dd = r[1]/d ;
  r2dd = r[2]/d ;

  mtv[0] = dd2c ;

 
  help = r[0]*dd2 ;
  help1 = dd2c * u[0] ;
 
  mtv[1] = help1 - help;

 
  help = r[1]*dd2 ;
  help1 = u[1] * dd2c;
 
  mtv[2] = help1 -  help;

 
  help = r[2]*dd2 ;
  help1 = u[2] * dd2c ;
 
  mtv[3] = help1 - help;
  mtv[4] = wert-dd2*ur;
  ur  =  ur/d ;
  help = g1*qqd2/dc ;

  mtinvv[0] = ur + help ;
  mtinvv[1] = -r0dd - g1ddc * u[0];
  mtinvv[2] = -r1dd - g1ddc * u[1] ;
  mtinvv[3] = -r2dd - g1ddc * u[2] ;  
  mtinvv[4] = g1ddc ;
  return(0) ;
}

int FluxCalcSW :: cplusvalues_con(const double normale[3], const double u[5],
	double eigpos[5], double v[3],double * c,double * qq, double pressure,int * total)
      
/* Berechnung der positiven Eigenwerte in abhaengigkeit von u und der groesse c */
{
 register double	 eigen ;
 register int		 i ;
 
 *total = 0 ;
   
 if(u[0] <= 0.){
    fprintf(stderr, "Fehler in cplusvalues: Dichte %lf\t%lf\n", u[0], pressure) ;
    return(-1);
  }
  else{
    *qq = 0 ;
    for( i = 0 ; i < 3 ; i++){
      v[i] = u[i+1]/u[0] ;
      *qq += v[i]*v[i] ;
    }
    *c = sqrt(GAMMA*pressure/u[0]) ;
  }
 
  
  eigen = 0.;
  for( i = 0 ; i < 3 ; i ++)
   eigen += normale[i]*v[i] ;
     
  if( eigen > 0.){
      (*total) ++ ;
       eigpos[0] = eigen ;
    }
    else{
      eigpos[0] = 0. ;
    }
    for ( i = 1 ; i < 3 ; i ++) {
      eigpos[i] = eigpos[0] ;
    }
    if( (eigen+*c) > 0.){
      (*total) ++ ;
       eigpos[3] = eigen+*c ;
    }
    else{
      eigpos[3] = 0. ;
    }
  
   if( (eigen-*c) > 0. ){
      (*total) ++ ;
      eigpos[4] = eigen-*c ;
    }

    else{
      if(*total == 2)
      
     /* dieser term von f(u)*n_{ij} abgezogen werden soll, 
         daher wird minuszeichn hier eingebaut */
      
	 eigpos[4] = -eigen+*c ;
      else
       eigpos[4] = 0. ;
    }
    return(0) ; 
}

 

int FluxCalcSW :: cminusvalues_con( const double normale[3], const double * u, 
	double * eigneg, double * v,double * c,double * qq,double pressure,int * total)
    
/* Berechnung der negativen Eigenwerte in abhaengigkeit von u und der 
   groessen r und c */

{double eigen;
 int i ; 
 
 *total = 0 ;
 
 if(u[0] <= 0. || pressure < 0. ){
   fprintf(stderr, "Fehler in cminusvalues: Dichte %f Druck %f\n", u[0], pressure) ;
   return(-1) ;
 }
 else{
   *qq = 0 ;
   for( i = 0 ; i < 3 ; i++){
     v[i] = u[i+1]/u[0] ;
     *qq += v[i]*v[i] ;
   }
   *c = sqrt(GAMMA*pressure/u[0]) ;
   
 }
 

 eigen = 0.;
 for( i = 0 ; i < 3 ; i ++){
   eigen += normale[i]*v[i] ;
 } 

 if( eigen < 0.){
   (*total) ++ ;
   eigneg[0] = eigen ;
 }
 else{
   eigneg[0] = 0. ;
 }
 for ( i = 1 ; i < 3 ; i ++) {
   eigneg[i] = eigneg[0] ;
 }

 
 if( (eigen-*c) < 0. ){
  (*total) ++ ;
   eigneg[4] = eigen-*c;
 }
 else{
   eigneg[4] = 0. ;
 }
 
 if( (eigen+*c) < 0.){
  (*total) ++ ;
   eigneg[3] = eigen+*c ;
 }

 else{
  if(*total == 2)
   eigneg[3] = -eigen-*c ;  /* da mit - spaeter gebraucgt siehe oben */
  else 
   eigneg[3] = 0. ;
 }
 return(0) ;
}

/*----------------------------------------------------------------------*/  
/*                                                                      */
/*   Dimensionsunabh"angige  Routinen  f"ur STEGER & WARMING FLUSS      */                                   
/*                                                                      */
/*----------------------------------------------------------------------*/

void FluxCalcSW :: fluss (const double ul [5], const double ur [5],
	const double large_normal [3], double sum [5]) { 
  static const double   d_zero[5] = { 0.0, 0.0, 0.0, 0.0, 0.0};
  double norm_len;
  double	 eigpos[5],eigneg[5],mtv[5],mtinvv[5] ;
  double         c,u[3],qq,pressure,pressurenb;
  double	 sum1[5],sum2[5];
  double         normal[3];
  int i,total;

  norm_len = sqrt(large_normal[0]*large_normal[0]+
  	          large_normal[1]*large_normal[1]+
	 	  large_normal[2]*large_normal[2]);
  
  {
 
    double x = 1.0 / norm_len ;

    normal[0] = large_normal[0] * x ;
    normal[1] = large_normal[1] * x ;
    normal[2] = large_normal[2] * x ;
  
  }

  pressure   = g1*(ul[4]-0.5/ul[0]*(ul[1]*ul[1]+ul[2]*ul[2]+ul[3]*ul[3]));
  pressurenb = g1*(ur[4]-0.5/ur[0]*(ur[1]*ur[1]+ur[2]*ur[2]+ur[3]*ur[3]));
	 
  memcpy( sum1, d_zero, sizeof(double [5]));
  memcpy( sum2, d_zero, sizeof(double [5])); 
  memcpy( sum, d_zero,  sizeof(double [5])); 

  if(0 > cplusvalues_con(normal,ul,eigpos,u,&c,&qq,pressure,&total)) {
     fprintf(stderr,"sumhjl() fehler in cminusvalues_con()"); 
     fflush(stderr) ;
     return ;
  }       
  if(total != 0 ){ 
     
    if(total == 1){ /* einer positiv */
      matrizen_third_vector(mtv,mtinvv,u, normal,ul[0],c,qq) ; 
      mult_with_vector(mtv,mtinvv,eigpos[3],ul,sum1) ;
    }
    if(total == 2){ /* zwei positiv, einer negativ */
      matrizen_fourth_vector(mtv,mtinvv,u, normal,ul[0],c,qq) ; 
      mult_with_vector(mtv,mtinvv,eigpos[4],ul,sum1) ;
      matrizen_mult_normal_mult_u( normal,ul,pressure,u,sum1) ;
    }
    if(total == 3)
      matrizen_mult_normal_mult_u(normal,ul,pressure,u,sum1) ; 
   } 
          
   if(0 > cminusvalues_con(normal,ur,eigneg,u,&c,&qq,pressurenb,&total)) {
     fprintf(stderr,"sumhjl() fehler in cminusvalues_con()\n");
     fflush(stderr);
     return ;
   }
      
   if(total != 0){ 
     if(total == 1){ /* einer negativ */
       matrizen_fourth_vector(mtv,mtinvv,u, normal,ur[0],c,qq) ; 
       mult_with_vector(mtv,mtinvv,eigneg[4],ur,sum2) ;
     } 
     if(total == 2){ /* zwei negativ, einer positiv */
   	 matrizen_third_vector(mtv,mtinvv,u, normal,ur[0],c,qq) ; 
         mult_with_vector(mtv,mtinvv,eigneg[3],ur,sum2) ;
	 matrizen_mult_normal_mult_u( normal,ur,pressurenb,u,sum2) ;
     }
     if(total == 3)
       matrizen_mult_normal_mult_u( normal,ur,pressurenb,u,sum2) ; 
   } 

   for ( i = 0 ; i < 5 ; i ++)
     sum[i] += norm_len*(sum1[i]+sum2[i]) ; 

  return ;
}



@


2.2
log
@RCS Kommentarerg"anzung und eine statische Zeichenkette mit der RCS Id
eingebaut, damit aus den ausgecheckten Dateien und auch aus den Objects
die Revision herausgefunden werden kann, aus der sie entstanden sind.
@
text
@d3 4
a6 4
	// $Source$
	// $Revision$
	// $Name$
	// $State$
d15 1
a15 1
static volatile char RCSId_swflux_cc [] = "$Id$" ;
d291 1
a291 1
void FluxCalcSW :: sw_fluss(const double ul [5], const double ur [5],
@


2.1
log
@Basisversion des Hexaeder/Tetraedergitters, die dann aus der Revision 2.1
weiterentwickelt werden soll. Das Define USE_TETRAS soll dann verschwinden,
d.h. alle Elementtypen sind prizipiell immer verf"ugbar. Eventuell m"ussen
die Geometriesockelklassen aus der gitter_sti.h rausgenommen werden.
@
text
@d3 5
d14 2
@


1.100
log
@Branch Point der Zwischenversion 1.100 als Zweig f"ur Fehlerkorrekturen der
alten Programmversion vor dem Einbau der Tetraederelemente.
@
text
@@


1.100.1.1
log
@*** empty log message ***
@
text
@@


1.100.1.2
log
@RCS Messages in die Kopfzeile (Kommentar) reingenommen und eine statische
Zeichenkette mit der RCS ID in jede Datei eingef"ugt, damit man die
Versionen aus denen ein ausf"uhrbares Programm gebau wurde rausfinden kann.
@
text
@a2 5
	// $Source$
	// $Revision$
	// $Name$
	// $State$

a8 2

static volatile char RCSId_swflux_cc [] = "$Id$" ;
@


1.4
log
@*** empty log message ***
@
text
@@


1.3
log
@*** empty log message ***
@
text
@d1 1
a1 1
	// � monika wierse 1996
@


1.2
log
@*** empty log message ***
@
text
@d46 1
a46 1
int fluxCalcSW :: matrizen_third_vector(double mtv[5],double mtinvv[5],
d93 1
a93 1
int fluxCalcSW :: matrizen_fourth_vector(double mtv[5],double mtinvv[5],
d147 1
a147 1
int fluxCalcSW :: cplusvalues_con(const double normale[3], const double u[5],
d213 1
a213 1
int fluxCalcSW :: cminusvalues_con( const double normale[3], const double * u, 
d284 1
a284 1
void fluxCalcSW :: sw_fluss(const double ul [5], const double ur [5],
@


1.1
log
@Initial revision
@
text
@d158 1
a158 1
    fprintf(stderr, "Fehler in cplusvalues: Dichte %f \n",u[0]) ;
d225 3
a227 2
/*   printf("Fehler in cminusvalues: Dichte %f Druck %f\n",u[0],pressure) ;	*/
   return(-1) ; }
d315 4
a318 2
  if(0 < cplusvalues_con(normal,ul,eigpos,u,&c,&qq,pressure,&total)) {
     fprintf(stderr,"sumhjl() fehler in cminusvalues_con()"); fflush(stderr);
d338 1
@
