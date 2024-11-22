// graphic.cc // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#include <cairomm/context.h>
#include <iostream>
#include "graphic_gui.h"

using namespace std;

static const Cairo::RefPtr<Cairo::Context>*ptcr (nullptr);

void set_context(const Cairo::RefPtr<Cairo::Context>& cr){
	static bool init(false);
	if (!init) {
		ptcr = &cr;
		init = true;
	}
}

void dessine_segment(double x1,double y1, double x2, double y2, Color couleur){
	double R,G,B;
	R = couleur.R;
	G = couleur.G;
	B = couleur.B;
	(*ptcr) -> set_line_width(5.0);
	(*ptcr) -> set_source_rgb(R,G,B);
	(*ptcr) -> move_to(x1,y1);
	(*ptcr) -> line_to(x2,y2);
	(*ptcr) -> stroke();
}

void dessine_cercle (double x,double y, double r, Color couleur){
	const double PI(3.14159);
	double R,G,B;
	R = couleur.R;
	G = couleur.G;
	B = couleur.B;
	(*ptcr) -> set_line_width(5.0);
	(*ptcr) -> set_source_rgb(1.,1.,1.);
	(*ptcr) -> arc(x,y,r,0,2*PI);
	(*ptcr) -> fill();
	(*ptcr) -> set_source_rgb(R,G,B);
	(*ptcr) -> arc(x,y,r,0,2*PI);
	(*ptcr) -> stroke();
}
