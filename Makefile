# Definitions de macros
OUT = projet
CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = projet.cc graphic.cc gui.cc ville.cc noeud.cc tools.cc error.cc
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
OFILES = $(CXXFILES:.cc=.o)

# Definition de la premiere regle

all: $(OUT)

projet.o: projet.cc gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

graphic.o: graphic.cc graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

gui.o: gui.cc gui.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)


# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile



clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ main

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
projet.o: projet.cc gui.h graphic_gui.h graphic.h ville.h noeud.h tools.h \
 constantes.h
graphic.o: graphic.cc graphic_gui.h graphic.h
gui.o: gui.cc gui.h graphic_gui.h graphic.h ville.h noeud.h tools.h \
 constantes.h
ville.o: ville.cc ville.h noeud.h tools.h constantes.h error.h
noeud.o: noeud.cc error.h noeud.h tools.h constantes.h
tools.o: tools.cc tools.h constantes.h graphic.h
error.o: error.cc error.h constantes.h
