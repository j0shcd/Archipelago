// tools.cc // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#include <iostream>
#include <cmath>
#include "tools.h"
#include "constantes.h"
#include "graphic.h"

using namespace std;

bool inter_cercles (Cercle cercle1, Cercle cercle2,double dist) {
		
	double distance = norme(cercle1.centre, cercle2.centre);
		
	if (distance < (cercle1.r + cercle2.r + dist))
		return true;                                  

	return false;
} 

bool inter_seg_cer(Cercle cercle1, double x1, double x2, double y1, double y2,
				   double dist) {
	double a,b,c,discriminant,v1,v2;
	
	x1 = x1 - cercle1.centre.x ;											
	x2 = x2 - cercle1.centre.x ; 												
	y1 = y1 - cercle1.centre.y ;
	y2 = y2 - cercle1.centre.y ;
	
	a =pow(x1-x2,2)+ pow(y2-y1,2);
	b=2*(x1*(x2-x1) + y1*(y2 - y1));
	c = pow(x1,2) + pow(y1,2) - pow(cercle1.r+dist,2);
	
	discriminant =pow(b,2) - 4*a*c;
	
	if (discriminant <=0) {
		return false;
	}
	
	v1 = (-b + sqrt(discriminant))/(2*a);
	v2 = (-b + sqrt(discriminant))/(2*a);
	if ((0 < v1 and
	 v1 < 1) or (0 < v2 and v2 < 1))
		return true;
	
	return false;
}
	

double norme (Coord coord1, Coord coord2) {
	
	double x = (coord1.x - coord2.x);
	double y = (coord1.y - coord2.y);
	
	double distance = sqrt(pow(x, 2) + pow(y, 2));
	
	return distance;
}

double produit_scalaire (Coord coord1, Coord coord2) {
	
	double x = (coord1.x * coord2.x);
	double y = (coord1.y * coord2.y);
	
	return (x+y);
}

Coord vecteur (Coord coord1, Coord coord2) {
	
	Coord vecteur;
	vecteur.x = (coord1.x - coord2.x);
	vecteur.y = (coord1.y - coord2.y);
	
	return vecteur;
}

double norme_vecteur(Coord coord1){
	
	return sqrt(pow(coord1.x, 2) + pow(coord1.y, 2));
}

// graphic

void change_couleur(int enum_couleur, double& R, double& G, double& B) {

	switch (enum_couleur) {
		case RED:
			R = 0.93; 
			G = 0.; 
			B = 0.;
			break;
				
		case GREEN:
			R = 0.;
			G = 0.93;
			B = 0.;
			break;
		

		case BLACK:
		default :
			R = 0.;
			G = 0.;
			B = 0.;
			break;
	}
}

void dessine_cercle_tools(double x,double y, double r,int enum_couleur){ 
	
	double R(0),G(0),B(0);
	change_couleur(enum_couleur,R,G,B);
	dessine_cercle(x,y,r,{R,G,B});
	
}

void dessine_segment_tools(double x1,double y1, double x2, double y2,int enum_couleur){
	
	double R(0),G(0),B(0);
	change_couleur(enum_couleur,R,G,B);
	dessine_segment(x1,y1,x2,y2,{R,G,B});
	
}

void dessine_rectangle(Coord point,double l,double L, int enum_couleur){
	
	double x = point.x;
	double y = point.y;
	dessine_segment_tools(x, y, x+l, y, enum_couleur);
	dessine_segment_tools(x+l, y, x+l, y-L, enum_couleur);
	dessine_segment_tools(x+l, y-L, x, y-L, enum_couleur);
	dessine_segment_tools(x, y-L, x, y, enum_couleur);
	
}

bool point_dans_cercle(Coord point, Cercle cercle) {
			   
	double dist = norme(point,{cercle.centre.x,cercle.centre.y});
	if (dist <= cercle.r) {
		return true;
	}
	return false;
}
