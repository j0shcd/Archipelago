// graphic.h // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#ifndef GRAPHIC_HEADER_H
#define GRAPHIC_HEADER_H

struct Color{
	double R;
	double G;
	double B;
};

void dessine_segment(double x1,double y1, double x2, double y2, Color couleur);
void dessine_cercle (double x,double y, double r, Color couleur);

#endif //GRAPHIC_HEADER_H
