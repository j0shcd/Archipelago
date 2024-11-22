// graphic_gui.h // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#ifndef GRAPHIC_GUI_HEADER_H
#define GRAPHIC_GUI_HEADER_H

#include <cairomm/context.h>
#include "graphic.h"

void set_context(const Cairo::RefPtr<Cairo::Context>& cr);
double get_ptcr();

#endif //GRAPHIC_GUI_HEADER_H
