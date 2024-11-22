// ville.h // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#ifndef VILLE_HEADER_H
#define VILLE_HEADER_H

#include <vector>
#include <string>

#include "noeud.h"

struct CriteresEval {
	double ENJ;
	double CI;
	double MTA;
};

class Ville {
public:
	Ville();
	double get_ENJ()const;
	double get_CI()const;
	double get_MTA()const;
	void set_ENJ(double ENJ_set);
	void set_CI(double CI_set);
	void set_MTA(double MTA_set);
	void set_filename(std::string filename_set);

private:
	std::string filename;
	double ENJ;
	double CI;
	double MTA;
};

void lecture(std::string filename);
void decodage_ligne(std::string line, int& etat, bool& state);
void save (std::string filename);
void supprime_vector();
void dessine_ville();
nbPos create_uid();
bool create_noeud(nbPos uid, double x, double y,nbPos nbp, char type, int& state_node);
void update_criteres();
CriteresEval get_criteres();
void find_noeud_courant(double x, double y, int& state_node, bool sp);
void unselect(int& state_node);
void deplace_noeud (double x, double y);
void delete_noeud(double x, double y);
void size_noeud(double x_init, double y_init, double x_final, double y_final);
bool noeud_actif();
void edit_link(double x, double y, bool sp_active);
void direct_shortest_path();
void reset_colors();

#endif //VILLE_HEADER_H
