// noeud.h // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#ifndef NOEUD_HEADER_H
#define NOEUD_HEADER_H

#include <vector>
#include <fstream>
#include <string>

#include "tools.h"
#include "constantes.h"

class Noeud {
	
public :
	
	Noeud(nbPos uid = 0, double x = 0, double y = 0, nbPos nbp = 0, char type = 'A', 
	bool in = true, double access = infinite_time, nbPos parent = no_link, 
	int couleur = BLACK);
	nbPos get_uid() const;
	nbPos get_nbp() const;
	double get_x() const;
	double get_y( )const;
	Coord get_position() const;
	std::vector<nbPos> get_links () const;
	char get_type() const;
	bool get_in() const;
	double get_access() const;
	nbPos get_parent() const;
	int get_couleur() const;
	void set_uid (nbPos uid_set);
	void set_nbp (nbPos nbp_set);
	void set_position(double position_set_x, double position_set_y);
	void set_link (nbPos uid);
	void set_type(char type_set);
	void set_in(bool new_in);
	void set_access(double new_access);
	void set_parent(nbPos new_parent);
	void set_couleur(int new_couleur);
	void remove_link (nbPos uid);
	
private: 
	
	nbPos uid;
	Coord position;
	nbPos nbp;
	std::vector<nbPos> links;
	char type;
	bool in;
	double access;
	nbPos parent;
	int couleur;
};

struct Lien {
	Coord coord1;
	nbPos uid1;
	Coord coord2;
	nbPos uid2;
	int couleur;
}; 

bool create_node (nbPos uid, double x, double y, nbPos nbp, char type, 
				  bool created_by_click);
bool create_link (nbPos uid1, nbPos uid2, bool created_by_click);
void change_color_with_uid(nbPos uid, int couleur);
bool test_node (nbPos uid, double x, double y, nbPos nbp, double dist);
bool test_link (nbPos uid1, nbPos uid2, double dist);

void add_logement (nbPos const& uid, double const& x, double const& y, nbPos const& 
nbp, char type);

void add_transport (nbPos const& uid, double const& x, double const& y, nbPos 
const& nbp, char type);

void add_production (nbPos const& uid, double const& x, double const& y, nbPos 
const& nbp, char type);

void add_link(double x1, double y1, nbPos uid1, double x2, double y2, nbPos uid2);
bool success_node (nbPos const& uid, double const& x, double const& y, 
				   nbPos const& nbp, double dist);
bool success_link_the_1st (nbPos uid1,nbPos uid2);
bool success_link_the_2nd (nbPos uid1, nbPos uid2, double dist);
bool success_link_the_3rd (nbPos uid1, nbPos uid2);
nbPos new_uid();
void supprime_liens_noeuds();

void sauvegarde_noeuds(std::ofstream& myfile);
void sauvegarde_liens(std::ofstream& filename);

void dessine_quartier();
void dessine_noeud(nbPos nbp, double x, double y, int couleur,char type);

double calcul_ENJ();

double calcul_CI();
void tests_CI (nbPos const& rank, Coord& position, double& capacite);

double calcul_MTA();
nbPos Dijkstra(nbPos const& d, char const& type);
bool compare_access (nbPos const& A, nbPos const& B);
nbPos find_min_access(std::vector<nbPos>& TA);
double compute_access(nbPos const& n, nbPos const& rank_node);
bool TA_empty();

Noeud find_node_with_uid(nbPos uid);
nbPos clicked_node(Coord click);
void move_node(double x, double y, nbPos uid);
bool tests_move(double x, double y, nbPos indice_courant);
void update_links(nbPos indice_courant);

void clear_links(nbPos uid, nbPos indice);
void delete_link(nbPos& indice1, nbPos uid1, nbPos& indice2, nbPos uid2);
void delete_node(nbPos uid);

void add_or_remove_link (nbPos uid_courant, nbPos new_connection);
bool link_does_not_exist(nbPos uid1, nbPos uid2);

void shortest_path(nbPos noeud_courant_uid);
void color_path (nbPos courant, nbPos parent);
void set_couleur_lien(nbPos uid1, nbPos uid2, int couleur);
void reset_color_nodes(nbPos uid);
void reset_color_links();

void size_node(nbPos uid, double x_init, double y_init, double x_final, 
			   double y_final);
bool success_node2(nbPos const& uid, double const& x, double const& y, 
				  nbPos nbp, double dist, nbPos const& rank_node);

#endif // NOEUD_HEADER_H
