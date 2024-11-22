// ville.cc // Joshua Cohen-Dumani // Gabriel Le Royer // v.11 // architecture b1

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "ville.h"
#include "constantes.h"
#include "noeud.h"
#include "error.h"

using namespace std;

namespace {
	Ville ville;
	static nbPos noeud_courant_uid(no_link);
}

enum {NBLOG, LOGEMENT, NBTRA, TRANSPORT, NBPRO, PRODUCTION, 
	  NBLIN, LINKS, END}; //etats lecture

Ville::Ville() : ENJ(0), CI(0), MTA(0){}

double Ville::get_ENJ()const {
	return ville.ENJ;
}

double Ville::get_CI()const {
	return ville.CI;
}

double Ville::get_MTA()const {
	return ville.MTA;
}

void Ville::set_ENJ(double ENJ_set){
	ville.ENJ = ENJ_set;
}

void Ville::set_CI(double CI_set){
	ville.CI = CI_set;
}

void Ville::set_MTA(double MTA_set){
	ville.MTA = MTA_set;
}

void Ville::set_filename(string filename_set){
	filename = filename_set;
}

void lecture(string filename) {
	ville.set_filename(filename);
	string line;
	ifstream fichier(filename);
	int etat(NBLOG);
	bool state(true);
	if (!(fichier.fail())) {
		while (getline(fichier >> ws, line)) {
			if (line[0]=='#' or line[0]=='\n' or line[0]=='\r') {
				continue;
			}
			decodage_ligne(line,etat,state);
		};	
		if (state) {
			cout << error::success();
		}
	}
	etat = NBLOG;
}

void save(string filename) {
	
	ville.set_filename(filename);
	ofstream myfile;
	myfile.open(filename);
	
	if (myfile.is_open()) {
		sauvegarde_noeuds(myfile);
		sauvegarde_liens(myfile);
	}
	else cout << "unable to open file" << endl;
	
	myfile.close();
	
}

void decodage_ligne(string line, int& etat, bool& state){
	
	stringstream data(line);
	static int i(0);
	static int total(0);
	nbPos uid(0), uid1(0);
	double x(0), y(0);
	nbPos nbp(0);
	char type;
	
	switch (etat) {
		case NBLOG :
			if (!(data >> total)) {
				cout << "erreur de lecture NBLOG" << endl; exit(0); }
			else {i = 0;}
			if (total==0) {etat = NBTRA; }
			else {etat = LOGEMENT;} break;

		case LOGEMENT :
			if(!(data >> uid >> x >> y >> nbp)) {
				cout << "erreur de lecture LOGEMENT" << endl; exit(0); }
			i++;
			if (i == total) {etat = NBTRA; }
			type = 'L';
			state = create_node(uid, x, y, nbp, type, false); break;			

		case NBTRA :
			if (!(data >> total)) {
				cout << "erreur de lecture NBTRA" << endl; exit(0); }
			else {i = 0;}
			if (total == 0) {etat = NBPRO; }
			else {etat = TRANSPORT;} break;

		case TRANSPORT :
			if(!(data >> uid >> x >> y >> nbp)) {
				cout << "erreur de lecture TRANSPORT"<< endl; exit(0); }
			else {++i;}
			if (i == total) {etat = NBPRO; }
			type = 'T';
			if (state){state = create_node(uid, x, y, nbp, type, false);} break;

		case NBPRO :
			if (!(data >> total)) {
				cout << "erreur de lecture NBPRO" << endl; exit(0); }
			else {i = 0;}
			if (total == 0) {etat = NBLIN; }
			else {etat = PRODUCTION;} break;

		case PRODUCTION :
			if(!(data >> uid >> x >> y >> nbp)) {
				cout << "erreur de lecture PRODUCTION" << endl; exit(0); }
			else {++i;}
			if (i == total) {etat = NBLIN; }
			type = 'P';
			if (state){state = create_node(uid, x, y, nbp, type, false); }break;
		
		case NBLIN :
			if (!(data >> total)) {
				cout << "erreur de lecture NBLIN" << endl; exit(0); }
			else {i = 0;}
			if (total == 0) {etat = NBLIN; }
			else {etat = LINKS;} break;
		
		case LINKS :
			if (!(data >> uid >> uid1)) {
				cout << "erreur de lecture LINKS" << endl; exit(0); }
			else {++i;}
			if (i == total) {etat = END; }
			if (state) {state=create_link(uid, uid1, false);} break;				

		case END :
			cout << "erreur format" << endl; exit(0);
		
		default : 
			cout << "erreur lecture etat" << endl; exit(0);
		}
}

void supprime_vector() {
	
	supprime_liens_noeuds();
}

void dessine_ville() {
	
	dessine_quartier();
}

bool create_noeud(nbPos uid, double x, double y,nbPos nbp, char type, 
				  int& state_node) {
	Coord click = {x, y};
	if (clicked_node(click) == no_link and state_node == 0) {
		bool created_by_click (true);
		return create_node(uid,x,y,nbp,type, created_by_click);
	}
	return false;
}

nbPos create_uid(){
	return new_uid();
}

void update_criteres() {
	
	double new_ENJ(calcul_ENJ());
	ville.set_ENJ(new_ENJ);
	
	double new_CI(calcul_CI());
	ville.set_CI(new_CI);
	
	double new_MTA(calcul_MTA());
	ville.set_MTA(new_MTA);
}


CriteresEval get_criteres() {
	
	return {ville.get_ENJ(), ville.get_CI(), ville.get_MTA()};
}

void find_noeud_courant(double x, double y, int& state_node, bool sp) {
	Coord click = {x, y};
	if (clicked_node(click) != no_link) {
		if (noeud_courant_uid != no_link) {
			reset_colors();
		}
		noeud_courant_uid = clicked_node(click);
		change_color_with_uid(noeud_courant_uid, RED);
		state_node = 1;
		if (sp) {
			shortest_path(noeud_courant_uid);
		}
	}
	else {
		reset_colors();
	}
}

void deplace_noeud (double x, double y) {
	if (noeud_courant_uid != no_link) 
		move_node(x, y, noeud_courant_uid);
}

void delete_noeud(double x, double y) {
	Coord click = {x, y};
	if (clicked_node(click) == noeud_courant_uid and noeud_courant_uid != no_link) {
		delete_node(noeud_courant_uid);
		noeud_courant_uid = no_link;
	}
}

void size_noeud(double x_init, double y_init, double x_final, double y_final){
	if(noeud_courant_uid != no_link) {
		size_node(noeud_courant_uid,x_init,y_init,x_final,y_final);
	}
}

bool noeud_actif() {
	if (noeud_courant_uid == no_link) 
		return false;
	
	return true;
}

void edit_link(double x, double y, bool sp_active) {
	Coord click = {x, y};
	nbPos new_connection (clicked_node(click));
	if (new_connection != no_link) {
		add_or_remove_link(noeud_courant_uid, new_connection);
		if (sp_active) {
			reset_colors();
			shortest_path(noeud_courant_uid);
		}
	}
}

void unselect(int& state_node){
	change_color_with_uid(no_link, BLACK);
	noeud_courant_uid = no_link;
	state_node = 0;
}

void direct_shortest_path() {
	if (noeud_courant_uid != no_link) 
		shortest_path(noeud_courant_uid);
}

void reset_colors() {
	reset_color_nodes(noeud_courant_uid);
	reset_color_links();
}
