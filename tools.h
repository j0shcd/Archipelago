// tools.h // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

struct Coord {
	double x;
	double y;
};
	
struct Cercle {
	Coord centre;
	double r;
};
	
struct Segment {
	Coord point;
	Coord vecteur;
};
	
enum enum_couleur{ BLACK, RED, GREEN, WHITE};

bool inter_cercles (Cercle cercle1, Cercle cercle2, double dist);
double norme (Coord coord1, Coord coord2);
bool inter_seg_cer (Cercle cercle1,double x1, double x2, double y1, double y2,
					double dist_min);
double produit_scalaire (Coord coord1, Coord coord2);
Coord vecteur (Coord coord1, Coord coord2);
double norme_vecteur(Coord coord1);

void change_couleur(int couleur, double& R, double& G, double& B);
void dessine_cercle_tools(double x,double y, double r,int enum_couleur);
void dessine_segment_tools(double x1,double y1, double x2, double y2,int enum_couleur);
void dessine_rectangle(Coord point,double l,double L, int enum_couleur);

bool point_dans_cercle(Coord point, Cercle cercle);
					   
#endif // TOOLS_HEADER_H
